#include <gtk/gtk.h>
#include <stdio.h>

char tabla[3][3];
int scor_x = 0;
int scor_o = 0;
int joc_terminat = 0;
char jucator_curent = 'X';
int putere_x = 1;
int putere_o = 1;
int putere_x_activa = 0;
int putere_o_activa = 0;

GtkWidget *butoane[3][3];
GtkWidget *label_status;
GtkWidget *label_scor;
GtkWidget *buton_putere_x;
GtkWidget *buton_putere_o;
GtkWindow *fereastra;

void actualizeaza_scor() {
    char text[100];
    sprintf(text, "Scor  X: %d    O: %d", scor_x, scor_o);
    gtk_label_set_text(GTK_LABEL(label_scor), text);
}

void reseteaza_scor() {
    scor_x = 0;
    scor_o = 0;
    actualizeaza_scor();
}

void seteaza_status(char *text) {
    gtk_label_set_text(GTK_LABEL(label_status), text);
}

void actualizeaza_putere() {
    if (putere_x == 1) {
        if (putere_x_activa == 1)
            gtk_button_set_label(GTK_BUTTON(buton_putere_x), "Putere X (ACTIVA)");
        else
            gtk_button_set_label(GTK_BUTTON(buton_putere_x), "Putere X");
        gtk_widget_set_sensitive(buton_putere_x, jucator_curent == 'X' && joc_terminat == 0);
    } else {
        gtk_button_set_label(GTK_BUTTON(buton_putere_x), "Putere X (folosita)");
        gtk_widget_set_sensitive(buton_putere_x, FALSE);
    }

    if (putere_o == 1) {
        if (putere_o_activa == 1)
            gtk_button_set_label(GTK_BUTTON(buton_putere_o), "Putere O (ACTIVA)");
        else
            gtk_button_set_label(GTK_BUTTON(buton_putere_o), "Putere O");
        gtk_widget_set_sensitive(buton_putere_o, jucator_curent == 'O' && joc_terminat == 0);
    } else {
        gtk_button_set_label(GTK_BUTTON(buton_putere_o), "Putere O (folosita)");
        gtk_widget_set_sensitive(buton_putere_o, FALSE);
    }
}

int verifica_castig(char simbol) {
    int i;

    for (i = 0; i < 3; i++) {
        if (tabla[i][0] == simbol && tabla[i][1] == simbol && tabla[i][2] == simbol)
            return 1;
        if (tabla[0][i] == simbol && tabla[1][i] == simbol && tabla[2][i] == simbol)
            return 1;
    }

    if (tabla[0][0] == simbol && tabla[1][1] == simbol && tabla[2][2] == simbol)
        return 1;
    if (tabla[0][2] == simbol && tabla[1][1] == simbol && tabla[2][0] == simbol)
        return 1;

    return 0;
}

int tabla_plina() {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (tabla[i][j] == ' ')
                return 0;
    return 1;
}

void reseteaza_tabla() {
    int i, j;

    joc_terminat = 0;
    jucator_curent = 'X';
    putere_x = 1;
    putere_o = 1;
    putere_x_activa = 0;
    putere_o_activa = 0;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            tabla[i][j] = ' ';
            gtk_button_set_label(GTK_BUTTON(butoane[i][j]), " ");
            gtk_widget_set_sensitive(butoane[i][j], TRUE);
        }
    }

    seteaza_status("Tura jucatorului X");
    actualizeaza_putere();
}

void schimba_jucatorul() {
    putere_x_activa = 0;
    putere_o_activa = 0;

    if (jucator_curent == 'X') {
        jucator_curent = 'O';
        seteaza_status("Tura jucatorului O");
    } else {
        jucator_curent = 'X';
        seteaza_status("Tura jucatorului X");
    }

    actualizeaza_putere();
}

void afiseaza_dialog(char *mesaj) {
    GtkWidget *dialog = gtk_message_dialog_new(
        fereastra,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", mesaj
    );
    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(dialog), "Apasa OK pentru o runda noua."
    );
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void apasa_casuta(GtkButton *button, gpointer data) {
    int index = GPOINTER_TO_INT(data);
    int r = index / 3;
    int c = index % 3;
    int putere_activa;
    char mesaj[64];
    char text[2];

    if (joc_terminat == 1)
        return;

    if (jucator_curent == 'X')
        putere_activa = putere_x_activa;
    else
        putere_activa = putere_o_activa;

    if (tabla[r][c] != ' ' && putere_activa == 0) {
        seteaza_status("Casuta ocupata! Activeaza puterea sau alege alta.");
        return;
    }

    if (putere_activa == 1) {
        if (jucator_curent == 'X') {
            putere_x = 0;
            putere_x_activa = 0;
        } else {
            putere_o = 0;
            putere_o_activa = 0;
        }
    }

    tabla[r][c] = jucator_curent;
    text[0] = jucator_curent;
    text[1] = '\0';
    gtk_button_set_label(GTK_BUTTON(butoane[r][c]), text);

    if (verifica_castig(jucator_curent)) {
        if (jucator_curent == 'X')
            scor_x++;
        else
            scor_o++;

        actualizeaza_scor();
        sprintf(mesaj, "Jucatorul %c a castigat!", jucator_curent);

        joc_terminat = 1;
        seteaza_status(mesaj);
        actualizeaza_putere();
        afiseaza_dialog(mesaj);
        reseteaza_tabla();
        return;
    }

    if (tabla_plina()) {
        joc_terminat = 1;
        seteaza_status("Remiza!");
        actualizeaza_putere();
        afiseaza_dialog("Remiza!");
        reseteaza_tabla();
        return;
    }

    schimba_jucatorul();
}

void apasa_putere_x(GtkButton *button, gpointer data) {
    if (joc_terminat == 1) return;
    if (jucator_curent != 'X') return;
    if (putere_x == 0) return;

    if (putere_x_activa == 0)
        putere_x_activa = 1;
    else
        putere_x_activa = 0;

    if (putere_x_activa == 1)
        seteaza_status("Puterea lui X este activa");
    else
        seteaza_status("Tura jucatorului X");

    actualizeaza_putere();
}

void apasa_putere_o(GtkButton *button, gpointer data) {
    if (joc_terminat == 1) return;
    if (jucator_curent != 'O') return;
    if (putere_o == 0) return;

    if (putere_o_activa == 0)
        putere_o_activa = 1;
    else
        putere_o_activa = 0;

    if (putere_o_activa == 1)
        seteaza_status("Puterea lui O este activa");
    else
        seteaza_status("Tura jucatorului O");

    actualizeaza_putere();
}

void apasa_reset_joc(GtkButton *button, gpointer data) {
    reseteaza_tabla();
}

void apasa_reset_scor(GtkButton *button, gpointer data) {
    reseteaza_scor();
    reseteaza_tabla();
    seteaza_status("Scorul a fost resetat. Tura jucatorului X");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box_principal;
    GtkWidget *titlu;
    GtkWidget *grid;
    GtkWidget *box_puteri;
    GtkWidget *box_reset;
    GtkWidget *buton_reset_joc;
    GtkWidget *buton_reset_scor;
    int i, j;

    window = gtk_application_window_new(app);
    fereastra = GTK_WINDOW(window);
    gtk_window_set_title(GTK_WINDOW(window), "Tic-Tac-Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 420, 520);
    gtk_container_set_border_width(GTK_CONTAINER(window), 12);

    box_principal = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box_principal);

    titlu = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(titlu), "<span size='x-large' weight='bold'>Tic-Tac-Toe</span>");
    gtk_box_pack_start(GTK_BOX(box_principal), titlu, FALSE, FALSE, 0);

    label_scor = gtk_label_new("Scor  X: 0    O: 0");
    gtk_box_pack_start(GTK_BOX(box_principal), label_scor, FALSE, FALSE, 0);

    label_status = gtk_label_new("Tura jucatorului X");
    gtk_box_pack_start(GTK_BOX(box_principal), label_status, FALSE, FALSE, 0);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
    gtk_box_pack_start(GTK_BOX(box_principal), grid, TRUE, TRUE, 0);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            GtkWidget *btn = gtk_button_new_with_label(" ");
            gtk_widget_set_size_request(btn, 110, 90);
            g_signal_connect(btn, "clicked", G_CALLBACK(apasa_casuta), GINT_TO_POINTER(i * 3 + j));
            gtk_grid_attach(GTK_GRID(grid), btn, j, i, 1, 1);
            butoane[i][j] = btn;
        }
    }

    box_puteri = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(box_principal), box_puteri, FALSE, FALSE, 0);

    buton_putere_x = gtk_button_new_with_label("Putere X");
    g_signal_connect(buton_putere_x, "clicked", G_CALLBACK(apasa_putere_x), NULL);
    gtk_box_pack_start(GTK_BOX(box_puteri), buton_putere_x, TRUE, TRUE, 0);

    buton_putere_o = gtk_button_new_with_label("Putere O");
    g_signal_connect(buton_putere_o, "clicked", G_CALLBACK(apasa_putere_o), NULL);
    gtk_box_pack_start(GTK_BOX(box_puteri), buton_putere_o, TRUE, TRUE, 0);

    box_reset = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(box_principal), box_reset, FALSE, FALSE, 0);

    buton_reset_joc = gtk_button_new_with_label("Reset joc");
    g_signal_connect(buton_reset_joc, "clicked", G_CALLBACK(apasa_reset_joc), NULL);
    gtk_box_pack_start(GTK_BOX(box_reset), buton_reset_joc, TRUE, TRUE, 0);

    buton_reset_scor = gtk_button_new_with_label("Reset scor");
    g_signal_connect(buton_reset_scor, "clicked", G_CALLBACK(apasa_reset_scor), NULL);
    gtk_box_pack_start(GTK_BOX(box_reset), buton_reset_scor, TRUE, TRUE, 0);

    reseteaza_tabla();
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("ro.alex.tictactoe", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
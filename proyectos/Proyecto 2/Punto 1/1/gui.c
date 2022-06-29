#include <gtk/gtk.h>
#include "gui.h"
#include "logica.h"

struct elementos_gui {
	GtkToggleButton * boton_dados[DADOS];
	GtkButton * boton_tirar;
	GtkButton * boton_terminar_turno;
	GtkButton * boton_dejar_de_tirar;

	GtkLabel * label_estado;

	/* el +1 es para almacenar el puntaje total */
	GtkLabel * local_labels_puntaje[CANTIDAD_CATEGORIAS+1];
	GtkLabel * remoto_labels_puntaje[CANTIDAD_CATEGORIAS+1];

	/* de glade a código GTK */
	GtkBuilder * constructor;

	GtkBox * panel_eleccion_categorias;
	GtkBox * panel_dados;

	GSList * lista_botones_categorias;

	GtkWidget * ventana;
};

static struct elementos_gui elementos_gui;
static int tiros_usados = 0, hay_que_tachar = 1;
static int dados_gui[DADOS];
static char categorias_tentativas[CANTIDAD_CATEGORIAS][LONGITUD_CADENA];
static char categorias_a_tachar[CANTIDAD_CATEGORIAS][LONGITUD_CADENA];

struct a_actualizar {
	int indice;
	int puntaje;
} a_actualizar;

void descartar_dados()
{
	for (int i = 0; i < DADOS; ++i) {
		if (gtk_toggle_button_get_active(elementos_gui.boton_dados[i])) {
			// descartar estos
			dados_gui[i] = descartar(i);
			gtk_toggle_button_set_active(elementos_gui.boton_dados[i], FALSE);
		} else {
			// conservar estos
			gtk_widget_set_sensitive((GtkWidget *) elementos_gui.boton_dados[i], FALSE);
		}
	}
}

void habilitar_botones_dados(gboolean estado)
{
	// si fue el primer turno, entonces hay que habilitar los botones de los datos
	for (int i = 0; i < DADOS; ++i) {
		gtk_widget_set_sensitive((GtkWidget *) elementos_gui.boton_dados[i], estado);
	}
}

void actualizar_numeros()
{
	char texto[LONGITUD_CADENA];
	GtkWidget * imagen;
	for (int i = 0; i < DADOS; ++i) {
		snprintf(texto, LONGITUD_CADENA-1, "fotos/40pixeles/%i.png", dados_gui[i]);
		imagen = gtk_image_new_from_file(texto);
		gtk_button_set_image((GtkButton *) elementos_gui.boton_dados[i], NULL);
		gtk_button_set_image((GtkButton *) elementos_gui.boton_dados[i], imagen);
	}
}

void actualizar_tachadas()
{
	obtener_lista_tachar(categorias_a_tachar);
	GSList * movil;
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) {
		if (strcmp(categorias_a_tachar[i], " ")) {
			movil = g_slist_nth(elementos_gui.lista_botones_categorias, i);
			gtk_button_set_label((GtkButton *) movil->data, categorias_a_tachar[i]);
			gtk_toggle_button_set_active(movil->data, TRUE);
			gtk_widget_show((GtkWidget *) movil->data);
		}
	}
}

void actualizar_categorias()
{
	obtener_categorias(categorias_tentativas);
	GSList * movil;
	hay_que_tachar = 1;
	char texto[LONGITUD_CADENA];
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) {
		if (strcmp(categorias_tentativas[i], " ")) {
			// se obtuvo puntaje en esta categoría
			movil = g_slist_nth(elementos_gui.lista_botones_categorias, i);
			gtk_button_set_label((GtkButton *) movil->data, categorias_tentativas[i]);
			gtk_toggle_button_set_active(movil->data, TRUE);
			gtk_widget_show((GtkWidget *) movil->data);
			hay_que_tachar = 0;
		}
	}
	if (hay_que_tachar) {
		actualizar_tachadas();
		snprintf(texto, LONGITUD_CADENA-1, "%s", "Elija una categoría a tachar.");
	} else {
		snprintf(texto, LONGITUD_CADENA-1, "%s", "Elija una categoría a anotar.");
	}
	gtk_widget_show((GtkWidget *) elementos_gui.panel_eleccion_categorias);
	gtk_label_set_text(elementos_gui.label_estado, texto);
}

void dejar_de_tirar()
{
	tiros_usados = 0;
	if (tiros_usados == 1) {
		// TODO: servido
	}
	descartar_dados();
	/* tirar_dados(dados_gui); */
	habilitar_botones_dados(FALSE);
	gtk_widget_set_sensitive((GtkWidget *) elementos_gui.boton_dejar_de_tirar, FALSE);
	gtk_widget_set_sensitive((GtkWidget *) elementos_gui.boton_tirar, FALSE);
	actualizar_categorias();
}

void tirar_dados_gui()
{
	tiros_usados++;
	/* tirar_dados(dados_gui); */

	if (tiros_usados == 1) {
		tirar_dados(dados_gui);
		habilitar_botones_dados(TRUE);
		gtk_widget_set_sensitive((GtkWidget *) elementos_gui.boton_dejar_de_tirar, TRUE);
	} else {
		descartar_dados();
	}

	actualizar_numeros();

	char texto[LONGITUD_CADENA];

	if (tiros_usados == TIROS) {
		dejar_de_tirar();
	} else {
		if (TIROS-tiros_usados == 1)
			snprintf(texto, LONGITUD_CADENA-1, "Le queda %i tiro. Elija los dados a descartar.", TIROS-tiros_usados);
		else
			snprintf(texto, LONGITUD_CADENA-1, "Le quedan %i tiros. Elija los dados a descartar.", TIROS-tiros_usados);
		gtk_label_set_text(elementos_gui.label_estado, texto);
	}
}

gboolean actualizar_dados_protegido(void * arg)
{
	int * dados_remotos = (int *) arg;
	char texto[LONGITUD_CADENA];
	GtkWidget * imagen;
	for (int i = 0; i < DADOS; ++i) {
		snprintf(texto, LONGITUD_CADENA-1, "fotos/40pixeles/%i.png", dados_remotos[i]);
		imagen = gtk_image_new_from_file(texto);
		gtk_button_set_image((GtkButton *) elementos_gui.boton_dados[i], NULL);
		gtk_button_set_image((GtkButton *) elementos_gui.boton_dados[i], imagen);
	}
	return FALSE;
}

void * actualizar_dados(int poner_dados_en_cero, int dados_remotos[DADOS])
{
	static int dados_remotos_aux[DADOS];
	if (!poner_dados_en_cero) {
		for (int i = 0; i < DADOS; ++i) dados_remotos_aux[i] = dados_remotos[i];
	}
	gdk_threads_add_idle(actualizar_dados_protegido, dados_remotos_aux);
	return NULL;
}

void efectuar_puntaje()
{
	GSList * movil;
	int encontre = 0, i = 0;
	while (!encontre && i < CANTIDAD_CATEGORIAS) {
		movil = g_slist_nth(elementos_gui.lista_botones_categorias, i);
		if (gtk_toggle_button_get_active(movil->data)) {
			// eligió este
			if (hay_que_tachar) {
				strncat(categorias_a_tachar[i], ": -", LONGITUD_CADENA);
				gtk_label_set_text(elementos_gui.local_labels_puntaje[i], categorias_a_tachar[i]);
				decidir_tachar(i);
			} else {
				gtk_label_set_text(elementos_gui.local_labels_puntaje[i], gtk_button_get_label(movil->data));
				decidir_puntaje(i);
			}
			encontre = 1;
		}
		i++;
	}
}

void ocultar_botones_puntaje_tentativo()
{
	for (int i = 0; i < CANTIDAD_CATEGORIAS; ++i) {
		gtk_widget_hide(g_slist_nth(elementos_gui.lista_botones_categorias, i)->data);
	}
}

void terminar_turno_gui()
{
	efectuar_puntaje();
	gtk_widget_hide((GtkWidget *) elementos_gui.panel_eleccion_categorias);
	/* gtk_widget_set_sensitive((GtkWidget *) elementos_gui.boton_tirar, TRUE); */
	/* gtk_widget_set_sensitive((GtkWidget *) elementos_gui.boton_dejar_de_tirar, FALSE); */
	ocultar_botones_puntaje_tentativo();
	if (partida_terminada()) {
		char texto[LONGITUD_CADENA];
		snprintf(texto, LONGITUD_CADENA,
			"%s %i",
			gtk_label_get_text(elementos_gui.local_labels_puntaje[CANTIDAD_CATEGORIAS]),
			puntaje_total()
		);
		gtk_label_set_text(elementos_gui.local_labels_puntaje[CANTIDAD_CATEGORIAS], texto);
		gtk_widget_hide((GtkWidget *) elementos_gui.panel_dados);
	} else {
		gtk_label_set_text(elementos_gui.label_estado, "Espere a que su oponente termine su turno.");
	}
}

void obtener_dados() {
	/* registrar botones */
	elementos_gui.boton_dados[0] = (GtkToggleButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_dado_1"));
	elementos_gui.boton_dados[1] = (GtkToggleButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_dado_2"));
	elementos_gui.boton_dados[2] = (GtkToggleButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_dado_3"));
	elementos_gui.boton_dados[3] = (GtkToggleButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_dado_4"));
	elementos_gui.boton_dados[4] = (GtkToggleButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_dado_5"));

	for (int i = 0; i < DADOS; ++i) gtk_toggle_button_set_active(elementos_gui.boton_dados[i], FALSE);
}

void obtener_puntajes()
{
	elementos_gui.local_labels_puntaje[0] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_1"));
	elementos_gui.local_labels_puntaje[1] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_2"));
	elementos_gui.local_labels_puntaje[2] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_3"));
	elementos_gui.local_labels_puntaje[3] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_4"));
	elementos_gui.local_labels_puntaje[4] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_5"));
	elementos_gui.local_labels_puntaje[5] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_6"));
	elementos_gui.local_labels_puntaje[6] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_escalera"));
	elementos_gui.local_labels_puntaje[7] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_full"));
	elementos_gui.local_labels_puntaje[8] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_poker"));
	elementos_gui.local_labels_puntaje[9] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_generala"));
	elementos_gui.local_labels_puntaje[10] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_generala_doble"));
	elementos_gui.local_labels_puntaje[11] = (GtkLabel*) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "local_puntaje_total"));

	elementos_gui.remoto_labels_puntaje[0] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_1"));
	elementos_gui.remoto_labels_puntaje[1] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_2"));
	elementos_gui.remoto_labels_puntaje[2] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_3"));
	elementos_gui.remoto_labels_puntaje[3] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_4"));
	elementos_gui.remoto_labels_puntaje[4] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_5"));
	elementos_gui.remoto_labels_puntaje[5] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_6"));
	elementos_gui.remoto_labels_puntaje[6] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_escalera"));
	elementos_gui.remoto_labels_puntaje[7] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_full"));
	elementos_gui.remoto_labels_puntaje[8] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_poker"));
	elementos_gui.remoto_labels_puntaje[9] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_generala"));
	elementos_gui.remoto_labels_puntaje[10] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_generala_doble"));
	elementos_gui.remoto_labels_puntaje[11] = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "remoto_puntaje_total"));
}

void obtener_botones()
{
	elementos_gui.boton_tirar = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_tirar"));
	elementos_gui.boton_terminar_turno = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_terminar_turno"));
	elementos_gui.boton_dejar_de_tirar = (GtkButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_dejar_de_tirar"));
}

gboolean puntaje_total_remoto_protegido(void * arg)
{
	int * total = (int *) arg;
	char texto[LONGITUD_CADENA];
	snprintf(texto, LONGITUD_CADENA,
		"%s %i",
		gtk_label_get_text(elementos_gui.remoto_labels_puntaje[CANTIDAD_CATEGORIAS]),
		*total
	);
	gtk_label_set_text(elementos_gui.remoto_labels_puntaje[CANTIDAD_CATEGORIAS], texto);
	return FALSE;
}

void * puntaje_total_remoto(int total)
{
	static int aux;
	aux = total;
	gdk_threads_add_idle(puntaje_total_remoto_protegido, (void *) &aux);
	return NULL;
}

gboolean activar_gui_protegido()
{
	if (a_actualizar.indice > -1) {
		char texto[LONGITUD_CADENA];
		if (a_actualizar.puntaje > -1) {
			// anotar
			if (a_actualizar.puntaje == 1)
				snprintf(texto, LONGITUD_CADENA, "%s %i punto",
					gtk_label_get_text(elementos_gui.remoto_labels_puntaje[a_actualizar.indice]),
					a_actualizar.puntaje);
			else
				snprintf(texto, LONGITUD_CADENA, "%s %i puntos",
					gtk_label_get_text(elementos_gui.remoto_labels_puntaje[a_actualizar.indice]),
					a_actualizar.puntaje);

		} else {
			// tachar
			snprintf(texto, LONGITUD_CADENA, "%s - ", 
				gtk_label_get_text(elementos_gui.remoto_labels_puntaje[a_actualizar.indice]));
		}
		gtk_label_set_text(elementos_gui.remoto_labels_puntaje[a_actualizar.indice], texto);
	}
	gtk_widget_set_sensitive((GtkWidget *) elementos_gui.panel_dados, TRUE);
	gtk_widget_set_sensitive((GtkWidget *) elementos_gui.boton_tirar, TRUE);
	gtk_label_set_text(elementos_gui.label_estado, "Presione el botón \"Tirar dados\"");
	return FALSE;
}

void * activar_gui(int indice, int puntaje)
{
	// nada más el hilo que inició la interfaz puede llamar funciones de GTK
	a_actualizar.indice = indice;
	a_actualizar.puntaje = puntaje;
	gdk_threads_add_idle(activar_gui_protegido, NULL);
	return NULL;
}

void inicializar_gui(int * argc, char *** argv)
{
	gtk_init(argc, argv);

	/* builder */
	elementos_gui.constructor = gtk_builder_new();
	gtk_builder_add_from_file(elementos_gui.constructor, ARCHIVO_GLADE, NULL);

	/* ventana */
	elementos_gui.ventana = GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "ventana"));

	obtener_dados();
	obtener_puntajes();
	obtener_botones();

	elementos_gui.panel_eleccion_categorias = (GtkBox *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "panel_eleccion_categorias"));
	elementos_gui.panel_dados = (GtkBox *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "panel_dados"));
	elementos_gui.lista_botones_categorias = (GSList *) gtk_radio_button_get_group(
			(GtkRadioButton *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "boton_primera_categoria"))
	);

	gtk_builder_add_callback_symbol(elementos_gui.constructor, "dejar_de_tirar", dejar_de_tirar);
	gtk_builder_add_callback_symbol(elementos_gui.constructor, "tirar_dados", tirar_dados_gui);
	gtk_builder_add_callback_symbol(elementos_gui.constructor, "terminar_turno", terminar_turno_gui);

	elementos_gui.label_estado = (GtkLabel *) GTK_WIDGET(gtk_builder_get_object(elementos_gui.constructor, "label_estado"));

	gtk_builder_connect_signals(elementos_gui.constructor, NULL);

	g_object_unref(elementos_gui.constructor);

	gtk_widget_show(elementos_gui.ventana);
	gtk_main();
}

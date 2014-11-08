/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#include "pocketvox-setup.h"
#include "pocketvox-chooser.h"
#include "config.h"
#include <string.h>
#include <gtk/gtk.h>

#include <libintl.h>
#define _(String) dgettext(GETTEXT_PACKAGE,String)

enum
{
	PROP_0,
};

G_DEFINE_TYPE (PocketvoxSetup, pocketvox_setup, G_TYPE_OBJECT);

struct _PocketvoxSetupPrivate
{
  GtkWidget   *window;
  GSettings   *settings;

  GHashTable *apps;
  GtkWidget *listBox;
};

//contain all informations about modules
typedef struct ModuleInfos
{
  GtkWidget *sw;
  GtkWidget *entry_path;
  GtkWidget *entry_id;
}ModuleInfos;

static gint current_line = 0;

static void pocketvox_setup_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_setup_parent_class)->dispose (object);
}

static void pocketvox_setup_finalize(GObject *object)
{
	/*PocketvoxSetup *setup= POCKETVOX_SETUP(object);

	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
		TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;
    */
	G_OBJECT_CLASS (pocketvox_setup_parent_class)->finalize (object);
}

static void pocketvox_setup_set_property (GObject      *gobject,
														 guint         prop_id,
														 const GValue *value,
														 GParamSpec   *pspec)
{
	switch (prop_id)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_setup_get_property (GObject    *gobject,
															guint       prop_id,
														 GValue     *value,
														 GParamSpec *pspec)
{
	switch (prop_id)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_setup_class_init (PocketvoxSetupClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxSetupPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_setup_get_property;
	gklass->set_property = pocketvox_setup_set_property;
	gklass->dispose      = pocketvox_setup_dispose;
	gklass->finalize     = pocketvox_setup_finalize;
}

static void pocketvox_setup_init (PocketvoxSetup *setup)
{
    g_return_if_fail(NULL != setup);

	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

    priv->apps = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
}

static void pocketvox_setup_remove_module(PocketvoxSetup *setup, gpointer data)
{
   	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

    GtkListBoxRow* row = gtk_list_box_get_selected_row(GTK_LIST_BOX(priv->listBox));

    if( row != NULL)
    {
        const gchar* name = gtk_widget_get_name(GTK_WIDGET(row));

        g_hash_table_remove(priv->apps, name);

	    gtk_container_remove(GTK_CONTAINER(priv->listBox), GTK_WIDGET(row));
    }
}

static void pocketvox_setup_search_dict(GtkEntry            *entry,
										GtkEntryIconPosition icon_pos,
										GdkEvent            *event,
										gpointer             user_data)
{
	gboolean choose_folder = !g_strcmp0("hmm", gtk_widget_get_name(GTK_WIDGET(entry)));
	PocketvoxChooser *chooser = pocketvox_chooser_new(choose_folder);
	gchar *path = pocketvox_chooser_get_filepath(chooser);

	g_return_if_fail(NULL != path);

	gtk_entry_set_text(entry, path);
}

static void pocketvox_setup_add_module(PocketvoxSetup *setup, gchar* key, gchar* value, gboolean isapps)
{
	g_return_if_fail(NULL != setup);
    //g_return_if_fail(NULL != value);
    //g_return_if_fail(NULL != key);

   	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

    ModuleInfos *info = (ModuleInfos *)g_malloc0(sizeof(ModuleInfos));

	GtkWidget* label_id 	= gtk_label_new(_("id"));
	GtkWidget* label_app	= gtk_label_new(_("Is this module is associated to a desktop app ?"));
	GtkWidget* label_path	= gtk_label_new(_("Dictionnary's path"));

    GtkWidget* row          = gtk_list_box_row_new();
    gchar *name             = g_strdup_printf("row_%d",current_line);
    gtk_widget_set_name(row, name);

	GtkWidget* grid 	    = gtk_grid_new();
	info->entry_id	        = gtk_entry_new();

    if(key != NULL)
    {
        gtk_entry_set_text(GTK_ENTRY(info->entry_id), key);
    }

    gtk_entry_set_placeholder_text(GTK_ENTRY(info->entry_id), _("your-id"));
    gtk_widget_set_tooltip_text(info->entry_id, _("Set the module's id"));

	info->sw		        = gtk_switch_new();
    gtk_widget_set_tooltip_text(info->sw, _("is this module associated to an desktop application ?"));

	info->entry_path	    = gtk_entry_new();
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY(info->entry_path), GTK_ENTRY_ICON_SECONDARY, "gtk-search");
    gtk_entry_set_placeholder_text(GTK_ENTRY(info->entry_path), _("your path"));
    gtk_widget_set_tooltip_text(info->entry_path,_("Set the dictionnary's path"));
	g_signal_connect(info->entry_path, "icon-press", G_CALLBACK(pocketvox_setup_search_dict), NULL);

	gtk_widget_set_hexpand(label_app, TRUE);
	gtk_widget_set_hexpand(label_path, TRUE);
	gtk_widget_set_hexpand(label_id, TRUE);
	gtk_widget_set_hexpand(info->sw, FALSE);

	g_object_set(G_OBJECT(info->sw),
				"active", isapps,
				NULL);

    if(value != NULL)
    {
        gtk_entry_set_text(GTK_ENTRY(info->entry_path),
                                                value);
    }

    gtk_misc_set_alignment(GTK_MISC(label_id),   0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(label_path), 0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(label_app),  0.0, 0.5);

	gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);
	gtk_grid_attach(GTK_GRID(grid), label_id, 	        0, 0, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), info->entry_id, 	2, 0, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), label_path,         0, 1, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), info->entry_path, 	2, 1, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), label_app, 	        0, 2, 3, 1);
	gtk_grid_attach(GTK_GRID(grid), info->sw,			3, 2, 1, 1);

    gtk_container_add(GTK_CONTAINER(row), grid);
	gtk_widget_show_all(row);

	gtk_list_box_prepend(GTK_LIST_BOX(priv->listBox), row);

    g_hash_table_insert(priv->apps, g_strdup(name), info);

    current_line ++;
}

static void pocketvox_setup_add_module_callback(PocketvoxSetup *setup, gpointer data)
{
    pocketvox_setup_add_module(setup, NULL, NULL, FALSE);
}

static void pocketvox_setup_get_modules_grid(PocketvoxSetup *setup)
{
   	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

	GVariant *modules;
	GVariantIter *iter;
	gchar *key, *value;
	gboolean isapps;

	priv->listBox = gtk_list_box_new();

    GtkWidget *ph_widget = gtk_label_new("Add your modules");
    gtk_label_set_markup(GTK_LABEL(ph_widget), _("<b><b><big> ADD YOUR MODULES </big></b></b>"));
    gtk_widget_show(ph_widget);

    gtk_list_box_set_placeholder(GTK_LIST_BOX(priv->listBox),ph_widget);

    //only need to connect signals
	modules = g_settings_get_value(priv->settings, "list-apps");

	//get the content of the GVariant
	g_variant_get(modules, "a(ssb)", &iter);

	//loop other all apps
	while(g_variant_iter_loop(iter, "(ssb)", &key, &value, &isapps))
	{
		pocketvox_setup_add_module(setup, key, value, isapps);
	}

	g_variant_iter_free(iter);
}

static GtkWidget* pocketvox_setup_get_user_grid(PocketvoxSetup *setup)
{
	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

	GtkWidget *grid 		 	= gtk_grid_new();
	GtkWidget *label_name 	 	= gtk_label_new(_("Your name"));
	GtkWidget *entry_name 		= gtk_entry_new();
	GtkWidget *label_keyword 	= gtk_label_new(_("Activation keyword"));
	GtkWidget *entry_keyword	= gtk_entry_new();
	GtkWidget *label_voice		= gtk_label_new(("Choose the espeak voice"));
	GtkWidget *combo_voice		= gtk_combo_box_text_new();

	gchar *voicesPath = (gchar *)g_getenv("ESPEAK_VOICES_PATH");

	g_return_if_fail(NULL != voicesPath);

	GDir *dir = g_dir_open(voicesPath, 0, NULL);
	const gchar *file ;

	while ((file = g_dir_read_name(dir)) != NULL)
	{
		gchar *path = g_strdup_printf("%s/%s", voicesPath, file);

		if(g_file_test(path, G_FILE_TEST_IS_DIR) == FALSE)
		{
			gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_voice), g_strdup(file), g_strdup(file));
		}

		g_free(path);
	}

	g_dir_close(dir);

    gtk_misc_set_alignment(GTK_MISC(label_name),        0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(label_keyword),     0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(label_voice),       0.0, 0.5);
    gtk_widget_set_hexpand(label_voice,        			TRUE);
    gtk_widget_set_hexpand(label_name,        			TRUE);
    gtk_widget_set_hexpand(label_keyword,         		TRUE);

	g_settings_bind(priv->settings, "name", entry_name, "text", G_SETTINGS_BIND_DEFAULT);
	g_settings_bind(priv->settings, "keyword", entry_keyword, "text", G_SETTINGS_BIND_DEFAULT);
	g_settings_bind(priv->settings, "voice", combo_voice, "active-id", G_SETTINGS_BIND_DEFAULT);

    gtk_widget_set_tooltip_text(entry_name, _("What is your name ?"));
    gtk_widget_set_tooltip_text(entry_keyword, _("What is your activation keyword ?"));
    gtk_widget_set_tooltip_text(combo_voice, _("Choose the language"));

    gtk_grid_attach(GTK_GRID(grid), label_name, 		0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_name, 		1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label_keyword, 		0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_keyword, 		1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label_voice,		0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo_voice,		1, 2, 2, 1);

    gtk_widget_show_all(grid);

    return grid;
}

static GtkWidget* pocketvox_setup_get_pocketsphinx_grid(PocketvoxSetup *setup)
{
	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

	GtkWidget *grid 		      = gtk_grid_new();
    GtkWidget *entry_lm           = gtk_entry_new();
    GtkWidget *entry_acoustic     = gtk_entry_new();
    GtkWidget *entry_dictionnary  = gtk_entry_new();

    GtkWidget *et_lm 	          = gtk_label_new("");
    GtkWidget *et_dict 	          = gtk_label_new("");
    GtkWidget *et_hmm	          = gtk_label_new("");

	gtk_label_set_markup(GTK_LABEL(et_lm),  _("<b> Language model </b>"));
	gtk_label_set_markup(GTK_LABEL(et_hmm), _("<b> Acoustic model </b>"));
	gtk_label_set_markup(GTK_LABEL(et_dict),_("<b> Dictionnary </b>"));

    gtk_misc_set_alignment(GTK_MISC(et_lm),   0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(et_dict), 0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(et_hmm),  0.0, 0.5);
    gtk_widget_set_hexpand(et_lm,    TRUE);
    gtk_widget_set_hexpand(et_dict,  TRUE);
    gtk_widget_set_hexpand(et_hmm,   TRUE);
    gtk_widget_set_hexpand(entry_lm,            TRUE);
    gtk_widget_set_hexpand(entry_acoustic,      TRUE);
    gtk_widget_set_hexpand(entry_dictionnary,   TRUE);
    gtk_widget_set_tooltip_text(entry_lm, _("Set the pocketsphinx's language model"));
    gtk_widget_set_tooltip_text(entry_acoustic, _("Set the pocketsphinx's acoustic model"));
    gtk_widget_set_tooltip_text(entry_dictionnary, _("Set the pocketsphinx's dictionnary"));

    gtk_widget_set_name(entry_lm, "lm");
    gtk_widget_set_name(entry_dictionnary, "dict");
    gtk_widget_set_name(entry_acoustic, "hmm");

	gtk_entry_set_icon_from_icon_name (GTK_ENTRY(entry_lm), GTK_ENTRY_ICON_SECONDARY, "gtk-search");
	g_signal_connect(entry_lm, "icon-press", G_CALLBACK(pocketvox_setup_search_dict), NULL);

	gtk_entry_set_icon_from_icon_name (GTK_ENTRY(entry_dictionnary), GTK_ENTRY_ICON_SECONDARY, "gtk-search");
	g_signal_connect(entry_dictionnary, "icon-press", G_CALLBACK(pocketvox_setup_search_dict), NULL);

	gtk_entry_set_icon_from_icon_name (GTK_ENTRY(entry_acoustic), GTK_ENTRY_ICON_SECONDARY, "gtk-search");
	g_signal_connect(entry_acoustic, "icon-press", G_CALLBACK(pocketvox_setup_search_dict), NULL);

    g_settings_bind(priv->settings, "lm", entry_lm, "text", G_SETTINGS_BIND_DEFAULT);
    g_settings_bind(priv->settings, "hmm", entry_acoustic, "text", G_SETTINGS_BIND_DEFAULT);
    g_settings_bind(priv->settings, "dict", entry_dictionnary, "text", G_SETTINGS_BIND_DEFAULT);

    gtk_grid_attach(GTK_GRID(grid), et_lm, 								0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_lm, 							0, 1, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), et_hmm, 							0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_acoustic, 					0, 3, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), et_dict,						 	0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_dictionnary, 					0, 5, 2, 1);

    gtk_widget_show_all(grid);

	return grid;
}

static GtkWidget* pocketvox_setup_get_notification_grid(PocketvoxSetup *setup)
{
	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

	GtkWidget *grid = gtk_grid_new();
    GtkWidget *label_visual  = gtk_label_new(_("Allow visual notifications"));
    GtkWidget *label_sound   = gtk_label_new(_("Allow sound notifications"));

    gtk_misc_set_alignment(GTK_MISC(label_visual),        0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(label_sound),         0.0, 0.5);
    gtk_widget_set_hexpand(label_visual,        TRUE);
    gtk_widget_set_hexpand(label_sound,         TRUE);

    GtkWidget* switch_sound  = gtk_switch_new();
    GtkWidget* switch_visual = gtk_switch_new();
    gtk_widget_set_tooltip_text(switch_sound, _("Allow sound notifications"));
    gtk_widget_set_tooltip_text(switch_visual, _("Allow visual notifications"));

	g_settings_bind(priv->settings, "visual-notification", switch_visual, "active", G_SETTINGS_BIND_DEFAULT);
	g_settings_bind(priv->settings, "sound-notification", switch_sound, "active", G_SETTINGS_BIND_DEFAULT);

    gtk_grid_attach(GTK_GRID(grid), label_visual,	0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), switch_visual, 	2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_sound, 	0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), switch_sound,	2, 3, 1, 1);

    gtk_widget_show_all(grid);

    return grid;
}

static void pocketvox_setup_combo_manage_entry_mic(GtkWidget *widget, gpointer data)
{
    GtkWidget *combo  = (GtkWidget *)data;
    const gchar* id   = gtk_combo_box_get_active_id(GTK_COMBO_BOX(combo));

    if( !g_strcmp0(id, "Alsa") == TRUE )
    {
        gtk_widget_show(widget);
    }
    else
    {
        gtk_widget_hide(widget);
    }
}

static void pocketvox_setup_combo_manage_network(GtkWidget *widget, gpointer data)
{
    GtkWidget *combo = (GtkWidget *)data;

    const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(combo));

    if( !g_strcmp0(id, "Network") == TRUE )
    {
        gtk_widget_show(widget);
    }
    else
    {
        gtk_widget_hide(widget);
    }
}

static GtkWidget* pocketvox_setup_get_gstreamer_grid(PocketvoxSetup *setup)
{
	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

	GtkWidget *grid = gtk_grid_new();

	GtkWidget* label_mode 	= gtk_label_new(_("Gstreamer source"));
	GtkWidget* combo_mode 	= gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_mode), "Default", 	_("Default"));
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_mode), "Alsa",		_("Alsa"));
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_mode), "Network",	_("Network"));
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_mode), 0);
    gtk_widget_set_tooltip_text(combo_mode, _("Set the source input"));

	GtkWidget* label_mic  	= gtk_label_new(_("Mic"));
	GtkWidget* entry_mic	= gtk_entry_new();
    gtk_widget_set_tooltip_text(entry_mic, _("Which microphone to use ?"));

    GtkWidget* label_host   = gtk_label_new(_("Host"));
    GtkWidget* entry_host   = gtk_entry_new();
    gtk_widget_set_tooltip_text(entry_host, _("Choose the host"));

    GtkWidget* label_port   = gtk_label_new(_("Port"));
    GtkWidget* entry_port   = gtk_entry_new();
    gtk_widget_set_tooltip_text(entry_port, _("Choose the port"));

	g_settings_bind(priv->settings, "source", combo_mode, "active-id", G_SETTINGS_BIND_DEFAULT);
	g_settings_bind(priv->settings, "device", entry_mic,  "text", G_SETTINGS_BIND_DEFAULT);
    g_settings_bind(priv->settings, "port",   entry_port, "text", G_SETTINGS_BIND_DEFAULT);
    g_settings_bind(priv->settings, "host",   entry_host, "text", G_SETTINGS_BIND_DEFAULT);

    g_signal_connect_swapped(combo_mode, "changed", G_CALLBACK(pocketvox_setup_combo_manage_entry_mic), entry_mic);
    g_signal_connect_swapped(combo_mode, "changed", G_CALLBACK(pocketvox_setup_combo_manage_entry_mic), label_mic);
    g_signal_connect_swapped(combo_mode, "changed", G_CALLBACK(pocketvox_setup_combo_manage_network),   label_host);
    g_signal_connect_swapped(combo_mode, "changed", G_CALLBACK(pocketvox_setup_combo_manage_network),   entry_host);
    g_signal_connect_swapped(combo_mode, "changed", G_CALLBACK(pocketvox_setup_combo_manage_network),   label_port);
    g_signal_connect_swapped(combo_mode, "changed", G_CALLBACK(pocketvox_setup_combo_manage_network),   entry_port);

    gtk_misc_set_alignment(GTK_MISC(label_mode),        0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(label_mic),         0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(label_port),        0.0, 0.5);
    gtk_misc_set_alignment(GTK_MISC(label_host),        0.0, 0.5);
    gtk_widget_set_hexpand(label_mode,        			TRUE);
    gtk_widget_set_hexpand(label_mic,         			TRUE);
    gtk_widget_set_hexpand(label_port,                  TRUE);
    gtk_widget_set_hexpand(label_host,                  TRUE);

	gtk_grid_attach(GTK_GRID(grid), label_mode, 		0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), combo_mode, 		1, 0, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), label_mic, 			0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), entry_mic, 	        1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label_host,         0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_host,         1, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label_port,         0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_port,         1, 3, 2, 1);

    gtk_widget_show_all(grid);

    pocketvox_setup_combo_manage_entry_mic(entry_mic, combo_mode);
    pocketvox_setup_combo_manage_entry_mic(label_mic, combo_mode);
    pocketvox_setup_combo_manage_network(label_host,  combo_mode);
    pocketvox_setup_combo_manage_network(entry_host,  combo_mode);
    pocketvox_setup_combo_manage_network(label_port,  combo_mode);
    pocketvox_setup_combo_manage_network(entry_port,  combo_mode);

	return grid;
}

static void pocketvox_stack_child_changed(GtkWidget *button, GParamSpec *pspec, gpointer data)
{
	GtkWidget* stack = (GtkWidget *)data;
	const gchar* name = gtk_stack_get_visible_child_name(GTK_STACK(stack));

	if(G_IS_OBJECT(button))
	{
		if(!g_strcmp0("Setup", name))
		{
			gtk_widget_hide(button);
		}
		else
		{
			gtk_widget_show(button);
		}
	}
}

static int pocketvox_module_check(gconstpointer a, gconstpointer b)
{
  gchar *ca = (gchar *)a;
  gchar *cb = (gchar *)b;

  g_return_val_if_fail(NULL != ca, -1);
  g_return_val_if_fail(NULL != cb, -1);

  return g_strcmp0(a, b);
}

static void pocketvox_setup_save_and_quit(PocketvoxSetup *setup, gpointer data)
{
    gboolean addModule;

    setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

	GVariantBuilder *builder;
	GVariant *value;

    GList* infos = g_hash_table_get_values(priv->apps);
    GList* iter = NULL;
    GList* tmp  = NULL;

	builder = g_variant_builder_new(G_VARIANT_TYPE("a(ssb)"));

    for(iter = infos; iter; iter = iter->next)
    {
        ModuleInfos *info = (ModuleInfos *)iter->data;
        const gchar *id   = gtk_entry_get_text(GTK_ENTRY(info->entry_id));
        const gchar *dict = gtk_entry_get_text(GTK_ENTRY(info->entry_path));
        gboolean isapp    = gtk_switch_get_active(GTK_SWITCH(info->sw));

        //by default each module should be added
        addModule = TRUE;

        //check if we have a valid module
        if( id == NULL
        || !g_strcmp0(id, "") == TRUE
        || dict == NULL
        || !g_strcmp0(dict, "") == TRUE)
        {
            g_warning("PocketvoxSetup: an invalid module couldn't be added to your configuration");
            addModule = FALSE;
        }

        //check if we already find this one
        if(addModule == TRUE)
        {
            addModule = addModule & (NULL == g_list_find_custom(tmp, id, pocketvox_module_check));

            if(addModule == FALSE)
            {
                g_warning("PocketvoxSetup: 2 modules cannot have the same ID");
            }
        }

        //add the new module to the GVariant
        if(addModule == TRUE)
        {
            g_variant_builder_add(builder, "(ssb)", id, dict, isapp);

            tmp = g_list_append(tmp, g_strdup(id));
        }
    }

    value = g_variant_new("a(ssb)", builder);
    g_variant_builder_unref(builder);
    g_settings_set_value(priv->settings, "list-apps", value);
	g_settings_sync();

    g_list_free(infos);

    for(iter=tmp; iter; iter=iter->next)
    {
        g_free((gchar *)iter->data);
    }
    g_list_free(tmp);

    g_hash_table_destroy(priv->apps);

    gtk_main_quit();
}

PocketvoxSetup* pocketvox_setup_new()
{
	PocketvoxSetup *setup = (PocketvoxSetup *)g_object_new(TYPE_POCKETVOX_SETUP, NULL);

	setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

	bindtextdomain (GETTEXT_PACKAGE, PROGRAMNAME_LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

	priv->settings = g_settings_new("org.pocketvox.config");

    //Build the window
    priv->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(priv->window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_title(GTK_WINDOW(priv->window), "pocketvox-gtk");
    gtk_window_set_default_size(GTK_WINDOW(priv->window), 500, 350);
    gtk_window_set_icon_name(GTK_WINDOW(priv->window), "pocketvox");
    gtk_container_set_border_width(GTK_CONTAINER(priv->window), 5);

	g_signal_connect_swapped(priv->window, "delete-event", G_CALLBACK(pocketvox_setup_save_and_quit), setup);

    GtkWidget *bar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(bar), TRUE);
    gtk_window_set_titlebar(GTK_WINDOW(priv->window), bar);

    //add a vertical box
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(priv->window), box);

    //a button for all
    GtkWidget* button_notification 	= gtk_button_new_with_label(_("Notifications"));
    GtkWidget* button_user 			= gtk_button_new_with_label(_("Users"));
    GtkWidget* button_pocketsphinx 	= gtk_button_new_with_label(_("Pocketsphinx"));
    GtkWidget* button_gstreamer		= gtk_button_new_with_label(_("Gstreamer"));

    gtk_widget_set_tooltip_text(button_notification, _("Display notifications parameters"));
    gtk_widget_set_tooltip_text(button_user, _("Display user parameters"));
    gtk_widget_set_tooltip_text(button_pocketsphinx, _("Display pocketsphinx parameters"));
    gtk_widget_set_tooltip_text(button_gstreamer, _("Display gstreamer parameters"));

    gtk_button_set_relief(GTK_BUTTON(button_notification), 	GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(button_user), 			GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(button_pocketsphinx), 	GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(button_gstreamer), 	GTK_RELIEF_NONE);

    GtkWidget *grid_button	= gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid_button), button_user, 		0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_button), button_notification, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_button), button_pocketsphinx, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_button), button_gstreamer,	0, 3, 1, 1);

    GtkWidget* grid_notification 	= pocketvox_setup_get_notification_grid(setup);
    GtkWidget* grid_user			= pocketvox_setup_get_user_grid(setup);
    GtkWidget* grid_pocketsphinx	= pocketvox_setup_get_pocketsphinx_grid(setup);
    GtkWidget* grid_gstreamer		= pocketvox_setup_get_gstreamer_grid(setup);

    //connect all signals (TODO)
    g_signal_connect_swapped(button_notification, "clicked", G_CALLBACK(gtk_widget_show), grid_notification);
    g_signal_connect_swapped(button_notification, "clicked", G_CALLBACK(gtk_widget_hide), grid_user);
    g_signal_connect_swapped(button_notification, "clicked", G_CALLBACK(gtk_widget_hide),  grid_pocketsphinx);
    g_signal_connect_swapped(button_notification, "clicked", G_CALLBACK(gtk_widget_hide), grid_gstreamer);

    g_signal_connect_swapped(button_pocketsphinx, "clicked", G_CALLBACK(gtk_widget_hide), grid_notification);
    g_signal_connect_swapped(button_pocketsphinx, "clicked", G_CALLBACK(gtk_widget_hide), grid_user);
    g_signal_connect_swapped(button_pocketsphinx, "clicked", G_CALLBACK(gtk_widget_show),  grid_pocketsphinx);
    g_signal_connect_swapped(button_pocketsphinx, "clicked", G_CALLBACK(gtk_widget_hide), grid_gstreamer);

    g_signal_connect_swapped(button_user, "clicked", G_CALLBACK(gtk_widget_hide), grid_notification);
    g_signal_connect_swapped(button_user, "clicked", G_CALLBACK(gtk_widget_show), grid_user);
    g_signal_connect_swapped(button_user, "clicked", G_CALLBACK(gtk_widget_hide),  grid_pocketsphinx);
    g_signal_connect_swapped(button_user, "clicked", G_CALLBACK(gtk_widget_hide), grid_gstreamer);

    g_signal_connect_swapped(button_gstreamer, "clicked", G_CALLBACK(gtk_widget_hide), grid_notification);
    g_signal_connect_swapped(button_gstreamer, "clicked", G_CALLBACK(gtk_widget_hide), grid_user);
    g_signal_connect_swapped(button_gstreamer, "clicked", G_CALLBACK(gtk_widget_hide),  grid_pocketsphinx);
    g_signal_connect_swapped(button_gstreamer, "clicked", G_CALLBACK(gtk_widget_show), grid_gstreamer);

    //add
    GtkWidget* gridBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(gridBox), grid_user, 		TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gridBox), grid_notification, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gridBox), grid_pocketsphinx, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(gridBox), grid_gstreamer, 	TRUE, TRUE, 0);

    GtkWidget* separator =  gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_widget_show(separator);

    GtkWidget *hgridBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(hgridBox), grid_button, 	FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hgridBox), separator, 	FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hgridBox), gridBox, 		TRUE, TRUE, 0);

    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack) , GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 1000);

    gtk_stack_add_titled(GTK_STACK(stack), hgridBox, "Setup", _("Configuration"));

	GtkWidget* scrolledWindow  = gtk_scrolled_window_new(NULL, NULL);

    //return void
    pocketvox_setup_get_modules_grid(setup);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), priv->listBox);
    gtk_widget_show_all(scrolledWindow);

    gtk_stack_add_titled(GTK_STACK(stack), scrolledWindow, "Modules", _("Modules"));

    //adding a task switcher
    GtkWidget* stackSwitcher = gtk_stack_switcher_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stackSwitcher), GTK_STACK(stack));

    gtk_header_bar_set_custom_title(GTK_HEADER_BAR(bar), stackSwitcher);

	GtkWidget *add_module_button 	= gtk_button_new_from_icon_name("gtk-new", GTK_ICON_SIZE_MENU);
	GtkWidget *remove_module_button = gtk_button_new_from_icon_name("gtk-delete", GTK_ICON_SIZE_MENU);
	gtk_button_set_relief(GTK_BUTTON(add_module_button), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(remove_module_button), GTK_RELIEF_NONE);

	gtk_header_bar_pack_start(GTK_HEADER_BAR(bar), add_module_button);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(bar), remove_module_button);

	g_signal_connect_swapped(remove_module_button, 	"clicked", 					G_CALLBACK(pocketvox_setup_remove_module), 			setup);
	g_signal_connect_swapped(add_module_button, 	"clicked", 					G_CALLBACK(pocketvox_setup_add_module_callback),    setup);
	g_signal_connect_swapped(stack, 				"notify::visible-child", 	G_CALLBACK(pocketvox_stack_child_changed), 			add_module_button);
	g_signal_connect_swapped(stack, 				"notify::visible-child", 	G_CALLBACK(pocketvox_stack_child_changed), 			remove_module_button);

    //add them to the vbox
    gtk_box_pack_start(GTK_BOX(box), stack, TRUE, TRUE, 0);

    gtk_widget_show(grid_user);
    gtk_widget_hide(grid_notification);
    gtk_widget_hide(grid_pocketsphinx);
    gtk_widget_hide(grid_gstreamer);
    gtk_widget_show(gridBox);

    gtk_widget_show_all(grid_button);
    gtk_widget_show(hgridBox);
    gtk_widget_show(box);
	gtk_widget_show(stack);
	gtk_widget_show(stackSwitcher);
	gtk_widget_show_all(bar);

    //get the setup
	return setup;
}

void pocketvox_setup_show(PocketvoxSetup *setup)
{
    g_return_if_fail(NULL != setup);

    setup->priv = G_TYPE_INSTANCE_GET_PRIVATE (setup,
			TYPE_POCKETVOX_SETUP, PocketvoxSetupPrivate);
	PocketvoxSetupPrivate *priv = setup->priv;

    gtk_widget_show(priv->window);

    gtk_main();
}

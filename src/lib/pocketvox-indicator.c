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

#include "pocketvox-indicator.h"
#include "pocketvox-types.h"
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <libappindicator/app-indicator.h>

#include "config.h"

#include <libintl.h>
#define _(String) dgettext(GETTEXT_PACKAGE, String)

enum
{
	INDICATOR_STATE,
	INDICATOR_QUIT,
	INDICATOR_MODULE_TOGGLED,
	LAST_SIGNAL
};

enum
{
	PROP_0,
};

struct _PocketvoxIndicatorPrivate
{
	//will appear in the panel
	AppIndicator *applet;

	//will contain all actions
	GtkWidget* menu;

    //state of the application
    PocketvoxState state;

	//GHashtable containing menuItem associated to their module id
	GHashTable *table;
	GtkWidget* modulesMenu;

    //idem but for apps
	GtkWidget* appsMenu;
	GHashTable *apps;

    //some items to hide/show them
    GtkWidget *modulesItem;
    GtkWidget *appsItem;
};


G_DEFINE_TYPE (PocketvoxIndicator, pocketvox_indicator, G_TYPE_OBJECT);

static guint pocketvox_indicator_signals[LAST_SIGNAL];

static void pocketvox_indicator_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_indicator_parent_class)->dispose (object);
}

static void pocketvox_indicator_finalize(GObject *object)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)object;

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;

	g_hash_table_destroy(priv->table);
	g_hash_table_destroy(priv->apps);

	G_OBJECT_CLASS (pocketvox_indicator_parent_class)->finalize (object);
}

static void pocketvox_indicator_set_property (GObject      *gobject,
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

static void pocketvox_indicator_get_property (GObject    *gobject,
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

static void pocketvox_indicator_class_init (PocketvoxIndicatorClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxIndicatorPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_indicator_get_property;
	gklass->set_property = pocketvox_indicator_set_property;
	gklass->dispose      = pocketvox_indicator_dispose;
	gklass->finalize     = pocketvox_indicator_finalize;

    pocketvox_indicator_signals[INDICATOR_STATE] =
        g_signal_new("indicator_state",
                     G_TYPE_FROM_CLASS(klass),
                     G_SIGNAL_RUN_LAST,
                     0,
                     NULL, NULL,
                     g_cclosure_marshal_VOID__POINTER,
                     G_TYPE_NONE,
                     1,
                     G_TYPE_STRING
        );

    pocketvox_indicator_signals[INDICATOR_QUIT] =
        g_signal_new("indicator_quit",
                     G_TYPE_FROM_CLASS(klass),
                     G_SIGNAL_RUN_LAST,
                     0,
                     NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE,
                     0
        );

    pocketvox_indicator_signals[INDICATOR_MODULE_TOGGLED] =
        g_signal_new("indicator_module_toggled",
                     G_TYPE_FROM_CLASS(klass),
                     G_SIGNAL_RUN_LAST,
                     0,
                     NULL, NULL,
                     g_cclosure_marshal_VOID__POINTER,
                     G_TYPE_NONE,
                     1,
                     G_TYPE_STRING
        );
}

static void pocketvox_indicator_free_item(gpointer data){}

static void pocketvox_indicator_init (PocketvoxIndicator *indicator)
{
	g_return_if_fail(NULL != indicator);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;

	priv->applet		= app_indicator_new("pocketvox_indicator",
									"audio-input-microphone-high-symbolic",
									APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

	priv->menu			= gtk_menu_new();
	priv->modulesMenu 	= gtk_menu_new();

	priv->appsMenu		= gtk_menu_new();
	priv->apps			= g_hash_table_new_full(g_str_hash, g_str_equal, g_free, pocketvox_indicator_free_item);

	priv->state 		= POCKETVOX_STATE_STOP;
	priv->table			= g_hash_table_new_full(g_str_hash, g_str_equal, g_free, pocketvox_indicator_free_item);
}

static void pocketvox_indicator_state_changed(GtkMenuItem *menuitem, gpointer user_data)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)user_data;

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;

	priv->state = !priv->state;

	gtk_menu_item_set_label(menuitem, priv->state ? _("Stop") : _("Run"));

	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_STATE], 0, priv->state ? "Run" : "Stop");
}

static void pocketvox_indicator_quit(GtkMenuItem *item, gpointer user_data)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)user_data;

	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_QUIT], 0);
}

PocketvoxIndicator* pocketvox_indicator_new()
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)g_object_new(TYPE_POCKETVOX_INDICATOR, NULL);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;

	app_indicator_set_status(priv->applet, APP_INDICATOR_STATUS_ACTIVE);

	GtkWidget* stateItem 		= gtk_menu_item_new_with_label(_("Run"));
    priv->modulesItem  	        = gtk_menu_item_new_with_label(_("Modules"));

    GtkWidget* separatorItem0	= gtk_separator_menu_item_new();
	GtkWidget* separatorItem1	= gtk_separator_menu_item_new();
	GtkWidget* quitItem 		= gtk_menu_item_new_with_label(_("Quit"));
    priv->appsItem			    = gtk_menu_item_new_with_label(_("Apps"));

	gtk_menu_item_set_submenu((GtkMenuItem *)priv->modulesItem, priv->modulesMenu);
	gtk_menu_item_set_submenu((GtkMenuItem *)priv->appsItem, priv->appsMenu);

	gtk_menu_attach((GtkMenu *)priv->menu, stateItem, 			0, 1, 0, 1);
	gtk_menu_attach((GtkMenu *)priv->menu, separatorItem0,		0, 1, 1, 2);
	gtk_menu_attach((GtkMenu *)priv->menu, priv->modulesItem, 	0, 1, 2, 3);
	gtk_menu_attach((GtkMenu *)priv->menu, priv->appsItem,		0, 1, 3, 4);
	gtk_menu_attach((GtkMenu *)priv->menu, separatorItem1,		0, 1, 4, 5);
	gtk_menu_attach((GtkMenu *)priv->menu, quitItem, 			0, 1, 5, 6);

	gtk_widget_show(priv->appsMenu);
	gtk_widget_hide(priv->appsItem);
	gtk_widget_show(priv->modulesMenu);
	gtk_widget_show(stateItem);
	gtk_widget_show(separatorItem0);
	gtk_widget_show(separatorItem1);
	gtk_widget_hide(priv->modulesItem);
	gtk_widget_show(quitItem);
	gtk_widget_show(priv->menu);

	g_signal_connect(stateItem, 	"activate", G_CALLBACK(pocketvox_indicator_state_changed), 	indicator);
	g_signal_connect(quitItem,  	"activate", G_CALLBACK(pocketvox_indicator_quit), 			indicator);

	app_indicator_set_menu(priv->applet, (GtkMenu *)priv->menu);

	return indicator;
}

static void pocketvox_indicator_module_toggled(PocketvoxIndicator *indicator, gpointer data)
{
	GtkCheckMenuItem *widget = (GtkCheckMenuItem *)data;

	g_return_if_fail(NULL != indicator);
	g_return_if_fail(NULL != widget);

	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_MODULE_TOGGLED], 0, gtk_menu_item_get_label((GtkMenuItem *)widget));
}


void pocketvox_indicator_add_apps_item(PocketvoxIndicator *indicator, gchar *id)
{
	g_return_if_fail(NULL != indicator);
	g_return_if_fail(NULL != id);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;

	GtkWidget* item = gtk_menu_item_new_with_label(id);
	//gtk_check_menu_item_set_draw_as_radio((GtkCheckMenuItem *)item, TRUE);
	//gtk_check_menu_item_set_active((GtkCheckMenuItem *)item, FALSE);

	//gtk_widget_set_sensitive(item, FALSE);

	g_hash_table_insert(priv->apps, g_strdup(id), item);

	gtk_menu_shell_append((GtkMenuShell *)priv->appsMenu, item);
	gtk_widget_queue_draw(priv->appsMenu);

	gtk_widget_show(item);
	gtk_widget_show(priv->appsMenu);
    gtk_widget_show(priv->appsItem);
}

void pocketvox_indicator_add_module_item(PocketvoxIndicator *indicator,gchar *id)
{
	g_return_if_fail(NULL != indicator);
	g_return_if_fail(NULL != id);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;

	GtkWidget* item = gtk_check_menu_item_new_with_label(id);
	gtk_check_menu_item_set_draw_as_radio((GtkCheckMenuItem *)item, TRUE);
	gtk_check_menu_item_set_active((GtkCheckMenuItem *)item, TRUE);

	g_signal_connect_swapped((GtkCheckMenuItem *)item, "toggled", G_CALLBACK(pocketvox_indicator_module_toggled), indicator);

	g_hash_table_insert(priv->table, g_strdup(id), item);

	gtk_menu_shell_append((GtkMenuShell *)priv->modulesMenu, item);
	gtk_widget_queue_draw(priv->modulesMenu);

	gtk_widget_show(item);
	gtk_widget_show(priv->modulesMenu);
    gtk_widget_show(priv->modulesItem);
}

void pocketvox_indicator_remove_module_item(PocketvoxIndicator *indicator, gchar *id)
{
	g_return_if_fail(NULL != indicator);
	g_return_if_fail(NULL != id);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;

	GtkWidget* item = (GtkWidget *)g_hash_table_lookup(priv->table, id);
	gtk_container_remove((GtkContainer *)priv->modulesMenu, item);
	g_hash_table_remove(priv->table, id);
	gtk_widget_queue_draw(priv->modulesMenu);
}

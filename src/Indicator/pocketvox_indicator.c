#include "pocketvox_indicator.h"
#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

enum
{
	PROP_0,
};

struct _PocketvoxIndicatorPrivate 
{
	//will appear in the panel
	AppIndicator *applet;
	//will contain all actions 
	GtkWidget *menu;	
};

G_DEFINE_TYPE (PocketvoxIndicator, pocketvox_indicator, G_TYPE_OBJECT);

static void pocketvox_indicator_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_indicator_parent_class)->dispose (object);
}

static void pocketvox_indicator_finalize(GObject *object)
{
	PocketvoxIndicator *indicator= POCKETVOX_INDICATOR(object);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
		TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;	
	
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
}

/** Initialisation de l'objet */
static void pocketvox_indicator_init (PocketvoxIndicator *indicator)
{
	g_return_if_fail(NULL != indicator);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;	

	priv->applet= app_indicator_new("pocketvox_indicator",
									"audio-input-microphone-high-symbolic",
									APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
	
	
	
	priv->menu	= gtk_menu_new();
}


PocketvoxIndicator* pocketvox_indicator_new()
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)g_object_new(TYPE_POCKETVOX_INDICATOR, NULL);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;	
	
	app_indicator_set_status(priv->applet, APP_INDICATOR_STATUS_ACTIVE);
	
	GtkWidget* stateItem = gtk_menu_item_new_with_label("Launch");
	GtkWidget* configItem = gtk_menu_item_new_with_label("Configure");
	GtkWidget* buildItem = gtk_menu_item_new_with_label("Rebuild");
	
	gtk_widget_show(stateItem);
	gtk_widget_show(configItem);
	gtk_widget_show(buildItem);
	
	gtk_menu_attach((GtkMenu *)priv->menu, stateItem, 0, 1, 0, 1);
	gtk_menu_attach((GtkMenu *)priv->menu, configItem, 0, 1, 1, 2);
	gtk_menu_attach((GtkMenu *)priv->menu, buildItem, 0, 1, 2, 3);
	
	gtk_widget_show(priv->menu);
	
	app_indicator_set_menu(priv->applet, (GtkMenu *)priv->menu);
	
	return indicator;																		
}

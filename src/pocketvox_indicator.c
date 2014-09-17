#include "pocketvox_indicator.h"
#include "pocketvox_types.h"
#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

enum 
{
	INDICATOR_STATE,
	INDICATOR_DICT,
	INDICATOR_LM,
	INDICATOR_ACOUSTIC,
	INDICATOR_REBUILD,
	INDICATOR_QUIT,
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
	GtkWidget *menu;	
	PocketVoxState state;
};

G_DEFINE_TYPE (PocketvoxIndicator, pocketvox_indicator, G_TYPE_OBJECT);

static guint pocketvox_indicator_signals[LAST_SIGNAL];

static void pocketvox_indicator_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_indicator_parent_class)->dispose (object);
}

static void pocketvox_indicator_finalize(GObject *object)
{			
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

    pocketvox_indicator_signals[INDICATOR_DICT] = 
        g_signal_new("indicator_dict",
                     G_TYPE_FROM_CLASS(klass),
                     G_SIGNAL_RUN_LAST,
                     0,
                     NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE,
                     0 
        );	

    pocketvox_indicator_signals[INDICATOR_LM] = 
        g_signal_new("indicator_lm",
                     G_TYPE_FROM_CLASS(klass),
                     G_SIGNAL_RUN_LAST,
                     0,
                     NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE,
                     0 
        );	

    pocketvox_indicator_signals[INDICATOR_ACOUSTIC] = 
        g_signal_new("indicator_acoustic",
                     G_TYPE_FROM_CLASS(klass),
                     G_SIGNAL_RUN_LAST,
                     0,
                     NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE,
                     0 
        );	

    pocketvox_indicator_signals[INDICATOR_REBUILD] = 
        g_signal_new("indicator_rebuild",
                     G_TYPE_FROM_CLASS(klass),
                     G_SIGNAL_RUN_LAST,
                     0,
                     NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE,
                     0 
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
	
	//initial state of the application
	priv->state = POCKETVOX_STATE_STOP;
}

static void pocketvox_indicator_state_changed(GtkMenuItem *menuitem, gpointer user_data)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)user_data;

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;	
	
	priv->state = !priv->state;
	
	gtk_menu_item_set_label(menuitem, priv->state ? "Stop" : "Run");
	
	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_STATE], 0, priv->state ? "Run" : "Stop");
}

static void pocketvox_indicator_rebuild(GtkMenuItem *item, gpointer user_data)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)user_data;
		
	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_REBUILD], 0);
}

static void pocketvox_indicator_quit(GtkMenuItem *item, gpointer user_data)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)user_data;
		
	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_QUIT], 0);	
}

static void pocketvox_indicator_dictionnary(GtkMenuItem *item, gpointer user_data)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)user_data;
		
	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_DICT], 0);	
}

static void pocketvox_indicator_lm(GtkMenuItem *item, gpointer user_data)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)user_data;
		
	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_LM], 0);	
}

static void pocketvox_indicator_acoustic(GtkMenuItem *item, gpointer user_data)
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)user_data;
		
	g_signal_emit(indicator, pocketvox_indicator_signals[INDICATOR_ACOUSTIC], 0);	
}

PocketvoxIndicator* pocketvox_indicator_new()
{
	PocketvoxIndicator *indicator = (PocketvoxIndicator *)g_object_new(TYPE_POCKETVOX_INDICATOR, NULL);

	indicator->priv = G_TYPE_INSTANCE_GET_PRIVATE (indicator,
			TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorPrivate);
	PocketvoxIndicatorPrivate *priv = indicator->priv;	
	
	app_indicator_set_status(priv->applet, APP_INDICATOR_STATUS_ACTIVE);
	
	GtkWidget* stateItem 	= gtk_menu_item_new_with_label("Run");
	GtkWidget* configItem 	= gtk_menu_item_new_with_label("Settings");
	GtkWidget* buildItem 	= gtk_menu_item_new_with_label("Rebuild");
	GtkWidget* quitItem 	= gtk_menu_item_new_with_label("Quit");
	
	GtkWidget* submenu		= gtk_menu_new();
	GtkWidget* dictItem		= gtk_menu_item_new_with_label("Set the dictionnary");
	GtkWidget* lmItem		= gtk_menu_item_new_with_label("Set the language model");
	GtkWidget* acousticItem = gtk_menu_item_new_with_label("Set the acoustic model"); 
	
	gtk_widget_show(dictItem);
	gtk_widget_show(lmItem);
	gtk_widget_show(acousticItem);
	
	gtk_menu_attach((GtkMenu *)submenu, dictItem, 		0, 1, 0, 1);
	gtk_menu_attach((GtkMenu *)submenu, lmItem, 		0, 1, 1, 2);
	gtk_menu_attach((GtkMenu *)submenu, acousticItem,	0, 1, 2, 3); 
	
	gtk_widget_show(submenu);
	gtk_menu_item_set_submenu((GtkMenuItem *)configItem, submenu);
		
	gtk_widget_show(stateItem);
	gtk_widget_show(configItem);
	gtk_widget_show(buildItem);
	gtk_widget_show(quitItem);
	
	gtk_menu_attach((GtkMenu *)priv->menu, stateItem, 	0, 1, 0, 1);
	gtk_menu_attach((GtkMenu *)priv->menu, configItem, 	0, 1, 1, 2);
	gtk_menu_attach((GtkMenu *)priv->menu, buildItem, 	0, 1, 2, 3);
	gtk_menu_attach((GtkMenu *)priv->menu, quitItem, 	0, 1, 3, 4);

	g_signal_connect(stateItem, 	"activate", G_CALLBACK(pocketvox_indicator_state_changed), 	indicator);
	g_signal_connect(buildItem, 	"activate", G_CALLBACK(pocketvox_indicator_rebuild), 		indicator);
	g_signal_connect(quitItem,  	"activate", G_CALLBACK(pocketvox_indicator_quit), 			indicator);
	g_signal_connect(dictItem,		"activate", G_CALLBACK(pocketvox_indicator_dictionnary), 	indicator);
	g_signal_connect(lmItem, 		"activate", G_CALLBACK(pocketvox_indicator_lm),				indicator);
	g_signal_connect(acousticItem,	"activate",	G_CALLBACK(pocketvox_indicator_acoustic),		indicator);
	
		
	gtk_widget_show(priv->menu);
	
	app_indicator_set_menu(priv->applet, (GtkMenu *)priv->menu);
	
	return indicator;																		
}

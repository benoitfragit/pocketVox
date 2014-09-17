#include "pocketvox_controller.h"


enum
{
	PROP_0,
};

struct _PocketvoxControllerPrivate 
{
	PocketvoxRecognizer *recognizer; 
	PocketvoxNotifier	*notifier;
	PocketvoxDictionnary *dictionnary;
	PocketvoxIndicator *indicator;
	GMainLoop* loop;
};

G_DEFINE_TYPE (PocketvoxController, pocketvox_controller, G_TYPE_OBJECT);

static void pocketvox_controller_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_controller_parent_class)->dispose (object);
}

static void pocketvox_controller_finalize(GObject *object)
{
	PocketvoxController *controller= POCKETVOX_CONTROLLER(object);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
		TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;	

	g_object_unref(priv->recognizer);
	g_object_unref(priv->notifier);
	g_object_unref(priv->dictionnary);
	g_object_unref(priv->indicator);
	
	G_OBJECT_CLASS (pocketvox_controller_parent_class)->finalize (object);
}

static void pocketvox_controller_set_property (GObject      *gobject,
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

static void pocketvox_controller_get_property (GObject    *gobject,
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

static void pocketvox_controller_class_init (PocketvoxControllerClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxControllerPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_controller_get_property;
	gklass->set_property = pocketvox_controller_set_property;
	gklass->dispose      = pocketvox_controller_dispose;
	gklass->finalize     = pocketvox_controller_finalize;
}

/** Initialisation de l'objet */
static void pocketvox_controller_init (PocketvoxController *controller){
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;	

	priv->recognizer 	= NULL;
	priv->notifier 		= NULL;
	priv->dictionnary 	= NULL;
	priv->indicator   	= NULL;
}

static void pocketvox_controller_state_changer(PocketvoxController *controller, gchar *state, gpointer user_data)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;	
	
	if(!g_strcmp0("Run", state))
	{
		pocketvox_recognizer_set_state(priv->recognizer, GST_STATE_PLAYING);
	}	
	else
	{
		pocketvox_recognizer_set_state(priv->recognizer, GST_STATE_PAUSED);
	}
	
	pocketvox_notifier_notify(priv->notifier,state, NULL);
}

static void pocketvox_controller_quit(PocketvoxController *controller, gpointer user_data)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;	
	
	pocketvox_controller_finalize(G_OBJECT(controller));
	
	g_main_loop_quit(priv->loop);
	g_main_loop_unref(priv->loop);
}

static void pocketvox_controller_rebuild(PocketvoxController *controller, gpointer user_data)
{
	g_warning("Should rebuild the language model file");
}

static void pocketvox_controller_set_dict(PocketvoxController *controller, gpointer user_data)
{
	g_warning("Set the dictionnary");
}

static void pocketvox_controller_set_lm(PocketvoxController *controller, gpointer user_data)
{
	g_warning("Set the lm");
}

static void pocketvox_controller_set_acoustic(PocketvoxController *controller, gpointer user_data)
{
	g_warning("Set the acoustic");
}

PocketvoxController* pocketvox_controller_new(PocketvoxRecognizer *recognizer, 
											  PocketvoxNotifier	*notifier,
											  PocketvoxDictionnary *dictionnary,
											  PocketvoxIndicator *indicator)
{
	g_return_val_if_fail(NULL != recognizer
					  && NULL != notifier
					  && NULL != dictionnary
					  && NULL != indicator,
					  NULL);
					  
	PocketvoxController *controller = (PocketvoxController *)g_object_new(TYPE_POCKETVOX_CONTROLLER, NULL);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;
	
	priv->recognizer 	= recognizer;
	priv->notifier 		= notifier;
	priv->dictionnary 	= dictionnary;
	priv->indicator 	= indicator;
	
	//connect all signals
	g_signal_connect_swapped(priv->recognizer, 
							"result",
							G_CALLBACK(pocketvox_notifier_notify),
							priv->notifier);
							
	g_signal_connect_swapped(priv->indicator,
							"indicator_state",
							G_CALLBACK(pocketvox_controller_state_changer),
							controller);
							
	g_signal_connect_swapped(priv->indicator, 	
							"indicator_quit",
							G_CALLBACK(pocketvox_controller_quit),
							controller);
	
	g_signal_connect_swapped(priv->indicator,
							"indicator_rebuild",
							G_CALLBACK(pocketvox_controller_rebuild),
							controller);

	g_signal_connect_swapped(priv->indicator,
							"indicator_dict",
							G_CALLBACK(pocketvox_controller_set_dict),
							controller);
	
	g_signal_connect_swapped(priv->indicator,
							"indicator_lm",
							G_CALLBACK(pocketvox_controller_set_lm),
							controller);
	
	g_signal_connect_swapped(priv->indicator,
							"indicator_acoustic",
							G_CALLBACK(pocketvox_controller_set_acoustic),
							controller);
								
	return controller;
}

void pocketvox_controller_start(PocketvoxController *controller)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;
	
	priv->loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(priv->loop);
}

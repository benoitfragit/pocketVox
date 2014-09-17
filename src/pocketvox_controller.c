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
};

G_DEFINE_TYPE (PocketvoxController, pocketvox_controller, G_TYPE_OBJECT);

static void pocketvox_controller_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_controller_parent_class)->dispose (object);
}

static void pocketvox_controller_finalize(GObject *object)
{
/*	PocketvoxController *controller= POCKETVOX_CONTROLLER(object);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
		TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;	
*/	
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
	
	priv->recognizer = recognizer;
	priv->notifier = notifier;
	priv->dictionnary = dictionnary;
	priv->indicator = indicator;
	
	//connect all signals
	
	
	return controller;
}

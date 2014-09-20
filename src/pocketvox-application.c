#include "pocketvox-application.h"
#include "pocketvox-recognizer.h"
#include "pocketvox-notifier.h"
#include "pocketvox-indicator.h"
#include "pocketvox-controller.h"
#include <gtk/gtk.h>
#include <gst/gst.h>

enum
{
	PROP_0,
};

struct _PocketvoxApplicationPrivate 
{
	PocketvoxIndicator  *indicator;
	PocketvoxNotifier   *notifier;
	PocketvoxRecognizer *recognizer;
	
	PocketvoxController *controller;
};

G_DEFINE_TYPE (PocketvoxApplication, pocketvox_application, G_TYPE_OBJECT);

static void pocketvox_application_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_application_parent_class)->dispose (object);
}

static void pocketvox_application_finalize(GObject *object)
{
	PocketvoxApplication *application= POCKETVOX_APPLICATION(object);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
		TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;	
	
	g_object_unref(priv->recognizer);
	g_object_unref(priv->notifier);
	g_object_unref(priv->indicator);
	g_object_unref(priv->controller);
	
	G_OBJECT_CLASS (pocketvox_application_parent_class)->finalize (object);
}

static void pocketvox_application_set_property (GObject      *gobject,
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

static void pocketvox_application_get_property (GObject    *gobject,
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

static void pocketvox_application_class_init (PocketvoxApplicationClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxApplicationPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_application_get_property;
	gklass->set_property = pocketvox_application_set_property;
	gklass->dispose      = pocketvox_application_dispose;
	gklass->finalize     = pocketvox_application_finalize;
}

static void pocketvox_application_init (PocketvoxApplication *application){
	g_return_if_fail(NULL != application);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;	
	
	priv->recognizer 	= NULL;
	priv->notifier 		= NULL;
	priv->controller 	= NULL;
	priv->indicator 	= NULL;
}

PocketvoxApplication* pocketvox_application_new(gchar* acoustic, gchar *lm, gchar *dic)
{
	PocketvoxApplication *application = (PocketvoxApplication *)g_object_new(TYPE_POCKETVOX_APPLICATION, NULL);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;
	
	gtk_init(NULL, NULL);
	gst_init(NULL, NULL);
	
	priv->indicator 	= pocketvox_indicator_new();
	priv->notifier 		= pocketvox_notifier_new();
	priv->recognizer 	= pocketvox_recognizer_new(acoustic, lm, dic);
	priv->controller	= pocketvox_controller_new(priv->recognizer, priv->notifier, priv->indicator);

	PocketvoxModule *m1 = pocketvox_module_new("m1", "/home/benoit/Projet/google2ubuntu/workinprogress/test.txt", FALSE);
	PocketvoxModule *m2 = pocketvox_module_new("m2", "/home/benoit/Projet/google2ubuntu/workinprogress/test.txt", FALSE);

	pocketvox_controller_add_module(priv->controller, m1);
	pocketvox_controller_add_module(priv->controller, m2);
	
	pocketvox_controller_remove_module(priv->controller, "m1");
			
	return application;																		
}

void pocketvox_application_start(PocketvoxApplication *application)
{
	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;
	
	pocketvox_controller_start(priv->controller);			
}

#include "pocketvox-application.h"
#include "pocketvox-recognizer.h"
#include "pocketvox-notifier.h"
#include "pocketvox-indicator.h"
#include "pocketvox-controller.h"
#include "pocketvox-profile.h"
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
	PocketvoxProfile 	*profile;
};

G_DEFINE_TYPE (PocketvoxApplication, pocketvox_application, G_TYPE_OBJECT);

static void pocketvox_application_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_application_parent_class)->dispose (object);
}

static void pocketvox_application_finalize(GObject *object)
{
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
	priv->profile		= NULL;
}

static void pocketvox_application_add_profile_module(gpointer key, gpointer value, gpointer data)
{
    gchar *app = (gchar *)key;
    gchar *dict = (gchar *)value;

    PocketvoxApplication *application = (PocketvoxApplication *)data;

    PocketvoxModule* module = pocketvox_module_new(g_strdup(app), g_strdup(dict), FALSE);

    //set the module ModuleProfile to TRUE :: TODO
    g_object_set(G_OBJECT(module), "apps", TRUE, NULL);

    //here we should define the execute methoee :: TODO

    //add the module to the application
    pocketvox_application_add_module( application, module);
}

PocketvoxApplication* pocketvox_application_new(gchar* path)
{
	g_return_val_if_fail(NULL != path, NULL);

	PocketvoxApplication *application = (PocketvoxApplication *)g_object_new(TYPE_POCKETVOX_APPLICATION, NULL);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	gtk_init(NULL, NULL);
	gst_init(NULL, NULL);

	//read the user profile
	priv->profile		= pocketvox_profile_new(path);

	//we will set the starting voice and give your name here
	gchar *name 		= pocketvox_profile_get_name(priv->profile);
	gchar *voice		= pocketvox_profile_get_voice(priv->profile);
	gchar *lm			= pocketvox_profile_get_lm(priv->profile);
	gchar *dic			= pocketvox_profile_get_dict(priv->profile);
	gchar *acoustic		= pocketvox_profile_get_acoustic(priv->profile);

    GHashTable* apps    = pocketvox_profile_get_profile_apps(priv->profile);

	priv->indicator 	= pocketvox_indicator_new(voice);
	priv->notifier 		= pocketvox_notifier_new(name, voice);
	priv->recognizer 	= pocketvox_recognizer_new(acoustic, lm, dic);
	priv->controller	= pocketvox_controller_new(priv->recognizer, priv->notifier, priv->indicator);

    g_hash_table_foreach(apps, pocketvox_application_add_profile_module, application);

	//a little startup msg
	gchar *startup = g_strdup_printf("Hello %s, I'm listening you", name);
	pocketvox_notifier_say(priv->notifier, startup);
	g_free(startup);

	return application;
}

void pocketvox_application_start(PocketvoxApplication *application)
{
	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	pocketvox_controller_start(priv->controller);

	//say goodbye to the user
	gchar *name = pocketvox_profile_get_name(priv->profile);
	gchar *msg = g_strdup_printf("Goodbye %s", name);
	pocketvox_notifier_say(priv->notifier, msg);
	g_free(msg);

	pocketvox_profile_save(priv->profile);
}

void pocketvox_application_add_module(PocketvoxApplication *application, PocketvoxModule *module)
{
	g_return_if_fail(NULL != application);
	g_return_if_fail(NULL != module);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	pocketvox_controller_add_module(priv->controller, module);
}

void pocketvox_application_remove_module(PocketvoxApplication *application, gchar *id)
{
	g_return_if_fail(NULL != application);
	g_return_if_fail(NULL != id);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	pocketvox_controller_remove_module(priv->controller, id);
}

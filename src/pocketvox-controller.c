#include "pocketvox-controller.h"
#include "pocketvox-chooser.h"
#include "pocketvox-xmanager.h"
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include <libintl.h>
#define _(String) dgettext(GETTEXT_PACKAGE, String)

enum
{
	PROP_0,
};

struct _PocketvoxControllerPrivate
{
	PocketvoxRecognizer *recognizer;
	PocketvoxNotifier	*notifier;
	PocketvoxIndicator 	*indicator;
	PocketvoxXmanager 	*xmanager;
	GHashTable 			*modules;
	GMainLoop			*loop;

	gboolean 			first_launch;
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

	g_hash_table_destroy(priv->modules);

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

static void pocketvox_controller_init (PocketvoxController *controller){
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	priv->recognizer 	= NULL;
	priv->notifier 		= NULL;
	priv->indicator   	= NULL;
	priv->modules		= NULL;
	priv->xmanager		= NULL;
	priv->modules		= g_hash_table_new_full(g_str_hash, g_str_equal, g_free, pocketvox_module_free);

	priv->first_launch	= TRUE;
}

static void pocketvox_controller_state_changer(PocketvoxController *controller, gchar *state, gpointer user_data)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	pocketvox_notifier_notify(priv->notifier,state, NULL);

	if(!g_strcmp0("Run", state))
	{
		if(priv->first_launch == TRUE)
		{
			pocketvox_notifier_say(priv->notifier, _("It's your first launch, I need some time to initialize myself"));
			priv->first_launch = FALSE;
		}

		pocketvox_recognizer_set_state(priv->recognizer, POCKETVOX_STATE_RUN);
	}
	else
	{
		pocketvox_recognizer_set_state(priv->recognizer, POCKETVOX_STATE_STOP);
	}
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

static void pocketvox_controller_set_dict(PocketvoxController *controller, gpointer user_data)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	PocketvoxChooser *chooser = pocketvox_chooser_new(FALSE);
	gchar *path = pocketvox_chooser_get_filepath(chooser);

	g_return_if_fail(NULL != path);

	pocketvox_recognizer_set(priv->recognizer, "dict", g_strdup(path));
}

static void pocketvox_controller_set_lm(PocketvoxController *controller, gpointer user_data)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	PocketvoxChooser *chooser = pocketvox_chooser_new(FALSE);
	gchar *path = pocketvox_chooser_get_filepath(chooser);

	g_return_if_fail(NULL != path);

	pocketvox_recognizer_set(priv->recognizer, "lm", g_strdup(path));
}

static void pocketvox_controller_set_acoustic(PocketvoxController *controller, gpointer user_data)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	PocketvoxChooser *chooser = pocketvox_chooser_new(TRUE);
	gchar *path = pocketvox_chooser_get_filepath(chooser);

	g_return_if_fail(NULL != path);

	pocketvox_recognizer_set(priv->recognizer, "hmm", g_strdup(path));
}

void pocketvox_controller_on_request(PocketvoxController *controller, gpointer hyp, gpointer user_data)
{
	gchar *request = (gchar *)hyp;

	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != hyp);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	gchar* window = pocketvox_xmanager_get_window(priv->xmanager);

	//put modules apps to activated
	g_hash_table_foreach(priv->modules, pocketvox_module_manage_apps, window);

	//make request
	g_hash_table_foreach(priv->modules, pocketvox_module_make_request, request);

	GList* modules = g_hash_table_get_values(priv->modules);
	gint i = 0, j = 0;
	gdouble mindist = -1.0f;
	gdouble dist;

	for(i = 0; i< g_list_length(modules); i++)
	{
		PocketvoxModule *module = g_list_nth_data(modules, i);

		dist = pocketvox_module_get_score(module);

		if(pocketvox_module_get_activated(module) == TRUE && (dist < mindist || i==0 ))
		{
			mindist = dist;
			j = i;
		}
	}

	PocketvoxModule *m = g_list_nth_data(modules, j);

	pocketvox_module_execute(m);

	g_list_free(modules);
}

void pocketvox_controller_set_module_state(PocketvoxController *controller, gpointer data, gpointer user_data)
{
	gchar *id = (gchar *)data;

	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != id );

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	PocketvoxModule *module =(PocketvoxModule *)g_hash_table_lookup(priv->modules, id);

	pocketvox_module_set_activated(module, !pocketvox_module_get_activated(module));
}

static void pocketvox_controller_set_voice(PocketvoxController* controller, gpointer v, gpointer user_data)
{
	gchar *voice = (gchar *)v;
	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != voice);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	pocketvox_notifier_set_voice(priv->notifier, voice);
}

static void pocketvox_controller_waiting(PocketvoxController *controller, gpointer data)
{
    g_return_if_fail(NULL != controller);

 	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

    pocketvox_notifier_say(priv->notifier, _("I'm waiting for your orders"));
}

PocketvoxController* pocketvox_controller_new(PocketvoxRecognizer *recognizer,
											  PocketvoxNotifier	*notifier,
											  PocketvoxIndicator *indicator)
{
	g_return_val_if_fail(NULL != recognizer
					  && NULL != notifier
					  && NULL != indicator,
					  NULL);

	PocketvoxController *controller = (PocketvoxController *)g_object_new(TYPE_POCKETVOX_CONTROLLER, NULL);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	priv->recognizer 	= recognizer;
	priv->notifier 		= notifier;
	priv->indicator 	= indicator;
	priv->xmanager		= pocketvox_xmanager_new();

	//connect all signals
	g_signal_connect_swapped(priv->recognizer,
							"result",
							G_CALLBACK(pocketvox_controller_on_request),
							controller);

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

	g_signal_connect_swapped(priv->indicator,
							"indicator_module_toggled",
							G_CALLBACK(pocketvox_controller_set_module_state),
							controller);

	g_signal_connect_swapped(priv->indicator,
							"indicator_language_toggled",
							G_CALLBACK(pocketvox_controller_set_voice),
							controller);

    g_signal_connect_swapped(priv->recognizer,
                            "waiting",
                            G_CALLBACK(pocketvox_controller_waiting),
                            controller);

	return controller;
}

void pocketvox_controller_start(PocketvoxController *controller)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

    GList *modules = g_hash_table_get_values(priv->modules);
    gint i;

    for(i = 0; i < g_list_length(modules); i++)
    {
        pocketvox_module_build_dictionnary((PocketvoxModule *)g_list_nth_data(modules, i));
    }
    g_list_free(modules);

	priv->loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(priv->loop);
}

void pocketvox_controller_add_module(PocketvoxController *controller, PocketvoxModule *module)
{
	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != module);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	gchar *id = pocketvox_module_get_id(module);

	g_hash_table_insert(priv->modules, g_strdup(id), module);

	if( pocketvox_module_is_apps(module) == FALSE )
	{
		pocketvox_indicator_add_module_item(priv->indicator, id);
	}
	else
	{
		pocketvox_indicator_add_apps_item(priv->indicator, id);
	}
}

void pocketvox_controller_remove_module(PocketvoxController *controller, gchar *id)
{
	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != id);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	g_hash_table_remove(priv->modules, id);
	pocketvox_indicator_remove_module_item(priv->indicator, id);
}

static void pocketvox_controller_write_dictionnary(gpointer key, gpointer value, gpointer user_data)
{
	gchar *filepath = (gchar *)user_data;
	PocketvoxModule *module =(PocketvoxModule *)value;

	gchar *raw = pocketvox_module_get_raw(module);

	//write the raw txt to file
	FILE *fp = fopen(filepath, "a+");
	fprintf(fp, "%s", raw);

	fclose(fp);
	g_free(raw);
}

gint pocketvox_controller_create_custom_lm_file(PocketvoxController *controller)
{
	gint res;

	g_return_val_if_fail(NULL != controller, -1);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	//create a new tmp file
	gchar* filepath = g_strdup("/tmp/pocketvox_raw.txt");
	FILE *fp = fopen(filepath, "w");
	fclose(fp);

	//for each module in the hashtable modules
	g_hash_table_foreach(priv->modules,
						 pocketvox_controller_write_dictionnary,
						 filepath);

	//use cmuclmtk tools to build a lm file
	res = system("text2wfreq < /tmp/pocketvox_raw.txt | wfreq2vocab > /tmp/pocketvox.vocab");
	g_return_val_if_fail(-1 != res, res);

	res = system("text2idngram -vocab /tmp/pocketvox.vocab -idngram /tmp/pocketvox.idngram < /tmp/pocketvox_raw.txt");
	g_return_val_if_fail(-1 != res, res);

	res = system("idngram2lm -vocab_type 0 -idngram /tmp/pocketvox.idngram -vocab /tmp/pocketvox.vocab -arpa /tmp/pocketvox.arpa");
	g_return_val_if_fail(-1 != res, res);

	res = system("sphinx_lm_convert -i /tmp/pocketvox.arpa -o /tmp/pocketvox.lm.dmp");
	g_return_val_if_fail(-1 != res, res);

	return 0;
}

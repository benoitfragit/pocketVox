#include "pocketvox-controller.h"
#include <stdio.h>

enum
{
	PROP_0,
};

struct _PocketvoxControllerPrivate 
{
	PocketvoxRecognizer *recognizer; 
	PocketvoxNotifier	*notifier;
	PocketvoxIndicator 	*indicator;
	GHashTable 			*modules;
	GMainLoop			*loop;
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
	g_object_unref(priv->indicator);
	
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
	
	priv->modules		= g_hash_table_new_full(g_str_hash, g_str_equal, g_free, pocketvox_module_free);
}

static void pocketvox_controller_state_changer(PocketvoxController *controller, gchar *state, gpointer user_data)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;	
	
	if(!g_strcmp0("Run", state))
	{
		pocketvox_recognizer_set_state(priv->recognizer, POCKETVOX_STATE_RUN);
	}	
	else
	{
		pocketvox_recognizer_set_state(priv->recognizer, POCKETVOX_STATE_STOP);
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

void pocketvox_controller_on_request(PocketvoxController *controller, gpointer hyp, gpointer user_data)
{
	gchar *request = (gchar *)hyp;
	
	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != hyp);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;	
	
	g_hash_table_foreach(priv->modules, pocketvox_module_make_request, request);
	
	GList* modules = g_hash_table_get_values(priv->modules);
	gint i;
	gdouble mindist = -1.0f;
	gdouble dist;
	gchar *cmd = NULL;
		
	for(i = 0; i< g_list_length(modules); i++)
	{
		PocketvoxModule *module = g_list_nth_data(modules, i);
		
		if(pocketvox_module_get_activated(module) == TRUE && (dist < mindist || i==0 ))
		{
			mindist = dist;
			cmd = pocketvox_module_get_command(module);
		}
	}
	
	g_warning("%s",cmd);
	
	g_list_free(modules);
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

void pocketvox_controller_add_module(PocketvoxController *controller, PocketvoxModule *module)
{
	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != module);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;
	
	g_hash_table_insert(priv->modules, g_strdup(pocketvox_module_get_id(module)), module);	
}

void pocketvox_controller_remove_module(PocketvoxController *controller, gchar *id)
{
	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != id);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;
	
	g_hash_table_remove(priv->modules, id);
}

void pocketvox_controller_build_lm_file(PocketvoxController *controller)
{
	FILE *file = NULL;
	const gchar filename[] = "/tmp/pockeetvox_lm.txt";
	g_return_if_fail(NULL != controller);
	
	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	//make a string request on all dictionnary and append it to the tmp file
	gchar *str = NULL;
	gint i;
	GList *modules = g_hash_table_get_values(priv->modules);
	
	for(i = 0; i < g_list_length(modules); i++)
	{
		PocketvoxModule* module = g_list_nth_data(modules, i);
		
		if(pocketvox_module_get_activated(module) == TRUE)
		{
			pocketvox_module_get_raw(module, str);
		}
	}
	
	g_list_free(modules);
	
	//write the string to a tmp file 
	file = fopen(filename, "w");
	
	if(file != NULL)
	{
		fprintf(file, "%s", str);
	
		fclose(file);
	}
	
	g_free(str);
}

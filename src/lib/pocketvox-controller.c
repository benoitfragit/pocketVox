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

#include "pocketvox-controller.h"
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
}

static void pocketvox_controller_state_changer(PocketvoxController *controller, gchar *state, gpointer user_data)
{
	g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	//pocketvox_notifier_notify(priv->notifier,state, NULL);

	if(!g_strcmp0("Run", state))
	{
        //listen to the user
		pocketvox_recognizer_set_state(priv->recognizer, POCKETVOX_STATE_RUN);
	}
	else
	{
        //no more audio
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

void pocketvox_controller_on_request(PocketvoxController *controller, gpointer hyp, gpointer user_data)
{
    GList *modules = NULL;
    gchar *request = (gchar *)hyp;
    gint i = 0, j = 0, n_threads;
    gdouble mindist = -1.0f, dist;
    GThreadPool *thread_pool = NULL;
    gboolean first_module = FALSE;

	g_return_if_fail(NULL != controller);
	g_return_if_fail(NULL != hyp);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

	gchar* window = pocketvox_xmanager_get_window(priv->xmanager);

	//put modules apps to activated
	g_hash_table_foreach(priv->modules, pocketvox_module_manage_apps, window);

	//make request
	//g_hash_table_foreach(priv->modules, pocketvox_module_make_request, request);

	modules     = g_hash_table_get_values(priv->modules);

    n_threads   = g_get_num_processors();
    thread_pool = g_thread_pool_new((GFunc)pocketvox_module_threaded_request, request, n_threads, TRUE, NULL );

    for(i = 0; i < g_list_length(modules); i++)
    {
        g_thread_pool_push(thread_pool, (PocketvoxModule *)g_list_nth_data(modules,i), NULL);
    }
    g_thread_pool_free(thread_pool, FALSE, TRUE);

    for(i = 0; i< g_list_length(modules); i++)
	{
		PocketvoxModule *module = g_list_nth_data(modules, i);

		dist = pocketvox_module_get_score(module);

        g_warning("%d %s %d %d %s %.5f",
                  i,
                  pocketvox_module_get_id(module),
                  pocketvox_module_is_apps(module),
                  pocketvox_module_get_activated(module),
                  pocketvox_module_get_command(module),
                  pocketvox_module_get_score(module));

		if(pocketvox_module_get_activated(module) == TRUE && (dist < mindist || first_module==FALSE ))
		{
			mindist = dist;
			j = i;

            first_module = TRUE;
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
							"indicator_module_toggled",
							G_CALLBACK(pocketvox_controller_set_module_state),
							controller);

    g_signal_connect_swapped(priv->recognizer,
                            "waiting",
                            G_CALLBACK(pocketvox_controller_waiting),
                            controller);

	return controller;
}

void pocketvox_controller_start(PocketvoxController *controller)
{
	GList* modules = NULL;
    gint i, n_threads;
    GThreadPool *thread_pool = NULL;

    g_return_if_fail(NULL != controller);

	controller->priv = G_TYPE_INSTANCE_GET_PRIVATE (controller,
			TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerPrivate);
	PocketvoxControllerPrivate *priv = controller->priv;

    modules   = g_hash_table_get_values(priv->modules);

    //create a GThreadPool to make dictionnaries loading smoother
    n_threads   = g_get_num_processors();
    thread_pool = g_thread_pool_new((GFunc)pocketvox_module_build_dictionnary, NULL, n_threads, TRUE, NULL);

    for(i = 0; i < g_list_length(modules); i++)
    {
        g_thread_pool_push(thread_pool, (PocketvoxModule *)g_list_nth_data(modules, i), NULL);
        //pocketvox_module_build_dictionnary((PocketvoxModule *)g_list_nth_data(modules, i));
    }
    g_thread_pool_free(thread_pool, FALSE, TRUE);
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

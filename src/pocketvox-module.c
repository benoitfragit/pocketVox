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

#include "pocketvox-module.h"
#include "pocketvox-dictionnary.h"
#include <stdlib.h>


enum
{
	PROP_0,
	PROP_MODULE_DICT,
	PROP_MODULE_ID,
	PROP_MODULE_CMD,
    PROP_MODULE_APPS
};

struct _PocketvoxModulePrivate
{
	PocketvoxDictionnary *dict;
	gchar *id;
	gchar *cmd;
	gboolean activated;
	gboolean apps;
	gdouble score;
};

G_DEFINE_TYPE (PocketvoxModule, pocketvox_module, G_TYPE_OBJECT);

static void pocketvox_module_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_module_parent_class)->dispose (object);
}

static void pocketvox_module_finalize(GObject *object)
{
	PocketvoxModule *module= POCKETVOX_MODULE(object);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
		TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	if(priv->id != NULL) g_free(priv->id);
	if(priv->cmd != NULL) g_free(priv->cmd);

	//g_object_unref(G_OBJECT(priv->dict));

	G_OBJECT_CLASS (pocketvox_module_parent_class)->finalize (object);
}

static void pocketvox_module_set_property (GObject      *gobject,
														 guint         prop_id,
														 const GValue *value,
														 GParamSpec   *pspec)
{
	PocketvoxModule *module = POCKETVOX_MODULE(gobject);
	PocketvoxModulePrivate *priv = module->priv;

	switch (prop_id)
	{
		case PROP_MODULE_ID:
			priv->id = g_strdup((gchar *)g_value_get_string(value));
			break;
		case PROP_MODULE_DICT:
			priv->dict =  pocketvox_dictionnary_new( (gchar *)g_value_get_string(value),FALSE);
			break;
                case PROP_MODULE_APPS:
                        priv->apps = (gboolean)g_value_get_boolean(value);
		        break;
                default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_module_get_property (GObject    *gobject,
														 guint       prop_id,
														 GValue     *value,
														 GParamSpec *pspec)
{
	PocketvoxModule *module = POCKETVOX_MODULE(gobject);
	PocketvoxModulePrivate *priv = module->priv;

	switch (prop_id)
	{
		case PROP_MODULE_CMD:
			g_value_set_string(value, priv->cmd);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_module_class_init (PocketvoxModuleClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxModulePrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_module_get_property;
	gklass->set_property = pocketvox_module_set_property;
	gklass->dispose      = pocketvox_module_dispose;
	gklass->finalize     = pocketvox_module_finalize;

	GParamSpec *pspec = g_param_spec_string ("id",
		"id",
		"id",
		NULL,
		G_PARAM_READWRITE);
	g_object_class_install_property (gklass, PROP_MODULE_ID, pspec);

	pspec = g_param_spec_string ("dict",
		"dict",
		"dict",
		NULL,
		G_PARAM_READWRITE);
	g_object_class_install_property (gklass, PROP_MODULE_DICT, pspec);

	pspec = g_param_spec_string ("cmd",
		"cmd",
		"cmd",
		NULL,
		G_PARAM_READWRITE);
	g_object_class_install_property (gklass, PROP_MODULE_CMD, pspec);

	pspec = g_param_spec_boolean ("apps",
								  "apps",
								  "apps",
								  FALSE,
								  G_PARAM_READWRITE);

	g_object_class_install_property (gklass, PROP_MODULE_APPS, pspec);
}

static void pocketvox_module_init (PocketvoxModule *module){
	g_return_if_fail(NULL != module);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	priv->id = NULL;
	priv->dict = NULL;
	priv->score = -1;
	priv->cmd = NULL;
	//people will be able do deactivate module individualy
	priv->activated = TRUE;
}

PocketvoxModule* pocketvox_module_new(gchar *id, gchar* path, gboolean loadtfidf)
{
	PocketvoxModule *module = (PocketvoxModule *)g_object_new(TYPE_POCKETVOX_MODULE, NULL);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	priv->id 	= g_strdup(id);

	priv->dict 	= pocketvox_dictionnary_new(path, loadtfidf);

	return module;
}

void pocketvox_module_make_request(gpointer key, gpointer value, gpointer user_data)
{
	PocketvoxModule *module = (PocketvoxModule *)value;
	gchar *request = g_strdup((gchar *)user_data);

	g_return_if_fail(NULL != module);
	g_return_if_fail(NULL != request);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	if(pocketvox_module_get_activated(module) == FALSE)
		goto end;

	priv->score = pocketvox_dictionnary_process_request(priv->dict, request);
	priv->cmd = pocketvox_dictionnary_get_result(priv->dict);

end:
	return;
}

gdouble pocketvox_module_get_score(PocketvoxModule *module)
{
	g_return_val_if_fail(NULL != module,	-1);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	return priv->score;
}

gchar* pocketvox_module_get_id(PocketvoxModule *module)
{
	g_return_val_if_fail(NULL != module,	NULL);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	return priv->id;
}

gboolean pocketvox_module_get_activated(PocketvoxModule *module)
{
	g_return_val_if_fail(NULL != module, FALSE);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	return priv->activated;
}

void pocketvox_module_set_activated(PocketvoxModule *module, gboolean state)
{
	g_return_if_fail(NULL != module);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	priv->activated = state;
}

void pocketvox_module_free(gpointer data)
{
//	g_object_unref(G_OBJECT(data));
}

gchar* pocketvox_module_get_command(PocketvoxModule *module)
{
	g_return_val_if_fail(NULL != module, NULL);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	return priv->cmd;
}

void pocketvox_module_execute(PocketvoxModule *module)
{
	g_return_if_fail(NULL != module);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	if(NULL != POCKETVOX_MODULE_GET_CLASS(module)->execute)
	{
		POCKETVOX_MODULE_GET_CLASS(module)->execute(module);
	}
	else
	{
		gchar* cc = g_strdup_printf("%s &", priv->cmd);

        if(cc != NULL)
        {
		    gint res = system(cc);
		    g_free(cc);
		    if( res == -1) g_error("An error occured when I've executed %s", priv->cmd);
	    }
    }
}

gboolean pocketvox_module_is_apps(PocketvoxModule *module)
{
	g_return_val_if_fail(NULL != module, FALSE);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

	return priv->apps;
}

void pocketvox_module_manage_apps(gpointer key, gpointer value, gpointer user_data)
{
	PocketvoxModule *module = (PocketvoxModule *)value;
	gchar *window = (gchar *)user_data;
	gchar *id = (gchar *)key;

	if(pocketvox_module_is_apps(module) == TRUE)
	{
		if(!g_strcmp0(id, window) == TRUE )
		{
			pocketvox_module_set_activated(module, TRUE);
		}
		else
		{
			pocketvox_module_set_activated(module, FALSE);
		}
	}
}

void pocketvox_module_build_dictionnary(PocketvoxModule *module)
{
	g_return_val_if_fail(NULL != module, FALSE);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;

    pocketvox_dictionnary_tfidf(priv->dict);
}

#include "pocketvox-module.h"
#include "pocketvox-dictionnary.h"

enum
{
	PROP_0,
};

struct _PocketvoxModulePrivate 
{
	PocketvoxDictionnary *dict;	
	gchar *id;
	gchar *cmd;
	gboolean activated;
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
	
	g_object_unref(G_OBJECT(priv->dict));
	
	G_OBJECT_CLASS (pocketvox_module_parent_class)->finalize (object);
}

static void pocketvox_module_set_property (GObject      *gobject,
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

static void pocketvox_module_get_property (GObject    *gobject,
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

static void pocketvox_module_class_init (PocketvoxModuleClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxModulePrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_module_get_property;
	gklass->set_property = pocketvox_module_set_property;
	gklass->dispose      = pocketvox_module_dispose;
	gklass->finalize     = pocketvox_module_finalize;
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
	
	g_return_val_if_fail(NULL != module,	-1);
	g_return_val_if_fail(NULL != request, 	-1);
	
	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;		
	
	priv->score = pocketvox_dictionnary_process_request(priv->dict, request);	
	priv->cmd = g_strdup(request);
	
	g_free(request);
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
	g_object_unref(G_OBJECT(data));
}

gchar* pocketvox_module_get_command(PocketvoxModule *module)
{
	g_return_val_if_fail(NULL != module, NULL);
	
	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;	
	
	return priv->cmd;
}

void pocketvox_module_get_raw(PocketvoxModule *module, gchar *raw)
{
	g_return_if_fail(NULL != module);

	module->priv = G_TYPE_INSTANCE_GET_PRIVATE (module,
			TYPE_POCKETVOX_MODULE, PocketvoxModulePrivate);
	PocketvoxModulePrivate *priv = module->priv;
	
	//get the raw sentence from the dictionnary
	pocketvox_dictionnary_get_raw(priv->dict, raw);	
}

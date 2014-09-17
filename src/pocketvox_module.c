#include "pocketvox_module.h"


G_DEFINE_INTERFACE(PocketvoxModule, pocketvox_module, 0)

static void pocketvox_module_default_init(PocketvoxModuleInterface *g_klass)
{
	//instanciation
}

void pocketvox_module_get_dictionnary(PocketvoxModule *module, gchar *str)
{
	g_return_if_fail(NULL != module);
	
	POCKETVOX_MODULE_GET_INTERFACE(module)->get_dictionnary(module, str);
}

gdouble pocketvox_module_make_request(PocketvoxModule *module, gchar *request)
{
	g_return_val_if_fail(NULL != module, -1.0);
	
	return POCKETVOX_MODULE_GET_INTERFACE(module)->make_request(module, request);
}

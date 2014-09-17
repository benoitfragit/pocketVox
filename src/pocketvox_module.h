#ifndef __POCKETVOX_MODULE_H__
#define __POCKETVOX_MODULE_H__

#include <glib.h>
#include <glib-object.h>
#include "pocketvox_dictionnary.h"

G_BEGIN_DECLS

#define TYPE_POCKETVOX_MODULE                  (pocketvox_module_get_type ())
#define POCKETVOX_MODULE(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_MODULE, PocketvoxModule))
#define IS_POCKETVOX_MODULE(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_MODULE))
#define POCKETVOX_MODULE_GET_INTERFACE(inst)    (G_TYPE_INSTANCE_GET_INTERFACE ((inst), TYPE_POCKETVOX_MODULE, PocketvoxModuleInterface))

typedef struct _PocketvoxModule                 PocketvoxModule;
typedef struct _PocketvoxModuleInterface        PocketvoxModuleInterface;

struct _PocketvoxModuleInterface
{
	/*< private >*/
	GTypeInterface parent_iface;

	void (*get_dictionnary) (PocketvoxModule *module, gchar *str);
	gdouble (*make_request) (PocketvoxModule *module, gchar *request);
};

GType pocketvox_module_get_type (void) G_GNUC_CONST;

void pocketvox_module_get_dictionnary(PocketvoxModule *module, gchar *str);
gdouble pocketvox_module_make_request(PocketvoxModule *module, gchar *request);

G_END_DECLS

#endif

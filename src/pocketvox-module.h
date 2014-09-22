#ifndef __POCKETVOX_MODULE_H__
#define __POCKETVOX_MODULE_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_POCKETVOX_MODULE                  (pocketvox_module_get_type ())
#define POCKETVOX_MODULE(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_MODULE, PocketvoxModule))
#define IS_POCKETVOX_MODULE(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_MODULE))
#define POCKETVOX_MODULE_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_MODULE, PocketvoxModuleClass))
#define IS_POCKETVOX_MODULE_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_MODULE))
#define POCKETVOX_MODULE_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_MODULE, PocketvoxModuleClass))

typedef struct _PocketvoxModule                 PocketvoxModule;
typedef struct _PocketvoxModulePrivate          PocketvoxModulePrivate;
typedef struct _PocketvoxModuleClass            PocketvoxModuleClass;

struct _PocketvoxModule
{
	/*< private >*/
	GInitiallyUnowned parent_instance;

	PocketvoxModulePrivate *priv;
};

struct _PocketvoxModuleClass
{
	/*< private >*/
	GInitiallyUnownedClass parent_class;

	/*public funcs and data here*/
	void (*execute)(PocketvoxModule *module);
	
};

GType pocketvox_module_get_type (void) G_GNUC_CONST;
	
PocketvoxModule* pocketvox_module_new(gchar *id, gchar *path, gboolean loadtfidf);
void pocketvox_module_make_request(gpointer key, gpointer value, gpointer user_data);
gchar* pocketvox_module_get_id(PocketvoxModule *module);
gboolean pocketvox_module_get_activated(PocketvoxModule *module);
void pocketvox_module_set_activated(PocketvoxModule *module, gboolean state);
void pocketvox_module_free(gpointer data);
gdouble pocketvox_module_get_score(PocketvoxModule *module);
gchar* pocketvox_module_get_command(PocketvoxModule *module);
void pocketvox_module_get_raw(PocketvoxModule *module, gchar *raw);

//overwritten by virtual methode
void pocketvox_module_execute(PocketvoxModule *module);
G_END_DECLS

#endif

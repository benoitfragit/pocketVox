#ifndef __POCKETVOX_APPLICATION_H__
#define __POCKETVOX_APPLICATION_H__
#include "pocketvox-module.h"

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_POCKETVOX_APPLICATION                  (pocketvox_application_get_type ())
#define POCKETVOX_APPLICATION(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_APPLICATION, PocketvoxApplication))
#define IS_POCKETVOX_APPLICATION(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_APPLICATION))
#define POCKETVOX_APPLICATION_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationClass))
#define IS_POCKETVOX_APPLICATION_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_APPLICATION))
#define POCKETVOX_APPLICATION_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationClass))

typedef struct _PocketvoxApplication                 PocketvoxApplication;
typedef struct _PocketvoxApplicationPrivate          PocketvoxApplicationPrivate;
typedef struct _PocketvoxApplicationClass            PocketvoxApplicationClass;

struct _PocketvoxApplication
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxApplicationPrivate *priv;
};

struct _PocketvoxApplicationClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_application_get_type (void) G_GNUC_CONST;


PocketvoxApplication* pocketvox_application_new(gchar *acoustic, gchar *lm, gchar *dic);
void pocketvox_application_start(PocketvoxApplication *application);
void pocketvox_application_add_module(PocketvoxApplication *application, PocketvoxModule *module);
void pocketvox_application_remove_module(PocketvoxApplication *application, gchar *id);

G_END_DECLS
#endif

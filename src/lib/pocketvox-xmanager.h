#ifndef __POCKETVOX_XMANAGER_H__
#define __POCKETVOX_XMANAGER_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_POCKETVOX_XMANAGER                  (pocketvox_xmanager_get_type ())
#define POCKETVOX_XMANAGER(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_XMANAGER, PocketvoxXmanager))
#define IS_POCKETVOX_XMANAGER(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_XMANAGER))
#define POCKETVOX_XMANAGER_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_XMANAGER, PocketvoxXmanagerClass))
#define IS_POCKETVOX_XMANAGER_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_XMANAGER))
#define POCKETVOX_XMANAGER_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_XMANAGER, PocketvoxXmanagerClass))

typedef struct _PocketvoxXmanager                 PocketvoxXmanager;
typedef struct _PocketvoxXmanagerPrivate          PocketvoxXmanagerPrivate;
typedef struct _PocketvoxXmanagerClass            PocketvoxXmanagerClass;

struct _PocketvoxXmanager
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxXmanagerPrivate *priv;
};

struct _PocketvoxXmanagerClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_xmanager_get_type (void) G_GNUC_CONST;

PocketvoxXmanager* pocketvox_xmanager_new();
gchar* pocketvox_xmanager_get_window(PocketvoxXmanager *xmanager);

G_END_DECLS
#endif

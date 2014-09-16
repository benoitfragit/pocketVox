#ifndef __POCKETVOX_NOTIFIER_H__
#define __POCKETVOX_NOTIFIER_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_POCKETVOX_NOTIFIER                  (pocketvox_notifier_get_type ())
#define POCKETVOX_NOTIFIER(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifier))
#define IS_POCKETVOX_NOTIFIER(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_NOTIFIER))
#define POCKETVOX_NOTIFIER_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierClass))
#define IS_POCKETVOX_NOTIFIER_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_NOTIFIER))
#define POCKETVOX_NOTIFIER_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierClass))

typedef struct _PocketvoxNotifier                 PocketvoxNotifier;
typedef struct _PocketvoxNotifierPrivate          PocketvoxNotifierPrivate;
typedef struct _PocketvoxNotifierClass            PocketvoxNotifierClass;

struct _PocketvoxNotifier
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxNotifierPrivate *priv;
};

struct _PocketvoxNotifierClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_notifier_get_type (void) G_GNUC_CONST;


PocketvoxNotifier* pocketvox_notifier_new();

void pocketvox_notifier_notify(PocketvoxNotifier *notifier, gpointer hyp, gpointer user_data);

G_END_DECLS
#endif

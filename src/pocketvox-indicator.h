#ifndef __POCKETVOX_INDICATOR_H__
#define __POCKETVOX_INDICATOR_H__

#include <glib.h>
#include <glib-object.h>


G_BEGIN_DECLS

#define TYPE_POCKETVOX_INDICATOR                  (pocketvox_indicator_get_type ())
#define POCKETVOX_INDICATOR(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_INDICATOR, PocketvoxIndicator))
#define IS_POCKETVOX_INDICATOR(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_INDICATOR))
#define POCKETVOX_INDICATOR_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorClass))
#define IS_POCKETVOX_INDICATOR_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_INDICATOR))
#define POCKETVOX_INDICATOR_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_INDICATOR, PocketvoxIndicatorClass))

typedef struct _PocketvoxIndicator                 PocketvoxIndicator;
typedef struct _PocketvoxIndicatorPrivate          PocketvoxIndicatorPrivate;
typedef struct _PocketvoxIndicatorClass            PocketvoxIndicatorClass;

struct _PocketvoxIndicator
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxIndicatorPrivate *priv;
};

struct _PocketvoxIndicatorClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_indicator_get_type (void) G_GNUC_CONST;

PocketvoxIndicator* pocketvox_indicator_new();
void pocketvox_indicator_add_module_item(PocketvoxIndicator *indicator, gchar *id);
void pocketvox_indicator_remove_module_item(PocketvoxIndicator *indicator, gchar *id);

G_END_DECLS

#endif

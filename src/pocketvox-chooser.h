#ifndef __POCKETVOX_CHOOSER_H__
#define __POCKETVOX_CHOOSER_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_POCKETVOX_CHOOSER                  (pocketvox_chooser_get_type ())
#define POCKETVOX_CHOOSER(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_CHOOSER, PocketvoxChooser))
#define IS_POCKETVOX_CHOOSER(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_CHOOSER))
#define POCKETVOX_CHOOSER_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_CHOOSER, PocketvoxChooserClass))
#define IS_POCKETVOX_CHOOSER_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_CHOOSER))
#define POCKETVOX_CHOOSER_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_CHOOSER, PocketvoxChooserClass))

typedef struct _PocketvoxChooser                 PocketvoxChooser;
typedef struct _PocketvoxChooserPrivate          PocketvoxChooserPrivate;
typedef struct _PocketvoxChooserClass            PocketvoxChooserClass;

struct _PocketvoxChooser
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxChooserPrivate *priv;
};

struct _PocketvoxChooserClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_chooser_get_type (void) G_GNUC_CONST;

PocketvoxChooser* pocketvox_chooser_new(gboolean selectFolder);
gchar* pocketvox_chooser_get_filepath(PocketvoxChooser *chooser);


G_END_DECLS
#endif

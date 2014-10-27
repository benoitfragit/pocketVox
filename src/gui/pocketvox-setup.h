#ifndef __POCKETVOX_SETUP_H__
#define __POCKETVOX_SETUP_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_POCKETVOX_SETUP                  (pocketvox_setup_get_type ())
#define POCKETVOX_SETUP(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_SETUP, PocketvoxSetup))
#define IS_POCKETVOX_SETUP(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_SETUP))
#define POCKETVOX_SETUP_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_SETUP, PocketvoxSetupClass))
#define IS_POCKETVOX_SETUP_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_SETUP))
#define POCKETVOX_SETUP_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_SETUP, PocketvoxSetupClass))

typedef struct _PocketvoxSetup                 PocketvoxSetup;
typedef struct _PocketvoxSetupPrivate          PocketvoxSetupPrivate;
typedef struct _PocketvoxSetupClass            PocketvoxSetupClass;

struct _PocketvoxSetup
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxSetupPrivate *priv;
};

struct _PocketvoxSetupClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_setup_get_type (void) G_GNUC_CONST;

PocketvoxSetup* pocketvox_setup_new();
void pocketvox_setup_show(PocketvoxSetup *setup);

G_END_DECLS
#endif

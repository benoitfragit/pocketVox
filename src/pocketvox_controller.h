#ifndef __POCKETVOX_CONTROLLER_H__
#define __POCKETVOX_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include "pocketvox_indicator.h"
#include "pocketvox_recognizer.h"
#include "pocketvox_dictionnary.h"
#include "pocketvox_notifier.h"

G_BEGIN_DECLS

#define TYPE_POCKETVOX_CONTROLLER                  (pocketvox_controller_get_type ())
#define POCKETVOX_CONTROLLER(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_CONTROLLER, PocketvoxController))
#define IS_POCKETVOX_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_CONTROLLER))
#define POCKETVOX_CONTROLLER_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerClass))
#define IS_POCKETVOX_CONTROLLER_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_CONTROLLER))
#define POCKETVOX_CONTROLLER_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_CONTROLLER, PocketvoxControllerClass))

typedef struct _PocketvoxController                 PocketvoxController;
typedef struct _PocketvoxControllerPrivate          PocketvoxControllerPrivate;
typedef struct _PocketvoxControllerClass            PocketvoxControllerClass;

struct _PocketvoxController
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxControllerPrivate *priv;
};

struct _PocketvoxControllerClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_controller_get_type (void) G_GNUC_CONST;


PocketvoxController* pocketvox_controller_new(PocketvoxRecognizer *recognizer, 
											  PocketvoxNotifier	*notifier,
											  PocketvoxDictionnary *dictionnary,
											  PocketvoxIndicator *indicator);

G_END_DECLS


#endif

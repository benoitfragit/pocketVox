#ifndef __POCKETVOX_DICTIONNARY_H__
#define __POCKETVOX_DICTIONNARY_H__


#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_POCKETVOX_DICTIONNARY                  (pocketvox_dictionnary_get_type ())
#define POCKETVOX_DICTIONNARY(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_DICTIONNARY, PocketvoxDictionnary))
#define IS_POCKETVOX_DICTIONNARY(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_DICTIONNARY))
#define POCKETVOX_DICTIONNARY_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_DICTIONNARY, PocketvoxDictionnaryClass))
#define IS_POCKETVOX_DICTIONNARY_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_DICTIONNARY))
#define POCKETVOX_DICTIONNARY_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_DICTIONNARY, PocketvoxDictionnaryClass))

typedef struct _PocketvoxDictionnary                 PocketvoxDictionnary;
typedef struct _PocketvoxDictionnaryPrivate          PocketvoxDictionnaryPrivate;
typedef struct _PocketvoxDictionnaryClass            PocketvoxDictionnaryClass;

struct _PocketvoxDictionnary
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxDictionnaryPrivate *priv;
};

struct _PocketvoxDictionnaryClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_dictionnary_get_type (void) G_GNUC_CONST;

PocketvoxDictionnary* pocketvox_dictionnary_new(gchar *filepath);

void pocketvox_dictionnary_display(PocketvoxDictionnary* dictionnary);

void pocketvox_dictionnary_add_new_request(PocketvoxDictionnary *dictionnary, gchar *request);

G_END_DECLS



#endif

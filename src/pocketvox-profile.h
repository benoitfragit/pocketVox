#ifndef __POCKETVOX_PROFILE_H__
#define __POCKETVOX_PROFILE_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_POCKETVOX_PROFILE                  (pocketvox_profile_get_type ())
#define POCKETVOX_PROFILE(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POCKETVOX_PROFILE, PocketvoxProfile))
#define IS_POCKETVOX_PROFILE(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POCKETVOX_PROFILE))
#define POCKETVOX_PROFILE_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_POCKETVOX_PROFILE, PocketvoxProfileClass))
#define IS_POCKETVOX_PROFILE_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_POCKETVOX_PROFILE))
#define POCKETVOX_PROFILE_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_POCKETVOX_PROFILE, PocketvoxProfileClass))

typedef struct _PocketvoxProfile                 PocketvoxProfile;
typedef struct _PocketvoxProfilePrivate          PocketvoxProfilePrivate;
typedef struct _PocketvoxProfileClass            PocketvoxProfileClass;

struct _PocketvoxProfile
{
  /*< private >*/
  GInitiallyUnowned parent_instance;

  PocketvoxProfilePrivate *priv;
};

struct _PocketvoxProfileClass
{
  /*< private >*/
  GInitiallyUnownedClass parent_class;

  /*public funcs and data here*/
};

GType pocketvox_profile_get_type (void) G_GNUC_CONST;

PocketvoxProfile* pocketvox_profile_new(gchar *path);

void pocketvox_profile_set_name(PocketvoxProfile *profile, gchar *name);
void pocketvox_profile_set_acoustic(PocketvoxProfile *profile, gchar *acoustic);
void pocketvox_profile_set_lm(PocketvoxProfile *profile, gchar *lm);
void pocketvox_profile_set_dict(PocketvoxProfile *profile, gchar *dict);
void pocketvox_profile_set_voice(PocketvoxProfile *profile, gchar *voice);

gchar* pocketvox_profile_get_name(PocketvoxProfile *profile);
gchar* pocketvox_profile_get_acoustic(PocketvoxProfile *profile);
gchar* pocketvox_profile_get_lm(PocketvoxProfile *profile);
gchar* pocketvox_profile_get_dict(PocketvoxProfile *profile);
gchar* pocketvox_profile_get_voice(PocketvoxProfile *profile);
gchar* pocketvox_profile_get_keyword(PocketvoxProfile *profile);
void pocketvox_profile_set_keyword(PocketvoxProfile *profile, gchar *k);

void pocketvox_profile_save(PocketvoxProfile *profile);

GHashTable* pocketvox_profile_get_profile_apps(PocketvoxProfile *profile);
G_END_DECLS
#endif

#include "pocketvox-profile.h"

enum
{
	PROP_0,
};

struct _PocketvoxProfilePrivate 
{
	gchar *path;
	gchar *name;
	gchar *voice;
	gchar *lm;
	gchar *dict;
	gchar *acoustic;
};

G_DEFINE_TYPE (PocketvoxProfile, pocketvox_profile, G_TYPE_OBJECT);

static void pocketvox_profile_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_profile_parent_class)->dispose (object);
}

static void pocketvox_profile_finalize(GObject *object)
{
	PocketvoxProfile *profile= POCKETVOX_PROFILE(object);

	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
		TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;	
		
	g_free(priv->name);
	g_free(priv->voice);
	g_free(priv->lm);
	g_free(priv->dict);
	g_free(priv->acoustic);
	g_free(priv->path);	
		
	G_OBJECT_CLASS (pocketvox_profile_parent_class)->finalize (object);
}

static void pocketvox_profile_set_property (GObject      *gobject,
														 guint         prop_id,
														 const GValue *value,
														 GParamSpec   *pspec)
{
	switch (prop_id)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_profile_get_property (GObject    *gobject,
														 guint       prop_id,
														 GValue     *value,
														 GParamSpec *pspec)
{
	switch (prop_id)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_profile_class_init (PocketvoxProfileClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxProfilePrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_profile_get_property;
	gklass->set_property = pocketvox_profile_set_property;
	gklass->dispose      = pocketvox_profile_dispose;
	gklass->finalize     = pocketvox_profile_finalize;
}

static void pocketvox_profile_init (PocketvoxProfile *profile){
	g_return_if_fail(NULL != profile);

	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;	
	
	priv->name  	= g_strdup("user");
	priv->voice 	= g_strdup("default");
	priv->lm		= NULL;
	priv->dict		= NULL;
	priv->acoustic 	= NULL;
}

PocketvoxProfile* pocketvox_profile_new(gchar *path)
{
	g_return_val_if_fail(NULL != path, NULL);
	g_return_val_if_fail(TRUE == g_file_test(path, G_FILE_TEST_EXISTS), NULL);

	PocketvoxProfile *profile = (PocketvoxProfile *)g_object_new(TYPE_POCKETVOX_PROFILE, NULL);

	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;	
	
	GKeyFile* keyfile;
	GKeyFileFlags flags;
	GError *error = NULL;
	keyfile = g_key_file_new();
	flags = G_KEY_FILE_KEEP_COMMENTS;
	
	priv->path = g_strdup(path);
	
	if(g_key_file_load_from_file (keyfile, path, flags, &error ) )
	{
		priv->name 		= g_key_file_get_string(keyfile, "profile", "name",  NULL);
		priv->voice 	= g_key_file_get_string(keyfile, "profile", "voice", NULL);
		priv->lm		= g_key_file_get_string(keyfile, "profile", "lm",	 NULL);	
		priv->dict  	= g_key_file_get_string(keyfile, "profile", "dict",	 NULL);
		priv->acoustic 	= g_key_file_get_string(keyfile, "profile", "acoustic", NULL);
		
		g_key_file_free(keyfile);
	}
	else
	{
		return NULL;
	}
	
	return profile;																		
}

gchar* pocketvox_profile_get_name(PocketvoxProfile *profile)
{
	g_return_val_if_fail(NULL != profile, "user");
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	return priv->name;
}

void pocketvox_profile_set_name(PocketvoxProfile *profile, gchar *name)
{
	g_return_if_fail(NULL != profile);
	g_return_if_fail(NULL != name);
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	priv->name = g_strdup(name);
}

gchar* pocketvox_profile_get_voice(PocketvoxProfile *profile)
{
	g_return_val_if_fail(NULL != profile, "default");
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	return priv->voice;
}

void pocketvox_profile_set_voice(PocketvoxProfile *profile, gchar *voice)
{
	g_return_if_fail(NULL != profile);
	g_return_if_fail(NULL != voice);
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	priv->voice = g_strdup(voice);
}

gchar* pocketvox_profile_get_lm(PocketvoxProfile *profile)
{
	g_return_val_if_fail(NULL != profile, NULL);
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	return priv->lm;
}

void pocketvox_profile_set_lm(PocketvoxProfile *profile, gchar *lm)
{
	g_return_if_fail(NULL != profile);
	g_return_if_fail(NULL != lm);
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	priv->lm = g_strdup(lm);
}

gchar* pocketvox_profile_get_dict(PocketvoxProfile *profile)
{
	g_return_val_if_fail(NULL != profile,NULL);
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	return priv->dict;
}

void pocketvox_profile_set_dict(PocketvoxProfile *profile, gchar *dict)
{
	g_return_if_fail(NULL != profile);
	g_return_if_fail(NULL != dict);
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	priv->dict = g_strdup(dict);
}

gchar* pocketvox_profile_get_acoustic(PocketvoxProfile *profile)
{
	g_return_val_if_fail(NULL != profile, NULL);
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	return priv->acoustic;
}

void pocketvox_profile_set_acoustic(PocketvoxProfile *profile, gchar *acoustic)
{
	g_return_if_fail(NULL != profile);
	g_return_if_fail(NULL != acoustic);
	
	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;		

	priv->acoustic = g_strdup(acoustic);
}

void pocketvox_profile_save(PocketvoxProfile *profile)
{
	g_return_if_fail(NULL != profile);

	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;	
	
	GKeyFile* keyfile;
    GError *error = NULL;
    gchar *data	= NULL;
    gsize size;
    keyfile = g_key_file_new();
    
    g_key_file_set_string(keyfile, "profile", "name", 		priv->name);
    g_key_file_set_string(keyfile, "profile", "voice", 		priv->voice);
    g_key_file_set_string(keyfile, "profile", "lm",			priv->lm);   
	g_key_file_set_string(keyfile, "profile", "dict",		priv->dict);
	g_key_file_set_string(keyfile, "profile", "acoustic", 	priv->acoustic);

	data = g_key_file_to_data(keyfile, &size, &error);
    g_file_set_contents(priv->path, data, size, &error);
    
    g_free(data);
    g_key_file_free(keyfile);		
}

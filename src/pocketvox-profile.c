/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

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
    gchar *keyword;
    gchar *material;
    gchar *device;
    GHashTable *apps;
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
	g_free(priv->keyword);
    g_free(priv->material);
    g_free(priv->device);

	g_hash_table_destroy(priv->apps);

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

	priv->name  	= NULL;
	priv->voice 	= NULL;
	priv->lm		= NULL;
	priv->dict		= NULL;
	priv->acoustic 	= NULL;
	priv->keyword   = NULL;
    priv->material  = NULL;
    priv->device    = NULL;

	priv->apps = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
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
	    //loading first/personal parameters
		priv->name 		= g_key_file_get_string(keyfile, "profile", "name",     NULL);
		priv->voice 	= g_key_file_get_string(keyfile, "profile", "voice",    NULL);
		priv->lm		= g_key_file_get_string(keyfile, "profile", "lm",	    NULL);
		priv->dict  	= g_key_file_get_string(keyfile, "profile", "dict",	    NULL);
		priv->acoustic 	= g_key_file_get_string(keyfile, "profile", "acoustic", NULL);
		priv->keyword   = g_key_file_get_string(keyfile, "profile", "keyword",  NULL);
        priv->material  = g_key_file_get_string(keyfile, "profile", "material", NULL);
        priv->device    = g_key_file_get_string(keyfile, "profile", "device",   NULL);

        //set the default value if not set in the profile
        if(     (NULL == priv->material )
          || (  !g_strcmp0(priv->material, "gsettingsaudiosrc") == FALSE && !g_strcmp0(priv->material, "alsasrc") == FALSE ) )
        {
            g_warning("BAD parameter: material should be alsasrc or gsettingsaudiosrc");
            priv->material = g_strdup("gsettingsaudiosrc");
        }

        if( (!g_strcmp0(priv->device,"alsasrc" ) == TRUE)
          && (NULL == priv->device))
        {
		    priv->device = g_strdup("hw:0");
        }

        priv->name 	    = priv->name 	  == NULL ? g_strdup(g_get_user_name())   : priv->name;
		priv->voice     = priv->voice 	  == NULL ? g_strdup("default")           : priv->voice;

	    //then load app category app=dict
	    //goal is to associate an destkop app to a dictionnary
	    gchar **groups  = g_key_file_get_groups(keyfile, NULL);

	    while ( *groups != NULL )
	    {
	        if(!g_strcmp0(*groups, "applications"))
	        {
	            gchar **keys = g_key_file_get_keys(keyfile, "applications", NULL, NULL);

	            //browse all keys
	            while( *keys != NULL)
	            {
	                gchar *dict = g_key_file_get_string(keyfile, "applications", *keys, NULL);

	                if( g_file_test(dict, G_FILE_TEST_EXISTS) )
	                {
	                    //add to a hashtable associated apps and dictionnary in order to
	                    //create a module specialized for this app
	                    g_hash_table_insert(priv->apps, g_strdup(*keys), g_strdup(dict));
	                }


	                keys ++;
	            }

	            break;
	        }

	        groups++;
	    }

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
    GList *keys = g_hash_table_get_keys(priv->apps);
    GList *values = g_hash_table_get_values(priv->apps);
	gint i;

    g_key_file_set_string(keyfile, "profile", "name", 		priv->name);
    g_key_file_set_string(keyfile, "profile", "voice", 		priv->voice);
    g_key_file_set_string(keyfile, "profile", "lm",			priv->lm);
	g_key_file_set_string(keyfile, "profile", "dict",		priv->dict);
	g_key_file_set_string(keyfile, "profile", "acoustic", 	priv->acoustic);
    g_key_file_set_string(keyfile, "profile", "keyword",    priv->keyword);
    g_key_file_set_string(keyfile, "profile", "material",   priv->material);

    if(priv->device != NULL)
    {
        g_key_file_set_string(keyfile, "profile", "device",     priv->device);
    }

	for(i = 0; i < g_list_length(keys); i++)
	{
		gchar *id = (gchar *)g_list_nth_data(keys, i);
		gchar *dict = (gchar *)g_list_nth_data(values, i);

		g_key_file_set_string(keyfile, "applications", id, dict);
	}

	g_list_free(keys);
	g_list_free(values);

	data = g_key_file_to_data(keyfile, &size, &error);
    g_file_set_contents(priv->path, data, size, &error);

    g_free(data);
    g_key_file_free(keyfile);
}

GHashTable* pocketvox_profile_get_profile_apps(PocketvoxProfile *profile)
{
	g_return_val_if_fail(NULL != profile, NULL);

	profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
			TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
	PocketvoxProfilePrivate *priv = profile->priv;

	return priv->apps;
}

void pocketvox_profile_set_keyword(PocketvoxProfile *profile, gchar *k)
{
  g_return_if_fail(NULL != profile);
  g_return_if_fail(NULL != k);

  profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
      TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
  PocketvoxProfilePrivate *priv = profile->priv;

  priv->keyword = g_strdup(k);
}

gchar* pocketvox_profile_get_keyword(PocketvoxProfile *profile)
{
  g_return_val_if_fail(NULL != profile, NULL);

  profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
      TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
  PocketvoxProfilePrivate *priv = profile->priv;

  return priv->keyword;
}

gchar* pocketvox_profile_get_material(PocketvoxProfile *profile)
{
  g_return_val_if_fail(NULL != profile, NULL);

  profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
      TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
  PocketvoxProfilePrivate *priv = profile->priv;

  return priv->material;
}

void pocketvox_profile_set_material(PocketvoxProfile *profile, gchar *m)
{
  g_return_val_if_fail(NULL != profile, NULL);

  profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
      TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
  PocketvoxProfilePrivate *priv = profile->priv;

  priv->material = g_strdup(m);
}

gchar* pocketvox_profile_get_device(PocketvoxProfile *profile)
{
  g_return_val_if_fail(NULL != profile, NULL);

  profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
      TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
  PocketvoxProfilePrivate *priv = profile->priv;

  return priv->device;
}

void pocketvox_profile_set_device(PocketvoxProfile *profile, gchar *m)
{
  g_return_val_if_fail(NULL != profile, NULL);

  profile->priv = G_TYPE_INSTANCE_GET_PRIVATE (profile,
      TYPE_POCKETVOX_PROFILE, PocketvoxProfilePrivate);
  PocketvoxProfilePrivate *priv = profile->priv;

  priv->device = g_strdup(m);
}

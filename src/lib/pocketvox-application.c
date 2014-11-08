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

#include "pocketvox-application.h"
#include "pocketvox-recognizer.h"
#include "pocketvox-notifier.h"
#include "pocketvox-indicator.h"
#include "pocketvox-controller.h"
#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gio/gio.h>
#include "config.h"
#include <stdlib.h>
#include <libintl.h>
#define _(String) dgettext(GETTEXT_PACKAGE,String)

enum
{
	PROP_0,
};

struct _PocketvoxApplicationPrivate
{
    PocketvoxIndicator  *indicator;
	PocketvoxNotifier   *notifier;
	PocketvoxRecognizer *recognizer;
	PocketvoxController *controller;

	//new method to acquire user params
	GSettings			*settings;
};

G_DEFINE_TYPE (PocketvoxApplication, pocketvox_application, G_TYPE_OBJECT);

static void pocketvox_application_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_application_parent_class)->dispose (object);
}

static void pocketvox_application_finalize(GObject *object)
{
    PocketvoxApplication *application = POCKETVOX_APPLICATION(object);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

    //unref all objects
    g_object_unref(priv->recognizer);
    g_object_unref(priv->indicator);
    g_object_unref(priv->controller);
    g_object_unref(priv->notifier);

    //This object will be destroyed when the reference count will reach 0
    G_OBJECT_CLASS (pocketvox_application_parent_class)->finalize (object);
}

static void pocketvox_application_set_property (GObject      *gobject,
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

static void pocketvox_application_get_property (GObject    *gobject,
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

static void pocketvox_application_class_init (PocketvoxApplicationClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxApplicationPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_application_get_property;
	gklass->set_property = pocketvox_application_set_property;
	gklass->dispose      = pocketvox_application_dispose;
	gklass->finalize     = pocketvox_application_finalize;
}

static void pocketvox_application_init (PocketvoxApplication *application){
	g_return_if_fail(NULL != application);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	priv->recognizer 	= NULL;
	priv->notifier 		= NULL;
	priv->controller 	= NULL;
	priv->indicator 	= NULL;

	priv->settings 		= g_settings_new("org.pocketvox.config");
}

static void pocketvox_application_add_profile_module(PocketvoxApplication *application, gchar* id, gchar* path, gboolean isapp)
{
    PocketvoxModule* module = pocketvox_module_new(g_strdup(id), g_strdup(path), FALSE);

    //set the module ModuleProfile to TRUE :: TODO
    g_object_set(G_OBJECT(module), "apps", isapp, NULL);

    //add the module to the application
    pocketvox_application_add_module( application, module);
}

PocketvoxApplication* pocketvox_application_new()
{
	PocketvoxApplication *application = (PocketvoxApplication *)g_object_new(TYPE_POCKETVOX_APPLICATION, NULL);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	bindtextdomain (GETTEXT_PACKAGE, PROGRAMNAME_LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

	gtk_init(NULL, NULL);
	gst_init(NULL, NULL);

	//access gettings parameters
	gchar *name 		= g_settings_get_string(priv->settings, "name");
	gchar *voice		= g_settings_get_string(priv->settings, "voice");

    gchar *lm			= g_settings_get_string(priv->settings, "lm");
	gchar *dic			= g_settings_get_string(priv->settings, "dict");
	gchar *acoustic		= g_settings_get_string(priv->settings, "hmm");

    gchar *keyword      = g_settings_get_string(priv->settings, "keyword");

    gchar *material     = g_settings_get_string(priv->settings, "source");
    gchar *device       = g_settings_get_string(priv->settings, "device");

    gchar *host         = g_settings_get_string(priv->settings, "host");
    gint  port          = atoi(g_settings_get_string(priv->settings, "port"));

    gboolean visual     = g_settings_get_boolean(priv->settings, "visual-notification");
    gboolean sound      = g_settings_get_boolean(priv->settings, "sound-notification");

	GVariant *modules;
	GVariantIter *iter;
	gchar *key, *value;
	gboolean isapps;

	priv->indicator 	= pocketvox_indicator_new();
	priv->notifier 		= pocketvox_notifier_new(name, voice);
	priv->recognizer 	= pocketvox_recognizer_new(acoustic, lm, dic, keyword, material, device, host, port);

    if(priv->indicator  == NULL
    || priv->notifier   == NULL
    || priv->recognizer == NULL)
    {
        g_error("A pocketvox component couldn't be created:\n%s %p\n%s %p\n%s %p",
                 "PocketvoxIndicator",priv->indicator,
                 "PocketvoxNotifier", priv->notifier,
                 "PocketvoxRecognizer", priv->recognizer);
    }

    g_object_set(G_OBJECT(priv->notifier),
                  "visual_notification", visual,
                  "sound_notification",  sound,
                  NULL);

	priv->controller = pocketvox_controller_new(priv->recognizer, priv->notifier, priv->indicator);

    if(priv->controller == NULL)
    {
        g_error("PocketvoxController couldn't be created");
    }

    //only need to connect signals
	modules = g_settings_get_value(priv->settings, "list-apps");

	//get the content of the GVariant
	g_variant_get(modules, "a(ssb)", &iter);

	//loop other all apps
	while(g_variant_iter_loop(iter, "(ssb)", &key, &value, &isapps))
	{
		pocketvox_application_add_profile_module(application, key, value, isapps);
	}

	g_variant_iter_free(iter);

	//a little startup msg
	gchar *startup = g_strdup_printf(_("Hello %s, I'm listening you, I need a few seconds to initialize myself"), name);
	pocketvox_notifier_say(priv->notifier, startup);
	g_free(startup);

	return application;
}

void pocketvox_application_start(PocketvoxApplication *application)
{
    application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	pocketvox_controller_start(priv->controller);
}

void pocketvox_application_add_module(PocketvoxApplication *application, PocketvoxModule *module)
{
	g_return_if_fail(NULL != application);
	g_return_if_fail(NULL != module);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	pocketvox_controller_add_module(priv->controller, module);
}

void pocketvox_application_remove_module(PocketvoxApplication *application, gchar *id)
{
	g_return_if_fail(NULL != application);
	g_return_if_fail(NULL != id);

	application->priv = G_TYPE_INSTANCE_GET_PRIVATE (application,
			TYPE_POCKETVOX_APPLICATION, PocketvoxApplicationPrivate);
	PocketvoxApplicationPrivate *priv = application->priv;

	pocketvox_controller_remove_module(priv->controller, id);
}

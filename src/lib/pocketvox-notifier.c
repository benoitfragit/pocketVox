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

#include "pocketvox-notifier.h"
#include <libnotify/notify.h>

#include "config.h"
#include <string.h>

#ifdef HAVE_LIBESPEAK
	#include <espeak/speak_lib.h>
#endif

enum
{
	PROP_0,
    PROP_SOUND_NOTIFICATION,
    PROP_VISUAL_NOTIFICATION
};

/**
* SECTION: PocketvoxNotifier
* @short_description: A PocketvoxNotifier.
* @include: pocketvox-notifier.h
*
* The #PocketvoxNotifier will send popup and sound notificatins to
* the user. Sound notifications are managed using Espeak library
*/
G_DEFINE_TYPE (PocketvoxNotifier, pocketvox_notifier, G_TYPE_OBJECT);

/**
 * PocketvoxNotifier:
 * @msg: a message.
 * @voice: the voice to use.
 * @name: the user name.
 *
 * a structure to store information
 */
struct _PocketvoxNotifierPrivate
{
	gchar *msg;
	gchar *voice;
	gchar *name;

    gboolean visual_notification;
    gboolean sound_notification;
};

static void pocketvox_notifier_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_notifier_parent_class)->dispose (object);
}

static void pocketvox_notifier_finalize(GObject *object)
{
	PocketvoxNotifier *notifier= POCKETVOX_NOTIFIER(object);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
		TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;

	if(priv->msg    != NULL) g_free(priv->msg);
	if(priv->voice  != NULL) g_free(priv->voice);
	if(priv->name   != NULL) g_free(priv->name);

	notify_uninit();

	G_OBJECT_CLASS (pocketvox_notifier_parent_class)->finalize (object);
}

static void pocketvox_notifier_set_property (GObject      *gobject,
														 guint         prop_id,
														 const GValue *value,
														 GParamSpec   *pspec)
{
	PocketvoxNotifier *notifier = POCKETVOX_NOTIFIER(gobject);
    PocketvoxNotifierPrivate *priv = notifier->priv;

    switch (prop_id)
	{
        case PROP_SOUND_NOTIFICATION:
            priv->sound_notification  = g_value_get_boolean(value);
            break;
        case PROP_VISUAL_NOTIFICATION:
            priv->visual_notification = g_value_get_boolean(value);
		    break;
        default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_notifier_get_property (GObject    *gobject,
														 guint       prop_id,
														 GValue     *value,
														 GParamSpec *pspec)
{
	PocketvoxNotifier *notifier = POCKETVOX_NOTIFIER(gobject);
    PocketvoxNotifierPrivate *priv = notifier->priv;

    switch (prop_id)
	{
        case PROP_VISUAL_NOTIFICATION:
            g_value_set_boolean(value, priv->visual_notification);
            break;
        case PROP_SOUND_NOTIFICATION:
            g_value_set_boolean(value, priv->sound_notification);
            break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_notifier_class_init (PocketvoxNotifierClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxNotifierPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_notifier_get_property;
	gklass->set_property = pocketvox_notifier_set_property;
	gklass->dispose      = pocketvox_notifier_dispose;
	gklass->finalize     = pocketvox_notifier_finalize;

    GParamSpec *pspec = g_param_spec_boolean ("visual_notification",
		"visual_notification",
		"visual_notification",
		TRUE,
		G_PARAM_READWRITE);
	g_object_class_install_property (gklass, PROP_VISUAL_NOTIFICATION, pspec);

    pspec = g_param_spec_boolean ("sound_notification",
		"sound_notification",
		"sound_notification",
		TRUE,
		G_PARAM_READWRITE);
	g_object_class_install_property (gklass, PROP_SOUND_NOTIFICATION, pspec);
}

static void pocketvox_notifier_init (PocketvoxNotifier *notifier){
	g_return_if_fail(NULL != notifier);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;

	priv->msg = NULL;

	notify_init ("PocketVox");

	priv->voice = g_strdup("default");
	priv->name = g_strdup("user");

    priv->visual_notification = TRUE;
    priv->sound_notification  = TRUE;

#ifdef HAVE_LIBESPEAK
	espeak_AUDIO_OUTPUT output;
	gint Buflength = 50;
	gint Options=0;
	gchar *path = NULL;

    espeak_Initialize(output, Buflength, path, Options );
    espeak_SetVoiceByName(priv->voice);
#endif
}

static void pocketvox_notifier_bubble_notification(PocketvoxNotifier*notifier)
{
	g_return_if_fail(NULL != notifier);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;

	g_return_if_fail(NULL != priv->msg);

    if(priv->visual_notification == TRUE)
    {
    	NotifyNotification * Bubble = notify_notification_new ("PocketVox", priv->msg, "dialog-information");

        notify_notification_show (Bubble, NULL);
        g_object_unref(G_OBJECT(Bubble));
    }
}

#ifdef HAVE_LIBESPEAK
static void pocketvox_notifier_sound_notification(PocketvoxNotifier *notifier)
{
	g_return_if_fail(NULL != notifier);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;

    if(priv->sound_notification == TRUE)
    {
	    guint Size = strlen(priv->msg) + 1;
        guint position=0, end_position=0, flags=espeakCHARS_AUTO, *unique_identifier;
        espeak_POSITION_TYPE position_type = POS_CHARACTER;
        void* user_data = NULL;

        espeak_Synth( priv->msg, Size, position, position_type, end_position, flags,
            unique_identifier, user_data );
        espeak_Synchronize( );
    }
}
#endif

/**
 * pocketvox_notifier_say:
 * @notifier: The #PocketvoxNotifier to use.
 * @msg: a message to notify to the user.
 *
 * Give a sound notification to the user
 *
 */
void pocketvox_notifier_say(PocketvoxNotifier *notifier, gchar *msg)
{
	g_return_if_fail(NULL != msg);
	g_return_if_fail(NULL != notifier);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;

	priv->msg = g_strdup(msg);

	pocketvox_notifier_sound_notification(notifier);
}


/**
 * pocketvox_notifier_notify:
 * @notifier: The #PocketvoxNotifier to use.
 * @hyp: The text to pass to the notifier.
 * @user_data: user data.
 *
 * Notify the user about important events with visual and sound notifications
 *
 */
void pocketvox_notifier_notify(PocketvoxNotifier *notifier, gpointer hyp, gpointer user_data)
{
	g_return_if_fail(notifier != NULL);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;


	if (priv->msg != NULL) g_free(priv->msg);

	priv->msg = g_strdup((gchar *)hyp);

	pocketvox_notifier_bubble_notification(notifier);

#ifdef HAVE_LIBESPEAK
	pocketvox_notifier_sound_notification(notifier);
#endif
}

/**
 * pocketvox_notifier_new:
 * @name  : the name of the user.
 * @voice : espeak's voice to use.
 *
 * Create a new PocketvoxNotifier
 *
 * Returns: the new PocketvoxNotifier.
 */
PocketvoxNotifier* pocketvox_notifier_new(gchar *name, gchar *voice)
{
	PocketvoxNotifier *notifier = (PocketvoxNotifier *)g_object_new(TYPE_POCKETVOX_NOTIFIER, NULL);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;

	priv->name 	= g_strdup(name);
	priv->voice = g_strdup(voice);

#ifdef HAVE_LIBESPEAK
    espeak_SetVoiceByName(priv->voice);
#endif

	return notifier;
}


/**
 * pocketvox_notifier_set_voice:
 * @notifier: The #PocketvoxNotifier to modify.
 * @voice: a gchar* representing an Espeak voice.
 *
 * Set the Espeak's voice of the notifier
 * be sure that the selected language is present on your system
 *
 */
void pocketvox_notifier_set_voice(PocketvoxNotifier *notifier, gchar *voice)
{
	g_return_if_fail(notifier 	!= NULL);
	g_return_if_fail(voice 		!= NULL);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;

	priv->voice = g_strdup(voice);
#ifdef HAVE_LIBESPEAK
    espeak_SetVoiceByName(priv->voice);
#endif
}

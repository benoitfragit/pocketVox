#include "pocketvox_notifier.h"
#include <libnotify/notify.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum
{
	PROP_0,
};

struct _PocketvoxNotifierPrivate 
{
	gchar *msg;
};

G_DEFINE_TYPE (PocketvoxNotifier, pocketvox_notifier, G_TYPE_OBJECT);

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
	
	if(priv->msg != NULL) g_free(priv->msg);
	
	notify_uninit();
	
	G_OBJECT_CLASS (pocketvox_notifier_parent_class)->finalize (object);
}

static void pocketvox_notifier_set_property (GObject      *gobject,
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

static void pocketvox_notifier_get_property (GObject    *gobject,
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

static void pocketvox_notifier_class_init (PocketvoxNotifierClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxNotifierPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_notifier_get_property;
	gklass->set_property = pocketvox_notifier_set_property;
	gklass->dispose      = pocketvox_notifier_dispose;
	gklass->finalize     = pocketvox_notifier_finalize;
}

/** Initialisation de l'objet */
static void pocketvox_notifier_init (PocketvoxNotifier *notifier){
	g_return_if_fail(NULL != notifier);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;	
	
	priv->msg = NULL;
	
	notify_init ("PocketVox");
}

static void pocketvox_notifier_sound_notification(PocketvoxNotifier* notifier)
{
	g_return_if_fail(NULL != notifier);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;	
	
	g_return_if_fail(NULL != priv->msg);
	
	pid_t pid;
	int status;
 
	pid = fork();
	if (pid < 0) {
		perror("fork");
		goto end;
	}
 
	if (pid == 0) {
		execlp("espeak", "espeak", priv->msg, (void*)0);
		perror("espeak");
		goto end;
	}
 
	waitpid(pid, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		goto end;

end:
	return;
}

static void pocketvox_notifier_bubble_notification(PocketvoxNotifier*notifier)
{
	g_return_if_fail(NULL != notifier);

	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;
	
	g_return_if_fail(NULL != priv->msg);
	
	NotifyNotification * Bubble = notify_notification_new ("PocketVox", priv->msg, "dialog-information");
	notify_notification_show (Bubble, NULL);
	g_object_unref(G_OBJECT(Bubble));	
}

void pocketvox_notifier_notify(PocketvoxNotifier *notifier, gpointer hyp, gpointer user_data)
{
	notifier->priv = G_TYPE_INSTANCE_GET_PRIVATE (notifier,
			TYPE_POCKETVOX_NOTIFIER, PocketvoxNotifierPrivate);
	PocketvoxNotifierPrivate *priv = notifier->priv;	
	
	
	if (priv->msg != NULL) g_free(priv->msg);
	
	priv->msg = g_strdup((gchar *)hyp);
	
	pocketvox_notifier_bubble_notification(notifier);
	pocketvox_notifier_sound_notification(notifier);
}

PocketvoxNotifier* pocketvox_notifier_new()
{
	PocketvoxNotifier *notifier = (PocketvoxNotifier *)g_object_new(TYPE_POCKETVOX_NOTIFIER, NULL);
	
	return notifier;																		
}

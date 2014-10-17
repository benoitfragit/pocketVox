#include "pocketvox-chooser.h"
#include <gtk/gtk.h>


#include "config.h"

#include <libintl.h>
#define _(String) dgettext(GETTEXT_PACKAGE, String)

enum
{
	PROP_0,
};

struct _PocketvoxChooserPrivate
{
	gchar *filepath;
};

G_DEFINE_TYPE (PocketvoxChooser, pocketvox_chooser, G_TYPE_OBJECT);

static void pocketvox_chooser_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_chooser_parent_class)->dispose (object);
}

static void pocketvox_chooser_finalize(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_chooser_parent_class)->finalize (object);
}

static void pocketvox_chooser_set_property (GObject      *gobject,
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

static void pocketvox_chooser_get_property (GObject    *gobject,
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

static void pocketvox_chooser_class_init (PocketvoxChooserClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxChooserPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_chooser_get_property;
	gklass->set_property = pocketvox_chooser_set_property;
	gklass->dispose      = pocketvox_chooser_dispose;
	gklass->finalize     = pocketvox_chooser_finalize;
}

static void pocketvox_chooser_init (PocketvoxChooser *chooser)
{
	g_return_if_fail(NULL != chooser);

	chooser->priv = G_TYPE_INSTANCE_GET_PRIVATE (chooser,
			TYPE_POCKETVOX_CHOOSER, PocketvoxChooserPrivate);
	PocketvoxChooserPrivate *priv = chooser->priv;

	priv->filepath = NULL;
}

PocketvoxChooser* pocketvox_chooser_new(gboolean selectFolder)
{
	PocketvoxChooser *chooser = (PocketvoxChooser *)g_object_new(TYPE_POCKETVOX_CHOOSER, NULL);

	chooser->priv = G_TYPE_INSTANCE_GET_PRIVATE (chooser,
			TYPE_POCKETVOX_CHOOSER, PocketvoxChooserPrivate);
	PocketvoxChooserPrivate *priv = chooser->priv;

	//creating the dialog
	GtkWidget * dialog = gtk_file_chooser_dialog_new(_("Choose a file or folder"),
												NULL,
												selectFolder ? GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER : GTK_FILE_CHOOSER_ACTION_OPEN,
												"_Cancel", GTK_RESPONSE_CANCEL,
												"_Open", GTK_RESPONSE_ACCEPT,
												NULL);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		priv->filepath = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	}

	gtk_widget_destroy (dialog);

	return chooser;
}

gchar* pocketvox_chooser_get_filepath(PocketvoxChooser *chooser)
{
	g_return_val_if_fail(NULL != chooser, NULL);

	chooser->priv = G_TYPE_INSTANCE_GET_PRIVATE (chooser,
			TYPE_POCKETVOX_CHOOSER, PocketvoxChooserPrivate);
	PocketvoxChooserPrivate *priv = chooser->priv;

	return priv->filepath;
}

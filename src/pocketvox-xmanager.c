#include "pocketvox-xmanager.h"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>


enum
{
	PROP_0,
};

G_DEFINE_TYPE (PocketvoxXmanager, pocketvox_xmanager, G_TYPE_OBJECT);

struct _PocketvoxXmanagerPrivate
{
	Display *display;
	Window root;
	Atom netactivewindow;
};

static void pocketvox_xmanager_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_xmanager_parent_class)->dispose (object);
}

static void pocketvox_xmanager_finalize(GObject *object)
{
	PocketvoxXmanager *xmanager= POCKETVOX_XMANAGER(object);

	xmanager->priv = G_TYPE_INSTANCE_GET_PRIVATE (xmanager,
		TYPE_POCKETVOX_XMANAGER, PocketvoxXmanagerPrivate);
	PocketvoxXmanagerPrivate *priv = xmanager->priv;

	XSync(priv->display, False);
	XCloseDisplay(priv->display);

	G_OBJECT_CLASS (pocketvox_xmanager_parent_class)->finalize (object);
}

static void pocketvox_xmanager_set_property (GObject      *gobject,
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

static void pocketvox_xmanager_get_property (GObject    *gobject,
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

static void pocketvox_xmanager_class_init (PocketvoxXmanagerClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxXmanagerPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_xmanager_get_property;
	gklass->set_property = pocketvox_xmanager_set_property;
	gklass->dispose      = pocketvox_xmanager_dispose;
	gklass->finalize     = pocketvox_xmanager_finalize;
}

static void pocketvox_xmanager_init (PocketvoxXmanager *xmanager){}

PocketvoxXmanager* pocketvox_xmanager_new()
{
	PocketvoxXmanager *xmanager = (PocketvoxXmanager *)g_object_new(TYPE_POCKETVOX_XMANAGER, NULL);

	xmanager->priv = G_TYPE_INSTANCE_GET_PRIVATE (xmanager,
			TYPE_POCKETVOX_XMANAGER, PocketvoxXmanagerPrivate);
	PocketvoxXmanagerPrivate *priv = xmanager->priv;

	priv->display = XOpenDisplay(0);
	priv->root = XDefaultRootWindow(priv->display);
  	priv->netactivewindow = XInternAtom(priv->display, "_NET_ACTIVE_WINDOW", False);

	return xmanager;
}

gchar* pocketvox_xmanager_get_window(PocketvoxXmanager *xmanager)
{
	g_return_val_if_fail(NULL != xmanager, NULL);

	xmanager->priv = G_TYPE_INSTANCE_GET_PRIVATE (xmanager,
			TYPE_POCKETVOX_XMANAGER, PocketvoxXmanagerPrivate);
	PocketvoxXmanagerPrivate *priv = xmanager->priv;

	gint format;
	unsigned long extra, n, window;
	unsigned char *data;
	Atom real;

	XGetWindowProperty(priv->display,
					   priv->root,
					   priv->netactivewindow,
					   0, ~0, False,
                       AnyPropertyType, &real, &format, &n, &extra,
                       &data) ;

	window = *(unsigned long *) data;
	XFree (data);

	XGetWindowProperty(priv->display, (Window) window, XA_WM_CLASS, 0, ~0, False,
                        AnyPropertyType, &real, &format, &n, &extra,
                        &data);

	return g_strdup((gchar *)data);
}

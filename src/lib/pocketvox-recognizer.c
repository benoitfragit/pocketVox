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

#include "pocketvox-recognizer.h"
#include <pocketsphinx.h>

enum
{
	SIGNAL_RESULT,
    SIGNAL_WAITING,
	LAST_SIGNAL
};

enum
{
	PROP_0,
	PROP_HMM,
	PROP_LM,
	PROP_DIC
};

struct _PocketvoxRecognizerPrivate
{
	gchar* lm;
	gchar* dic;
	gchar* hmm;
	PocketvoxState state;
    GstElement *pipeline;
    gchar *keyword;
    gboolean waiting;
};

G_DEFINE_TYPE (PocketvoxRecognizer, pocketvox_recognizer, G_TYPE_OBJECT);

static guint pocketvox_recognizer_signals[LAST_SIGNAL];

static void pocketvox_recognizer_dispose(GObject *object)
{
	G_OBJECT_CLASS (pocketvox_recognizer_parent_class)->dispose (object);
}

static void pocketvox_recognizer_finalize(GObject *object)
{
	PocketvoxRecognizer *recognizer = POCKETVOX_RECOGNIZER(object);

	recognizer->priv = G_TYPE_INSTANCE_GET_PRIVATE (recognizer,
		TYPE_POCKETVOX_RECOGNIZER, PocketvoxRecognizerPrivate);
	PocketvoxRecognizerPrivate *priv = recognizer->priv;

	g_free(priv->hmm);
	g_free(priv->dic);
	g_free(priv->lm);
    g_free(priv->keyword);

	G_OBJECT_CLASS (pocketvox_recognizer_parent_class)->finalize (object);
}

static void pocketvox_recognizer_set_property (GObject      *gobject,
														 guint         prop_id,
														 const GValue *value,
														 GParamSpec   *pspec)
{
	PocketvoxRecognizer *recognizer = POCKETVOX_RECOGNIZER(gobject);
	PocketvoxRecognizerPrivate *priv = recognizer->priv;

	switch (prop_id)
	{
		case PROP_HMM:
			priv->hmm = g_strdup((gchar *)g_value_get_string(value));
			break;
		case PROP_LM:
			priv->lm  = g_strdup((gchar *)g_value_get_string(value));
			break;
		case PROP_DIC:
			priv->dic = g_strdup((gchar *)g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
		break;
	}
}

static void pocketvox_recognizer_get_property (GObject    *gobject,
														 guint       prop_id,
														 GValue     *value,
														 GParamSpec *pspec)
{
	PocketvoxRecognizer *recognizer = POCKETVOX_RECOGNIZER(gobject);
	PocketvoxRecognizerPrivate *priv = recognizer->priv;

	switch (prop_id)
	{
		case PROP_HMM:
			g_value_set_string(value, priv->hmm);
			break;
		case PROP_LM:
			g_value_set_string(value, priv->lm);
			break;
		case PROP_DIC:
			g_value_set_string(value, priv->dic);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
			break;
	}
}

static void pocketvox_recognizer_class_init (PocketvoxRecognizerClass *klass)
{
	g_type_class_add_private (klass, sizeof (PocketvoxRecognizerPrivate));

	GObjectClass *gklass = G_OBJECT_CLASS(klass);
	gklass->get_property = pocketvox_recognizer_get_property;
	gklass->set_property = pocketvox_recognizer_set_property;
	gklass->dispose      = pocketvox_recognizer_dispose;
	gklass->finalize     = pocketvox_recognizer_finalize;

	GParamSpec *pspec = g_param_spec_string ("hmm",
		"hmm",
		"hmm",
		NULL,
		G_PARAM_READWRITE);
	g_object_class_install_property (gklass, PROP_HMM, pspec);

	pspec = g_param_spec_string("lm",
		"lm",
		"lm",
		NULL,
		G_PARAM_READWRITE);
	g_object_class_install_property(gklass, PROP_LM, pspec);

	pspec = g_param_spec_string("dic",
		"dic",
		"dic",
		NULL,
		G_PARAM_READWRITE);
	g_object_class_install_property(gklass, PROP_DIC, pspec);

    pocketvox_recognizer_signals[SIGNAL_RESULT] =
        g_signal_new("result",
                     G_TYPE_FROM_CLASS(klass),
                     G_SIGNAL_RUN_LAST,
                     0,
                     NULL, NULL,
                     g_cclosure_marshal_VOID__POINTER,
                     G_TYPE_NONE,
                     1, G_TYPE_STRING
            );

    pocketvox_recognizer_signals[SIGNAL_WAITING] =
        g_signal_new("waiting",
                    G_TYPE_FROM_CLASS(klass),
                    G_SIGNAL_RUN_LAST,
                    0,
                    NULL, NULL,
                    g_cclosure_marshal_VOID__POINTER,
                    G_TYPE_NONE,
                    0
        );
}


static void pocketvox_recognizer_init (PocketvoxRecognizer *recognizer)
{
	g_return_if_fail(NULL != recognizer);

	recognizer->priv = G_TYPE_INSTANCE_GET_PRIVATE (recognizer,
			TYPE_POCKETVOX_RECOGNIZER, PocketvoxRecognizerPrivate);
	PocketvoxRecognizerPrivate *priv = recognizer->priv;

	priv->hmm     = NULL;
	priv->lm 	  = NULL;
    priv->dic     = NULL;
	priv->state   = POCKETVOX_STATE_RUN;
    priv->keyword = NULL;
    priv->waiting = TRUE;
    priv->pipeline = NULL;
}


static void pocketvox_recognizer_process_result(GstElement* sphinx, gchar *hyp, gchar* uttid, gpointer data)
{
	GstStructure *stt = gst_structure_empty_new("result");
	GValue hypv = G_VALUE_INIT, uttidv = G_VALUE_INIT;
	GstMessage *msg;

	g_value_init(&hypv, G_TYPE_STRING);
	g_value_init(&uttidv, G_TYPE_STRING);

	g_value_set_string(&hypv, g_strdup(hyp));
	g_value_set_string(&uttidv, g_strdup(uttid));

	gst_structure_set_value(stt,"hyp",	&hypv);
	gst_structure_set_value(stt,"uttid",&uttidv);

	msg = gst_message_new_custom( GST_MESSAGE_APPLICATION, GST_OBJECT(sphinx), stt);
	gst_element_post_message(sphinx, msg);
}

static void pocketvox_recognizer_parse_bus_message(GstBus *bus, GstMessage *msg, gpointer user_data)
{
	PocketvoxRecognizer *recognizer = (PocketvoxRecognizer *)user_data;
	GstStructure *stt = msg->structure;

	if(!g_strcmp0(gst_structure_get_name(stt), "result"))
	{
		//get values
		gchar* hyp = (gchar *)g_value_get_string(gst_structure_get_value(stt, "hyp"));
		gchar* uttid = (gchar *)g_value_get_string(gst_structure_get_value(stt, "uttid"));

		g_warning("RESULT %s %s", hyp , uttid);

		g_return_if_fail(NULL != recognizer);

        recognizer->priv = G_TYPE_INSTANCE_GET_PRIVATE (recognizer,
            TYPE_POCKETVOX_RECOGNIZER, PocketvoxRecognizerPrivate);
        PocketvoxRecognizerPrivate *priv = recognizer->priv;

        if(priv->keyword == NULL)
        {
            //if the keyword is not available then emit the signal
            g_signal_emit(recognizer, pocketvox_recognizer_signals[SIGNAL_RESULT],
                0, hyp);
        }
        else
        {
            if(priv->waiting == FALSE)
            {
                g_signal_emit(recognizer, pocketvox_recognizer_signals[SIGNAL_RESULT],
                    0, hyp);

                priv->waiting = TRUE;
            }
            else
            {
                gboolean state = priv->waiting;

                g_warning("%s", hyp);
                priv->waiting = g_strcmp0(hyp, priv->keyword);

                //start a sound notification when the user should speak
                if(state != priv->waiting && priv->waiting == FALSE)
                {
                    g_signal_emit(recognizer, pocketvox_recognizer_signals[SIGNAL_WAITING],
                        0);
                }
            }
        }
    }
}

PocketvoxRecognizer* pocketvox_recognizer_new(gchar* hmm, gchar* lm, gchar* dic, gchar *key, gchar *mat, gchar *dev, gchar* host, gint port)
{
	GstElement *source, *converter, *sampler, *vader, *sphinx;
	GstBus *bus = NULL;

    gchar *material = NULL;
    gchar *device = dev == NULL ? g_strdup("hw:0") : g_strdup(dev);

	if(mat == NULL || !g_strcmp0(mat, "Default") == TRUE)
	{
		material = g_strdup("gsettingsaudiosrc");
	}
	else
	{
		if( !g_strcmp0(mat, "Alsa") == TRUE)
		{
			material = g_strdup("alsasrc");
		}
		else
		{
            if( !g_strcmp0(mat, "Network") == TRUE)
            {
                if( NULL != host)
                {
                    material = g_strdup("tcpserversrc");
                }
                else
                {
                    g_warning("In order to receive order from network you need to give an host and a port");

                    material = g_strdup("gsettingsaudiosrc");
                }
            }
			else
            {
                material = g_strdup("gsettingsaudiosrc");
		    }
        }
	}

	g_return_val_if_fail(g_file_test(hmm, G_FILE_TEST_EXISTS), 	NULL);
	g_return_val_if_fail(g_file_test(lm, G_FILE_TEST_EXISTS), 	NULL);
	g_return_val_if_fail(g_file_test(dic, G_FILE_TEST_EXISTS), 	NULL);

	PocketvoxRecognizer *recognizer = (PocketvoxRecognizer *)g_object_new(TYPE_POCKETVOX_RECOGNIZER,
																		"hmm", 	hmm,
																		"lm", 	lm,
																		"dic",	dic,
																		NULL);

	g_return_val_if_fail(NULL != recognizer, NULL);

	recognizer->priv = G_TYPE_INSTANCE_GET_PRIVATE (recognizer,
		TYPE_POCKETVOX_RECOGNIZER, PocketvoxRecognizerPrivate);
	PocketvoxRecognizerPrivate *priv = recognizer->priv;

	priv->hmm     = g_strdup(hmm);
	priv->lm      = g_strdup(lm);
	priv->dic     = g_strdup(dic);
    priv->keyword = g_strdup(key);

	//build the pipeline auto-threshold=true
    priv->pipeline = gst_pipeline_new("pipeline");
    g_assert(priv->pipeline);

    bus = gst_pipeline_get_bus(GST_PIPELINE(priv->pipeline));
    g_assert(bus);

    source    = gst_element_factory_make(material, "src");
    g_assert(source);

    //set the device name if we use alsasrc input
    if( !g_strcmp0(material, "alsasrc") == TRUE )
    {
        g_object_set(G_OBJECT(source),
                      "device", device,
                      NULL);
    }
    else
    {
        if(!g_strcmp0(material, "tcpserversrc"))
        {
            g_object_set(G_OBJECT(source),
                        "host", host,
                        "port", port,
                        NULL);
        }
    }

    converter = gst_element_factory_make("audioconvert",      "convert");
    g_assert(converter);

    sampler   = gst_element_factory_make("audioresample",     "resample");
    g_assert(sampler);

    vader     = gst_element_factory_make("vader",             "vader");
    g_assert(vader);
    g_object_set(G_OBJECT(vader),
                "auto-threshold", TRUE,
                NULL);


    sphinx    = gst_element_factory_make("pocketsphinx",      "psphinx");
    g_assert(sphinx);

    gst_bin_add_many(GST_BIN(priv->pipeline),
                    source,
                    converter,
                    sampler,
                    vader,
                    sphinx,
                    NULL);

    if(!g_strcmp0(material, "tcpserversrc") == FALSE )
    {
        gst_element_link_many(source, converter, sampler, vader, sphinx, NULL);
    }
    else
    {
        GstCaps* caps = gst_caps_new_simple("audio/x-raw-int",
            "endianness", G_TYPE_INT,     1234,
            "signed",     G_TYPE_BOOLEAN, TRUE,
            "width",      G_TYPE_INT,     32,
            "depth",      G_TYPE_INT,     32,
            "rate",       G_TYPE_INT,     44100,
            "channels",   G_TYPE_INT,     2,
            NULL);

        gst_element_link_filtered(source, converter, caps);
        gst_element_link_many(converter, sampler, vader, sphinx, NULL);

        gst_caps_unref(caps);
    }

	//set properties
	g_object_set(G_OBJECT(sphinx),
					"hmm", 	priv->hmm,
					"lm",  	priv->lm,
					"dict", priv->dic,
					NULL);

	//connect to a callback function
	g_signal_connect(sphinx, "result", G_CALLBACK(pocketvox_recognizer_process_result), NULL );

	gst_bus_add_signal_watch(bus);
	g_signal_connect(bus, "message::application", G_CALLBACK(pocketvox_recognizer_parse_bus_message), recognizer);

	//play
	gst_element_set_state(priv->pipeline, GST_STATE_PLAYING);

    if(gst_element_get_state(priv->pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE)
    {
        g_warning("PocketvoxRecognizer: unable to put the pipeline in playing mode");

        return NULL;
    }

    g_free(material);
    g_free(device);

    return recognizer;
}

void pocketvox_recognizer_set_state(PocketvoxRecognizer *recognizer,PocketvoxState state)
{
	g_return_if_fail(NULL != recognizer);

	recognizer->priv = G_TYPE_INSTANCE_GET_PRIVATE (recognizer,
		TYPE_POCKETVOX_RECOGNIZER, PocketvoxRecognizerPrivate);
	PocketvoxRecognizerPrivate *priv = recognizer->priv;

	switch(state)
	{
		case POCKETVOX_STATE_RUN:
			gst_element_set_state(priv->pipeline, GST_STATE_PLAYING);
			break;
		case POCKETVOX_STATE_STOP:
			gst_element_set_state(priv->pipeline, GST_STATE_PAUSED);
			break;
		default:
			break;
	}

	priv->state = state;
}

void pocketvox_recognizer_set(PocketvoxRecognizer *recognizer, gchar* ppt, gchar *path)
{
	g_return_if_fail(NULL != recognizer);
	g_return_if_fail(!g_strcmp0("dict",ppt)
				  || !g_strcmp0("lm",ppt)
				  || !g_strcmp0("hmm", ppt) );

	pocketvox_recognizer_set_state(recognizer, POCKETVOX_STATE_STOP);

	recognizer->priv = G_TYPE_INSTANCE_GET_PRIVATE (recognizer,
		TYPE_POCKETVOX_RECOGNIZER, PocketvoxRecognizerPrivate);
	PocketvoxRecognizerPrivate *priv = recognizer->priv;

	GstElement *sphinx = gst_bin_get_by_name(GST_BIN(priv->pipeline), "asr");
	g_object_set(G_OBJECT(sphinx),
					ppt, path,
					NULL);

	//put the pipeline in the same state
	pocketvox_recognizer_set_state(recognizer,priv->state);
}

void pocketvox_recognizer_set_keyword(PocketvoxRecognizer *recognizer, gchar *keyword)
{
    g_return_if_fail(NULL != recognizer);
    g_return_if_fail(NULL != keyword);

	recognizer->priv = G_TYPE_INSTANCE_GET_PRIVATE (recognizer,
		TYPE_POCKETVOX_RECOGNIZER, PocketvoxRecognizerPrivate);
	PocketvoxRecognizerPrivate *priv = recognizer->priv;

    priv->keyword = g_strdup(keyword);
}

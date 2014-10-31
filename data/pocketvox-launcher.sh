#!/bin/bash
export ESPEAK_VOICES_PATH=/usr/lib/i386-linux-gnu/espeak-data/voices
export GST_PLUGIN_PATH=/usr/local/lib/gstreamer-0.10
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/usr/include/espeak

if [ "$1" == "-gtk" ];
then
{
	pocketvox-gtk
}
else
{
	pocketvox
}
fi

exit 0

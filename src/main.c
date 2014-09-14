#include <stdio.h>
#include "pocketvox_recognizer.h"
#include "pocketvox_notifier.h"
#include "pocketvox_dictionnary.h"

int main(int argc, char **argv)
{
	PocketvoxDictionnary* dict = pocketvox_dictionnary_new("/home/benoit/Bureau/dico.txt");
	pocketvox_dictionnary_display(dict);
	
	pocketvox_dictionnary_add_new_request(dict, "dire comment vas tu");
	/*
	PocketvoxNotifier *pn = pocketvox_notifier_new();
	
	
	PocketvoxRecognizer *pr = pocketvox_recognizer_new("/home/benoit/Projet/voicerecognition/lium_french_f0",
													   "/home/benoit/Projet/voicerecognition/lm/dictionnary.lm.dmp",
													   "/home/benoit/Projet/voicerecognition/frenchWords62K.dic");	

	g_signal_connect_swapped(pr, "result", G_CALLBACK(pocketvox_notifier_notify), pn);

	//start gstreamer
	gst_init(&argc, &argv);
	
	pocketvox_recognizer_start(pr);
	*/
	return 0;
}

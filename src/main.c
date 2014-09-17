#include <stdio.h>
#include "pocketvox_recognizer.h"
#include "pocketvox_notifier.h"
#include "pocketvox_dictionnary.h"
#include "pocketvox_indicator.h"
#include "pocketvox_controller.h"

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	gst_init(&argc, &argv);
	
	PocketvoxIndicator *indicator = pocketvox_indicator_new();
	PocketvoxDictionnary* dict = pocketvox_dictionnary_new("/home/benoit/Bureau/dico.ini", TRUE);
	PocketvoxNotifier *pn = pocketvox_notifier_new();
	PocketvoxRecognizer *pr = pocketvox_recognizer_new("/home/benoit/Projet/voicerecognition/lium_french_f0",
													   "/home/benoit/Projet/voicerecognition/lm/dictionnary.lm.dmp",
													   "/home/benoit/Projet/voicerecognition/frenchWords62K.dic");	

	PocketvoxController *ctr = pocketvox_controller_new(pr, pn, dict, indicator);
	
	pocketvox_controller_start(ctr);
	
	return 0;
}

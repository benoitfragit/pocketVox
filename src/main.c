#include <stdio.h>
#include "pocketvox-recognizer.h"
#include "pocketvox-notifier.h"
#include "pocketvox-indicator.h"
#include "pocketvox-controller.h"

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	gst_init(&argc, &argv);
	
	PocketvoxIndicator *indicator = pocketvox_indicator_new();
	PocketvoxNotifier *pn = pocketvox_notifier_new();
	PocketvoxRecognizer *pr = pocketvox_recognizer_new("/home/benoit/Projet/voicerecognition/lium_french_f0",
													   "/home/benoit/Projet/voicerecognition/lm/dictionnary.lm.dmp",
													   "/home/benoit/Projet/voicerecognition/frenchWords62K.dic");	

	PocketvoxController *ctr = pocketvox_controller_new(pr, pn, indicator);
	
	pocketvox_controller_start(ctr);
	
	return 0;
}

#include <stdio.h>
#include "pocketvox-application.h"

int main(int argc, char **argv)
{	
	gchar *dict 	= "/home/benoit/Projet/voicerecognition/frenchWords62K.dic";
	gchar *lm 		= "/home/benoit/Projet/voicerecognition/lm/dictionnary.lm.dmp";
	gchar *acoustic = "/home/benoit/Projet/voicerecognition/lium_french_f0";
	
	PocketvoxApplication *application = pocketvox_application_new(acoustic, lm, dict);
	
	pocketvox_application_start(application);
	
	return 0;
}

#include "pocketvox-application.h"

int main(int argc, char **argv)
{	
	gchar *dict 	= "/home/benoit/Projet/voicerecognition/frenchWords62K.dic";
	gchar *lm 		= "/home/benoit/Projet/voicerecognition/lm/dictionnary.lm.dmp";
	gchar *acoustic = "/home/benoit/Projet/voicerecognition/lium_french_f0";
	
	PocketvoxApplication *application = pocketvox_application_new(acoustic, lm, dict);


	PocketvoxModule *m1 = pocketvox_module_new("Move", "/home/benoit/Projet/google2ubuntu/workinprogress/test.txt", FALSE);
	PocketvoxModule *m2 = pocketvox_module_new("Open", "/home/benoit/Projet/google2ubuntu/workinprogress/test.txt", FALSE);
	PocketvoxModule *m3 = pocketvox_module_new("Application", "/home/benoit/Projet/google2ubuntu/workinprogress/test.txt", FALSE);


	pocketvox_application_add_module(application, m1);
	pocketvox_application_add_module(application, m2);

	pocketvox_application_remove_module(application, "Move");
	pocketvox_application_add_module(application, m3);			
			
	
	pocketvox_application_start(application);
	
	return 0;
}

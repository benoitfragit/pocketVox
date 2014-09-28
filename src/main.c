#include "pocketvox-application.h"

int main(int argc, char **argv)
{	
	PocketvoxApplication *application = pocketvox_application_new("/home/benoit/Bureau/benoit.profile");


	PocketvoxModule *m1 = pocketvox_module_new("Move", "/home/benoit/Projet/google2ubuntu/workinprogress/test.txt", FALSE);
	PocketvoxModule *m2 = pocketvox_module_new("Open", "/home/benoit/Bureau/MyModule/MyModule.dic", FALSE);
	PocketvoxModule *m3 = pocketvox_module_new("Application", "/home/benoit/Projet/google2ubuntu/workinprogress/test.txt", FALSE);

	pocketvox_application_add_module(application, m1);
	pocketvox_application_add_module(application, m2);

	pocketvox_application_remove_module(application, "Move");
	pocketvox_application_add_module(application, m3);			
		
	pocketvox_application_start(application);
		
	return 0;
}

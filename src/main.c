#include "pocketvox-application.h"

int main(int argc, char **argv)
{
	PocketvoxApplication *application = pocketvox_application_new("/home/benoit/.pocketvox-xubuntu/pocketvox-xubuntu.profile");

	pocketvox_application_start(application);

	return 0;
}

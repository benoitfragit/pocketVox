#include "pocketvox-setup.h"

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	
	PocketvoxSetup *setup = pocketvox_setup_new();
	
	pocketvox_setup_show(setup);
	
	return 0;
}

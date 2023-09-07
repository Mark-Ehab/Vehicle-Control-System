/*
 * metaWare
 * @return
 */
#include "App/application.h"
#include "Commons/std_types.h"

int main(){
	/* initializing the application */
	App_init();
	while(TRUE){
		/* saving or getting the data */
		App_saveOrGet();
	}
}


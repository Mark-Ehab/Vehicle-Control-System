/*
 * metaWare
 * @return
 */
#include "Commons/std_types.h"
#include "APP/application.h"

int main(){
	/* initializing the application */
	App_init();
	while(TRUE){
		/* saving or getting the data */
		App_saveOrGet();
	}
}


/****************************************************************/
/* 		Author   	 : Mark Ehab                    */
/* 		Date     	 : Jan 17, 2022               	*/
/*              Description	 : Service Functions Interface  */
/* 	        Version          : V1.0                         */
/****************************************************************/

#ifndef LIB_SERVICE_FUNCTIONS_H
#define LIB_SERVICE_FUNCTIONS_H

/*-----------------------------------------------------------------------------------*/
/*                                                                                   */
/*                           	 FUNCTIONS PROTOTYPES		                     */
/*                                                                                   */
/*-----------------------------------------------------------------------------------*/

/**********************************************************************************/
/* Description     : This function is used to map a value in some input range to  */
/*		     its equivalent value in output range			  */
/* Input Arguments : s32 Copy_s32InputMin  , s32 Copy_s32InputMax, 	          */
/* 		     s32 Copy_u32OutputMin , s32 Copy_s32OutputMax, 		  */
/* 		     s32 Copy_s32InputVal			   		  */
/* Return          : s32                 	      			          */
/**********************************************************************************/
sint32 SERV_s32Map(  sint32 Copy_s32InputMin  , sint32 Copy_s32InputMax
		, sint32 Copy_s32OutputMin , sint32 Copy_s32OutputMax
		, sint32 Copy_s32InputVal );
/**********************************************************************************/
/* Description     : Function to get modulus of two numbers			  */
/* Input Arguments : f32 Copy_f32Num1 , f32 Copy_f32Num2		          */
/* Return          : f32						          */
/**********************************************************************************/
float32 SERV_f32Mod(float32 Copy_f32Num1 , float32 Copy_f32Num2);
/**********************************************************************************/
/* Description     : Function to get ceil of the result of two dividend numbers   */
/* Input Arguments : f32 Copy_f32Num1 , f32 Copy_f32Num2		          */
/* Return          : u32  	        	      			          */
/**********************************************************************************/
uint32 SERV_u32CeilDev(float32 Copy_f32Num1 , float32 Copy_f32Num2);
/**********************************************************************************/
/* Description     : Function to get the power of a number		          */
/* Input Arguments : u32 Copy_u32Base , u32 Copy_u32Power		          */
/* Return          : u32  	        	      			          */
/**********************************************************************************/
uint32 SERV_u32Pow(uint32 Copy_u32Base , uint32 Copy_u32Power);

#endif /* LIB_SERVICE_FUNCTIONS_H */

#include "chlib_k.h"
#include "arm_math.h"
#include "dsp_fast_demo.h"
 /**
 * @brief  演示arm_cos_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-27
 */
void arm_cos_f32_demo (void) {
	float pi = 3.14159f; 			//π
	float pi_2 = pi/2; 				//π/2
	float pi_3_2 = pi_2 * 3;		//3π/2
	
	printf("cos(PI) = %f\n\r",arm_cos_f32(pi));	//打印COS(π) 的近似计算结果
	printf("cos(PI/2) = %f\n\r",arm_cos_f32(pi_2));//打印COS(π/2)的近似计算结果
	printf("cos(3PI/2) = %f\n\r",arm_cos_f32(pi_3_2));//打印COS(3π/2)的近似计算结果
}

 /**
 * @brief  演示arm_sin_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-27
 */
void arm_sin_f32_demo (void) {
	float pi = 3.14159f; 						//π
	float pi_2 = pi/2; 							//π/2
	float pi_3_2 = pi_2 * 3;					//3π/2
	
	printf("sin(PI) = %f\n\r",arm_sin_f32(pi));	//打印SIN(π) 的近似计算结果
	printf("sin(PI/2) = %f\n\r",arm_sin_f32(pi_2));//打印SIN(π/2)的近似计算结果
	printf("sin(3PI/2) = %f\n\r",arm_sin_f32(pi_3_2));//打印SIN(3π/2)的近似计算结果
}

 /**
 * @brief  演示arm_sqrt_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-27
 */
void arm_sqrt_f32_demo(void) {
/**
		由于很多数的平方根结果都为 无理数（结果非无理数的可以看做无理数的一种极限情况），
	所以常规开平方根运算在数字信号处理中经常使用 牛顿-拉夫逊法（Newton-Raphson method）来计算平方根
	说白了就是将平方根函数（幂级数运算） 用泰勒级数展开，并将展开项的前几项联立一个方程组并领其解为0来求解一个平方根的近似值。
	这个方法的最大优点就是f(x) = 0 这个方程单根的附近具有平放收敛性。并且其计算的时间复杂度低。有兴趣的读者可自行学习。
	本函数内部使用的就是Newton-Raphson method的迭代方程来求解平方根的近似值。
*/
	
	float32_t number_4 = 4.0f;
	float32_t number_9 = 9.0f;
	float32_t number_2 = 2.0f;
	float32_t number_3 = 3.0f;
	float32_t result = 0;
	
	arm_sqrt_f32(number_4,&result);				//计算4的平方根的近似值
	printf("the sqrt of 4 is %f\n\r",result);
	
	arm_sqrt_f32(number_9,&result);				//计算9的平方根的近似值
	printf("the sqrt of 9 is %f\n\r",result);
	
	arm_sqrt_f32(number_2,&result);				//计算2的平方根的近似值
	printf("the sqrt of 2 is %f\n\r",result);	

	arm_sqrt_f32(number_3,&result);				//计算3的平方根的近似值
	printf("the sqrt of 3 is %f\n\r",result);
	
}

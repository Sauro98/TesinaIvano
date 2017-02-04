#ifndef SECURE_BOXES_H
#define SECURE_BOXES_H

#include "permutations.h"

		//SECURE BOX 1
		static const uchar SECURE_BOX_1_ROW_0[] = {
		    14 , 4 , 13 , 1 , 2 , 15 , 11 , 8 , 3 , 10 , 6 , 12 , 5 , 9 , 0 , 7
		};
		
		static const uchar SECURE_BOX_1_ROW_1[] = {
		    0 , 15 , 7 , 4 , 14 , 2 , 13 , 1 , 10 , 6 , 12 , 11 , 9 , 5 , 3 , 8
		};
		
		static const uchar SECURE_BOX_1_ROW_2[] = {
		    4 , 1 , 14 , 8 , 13 , 6 , 2 , 11 , 15 , 12 , 9 , 7 , 3 , 10 , 5 , 0
		};
		
		static const uchar SECURE_BOX_1_ROW_3[] = {
		    15 , 12 , 8 , 2 , 4 , 9 , 1 , 7 , 5 , 11 , 3 , 14 , 10 , 0 , 6 , 13
		};
		
		//SECURE BOX 2
		
		static const uchar SECURE_BOX_2_ROW_0[] = {
		    15 , 1 , 8 , 14 , 6 , 11 , 3 , 4 , 9 , 7 , 2 , 13 , 12 , 0 , 5 , 10
		};
		
		static const uchar SECURE_BOX_2_ROW_1[] = {
		    3 , 13 , 4 , 7 , 15 , 2 , 8 , 14 , 12 , 0 , 1 , 10 , 6 , 9 , 11 , 5
		};
		
		static const uchar SECURE_BOX_2_ROW_2[] = {
		    0 , 14 , 7 , 11 , 10 , 4 , 13 , 1 , 5 , 8 , 12, 6 , 9 , 3 , 2 , 15
		};
		
		static const uchar SECURE_BOX_2_ROW_3[] = {
		    13 , 8 , 10, 1 , 3 , 15 , 4 , 2 , 11 , 6 , 7 , 12 , 0 , 5 , 14 , 9
		};
		
		//SECURE BOX 3
		
		static const uchar SECURE_BOX_3_ROW_0[] = {
		    10 , 0 , 9 , 14 , 6 , 3 , 15 , 5 , 1 , 13 , 12 , 7 , 11 , 4 , 2 , 8
		};
		
		static const uchar SECURE_BOX_3_ROW_1[] = {
		    13 , 7 , 0 , 9 , 3 , 4 , 6 , 10 , 2 , 8 , 5 , 14 , 12 , 11 , 15 , 1
		};
		
		static const uchar SECURE_BOX_3_ROW_2[] = {
		    13 , 6 , 4 , 9 , 8 , 15 , 3 , 0 , 11 , 1 , 2 , 12 , 5 , 10 , 14 , 7
		};
		
		static const uchar SECURE_BOX_3_ROW_3[] = {
		    1 , 10 , 13 ,0 , 6 , 9 , 8 , 7 , 4 , 15, 14, 3 , 11 , 5 , 2 , 12
		};
		
		//SECURE BOX 4
		
		static const uchar SECURE_BOX_4_ROW_0[] = {
		    7 , 13 , 14 , 3 , 0 , 6 , 9 , 10 , 1 , 2, 8 , 5 , 11 , 12 , 4 , 15
		};
		
		static const uchar SECURE_BOX_4_ROW_1[] = {
		    13 , 8 , 11 , 5 , 6 , 15 , 0 , 3 , 4 , 7 , 2 , 12 , 1 , 10 , 14 , 9
		};
		
		static const uchar SECURE_BOX_4_ROW_2[] = {
		    10 , 6 , 9 , 0 , 12, 11 , 7 , 13 , 15 , 1 , 3 , 14, 5 , 2 , 8 , 4
		};
		
		static const uchar SECURE_BOX_4_ROW_3[] = {
		    3 , 15 , 0 , 6 , 10 , 1 , 13 , 8 , 9 , 4 , 5 , 11 , 12 , 7 , 2 , 14
		};
		
		//SECURE BOX 5
		
		static const uchar SECURE_BOX_5_ROW_0[] = {
		    2 , 12 , 4 , 1 , 7 , 10 , 11 , 6 , 8 , 5 , 3 ,15 , 13 , 0 , 14 , 9
		};
		
		static const uchar SECURE_BOX_5_ROW_1[] = {
		    14 ,11 ,2 , 12 , 4 , 7 , 13 , 1 , 5 , 0 , 15 , 10 , 3 , 9 , 8 , 6
		};
		
		static const uchar SECURE_BOX_5_ROW_2[] = {
		    4 ,2 , 1 , 11 , 10 , 13 , 7 , 8 , 15 , 9 , 12 , 5 , 6 , 3 , 0 , 14
		};
		
		static const uchar SECURE_BOX_5_ROW_3[] = {
		    11 , 8 , 12 , 7 , 1 , 14, 2 , 13, 6 , 15 , 0 , 9 , 10 , 4 , 5 , 3
		};
		
		//SECURE BOX 6
		
		static const uchar SECURE_BOX_6_ROW_0[] = {
		    12 , 1 , 10 , 15 , 9 , 2 , 6 , 8 , 0 , 13 , 3 , 4 , 14 , 7 , 5 , 11
		};
		
		static const uchar SECURE_BOX_6_ROW_1[] = {
		    10 , 15 , 4 , 2 , 7 , 12 , 9 , 5 , 6 , 1 , 13 , 14 , 0 , 11 , 3 , 8
		};
		
		static const uchar SECURE_BOX_6_ROW_2[] = {
		    9 ,14 , 15 , 5 , 2 , 8 , 12 , 3 , 7 , 0 , 4 , 10 , 1 , 13 , 11, 6
		};
		
		static const uchar SECURE_BOX_6_ROW_3[] = {
		    4 , 3 , 2 , 12 , 9 , 5 , 15, 10 , 11 , 14 , 1 , 7 , 6 , 0 , 8 , 13
		};
		
		//SECURE BOX 7
		
		static const uchar SECURE_BOX_7_ROW_0[] = {
		    4 , 11 ,2 , 14 , 15 ,0 , 8 , 13 , 3 , 12 ,9 , 7 , 5 , 10 , 6 , 1
		};
		
		static const uchar SECURE_BOX_7_ROW_1[] = {
		    13 , 0 , 11 , 7 , 4 , 9 , 1 , 10 , 14 ,3 , 5 , 12 ,2 , 15, 8 , 6
		};
		
		static const uchar SECURE_BOX_7_ROW_2[] = {
		    1 , 4 , 11 , 13 , 12 , 3 , 7 , 14 , 10 , 15 , 6 , 8 , 0 , 5 , 9 , 2
		};
		
		static const uchar SECURE_BOX_7_ROW_3[] = {
		    6, 11 , 13 , 8 , 1 , 4 , 10 , 7 , 9 , 5 , 0 , 15, 14 , 2 , 3 , 12
		};
		
		//SECURE BOX 8
		
		static const uchar SECURE_BOX_8_ROW_0[] = {
		    13 , 2 , 8 , 4 , 6 , 15 , 11 , 1 , 10 , 9 , 3 , 14 ,5 , 0 , 12 , 7
		};
		
		static const uchar SECURE_BOX_8_ROW_1[] = {
		    1 , 15 , 13 , 8 , 10 , 3 , 7 , 4 , 12 , 5 , 6 , 11 , 0 , 14 , 9 , 2
		};
		
		static const uchar SECURE_BOX_8_ROW_2[] = {
		    7 ,11 ,4 , 1 , 9 , 12 , 14 , 2 , 0 , 6 , 10 , 13 , 15 , 3 , 5 , 8
		};
		
		static const uchar SECURE_BOX_8_ROW_3[] = {
		    2 ,1 , 14 , 7 , 4 , 10 , 8 , 13 , 15 , 12 , 9 , 0 , 3 , 5 , 6 , 11
		};

class Secure_boxes
{
	private:
		static uchar get_extremes(uchar original);
	public:
		static uchar secure_box_1(uchar original);
		static uchar secure_box_2(uchar original);
		static uchar secure_box_3(uchar original);
		static uchar secure_box_4(uchar original);
		static uchar secure_box_5(uchar original);
		static uchar secure_box_6(uchar original);
		static uchar secure_box_7(uchar original);
		static uchar secure_box_8(uchar original);
};

#endif

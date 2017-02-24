#include "secure_boxes.h"

uchar Secure_boxes::secure_box_1(uchar original){
	uchar extremes = get_extremes(original);
	uchar centrals = ((original << 1) & mask_6_bits) >> 2;
	switch(extremes){
		case 0:
			return SECURE_BOX_1_ROW_0[centrals];
			break;
		case 1:
			return SECURE_BOX_1_ROW_1[centrals];
			break;
		case 2:
			return SECURE_BOX_1_ROW_2[centrals];
			break;
		case 3:
			return SECURE_BOX_1_ROW_3[centrals];
			break;
	}
	return 0;
}

uchar Secure_boxes::secure_box_2(uchar original){
	uchar extremes = get_extremes(original);
	uchar centrals = ((original << 1) & mask_6_bits) >> 2;
	switch(extremes){
		case 0:
			return SECURE_BOX_2_ROW_0[centrals];
			break;
		case 1:
			return SECURE_BOX_2_ROW_1[centrals];
			break;
		case 2:
			return SECURE_BOX_2_ROW_2[centrals];
			break;
		case 3:
			return SECURE_BOX_2_ROW_3[centrals];
			break;
	}
	return 0;
}

uchar Secure_boxes::secure_box_3(uchar original){
	uchar extremes = get_extremes(original);
	uchar centrals = ((original << 1) & mask_6_bits) >> 2;
	switch(extremes){
		case 0:
			return SECURE_BOX_3_ROW_0[centrals];
			break;
		case 1:
			return SECURE_BOX_3_ROW_1[centrals];
			break;
		case 2:
			return SECURE_BOX_3_ROW_2[centrals];
			break;
		case 3:
			return SECURE_BOX_3_ROW_3[centrals];
			break;
	}
	return 0;
}

uchar Secure_boxes::secure_box_4(uchar original){
	uchar extremes = get_extremes(original);
	uchar centrals = ((original << 1) & mask_6_bits) >> 2;
	switch(extremes){
		case 0:
			return SECURE_BOX_4_ROW_0[centrals];
			break;
		case 1:
			return SECURE_BOX_4_ROW_1[centrals];
			break;
		case 2:
			return SECURE_BOX_4_ROW_2[centrals];
			break;
		case 3:
			return SECURE_BOX_4_ROW_3[centrals];
			break;
	}
	return 0;
}

uchar Secure_boxes::secure_box_5(uchar original){
	uchar extremes = get_extremes(original);
	uchar centrals = ((original << 1) & mask_6_bits) >> 2;
	switch(extremes){
		case 0:
			return SECURE_BOX_5_ROW_0[centrals];
			break;
		case 1:
			return SECURE_BOX_5_ROW_1[centrals];
			break;
		case 2:
			return SECURE_BOX_5_ROW_2[centrals];
			break;
		case 3:
			return SECURE_BOX_5_ROW_3[centrals];
			break;
	}
	return 0;
}

uchar Secure_boxes::secure_box_6(uchar original){
	uchar extremes = get_extremes(original);
	uchar centrals = ((original << 1) & mask_6_bits) >> 2;
	switch(extremes){
		case 0:
			return SECURE_BOX_6_ROW_0[centrals];
			break;
		case 1:
			return SECURE_BOX_6_ROW_1[centrals];
			break;
		case 2:
			return SECURE_BOX_6_ROW_2[centrals];
			break;
		case 3:
			return SECURE_BOX_6_ROW_3[centrals];
			break;
	}
	return 0;
}

uchar Secure_boxes::secure_box_7(uchar original){
	uchar extremes = get_extremes(original);
	uchar centrals = ((original << 1) & mask_6_bits) >> 2;
	switch(extremes){
		case 0:
			return SECURE_BOX_7_ROW_0[centrals];
			break;
		case 1:
			return SECURE_BOX_7_ROW_1[centrals];
			break;
		case 2:
			return SECURE_BOX_7_ROW_2[centrals];
			break;
		case 3:
			return SECURE_BOX_7_ROW_3[centrals];
			break;
	}
	return 0;
}

uchar Secure_boxes::secure_box_8(uchar original){
	uchar extremes = get_extremes(original);
	uchar centrals = ((original << 1) & mask_6_bits) >> 2;
	switch(extremes){
		case 0:
			return SECURE_BOX_8_ROW_0[centrals];
			break;
		case 1:
			return SECURE_BOX_8_ROW_1[centrals];
			break;
		case 2:
			return SECURE_BOX_8_ROW_2[centrals];
			break;
		case 3:
			return SECURE_BOX_8_ROW_3[centrals];
			break;
	}
	return 0;
}

uchar Secure_boxes::get_extremes(uchar original){
	uchar first_bit = Permutations::get_single_bit(original,6,1);
	uchar last_bit = Permutations::get_single_bit(original,6,6);
	return (first_bit << 1) | last_bit;
}

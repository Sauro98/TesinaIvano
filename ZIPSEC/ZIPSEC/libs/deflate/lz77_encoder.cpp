#include "lz77_encoder.h" // class's header file
#include <process.h> 
#include <algorithm>
#include <time.h>
// class constructor
LZ77_Encoder::LZ77_Encoder(char* _original, int o_size)
{
	tdArray = new int_deque[256 * 256 * 256];
	perc = 0;
	prevperc = -1;
	original = _original;
	lh_index = 0;
	original_length = o_size;
	//last_update_index = 0;
	//int size = (o_size < HASHTABLE_MAX_SIZE) ? o_size : HASHTABLE_MAX_SIZE;
	//update_key_array(size);

}

// class destructor
LZ77_Encoder::~LZ77_Encoder()
{
	// insert your code here
	delete[] tdArray;
}


void LZ77_Encoder::reset() {
	lh_index = 0;
}


bool LZ77_Encoder::hasMore() {
	return lh_index < original_length;
}

lz77_r LZ77_Encoder::getNext() {


	/*if (lh_index >= last_update_index) {
		int length_left = original_length - last_update_index;
		int size = (length_left < HASHTABLE_MAX_SIZE) ? length_left : HASHTABLE_MAX_SIZE;
		update_key_array(size);
	}*/

	int length = 0;
	int origin = 0;
	//start from the last element in the search buffer
	int search_index = lh_index - 1;
	//look back inside the search buffer, while of course being inside the string, until a prefix is found or the end of the buffer is reached
	//every time the first charachter of the look ahead buffer is found in the search buffer the rest of the buffer is looked forward until the next charachter in the search buffer is different from the 
	//next charachter in the look ahead buffer. If the length of the newly found prefix is greater or equal to the length of the previously found prefixes, then the index of the current prefix is saved in j

	if (lh_index > 3 && lh_index < original_length - 3) {

		int x = char_to_uint(original[lh_index]);
		int y = char_to_uint(original[lh_index + 1]);
		int z = char_to_uint(original[lh_index + 2]);

		int_deque* mydeque = &tdArray[INDEX_3D(x,y,z)];
		if(mydeque->size() < HASHTABLE_MAX_SIZE)
		mydeque->push_back(lh_index);

		for (auto it = mydeque->begin(); it != mydeque->end(); ++it) {
			int current_search_index = *it;
			
			if (current_search_index >= lh_index)
				break;

			if (current_search_index > (lh_index - SEARCH_BUFFER_LENGTH)) {

				int current_length = 3;

				while ((lh_index + current_length) != (original_length - 1) && original[current_search_index + current_length] == original[lh_index + current_length] && current_length < LOOK_AHEAD_BUFFER_LENGTH) {
					current_length++;
				}

				if (current_length > length) {
					length = current_length;
					origin = lh_index - current_search_index;
				}
				if (current_length == LOOK_AHEAD_BUFFER_LENGTH)
					break;
			} else {
				mydeque->erase(it);
				it = mydeque->begin();
			}
		}
	}
	                                     // this clever hint was found in zlib source code and actually improves compression
	if ((length == 0 && origin == 0) || (length == 3 && (origin < (lh_index - 4096)))) {
		lh_index++;
		return lz77_r(char_to_uint(original[lh_index - 1]));
	} else {
		lh_index = lh_index + length;
		return lz77_r(length, origin);
	}
}

void LZ77_Encoder::print_perc(){

	//avoid n/0 bug
	int total = (original_length >= 100) ? original_length / 100 : 1;
	perc = lh_index/total;
	if (lh_index == original_length) perc = 100; //small files fix

	if (perc != prevperc && perc % 10 == 0) {
		if ( perc != 0)
			rollback_line();
		print(perc<<" % ... ("<<lh_index<<"/"<<original_length<<")");
		prevperc = perc;
	}
}


/*void LZ77_Encoder::update_key_array(int size) {
	int prev_index = last_update_index;
	int max = prev_index + (size - 2);
	for (; last_update_index < max; last_update_index++) {

		int x = char_to_uint(original[last_update_index]);
		int y = char_to_uint(original[last_update_index + 1]);
		int z = char_to_uint(original[last_update_index + 2]);

		int_deque* currdeque = &tdArray[INDEX_3D(x,y,z)];

		currdeque->push_back(last_update_index);

		int scope_limit = lh_index - SEARCH_BUFFER_LENGTH;

		while (currdeque->front() < scope_limit)
			currdeque->pop_front();
	}
}*/

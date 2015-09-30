/*
 File: frame_pool.C
 
 Author: Patrick Sheehan
 CSCE 410 - P2
 Texas A&M University
 Date  : 9/24/15
 
 Description: Management of the Free-Frame Pool.
 
 References: http://stackoverflow.com/questions/17120681/implement-the-bit-map-in-the-following-situation
 */

#include "frame_pool.H"
#include <stdlib.h>


#define MB * (0x1 << 20)
#define KB * (0x1 << 10)
#define KERNEL_POOL_START_FRAME ((2 MB) / (4 KB))   // 512
#define KERNEL_POOL_SIZE ((2 MB) / (4 KB))          // 512

#define FRAME_SIZE 4 KB


unsigned long * start_frame;
unsigned long number_of_frames;
unsigned char * frame_vacancy_bitmap;

int main(int argc, char** argv) {

	FramePool kernel_mem_pool(KERNEL_POOL_START_FRAME,
                              KERNEL_POOL_SIZE,
                              0);

	/*		TEST THE GET_FRAME() 		*/
	int i;
	for (i = 0; i < 10; i++) {
		unsigned long frame = kernel_mem_pool.get_frame();
		printf("Got kernel frame <#%lu>\n", frame);
	}

	printVacancyBitmap();

	return 0;
}

void printVacancyBitmap() {

	printf("\nNon-zero bytes in the vacancy bitmap:\n");
	for (i = 0; i < (number_of_frames / 8); i++) {
		// int val = *(frame_vacancy_bitmap + i);
		if (*(frame_vacancy_bitmap + i)) {
			printf("bitmap[%d] = %d\n", i, *(frame_vacancy_bitmap + i));
		}
	}
	printf("\n");
}

FramePool::FramePool(unsigned long _base_frame_no,
                     unsigned long _nframes,
                     unsigned long _info_frame_no) 
{

	printf("Initializing FramePool\n");

	// unsigned long foo = 0;
	// unsigned long * TEST_ADDR = &foo;
	// start_frame = TEST_ADDR;
	// ** OR **
	// start_frame = (unsigned long *) 0x7f70308000; //TEST_ADDR;
	// ** OR **
	// Start Kernel mem pool at 2MB
	// start_frame = (unsigned long *) 0x20000; 
	// ** OR **
	start_frame = (unsigned long *) malloc(_nframes * FRAME_SIZE);

	// Adjust for the base frame parameter
	// start_frame += _base_frame_no * FRAME_SIZE;

	// Store the number of frames in this pool
	number_of_frames = _nframes;


	printf("Physical address starts at:	%p\n", start_frame);
	printf("Range of frames: [%lu, %lu]\n", 
			_base_frame_no, _base_frame_no + number_of_frames);
	
	// Initialize the bitmap pointer according to the info frame
	frame_vacancy_bitmap = (unsigned char *) start_frame + (_info_frame_no * FRAME_SIZE);
	printf("Vacancy bitmap page <#%lu>\n", _info_frame_no);
	printf("Vacancy bitmap physical address: %p\n", frame_vacancy_bitmap);


	// Clear all entries in the bitmap
	int i;
	for (i = 0; i < number_of_frames; i++) {
		clear_nth_bit(frame_vacancy_bitmap, i);
	}

	printf("Cleared all entries in vacancy bitmap\n");

	// Mark the bitmap frame as inaccessible
	mark_inaccessible(_info_frame_no, 1);
	printf("Marked Vacancy bitmap frame as inaccessible\n");
}

unsigned long FramePool::get_frame() {

	// Allocate an available frame from the the bitmap,
	// If successful, return its frame number. Otherwise return 0

	printf("BEGIN: FramePool searching for an available frame\n");
	unsigned long i;
	for (i = 0; i < number_of_frames; i++) {

		if (get_nth_bit(frame_vacancy_bitmap, i) == 0) {
			set_nth_bit(frame_vacancy_bitmap, i);
			printf("SUCCESS: FramePool found vacancy at frame <#%lu>\n", i);
			return i;
		}
	}


	printf("ERROR: FramePool could not find a vacant frame\n");
	return 0;
}

void FramePool::mark_inaccessible(unsigned long _base_frame_no,
								  unsigned long _nframes) {

	int i;
	for (i = _base_frame_no; i < _base_frame_no + _nframes; i++) {
		set_nth_bit(frame_vacancy_bitmap, i);
	}
}

static void release_frame(unsigned long _frame_no) {


	// FramePool *fp = (FramePool *)  SOME_FRAME_POOL_REF
	/* TODO: make sure _frame_no is in range */

	// clear_nth_bit(frame_vacancy_bitmap, _frame_no);
}

/* 			BITMAP FUNCTIONS			*/

void FramePool::set_nth_bit(unsigned char *bitmap, int idx)
{
    bitmap[idx / CHAR_BIT] |= 1 << (idx % CHAR_BIT);
}

void FramePool::clear_nth_bit(unsigned char *bitmap, int idx)
{
    bitmap[idx / CHAR_BIT] &= ~(1 << (idx % CHAR_BIT));
}

int FramePool::get_nth_bit(unsigned char *bitmap, int idx)
{
    return (bitmap[idx / CHAR_BIT] >> (idx % CHAR_BIT)) & 1;
}

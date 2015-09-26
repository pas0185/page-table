/*
 File: frame_pool.C
 
 Author: Patrick Sheehan
 CSCE 410 - P2
 Texas A&M University
 Date  : 9/24/15
 
 Description: Management of the Free-Frame Pool.
 
 
 */

#include "frame_pool.H"

unsigned long FramePool::frame_bitmap;
unsigned long FramePool::start_frame;
unsigned long FramePool::pool_size;


FramePool::FramePool(unsigned long _base_frame_no,
                     unsigned long _nframes,
                     unsigned long _info_frame_no) {
    
	// Set the base address for this frame pool
	start_frame = (unsigned long *) _base_frame_no;
	
	// Assign number of frames to be managed
	number_of_frames = _nframes;

	// Assign a frame for bookkeeping on frame availability 
	frame_vacancy_bitmap = (unsigned long *) _info_frame_no;
	
    
}

unsigned long FramePool::get_frame() {

	return -1;
}

void FramePool::mark_inaccessible(unsigned long _base_frame_no,
								  unsigned long _nframes) {


}

static void FramePool::release_frame(unsigned long _frame_no) {

}
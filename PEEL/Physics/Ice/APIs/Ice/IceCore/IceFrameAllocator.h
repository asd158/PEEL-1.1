///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a frame allocator.
 *	\file		IceAllocator.h
 *	\author		Pierre Terdiman
 *	\date		February, 23, 2008
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEFRAMEALLOCATOR_H
#define ICEFRAMEALLOCATOR_H

	FUNCTION ICECORE_API void*	FrameAlloc(udword nb_bytes);
	FUNCTION ICECORE_API void	FrameFree(void* memory, udword nb_bytes);
	FUNCTION ICECORE_API udword	ResetFrameMemory();

#endif // ICEFRAMEALLOCATOR_H

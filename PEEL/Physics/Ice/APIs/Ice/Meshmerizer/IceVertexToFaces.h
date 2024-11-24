///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains vertex-to-faces code.
 *	\file		IceVertexToFaces.h
 *	\author		Pierre Terdiman
 *	\date		January, 17, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEVERTEXTOFACES_H
#define ICEVERTEXTOFACES_H

	class MESHMERIZER_API VertexToFaces
	{
		public:
		// Constructor / destructor
								VertexToFaces();
								~VertexToFaces();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Creates vertex-to-faces data.
		 *	\param		nb_verts	[in] the number of vertices
		 *	\param		nb_faces	[in] the number of triangles
		 *	\param		faces		[in] the list of triangles
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bool			Create(udword nb_verts, udword nb_faces, const IndexedTriangle* faces);

		// Data access
		inline_	udword			GetNbVerts()		const	{ return mNbVerts;		}
		inline_	const udword*	GetCounts()			const	{ return mCounts;		}
		inline_	const udword*	GetOffsets()		const	{ return mOffsets;		}
		inline_	const udword*	GetFaceIndices()	const	{ return mFaceIndices;	}

		private:
				udword			mNbVerts;
				udword*			mCounts;
				udword*			mOffsets;
				udword*			mFaceIndices;
	};

#endif // ICEVERTEXTOFACES_H
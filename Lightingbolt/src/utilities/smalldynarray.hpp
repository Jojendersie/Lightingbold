// This is the definition of a dynamic array with a fast append at the end.
// In differece to std::vector this takes nearly no code space.
// The type of the array is an arbitraty pointer (all usecases in the game
// need pointer arrays).
// The array has an implicit capacity of 2^y > n, where n is the number of
// objects in the array.
#pragma once

#include "../predecl.hpp"
#include <malloc.h>

namespace Utilities
{

	// ************************************************************************** //
	// Fast method to check wether a number is a potence of two (2^x=n) or not.
	inline bool IsPotOf2( uint _x )
	{
		// The second is said to be faster than the first variant
		//return (x != 0) && ((x & (x - 1)) == 0);
		return (_x>=1) & ((_x&(_x-1)) < 1);
	}

	class DynArray
	{
	private:
		uint m_uiNumObjects;
		void** A;
	public:
		uint GetNum() const	{ return m_uiNumObjects; }

		DynArray() : m_uiNumObjects(0), A(nullptr)	{}

		// Attention! delete all objects manually before destruction.
#ifdef DYNAMIC_RELOAD
		~DynArray()	{ free( A ); A = nullptr; m_uiNumObjects = 0; }
#else
		~DynArray()	{ free( A ); }
#endif

		// This access function is so small, that it will be inlined, or it doesn't
		// matter if it is compiled for each class.
		template<class T> T* Get( uint _Idx ) const	{ return (T*)A[_Idx]; }

		void Append( void* _pNew )
		{
			// Bad special case - array is empty only the first time. Could be solved
			// with an empty default entry.
			if( m_uiNumObjects == 0 )
			{
				A = (void**)malloc( sizeof(void*) );
			} else if( IsPotOf2( m_uiNumObjects ) )
			{
				A = (void**)realloc( A, m_uiNumObjects*2*sizeof(void*) );
			}

			// Array size is now large enough. Simple append the new element.
			A[m_uiNumObjects++] = _pNew;
		}

		void Remove( uint _Idx )
		{
			Assert( _Idx < m_uiNumObjects );
			A[_Idx] = A[--m_uiNumObjects];
			// Reallocation occurs only in Add. Space is still allocated, but
			// traversing over the array is now one element shorter.
		}
	};
};
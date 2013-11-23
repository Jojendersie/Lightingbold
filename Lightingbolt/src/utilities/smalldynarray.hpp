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

	template <class T> 
	class DynArray
	{
	private:
		uint m_size;
		T* m_data;

		void Reallocate();
        void Reallocate(int _newSize);
	public:
		uint GetNum() const	{ return m_uiNumObjects; }

		DynArray() : m_uiNumObjects(0), A(nullptr)	{}

		~DynArray()	{ free( m_data ); }

		// This access function is so small, that it will be inlined, or it doesn't
		// matter if it is compiled for each class.
		T* Get( uint _Idx ) const { return m_data[_Idx]; }

		void Append( T* _pNew )
		{
			// Bad special case - array is empty only the first time. Could be solved
			// with an empty default entry.
			if( m_uiNumObjects == 0 )
			{
				m_data = (T*)malloc( sizeof(T*) );
			} else if( IsPotOf2( m_uiNumObjects ) )
			{
				m_data = (T*)realloc( m_data, m_uiNumObjects*2*sizeof(T*) );
			}

			// Array size is now large enough. Simple append the new element.
			m_data[m_uiNumObjects++] = _pNew;
		}

		void Remove( uint _Idx )
		{
			Assert( _Idx < m_uiNumObjects );
			m_data[_Idx] = m_data[--m_uiNumObjects];
			// Reallocation occurs only in Add. Space is still allocated, but
			// traversing over the array is now one element shorter.
		}
	};
};
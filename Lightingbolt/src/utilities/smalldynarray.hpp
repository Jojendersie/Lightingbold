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

	/*template <class T> 
	class DynArray
	{
	private:
		uint m_size;
		T* m_data;

	public:
		uint GetNum() const	{ return m_size; }

		DynArray() : m_size(0), m_data(nullptr)	{}

		~DynArray()	{ free( m_data ); }

		// This access function is so small, that it will be inlined, or it doesn't
		// matter if it is compiled for each class.
		T* Get( uint _Idx ) const { return m_data[_Idx]; }

		void Append( T* _pNew )
		{
			// Bad special case - array is empty only the first time. Could be solved
			// with an empty default entry.
			if( m_size == 0 )
			{
				m_data = (T*)malloc( sizeof(T*) );
			} else if( IsPotOf2( m_size ) )
			{
				m_data = (T*)realloc( m_data, m_size*2*sizeof(T*) );
			}

			// Array size is now large enough. Simple append the new element.
			m_data[m_size++] = *_pNew;
		}

		void Remove( uint _Idx )
		{
			Assert( _Idx < m_size );
			m_data[_Idx] = m_data[--m_size];
			// Reallocation occurs only in Add. Space is still allocated, but
			// traversing over the array is now one element shorter.
		}

		T* operator [](int _idx)
		{
			return m_data[_idx];
		}
	};*/

	template <class T> 
	class DynArray
	{
	private:
		int m_size;
		int m_capacity;
		T* m_data;

		void reallocate();
	public:
		DynArray() : m_size(0), m_capacity(1), m_data(nullptr)	{ m_data = new T[m_capacity]; }
		DynArray(int _capacity) : m_size(0), m_capacity(_capacity), m_data(nullptr)	{ m_data = new T[m_capacity]; }
		~DynArray()	{ delete[] m_data; }

		int size() {return m_size;}
		int capacity() {return m_capacity;}

		void append(const T& _element) 
		{
			if(m_size<m_capacity) m_data[m_size++] = _element;
			else
			{
				m_capacity*=2;
				T* temp = new T[m_capacity];
				for(int i=0;i<m_size;++i)
					temp[i]=m_data[i];
				delete[] m_data;
				m_data = temp;
				m_data[m_size++]= _element;
			}
		}

		void remove(int _idx)
		{
			Assert( _idx < m_size );
			m_data[_idx] = m_data[--m_size];
		}

		T* operator [](int _idx)
		{
			return &m_data[_idx];
		}

	};
};
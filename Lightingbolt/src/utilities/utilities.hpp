// Assertions, which break direct in the source and not somewhere in a std header file

#pragma once

#ifdef _DEBUG
#include <intrin.h>

// Simple assertion like cassert, but stops in the line of source, where the
// assertion is located in not in some other file like the std::assert.
#define Assert(a) if(!(a)) __debugbreak()

// The Assert__ execute A (even in release) and test for non-/equality with Const (debug only)
#define AssertEq(A,Const) if((A)!=(Const)) __debugbreak()
#define AssertNeq(A,Const) if((A)==(Const)) __debugbreak()

#else	// _DEBUG


#define Assert(a)

// The Assert__ execute A (even in release) and test for non-/equality with Const (debug only)
#define AssertEq(A,Const) A
#define AssertNeq(A,Const) A

#endif // _DEBUG




#include <limits.h>

#if(UCHAR_MAX == 0xff)
	typedef unsigned char uint8;
	typedef unsigned char byte;
#else
	#error "sizeof(char) is not 1"
#endif

#if(SCHAR_MAX == 0x7f)
	typedef signed char int8;
#else
	#error "sizeof(char) is not 1"
#endif

#if(USHRT_MAX == 0xffff)
	typedef unsigned short uint16;
#else
	#if(UINT_MAX == 0xffff)
		typedef unsigned int uint16;
	#else
		#error "cannot support 16 bit unsigned integer on this plattform"
	#endif
#endif

#if(SHRT_MAX == 0x7fff)
	typedef signed short int16;
#else
	#if(INT_MAX == 0x7fff)
		typedef signed int int16;
	#else
		#error "cannot support 16 bit signed integer on this plattform"
	#endif
#endif

#if(USHRT_MAX == 0xffffffff)
	typedef unsigned short uint32;
#else
	#if(UINT_MAX == 0xffffffff)
		typedef unsigned int uint32;
	#else
		#if(ULONG_MAX == 0xffffffff)
			typedef unsigned long uint32;
		#else
			#error "cannot support 32 bit unsigned integer on this plattform"
		#endif
	#endif
#endif

#if(SHRT_MAX == 0x7fffffff)
	typedef signed short int32;
#else
	#if(INT_MAX == 0x7fffffff)
		typedef signed int int32;
		#else
			#if(LONG_MAX == 0x7fffffff)
			typedef signed long int32;
		#else
			#error "cannot support 32 bit signed integer on this plattform"
		#endif
	#endif
#endif

#if(ULLONG_MAX == 0xffffffffffffffff)
	typedef unsigned long long uint64;
#else
	#if(UINT_MAX == 0xffffffffffffffff)
		typedef unsigned int uint64;
	#else
		#if(ULONG_MAX == 0xffffffffffffffff)
			typedef unsigned long uint64;
		#else
			#error "cannot support 64 bit unsigned integer on this plattform"
		#endif
	#endif
#endif

#if(LLONG_MAX == 0x7fffffffffffffff)
	typedef signed long long int64;
#else
	#if(INT_MAX == 0x7fffffffffffffff)
		typedef signed int int64;
		#else
			#if(LONG_MAX == 0x7fffffffffffffff)
			typedef signed long int64;
		#else
			#error "cannot support 64 bit signed integer on this plattform"
		#endif
	#endif
#endif

typedef unsigned int uint;





// ************************************************************************** //
// Fast method to check wether a number is a potence of two (2^x=n) or not.
inline bool IsPotOf2( uint _x )
{
	// The second is said to be faster than the first variant
	//return (x != 0) && ((x & (x - 1)) == 0);
	return (_x>=1) & ((_x&(_x-1)) < 1);
}



// ************************************************************************** //
// A small scoped buffer for any purpose
#include <malloc.h>
class Buffer
{
public:
	void* pAnything;
	Buffer( int _iSize )	{ pAnything = malloc(_iSize);}
	~Buffer()				{ free(pAnything); }
};

namespace Saga {
	/**************************************************************************//**
	* \brief	A fast memory allocation unit to get objects of the same size.
	* \details	This unit provides only plain memory. There are no constructor and
	*			destructor calls. (Replaces malloc/free).
	*****************************************************************************/
	class PoolAllocator
	{
		void** m_pPools;		///< Array of memory blocks with sizes of 256 * instance size.
		void* m_pNextFree;		///< The next free instance or 0 if there is no place in the current pools.
		int m_iNumPools;		///< Number of large blocks in m_pPools.
		int m_iInstanceSize;	///< Size of the allocable instances
		int m_iBlockSize;		///< Size of on block in the pool
	public:
		PoolAllocator( int _iInstanceSize );
		~PoolAllocator();

		void* AllocInstance();
		void FreeInstance( void* _pInstance );

		/// \brief Keep the allocated blocks but reset all instances to empty.
		///
		void FreeAll();
	};
};
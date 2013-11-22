#pragma once

#include "../predecl.hpp"

namespace Graphic {

	/// \brief A class to load a single shader file and hold it up to date.
	class Shader
	{
	public:
		enum struct Type {
			VERTEX,
			GEOMETRY,
			PIXEL
		};

		/// \brief Load a shader from file.
		/// \param [in] _fileName A persistent string with the file name.
		///		This reference is used in the update (DynamicReload()).
		/// \param [in] _type Which shader type is in the file.
		Shader( const wchar_t* _fileName, Type _type );

		~Shader();

		/// \brief Use this shader
		void Set();

#ifdef DYNAMIC_SHADER_RELOAD
		void DynamicReload();
#endif

	private:
		Type m_type;			///< Remember type for reloads and sets
		const wchar_t* m_fileName;	///< Name given in constructor

		void Load();			///< Loads the file m_fileName with m_type

		/// This is exactly one type of shader
		ID3DBlob* m_shader;
	};

} // namespace Graphic
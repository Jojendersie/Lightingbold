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
		/// \param [in] _outLayout Optional for geometry shaders which should
		///		be used with transform feedback.
		/// \param [in] _layoutSize Size of the layout descriptor 
		Shader( const wchar_t* _fileName, Type _type,
			D3D11_SO_DECLARATION_ENTRY* _outLayout = nullptr, int _layoutSize = 0);

		~Shader();

		/// \brief Use this shader
		void Set();

#ifdef DYNAMIC_SHADER_RELOAD
		void DynamicReload();
#endif

	private:
		Type m_type;			///< Remember type for reloads and sets
		const wchar_t* m_fileName;	///< Name given in constructor
#ifdef DYNAMIC_SHADER_RELOAD
		time_t m_lastModified;
#endif

		/// \brief Loads the file m_fileName with m_type
		/// \see Shader::Shader();
		void Load(D3D11_SO_DECLARATION_ENTRY* _outLayout = nullptr, int _layoutSize = 0);

		/// This is exactly one type of shader
		ID3DBlob* m_shader;
		union {
			ID3D11VertexShader* m_vertexShader;
			ID3D11GeometryShader* m_geometryShader;
			ID3D11PixelShader* m_pixelShader;
		};

		friend struct Vertex;
		friend struct PhotonVertex;
	};

	/// \brief A list of all shaders to be passed between game states.
	struct ShaderList {
		Shader* VSPassThrough;		///< Vertex shader which only passes the vertex info to the next stage

		Shader* GSQuad;				///< Create a rotated quad 
		Shader* GSSimulate;			///< Move a photon...

		Shader* PSBlob;				///< Draw a shaped object.

		ShaderList() : VSPassThrough(nullptr), GSQuad(nullptr), GSSimulate(nullptr), PSBlob(nullptr)	{}
		~ShaderList()
		{
			delete VSPassThrough;
			delete GSQuad;
			delete GSSimulate;
			delete PSBlob;
		}
	};

} // namespace Graphic
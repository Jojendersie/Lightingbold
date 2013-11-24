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
		void set();

#ifdef DYNAMIC_SHADER_RELOAD
		void dynamicReload();
#endif

	private:
		Type m_type;			///< Remember type for reloads and sets
		const wchar_t* m_fileName;	///< Name given in constructor
#ifdef DYNAMIC_SHADER_RELOAD
		time_t m_lastModified;
#endif
		D3D11_SO_DECLARATION_ENTRY* m_outLayoutDesc;	///< Optional set for geometry shaders
		int m_outLayoutNum;								///< Size of the refereced desc

		/// \brief Loads the file m_fileName with m_type
		/// \see Shader::Shader();
		void load();

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
		Shader* VSPassPhoton;		///< An other vertex shader to pass the photon vertex format.

		Shader* GSQuad;				///< Create a rotated quad
		Shader* GSInitPhotons;		///< Sample some photons at player locations
		Shader* GSSimulate;			///< Move a photon...

		Shader* PSBlob;				///< Draw a shaped object.
		Shader* PSPhoton;			///< Draw a point with some energy
		Shader* PSTexture;			///< Sample a texture
		Shader* PSRefractionMap;	///< Draw a refraction map
		Shader* PSBlur;				///< Gaußian blur

		ShaderList() : VSPassThrough(nullptr), GSQuad(nullptr), GSInitPhotons(nullptr), GSSimulate(nullptr), PSBlob(nullptr), PSPhoton(nullptr), VSPassPhoton(nullptr), PSRefractionMap(nullptr), PSBlur(nullptr)	{}
		~ShaderList()
		{
			delete VSPassThrough;
			delete VSPassPhoton;
			delete GSQuad;
			delete GSInitPhotons;
			delete GSSimulate;
			delete PSBlob;
			delete PSPhoton;
			delete PSTexture;
			delete PSRefractionMap;
			delete PSBlur;
		}
	};

} // namespace Graphic
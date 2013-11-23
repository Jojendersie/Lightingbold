#pragma once

#include "../predecl.hpp"

namespace Graphic {
	
	/// \brief A resource which can be set as rendertarget or read as texture.
	class RenderTarget
	{
	private:
		ID3D11ShaderResourceView* m_textureView;
		ID3D11RenderTargetView* m_targetView;

		/// \brief A viewport that maches the render target size.
		///
		D3D11_VIEWPORT* m_viewPort;
		void CreateViewPort( uint _width, uint _height );

		/// \brief A compatible depth buffer for this target.
		/// \details This can be a reference to a depth buffer off an other
		///		target. If this or the other target are setted one after the
		///		other no state change is made for the depth buffer.
		ID3D11DepthStencilView* m_depthBufferRef;
		ID3D11ShaderResourceView* m_DSTextureView;

		/// \brief The texture slot where this target is bound. A render target
		///		can never be bound to more than one slot.
		/// \details The default value if the target is not bound is 0xffffffff.
		mutable uint m_lastTextureSlot;
		mutable uint m_DSLastTextureSlot;

		// Make resource uncopyable
		RenderTarget( const RenderTarget& _RT );
		RenderTarget& operator=( const RenderTarget& _RT );
	public:
		struct CREATION_FLAGS {
			static const uint NO_DEPTH = 1;				///< Do not create a corresponding depth stencil view.
			static const uint NO_TARGET = 2;			///< Do not create a (color) render target - only a depth stencil target.
			static const uint TARGET_TEXTURE_VIEW = 4;	///< Allow read access to the render target.
			static const uint DEPTH_TEXTURE_VIEW = 8;	///< Allow read access for depth stencil view. Only available if depth stencil was not disabled.
		};

		/// \brief Create a render target with the given size and format.
		/// \param [in] _width A width which should satisfy the size
		///		constraints of the DirectX version. Most times the window size
		///		or a potence of two is useful.
		/// \param [in] _height A width which should satisfy the size
		///		constraints of the DirectX version. Most times the window size
		///		or a potence of two is useful.
		/// \param [in] _format An arbitrary allowed format for render targets.
		/// \param [in] _flags A combination of flags from the CREATION_FLAGS.
		/// \param [in] _initalData Data to be loaded if used as texture only.
		///		This parameter might be zero
		RenderTarget( uint _width, uint _height, DXGI_FORMAT _format,
					  uint _flags, const void* _initalData );

		/// \brief Fill this render target with already existing resources and
		///		give away the ownership of the resources.
		/// \details If this object is deleted it will release the given
		///		resources because it explicitly gets the ownership.
		/// \param [in] _targetView A valid view to a render target resource.
		/// \param [in] _depthStencil A compatible depth/stencil buffer for
		///		the current target.
		/// \param [in] _flags A combination of flags from CREATION_FLAGS\{NO_DEPTH}.
		RenderTarget( ID3D11RenderTargetView* _targetView,
					  ID3D11DepthStencilView* _depthStencil,
					  uint _flags );

		/// \brief A copy constructor which uses the reference counting.
		///
		//RenderTarget( const RenderTarget& _RT );

		~RenderTarget();

		/// \brief Set the texture to be read in the shader.
		/// \param [in] _slot Texture slot/id to determine on which shader
		///		register this texture should be set. The value has to be in
		///		[0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1].
		void SetAsTexture( uint _slot ) const;

		/// \brief Set the according depth buffer texture to be read in the shader.
		/// \param [in] _slot Texture slot/id to determine on which shader
		///		register this texture should be set. The value has to be in
		///		[0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1].
		void SetDSAsTexture( uint _slot ) const;

		/// \brief Set as current target (single target only).
		/// \details This method just forgetts what was before. Make sure you
		///		can reset the target.
		///
		///		This will automatically unbind the texture from any texture slot.
		void SetAsTarget() const;

		/// \brief Overwrites this target with a constant color.
		/// \param [in] _color Has to be a valid 4 component float array.
		void Clear( const float* _color );

		/// \brief Clears the corresponding depth buffer.
		/// \details This target should be set before.
		//void ClearZ( float _depth );

		uint GetWidth() const;
		uint GetHeight() const;
	};


	/// \brief A list of all targets to be passed between game states.
	struct RenderTargetList {
		RenderTarget* BackBuffer;
		RenderTarget* PhotonMap;

		RenderTargetList() : BackBuffer(nullptr), PhotonMap(nullptr)	{}
		~RenderTargetList()
		{
			delete BackBuffer;
			delete PhotonMap;
		}
	};
}
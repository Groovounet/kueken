#ifndef KUEKEN_BUFFER_INCLUDED
#define KUEKEN_BUFFER_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	struct env
	{
		struct version
		{
			int Major; //glGetIntegerv(GL_MAJOR_VERSION, &MinorVersion);
			int Minor; //glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
		};
		
		version Version;
		
		struct texture
		{
			int MaxTextureSize; //GL_MAX_TEXTURE_SIZE
			int MaxTexture3DSize; //GL_MAX_3D_TEXTURE_SIZE
			int MaxTextureBufferSize; //GL_MAX_TEXTURE_BUFFER_SIZE
			int MaxTextureCubeSize; //GL_MAX_CUBE_MAP_TEXTURE_SIZE
			int MaxTextureRectSize; //GL_MAX_RECTANGLE_TEXTURE_SIZE
			int MaxRenderbufferSize; //GL_MAX_RENDERBUFFER_SIZE
			int MaxTextureArrayTextureLayers; //GL_MAX_ARRAY_TEXTURE_LAYERS
		};
		
		texture Texture;
		
		struct program
		{
			struct uniform
			{
				int MaxBlockSize; // GL_MAX_UNIFORM_BLOCK_SIZE
				int MaxBufferBinding; // GL_MAX_UNIFORM_BUFFER_BINDINGS
				int MaxSubroutines; // GL_MAX_SUBROUTINES
				int MaxSubroutineLocations; // GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS
			};
			
			uniform Uniform;
		
			struct vertex
			{
				int MaxAttribs; // GL_MAX_VERTEX_ATTRIBS
				int MaxOutputComponents; // GL_MAX_VERTEX_OUTPUT_COMPONENTS
				int MaxUniformComponents; // GL_MAX_VERTEX_UNIFORM_COMPONENTS
				int MaxUniformBlocks; // GL_MAX_VERTEX_UNIFORM_BLOCKS
				int MaxImageUnits; // GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
				int MaxAtomicCounters; // GL_MAX_VERTEX_ATOMIC_COUNTERS
				int MaxAtomicCounterBuffers; // GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS
			};
			
			vertex Vertex;
			
			struct patch
			{
				int MaxInputComponents; // GL_MAX_TESS_CONTROL_INPUT_COMPONENTS
				int MaxOutputComponents; // GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS
				int MaxUniformComponents; // GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS	
				int MaxUniformBlocks; // GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS
				int MaxImageUnits; // GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS
				int MaxAtomicCounters; // GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS
				int MaxAtomicCounterBuffers; // GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS
			};
			
			patch Patch;
			
			struct domain
			{
				int MaxInputComponents; // GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS
				int MaxOutputComponents; // GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS
				int MaxUniformComponents; // GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS			
				int MaxUniformBlocks; // GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS
				int MaxImageUnits; // GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS
				int MaxAtomicCounters; // GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS
				int MaxAtomicCounterBuffers; // GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS
			};
			
			domain Domain;
				
			struct primitive
			{
				int MaxInputComponents; // GL_MAX_GEOMETRY_INPUT_COMPONENTS
				int MaxOutputComponents; // GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
				int MaxUniformComponents; // GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
				int MaxUniformBlocks; // GL_MAX_GEOMETRY_UNIFORM_BLOCKS
				int MaxImageUnits; // GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
				int MaxAtomicCounters; // GL_MAX_GEOMETRY_ATOMIC_COUNTERS
				int MaxAtomicCounterBuffers; // GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS
				int MaxStreams; // GL_MAX_VERTEX_STREAMS
			};
			
			primitive Primitive;
			
			struct fragment
			{
				int MaxInputComponents; // GL_MAX_FRAGMENT_INPUT_COMPONENTS
				int MaxUniformComponents; // GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
				int MaxUniformBlocks; // GL_MAX_FRAGMENT_UNIFORM_BLOCKS
				int MaxImageUnits; // GL_MAX_FRAGMENT_TEXTURE_IMAGE_UNITS
				int MaxAtomicCounters; // GL_MAX_FRAGMENT_ATOMIC_COUNTERS
				int MaxAtomicCounterBuffers; // GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS
			};
			
			fragment Fragment;
		};
		
		program Program;
	};
	
	env readEnv();
}//namespace kueken

#endif//KUEKEN_BUFFER_INCLUDED

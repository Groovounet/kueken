#ifndef KUEKEN_BUFFER_INCLUDED
#define KUEKEN_BUFFER_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	struct env
	{
		// MAX_CLIP_DISTANCES
		// MAX_VIEWPORT_DIMS // float
		// MAX_VIEWPORTS
		// VIEWPORT_SUBPIXEL_BITS
		// VIEWPORT_BOUNDS_RANGE // float
		// LAYER_PROVOKING_VERTEX
		// VIEWPORT_INDEX_PROVOKING_VERTEX
		// POINT_SIZE_RANGE // float
		// POINT_SIZE_GRANULARITY // float
		// ALIASED_LINE_WIDTH_RANGE
		// SMOOTH_LINE_WIDTH_RANGE
		// SMOOTH_LINE_WIDTH_GRANULARITY
		// MAX_ELEMENTS_INDICES
		// MAX_ELEMENTS_VERTICES
		// COMPRESSED TEXTURE FORMATS
		// NUM COMPRESSED TEXTURE FORMATS
		// PROGRAM BINARY FORMATS
		// NUM PROGRAM BINARY FORMATS
		// SHADER BINARY FORMATS
		// NUM SHADER BINARY FORMATS
		// SHADER COMPILER
		// MIN MAP BUFFER ALIGNMENT
		// EXTENSIONS
		// NUM EXTENSIONS
		// CONTEXT FLAGS
		// SHADING LANGUAGE VERSION
		// MAX VERTEX ATTRIBS
		// MAX VERTEX UNIFORM VECTORS
		// MAX TESS GEN LEVEL
		// MAX PATCH VERTICES
		// MAX GEOMETRY TOTAL OUTPUT COMPONENTS
		// MAX GEOMETRY SHADER INVOCATIONS
		// MAX TEXTURE IMAGE UNITS
		// MIN PROGRAM TEXTURE GATHER OFFSET
		// MAX PROGRAM TEXTURE GATHER OFFSET
		// MIN PROGRAM TEXEL OFFSET
		// MAX PROGRAM TEXEL OFFSET
		// UNIFORM BUFFER OFFSET ALIGNMENT
		// MAX COMBINED TEXTURE IMAGE UNITS
		// MAX COMBINED IMAGE UNITS AND FRAGMENT OUTPUTS
		// MAX COMBINED IMAGE UNIFORMS
		// MAX SAMPLE MASK WORDS
		// MAX SAMPLES
		// MAX COLOR TEXTURE SAMPLES
		// MAX DEPTH TEXTURE SAMPLES
		// MAX INTEGER SAMPLES
		// QUERY COUNTER BITS
		// MAX SERVER WAIT TIMEOUT
		// MIN FRAGMENT INTERPOLATION OFFSET // float
		// MAX FRAGMENT INTERPOLATION OFFSET // float
		// FRAGMENT INTERPOLATION OFFSET BITS
		
	
		struct version
		{
			int Major; //glGetIntegerv(GL_MAJOR_VERSION, &MinorVersion);
			int Minor; //glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
			// RENDERER
			// VENDOR
			// VERSION
		};
		
		version Version;
		
		struct texture
		{
			int MaxSize; //GL_MAX_TEXTURE_SIZE
			int Max3DSize; //GL_MAX_3D_TEXTURE_SIZE
			int MaxBufferSize; //GL_MAX_TEXTURE_BUFFER_SIZE
			int MaxCubeSize; //GL_MAX_CUBE_MAP_TEXTURE_SIZE
			int MaxRectSize; //GL_MAX_RECTANGLE_TEXTURE_SIZE
			int MaxRenderbufferSize; //GL_MAX_RENDERBUFFER_SIZE
			int MaxArrayLayers; //GL_MAX_ARRAY_TEXTURE_LAYERS
			int MaxLodBias; //MAX_TEXTURE_LOD_BIAS
		};
		
		texture Texture;
		
		struct image
		{
			int maxUnits; //GL_MAX_IMAGE_UNITS
			int maxSamples; //GL_MAX_IMAGE_SAMPLES
		};
		
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
				int MaxCombinedUniformComponents; // GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS
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
				int MaxCombinedUniformComponents; // GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS
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
				int MaxCombinedUniformComponents; // GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS
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
				int MaxCombinedUniformComponents; // GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
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
				int MaxCombinedUniformComponents; // GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS
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

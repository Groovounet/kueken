namespace glv{
namespace detail
{
	inline mesh buildPlaneElement
	(
		glm::uint channels,
		glm::vec2 const & Size
	)
	{
		static const glm::uint VertexCount = 4;
		static const glm::uint ElementCount = 6;

		static const glm::uint PositionSize = VertexCount * sizeof(glm::vec2);
		static const glm::vec2 PositionData[VertexCount] =
		{
			glm::vec2(-1.0f,-1.0f),
			glm::vec2( 1.0f,-1.0f),
			glm::vec2( 1.0f, 1.0f),
			glm::vec2(-1.0f, 1.0f)
		};

		glm::vec2 PositionCopy[VertexCount];
		for(std::size_t i = 0; i < VertexCount; ++i)
			PositionCopy[i] = PositionData[i] * Size;

		static const glm::uint TexcoordSize = VertexCount * sizeof(glm::vec2);
		static const glm::vec2 TexcoordData[VertexCount] =
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};

		static const glm::uint NormalSize = VertexCount * sizeof(glm::vec3);
		static const glm::vec3 NormalData[VertexCount] =
		{
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		static const glm::uint TangentSize = VertexCount * sizeof(glm::vec3);
		static const glm::vec3 TangentData[VertexCount] =
		{
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		};

		static const glm::uint BiTangentSize = VertexCount * sizeof(glm::vec3);
		static const glm::vec3 BiTangentData[VertexCount] =
		{
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		};

		static const glm::uint ElementSize = ElementCount * sizeof(glm::uint16);
		static const glm::uint16 ElementData[ElementCount] =
		{
			0, 1, 2, 2, 3, 0
		};

		format Format;
		assert(channels & POSITION2_BIT);
		Format.setAttribute(SLOT0, POSITION2, FLOAT32);
		if(channels & TEXCOORD_BIT)
			Format.setAttribute(SLOT0, TEXCOORD, FLOAT32);
		if(channels & NORMAL_BIT)
			Format.setAttribute(SLOT0, NORMAL, FLOAT32);
		if(channels & TANGENT_BIT)
			Format.setAttribute(SLOT0, TANGENT, FLOAT32);
		if(channels & BINORMAL_BIT)
			Format.setAttribute(SLOT0, BINORMAL, FLOAT32);
		if(channels & ELEMENT_BIT)
			Format.setElement(UINT16);

		mesh Mesh(Format, VertexCount, ElementCount);

		Mesh.setArray(SLOT0, POSITION2, PositionSize, &PositionCopy[0]);
		if(channels & TEXCOORD_BIT)
			Mesh.setArray(SLOT0, TEXCOORD, TexcoordSize, &TexcoordData[0]);
		if(channels & NORMAL_BIT)
			Mesh.setArray(SLOT0, NORMAL, NormalSize, &NormalData[0]);
		if(channels & TANGENT_BIT)
			Mesh.setArray(SLOT0, TANGENT, TangentSize, &TangentData[0]);
		if(channels & BINORMAL_BIT)
			Mesh.setArray(SLOT0, BINORMAL, BiTangentSize, &BiTangentData[0]);

		Mesh.setElement(ElementSize, ElementData);

		return Mesh;
	}

	inline mesh buildPlaneArray
	(
		glm::uint channels,
		glm::vec2 const & Size
	)
	{
		static const glm::uint VertexCount = 6;

		static const glm::uint PositionSize = VertexCount * sizeof(glm::vec2);
		static const glm::vec2 PositionData[VertexCount] =
		{
			glm::vec2(-1.0f,-1.0f),
			glm::vec2( 1.0f,-1.0f),
			glm::vec2( 1.0f, 1.0f),
			glm::vec2( 1.0f, 1.0f),
			glm::vec2(-1.0f, 1.0f),
			glm::vec2(-1.0f,-1.0f)
		};

		glm::vec2 PositionCopy[VertexCount];
		for(std::size_t i = 0; i < VertexCount; ++i)
			PositionCopy[i] = PositionData[i] * Size;

		static const glm::uint TexcoordSize = VertexCount * sizeof(glm::vec2);
		static const glm::vec2 TexcoordData[VertexCount] =
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f)
		};

		static const glm::uint NormalSize = VertexCount * sizeof(glm::vec3);
		static const glm::vec3 NormalData[VertexCount] =
		{
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		static const glm::uint TangentSize = VertexCount * sizeof(glm::vec3);
		static const glm::vec3 TangentData[VertexCount] =
		{
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		};

		static const glm::uint BiTangentSize = VertexCount * sizeof(glm::vec3);
		static const glm::vec3 BiTangentData[VertexCount] =
		{
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		};

		format Format;
		assert(channels & POSITION2_BIT);
		Format.setAttribute(SLOT0, POSITION2, FLOAT32);
		if(channels & TEXCOORD_BIT)
			Format.setAttribute(SLOT0, TEXCOORD, FLOAT32);
		if(channels & NORMAL_BIT)
			Format.setAttribute(SLOT0, NORMAL, FLOAT32);
		if(channels & TANGENT_BIT)
			Format.setAttribute(SLOT0, TANGENT, FLOAT32);
		if(channels & BINORMAL_BIT)
			Format.setAttribute(SLOT0, BINORMAL, FLOAT32);

		mesh Mesh(Format, VertexCount, 0);
		Mesh.setArray(SLOT0, POSITION2, PositionSize, &PositionCopy[0]);
		if(channels & TEXCOORD_BIT)
			Mesh.setArray(SLOT0, TEXCOORD, TexcoordSize, &TexcoordData[0]);
		if(channels & NORMAL_BIT)
			Mesh.setArray(SLOT0, NORMAL, NormalSize, &NormalData[0]);
		if(channels & TANGENT_BIT)
			Mesh.setArray(SLOT0, TANGENT, TangentSize, &TangentData[0]);
		if(channels & BINORMAL_BIT)
			Mesh.setArray(SLOT0, BINORMAL, BiTangentSize, &BiTangentData[0]);

		return Mesh;
	}

	inline mesh buildPlaneArray
	(
		glm::uint channels,
		glm::vec2 const & Size,
		glm::uvec2 const & Subdivision
	)
	{
		glm::uint VertexCount = 6 * Subdivision.x * Subdivision.y;

		glm::uint PositionSize = VertexCount * sizeof(glm::vec2);
		std::vector<glm::vec2> PositionData;//(VertexCount);

		for(std::size_t j = 0; j < Size.y * Subdivision.y; ++j)
		for(std::size_t i = 0; i < Size.x * Subdivision.x; ++i)
		{
			glm::vec2 p00 = glm::mix(
				glm::vec2(-1.0f), 
				glm::vec2( 1.0f), 
				glm::vec2(float(i + 0) / float(Size.x * Subdivision.x), float(j + 0) / float(Size.y * Subdivision.y)));

			glm::vec2 p01 = glm::mix(
				glm::vec2(-1.0f), 
				glm::vec2( 1.0f), 
				glm::vec2(float(i + 1) / float(Size.x * Subdivision.x), float(j + 0) / float(Size.y * Subdivision.y)));

			glm::vec2 p11 = glm::mix(
				glm::vec2(-1.0f), 
				glm::vec2( 1.0f), 
				glm::vec2(float(i + 1) / float(Size.x * Subdivision.x), float(j + 1) / float(Size.y * Subdivision.y)));

			glm::vec2 p10 = glm::mix(
				glm::vec2(-1.0f), 
				glm::vec2( 1.0f), 
				glm::vec2(float(i + 0) / float(Size.x * Subdivision.x), float(j + 1) / float(Size.y * Subdivision.y)));

			PositionData.push_back(p00 * Size);
			PositionData.push_back(p01 * Size);
			PositionData.push_back(p11 * Size);
			PositionData.push_back(p11 * Size);
			PositionData.push_back(p10 * Size);
			PositionData.push_back(p00 * Size);
		}

		glm::uint TexcoordSize = VertexCount * sizeof(glm::vec2);
		std::vector<glm::vec2> TexcoordData;//(VertexCount);

		for(std::size_t j = 0; j < Size.y * Subdivision.y; ++j)
		for(std::size_t i = 0; i < Size.x * Subdivision.x; ++i)
		{
			glm::vec2 p00 = glm::mix(
				glm::vec2( 0.0f), 
				glm::vec2( 1.0f), 
				glm::vec2(float(i + 0) / float(Size.x * Subdivision.x), float(j + 0) / float(Size.y * Subdivision.y)));

			glm::vec2 p01 = glm::mix(
				glm::vec2( 0.0f), 
				glm::vec2( 1.0f), 
				glm::vec2(float(i + 1) / float(Size.x * Subdivision.x), float(j + 0) / float(Size.y * Subdivision.y)));

			glm::vec2 p11 = glm::mix(
				glm::vec2( 0.0f), 
				glm::vec2( 1.0f), 
				glm::vec2(float(i + 1) / float(Size.x * Subdivision.x), float(j + 1) / float(Size.y * Subdivision.y)));

			glm::vec2 p10 = glm::mix(
				glm::vec2( 0.0f), 
				glm::vec2( 1.0f), 
				glm::vec2(float(i + 0) / float(Size.x * Subdivision.x), float(j + 1) / float(Size.y * Subdivision.y)));

			TexcoordData.push_back(p00);
			TexcoordData.push_back(p01);
			TexcoordData.push_back(p11);
			TexcoordData.push_back(p11);
			TexcoordData.push_back(p10);
			TexcoordData.push_back(p00);
		}

		glm::uint NormalSize = VertexCount * sizeof(glm::vec3);
		std::vector<glm::vec3> NormalData;//(VertexCount);
		for(std::size_t j = 0; j < Size.y * Subdivision.y; ++j)
		for(std::size_t i = 0; i < Size.x * Subdivision.x; ++i)
		{
			for(std::size_t c = 0; c < 6; ++c)
				NormalData.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		}

		glm::uint TangentSize = VertexCount * sizeof(glm::vec3);
		std::vector<glm::vec3> TangentData;//(VertexCount);
		for(std::size_t j = 0; j < Size.y * Subdivision.y; ++j)
		for(std::size_t i = 0; i < Size.x * Subdivision.x; ++i)
		{
			for(std::size_t c = 0; c < 6; ++c)
				TangentData.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
		}


		glm::uint BiTangentSize = VertexCount * sizeof(glm::vec3);
		std::vector<glm::vec3> BiTangentData;//(VertexCount);
		for(std::size_t j = 0; j < Size.y * Subdivision.y; ++j)
		for(std::size_t i = 0; i < Size.x * Subdivision.x; ++i)
		{
			for(std::size_t c = 0; c < 6; ++c)
				TangentData.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		format Format;
		assert(channels & POSITION2_BIT);
		Format.setAttribute(SLOT0, POSITION2, FLOAT32);
		if(channels & TEXCOORD_BIT)
			Format.setAttribute(SLOT0, TEXCOORD, FLOAT32);
		if(channels & NORMAL_BIT)
			Format.setAttribute(SLOT0, NORMAL, FLOAT32);
		if(channels & TANGENT_BIT)
			Format.setAttribute(SLOT0, TANGENT, FLOAT32);
		if(channels & BINORMAL_BIT)
			Format.setAttribute(SLOT0, BINORMAL, FLOAT32);

		mesh Mesh(Format, VertexCount, 0);
		Mesh.setArray(SLOT0, POSITION2, PositionSize, &PositionData[0]);
		if(channels & TEXCOORD_BIT)
			Mesh.setArray(SLOT0, TEXCOORD, TexcoordSize, &TexcoordData[0]);
		if(channels & NORMAL_BIT)
			Mesh.setArray(SLOT0, NORMAL, NormalSize, &NormalData[0]);
		if(channels & TANGENT_BIT)
			Mesh.setArray(SLOT0, TANGENT, TangentSize, &TangentData[0]);
		if(channels & BINORMAL_BIT)
			Mesh.setArray(SLOT0, BINORMAL, BiTangentSize, &BiTangentData[0]);

		return Mesh;
	}

}//namespace detail

	inline mesh buildPlane
	(
		glm::uint channels,
		glm::vec2 const & Size
	)
	{
		if(channels & glv::ELEMENT_BIT)
			return detail::buildPlaneElement(channels, Size);
		else
			return detail::buildPlaneArray(channels, Size);
	}

	inline mesh buildPlane
	(
		glm::uint channels,
		glm::vec2 const & Size,
		glm::uvec2 const & Subdivision
	)
	{
		assert(!(channels & glv::ELEMENT_BIT));
//		if(channels & glv::ELEMENT_BIT)
//			return detail::buildPlaneElement(channels, Size, Subdivision);
//		else
			return detail::buildPlaneArray(channels, Size, Subdivision);
	}

}//namespace glv

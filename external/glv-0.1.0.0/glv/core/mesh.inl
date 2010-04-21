namespace glv{
namespace detail{

inline std::size_t getTypeSize
(
	type Type
)
{
	static std::size_t Size[TYPE_MAX] =
	{
		0,						// TYPE_NULL
		sizeof(glm::float16),	// FLOAT16
		sizeof(glm::float32),	// FLOAT32
		sizeof(glm::int8),		// INT8
		sizeof(glm::int16),		// INT16
		sizeof(glm::int32),		// INT32
		sizeof(glm::uint8),		// UINT8
		sizeof(glm::uint16),	// UINT16
		sizeof(glm::uint32)		// UINT32
	};

	return Size[Type];
}

inline std::size_t getChannelSize
(
	channel Channel
)
{
	static std::size_t Size[CHANNEL_MAX] =
	{
		1,	// ELEMENT	
		2,	// POSITION2
		3,	// POSITION3
		3,	// COLOR3
		4,	// COLOR4
		3,	// NORMAL
		3,	// TANGENT
		3,	// BINORMAL
		2	// TEXCOORD
	};

	return Size[Channel];
}

}//namespace detail

/////////
// Format

inline format::format() :
	Attributes(CHANNEL_MAX),
	Primitive(TRIANGLES)
{}

inline void format::setElement
(
	type Type
)
{
	Element.Type = Type;
}

inline void format::setPrimitive
(
	primitive Primitive
)
{
	this->Primitive = Primitive;
}

inline std::size_t format::size() const
{
	std::size_t Size = 0;
	for(std::size_t i = 0; i < Attributes.size(); ++i)
		Size += detail::getChannelSize(channel(i)) * detail::getTypeSize(Attributes[i].Type);
	return Size;
}

inline void format::build()
{
	for(std::size_t i = 0; i < Attributes.size(); ++i)
		Attributes[i].Component = detail::getChannelSize(channel(i));

	Attributes[0].Offset = 0;
	for(std::size_t i = 1; i < Attributes.size(); ++i)
		Attributes[i].Offset += Attributes[i - 1].Offset + Attributes[i - 1].Component * detail::getTypeSize(Attributes[i - 1].Type);
}

//inline void format::build_format()
//{
//	for(std::size_t Slot = 0; Slot < Slots.size(); ++Slot)
//	for(std::size_t Attribute = 0; Attribute < Attributes.size(); ++Attribute)
//	{
//		if(Attributes[Attribute].Slot != Slot)
//			continue;
//
//		glm::uint CompSize = Attributes[Attribute].Component;
//		glm::uint TypeSize = detail::getTypeSize(Attributes[Attribute].Type);
//		Slots[Slot].Stride = CompSize * TypeSize;
//	}
//}

///////////
// mesh

inline mesh::mesh()
{}

inline mesh::mesh
(
	format const & Format, 
	std::size_t VertexCount, 
	std::size_t ElementCount
) :
	Format(Format),
	VertexCount(VertexCount),
	ElementCount(ElementCount)
{
	this->Format.build();
	if(VertexCount > 0)
	for(std::size_t Slot = 0; Slot < VertexData.size(); ++Slot)
	{
		if(vertexSize(glv::slot(Slot)) == 0)
			continue;
		VertexData[Slot].reset(new glm::byte[vertexSize(slot(Slot))]);
	}

	if(ElementCount > 0)
	{
		std::size_t ElementSize = elementSize();
		ElementData.reset(new glm::byte[ElementSize]);
	}
}

inline bool mesh::has(channel Channel) const
{
	return Format.Attributes[Channel].Type != TYPE_NULL;
}

inline std::size_t mesh::stride(glv::slot Slot) const
{
	std::size_t Stride = 0;
	for(std::size_t Channel = 0; Channel < Format.Attributes.size(); ++Channel)
	{
		if(Format.Attributes[Channel].Slot != Slot)
			continue;
		if(Format.Attributes[Channel].Type == TYPE_NULL)
			continue;
		Stride += Format.Attributes[Channel].Component * detail::getTypeSize(Format.Attributes[Channel].Type);
	}
	return Stride;
}

inline glv::type mesh::type(channel Channel) const
{
	return Format.Attributes[Channel].Type;
}

inline bool mesh::normalized(channel Channel) const
{
	return Format.Attributes[Channel].Normalized;
}

inline std::size_t mesh::component(channel Channel) const
{
	return Format.Attributes[Channel].Component;
}

inline std::size_t mesh::offset(channel Channel) const
{
	return Format.Attributes[Channel].Offset;
}

inline void mesh::setArray
(
	glv::slot Slot, 
	channel Channel, 
	std::size_t Size, 
	void const * const Data
)
{
	assert(Format.Attributes[Channel].Type != TYPE_NULL);

	glm::vec2* TestSrc = (glm::vec2*)Data;
	glm::vec2* TestDst = (glm::vec2*)VertexData[Slot].get();

	std::size_t Offset = Format.Attributes[Channel].Offset;
	std::size_t ElementSize = detail::getChannelSize(Channel) * detail::getTypeSize(Format.Attributes[Channel].Type);
	std::size_t Count = Size / ElementSize;
	for(std::size_t Element = 0; Element < Count; ++Element)
	{
		std::size_t Stride = stride(Slot);
		std::size_t DstIndex = Element * Stride + Offset;
		std::size_t SrcIndex = Element * ElementSize;

		memcpy(
			VertexData[Slot].get() + DstIndex,
			(glm::byte*)Data + SrcIndex,
			ElementSize);
	}
}

inline void mesh::setElement
(
	std::size_t Size, 
	void const * const Data
)
{
	memcpy(ElementData.get(), Data, Size);
}

//inline void mesh::setVertex
//(
//	std::size_t Index, 
//	void const * const Data
//)
//{
//	assert(Index < vertexSize(SLOT0) / stride(SLOT0)); // Vertex count
//
//	std::size_t DataOffset = 0;
//	for(std::size_t Channel = 0; Channel < CHANNEL_MAX; ++Channel)
//	{
//		if(Format.Attributes[Channel].Type == TYPE_NULL)
//			continue;
//		std::size_t DstIndex = Index * stride(slot(0));
//		memcpy(
//			VertexData[0].get() + DstIndex,
//			static_cast<glm::byte const * const>(Data) + DataOffset,
//			stride(slot(0)));
//	}
//}

inline void const * const mesh::vertexData(glv::slot Slot) const
{
	return VertexData[Slot].get();
}

inline void * mesh::vertexData(slot Slot)
{
	return VertexData[Slot].get();
}

inline void const * const mesh::elementData() const
{
	return ElementData.get();
}

inline void * mesh::elementData()
{
	return ElementData.get();
}

inline std::size_t mesh::vertexSize(glv::slot Slot) const
{
	//std::size_t Size = 0;

	//for(std::size_t i = 0; i < Format.Attributes.size(); ++i)
	//{
	//	if(Format.Attributes[i].Slot != Slot)
	//		continue;
	//	Size += ;
	//}

	return VertexCount * stride(Slot);
}

inline std::size_t mesh::vertexCount() const
{
	return VertexCount;
}

inline std::size_t mesh::elementCount() const
{
	return ElementCount;
}

inline std::size_t mesh::elementSize() const
{
	return ElementCount * detail::getTypeSize(Format.Element.Type);
}

inline std::string mesh::error() const
{
	return Error;
}

}//namespace glv

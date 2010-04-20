#ifndef GLG_MESH_INCLUDED
#define GLG_MESH_INCLUDED

// GLM
#include "glm/glm.hpp"
#include "glm/gtx/raw_data.hpp"
#include "glm/gtx/number_precision.hpp"

// STD
#include <string>
#include <vector>

// Boost
#include <boost/shared_array.hpp>
#include <boost/array.hpp>

namespace glv
{
	enum type
	{
		TYPE_NULL,
		FLOAT16,
		FLOAT32,
		INT8,
		INT16,
		INT32,
		UINT8,
		UINT16,
		UINT32,
		TYPE_MAX
	};

	enum channel
	{
		ELEMENT,
		POSITION2,
		POSITION3,
		POSITION = POSITION3,
		COLOR3,
		COLOR4,
		COLOR = COLOR4, 
		NORMAL,
		TANGENT,
		BINORMAL,
		TEXCOORD,
		CHANNEL_MAX
	};

	enum channel_bit
	{
		ELEMENT_BIT = (1 << ELEMENT),
		POSITION2_BIT = (1 << POSITION2),
		POSITION3_BIT = (1 << POSITION3),
		POSITION_BIT = (1 << POSITION),
		COLOR3_BIT = (1 << COLOR3),
		COLOR4_BIT = (1 << COLOR4),
		COLOR_BIT = (1 << COLOR), 
		NORMAL_BIT = (1 << NORMAL),
		TANGENT_BIT = (1 << TANGENT),
		BINORMAL_BIT = (1 << BINORMAL),
		TEXCOORD_BIT = (1 << TEXCOORD),
	};

	enum attrib
	{
		ATTRIB0,
		ATTRIB1,
		ATTRIB2,
		ATTRIB3,
		ATTRIB4,
		ATTRIB5,
		ATTRIB6,
		ATTRIB7,
		ATTRIB_MAX
	};

	enum slot
	{
		SLOT0,
		SLOT1,
		SLOT2,
		SLOT3,
		SLOT4,
		SLOT5,
		SLOT6,
		SLOT7,
		SLOT_MAX
	};

	enum primitive
	{
		TRIANGLES
	};

	class format
	{
		friend class mesh;

	public:
		format();

		void setAttribute(
			glv::slot Slot,
			attrib Attrib,
			std::size_t Component, 
			type Type);

		void setAttribute(
			glv::slot Slot,
			channel Channel, 
			type Type)
		{
			attribute Attribute;
			Attribute.Slot = Slot;
			Attribute.Type = Type;
			Attributes[Channel] = Attribute;
		}

		void setElement(
			type Type);

		void setPrimitive(
			primitive Primitive);

	private:
		void build();
		void build_format();
		std::size_t size() const;

		struct attribute
		{
			attribute() :
				Type(TYPE_NULL),
				Slot(SLOT0),
				Offset(0),
				Component(0),
				Normalized(false)
			{}

			type Type;
			glv::slot Slot;
			glm::uint Offset;
			glm::uint Component;
			bool Normalized;
		};

		struct slot
		{
			slot() :
				Stride(0)
			{}

			glm::uint Stride;
		};

		std::vector<attribute> Attributes;
		//std::vector<slot> Slots;
		attribute Element;
		
		primitive Primitive;
	};

	class mesh
	{
	public:
		mesh();
		mesh(
			format const & Format, 
			std::size_t VertexCount, 
			std::size_t ElementCount);

		bool has(channel Channel) const; 
		glv::type type(channel Channel) const;
		bool normalized(channel Channel) const;
		std::size_t offset(channel Channel) const;
		std::size_t stride(glv::slot Slot) const;
		std::size_t component(channel Channel) const;
		void setArray(glv::slot Slot, channel Channel, std::size_t Size, void const * const Data);
		//void setVertex(std::size_t Index, void const * const Data);
		void setAttrib(glv::slot Slot, std::size_t Index, void const * const Data);
		void setElement(std::size_t Size, void const * const Data);

		void const * const vertexData(glv::slot Slot) const;
		void * vertexData(glv::slot Slot);
		std::size_t vertexSize(slot Slot) const;
		std::size_t vertexCount() const;

		void const * const elementData() const;
		void * elementData();
		std::size_t elementSize() const;
		std::size_t elementCount() const;

		std::string error() const;

	private:
		format Format;
		boost::array<boost::shared_array<glm::byte>, SLOT_MAX> VertexData;
		boost::shared_array<glm::byte> ElementData;
		std::size_t VertexCount;
		std::size_t ElementCount;
		std::string Error;
	};

}//namespace glv

#include "mesh.inl"

#endif//GLG_MESH_INCLUDED

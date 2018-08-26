#include "OBJPassthrough.h"
#include <Graphics\Renderer_Interface.h>
struct membuf : std::streambuf
{
	membuf(char* begin, char* end) {
		this->setg(begin, begin, end);
	}
	pos_type seekoff(off_type off,
		std::ios_base::seekdir dir,
		std::ios_base::openmode which = std::ios_base::in) {
		if (dir == std::ios_base::cur)
			gbump(int(off));
		else if (dir == std::ios_base::end)
			setg(eback(), egptr() + off, egptr());
		else if (dir == std::ios_base::beg)
			setg(eback(), eback() + off, egptr());
		return gptr() - eback();
	}
	pos_type seekpos(pos_type sp, std::ios_base::openmode which) override {
		return seekoff(sp - pos_type(off_type(0)), std::ios_base::beg, which);
	}
};
DLL_EXPORT Utilities::Error Parse(uint32_t guid, void * data, uint64_t size, void ** parsedData, uint64_t * parsedSize)
{
	if (data == nullptr)
		RETURN_ERROR_EX("No data to parse: ", guid);

	ObjParser::Interpreter parser;
	membuf buf((char*)data, (char*)data + size);
	istream stream(&buf);
	auto res = parser.parse(&stream);
	if (res)
		RETURN_ERROR_EX("Could not parse resource: ", guid);

	*parsedData = parser.GetData();
	*parsedSize = (*(ArfData::ArfData*)parsedData).data.allocated + sizeof(ArfData::ArfData);
	RETURN_SUCCESS;
}

DLL_EXPORT Utilities::Error DestroyParsedData(uint32_t guid, void * data, uint64_t size)
{
	operator delete(data);
	RETURN_SUCCESS;
}
struct MeshInfo
{
	uint8_t numSubmeshes;
	ArfData::SubMesh* subMeshes;
};
DLL_EXPORT Utilities::Error Load(uint32_t guid, void * data, uint64_t size, void ** loadedDataRAM, uint64_t * loadedSizeRAM, uint64_t * loadedSizeVRAM)
{
	auto ph = Graphics::Get()->GetPipelineHandler();

	ArfData::ArfData& adata = *(ArfData::ArfData*)data;
	*loadedSizeRAM = sizeof(uint8_t) + sizeof(ArfData::SubMesh)*adata.data.NumSubMesh;
	auto ldata = new MeshInfo();
	ldata->numSubmeshes = adata.data.NumSubMesh;
	ldata->subMeshes = new ArfData::SubMesh[ldata->numSubmeshes];
	memcpy(ldata->subMeshes, adata.pointers.subMesh, sizeof(ArfData::SubMesh)*adata.data.NumSubMesh);

	*loadedDataRAM = ldata;
	*loadedSizeVRAM = 0;
	if (adata.data.NumPos == 0)
		RETURN_ERROR_EX("Mesh with no positions", guid);
	if(adata.data.NumFace == 0)
		RETURN_ERROR_EX("Mesh with no faces", guid);
	PASS_IF_ERROR(
		ph->CreateBuffer(
			Utilities::GUID(guid) + Utilities::GUID("Positions"),
			Graphics::Pipeline::Buffer::StructuredBuffer(adata.pointers.positions, sizeof(ArfData::Position), adata.data.NumPos)));
	
	*loadedSizeVRAM += sizeof(ArfData::Position)*adata.data.NumPos;
	

	if (adata.data.NumNorm > 0)
	{
		auto& r = ph->CreateBuffer(
			Utilities::GUID(guid) + Utilities::GUID("Normals"),
			Graphics::Pipeline::Buffer::StructuredBuffer(adata.pointers.normals, sizeof(ArfData::Normal), adata.data.NumNorm));
		if (r.hash != "Success"_hash)
		{
			ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("Positions"));
			return r;
		}
		*loadedSizeVRAM += sizeof(ArfData::Normal)*adata.data.NumNorm;
	}
	if (adata.data.NumTex > 0)
	{
		auto& r = ph->CreateBuffer(
			Utilities::GUID(guid) + Utilities::GUID("TexCoords"),
			Graphics::Pipeline::Buffer::StructuredBuffer(adata.pointers.texCoords, sizeof(ArfData::TexCoord), adata.data.NumTex));
		if (r.hash != "Success"_hash)
		{
			ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("Positions"));
			ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("Normals"));
			return r;
		}
	
	*loadedSizeVRAM += sizeof(ArfData::TexCoord)*adata.data.NumTex;
	}
	{
		auto& r = ph->CreateBuffer(
			Utilities::GUID(guid) + Utilities::GUID("Faces"),
			Graphics::Pipeline::Buffer::StructuredBuffer(adata.pointers.faces, sizeof(ArfData::Face), adata.data.NumFace));
		if (r.hash != "Success"_hash)
		{
			ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("Positions"));
			ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("Normals"));
			ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("TexCoords"));
			return r;
		}
		*loadedSizeVRAM += sizeof(ArfData::Face)*adata.data.NumFace;
	}

	RETURN_SUCCESS;
}

DLL_EXPORT Utilities::Error Unload(uint32_t guid, void * dataRAM, uint64_t sizeRAM, uint64_t sizeVRAM)
{
	auto ph = Graphics::Get()->GetPipelineHandler();
	ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("Positions"));
	ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("Normals"));
	ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("TexCoords"));
	ph->DestroyBuffer(Utilities::GUID(guid) + Utilities::GUID("Faces"));
	auto ldata = (MeshInfo*)dataRAM;
	delete[] ldata->subMeshes;
	delete ldata;
	RETURN_SUCCESS;
}
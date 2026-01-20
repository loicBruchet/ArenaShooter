#include "Obj.h"
#include "File.h"
#include "Maths/Triangulation/Triangulation.h"



#include "Utils.h"

namespace gce {
	
	Obj::Obj(String const& path) : pFile(nullptr), m_tempNormVect(), m_tempPosVect()
	{
		File* file = new File(path,File::Type::READ_ONLY);
		pFile = file;
		m_path = path;
	}

	Obj::Obj(String const&& path) : pFile(nullptr), m_tempNormVect(), m_tempPosVect()
	{
		File* file = new File(path, File::Type::READ_ONLY);
		pFile = file;
		m_path = path;
	}

	Obj::~Obj()
	{
		delete pFile;
	}

	void Obj::LoadObj()
	{
		std::vector<uint32> indices;	
		uint32 last = 0;
		while (pFile->IsEnd() == false)
		{
			std::vector<String> data = gce::SplitString(pFile->ReadLine(), ' ');
			
			data = gce::EmptyRemover(data);
			if (data.size() == 0) { continue; }

			if (data[0] == "v")
			{
				ASSERT(data.size() == 4, "Invalid position", ERROR_);

				float32 pos1 = gce::ConvertStringToFloat(data[1]);
				float32 pos2 = gce::ConvertStringToFloat(data[2]);
				float32 pos3 = gce::ConvertStringToFloat(data[3]);
				m_tempPosVect.push_back({ pos1, pos2, pos3 });
			}
			if (data[0] == "vn")
			{
				ASSERT(data.size() == 4, "Invalid normal", ERROR_);

				float32 pos1 = gce::ConvertStringToFloat(data[1]);
				float32 pos2 = gce::ConvertStringToFloat(data[2]);
				float32 pos3 = gce::ConvertStringToFloat(data[3]);
				m_tempNormVect.push_back({ pos1, pos2, pos3 });
			}
			if (data[0] == "vt")
			{
				ASSERT(data.size() == 3, "Invalid textureCoordinate", ERROR_);

				float32 pos1 = gce::ConvertStringToFloat(data[1]);
				float32 pos2 = gce::ConvertStringToFloat(data[2]);
				m_tempCoordVect.push_back({ pos1, 1.0f - pos2 });
			}
			if (data[0] == "f")
			{
				ASSERT(data.size() >= 4, "Invalid face", ERROR_);
				for (int64 i = 1; i < data.size(); i++)
				{
					std::vector<String> face = gce::SplitString((data[i]), '/');

					obj::Vertex tempVertex;

					if (face[0] != "")
						tempVertex.position = m_tempPosVect[gce::ConvertStringToInt(face[0]) - 1];
					if (face[1] != "")
						tempVertex.textureCoordinate = m_tempCoordVect[gce::ConvertStringToInt(face[1]) - 1];
					if (face[2] != "")
						tempVertex.normal = m_tempNormVect[gce::ConvertStringToInt(face[2]) - 1];

					vertices.push_back(tempVertex);
				}

				if (data.size() == 4)
				{
					indices.push_back(last + 0);
					indices.push_back(last + 1);
					indices.push_back(last + 2);
					last += 3;
				}
				else if (data.size() == 5)
				{
					indices.push_back(last + 0);
					indices.push_back(last + 1);
					indices.push_back(last + 3);

					indices.push_back(last + 1);
					indices.push_back(last + 2);
					indices.push_back(last + 3);
					last += 4;
				}
				else
				{
					std::vector<gce::Vector3f32> posVertices;

					for (uint8 i = 0; i < data.size() - 1; ++i)
					{
						posVertices.push_back(vertices[last + i].position);
					}

					std::vector<uint32> tempIndices = gce::Triangulate(gce::ProjectToPlane(posVertices));

					for (int8 i = tempIndices.size() - 1; i >= 0; --i)
					{
						indices.push_back(last + tempIndices[i]);
					}
					last += data.size() - 1;
				}
			}
		}
		meshs.push_back(Mesh());
		(--meshs.end())->indices = indices;
	}
}
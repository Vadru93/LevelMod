#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Defines.h"
#include "Node.h"


char* QScript::QBTypes[] = {
	 "None",//End of array / struct and used when parsing / calling scripts
	 "Int",
	 "Float",
	 "String",
	 "Local String",//Used in struct / array	
	 "Pair",
	 "Vector",
	 "Scripted Function",
	 "Compiled Function",
	 "Global Variable/Function",//Member Function in Thps4
	 "Struct",
	 "Local Struct",//Used in struct / array, Structure Pointer in Thps4
	 "Array",
	 "Local Variable/Function",//Used in struct / array, Name in Thps4
	 "Unknown",
	 "Unknown",
	 "Unknown",
	 "Unknown",
	 "Unknown"
};
QScript::QBScript* QScript::Scripts;
std::vector<QScript::CompressedNode> QScript::compNodes;
std::vector<DWORD> QScript::qbKeys;

using namespace std;
using namespace QScript;


void CStruct::AddCompressedNode(DWORD checksum, QBKeyInfoContainer* container)
{

	for (unsigned int i = 0; i < compNodes.size(); i++)
	{
		if (compNodes[i].checksum == checksum)
		{
			if (!compNodes[i].added)
			{

				CStructHeader* header = compNodes[i].first;
				/*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;

				compNodes[i].first = pMap->NextHeader;*/

				CStructHeader* added = this->AddCStruct(header->QBkey, header->Type, header->Data);
				compNodes[i].first = added;
				header = header->NextHeader;

				while (header)
				{

					added = this->AddCStruct(header->QBkey, header->Type, header->Data);
					header = header->NextHeader;
				}
				/*pMap->NextHeader = 0;*/
				compNodes[i].added = true;
				compNodes[i].last = added;
			}
			else
			{
				if (this->tail)
					this->tail->NextHeader = compNodes[i].first;

				if (!this->head)
					this->head = compNodes[i].first;
				this->tail = compNodes[i].last;

				if (container)
				{
					CStructHeader* header = this->head;
					while (header != this->tail && header)
					{

						_printf("CONTAINER:.:..\n");
						typedef DWORD(__cdecl* const pShiftQBKey)(DWORD qbKey);
						DWORD shifted = pShiftQBKey(0x00426DB0)(header->Data);


						typedef QBKeyInfo* (__thiscall* const pAddInfo)(QBKeyInfoContainer* pThis, DWORD qbKey);

						QBKeyInfo* info = pAddInfo(0x00428A30)(container, shifted);

						typedef void(__thiscall* const pAddContainer)(CStruct* pThis, QBKeyInfo* info);
						pAddContainer(0x004292B0)(this, info);

						header = header->NextHeader;
					}
				}
			}
			_printf("Added Comp Node: %X %X COMP %X\n", checksum, this, &compNodes[i]);


			break;
		}
	}

}

BYTE* CStruct::MaybeAddCompressed(DWORD qbKey, BYTE* pFile, QBKeyInfoContainer* container)
{
	typedef BYTE* (__thiscall* const pAddCStruct)(CStruct* pThis, DWORD qbKey, BYTE* pFile, QBKeyInfoContainer* other);
	if (qbKey != 0x68A9B4E1)
		return pAddCStruct(0x00428DF0)(this, qbKey, pFile, container);

	pFile++;
	DWORD checksum = *(DWORD*)pFile;
	pFile += 4;
	AddCompressedNode(checksum, container);
	return pFile;
}



EXTERN void CStruct::FreeHeader()
{
	CStructHeader* p = (CStructHeader*)this;

	while (p != NULL)
	{
		if (p->pData != NULL)
		{
			if (p->Type == 10 || p->Type == 11)
				((CStruct*)p->pData)->FreeHeader();
			else if (p->Type == 12)
				((CArray*)p->pData)->Free();
			free(p->pData);
		}
		p = p->NextHeader;
	}
}

EXTERN void CStruct::RemoveParam(DWORD name)
{
	CStructHeader* header = head;
	while (header != NULL)
	{
		if (header->QBkey == name)
		{
			DellocateCStruct(header);
			break;
		}
		header = header->NextHeader;
	}
}

EXTERN void CStruct::Free()
{
	if (head == NULL)
		return;

	CStructHeader* p = head;

	while (p != tail)
	{
		if (p->pData != NULL)
		{
			if (p->Type == 10 || p->Type == 11)
				((CStruct*)p->pData)->FreeHeader();
			else if (p->Type == 12)
				((CArray*)p->pData)->Free();
			free(p->pData);
		}
		p = p->NextHeader;
	}
}

EXTERN CStructHeader* CStruct::AddParam(const char* name, QBKeyHeader::QBKeyType type)
{
	if (head)
	{
		if (tail)
		{
			CStructHeader* param = AllocateCStruct();
			tail->NextHeader = param;
			tail = param;
			tail->Type = type;
			tail->SetName(name);
			tail->NextHeader = NULL;
		}
		else
		{
			CStructHeader* param = head->NextHeader;
			CStructHeader* prevparam = NULL;
			while (param)
			{
				prevparam = param;
				param = param->NextHeader;
			}
			prevparam->NextHeader = AllocateCStruct();
			param = prevparam->NextHeader;
			tail->NextHeader = param;
			tail = param;
			tail->Type = type;
			tail->SetName(name);
			tail->NextHeader = NULL;
		}
	}
	else
	{
		head = AllocateCStruct();
		tail = head;
		tail->Type = type;
		tail->SetName(name);
		tail->NextHeader = NULL;
	}
	return tail;
}

EXTERN DWORD CStructHeader::GetSize()
{
	DWORD size = 0;

	CStructHeader* header = this;
	while (header)
	{
		switch (header->Type)
		{
		case QBKeyHeader::INT:
			size += sizeof(CStructHeader);
			break;
		case QBKeyHeader::FLOAT:
			size += sizeof(CStructHeader);
			break;
		case QBKeyHeader::ARRAY:
			size += header->pArray->GetSize() + sizeof(CStructHeader);
			break;

		case QBKeyHeader::PAIR:
			size += 8 + sizeof(QBKeyHeader);
			break;

		case QBKeyHeader::LOCAL_STRUCT:
		case QBKeyHeader::STRUCT:
			size += (*(CStructHeader**)header->pStruct)->GetSize() + sizeof(QBKeyHeader);
			break;

		case QBKeyHeader::LOCAL:
			size += sizeof(CStructHeader);
			break;

		case QBKeyHeader::STRING:
		case QBKeyHeader::LOCAL_STRING:
			size += sizeof(CStructHeader) + strlen(header->pStr) + 1;
			break;

		case QBKeyHeader::COMPILED_FUNCTION:
			size += sizeof(CStructHeader);
			break;


		case QBKeyHeader::SCRIPTED_FUNCTION:
			size += sizeof(CStructHeader);
			break;

		case QBKeyHeader::VECTOR:
			size += sizeof(CStructHeader) + 12;
			break;

		default:
			size += sizeof(CStructHeader);
		}
		header = header->NextHeader;
	}
	return size;
}




CStructHeader* CScript::GetParam(DWORD name)
{
	CStructHeader* header = params->head;

	while (header)
	{
		if (header->QBkey == name)
			return header;

		if (header->Type == QBKeyHeader::LOCAL_STRUCT || header->Type == QBKeyHeader::STRUCT)
		{
			CStructHeader* pStruct = *(CStructHeader**)header->pStruct;

			while (pStruct)
			{
				if (pStruct->QBkey == name)
					return pStruct;
				else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
				{
					//Only 2 levels for now
					CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
					while (pStruct2)
					{
						if (pStruct2->QBkey == name)
							return pStruct2;
						pStruct2 = pStruct2->NextHeader;
					}
				}
				pStruct = pStruct->NextHeader;
			}
		}
		else if (header->Type == QBKeyHeader::ARRAY)
		{
			_printf("Parsing array...\n");
			CArray* pArray = header->pArray;

			for (int i = 0; i < pArray->GetNumItems(); i++)
			{
				if (pArray->Type == 0xA || pArray->Type == 0xB)
				{
					//_printf("Index %d of %d\n", i, pArray->GetNumItems());
					CStructHeader* pStruct = pArray->GetCStruct(i);

					while (pStruct)
					{
						_printf("pStruct %p NextHeader %p\n", pStruct, pStruct->NextHeader);
						_printf("Name %s Data %X\n", FindChecksumName(pStruct->QBkey), pStruct->Data);
						if (pStruct->QBkey == name)
							return pStruct;
						else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
						{
							CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
							while (pStruct2)
							{
								if (pStruct2->QBkey == name)
									return pStruct2;
								pStruct2 = pStruct2->NextHeader;
							}
						}
						pStruct = pStruct->NextHeader;
					}
				}
			}
		}
		header = header->NextHeader;
	}
	return NULL;
}

bool CScript::GotParam(DWORD name)
{
	CStructHeader* header = params->head;

	while (header)
	{
		if (header->QBkey == name)
			return true;

		if (header->Type == QBKeyHeader::LOCAL_STRUCT || header->Type == QBKeyHeader::STRUCT)
		{
			CStructHeader* pStruct = *(CStructHeader**)header->pStruct;

			while (pStruct)
			{
				if (pStruct->QBkey == name)
					return true;
				else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
				{
					//Only 2 levels for now
					CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
					while (pStruct2)
					{
						if (pStruct2->QBkey == name)
							return true;
						pStruct2 = pStruct2->NextHeader;
					}
				}
				pStruct = pStruct->NextHeader;
			}
		}
		else if (header->Type == QBKeyHeader::ARRAY)
		{
			_printf("Parsing array...\n");
			CArray* pArray = header->pArray;

			for (int i = 0; i < pArray->GetNumItems(); i++)
			{
				if (pArray->Type == 0xA || pArray->Type == 0xB)
				{
					//_printf("Index %d of %d\n", i, pArray->GetNumItems());
					CStructHeader* pStruct = pArray->GetCStruct(i);

					while (pStruct)
					{
						_printf("pStruct %p NextHeader %p\n", pStruct, pStruct->NextHeader);
						_printf("Name %s Data %X\n", FindChecksumName(pStruct->QBkey), pStruct->Data);
						if (pStruct->QBkey == name)
							return true;
						else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
						{
							CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
							while (pStruct2)
							{
								if (pStruct2->QBkey == name)
									return true;
								pStruct2 = pStruct2->NextHeader;
							}
						}
						pStruct = pStruct->NextHeader;
					}
				}
			}
		}
		header = header->NextHeader;
	}
	return false;
}


EXTERN DWORD CScript::GetNodeName()
{
	return node->name;
}

void QBScript::CreateQBTable(BYTE* table)
{
	while (*table == ScriptToken::Table)
	{
		table++;
		int key = *(int*)table;
		table += 4;
		char* name = (char*)table;
		//_printf("QbNAme %s\n", name);
		DWORD len = strlen(name) + 1;
		name = new char[len];
		if (!name)
			MessageBox(0, "nooo", "", 0);
		memcpy(name, (char*)table, len);

		//_printf("QbAllocated %s\n", name);
		map<int, char*>::iterator it = qbTable.find(key);
		if (it == qbTable.end())
		{
			/*FILE* debugFile = fopen("debug.txt", "r+t");
			fseek(debugFile, 0, SEEK_END);
			fprintf(debugFile, "AddingKey %s %X, in file %s\r\n", name, key, fileName);
			printf("AddingKey %s %X, in file %s\r\n", name, key, fileName);
			fclose(debugFile);*/
			//_printf("AddChecksum %s 0x%X\n", name, key);
			qbTable.insert(pair<int, char*>(key, name));
			qbKeys.push_back(key);
		}
		/*else if (_stricmp(it->second, name))
		{
			FILE* debugFile = fopen("debug.txt", "r+t");
			fseek(debugFile, 0, SEEK_END);
			fprintf(debugFile, "found collision %s %s\r\n", it->second, name);
			fclose(debugFile);

		}*/
		//_printf("inserted\n");
		table += len;
		//_printf("added\n");
	}
}

void QBScript::AddScripts()
{
	char* qdir = (char*)0x5BBAF8;
	char dir[256] = "data\\";
	DWORD len = strlen(qdir) + 1;
	memcpy(&dir[5], qdir, len);

	FILE* f = fopen(dir, "rb+");




	fseek(f, 0, SEEK_END);
	DWORD size = ftell(f);

	fseek(f, 0, SEEK_SET);
	BYTE* pFile = new BYTE[size + 1];
	fread(pFile, size, 1, f);
	fclose(f);
	BYTE* oldData = pFile;

	//MessageBox(0, dir, "going to parse", 0);
	while (pFile < (oldData + size - 5))
	{
		unsigned int i = 13;
		while (pFile < (oldData + size) && *pFile != 0x0A && *pFile != 0x0D)
		{
			dir[i] = *pFile;
			dir[i + 1] = 0;
			i++;
			pFile++;
		}

		dir[i] = 0;


		OpenScript(dir);
		pFile++;
		while (pFile < (oldData + size) && (*pFile == 0x0D || *pFile == 0x0A))
			pFile++;
	}
	delete[] oldData;
}

char* QBScript::GetQBKeyName(int checksum)
{
	if (qbTable.size())
	{
		std::map<int, char*>::iterator it;

		it = qbTable.find(checksum);

		if (it != qbTable.end())
		{
			return it->second;
		}
		/*else
			printf("couldn't find QBKey %X\n", checksum);*/
	}
	/*else
		printf("couldn't find QBKey %X\n", checksum);*/
	return NULL;
}

void QBScript::OpenScript(char* path)
{
	fileName = path;
	_printf("OpenScript: %s\n", path);//MessageBox(0, "OpenScript", path, 0);
	FILE* f = fopen(path, "rb+");

	fseek(f, 0, SEEK_END);
	DWORD size = ftell(f);

	fseek(f, 0, SEEK_SET);
	BYTE* pFile = new BYTE[size + 1];
	fread(pFile, size, 1, f);
	fclose(f);

	//data = pFile;
	BYTE* oldData = pFile;

	while (true)
	{
		switch (*pFile)
		{
		case ScriptToken::NewLineNumber:
		case ScriptToken::QBKey:
		case ScriptToken::Int:
		case ScriptToken::Float:
		case 0x2E:
			pFile += 5;
			break;

		case ScriptToken::String:
		case ScriptToken::LocalString:
			pFile++;
			pFile += *(int*)pFile + 4;
			break;

		case ScriptToken::Vector:
			pFile += 13;
			break;

		case ScriptToken::Pair:
			pFile += 9;
			break;

		case ScriptToken::EndOfFile:
		case ScriptToken::Table:
			_printf("parsing qbTable %X\n", pFile);
			goto done;

		case 0x47:
		case 0x48:
		case 0x49:
			pFile += 3;
			break;

		default:
			//_printf("default: %X\n", *pFile);
			/*char def[2];
			sprintf(def, "%X", *pFile);
			MessageBox(0,"default", def, 0);*/
			pFile++;
			break;

		case 0x2F:
		case 0x37:
		case 0x40:
		case 0x41:
			pFile++;
			int count = *(int*)pFile;
			pFile += 4;
			/*if (*(int*)pFile == 65537 || (count == 1 && *(WORD*)pFile == 1))
			{
				pFile += 2 * count;
			}*/
			pFile += count * 4;
			break;
		}
	}
done:
	CreateQBTable(pFile);
	_printf("END OpenScript\n");
	delete[] oldData;

}

void QBScript::ClearMap()
{
	map<int, char*>::iterator end = qbTable.end();
	for (map<int, char*>::iterator it = qbTable.begin(); it != end; it++)
	{
		delete[] it->second;
	}
	qbTable.clear();
}
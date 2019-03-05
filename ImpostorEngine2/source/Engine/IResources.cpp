//Fuck that shit
/*#if INTERFACE
#include <Engine/IResource.h>

//class IResources {
//public:
	static const char* FormatPath(const char* path, int mode);
	static IResource* Load(const char* bpath);
	static IResource* Load(const char* bpath, int mode);
	static IResource* Load(const char* bpath, bool inMemory);
	static IResource* Open(const char* bpath);
	static IResource* Load(const char* bpath, int mode);
	static IResource* Load(const char* bpath, bool inMemory, int mode);
	static IResource* Open(const char* bpath, int mode);
	static bool Close(IResource* res);
	static uint64_t Decompress(void* dst, int dstLen, const void* src, int srcLen);
};
#endif*/

#include <Engine/IApp.h>
#include <Engine/IResources.h>
#include <fstream>
#include <sstream>

char buffmantime[0x100];

PUBLIC STATIC IResource* IResources::Load(const char* bpath) {
	return IResources::Load(bpath, 0);
}
PUBLIC STATIC IResource* IResources::Load(const char* bpath, bool inMemory) {
	return Load(bpath, inMemory,0);
}

PUBLIC STATIC void IResources::FormatPath(const char* path) {
	FormatPath(path, 0);
}

PUBLIC STATIC IResource* IResources::Open(const char* bpath) {
	return Load(bpath,0);
}
PUBLIC STATIC IResource* IResources::Load(const char* bpath, int mode) {
	return IResources::Load(bpath, false);
}
PUBLIC STATIC IResource* IResources::Load(const char* bpath, bool inMemory, int mode) {
	FormatPath(bpath, mode);
	const char* path = buffmantime;
    char FullName[256];
    sprintf(FullName, "%s%s", IFILE(""), path);
	//printf(FullName);
	IApp::Print(0, FullName);
    IResource* res = new IResource();
    res->ZipLoading = false;
    res->RW = NULL;

    if (res->ZipLoading) { // zip loading
        #if 0
		const char* zipfilename = IFILE("../Resources.zip");

        #ifdef USEWIN32IOAPI
			fill_win32_filefunc64A(&ffunc);
			res->uf = unzOpen2_64(zipfilename,&ffunc);
		#else
			res->uf = unzOpen64(zipfilename);
		#endif

		if (res->uf == NULL) {
            zipfilename = "Resources.zip";
            #ifdef USEWIN32IOAPI
    			fill_win32_filefunc64A(&ffunc);
    			res->uf = unzOpen2_64(zipfilename, &ffunc);
    		#else
    			res->uf = unzOpen64(zipfilename);
    		#endif

            if (res->uf == NULL) {
                IApp::Print(2, "Resources.zip not found!");
                return NULL;
            }
		}

        if (unzLocateFile(res->uf, path, 1) != UNZ_OK) {
            IApp::Print(2, "'%s' was not found in Resources.zip!", path);
            return NULL;
        }

        if (unzOpenCurrentFile(res->uf) != UNZ_OK) {
            IApp::Print(2, "Could not open '%s'!", path);
            return NULL;
        }


        char filename_inzip[256];
        if (unzGetCurrentFileInfo64(res->uf, &res->info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0) != UNZ_OK) {
            IApp::Print(2, "Could not get information on '%s'!", path);
            return NULL;
        }

        if (inMemory) {
            int err = 0;
            size_t offset = 0;
            uint8_t* buffer = (uint8_t*)malloc(0x4000);
            res->Buffer = (uint8_t*)malloc((size_t)res->info.uncompressed_size);
            do {
                err = unzReadCurrentFile(res->uf, buffer, 0x4000);
                if (err < 0) {
                    IApp::Print(2, "Error while reading '%s'! (Error Code: %d)", path, err);
                    break;
                }
                if (err == 0)
                    break;
                memcpy(res->Buffer + offset, buffer, err);
                offset += err;
            }
            while (err > 0);

            free(buffer);

            res->RW = SDL_RWFromMem(res->Buffer, (size_t)res->info.uncompressed_size);

            res->ZipLoading = false;
        }
        #endif
    }
    else {
        res->RW = SDL_RWFromFile(FullName, "rb");
        if (!res->RW) {
			IApp::Print(2, "%s", SDL_GetError());
            return NULL;
        }
    }
    res->path = path;

    return res;
}

PUBLIC STATIC void IResources::FormatPath(const char* path, int mode) {
	char helpme[0x40];
	char temp[0x80];
	sprintf(helpme, path);
	for (int i = 0; i < 0x100; i++)
	{
		buffmantime[i] = 0;
	}
	switch (mode) {
	case 2:
	case 1: {
		StrCopy(temp, "Source/Resources/");
		StrAdd(temp, "Mixed/");
		StrAdd(temp, helpme);
		StrAdd(buffmantime, "Mixed/");
		StrAdd(buffmantime, helpme);
		FILE* cfile = fopen(temp, "rb");
		if (cfile) {
			fclose(cfile);
			break;
		}
	}
	case 0: {
		for (int i = 0; i < 0x100; i++) {
			buffmantime[i] = 0;
		}
		StrCopy(temp, "Source/Resources/");
		StrAdd(temp, "Classic/");
		StrAdd(temp, helpme);
		StrAdd(buffmantime, "Classic/");
		StrAdd(buffmantime, helpme);
		FILE* cfile = fopen(temp, "rb");
		if (cfile) {
			fclose(cfile);
			break;
		}
	}
	default:
		return;
	}
}

//Using RSDK code to fucking make this work AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa - Ducky
PUBLIC STATIC int IResources::StrCopy(char* dest, char* src)
{
	int result;
	int v3; // ecx
	char v4; // dl

	v3 = 0;
	v4 = *src;
	if (*src)
	{
		result = 0;
		do
		{
			*(byte*)(dest + v3) = v4;
			v4 = src[++result];
			v3 = result;
		} while (v4);
	}
	*(byte*)(dest + v3) = 0;
	return result;
}

PUBLIC STATIC int IResources::StrAdd(char* dest, char* src)
{
	char* v2; // edx
	int result; // eax
	char* v4; // edi
	char v5; // dl
	char* v6; // esi

	if (dest == "" || src == "")
	{
		return 0;
	}

	if (*dest)
	{
		v2 = dest + 1;
		result = 0;
		do
		{
			v4 = v2++;
			++result;
		} while (*(v2 - 1));
	}
	else
	{
		v4 = dest;
		result = 0;
	}
	v5 = *src;
	if (*src)
	{
		v6 = &src[-result];
		do
		{
			dest[result++] = v5;
			v5 = v6[result];
		} while (v5);
		v4 = &dest[result];
	}
	*v4 = 0;
	return result;
}

PUBLIC STATIC IResource* IResources::Open(const char* bpath, int mode) {
	FormatPath(bpath, mode);
	const char* path = buffmantime;
	char FullName[256];
	sprintf(FullName, "%s%s", IFILE(""), path);

	IResource* res = new IResource();
	res->ZipLoading = false;
	res->RW = SDL_RWFromFile(FullName, "wb");
	if (!res->RW) {
		return NULL;
	}
	res->path = path;

	return res;
}

PUBLIC STATIC bool IResources::Close(IResource* res) {
    if (res == NULL) {
        IApp::Print(2, "Resource error.");
        return false;
    }
    if (!res->ZipLoading) {
        if (res->RW == NULL) {
            IApp::Print(2, "Resource \"%s\" does not exist.\n", res->path);
            return false;
        }
        SDL_RWclose(res->RW);
        res->RW = NULL;
    }
    if (res->Buffer) {
        free(res->Buffer);
        res->Buffer = NULL;
    }
    delete res;
    return true;
}

PUBLIC STATIC uint64_t IResources::Decompress(void* dst, int dstLen, const void* src, int srcLen) {
    z_stream strm  = {0};
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef *) src;
    strm.next_out  = (Bytef *) dst;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    int err = -1;
    unsigned long ret = -1;

    err = inflateInit2(&strm, (15 + 32)); //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
    if (err == Z_OK) {
        err = inflate(&strm, Z_FINISH);
        if (err == Z_STREAM_END) {
            ret = strm.total_out;
        }
        else {
             inflateEnd(&strm);
             return err;
        }
    }
    else {
        inflateEnd(&strm);
        return err;
    }

    inflateEnd(&strm);
    return ret;
}

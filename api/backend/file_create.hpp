#ifndef WINDOWS_FILE_API_FILE_CREATE_AND_DELTE_H
#define WINDOWS_FILE_API_FILE_CREATE_AND_DELETE_H
#include "../common.hpp"
#include "empty_file.hpp"

using namespace WindowsFileAPI_EmptyFile;

namespace WindowsFileAPI_FileCreateAndDelete
{
    class FileCreateAndDelete : public EmptyFile
    {
    protected:
        TCHAR *file_to_create;

    public:
        /* Needed just in case developers explicitly includes this header file. */
        explicit FileCreateAndDelete(TCHAR *file)
            : file_to_create(file), EmptyFile(file)
        {
            check_valid_version();
        }

        explicit FileCreateAndDelete()
            : file_to_create(nullptr), EmptyFile()
        {
            check_valid_version();
        }

        template<typename T = const char *>
        #ifndef WRONG_VERSION /* avoid compilation errors in a case where the developer is not using C++20. */
            requires std::is_same<T, const char *>::value
                || std::is_same<T, TCHAR *>::value
        #endif
        void create_file_BN(T filename)
        {
            /* First check to see if the file already exists. */
            HANDLE hHandle = CreateFile(
                filename,
                GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

            if(hHandle == INVALID_HANDLE_VALUE)
            {
                /* If the file does not exist, create it. */
                CloseHandle(hHandle);

                hHandle = CreateFile(
                    filename,
                    GENERIC_WRITE,
                    FILE_SHARE_WRITE,
                    NULL,
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );

                assert(hHandle != INVALID_HANDLE_VALUE,
                    "Error creating the file %s.\n",
                    filename)
                
                /* Close the HANDLE, file has been created. */
                CloseHandle(hHandle);
                return;
            }

            /* File already exists. */
            CloseHandle(hHandle);
        }

        void create_file_and_write_BN(TCHAR *filename, const char *data, bool clear_file_if_exists = true)
        {
            create_file_BN(filename);

            if(clear_file_if_exists)
                if(is_file_empty_BN(filename) == FALSE)
                    empty_file_BN(filename);
            
            HANDLE hHandle = CreateFile(
                filename,
                GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

            assert(hHandle != INVALID_HANDLE_VALUE,
                "Error opening file %s in GENERIC_WRITE mode.\n",
                file_to_create)

            DWORD bytes_written = 0;
            BOOL error = WriteFile(
                hHandle,
                data,
                strlen(data),
                &bytes_written,
                NULL
            );

            assert(bytes_written == strlen(data),
                "Error writing all the bytes from data to %s.\n",
                file_to_create)
            
            CloseHandle(hHandle);
        }

        void create_file_WD()
        {
            if(file_to_create == nullptr) return;

            create_file_BN(file_to_create);
        }

        void create_file_and_write_WD(const char *data, bool clear_file_if_exists = true)
        {
            if(file_to_create == nullptr) return;

            create_file_and_write_BN(file_to_create, data, clear_file_if_exists);
        }

        ~FileCreateAndDelete() = default;
    };
}

#endif
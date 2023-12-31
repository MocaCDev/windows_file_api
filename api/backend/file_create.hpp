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

        void set_file_to_create(TCHAR *file)
        {
            if(file_to_create) delete file_to_create;

            /* Allocate heap memory and copy over the value. */
            file_to_create = new TCHAR[strlen((const char *)file) + 1];
            assert(file_to_create,
                "Error allocating heap memory for `file_to_create`.\n\tTry running program again.\n")
            
            memcpy(file_to_create, file, strlen((const char *)file) + 1);
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
                    CREATE_NEW,
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
            
            /* The desired access will always be FILE_APPEND_DATA.
             *
             * Regardless if the file gets emptied or not; if it gets emptied,
             * the data will get appended from the position of 0, else it will
             * append from wherever the file last ended off at.
             * 
             * */
            HANDLE hHandle = CreateFile(
                filename,
                FILE_APPEND_DATA,
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

        ~FileCreateAndDelete()
        {
            if(file_to_create) delete file_to_create;
        }
    };
}

#endif
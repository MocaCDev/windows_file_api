#ifndef WINDOWS_FILE_API_FILE_CREATE_AND_DELTE_H
#define WINDOWS_FILE_API_FILE_CREATE_AND_DELETE_H
#include "../common.hpp"

namespace WindowsFileAPI_FileCreateAndDelete
{
    class FileCreateAndDelete
    {
    protected:
        TCHAR *file_to_create;

    public:
        /* Needed just in case developers explicitly includes this header file. */
        explicit FileCreateAndDelete(TCHAR *filename)
            : file_to_create(filename)
        {
            check_valid_version();
        }

        explicit FileCreateAndDelete()
            : file_to_create(nullptr)
        {
            check_valid_version();
        }

        template<typename T = const char *>
        #ifndef WRONG_VERSION /* avoid compilation errors in a case where the developer is not using C++20. */
            requires std::is_same<T, const char *>::value
                || std::is_same<T, TCHAR *>::value
        #endif
        void create_file(T filename)
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

        ~FileCreateAndDelete() = default;
    };
}

#endif
#ifndef WINDOWS_FILE_API_FILE_CREATE_AND_DELTE_H
#define WINDOWS_FILE_API_FILE_CREATE_AND_DELETE_H
#include "../common.hpp"

namespace WindowsFileAPI_FileCreateAndDelete
{
    class FileCreateAndDelete
    {
    protected:
        std::vector<TCHAR *> files_to_create;

    public:
        /* Needed just in case developers explicitly includes this header file. */
        explicit FileCreateAndDelete(std::vector<TCHAR *>files)
            : files_to_create(files)
        {
            check_valid_version();
        }

        explicit FileCreateAndDelete()
        {
            check_valid_version();
        }

        void create_all()
        {
            for(auto i = files_to_create.cbegin(); i != files_to_create.cend(); i++)
                create_explicit<TCHAR *>(*i);
        }

        template<typename T = const char *>
        #ifndef WRONG_VERSION /* avoid compilation errors in a case where the developer is not using C++20. */
            requires std::is_same<T, const char *>::value
                || std::is_same<T, TCHAR *>::value
        #endif
        void create_explicit(T filename)
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
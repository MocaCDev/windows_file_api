#ifndef WINDOWS_FILE_API_EMPTY_FILE_H
#define WINDOWS_FILE_API_EMPTY_FILE_H
#include "../common.hpp"

namespace WindowsFileAPI_EmptyFile
{
    class EmptyFile
    {
    protected:
        TCHAR *file_to_empty;

    public:
        explicit EmptyFile(TCHAR *filename)
            : file_to_empty(nullptr)
        {
            /* This is kind of repetitive, but it needs to be otherwise there will be complications
             * upon developers including these header files explicitly.
             * */
#ifdef WRONG_VERSION
           error("Wrong Version Of C++:\n\tRequired Version Is C++20: -std=c++20.\n")
#endif 
        }

        explicit EmptyFile()
        {
#ifdef WRONG_VERSION
           error("Wrong Version Of C++:\n\tRequired Version Is C++20: -std=c++20.\n")
#endif 
        }

        /* empty_file_BN - Empty a file by name (BN).
         *
         * Arguments:
         *  filename - the file the developer wants to empty.
         * 
         * Returns:
         *  Nothing.
         * 
         * Errors:
         *  empty_file_BN will error if:
         *      1. The file does not get deleted as it should.
         *      2. The recreation of the file does not proceed as it should.
         * 
         * */
        template<typename T = const char *>
        #ifndef WRONG_VERSION /* avoid compilation errors in a case where the developer is not using C++20. */
            requires std::is_same<T, const char *>::value
                || std::is_same<T, TCHAR *>::value
        #endif
        void empty_file_BN(T filename)
        {
            /* First, make sure the file exists. */
            HANDLE hHandle = CreateFile(
                filename,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

            /* If the HANDLE is invalid, just return. The file does not exist. */
            if(hHandle == INVALID_HANDLE_VALUE)
            {
                CloseHandle(hHandle);
                return;
            }

            /* Close the HANDLE, it is not needed anymore.
             * Proceed to "empty" the file.
             * */
            CloseHandle(hHandle);

            BOOL is_deleted = DeleteFile(filename);

            assert(is_deleted != FALSE,
                "Error (1) attempting to empty the file %s.\n",
                filename)
            
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
                "Error (2) attempting to empty the file %s.\n",
                filename)
            
            /* Close the HANDLE, the file has been emptied. */
            CloseHandle(hHandle);
        }

        /* empty_file_WD - Empty file by file passed to constructor. (WD = With Default)
         * 
         * Disclaimer:
         *  The only way this function can successfully be invoked is if the developer includes this
         *  header file explicitly and initiates the said object with a filename.
         *  Else, this file will not be used and can be safely assumed to be depricated.
         * 
         * Disclaimer 2:
         *  If said developer explicitly includes this header file, they must know the API is supported on
         *  Windows only. Else, they will run into errors. (shouldn't be a problem)
         * 
         * Arguments:
         *  Nothing.
         * 
         * Returns:
         *  Nothing.
         * 
         * Errors:
         *  empty_file_NN will error if:
         *      1. The file passed to the constructor of said object does not exist.
         *      2. The recreation of the file does not proceed as it should.
         * 
         * */
        void empty_file_WD()
        {
            empty_file_BN<TCHAR *>(file_to_empty);
        }

        /* is_file_empty_BN - Check if a file is empty by name (BN).
         *
         * Arguments:
         *  filename - the file the developer want to check.
         * 
         * Errors:
         *  None.
         * 
         * Returns:
         *  BOOL - TRUE if file is empty/does not exist, FALSE if file is not empty.
         * 
         * */
        template<typename T = const char *>
        #ifndef WRONG_VERSION /* avoid compilation errors in a case where the developer is not using C++20. */
            requires std::is_same<T, const char *>::value
                || std::is_same<T, TCHAR *>::value
        #endif
        BOOL is_file_empty_BN(T filename)
        {
            HANDLE hHandle = CreateFile(
                filename,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

            if(hHandle == INVALID_HANDLE_VALUE)
            {
                CloseHandle(hHandle);
                return TRUE;
            }

            DWORD file_size = GetFileSize(hHandle, NULL);
            CloseHandle(hHandle);

            if(file_size > 0) return FALSE;

            return TRUE;
        }

        /* is_file_empty_WD - Check if a file is empty using the filename passed to the constructor. (WD = With Default)
         *
         * Disclaimer:
         *  See above disclaimer for `empty_file_WD`; same applies to below function.
         * 
         * Arguments:
         *  Nothing.
         * 
         * Errors:
         *  None.
         * 
         * Returns:
         *  BOOL - TRUE if file is empty/does not exist, FALSE if file is not empty.
         * 
         * */
        BOOL is_file_empty_WD()
        {
            return is_file_empty_BN(file_to_empty);
        }

        ~EmptyFile() = default;
    };
}

#endif
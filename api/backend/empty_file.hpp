#ifndef WINDOWS_FILE_API_EMPTY_FILE_H
#define WINDOWS_FILE_API_EMPTY_FILE_H
#include "../common.hpp"

namespace WindowsFileAPI_EmptyFile
{
    class EmptyFile
    {
    protected:
        std::vector<TCHAR *> files_to_empty;

    public:
        /* Needed just in case developers explicitly includes this header file. */
        explicit EmptyFile(std::vector<TCHAR *> files)
            : files_to_empty(files)
        {
            /* This is kind of repetitive, but it needs to be otherwise there will be complications
             * upon developers including these header files explicitly.
             * */
            check_valid_version();
        }

        explicit EmptyFile()
        {
            check_valid_version();
        }

        void empty_all()
        {
            for(auto i = files_to_empty.cbegin(); i != files_to_empty.cend(); i++)
                empty_file_BN<TCHAR *>(*i);
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
         *  file - the file to delete within `files_to_empty`
         * 
         * Returns:
         *  Nothing.
         * 
         * Errors:
         *  empty_file_NN will error if:
         *      1. The file passed to the constructor of said object does not exist.
         *      2. The recreation of the file does not proceed as it should.
         *      3. The file passed as an argument was not found in `files_to_empty`
         * 
         * */
        void empty_file_WD(TCHAR *file)
        {
            if(files_to_empty.size() == 0) return;
            for(auto i = files_to_empty.cbegin(); i != files_to_empty.cend(); i++)
            {
                if(strcmp((const char *)*i, (const char *)file) == 0)
                {
                    empty_file_BN<TCHAR *>(*i);
                    return;
                }
            }

            /* Error if the files was not found.
             * Could be useful just in case the developer makes a mistake.
             * */
            error("The file %s was not found in the vector of files to empty.\n")
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
         *  file - the file the developer wants to check
         * 
         * Errors:
         *  None.
         * 
         * Returns:
         *  BOOL - TRUE if file is empty/does not exist, FALSE if file is not empty.
         * 
         * */
        BOOL is_file_empty_WD(TCHAR *file)
        {
            for(auto i = files_to_empty.cbegin(); i != files_to_empty.cend(); i++)
            {
                if(strcmp((const char *)*i, (const char *)file) == 0)
                    return is_file_empty_BN<TCHAR *>(*i);
            }

            /* If we get here, the file does not exist. Just return TRUE. */
            return TRUE;
        }

        ~EmptyFile() = default;
    };
}

#endif
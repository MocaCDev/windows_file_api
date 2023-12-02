#ifndef WINDOWS_FILE_API_BASE_API_H
#define WINDOWS_FILE_API_BASE_API_H

/* The code only exists so long as the OS is Windows.
 * If the OS is not Windows, this API has no inherent value,
 * or usage.
 * 
 * This "wrapper" is also useful such that it restricts the amount of
 * similar checks we'll have to do in backend-related functionality.
 * */
#ifdef __WIN32
#include "common.hpp"
#include "backend/empty_file.hpp"
#include "backend/file_create.hpp"

using namespace WindowsFileAPI_EmptyFile;
using namespace WindowsFileAPI_FileCreateAndDelete;

/* Base file where all API comes together.
 * No need to explicitly include other files,
 * `BaseAPI` will inherit all other code such that developers only have
 * to include `base_api.hpp` to restrict further includes.
 * */

namespace WindowsFileAPI_BaseAPI
{
    /* Template description and implied usage of vector passed to BaseAPI constructor:
     *  If A is true, B is false vector size must be 1, thereafter
     *  vector[0] will be passed to EmptyFile constructor.
     *  
     *  If A is false, B is true vector size must be 1, thereafter
     *  vector[0] will be passed to FileCreateAndDelete constructor.
     *  
     *  If A is true, B is true vector size must be 2, therafter
     *  vector[0] will be passed to EmptyFile constructor and
     *  vector[1] will be passed to FileCreateAndDelete constructor.
     * 
     *  If A is false, B is false vector size must be 0 thereafter
     *  no actions will be performed and it would be best to invoke
     *  the BaseAPI object constructor that accepts no arguments.
     * 
     * */
    template<bool A = false, bool B = false>
    class BaseAPI : public EmptyFile, public FileCreateAndDelete
    {
    private:
        /* Needed just in case `BaseAPI(std::vector<TCHAR *>)` gets called
         * to initiate the `BaseAPI` object.
         * */
        EmptyFile *empty_file_object = nullptr;
        FileCreateAndDelete *create_and_delete_object = nullptr;

    public:
        explicit BaseAPI()
            : EmptyFile(), FileCreateAndDelete()
        {
            check_valid_version();
        }

        explicit BaseAPI(std::vector<TCHAR *> files)
        {
            check_valid_version();

            /* Do nothing if both are false (which are there default values)*/
            if(!A && !B) return;

            assert(!(A && B),
                "Both A and B are true. Invoke one of the following BaseAPI constructors in accordance to A and B being true:\n\tBaseApi(std::vector<TCHAR *>, std::vector<TCHAR *>)\n\tBaseApi(std::vector<TCHAR *>, std::vector<TCHAR *>, bool empty_all_files = false, bool create_all_files = false, bool delete_all_files = false)")

            /* Make sure the size is at least >= 1 */
            assert(files.size() >= 1,
                "Too few files in vector passed to BaseAPI constructor.\n")

            if(A) { empty_file_object = new EmptyFile(files); return; }
            if(B) { create_and_delete_object = new FileCreateAndDelete(files); return; }
        }

        BaseApi(std::vector<TCHAR *> file_set1, std::vector<TCHAR *> file_set2)
        {
            check_valid_version();

            assert(A && B,
                "Cannot call BaseAPI constructor with file set 1 and file set 2 when both parementers to template are not true.\n")

            assert(file_set1.size() >= 1 && file_set2.size() >= 1,
                "Too few files in file set 1 or file set 2 passed to BaseAPI constructor.\n")
            
            empty_file_object = new EmptyFile(file_set1);
            create_and_delete_object = new FileCreateAndDelete(file_set2);
        }

        BaseApi(std::vector<TCHAR *> file_set1, std::vector<TCHAR *> file_set2, bool empty_all_files = false, bool create_all_files = false, bool delete_all_files = false)
        {
            check_valid_version();

            assert(A && B,
                "Cannot call BaseAPI constructor with file set 1 and file set 2 when both parementers to template are not true.\n")

            assert(file_set1.size() >= 1 && file_set2.size() >= 1,
                "Too few files in file set 1 or file set 2 passed to BaseAPI constructor.\n")
            
            empty_file_object = new EmptyFile(file_set1);
            create_and_delete_object = new FileCreateAndDelete(file_set2);
        }

        ~BaseAPI() = default;
    };
}
#else
#error "NO"
#endif

#endif
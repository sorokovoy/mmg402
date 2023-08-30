#ifndef MMG402Directory_H
#define MMG402Directory_H

#include "MMG402IO\libs\dirent.h"

#include <vector>
#include <string>

namespace MMG402IO
{
  namespace libs
  {

    /*    #define DT_UNKNOWN 0
    #define DT_REG S_IFREG
    #define DT_DIR S_IFDIR
    #define DT_FIFO S_IFIFO
    #define DT_SOCK S_IFSOCK
    #define DT_CHR S_IFCHR
    #define DT_BLK S_IFBLK
    #define DT_LNK S_IFLNK
    */

    inline std::vector<std::string> GetFilesFromFolder(const char* ipDirectory)
    {
      std::vector<std::string> result;
      DIR* dir;
      struct dirent* ent;
      if ((dir = opendir (ipDirectory)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
          if (ent->d_type == DT_REG) {
            result.push_back(ent->d_name);
          }
        }
        closedir (dir);
      } else {
        /* could not open directory */
      }

      return result;
    }


  }; // libs

}; // MMG402IO

#endif // MMG402CImg_H



#include "MMG402IO\MMG402IOSingle1DCImage.h"
#include "MMG402IO\MMG402IOGroup2DCImage.h"

#include "MMG402IO\libs\MMG402Directory.h"

class BonesFileNameParcer : public MMG402IO::IMMG402IOGroup2DCImageFileNameParcer
{
public:
  virtual double Parce(const char* ipFileName) const
  {
    int h, min;
    sscanf(ipFileName, "%2d%2d", &h, &min);
    return (double)(h * 60 * 60 + min * 60);
  }
};


void main ()
{

  MMG402Domain::IMMG402Domain<double, 2>* domain = new MMG402Domain::MMG402RectDomain<double, 2>(MMG402Domain::MMG402Rectangle<double, 2>(MMG402Domain::MMG402DomainPoint<double, 2>(-5, -5), MMG402Domain::MMG402DomainPoint<double, 2>(5, 5)));
  MMG402Grid::IMMG402Grid<double, 2>* grid = new MMG402Grid::MMG402GridUniform<double, 2>(domain,  new MMG402Grid::MMG402GridIndexerLinear<2>(MMG402Grid::IMMG402GridIndexer<2>::GridIndex(256, 256)));


  MMG402IO::MMG402IOGroup2DCImage<double, double> d("D:\\Projects\\data\\bonesgrad.bmp", 0, 1);
  d.Read("D:\\Projects\\data\\bones", grid, new BonesFileNameParcer());

  /*
  std::vector<std::string> result = MMG402IO::libs::GetFilesFromFolder("D:\\Projects");

      std::tm t = {};
    std::istringstream ss("23:12:34");
   // ss.imbue(std::locale::empty());
    ss >> std::get_time(&t, "%H:%M:%S");
    if (ss.fail()) {
        std::cout << "Parse failed\n";
    } else {
      //  std::cout << std::put_time(&t, "%c") << '\n';
    }
    time_t t1 = mktime(&t);
  //MMG402IO::MMG402IOSingle1DCImage<double, double> d(0, 0, 0);
  */
}
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

// constant values
const int kHeight = 30;
const int kWidth = 80;
const int kBin_num = kHeight;
const char kChar = '*';

// functions
double GetMean   ( vector < double >& vval );
double GetStdDev ( vector < double >& vval );
double GetIntegral( vector < double >& vval );
void GetContent  ( vector < double >& vval, vector < double >& vcenter, vector < int >& vcontent );

void PrintHeader ();
void PrintHist   ( vector < double >& vcenter, vector < int >& vcontent );
void PrintFooter ( vector < double >& vval );
void ReadFile( string file, vector < double >& vval );

int main( int argc, char* argv[] )
{

  // in the case of no argument
  if( argc == 1 )
    {
      cout << "Usage: " << argv[0] << " [--no-hist] [values]" << endl;
      cerr << "Program is stopped" << endl;
      return -1;
    }

  // get values
  vector < double > vval;

  // check whether the first argument is filename or not
  string first_argument = argv[1];
  bool no_hist = false;

  if( first_argument.find( ".dat" ) != string::npos ||
      first_argument.find( ".txt" ) != string::npos )
    {
      cout << "Data file mode" << endl;
      ReadFile( first_argument, vval );
    }
  else
    {

      int first_index = 1;
      if( first_argument == "--no-hist" )
        {
          no_hist = true;
          first_index++;
        }


      cout << "Direct mode" << endl;
      // loop over arguments
      for( int i=first_index; i<argc; i++ )
        {

          // conversion from char* to double
          stringstream ss( argv[i] );
          double val;
          ss >> val;

          // if argument is not number, skip this
          if( ss.fail() )
            {
              cout << argv[i] << " is skipped" << endl;
              continue;
            }

          // store value
          vval.push_back( val );
        }
    }

  // if nothig is stored, stop the program
  if( vval.size() == 0 )
    {
      cerr << "invarid arguments" << endl;
      cerr << "Program is stopped" << endl;
      return -1;
    }

  // count numbers
  vector < int > vcontent(kBin_num, 0);
  vector < double > vcenter;
  GetContent( vval, vcenter, vcontent );

  // print hist
  if( no_hist == false )
    {
      PrintHeader();
      PrintHist( vcenter, vcontent );
    }

  PrintFooter( vval );

  return 0;
}

//
void GetContent( vector < double >& vval, vector < double >& vcenter, vector < int >& vcontent )
{

  // define range of histogram
  double xmax = *max_element( vval.begin(), vval.end() );
  double xmin = *min_element( vval.begin(), vval.end() );
  double bin_width = (xmax - xmin ) / (double)kBin_num;

  if( bin_width == 0)
    cout << " - No variation in given samples" << endl;

  // calculate center of bins
  double xi = xmin;
  for( int i=0 ; i<kBin_num ; i++, xi += bin_width )
    vcenter.push_back( xi + bin_width/2 );

  // counte numbers and set the bin contents
  for( int i=0; i<vval.size(); i++ )
    {
      for( int j=0; j<kBin_num; j++ )
        {
          if( vval[i] >= vcenter[j] - bin_width/2 && vval[i] < vcenter[j] + bin_width/2 )
            {
              vcontent[j] += 1;
              continue;
            }
        }
    }
}

// return mean of vval
double GetMean( vector < double >& vval )
{
  return GetIntegral( vval ) / vval.size();
}

// return std. dev. of vval
double GetStdDev( vector < double >& vval )
{
  double sum = 0;
  double mean = GetMean( vval );

  for( int i=0; i<vval.size(); i++ )
    sum += pow(mean - vval[i] , 2 );

  return sqrt( sum / vval.size() );
}

double GetIntegral( vector < double >& vval )
{
  double sum = 0;
  for( int i=0; i<vval.size(); i++ )
    sum += vval[i];
  return sum;
}
// print header
void PrintHeader()
{
  cout << "\n";
  cout << "  " << string( 11, '=' ) << endl;
  cout << "   HISTOGRAM" << endl;
  cout << "  " << string( 11, '=' ) << endl;
  cout << "\n";
}

// print histogram and axis
void PrintHist( vector < double >& vcenter, vector < int >& vcontent )
{

  // normalize histogram
  // set the length of the longest bin to kWidth
  int max = *max_element( vcontent.begin(), vcontent.end() );
  int min = *min_element( vcontent.begin(), vcontent.end() );

  if( max - min == 0 )
    return;

  vector < int > vcontent_normalized( kBin_num );
  for( int i=0; i<vcontent.size(); i++ )
    {
      if( vcontent[i] == 0 )
        vcontent_normalized[i] = 0;
      else
        vcontent_normalized[i]
          = vcontent[i] * kWidth / *max_element( vcontent.begin(), vcontent.end() ) ;
    }

  // print histogram
  for( int i=0; i<kBin_num; i++ )
    {
      cout << setw(10) << setprecision( 4 ) << vcenter[i] << " | "
           << string( vcontent_normalized[i], kChar ) << " "
           << vcontent[i]
           << endl;
    }

  // print axis
  cout << string( 10 + 3 + kWidth -1 , '-' ) << ">" << endl;
  cout << setw(10) << "Entries"
       << setw(2) << 0
       << setw( (10 + 3 + kWidth - 12 )/4) << 1 * max / 4
       << setw( (10 + 3 + kWidth - 12 )/4) << 2 * max / 4
       << setw( (10 + 3 + kWidth - 12 )/4) << 3 * max / 4
       << setw( (10 + 3 + kWidth - 12 )/4) << 4 * max / 4
       << endl;
}

// print footer (stats. box)
void PrintFooter( vector < double >& vval )
{

  int width = kWidth / 2;
  cout << "  +" << string( width - 2 , '-' ) << "+" << endl;
  cout << "  | "
       << "Entry     : "
       << setw( width - 4 - 12 ) << setprecision(6) << vval.size()
       << " |"
       << endl;
  cout << "  | "
       << "Mean      : "
       << setw( width - 4 - 12 ) << setprecision(6) << GetMean( vval )
       << " |"
       << endl;
  cout << "  | "
       << "Std. Dev. : "
       << setw( width - 4 - 12) << setprecision(6) << GetStdDev( vval )
       << " |"
       << endl;
  cout << "  | "
       << "Max       : "
       << setw( width - 4 - 12) << setprecision(6) << *max_element( vval.begin(), vval.end() )
       << " |"
       << endl;
  cout << "  | "
       << "Min       : "
       << setw( width - 4 - 12) << setprecision(6) << *min_element( vval.begin(), vval.end() )
       << " |"
       << endl;
  cout << "  | "
       << "Integral  : "
       << setw( width - 4 - 12) << setprecision(6) << GetIntegral( vval )
       << " |"
       << endl;
  cout << "  +" << string( width - 2 , '-' ) << "+" << endl;
}

void ReadFile( string file, vector < double >& vval )
{
  ifstream ifs( file.c_str() );
  if( ifs.fail() )
    {
      cerr << "==============================================" << endl;
      cerr << file << " is not found" << endl;
      cerr << "==============================================" << endl;
      return;
    }

  while( !ifs.eof() )
    {
      string stemp;
      ifs >> stemp;

      stringstream ss(stemp);
      double dtemp;
      ss >> dtemp;

      if( ss.fail() )
        {
          cerr << stemp << " is skipped" << endl;
          continue;
        }
      vval.push_back( dtemp );
    }
}

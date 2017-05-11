#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// constant values
const int kHeight = 30;
const int kWidth = 80;
const int kBin_num = kHeight;
const string kChar = "*";

// functions
double GetMean   ( vector < double >& vval );
double GetStdDev ( vector < double >& vval );
double GetIntegral( vector < double >& vval );
string GetWords  ( string word, int num );
void GetContent  ( vector < double >& vval, vector < double >& vcenter, vector < int >& vcontent );

void PrintHeader ();
void PrintHist   ( vector < double >& vcenter, vector < int >& vcontent );
void PrintFooter ( vector < double >& vval );

int main( int argc, char* argv[] )
{

  // in the case of no argument
  if( argc == 1 )
    {
      cout << "Usage: " << argv[0] << " [values]" << endl;
      cerr << "Program is stopped" << endl;
      return -1;
    }

  // get values
  vector < double > vval;
  // loop over arguments
  for( int i=1; i<argc; i++ )
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
  PrintHeader();
  PrintHist( vcenter, vcontent );
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

  // calculate center of bins
  double xi = xmin;
  for( int i=0 ; i<kBin_num ; i++, xi += bin_width )
    vcenter.push_back( xi + bin_width/2 );

  // counte numbers and set the bin contents
  for( int i=0; i<vval.size(); i++ )
      for( int j=0; j<kBin_num; j++ )
	  if( vval[i] >= vcenter[j] - bin_width/2 && vval[i] < vcenter[j] + bin_width/2 )
	    {
	      vcontent[j] += 1;
	      continue;
	    }

}

// return "word" * "num"
string GetWords( string word, int num )
{
  string words = "";
  for( int i=0; i<num; i++ )
    words += word;
  return words;
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
  cout << "  " << GetWords( "=" , 11 ) << endl;
  cout << "   HISTOGRAM" << endl;
  cout << "  " << GetWords( "=" , 11 ) << endl;
  cout << "\n";
}

// print histogram and axis
void PrintHist( vector < double >& vcenter, vector < int >& vcontent )
{

  // normalize histogram
  // set the length of the longest bin to kWidth
  int max = *max_element( vcontent.begin(), vcontent.end() );

  vector < int > vcontent_normalized( kBin_num );
  for( int i=0; i<vcontent.size(); i++ )
    vcontent_normalized[i]
      = vcontent[i] * kWidth / *max_element( vcontent.begin(), vcontent.end() ) ;

  // print histogram
  for( int i=0; i<kBin_num; i++ )
    {
      cout << setw(10) << setprecision( 4 ) << vcenter[i] << " | " 
	   << GetWords( kChar , vcontent_normalized[i] ) << " "
	   << vcontent[i]
	   << endl;
    }

  // print axis
  cout << GetWords( "-" , 10 + 3 + kWidth -1) << ">" << endl;
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
  cout << "  +" << GetWords( "-" , width - 2 ) << "+" << endl;
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
  cout << "  +" << GetWords( "-" , width - 2 ) << "+" << endl;
}

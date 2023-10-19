////////////////////////////////////////////////////////////////////////////////////////////
//
//          Nome do Projeto: TMH - Truco Message Handler
//          Descrição: Servido de mensagens para o jogo de Truco
//          Autor: Rayan Silva
//          Ano: 2023
//          Licença: MIT (Consulte a seção abaixo para mais detalhes)
//
//          MIT License
//
//          Copyright (c) 2023 Rayan Silva
//
//          Permission is hereby granted, free of charge, to any person obtaining a copy
//          of this software and associated documentation files (the "Software"), to deal
//          in the Software without restriction, including without limitation the rights
//          to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//          copies of the Software, and to permit persons to whom the Software is
//          furnished to do so, subject to the following conditions:
//
//          The above copyright notice and this permission notice shall be included in
//          all copies or substantial portions of the Software.
//
//          THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//          IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//          FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//          AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//          LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//          OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//          THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//       MODULE:        commommethod
//       DATE:          21/05/2003
//
//       DESCRIPTION:   methods to help data type manipulation
//                      and conversion
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CommonMethod_
#define _CommonMethod_

#include <limits>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <string>

namespace datatype
{
   class ATNumPunct : public std::numpunct_byname< char >
   {
   public:
      ////////////////////////////////////////////////////////////////////////////////
      //       constructor
      ////////////////////////////////////////////////////////////////////////////////
      ATNumPunct( const std::string &location ) :
      std::numpunct_byname<char>( location.c_str() )
      {
      }

      virtual ~ATNumPunct() {};

      ////////////////////////////////////////////////////////////////////////////////
      //       public methods
      ////////////////////////////////////////////////////////////////////////////////
      virtual std::numpunct<char>::char_type do_thousands_sep() const
      {
         return '\0'; // interpreted as nothing by the locale class
      }

   private:
      ATNumPunct( const ATNumPunct &other ) = delete;
      ATNumPunct& operator=( const ATNumPunct &other ) = delete;
   };

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         GetMaxValue( T )
   //
   //     DESCRIPTION:    return the maximum possible value to
   //                     the variable
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   template< typename T >
   T GetMaxValue( T variable )
   {
      return (std::numeric_limits< T >::max)();
   }

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         isLittleEndian ( )
   //
   //     DESCRIPTION:    return whether bytes at lower
   //                     addresses have lower significance
   //                     ( little-endian )
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   bool IsLittleEndian();

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         ReverseEndian ( input )
   //
   //     DESCRIPTION:    invert the order the bytes are stored
   //                     in memory( little-endiand -> big-endian
   //                     or big-endian ->little-endian )
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   template< typename T >
   T ReverseEndian( const T &input )
   {
      T temp;
      const char *inputAux = reinterpret_cast< const char * >( &input );
      char *tempAux = reinterpret_cast< char * >( &temp );
      size_t varSize = sizeof( input );
      for( size_t i = 0; i < varSize; ++i )
      {
         tempAux[ i ] = inputAux[ varSize - i - 1 ];
      }
      return temp;
   }

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         fromIntegralToBinaryString ( input )
   //
   //     DESCRIPTION:    get the integral bytes and put on a
   //                     string
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   template< typename T >
   std::string &fromIntegralToBinaryString( T &inputIntegral, std::string &outputString )
   {
      char *inputAux = reinterpret_cast< char * >( &inputIntegral );
      size_t varSize = sizeof( inputIntegral );
      for( size_t i = 0; i < varSize; ++i )
      {
         outputString.push_back( inputAux[ i ] );
      }
      return outputString;
   }

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         toupperstr   ( text )
   //
   //     DESCRIPTION:    convert the text to an uppercase
   //                     representation
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   void toupperstr(std::string &text);

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         tolowerstr   ( text )
   //
   //     DESCRIPTION:    convert the text to an uppercase
   //                     representation
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   void tolowerstr(std::string &text);

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:           trimstr ( text )
   //
   //     DESCRIPTION:      remove white spaces from a string
   //
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   std::string& trimstr( std::string &text );
   std::string trimstr( const std::string &text);

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:           TrimLeftZeros ( text )
   //
   //     DESCRIPTION:      remove the left zeros from a string
   //
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   void TrimLeftZeros( std::string &text );
   std::string TrimLeftZeros( const std::string &text );

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         CalculateCrc ( text )
   //
   //     DESCRIPTION:      calculate the CRC from the text
   //                      according to the standard CCITT
   //                      polynomial X^16 + X^12 + X^5 + 1
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   unsigned short CalculateCrc16(const std::string &text);

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         CalculateChecksumSum ( text )
   //
   //     DESCRIPTION:      calculate the check sum from the text
   //                      adding the character values mod 256
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   char CalculateChecksumSum(const std::string &text);

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         CalculateChecksumXOR ( text )
   //
   //     DESCRIPTION:      calculate the checksum from the text
   //                      xoring the character values
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   char CalculateChecksumXOR(const std::string &text);

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         FromStringTo ( string )
   //
   //     DESCRIPTION:    convert a string to another type
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   template<typename T>
   T FromStringTo(const std::string& text, const std::string &localeName);

   template<typename T>
   T FromStringTo( const std::string& text )
   {
      return FromStringTo<T>( text, "" );
   }

   template<typename T>
   T FromStringTo(const std::string& text, const std::string &localeName)
   {
      std::istringstream inputStream(text);
      T output;

      #ifndef WINCE
      std::locale locale;
      try
      {
         locale = std::locale( std::locale( localeName.c_str() ) );
      }
      catch( ... )
      {
         locale = std::locale( std::locale( "" ) );
      }
      locale = std::locale( locale, new datatype::ATNumPunct( locale.name() ) );

      inputStream.imbue( locale );
      #endif

      inputStream >> std::noskipws >> output;
      return output;
   }

   // Generic format methods
   const std::string &formatNumberToConvert( const std::string &textToConvert, std::string &convertedText );
   const std::string &formatFloatingPointToConvert( const std::string &textToConvert, std::string &convertedText );

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:          formatValueToConvert( const std::string &textToConvert,
   //                                            std::string &convertedText )
   //
   //     DESCRIPTION:     Format value to convert.
   //
   //     COMMENTS:        If needs some specific format to some type,
   //                      create it's partial specialiaziation.
   //                      The generic implementation always returns the
   //                      reference to 'textToConvert'.
   //
   //                      ** Internal use, don't use this method outside
   //                         CommomMethod.h or CommomMethod.cpp
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   #ifdef WIN32
   #pragma warning( push )
   #pragma warning( disable : 4100 )
   #endif
   // Some code
   template< typename T >
   const std::string &formatValueToConvert( const std::string &textToConvert, std::string &convertedText )
   {
      return textToConvert;
   }
   #ifdef WIN32
   #pragma warning( pop )
   #endif

   template<>
   const std::string &formatValueToConvert< short >( const std::string &textToConvert, std::string &convertedText );

   template<>
   const std::string &formatValueToConvert< int >( const std::string &textToConvert, std::string &convertedText );

   template<>
   const std::string &formatValueToConvert< long >( const std::string &textToConvert, std::string &convertedText );

   template<>
   const std::string &formatValueToConvert< double >( const std::string &textToConvert, std::string &convertedText );

   template<>
   const std::string &formatValueToConvert< float >( const std::string &textToConvert, std::string &convertedText );

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:          convertFromStringTo( const std::string &text,
   //                                           const char *scanfFormatString,
   //                                           T output = 0 )
   //
   //     DESCRIPTION:     Generic method to convert from string to other type.
   //
   //     COMMENTS:        If the text to convert is empty, don't convert,
   //                      and returns 'output' value.
   //                      Use 'formatValueToConvert' function.
   //                      Returns the converted value.
   //
   //                      ** Internal use, don't use this method outside
   //                         CommomMethod.h or CommomMethod.cpp
   //
   //     THROWS:          std::range_error - If 'sscanf' convertion fails.
   ////////////////////////////////////////////////////////////////////////////////
   template< typename T >
   T convertFromStringTo( const std::string &text, const char *scanfFormatString, T output = 0 )
   {
      if( !text.empty() )
      {
         std::string convertedText;
         const std::string &textToConvert = formatValueToConvert< T >( text, convertedText );

         if( sscanf( textToConvert.c_str(), scanfFormatString, &output ) != 1 )
         {
            throw std::range_error( text );
         }
      }

      return output;
   }

   template<>
   short FromStringTo< short >(const std::string& text);

   template<>
   int FromStringTo< int >(const std::string& text);

   template<>
   long FromStringTo< long >(const std::string& text);

   template<>
   float FromStringTo< float >(const std::string& text);

   template<>
   double FromStringTo< double >(const std::string& text);

   template<>
   unsigned short FromStringTo<unsigned short>(const std::string& text);

   template<>
   unsigned int FromStringTo<unsigned int>(const std::string& text);

   template<>
   unsigned long FromStringTo<unsigned long>(const std::string& text);

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:           ToStringFrom ( input )
   //
   //     DESCRIPTION:      convert an input to it's string
   //                       representation
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   template<typename T>
   std::string ToStringFrom(const T& input, const std::string &localeName);

   template<typename T>
   std::string ToStringFrom(const T& input)
   {
      return ToStringFrom<T>( input, "" );
   }

   template<typename T>
   std::string ToStringFrom(const T& input, const std::string &localeName)
   {
      std::ostringstream outputStream;

      #ifndef WINCE
      std::locale locale;
      try
      {
         if( !localeName.empty() )
         {
            locale = std::locale( localeName.c_str() );
         }
         else
         {
            locale = std::locale( std::locale::classic() );
         }
      }
      catch( ... )
      {
         locale = std::locale( std::locale::classic() );
      }
      locale = std::locale( locale, new datatype::ATNumPunct( locale.name() ) );

      outputStream.imbue( locale );
      #endif
      outputStream << std::noskipws << input;
      return outputStream.str();
   }

   template<>
   std::string ToStringFrom<short>(const short& input);

   template<>
   std::string ToStringFrom<int>(const int& input);

   template<>
   std::string ToStringFrom<long>(const long& input);

   template<>
   std::string ToStringFrom<float>(const float& input);

   template<>
   std::string ToStringFrom<double>(const double& input);

   template<>
   std::string ToStringFrom<unsigned short>(const unsigned short& input);

   template<>
   std::string ToStringFrom<unsigned int>(const unsigned int& input);

   template<>
   std::string ToStringFrom<unsigned long>(const unsigned long& input);

   template< typename T >
   std::string FromStringToTypeScalableBinaryVector( const std::string &text )
   {
      std::istringstream inputStream;
      std::string output("");
      T binaryValue;
      unsigned short stepValue = 0;

      // Input string must me multiple of desired output type size
      if( ( text.size() % sizeof(T) != 0 ) || !text.size() )
      {
         return "";
      }

      for( size_t i = 0; i < text.size(); i += sizeof(T) )
      {
         inputStream.str( text.substr( i, sizeof(T) ) );
         if( sizeof( T ) == 1 )
         {
            inputStream >> std::noskipws >> stepValue;
            binaryValue = stepValue;
         }
         else
         {
            inputStream >> std::noskipws >> binaryValue;
         }
         inputStream.clear();

         output += ToStringFrom< T >( binaryValue ).substr( 0, sizeof(T) );
      }

      return output;
   }

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         FromStringToHexa ( stringText )
   //
   //     DESCRIPTION:    convert from string to hexa
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   std::string FromStringToHexa( const std::string &stringText );

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         FromHexaToString( hexaText )
   //
   //     DESCRIPTION:    convert from hexa to string
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   std::string FromHexaToString( const std::string &hexaText );

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         FromHexaToInt( hexaNumber )
   //
   //     DESCRIPTION:    convert from hexa to int
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   int FromHexaToInt( const std::string &hexaNumber );

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         FromIntToHexa( intNumber )
   //
   //     DESCRIPTION:    convert from int to hexa
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   std::string FromIntToHexa( int intNumber );

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:          FromIntToHexa( long number, int size )
   //
   //     DESCRIPTION:     Transform an integer to hexadecimal in string with specified size.
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   std::string FromIntToHexa( const int number, const std::streamsize size );

   template < typename T >
   std::string ToHexaFrom( T typeNumber )
   {
      std::ostringstream hexaText;
      hexaText << std::hex << std::uppercase << typeNumber;
      std::string response = hexaText.str();
      if( !response.empty() )
      {
         return "0" + response;
      }
      return response;
   }


   template< typename T >
   void Split( const std::string &text, const char delim, std::vector< T > &elementsVector )
   {
      std::stringstream stream( text );
      std::string code;
      while( std::getline( stream, code, delim ) )
      {
         if( !code.empty() )
         {
            elementsVector.push_back( datatype::FromStringTo< T >( code ) );
         }
      }
   }

   void ReplaceString( std::string& str, const std::string& from, const std::string& to );

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         FileLength( std::string name )
   //
   //     DESCRIPTION:    return the length of the file
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   std::streamoff FileLength( const std::string &name );

   std::string RemoveZeros( const std::string &text );

   #ifdef WIN32
   std::string GetCurrentPath();
   #endif

   #if defined( WIN32 ) || defined( WINCE )
   void ConvertUnicodeToAscii( std::string &outputBuffer, LPWSTR inputBuffer );
   void ConvertAsciiToUnicode( LPWSTR outputBuffer, const std::string &inputBuffer, int outputWideSize );
   #endif

   ////////////////////////////////////////////////////////////////////////////////
   //     METHOD:         GetAbsolutePath( std::string name )
   //
   //     DESCRIPTION:    return the absolute path of the file
   //
   //     COMMENTS:
   //
   //     THROWS:
   ////////////////////////////////////////////////////////////////////////////////
   bool GetAbsolutePath( const std::string &path, std::string &fullPath );
}
#endif
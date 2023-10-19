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
#include "utility/CommonMethod.h"
#include <iomanip>

#if defined( linux ) | defined( __linux ) | defined( __linux__ )
#include "linux/limits.h"
#endif

namespace datatype
{
////////////////////////////////////////////////////////////////////////////////
//     METHOD:         toupperstr( text )
//
//     DESCRIPTION:    convert the text to an uppercase
//                     representation
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
void toupperstr( std::string &text )
{
    for( std::string::iterator index = text.begin();
            index!= text.end();
            ++index )
    {
        *index = static_cast< char >( toupper( *index ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         tolowerstr( text )
//
//     DESCRIPTION:    convert the text to an uppercase
//                     representation
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
void tolowerstr(std::string &text)
{
    for( std::string::iterator index = text.begin();
            index!= text.end();
            ++index )
    {
        *index = static_cast< char >( tolower( *index ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         trimstr( text )
//
//     DESCRIPTION:    remove white spaces from a string
//
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
std::string& trimstr( std::string& text )
{
    std::string whitespaces(" \t\f\v\n\r");
    std::string::size_type pos = text.find_last_not_of( whitespaces );
    if ( pos != std::string::npos )
    {
        text.erase( pos + 1 );
        pos = text.find_first_not_of( whitespaces );
        if ( pos != std::string::npos )
        {
            text.erase( 0, pos );
        }
    }
    else
    {
        text.erase( text.begin(), text.end() );
    }
    return text;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         trimstr( text )
//
//     DESCRIPTION:    remove white spaces from a string
//
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
std::string trimstr( const std::string &text)
{
    std::string textResult( text );
    trimstr( textResult );

    return textResult;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         TrimLeftZeros( text )
//
//     DESCRIPTION:    remove the left zeros from a string
//
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
void TrimLeftZeros( std::string &text )
{
    std::string::size_type pos = text.find_first_not_of( "0" );

    if ( pos != std::string::npos )
    {
        text.erase( 0, pos );
    }
}


////////////////////////////////////////////////////////////////////////////////
//     METHOD:         ReplaceString( std::string& str, const std::string& from, const std::string& to )
//
//     DESCRIPTION:    replace terms in a string
//
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
void ReplaceString( std::string& str, const std::string& from, const std::string& to )
{
    if( from.empty() )
    {
        return;
    }

    std::string::size_type start_pos = 0;
    while( ( start_pos = str.find( from, start_pos ) ) != std::string::npos )
    {
        str.replace( start_pos, from.length(), to );
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::streamoff FileLength( const std::string &name )
{
    std::ifstream file( name.c_str() );
    file.seekg( 0, std::ios_base::end );
    return file.tellg();
}

std::string RemoveZeros( const std::string &text )
{
    return datatype::ToStringFrom< int >( datatype::FromStringTo< int >( text ) );
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         TrimLeftZeros( text )
//
//     DESCRIPTION:    remove the left zeros from a string
//
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
std::string TrimLeftZeros( const std::string &text )
{
    std::string textResult( text );
    TrimLeftZeros( textResult );

    return textResult;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         CalculateCrc16 ( text )
//
//     DESCRIPTION:      calculate the CRC from the text
//                      according to the standard CCITT
//                      polynomial X^16 + X^12 + X^5 + 1
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
unsigned short CalculateCrc16(const std::string &text)
{
    unsigned short checkSum = 0xFFFF;

    if( text.empty() )
    {
        return 0;
    }

    std::string::const_iterator index = text.begin();
    for( ; index != text.end(); ++index )
    {
        for( int i = 0; i < 8; ++i )
        {
            if( ( ( unsigned short )( ( *index ) << ( i + 8 ) ) ^ checkSum ) & 0x8000 )
            {
                checkSum = ( checkSum << 1 ) ^ 0x1021U;
            }
            else
            {
                checkSum <<= 1;
            }
        }
    }

    return checkSum;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         CalculateChecksumSum ( text )
//
//     DESCRIPTION:      calculate the CRC from the text
//                      according to the standard CCITT
//                      polynomial X^16 + X^12 + X^5 + 1
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
char CalculateChecksumSum(const std::string &text)
{
    int checkSum = 0;
    enum
    {
        CKSUM_MODULO = 256
    };

    // if 'text' is empty return
    if ( text.size() == 0 )
    {
        return 0;
    }

    // calculate CRC
    std::string::const_iterator index = text.begin();
    for(; index != text.end(); ++index )
    {
        checkSum += (int)( *index );
        if( checkSum >= CKSUM_MODULO )
        {
            checkSum = ( checkSum % CKSUM_MODULO ) + 1;
        }
    }

    return (char)checkSum;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         CalculateChecksumXOR ( text )
//
//     DESCRIPTION:      calculate the CRC from the text
//                      according to the standard CCITT
//                      polynomial X^16 + X^12 + X^5 + 1
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
char CalculateChecksumXOR(const std::string &text)
{
    char checkSum = 0;

    if ( text.size() == 0 )
    {
        return 0;
    }

    std::string::const_iterator index = text.begin();
    for(; index != text.end(); ++index )
    {
        checkSum ^= *index;
    }

    return checkSum;
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
//Commented because annother method with better performance was found.
//std::string FromStringToHexa( const std::string &stringText )
//{
//   std::ostringstream hexaText;
//   std::string::const_iterator index = stringText.begin();
//   while( index != stringText.end() )
//   {
//      hexaText << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)(0xFF & *(index++));
//   }
//   return hexaText.str();
//}

//The new method with better performance.
//This code is about 160 times more fast than the old method.
std::string FromStringToHexa( const std::string &stringText )
{
    static char const* digits( "0123456789ABCDEF" );

    std::string result( stringText.size() * 2, 0 );
    std::string::iterator ptr( result.begin() );

    for( std::string::const_iterator cur( stringText.begin() ), end( stringText.end() ); cur != end; ++cur )
    {
        unsigned char c( *cur );

        *ptr++ = digits[ c >> 4 ];
        *ptr++ = digits[ c & 15 ];
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         FromHexaToString( hexaText )
//
//     DESCRIPTION:    convert from hexa to string
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
std::string FromHexaToString( const std::string &hexaText )
{
    std::string text;
    int charAux;
    size_t i = 0;
    while( i < hexaText.size() )
    {
        std::istringstream stringText( hexaText.substr( i, 2 ) );
        stringText >> std::hex >> charAux;
        text += (char)charAux;
        i += 2;
    }
    return text;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         FromHexaToInt( hexaNumber )
//
//     DESCRIPTION:    convert from hexa to int
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
int FromHexaToInt( const std::string &hexaNumber )
{
    int number;
    std::istringstream stringText( hexaNumber );
    stringText >> std::hex >> number;
    return number;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:         FromIntToHexa( intNumber )
//
//     DESCRIPTION:    convert from int to hexa
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
std::string FromIntToHexa( int intNumber )
{
    std::ostringstream hexaText;
    hexaText << std::hex << std::uppercase << intNumber;
    std::string response = hexaText.str();
    // if response.size() is an odd number, start string with a zero char
    if( response.size() & 1 )
    {
        response.insert( response.begin(), '0' );
    }
    return response;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:          FromIntToHexa( number, size )
//
//     DESCRIPTION:     Transform an integer to hexadecimal in string with specified size.
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
std::string FromIntToHexa( const int number, const std::streamsize size )
{
    std::string hexaValue = FromIntToHexa( number );
    std::ostringstream formatedHexaString;
    formatedHexaString << std::setfill( '0' ) << std::setw( size ) << hexaValue;
    return formatedHexaString.str();
}

#if defined( WIN32 ) || defined( WINCE )
////////////////////////////////////////////////////////////////////////////////
//     METHOD:          ConvertUnicodeToAscii( number, size )
//
//     DESCRIPTION:     Convert an unicode string to an ascii string
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
void ConvertUnicodeToAscii( std::string &outputBuffer, LPWSTR inputBuffer )
{
    size_t length = wcslen( inputBuffer );
    char *temp = new char[ length + 1 ];

    WideCharToMultiByte( CP_ACP,
                         WC_COMPOSITECHECK, // WC_NO_BEST_FIT_CHARS
                         inputBuffer,
                         static_cast< int >( length ),
                         temp,
                         static_cast< int >( length ),
                         NULL,
                         NULL );

    temp[ length ] = '\0';
    outputBuffer = temp;
    delete [] temp;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:          ConvertUnicodeToAscii( number, size )
//
//     DESCRIPTION:     Convert an ascii string to an unicode string
//
//     COMMENTS:
//
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
void ConvertAsciiToUnicode( LPWSTR outputBuffer, const std::string &inputBuffer, int outputWideSize )
{
    size_t length;

    length = MultiByteToWideChar( CP_ACP,
                                  0,
                                  inputBuffer.c_str(),
                                  static_cast< int >( inputBuffer.size() ),
                                  outputBuffer,
                                  outputWideSize - 1 );

    if( length > 0 )
    {
        outputBuffer[ length ] = _T( '\0' );
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////
//     METHOD:          formatNumberToConvert( const std::string &textToConvert, std::string &convertedText )
//
//     DESCRIPTION:     Format number to convert.
//
//     COMMENTS:        Remove characters before '-', if have it.
//                      Because to format negative value, the string must initiate with '-'.
//                      Return reference to formatted value.
//
//                      ** Internal use, don't use this method outside
//                         CommomMethod.h or CommomMethod.cpp
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
const std::string &formatNumberToConvert( const std::string &textToConvert, std::string &convertedText )
{
    std::string::size_type pos = textToConvert.find( '-' );
    if( pos != std::string::npos && pos > 0 )
    {
        convertedText = textToConvert.substr( pos );
        return convertedText;
    }

    return textToConvert;
}

////////////////////////////////////////////////////////////////////////////////
//     METHOD:          formatFloatingPointToConvert( const std::string &textToConvert,
//                                                    std::string &convertedText )
//
//     DESCRIPTION:     Format floating point value, to convert.
//
//     COMMENTS:        Use 'formatNumberToConvert' function.
//                      Change ',' characters to '.'.
//                      Return reference to formatted value.
//
//                      ** Internal use, don't use this method outside
//                         CommomMethod.h or CommomMethod.cpp
//     THROWS:
////////////////////////////////////////////////////////////////////////////////
const std::string &formatFloatingPointToConvert( const std::string &textToConvert, std::string &convertedText )
{
    const std::string &refToReturn = formatNumberToConvert( textToConvert, convertedText );

    // Changing ',' to '.'
    const std::string::size_type pos = refToReturn.find( ',' );
    if( pos != std::string::npos )
    {
        if( &refToReturn != &convertedText )
        {
            convertedText = refToReturn;
        }

        convertedText.replace( pos, 1, 1, '.' );

        return convertedText;
    }

    return refToReturn;
}

template<>
const std::string &formatValueToConvert< short >( const std::string &textToConvert, std::string &convertedText )
{
    return formatNumberToConvert( textToConvert, convertedText );
}

template<>
const std::string &formatValueToConvert< int >( const std::string &textToConvert, std::string &convertedText )
{
    return formatNumberToConvert( textToConvert, convertedText );
}

template<>
const std::string &formatValueToConvert< long >( const std::string &textToConvert, std::string &convertedText )
{
    return formatNumberToConvert( textToConvert, convertedText );
}

template<>
const std::string &formatValueToConvert< double >( const std::string &textToConvert, std::string &convertedText )
{
    return formatFloatingPointToConvert( textToConvert, convertedText );
}

template<>
const std::string &formatValueToConvert< float >( const std::string &textToConvert, std::string &convertedText )
{
    return formatFloatingPointToConvert( textToConvert, convertedText );
}

template<>
short FromStringTo< short >(const std::string& text)
{
    return convertFromStringTo< short >( text, "%hd" );
}

template<>
int FromStringTo< int >(const std::string& text)
{
    return convertFromStringTo< int >( text, "%d" );
}

template<>
long FromStringTo< long >(const std::string& text)
{
    return convertFromStringTo< long >( text, "%ld" );
}

template<>
float FromStringTo< float >(const std::string& text)
{
    return convertFromStringTo< float >( text, "%f", 0.0 );
}

template<>
double FromStringTo< double >(const std::string& text)
{
    return convertFromStringTo< double >( text, "%lf", 0.0 );
}

template<>
unsigned short FromStringTo<unsigned short>(const std::string& text)
{
    return convertFromStringTo< unsigned short >( text, "%hu" );
}

template<>
unsigned int FromStringTo<unsigned int>(const std::string& text)
{
    return convertFromStringTo< unsigned int >( text, "%u" );
}

template<>
unsigned long FromStringTo<unsigned long>(const std::string& text)
{
    return convertFromStringTo< unsigned long >( text, "%lu" );
}

template<>
std::string ToStringFrom<short>(const short& input)
{
    char outputStream[12] = "\0";
    sprintf( outputStream, "%hd", input );
    return std::string(outputStream);
}

template<>
std::string ToStringFrom<int>(const int& input)
{
    char outputStream[12] = "\0";
    sprintf( outputStream, "%d", input );
    return std::string(outputStream);
}

template<>
std::string ToStringFrom<long>(const long& input)
{
    char outputStream[12] = "\0";
    sprintf( outputStream, "%ld", input );
    return std::string(outputStream);
}

template<>
std::string ToStringFrom<float>(const float& input)
{
    char outputStream[1024] = "\0";
    sprintf( outputStream, "%f", input );
    return std::string(outputStream);
}

template<>
std::string ToStringFrom<double>(const double& input)
{
    // This is dynamic allocation version of max size of double for use
    // in plataform independent code
    //static int maxBytes = 0;
    //static std::ostringstream aux;
    //if( maxBytes == 0 )
    //{
    //   aux << std::fixed << std::numeric_limits<double>::max();
    //   maxBytes = aux.str().size() + 2;
    //}
    //char *outputStream = new char[ maxBytes ];
    //memset( outputStream, 0x0, maxBytes );
    //sprintf( outputStream, "%lf", input );
    //std::string ret( outputStream );
    //delete []outputStream;
    //return ret;
    char outputStream[1024] = "\0";
    sprintf( outputStream, "%lf", input );
    return std::string(outputStream);
}

template<>
std::string ToStringFrom<unsigned short>(const unsigned short& input)
{
    char outputStream[12] = "\0";
    sprintf( outputStream, "%hu", input );
    return std::string(outputStream);
}

template<>
std::string ToStringFrom<unsigned int>(const unsigned int& input)
{
    char outputStream[12] = "\0";
    sprintf( outputStream, "%u", input );
    return std::string(outputStream);
}

template<>
std::string ToStringFrom<unsigned long>(const unsigned long& input)
{
    char outputStream[12] = "\0";
    sprintf( outputStream, "%lu", input );
    return std::string(outputStream);
}

#if !defined( WINCE ) && defined( WIN32 )
std::string GetCurrentPath()
{
    std::string path;
    std::string dir;
    std::string::size_type pos;

    char pathArray[ _MAX_PATH ];
    DWORD length = 0;

    if( ( length = GetModuleFileName( NULL, pathArray, _MAX_PATH ) ) != 0 )
    {
        path.append( pathArray, static_cast< size_t >( length ) );
    }

    if( ( pos = path.rfind( "\\" ) ) != std::string::npos )
    {
        if( path[ 0 ] == '\"' )
        {
            dir = path.substr( 1, pos );
        }
        else
        {
            dir = path.substr( 0, ( pos + 1 ) );
        }
    }
    else
    {
        dir = ".\\";
    }

    return dir;
}
#endif

#if !defined( WINCE )
bool GetAbsolutePath( const std::string &path, std::string &fullPath )
{
#if defined( WIN32 )
    char szFullPath[ MAX_PATH ];
    if( !GetFullPathName( path.c_str(), MAX_PATH, szFullPath, NULL ) )
    {
        return false;
    }
#else
    char szFullPath[ PATH_MAX ];
    if( realpath( path.c_str(), szFullPath ) == NULL )
    {
        return false;
    }
#endif

    fullPath = szFullPath;
    return true;
}
#endif
}

#ifndef __RFileParser_H__
#define __RFileParser_H__
#include <stdio.h>
#include <string>
#include "RFileloader.h"
typedef std::string RWord;
#define RWORDS_COUNT 1
const RWord res_word[RWORDS_COUNT] = { "$include" };
class RFileParser
{
public:
	static uint stringLength( const char *a )
	{
		for( int i = 0; ; ++i )
		{
			if( a[i] == '\0' ) return i;
		}
		return 0;
	}
	static std::string genParsed( std::string fullname )
	{
		int path_pos = getFileNamePosition( fullname );
		const std::string catalog = fullname.substr( 0 , path_pos );
		RWord str = RFileLoader::loadFile( fullname );
		//std::cout << str << "\n";
		size_t pos = 0;
		for( const char *t = str.c_str(); *t != '\0'; ++t , ++pos )
        {
			if( *t == '$' )
			{
				size_t size = 0;
				size = toSpace( t );
				RWord command = str.substr( pos , size );
				str.erase( pos , size + 1 );
				int pi = getPreproc( command );
				size = toSpace( t );
				//std::cout << size << "\n";
				RWord wr = str.substr( pos , size );
				str.erase( pos , size );
				switch( pi )
				{
					case 0:
					{
						std::string fullname_( catalog );
						fullname_.append( wr );
						std::string file = RFileLoader::loadFile( fullname_.c_str() );
						str.insert( pos , file );
						t = str.c_str();
						pos = 0;
					}
					break;
				}
			}
		}
		return str;
    }
	static uint toSpace( const char *r )
	{
		uint size = 0;
		for( ; *r != '\n' && *r != ' ' && *r != '\0' ; ++r , ++size ){}
		return size;
	}
	static uint getPreproc( RWord const c )
    {
		for( uint i = 0; i < RWORDS_COUNT; ++i )
            if( !bool( res_word[i].compare( c ) ) ) return i;
		return 0;
    }
	static bool compare( RWord const a , RWord const b )
    {
        /*while( ( *a++ == *b++ ) && ( *a != '\0' && *b != '\0' ) ){}
        printf( "%c %c\n" , *a , *b );
        if( a[-1] == b[-1] && a[0] == b[0] )
            return true;*/
        return !bool( a.compare( b ) );
    }
	static uint getFileNamePosition( std::string &fullname )
	{
		uint l = fullname.size();
		for( uint i = l - 1; i >= 0; --i )
			if( fullname.c_str()[i] == '/' ) return i + 1;
		return 0;
	}
};
#endif

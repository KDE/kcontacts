#include <sys/times.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>

#include "addressbook.h"
#include "vcardformat.h"
#include "plugins/file/resourcefile.h"

using namespace KABC;

int main(int argc,char **argv)
{
  KAboutData aboutData("bigwrite","BigWriteKabc","0.1");
  KCmdLineArgs::init(argc,argv,&aboutData);

  KApplication app;

  AddressBook ab;
  ResourceFile r( "my.kabc" );
  ab.addResource( &r );
  
  for( int i = 0; i < 5000; ++i ) {
    Addressee a;
    a.setGivenName( "number" + QString::number( i ) );
    a.setFamilyName( "Name" );
    a.insertEmail( QString::number( i ) + "@domain" );
    
    ab.insertAddressee( a );
  }
  printf( "\n" );
  
  Ticket *t = ab.requestSaveTicket( &r );
  if ( t ) {
    struct tms start;

    times( &start );

#if 0
    kdDebug() << "utime : " << int( start.tms_utime ) << endl;
    kdDebug() << "stime : " << int( start.tms_stime ) << endl;
    kdDebug() << "cutime: " << int( start.tms_cutime ) << endl;
    kdDebug() << "cstime: " << int( start.tms_cstime ) << endl;
#endif
	    
    if ( !ab.save( t ) ) {
      kdDebug() << "Can't save." << endl;
    }

    struct tms end;

    times( &end );

#if 0
    kdDebug() << "utime : " << int( end.tms_utime ) << endl;
    kdDebug() << "stime : " << int( end.tms_stime ) << endl;
    kdDebug() << "cutime: " << int( end.tms_cutime ) << endl;
    kdDebug() << "cstime: " << int( end.tms_cstime ) << endl;
#endif

    kdDebug() << "UTime: " << int( end.tms_utime ) - int( start.tms_utime ) << endl; 
    kdDebug() << "STime: " << int( end.tms_stime ) - int( start.tms_stime ) << endl; 

  } else {
    kdDebug() << "No ticket for save." << endl;
  }
}

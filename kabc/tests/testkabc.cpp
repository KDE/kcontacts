#include <QtCore/QFile>
#include <QtGui/QImage>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kstandarddirs.h>

#include "kabc/geo.h"
#include "kabc/secrecy.h"
#include "kabc/stdaddressbook.h"
#include "kabc/timezone.h"
#include "kabc/key.h"
#include "kabc/vcardconverter.h"

using namespace KABC;

int main(int argc,char **argv)
{
    KAboutData aboutData("testkabc",I18N_NOOP("TestKabc"),"0.1");
    KCmdLineArgs::init(argc, argv, &aboutData);

    KApplication app( false );
    AddressBook *ab = StdAddressBook::self();

#define READ

#ifdef READ
    AddressBook::Iterator it;
    for ( it = ab->begin(); it != ab->end(); ++it ) {
      VCardConverter converter;
      QString vcard = converter.createVCard( *it );
      kDebug() << "card=" << vcard << endl;
    }
#else
    Addressee addr;

    addr.setGivenName("Tobias");
    addr.setFamilyName("Koenig");


    Picture pic;
    QImage img;
    img.load("/home/tobias/test.png");
/*
    pic.setData(img);
    pic.setType(QImage::imageFormat("/home/tobias/test.png"));
*/
    pic.setUrl("http://www.mypict.de");
    addr.setLogo( pic );

    ab->insertAddressee( addr );

    StdAddressBook::save();
#endif

    return 0;
}

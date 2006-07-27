#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kstandarddirs.h>

#include "kabc/addressbook.h"
#include "kabc/addresseelist.h"

using namespace KABC;

static const KCmdLineOptions options[] =
{
  { "save", "", 0 },
  { "number", "", 0 },
  KCmdLineLastOption
};

int main(int /*argc*/,char /* **argv*/)
{
/*  KAboutData aboutData("testaddresseelist","TestAddresseeList","0.1");
  KCmdLineArgs::init(argc, argv, &aboutData);
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app;
  KCmdLineArgs* args = KCmdLineArgs::parsedArgs(); */

  kDebug() << "Creating addressees" << endl;
  Addressee a, b, c, d, e, f;
  a.setGivenName ("Peter");
  a.setFamilyName("Pan");
  a.setFormattedName("Pan, Peter");
  a.setUid("Asdf");
  b.setGivenName ("Phileas");
  b.setFamilyName("Fogg");
  b.setFormattedName("Fogg, Phileas");
  b.setUid("Rsdf");
  c.setGivenName ("Jim");
  c.setFamilyName("Hawkins");
  c.setFormattedName("Hawkins, Jim");
  c.setUid("Fhwn");
  d.setGivenName ("John");
  d.setFamilyName("Silver");
  d.setPrefix    ("Long");
  d.setFormattedName("Long John Silver");
  d.setUid("Z2hk");
  e.setGivenName ("Alice");
  e.setFamilyName("Liddel");
  e.setFormattedName("Liddel, Alice");
  e.setUid("kk45");
  f.setGivenName ("Edmond");
  f.setFamilyName("Dantes");
  f.setFormattedName("Dantes, Edmond");
  f.setUid("78ze");

  kDebug() << "Adding to list" << endl;
  AddresseeList list;
  list.append(a);
  list.append(b);
  list.append(c);
  list.append(d);
  list.append(e);
  list.append(f);

  list.sortBy(FamilyName);
  if ( !(   (*list.at(0)).uid()=="78ze"
         && (*list.at(1)).uid()=="Rsdf"
         && (*list.at(2)).uid()=="Fhwn"
         && (*list.at(3)).uid()=="kk45"
         && (*list.at(4)).uid()=="Asdf"
         && (*list.at(5)).uid()=="Z2hk"
        ) ) {
      kError() << "SORTING BY FAMILY NAME NOT CORRECT!" << endl;
      kDebug() << "list sorted by family name:" << endl;
      list.dump();
  } else {
      kDebug() << "Sorting by family name correct." << endl;
  }
  list.setReverseSorting(true);
  list.sort();
  if ( !(   (*list.at(5)).uid()=="78ze"
         && (*list.at(4)).uid()=="Rsdf"
         && (*list.at(3)).uid()=="Fhwn"
         && (*list.at(2)).uid()=="kk45"
         && (*list.at(1)).uid()=="Asdf"
         && (*list.at(0)).uid()=="Z2hk"
        ) ) {
      kError() << "REVERSE SORTING BY FAMILY NAME NOT CORRECT!" << endl;
      kDebug() << "list reverse sorted by family name:" << endl;
      list.dump();
  } else {
      kDebug() << "Reverse sorting by family name correct." << endl;
  }

  list.setReverseSorting(false);
  list.sortBy(FormattedName);
  if ( !(   (*list.at(0)).uid()=="78ze"
         && (*list.at(1)).uid()=="Rsdf"
         && (*list.at(2)).uid()=="Fhwn"
         && (*list.at(3)).uid()=="kk45"
         && (*list.at(4)).uid()=="Z2hk"
         && (*list.at(5)).uid()=="Asdf"
        ) ) {
      kError() << "SORTING BY FORMATTED NAME NOT CORRECT!" << endl;
      kDebug() << "list sorted by formatted name:" << endl;
      list.dump();
  } else {
      kDebug() << "Sorting by formatted name correct." << endl;
  }
  list.setReverseSorting(true);
  list.sort();
  if ( !(   (*list.at(5)).uid()=="78ze"
         && (*list.at(4)).uid()=="Rsdf"
         && (*list.at(3)).uid()=="Fhwn"
         && (*list.at(2)).uid()=="kk45"
         && (*list.at(1)).uid()=="Z2hk"
         && (*list.at(0)).uid()=="Asdf"
        ) ) {
      kError() << "REVERSE SORTING BY FORMATTED NAME NOT CORRECT!" << endl;
      kDebug() << "list reverse sorted by formatted name:" << endl;
      list.dump();
  } else {
      kDebug() << "Reverse sorting by formatted name correct." << endl;
  }


  list.setReverseSorting(false);
  list.sortBy(Uid);
  if ( !(   (*list.at(0)).uid()=="78ze"
         && (*list.at(1)).uid()=="Asdf"
         && (*list.at(2)).uid()=="Fhwn"
         && (*list.at(3)).uid()=="Rsdf"
         && (*list.at(4)).uid()=="Z2hk"
         && (*list.at(5)).uid()=="kk45"
        ) ) {
      kError() << "SORTING BY UID NOT CORRECT!" << endl;
      kDebug() << "list sorted by Uid:" << endl;
      list.dump();
  } else {
      kDebug() << "Sorting by Uid correct." << endl;
  }
  list.setReverseSorting(true);
  list.sortBy(Uid);
  if ( !(   (*list.at(5)).uid()=="78ze"
         && (*list.at(4)).uid()=="Asdf"
         && (*list.at(3)).uid()=="Fhwn"
         && (*list.at(2)).uid()=="Rsdf"
         && (*list.at(1)).uid()=="Z2hk"
         && (*list.at(0)).uid()=="kk45"
        ) ) {
      kError() << "REVERSE SORTING BY UID NOT CORRECT!" << endl;
      kDebug() << "list sorted by Uid:" << endl;
      list.dump();
  } else {
      kDebug() << "Reverse sorting by Uid correct." << endl;
  }

  // zero, one or two entries might give errors in a poor sorting
  // implementation
  kDebug() << "sorting empty list" << endl;
  AddresseeList list2;
  list2.sort();

  kDebug() << "sorting one entry list" << endl;
  list2.append(a);
  list2.sort();

  kDebug() << "sorting two entry list" << endl;
  list2.append(f);
  list2.setReverseSorting(false);
  list2.sort();
  if ( !(   (*list2.at(0)).uid()=="78ze"
         && (*list2.at(1)).uid()=="Asdf"
        ) ) {
      kError() << "SORTING BY FORMATTED NAME IN A TWO ENTRY LIST NOT CORRECT!" << endl;
      kDebug() << "list sorted by formatted name, two entries:" << endl;
      list2.dump();
  } else {
      kDebug() << "Sorting by FormattedName in a two entry list correct." << endl;
  }
  list2.setReverseSorting(true);
  list2.sort();
  if ( !(   (*list2.at(1)).uid()=="78ze"
         && (*list2.at(0)).uid()=="Asdf"
        ) ) {
      kError() << "REVERSE SORTING BY FORMATTED NAME IN A TWO ENTRY LIST NOT CORRECT!" << endl;
      kDebug() << "list reverse sorted by formatted name, two entries:" << endl;
      list2.dump();
  } else {
      kDebug() << "Reverse sorting by FormattedName in a two entry list correct." << endl;
  }

}



#ifndef KABC_RESOURCEFILE_H
#define KABC_RESOURCEFILE_H

#include <sys/types.h>

#include "resource.h"

class QTimer;

namespace KABC {

class Format;

/**
  @internal
*/
class ResourceFile : public QObject, public Resource {
    Q_OBJECT
  public:
    ResourceFile( AddressBook *, const QString &filename, Format *format=0 );
    ~ResourceFile();
  
    bool open();
    void close();
  
    Ticket *requestSaveTicket();

    bool load( AddressBook * );
    bool save( Ticket * );

    /**
      Set name of file to be used for saving.
    */
    void setFileName( const QString & );
    /**
      Return name of file used for loading and saving the address book.
    */
    QString fileName() const;

  protected slots:
    void checkFile();

  protected:
    bool lock( const QString &fileName );
    void unlock( const QString &fileName );


  private:
    QString mFileName;
    Format *mFormat;

    QString mLockUniqueName;
    
    QTimer *mFileCheckTimer;
    time_t mChangeTime;
};

}

#endif

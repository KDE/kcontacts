#ifndef KABC_RESOURCE_H
#define KABC_RESOURCE_H

#include "addressbook.h"

namespace KABC {

/**
  @short Helper class for handling coordinated save of address books.
  
  This class is used as helper class for saving address book.
  @see requestSaveTicket(), save().
*/
class Ticket
{
    friend class Resource;
  public:
    Resource *resource() { return mResource; }

  private:    
    Ticket( Resource *resource ) : mResource( resource ) {}
    
    Resource *mResource;
};


/**
  @internal
*/
class Resource {
  public:
    Resource( AddressBook * );
    virtual ~Resource();
  
    virtual bool open();
    virtual void close();
  
    virtual Ticket *requestSaveTicket();
  
    virtual bool load();
    virtual bool save( Ticket * );

    AddressBook *addressBook() { return mAddressBook; }

    virtual QString identifier();

    virtual void removeAddressee( const Addressee& addr );

    virtual void setReadOnly( bool value );
    virtual bool readOnly();

    virtual void setFastResource( bool value );
    virtual bool fastResource();

    virtual void setName( const QString& name );
    virtual QString name() const;

    virtual QString typeInfo() const;
    virtual QString paramInfo() const;

    static QString cryptStr( const QString & );

  protected:
    Ticket *createTicket( Resource * );

  private:
    AddressBook *mAddressBook;
    QString mName;
    bool mReadOnly;
    bool mFastResource;
};

}

#endif

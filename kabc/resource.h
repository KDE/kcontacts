#ifndef KABC_RESOURCE_H
#define KABC_RESOURCE_H

#include "addressbook.h"

namespace KABC {

/**
  @short Helper class for handling coordinated save of address books.
  
  This class is used as helper class for saving address book. @See
  requestSaveTicket(), save().
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
  
    virtual bool load( AddressBook * );
    virtual bool save( Ticket * );

    void setAddressBook( AddressBook * );
    AddressBook *addressBook() { return mAddressBook; }

  protected:
    Ticket *createTicket( Resource * );

  private:
    AddressBook *mAddressBook;
};

}

#endif

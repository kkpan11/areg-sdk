#pragma once

/************************************************************************
 * \file        subscriber/src/Subscriber.hpp
 * \ingroup     AREG SDK, Automated Real-time Event Grid Software Development Kit examples
 * \author      Artak Avetyan
 * \brief       Collection of AREG SDK examples.
 *              This file is a demonstration of PubSub feature service consumer.
 *              It dynamically subscribes and unsubscribes on data update.
 ************************************************************************/
 /************************************************************************
  * Include files.
  ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/component/Component.hpp"
#include "examples/22_pubsub/services/PubSubClientBase.hpp"

/**
 * \brief   The PubSub service consumer object.
 *          This dynamically subscribes and unsubscribes on data update
 *          and automatically receives the data update notifications,
 *          outputs on console the old and actual values to compare.
 *          There are 2 types of data updated: on change and always.
 *              - On change: this means to receive update notifications
 *                           only if the value is changed. For example,
 *                           the string should be received only when value changed.
 *              - Always   : this means to receive update notification each
 *                           the value is set even if the value is not updated.
 **/
class Subscriber: protected Component
                , protected PubSubClientBase
{
//////////////////////////////////////////////////////////////////////////
// Static methods
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Called by system to instantiate the component.
     * \param   entry   The entry of registry, which describes the component.
     * \param   owner   The component owning thread.
     * \return  Returns instantiated component to run in the system
     **/
    static Component * CreateComponent( const NERegistry::ComponentEntry & entry, ComponentThread & owner );

    /**
     * \brief   Called by system to delete component and free resources.
     * \param   compObject  The instance of component previously created by CreateComponent method.
     * \param   entry   The entry of registry, which describes the component.
     **/
    static void DeleteComponent( Component & compObject, const NERegistry::ComponentEntry & entry );

//////////////////////////////////////////////////////////////////////////
// Constructor / destructor
//////////////////////////////////////////////////////////////////////////
protected:

    /**
     * \brief   Instantiates the component object.
     * \param   entry   The entry of registry, which describes the component.
     * \param   owner   The component owning thread.
     **/
    Subscriber( const NERegistry::ComponentEntry & entry, ComponentThread & owner );

    virtual ~Subscriber( void ) = default;

//////////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////////
protected:
/************************************************************************/
// IEProxyListener Overrides
/************************************************************************/
    /**
     * \brief   Triggered when receives service provider connected / disconnected event.
     *          When the service provider is connected, the client objects can set the listeners here.
     *          When the service provider is disconnected, the client object should clean the listeners.
     *          Up from connected status, the clients can subscribe and unsubscribe on updates,
     *          responses and broadcasts, can trigger requests. Before connection, the clients cannot
     *          neither trigger requests, nor receive data update messages.
     * \param   status  The service connection status.
     * \param   proxy   The Service Interface Proxy object, which is notifying service connection.
     * \return  Return true if this service connect notification was relevant to client object.
     **/
    virtual bool serviceConnected( NEService::eServiceConnection status, ProxyBase & proxy ) override;

    /**
     * \brief   Triggered, when StringOnChange attribute is updated. The function contains
     *          attribute value and validation flag. When notification is enabled,
     *          the method should be overwritten in derived class.
     *          Attributes StringOnChange description:
     *          This simple string should send update notification only on value change.
     * \param   StringOnChange  The value of StringOnChange attribute.
     * \param   state           The data validation flag.
     **/
    virtual void onStringOnChangeUpdate( const String & StringOnChange, NEService::eDataStateType state ) override;

    /**
     * \brief   Triggered, when IntegerAlways attribute is updated. The function contains
     *          attribute value and validation flag. When notification is enabled,
     *          the method should be overwritten in derived class.
     *          Attributes IntegerAlways description:
     *          This simple integer should send update notification always when value is set.
     * \param   IntegerAlways   The value of IntegerAlways attribute.
     * \param   state           The data validation flag.
     **/
    virtual void onIntegerAlwaysUpdate( unsigned int IntegerAlways, NEService::eDataStateType state ) override;

    /**
     * \brief   Triggered, when ServiceProviderState attribute is updated. The function contains
     *          attribute value and validation flag. When notification is enabled,
     *          the method should be overwritten in derived class.
     *          Attributes ServiceProviderState description:
     *          The state of service provider. The value is always valid, updated on change.
     * \param   ServiceProviderState    The value of ServiceProviderState attribute.
     * \param   state                   The data validation flag.
     **/
    virtual void onServiceProviderStateUpdate( NEPubSub::eServiceState ServiceProviderState, NEService::eDataStateType state ) override;

//////////////////////////////////////////////////////////////////////////
// Hidden methods
//////////////////////////////////////////////////////////////////////////
private:
    inline Subscriber & self(void);

//////////////////////////////////////////////////////////////////////////
// Hidden methods
//////////////////////////////////////////////////////////////////////////
private:

    uint32_t    mOldInteger;    //!< Old integer value to compare when output on console.
    bool        mOldState;      //!< Flag, indicating whether the old integer value is valid or not.
    String      mOldString;     //!< Old string value to compare when output on console.

//////////////////////////////////////////////////////////////////////////
// Forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    Subscriber(void);
    DECLARE_NOCOPY_NOMOVE(Subscriber);
};

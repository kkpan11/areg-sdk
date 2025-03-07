/************************************************************************
 * \file        common/src/Publisher.cpp
 * \ingroup     AREG SDK, Automated Real-time Event Grid Software Development Kit examples
 * \author      Artak Avetyan
 * \brief       Collection of AREG SDK examples.
 *              This file is demonstration of data PubSub (Publish / Subscribe) features.
 ************************************************************************/
 /************************************************************************
  * Include files.
  ************************************************************************/

#include "common/src/Publisher.hpp"

#include "areg/appbase/Application.hpp"
#include "areg/component/Component.hpp"
#include "areg/component/ComponentThread.hpp"
#include "areg/logging/GELog.h"
#include "aregextend/console/Console.hpp"

//////////////////////////////////////////////////////////////////////////
// Log scopes
//////////////////////////////////////////////////////////////////////////

DEF_LOG_SCOPE(examples_23_pubsubmix_common_Publisher_clientConnected);
DEF_LOG_SCOPE(examples_23_pubsubmix_common_Publisher_start);
DEF_LOG_SCOPE(examples_23_pubsubmix_common_Publisher_stop);
DEF_LOG_SCOPE(examples_23_pubsubmix_common_Publisher_invalidate);
DEF_LOG_SCOPE(examples_23_pubsubmix_common_Publisher_quit);
DEF_LOG_SCOPE(examples_23_pubsubmix_common_Publisher_processTimer);


namespace
{
    String generateString(uint32_t seqNr)
    {
        String result;
        return result.format("string_%u", seqNr);
    }
}

//////////////////////////////////////////////////////////////////////////
// Publisher class methods
//////////////////////////////////////////////////////////////////////////

Publisher::Publisher( Component & owner )
    : PubSubMixStub     ( owner )
    , IETimerConsumer   ( )

    , mTimerOnChange    (static_cast<IETimerConsumer &>(self()), owner.getRoleName() + "_OnUpdateTimer")
    , mTimerAlways      (static_cast<IETimerConsumer &>(self()), owner.getRoleName() + "_AlwaysTimer")
    , mClientCount      (0)

    , mSeqString        (0)
    , mCountString      (0)

    , mSeqInteger       (0)
    , mCountInteger     (0)

    , mLock             (false)
{
}

bool Publisher::clientConnected(const ProxyAddress & client, NEService::eServiceConnection status)
{
    LOG_SCOPE(examples_23_pubsubmix_common_Publisher_clientConnected);
    bool result = PubSubMixStub::clientConnected(client, status);

    LOG_DBG("Connection status [ %s ] of the consumer [ %s ]", NEService::getString(status), ProxyAddress::convAddressToPath(client).getString());
    mClientCount += (NEService::isServiceConnected(status) ? 1 : -1);
    LOG_DBG("There are [ %d ] connected service consumers", mClientCount);

    if (isServiceProviderStateValid() == false)
    {
        setServiceProviderState(NEPubSubMix::eServiceState::Uninitialized);
    }

    return result;
}

void Publisher::start(void)
{
    LOG_SCOPE(examples_23_pubsubmix_common_Publisher_start);

    Lock lock(mLock);
    LOG_DBG("Requested to re-start the service run. Reset values and re-start timers, there are [ %d ] connected clients",  mClientCount);

    mTimerAlways.stopTimer();
    mTimerOnChange.stopTimer();

    setServiceProviderState(NEPubSubMix::eServiceState::Running);
    const String & roleName = PubSubMixStub::getServiceRole();

    if (isIntegerAlwaysValid() == false)
    {
        mSeqInteger = 0; 
        setIntegerAlways( { mSeqInteger, roleName } );
    }

    if (isStringOnChangeValid() == false)
    {
        mSeqString = 0;
        setStringOnChange( { generateString(mSeqString), roleName } );
    }

    mTimerAlways.startTimer(NEPubSubMix::TimeoutAlways, getComponentThread(), Timer::CONTINUOUSLY);
    mTimerOnChange.startTimer(NEPubSubMix::TimeoutOnChange, getComponentThread(), Timer::CONTINUOUSLY);
}

void Publisher::stop(void)
{
    LOG_SCOPE(examples_23_pubsubmix_common_Publisher_stop);

    Lock lock(mLock);
    LOG_DBG("Stopped servicing, resets data, wait for further instructions. There are [ %d ] connected clients", mClientCount);

    mTimerAlways.stopTimer();
    mTimerOnChange.stopTimer();

    setServiceProviderState(NEPubSubMix::eServiceState::Stopped);
}

void Publisher::invalidate(void)
{
    LOG_SCOPE(examples_23_pubsubmix_common_Publisher_invalidate);

    Lock lock(mLock);
    LOG_DBG("Invalidating all data. There are [ %d ] connected clients", mClientCount);

    mCountString = 0;
    mCountInteger = 0;
    mSeqString = 0;
    mSeqInteger = 0;

    setServiceProviderState(NEPubSubMix::eServiceState::Uninitialized);

    invalidateIntegerAlways();
    invalidateStringOnChange();
}

void Publisher::quit(void)
{
    LOG_SCOPE(examples_23_pubsubmix_common_Publisher_quit);

    Lock lock(mLock);
    LOG_DBG("Requested to quit.There are[% d] connected clients", mClientCount);

    mTimerAlways.stopTimer();
    mTimerOnChange.stopTimer();

    setServiceProviderState(NEPubSubMix::eServiceState::Shutdown);
    Application::signalAppQuit();
}

void Publisher::processTimer(Timer & timer)
{
    LOG_SCOPE(examples_23_pubsubmix_common_Publisher_processTimer);

    const String roleName = getServiceRole();

    if (&timer == &mTimerAlways)
    {
        Lock lock(mLock);
        if (++ mCountInteger > NEPubSubMix::CycleAlways)
        {
            ++ mSeqInteger;
            mCountInteger = 0;
        }

        LOG_DBG("Timer \'Update Always\' has expired, Integer sequence number is [ %u ], the data should be updated", mSeqInteger);
        setIntegerAlways({ mSeqInteger, roleName });
    }
    else if (&timer == &mTimerOnChange)
    {
        Lock lock(mLock);
        if (++ mCountString > NEPubSubMix::CycleAlways)
        {
            ++ mSeqString;
            mCountString = 0;
        }

        String data(generateString(mSeqString));
        LOG_DBG("Timer \'Update OnChange\' has expired, String is [ %s ], the data should be updated only on update", data.getString());
        setStringOnChange({ data, roleName });
    }
    else
    {
        ASSERT(false);
    }
}

inline Publisher & Publisher::self(void)
{
    return (*this);
}

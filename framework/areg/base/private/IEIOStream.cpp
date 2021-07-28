/************************************************************************
 * \file        areg/base/private/IEIOStream.cpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Input / Output stream interfaces
 *
 ************************************************************************/
#include "areg/base/IEIOStream.hpp"
#include "areg/base/String.hpp"
#include "areg/base/WideString.hpp"
#include "areg/base/NEUtilities.hpp"

//////////////////////////////////////////////////////////////////////////
// IEInStream class implementation: to read data from
//////////////////////////////////////////////////////////////////////////

IEInStream::IEInStream( void )
{
    ; // do nothing
}

IEInStream::~IEInStream( void )
{
    ; // do nothing
}

uint8_t IEInStream::read8Bits(void) const
{
    uint8_t result = 0;
    read( reinterpret_cast<unsigned char *>(&result), 1 );
    return result;
}

uint16_t IEInStream::read16Bits(void) const
{
    uint16_t result = 0;
    read( reinterpret_cast<unsigned char *>(&result), 2 );
    return result;
}

uint32_t IEInStream::read32Bits(void) const
{
    uint32_t result = 0;
    read( reinterpret_cast<unsigned char *>(&result), 4 );
    return result;
}

uint64_t IEInStream::read64Bits(void) const
{
    uint64_t result = 0;
    read( reinterpret_cast<unsigned char *>(&result), 8 );
    return result;
}

//////////////////////////////////////////////////////////////////////////
// IEOutStream class implementation: to write data to
//////////////////////////////////////////////////////////////////////////

IEOutStream::IEOutStream( void )
{
    ; // do nothing
}

IEOutStream::~IEOutStream( void )
{
    ; // do nothing
}

bool IEOutStream::write8Bits( uint8_t value8Bit )
{
    return write( reinterpret_cast<const unsigned char *>(&value8Bit), 1) == 1;
}

bool IEOutStream::write16Bits( uint16_t value16Bit )
{
    return write( reinterpret_cast<const unsigned char *>(&value16Bit), 2) == 2;
}

bool IEOutStream::write32Bits( uint32_t value32Bit )
{
    return write( reinterpret_cast<const unsigned char *>(&value32Bit), 4) == 4;
}

bool IEOutStream::write64Bits( uint64_t value64Bit )
{
    return write( reinterpret_cast<const unsigned char *>(&value64Bit), 8) == 8;
}

//////////////////////////////////////////////////////////////////////////
// IEIOStream class declaration: to read / write data
//////////////////////////////////////////////////////////////////////////

IEIOStream::IEIOStream( void )
    : IEInStream   ( )
    , IEOutStream  ( )
{
    ; // do nothing
}

IEIOStream::~IEIOStream( void )
{
    ; // do nothing
}
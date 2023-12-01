#include "Pch.h"

#include <Kafka/Stream/IInputStream.h>
#include <Kafka/Stream/IOutputStream.h>
#include "Datas/ManData.h"

namespace Fq4Editor
{

ManData::ManData()
    : name( 0 )
    , grow( 0x0A )
    , leadership( 0 )
    , element( ElementNone )
    , fatigue( 0 )
    , exp( 0 )
    , job( 0 )
    , team( 0 )
    , arrowAvoid( 0 )
    , arrowDef( 0 )
    , magicDef( 0 )
    , item( 0 )
    , level( 0 )
    , hpRate( 0 )
    , hitPoint( 0 )
    , attack( 0 )
    , attackRate( 0 )
    , defence( 0 )
    , defenceRate( 0 )
    , roleId( 0 )
{}

ManData::~ManData()
{}

void ManData::WriteToStream( std::shared_ptr<Kafka::Stream::IOutputStream> stream )
{
    if ( !stream->CanWrite() )
    {
        return;
    }

    stream->WriteUint16( name );
    stream->SkipWrite( sizeof( Uint16 ) );
    stream->WriteUint8( grow );
    stream->WriteUint8( leadership );
    stream->WriteUint8( element );
    stream->SkipWrite( sizeof( Uint8 ) );
    stream->WriteUint8( fatigue );
    stream->WriteUint8( exp );
    stream->WriteUint8( job );
    stream->WriteUint8( team );
    stream->SkipWrite( sizeof( Uint16 ) );
    stream->WriteUint8( arrowAvoid );
    stream->WriteUint8( arrowDef );
    stream->WriteUint8( magicDef );
    stream->SkipWrite( sizeof( Uint16 ) );
    stream->WriteUint8( item );
    stream->SkipWrite( sizeof( Uint16 ) );
    stream->WriteUint8( level );
    stream->WriteUint8( hpRate );
    stream->WriteUint16( hitPoint );
    stream->WriteUint8( attack );
    stream->WriteUint8( attackRate );
    stream->WriteUint8( defence );
    stream->WriteUint8( defenceRate );
    stream->WriteUint16( roleId );
}

void ManData::ReadFromStream( std::shared_ptr<Kafka::Stream::IInputStream> stream )
{
    if ( !stream->CanRead() )
    {
        return;
    }

    name = stream->ReadUint16();
    stream->SkipRead( sizeof( Uint16 ) );
    grow = stream->ReadUint8();
    leadership = stream->ReadUint8();
    element = static_cast< Element >( stream->ReadUint8() );
    stream->SkipRead( sizeof( Uint8 ) );
    fatigue = stream->ReadUint8();
    exp = stream->ReadUint8();
    job = stream->ReadUint8();
    team = stream->ReadUint8();
    stream->SkipRead( sizeof( Uint16 ) );
    arrowAvoid = stream->ReadUint8();
    arrowDef = stream->ReadUint8();
    magicDef = stream->ReadUint8();
    stream->SkipRead( sizeof( Uint16 ) );
    item = stream->ReadUint8();
    stream->SkipRead( sizeof( Uint16 ) );
    level = stream->ReadUint8();
    hpRate = stream->ReadUint8();
    hitPoint = stream->ReadUint16();
    attack = stream->ReadUint8();
    attackRate = stream->ReadUint8();
    defence = stream->ReadUint8();
    defenceRate = stream->ReadUint8();
    roleId = stream->ReadUint16();
}

} // namespace Fq4Editor

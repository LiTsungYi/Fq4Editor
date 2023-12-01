#pragma once

#include <Kafka/Basic/Types.h>
#include <Kafka/Stream/ISerializable.h>

namespace Kafka
{

struct IInputStream;
struct IOutputStream;

} // namespace Kafka

namespace Fq4Editor
{

// First Address @ 0x00002994
// 0x00002990h XX XX XX XX a1 a1 00 00 a2 a3 a4 00 a5 a6 a7 a8
// 0x000029a0h 00 00 b1 b2 b3 00 00 b4 00 00 c1 c2 c3 c3 d1 d2
// 0x000028b0h d3 d4 d5 d5 XX XX XX XX XX XX XX XX XX XX XX XX
//
// a1: 名字
// a2: 成長
// a3: 領導 (00:不可 其他數值皆可, 值越大表示帶兵能力越強)
// a4: 屬性 有些職業已經有基本屬性在,故要選擇該人物所缺少的屬性
//     0x10 = f, 0x20 = e, 0x40 = a, 0x80 = w
// a5: 疲勞 (值越大 作戰能力越低)
// a6: 經驗
// a7: 職業 (會關係到GE和該職業基本屬性)
// a8: 陣營 (0=未加入, 10=主角)
// b1: AA對箭回避力 最大為 0-99
// b2: AD對箭防禦力 最大為 0-30 (為負值)
// b3: MD對魔法防禦力 最大為 0-30 (為負值)
// b4: 道具 0-48 (00:無)
// c1: 等級
// c2: HR生命回復速度
// c3: HP生命力
// d1: AT攻擊力( 騎馬兵請先將遊戲中數值 - 15再來尋找 )
// d2: AR命中率
// d3: DF防禦力( 騎馬兵請先將遊戲中數值 - 20再來尋找 )
// d4: DR迴避率
// d5: 角色ID


enum Element
{
    ElementNone = 0x00,
    ElementFire = 0x10,
    ElementEarth = 0x20,
    ElementAir = 0x40,
    ElementWater = 0x80,
};


class ManData : Kafka::Stream::ISerializable
{
public:
    ManData();
    ~ManData();

public:
    virtual void WriteToStream( std::shared_ptr<Kafka::Stream::IOutputStream> stream ) override;
    virtual void ReadFromStream( std::shared_ptr<Kafka::Stream::IInputStream> stream ) override;

public:
    Uint16 name;
    Uint8 grow;
    Uint8 leadership;
    Element element;
    Uint8 fatigue;
    Uint8 exp;
    Uint8 job;
    Uint8 team;
    Uint8 arrowAvoid;
    Uint8 arrowDef;
    Uint8 magicDef;
    Uint8 item;
    Uint8 level;
    Uint8 hpRate;
    Uint16 hitPoint;
    Uint8 attack;
    Uint8 attackRate;
    Uint8 defence;
    Uint8 defenceRate;
    Uint16 roleId;

};

} // namespace Fq4Editor

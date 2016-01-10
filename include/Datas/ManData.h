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
// a1: �W�r
// a2: ����
// a3: ��� (00:���i ��L�ƭȬҥi, �ȶV�j��ܱa�L��O�V�j)
// a4: �ݩ� ����¾�~�w�g�����ݩʦb,�G�n��ܸӤH���үʤ֪��ݩ�
//     0x10 = f, 0x20 = e, 0x40 = a, 0x80 = w
// a5: �h�� (�ȶV�j �@�ԯ�O�V�C)
// a6: �g��
// a7: ¾�~ (�|���Y��GE�M��¾�~���ݩ�)
// a8: �}�� (0=���[�J, 10=�D��)
// b1: AA��b�^�פO �̤j�� 0-99
// b2: AD��b���m�O �̤j�� 0-30 (���t��)
// b3: MD���]�k���m�O �̤j�� 0-30 (���t��)
// b4: �D�� 0-48 (00:�L)
// c1: ����
// c2: HR�ͩR�^�_�t��
// c3: HP�ͩR�O
// d1: AT�����O( �M���L�Х��N�C�����ƭ� - 15�A�ӴM�� )
// d2: AR�R���v
// d3: DF���m�O( �M���L�Х��N�C�����ƭ� - 20�A�ӴM�� )
// d4: DR�j�ײv
// d5: ����ID


enum Element
{
    ElementNone = 0x00,
    ElementFire = 0x10,
    ElementEarth = 0x20,
    ElementAir = 0x40,
    ElementWater = 0x80,
};


class ManData : Kafka::ISerializable
{
public:
    ManData();
    ~ManData();

public:
    virtual void WriteToStream( std::shared_ptr<Kafka::IOutputStream> stream ) override;
    virtual void ReadFromStream( std::shared_ptr<Kafka::IInputStream> stream ) override;

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

#include "Pch.h"

#include <sstream>
#include <WinBase.h>
#include <Kafka/Stream/OutputFileStream.h>
#include <Kafka/Stream/InoutFileStream.h>
#include "../resource.h"
#include "Window/Fq4EditorWindow.h"
#include "Datas/Job.h"

namespace Fq4Editor
{

Fq4EditorWindow::Fq4EditorWindow( HINSTANCE hInstance )
    : EditorWindow( hInstance )
{
    Init();
}

Fq4EditorWindow::~Fq4EditorWindow()
{
}

void Fq4EditorWindow::Init()
{
    std::shared_ptr<Kafka::Stream::InputFileStream> fileStream;
    fileStream.reset( new Kafka::Stream::InputFileStream() );
    fileStream->OpenFile( "FQ4GD.X", true );

    if ( fileStream == nullptr || !fileStream->IsOpened() )
    {
        return;
    }

    fileStream->SeekRead( TeamOffset );
    for ( ;; )
    {
        ManData data;
        data.ReadFromStream( fileStream );

        if ( data.name == 0 )
        {
            break;
        }

        switch ( data.job )
        {
        case 0x3F: // 國王 (森林國 雷斯達)
        case 0x6C: // 精靈王 (精靈國 薩內多)
        case 0x4D: // 國王 (礦山國 格林斯)
        case 0x74: // 國王 (海盜國 慕尼頓)
            m_kingDatas.push_back( data );
            break;

        case 0x37: // 紅戰士
        case 0x40: // 學者
        case 0x41: // 製圖家
        case 0x42: // 勇士
        case 0x43: // 紅騎士
        case 0x44: // Ｒ士兵
        case 0x45: // Ｒ弓兵
            m_forestDatas.push_back( data );
            break;

        case 0x6D: // 精靈
        case 0x6E: // 精靈
        case 0x6F: // 大精靈
        case 0x70: // 靈魔女
            m_elfDatas.push_back( data );
            break;

        case 0x11: // 學者
        case 0x14: // 尼僧
        case 0x36: // 隊員
        case 0x4F: // 學者
        case 0x51: // 綠騎士
        case 0x52: // Ｇ士兵
        case 0x53: // Ｇ戰士
            m_mineDatas.push_back( data );
            break;

        case 0x4E: // 皇后 (弗里森 2A 02, 雪爾維亞 19 02)
            if ( data.name == 0x22A || data.name == 0x219 )
            {
                m_mineDatas.push_back( data );
            }
            break;

        case 0x75: // 海盜
        case 0x76: // 大海盜
        case 0x77: // 公爵
        case 0x78: // 克學士
        case 0x79: // 槍學士
        case 0x7A: // 守衛
            m_seaDatas.push_back( data );
            break;

        case 0x7F: // 土精靈 (米里拉木 64 02)
            if ( data.name == 0x264 )
            {
                m_seaDatas.push_back( data );
            }
            break;

        case 0x46: // 王子 (雷克斯)
        case 0x49: // 白戰士
        case 0x4B: // 槍兵
            m_plainMagicianDatas.push_back( data );
            break;

        case 0x47: // 王子 (傑蘭)
        case 0x4C: // 誘導槍
        case 0xC2: // 騎士 (平原國)
            m_plainKnightDatas.push_back( data );
            break;
        }

    }
}

LRESULT CALLBACK Fq4EditorWindow::OnCommand( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    int wmId = LOWORD( wParam );
    switch ( wmId )
    {
    case IDM_KINGS:
        {
            int count = GiveTeam( m_kingDatas );
            std::wstring&& message = LogTeamResult( count, m_kingDatas.size() );
            MessageBox( hWnd, message.c_str(), L"國王們", MB_OK );
        }
        break;

    case IDM_FOREST:
        {
            int count = GiveTeam( m_forestDatas );
            std::wstring&& message = LogTeamResult( count, m_forestDatas.size() );
            MessageBox( hWnd, message.c_str(), L"森林國 雷斯達", MB_OK );
        }
        break;

    case IDM_ELF:
        {
            int count = GiveTeam( m_elfDatas );
            std::wstring&& message = LogTeamResult( count, m_elfDatas.size() );
            MessageBox( hWnd, message.c_str(), L"精靈國 薩內多", MB_OK );
        }
        break;

    case IDM_MINE:
        {
            int count = GiveTeam( m_mineDatas );
            std::wstring&& message = LogTeamResult( count, m_mineDatas.size() );
            MessageBox( hWnd, message.c_str(), L"礦山國 格林斯", MB_OK );
        }
        break;

    case IDM_SEA:
        {
            int count = GiveTeam( m_seaDatas );
            std::wstring&& message = LogTeamResult( count, m_seaDatas.size() );
            MessageBox( hWnd, message.c_str(), L"海盜國 慕尼頓", MB_OK );
        }
        break;

    case IDM_PLAIN_MAGICIAN:
        {
           int count = GiveTeam( m_plainMagicianDatas );
           std::wstring&& message = LogTeamResult( count, m_plainMagicianDatas.size() );
           MessageBox( hWnd, message.c_str(), L" 平原國 休利亞(魔法軍團)", MB_OK );
        }
        break;

    case IDM_PLAIN_KNIGHT:
        {
            int count = GiveTeam( m_plainKnightDatas );
            std::wstring&& message = LogTeamResult( count, m_plainKnightDatas.size() );
            MessageBox( hWnd, message.c_str(), L"平原國 休利亞(騎士軍團)", MB_OK );
        }
        break;

    case IDM_ITEM:
        {
            GiveItem();
            MessageBox( hWnd, L"所有道具 +10", L"ITEM", MB_OK );
        }
        break;

    case IDM_MONEY:
        {
            GiveMoney();
            MessageBox( hWnd, L"獲得 $10000", L"MONEY", MB_OK );
        }
        break;

    case IDM_TIRED:
        {
            ReleaseFatigue();
        }
    break;

    default:
        return EditorWindow::OnCommand( hWnd, message, wParam, lParam );
    }

    return 0;
}

int Fq4EditorWindow::GiveTeam( std::vector<ManData>& datas )
{
    int total = 0;
    size_t count = 0;
    for ( size_t i = 0; i < m_datas.size(); ++i )
    {
        ManData& data = m_datas[ i ];
        // 0x39=Ｂ士兵, 0x3A=Ｂ戰士, 0x3B=Ｂ弓兵, 0x54=士兵, 0x55=戰士, 0x57=狙擊手, 0x58=弓箭手
        if ( ( data.job == 0x39 || data.job == 0x3A || data.job == 0x3B ||
            data.job == 0x54 || data.job == 0x55 || data.job == 0x57 || data.job == 0x58 ) &&
            data.leadership == 0 && data.team == 0x10 )
        {
            ManData& prevData = datas[ count++ ];
            if ( !CanGiveThisMember( prevData.roleId ) )
            {
                break;
            }

            data.name = prevData.name;
            data.grow = prevData.grow;
            data.leadership = prevData.leadership;
            data.element = prevData.element;
            data.job = prevData.job;
            data.arrowAvoid = prevData.arrowAvoid;
            data.arrowDef = prevData.arrowDef;
            data.magicDef = prevData.magicDef;
            data.item = prevData.item;
            data.level = prevData.level;
            data.hpRate = prevData.hpRate;
            data.hitPoint = prevData.hitPoint;
            data.attack = prevData.attack;
            data.attackRate = prevData.attackRate;
            data.defence = prevData.defence;
            data.defenceRate = prevData.defenceRate;
            data.roleId = prevData.roleId;

            ++total;
            m_teams.insert( data.roleId );
            if ( count >= datas.size() )
            {
                return total;
            }
        }
    }

    return 0;
}

bool Fq4EditorWindow::GiveItem()
{
    if ( m_fileStream == nullptr || !m_fileStream->IsOpened() )
    {
        return false;
    }

    std::vector<Uint8> items;
    m_fileStream->SeekRead( ItemOffset );
    for ( size_t count = 0; count <= MaxItem; ++count )
    {
        Uint8 value = m_fileStream->ReadUint8();
        value += StepItem;
        if ( value > MaxItemNumber )
        {
            value = MaxItemNumber;
        }
        items.push_back( value );
    }

    m_fileStream->SeekWrite( ItemOffset );
    for ( size_t count = 0; count <= MaxItem; ++count )
    {
        Uint8 value = items[ count ];
        m_fileStream->WriteUint8( value );
    }

    return true;
}

bool Fq4EditorWindow::GiveMoney()
{
    if ( m_fileStream == nullptr || !m_fileStream->IsOpened() )
    {
        return false;
    }

    m_fileStream->SeekRead( MoneyOffset );
    Uint16 value = m_fileStream->ReadUint16();
    value += StepMoney;
    if ( value > MaxMoney )
    {
        value = MaxMoney;
    }

    m_fileStream->SeekWrite( MoneyOffset );
    m_fileStream->WriteUint16( value );

    return true;
}

void Fq4EditorWindow::ReleaseFatigue()
{
    for ( size_t i = 0; i < m_datas.size(); ++i )
    {
        ManData& data = m_datas[ i ];
        if ( data.team == 0x10 )
        {
            data.fatigue = 0;
            data.exp = 99;
        }
    }

}

void Fq4EditorWindow::OnFileOpened()
{
    if ( m_fileStream == nullptr || !m_fileStream->IsOpened() )
    {
        return;
    }

    m_datas.clear();
    m_alives.clear();
    m_teams.clear();
    m_fileStream->SeekRead( TeamOffset );
    for ( ;; )
    {
        ManData data;
        data.ReadFromStream( m_fileStream );

        if ( data.name == 0 )
        {
            break;
        }

        m_datas.push_back( data );
        m_alives.insert( data.roleId );
        if ( data.name )
        {
            m_teams.insert( data.roleId );
        }
    }
}

void Fq4EditorWindow::OnFileSaved()
{
    if ( m_fileStream == nullptr || !m_fileStream->IsOpened() )
    {
        return;
    }

    m_fileStream->SeekWrite( TeamOffset );
    for ( size_t i = 0; i < m_datas.size(); ++i )
    {
        ManData& data = m_datas[ i ];
        data.WriteToStream( m_fileStream );
    }
}

bool Fq4EditorWindow::CanGiveThisMember( Uint16 roleId )
{
    if ( m_alives.find( roleId ) != m_alives.end() )
    {
        return false;
    }

    if ( m_teams.find( roleId ) != m_teams.end() )
    {
        return false;
    }

    return true;
}

std::wstring Fq4EditorWindow::LogTeamResult( int count, int total ) const
{
    std::wstringstream stream;
    stream.setf( std::ios::dec );
    stream << L"本次更新角色數: " << count << L" / " << total;
    return stream.str();
}

void Fq4EditorWindow::Log( std::wstringstream& stringStream, const ManData& data ) const
{
    stringStream << data.roleId << L",";
    stringStream << data.name << L",";
    stringStream << data.grow << L",";
    stringStream << data.leadership << L",";
    stringStream << data.element << L",";
    stringStream << RoleList[ data.job ] << L",";
    stringStream << data.arrowAvoid << L",";
    stringStream << data.arrowDef << L",";
    stringStream << data.magicDef << L",";
    stringStream << data.item << L",";
    stringStream << data.level << L",";
    stringStream << data.hpRate << L",";
    stringStream << data.hitPoint << L",";
    stringStream << data.attack << L",";
    stringStream << data.attackRate << L",";
    stringStream << data.defence << L",";
    stringStream << data.defenceRate << L",";
    stringStream << data.team << std::endl;
}

} // namespace Fq4Editor

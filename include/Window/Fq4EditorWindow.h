#pragma once

#include <vector>
#include <set>
#include "Datas/ManData.h"
#include "Window/EditorWindow.h"

namespace Fq4Editor
{

class Fq4EditorWindow : public EditorWindow
{
private:
    const int MoneyOffset = 0x000047E;
    const int ItemOffset = 0x00000380;
    const int TeamOffset = 0x00002994;
    const Uint8 MaxItem = 72;
    const Uint8 StepItem = 10;
    const Uint8 MaxItemNumber = 99;
    const Uint16 StepMoney = 1000;
    const Uint16 MaxMoney = 20000;

public:
    Fq4EditorWindow( HINSTANCE hInstance );
    ~Fq4EditorWindow();

private:
    void Init();
    virtual LRESULT CALLBACK OnCommand( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) override;
    virtual void OnFileOpened() override;
    virtual void OnFileSaved() override;

    int GiveTeam( std::vector<ManData>& datas );
    bool GiveItem();
    bool GiveMoney();
    void ReleaseFatigue();

    bool CanGiveThisMember( Uint16 roleId );
    std::wstring LogTeamResult( int count, int total ) const;
    void Log( std::wstringstream& stringStream, const ManData& data ) const;

private:
    std::vector<ManData> m_kingDatas;
    std::vector<ManData> m_forestDatas;
    std::vector<ManData> m_elfDatas;
    std::vector<ManData> m_mineDatas;
    std::vector<ManData> m_seaDatas;
    std::vector<ManData> m_plainMagicianDatas;
    std::vector<ManData> m_plainKnightDatas;
    std::vector<ManData> m_datas;
    std::set<Uint16> m_alives;
    std::set<Uint16> m_teams;
};

} // namespace Fq4Editor

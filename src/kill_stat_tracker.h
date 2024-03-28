#include "Configuration/Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

using std::stringstream;
using std::ifstream;
using std::ofstream;

class KillStatTracker : public PlayerScript
{
public:
    stringstream fullStream;
    bool loggingEnabled = sConfigMgr->GetOption<bool>("KillDetailedLogging.enabled", true);
    int logDumpSize = sConfigMgr->GetOption<int>("KillDetailedLogging.dumpSize", 0);

    KillStatTracker() : PlayerScript("KillStatTracker")
    {
        // If the file doesn't exist we will create it and add the appropriate headers

        ifstream ifile("kills.log");
        if (!ifile)
        {
            fullStream << "timestamp, player, faction, level, maxhealth, currenthealth, creature, creaturefaction, creaturemaxhealth, zoneid, areaid, isgamemaster\n";
            StringDump();
        }

    }

    void OnCreatureKill(Player* player, Creature* creature) override;
    void OnCreatureKilledByPet(Player* player, Creature* creature) override;

private:
    int insertCount = 0;

    void StringDump()
    {
        insertCount++;

        if (insertCount > logDumpSize)
        {
            // Dump to log
            ofstream logFile;
            logFile.open("kills.log", ofstream::app);
            logFile << fullStream.rdbuf();
            logFile.close();

            // Reset the buffer
            fullStream.str("");
            fullStream.clear();

            // Reset line count
            insertCount = 0;
        }
    }
};

void AddKillStatTrackerScripts()
{
    new KillStatTracker();
}

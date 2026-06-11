#include "save_io.h"

/* ---- offset tables for 7 save versions ---- */
static int HEADER[VERSIONS]     = {0x24e2,0x24ee,0x2588,0x29c0,0x3ef8,0x471a,0x471b};
static int RUPEE[VERSIONS]      = {0x00e0a0,0x00e110,0x00e110,0x00e678,0x00e730,0x00eaf8,0x00eaf8};
static int ITEMS[VERSIONS]      = {0x052828,0x0528d8,0x0528c0,0x053890,0x05fa48,0x060408,0x060408};
static int QUANT[VERSIONS]      = {0x063340,0x0633f0,0x0633d8,0x064550,0x070730,0x0711c8,0x0711c8};
static int F_WPN[VERSIONS]      = {0x050328,0x0503d8,0x0503c0,0x051270,0x05d420,0x05dd20,0x05dd20};
static int FV_WPN[VERSIONS]     = {0x0a9ca8,0x0a9d78,0x0a9d58,0x0ab8d0,0x0c3bd8,0x0c4c68,0x0c4c68};
static int F_BOW[VERSIONS]      = {0x0045f0,0x0045f8,0x0045f8,0x0047e8,0x004828,0x004990,0x004990};
static int FV_BOW[VERSIONS]     = {0x00a8e0,0x00a940,0x00a940,0x00ae08,0x00ae90,0x00b1e0,0x00b1e0};
static int F_SHLD[VERSIONS]     = {0x0b5810,0x0b58e8,0x0b58c8,0x0b7910,0x0cfc70,0x0d1038,0x0d1038};
static int FV_SHLD[VERSIONS]    = {0x063218,0x0632c8,0x0632b0,0x064420,0x070600,0x071098,0x071098};

/* ---- globals ---- */
int g_version;
FILE *g_fp;

char g_itemName[MAX_ITEMS][30];
int  g_quant[MAX_ITEMS];
int  g_newQuant[MAX_ITEMS];
int  g_mod[MAX_ITEMS];
int  g_newMod[MAX_ITEMS];
int  g_itemCount;
int  g_weaponCount;
int  g_bowCount;
int  g_shieldCount;

int  g_rupees;
int  g_newRupees;

char g_quests[MAX_QUESTS][64];
int  g_questFlag[MAX_QUESTS];
int  g_newQuestFlag[MAX_QUESTS];
int  g_questCount;

/* ---- translation tables ---- */
static const char *RAW[200] = {
    "Weapon_Sword_001","Weapon_Sword_002","Weapon_Sword_003","Weapon_Sword_004",
    "Weapon_Sword_005","Weapon_Sword_006","Weapon_Sword_007","Weapon_Sword_008",
    "Weapon_Sword_009","Weapon_Sword_013","Weapon_Sword_014","Weapon_Sword_015",
    "Weapon_Sword_016","Weapon_Sword_017","Weapon_Sword_018","Weapon_Sword_019",
    "Weapon_Sword_020","Weapon_Sword_021","Weapon_Sword_022","Weapon_Sword_023",
    "Weapon_Sword_024","Weapon_Sword_025","Weapon_Sword_027","Weapon_Sword_029",
    "Weapon_Sword_030","Weapon_Sword_031","Weapon_Sword_033","Weapon_Sword_034",
    "Weapon_Sword_035","Weapon_Sword_040","Weapon_Sword_041","Weapon_Sword_043",
    "Weapon_Sword_044","Weapon_Sword_047","Weapon_Sword_048","Weapon_Sword_049",
    "Weapon_Sword_050","Weapon_Sword_051","Weapon_Sword_052","Weapon_Sword_053",
    "Weapon_Sword_056","Weapon_Sword_057","Weapon_Sword_058","Weapon_Sword_059",
    "Weapon_Sword_060","Weapon_Sword_061","Weapon_Sword_062","Weapon_Sword_070",
    "Weapon_Sword_071","Weapon_Sword_072","Weapon_Sword_500","Weapon_Sword_502",
    "Weapon_Sword_503","Weapon_Lsword_001","Weapon_Lsword_002","Weapon_Lsword_003",
    "Weapon_Lsword_004","Weapon_Lsword_005","Weapon_Lsword_006","Weapon_Lsword_010",
    "Weapon_Lsword_011","Weapon_Lsword_012","Weapon_Lsword_013","Weapon_Lsword_014",
    "Weapon_Lsword_015","Weapon_Lsword_016","Weapon_Lsword_017","Weapon_Lsword_018",
    "Weapon_Lsword_019","Weapon_Lsword_020","Weapon_Lsword_023","Weapon_Lsword_024",
    "Weapon_Lsword_027","Weapon_Lsword_029","Weapon_Lsword_030","Weapon_Lsword_031",
    "Weapon_Lsword_032","Weapon_Lsword_033","Weapon_Lsword_034","Weapon_Lsword_035",
    "Weapon_Lsword_036","Weapon_Lsword_037","Weapon_Lsword_038","Weapon_Lsword_041",
    "Weapon_Lsword_045","Weapon_Lsword_047","Weapon_Lsword_051","Weapon_Lsword_054",
    "Weapon_Lsword_055","Weapon_Lsword_056","Weapon_Lsword_057","Weapon_Lsword_059",
    "Weapon_Lsword_060","Weapon_Lsword_074","Weapon_Spear_001","Weapon_Spear_002",
    "Weapon_Spear_003","Weapon_Spear_004","Weapon_Spear_005","Weapon_Spear_006",
    "Weapon_Spear_007","Weapon_Spear_008","Weapon_Spear_009","Weapon_Spear_010",
    "Weapon_Spear_011","Weapon_Spear_012","Weapon_Spear_013","Weapon_Spear_014",
    "Weapon_Spear_015","Weapon_Spear_016","Weapon_Spear_017","Weapon_Spear_018",
    "Weapon_Spear_021","Weapon_Spear_022","Weapon_Spear_023","Weapon_Spear_024",
    "Weapon_Spear_025","Weapon_Spear_027","Weapon_Spear_028","Weapon_Spear_029",
    "Weapon_Spear_030","Weapon_Spear_031","Weapon_Spear_032","Weapon_Spear_033",
    "Weapon_Spear_034","Weapon_Spear_035","Weapon_Spear_036","Weapon_Spear_037",
    "Weapon_Spear_038","Weapon_Spear_047","Weapon_Spear_049","Weapon_Spear_050",
    "Weapon_Bow_001","Weapon_Bow_002","Weapon_Bow_003","Weapon_Bow_004",
    "Weapon_Bow_006","Weapon_Bow_009","Weapon_Bow_011","Weapon_Bow_013",
    "Weapon_Bow_014","Weapon_Bow_015","Weapon_Bow_016","Weapon_Bow_017",
    "Weapon_Bow_023","Weapon_Bow_026","Weapon_Bow_027","Weapon_Bow_028",
    "Weapon_Bow_029","Weapon_Bow_030","Weapon_Bow_032","Weapon_Bow_033",
    "Weapon_Bow_035","Weapon_Bow_036","Weapon_Bow_038","Weapon_Bow_040",
    "Weapon_Bow_071","Weapon_Bow_072","Weapon_Shield_001","Weapon_Shield_002",
    "Weapon_Shield_003","Weapon_Shield_004","Weapon_Shield_005","Weapon_Shield_006",
    "Weapon_Shield_007","Weapon_Shield_008","Weapon_Shield_009","Weapon_Shield_013",
    "Weapon_Shield_014","Weapon_Shield_015","Weapon_Shield_016","Weapon_Shield_017",
    "Weapon_Shield_018","Weapon_Shield_021","Weapon_Shield_022","Weapon_Shield_023",
    "Weapon_Shield_025","Weapon_Shield_026","Weapon_Shield_030","Weapon_Shield_031",
    "Weapon_Shield_032","Weapon_Shield_033","Weapon_Shield_034","Weapon_Shield_035",
    "Weapon_Shield_036","Weapon_Shield_037","Weapon_Shield_038","Weapon_Shield_040",
    "Weapon_Shield_041","Weapon_Shield_042","Weapon_Shield_057","NormalArrow",
    "FireArrow","IceArrow","ElectricArrow","BombArrow_A","AncientArrow"
};
static const char *DISP[200] = {
    "Traveler's Sword","Soldier's Broadsword","Knight's Broadsword","Boko Club",
    "Spiked Boko Club","Dragonbone Boko Club","Lizal Boomerang","Lizal Forked Boomerang",
    "Lizal Tri-Boomerang","Guardian Sword","Guardian Sword+","Guardian Sword++",
    "Lynel Sword","Mighty Lynel Sword","Savage Lynel Sword","Bokoblin Arm",
    "Lizalfos Arm","Rusty Broadsword","Soup Ladle","Ancient Short Sword",
    "Royal Broadsword","Forest Dweller's Sword","Zora Sword","Gerudo Scimitar",
    "Moonlight Scimitar","Feathered Edge","Flameblade","Frostblade",
    "Thunderblade","Spring-Loaded Hammer","Eightfold Blade","Torch",
    "Tree Branch","Royal Guard's Sword","Meteor Rod","Blizzard Rod",
    "Thunderstorm Rod","Boomerang","Scimitar of the Seven","Vicious Sickle",
    "Master Sword (Broken)","Goddess Sword","Hero's Sword","Sea-Breeze Boomerang",
    "Fire Rod","Ice Rod","Lightning Rod","Master Sword",
    "M. Sword (no malice)","M. Sword (malice)","Demon Carver","Lantern",
    "OH Obliterator","Traveler's Claymore","Soldier's Claymore",
    "Knight's Claymore","Boko Bat","Spiked Boko Bat","Dragonbone Boko Bat",
    "Moblin Club","Spiked Moblin Club","Dragonbone Moblin Club","Ancient Battle Axe",
    "Ancient Battle Axe+","Ancient Battle Axe++","Lynel Crusher","Mighty Lynel Crusher",
    "Savage Lynel Crusher","Moblin Arm","Rusty Claymore","Ancient Bladesaw",
    "Royal Claymore","Silver Longsword","Golden Claymore","Double Axe",
    "Iron Sledgehammer","Woodcutter's Axe","Great Flameblade","Great Frostblade",
    "Great Thunderblade","Cobble Crusher","Stone Smasher","Boat Oar",
    "Eightfold Longblade","Farming Hoe","Royal Guard's Claymore","Giant Boomerang",
    "Boulder Breaker","Edge of Duality","Korok Leaf","Sword of the Six Sages",
    "Biggoron's Sword","Fierce Deity Sword","Windcleaver","Traveler's Spear",
    "Soldier's Spear","Knight's Halberd","Boko Spear","Spiked Boko Spear",
    "Dragonbone Boko Spear","Lizal Spear","Enhanced Lizal Spear","Forked Lizal Spear",
    "Moblin Spear","Spiked Moblin Spear","Dragonbone Moblin Spear","Guardian Spear",
    "Guardian Spear+","Guardian Spear++","Lynel Spear","Mighty Lynel Spear",
    "Savage Lynel Spear","Rusty Halberd","Farmer's Pitchfork","Ancient Spear",
    "Royal Halberd","Forest Dweller's Spear","Zora Spear","Silverscale Spear",
    "Gerudo Spear","Throwing Spear","Drillshaft","Feathered Spear",
    "Flamespear","Frostspear","Thunderspear","Wooden Mop",
    "Serpentine Spear","Fishing Harpoon","Royal Guard's Spear","Ceremonial Trident",
    "Lightscale Trident","Traveler's Bow","Soldier's Bow","Spiked Boko Bow",
    "Boko Bow","Lizal Bow","Lynel Bow","Strengthened Lizal Bow",
    "Forest Dweller's Bow","Silver Bow","Golden Bow","Swallow Bow",
    "Falcon Bow","Ancient Bow","Mighty Lynel Bow","Dragonbone Boko Bow",
    "Great Eagle Bow","Phrenic Bow","Steel Lizal Bow","Savage Lynel Bow",
    "Royal Guard's Bow","Knight's Bow","Royal Bow","Wooden Bow",
    "Duplex Bow","Bow of Light","Twilight Bow","Wooden Shield",
    "Soldier's Shield","Knight's Shield","Boko Shield","Spiked Boko Shield",
    "Dragonbone Boko Shield","Lizal Shield","Reinforced Lizal Shield","Steel Lizal Shield",
    "Guardian Shield","Guardian Shield+","Guardian Shield++","Lynel Shield",
    "Mighty Lynel Shield","Savage Lynel Shield","Rusty Shield","Royal Shield",
    "Forest Dweller's Shield","Silver Shield","Gerudo Shield","Hylian Shield",
    "Hunter's Shield","Fisherman's Shield","Royal Guard's Shield","Emblazoned Shield",
    "Traveler's Shield","Radiant Shield","Daybreaker","Ancient Shield",
    "Pot Lid","Shield of the Mind's Eye","Kite Shield","Hero's Shield",
    "Arrow","Fire Arrow","Ice Arrow","Shock Arrow",
    "Bomb Arrow","Ancient Arrow"
};

static const char *QUEST_NAMES[MAX_QUESTS] = {
    "Follow the Sheikah Slate",
    "Seek Out Impa",
    "Locked Mementos",
    "The Isolated Plateau",
    "Vah Ruta of Zora's Domain",
    "Vah Rudania of Death Mountain",
    "Vah Medoh of Tabantha",
    "Vah Naboris of Gerudo Desert",
    "Free the Divine Beasts",
    "Destroy Ganon",
    "The Hero's Sword",
    "Captured Memories",
    "The Crown Beast",
    "The Champion's Ballad",
    "EX The Legend of the Hero",
    "EX Strange Masquerade",
    "EX Treasure: Ancient Mask",
    "EX Treasure: Fairy Clothes",
    "EX Treasure: Phantasma",
    "EX Treasure: Royal Guard",
    "EX Treasure: Traveler",
    "EX Treasure: Twilight",
    "EX Treasure: Hero of Time",
    "EX Treasure: Wind Waker",
    "EX Treasure: Dark Armor",
    "EX Treasure: Hero of Twilight",
    "EX Treasure: Tingle's Outfit",
    "EX Treasure: Midna's Helmet",
    "EX Treasure: Majora's Mask",
    "EX Treasure: Phantom Armor",
    "EX Treasure: Salvage Vest",
    "EX Treasure: Island Lobster",
    "EX Treasure: Nintendo Labo",
    "EX Treasure: Xenoblade 2",
};

/* ---- save file operations ---- */

static int read_items(int version) {
    int chk[5];
    g_itemCount = 0;
    g_weaponCount = 0;
    g_bowCount = 0;
    g_shieldCount = 0;

    for (int y = 0; y < 50; y++) {
        int off = y * 128;
        g_itemName[y][0] = 0;

        for (int x = 0; x < 5; x++) {
            fseek(g_fp, ITEMS[version] + 8 * x + off, SEEK_SET);
            fread(&chk[x], sizeof(int), 1, g_fp);
            char buf[5];
            memcpy(buf, &chk[x], 4);
            buf[4] = 0;
            int len = strlen(buf);
            if (len > 0 && buf[len-1] == 2) buf[len-1] = 0;
            strcat(g_itemName[y], buf);

            if (strcmp(buf,"swor")==0||strcmp(buf,"word")==0||strcmp(buf,"pear")==0) g_weaponCount++;
            if (strcmp(buf,"ow_0")==0) g_bowCount++;
            if (strcmp(buf,"hiel")==0) g_shieldCount++;
        }

        if (strcmp(g_itemName[y],"Armo")==0 || strlen(g_itemName[y])==0)
            break;

        fseek(g_fp, QUANT[version] + 8 * y, SEEK_SET);
        fread(&g_quant[y], sizeof(int), 1, g_fp);
        g_newQuant[y] = g_quant[y];
        g_itemCount++;
    }

    int wpn_off = g_weaponCount;
    for (int x = 0; x < wpn_off; x++) {
        fseek(g_fp, F_WPN[version] + 8 * x, SEEK_SET);
        fread(&g_mod[x], sizeof(int), 1, g_fp);
        fseek(g_fp, FV_WPN[version] + 8 * x, SEEK_SET);
        fread(&g_mod[x + MAX_ITEMS/2], sizeof(int), 1, g_fp);
        g_newMod[x] = g_mod[x];
        g_newMod[x + MAX_ITEMS/2] = g_mod[x + MAX_ITEMS/2];
    }
    for (int x = 0; x < g_bowCount; x++) {
        int idx = x + wpn_off;
        fseek(g_fp, F_BOW[version] + 8 * x, SEEK_SET);
        fread(&g_mod[idx], sizeof(int), 1, g_fp);
        fseek(g_fp, FV_BOW[version] + 8 * x, SEEK_SET);
        fread(&g_mod[idx + MAX_ITEMS/2], sizeof(int), 1, g_fp);
        g_newMod[idx] = g_mod[idx];
        g_newMod[idx + MAX_ITEMS/2] = g_mod[idx + MAX_ITEMS/2];
    }
    int wpnbow = wpn_off + g_bowCount;
    for (int x = 0; x < g_shieldCount; x++) {
        int idx = x + wpnbow + 6;
        fseek(g_fp, F_SHLD[version] + 8 * x, SEEK_SET);
        fread(&g_mod[idx], sizeof(int), 1, g_fp);
        fseek(g_fp, FV_SHLD[version] + 8 * x, SEEK_SET);
        fread(&g_mod[idx + MAX_ITEMS/2], sizeof(int), 1, g_fp);
        g_newMod[idx] = g_mod[idx];
        g_newMod[idx + MAX_ITEMS/2] = g_mod[idx + MAX_ITEMS/2];
    }
    return 1;
}

int load_save(int slot) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        g_itemName[i][0] = 0;
        g_quant[i] = 0; g_newQuant[i] = 0;
        g_mod[i] = 0; g_newMod[i] = 0;
        g_mod[i + MAX_ITEMS/2] = 0; g_newMod[i + MAX_ITEMS/2] = 0;
    }
    g_itemCount = 0; g_weaponCount = 0; g_bowCount = 0; g_shieldCount = 0;
    g_rupees = 0; g_newRupees = 0;
    g_questCount = 0;

    char path[75];
    snprintf(path, sizeof path, "save:/%d/game_data.sav", slot);
    g_fp = fopen(path, "r+b");
    if (!g_fp) return 0;

    int hdr;
    fread(&hdr, sizeof(int), 1, g_fp);
    for (g_version = 0; g_version < VERSIONS; g_version++)
        if (hdr == HEADER[g_version]) break;
    if (g_version >= VERSIONS) { fclose(g_fp); g_fp = NULL; return 0; }

    fseek(g_fp, RUPEE[g_version], SEEK_SET);
    fread(&g_rupees, sizeof(int), 1, g_fp);
    g_newRupees = g_rupees;

    read_items(g_version);

    /* ---- quest flags (offsets based on version) ---- */
    g_questCount = sizeof(QUEST_NAMES) / sizeof(QUEST_NAMES[0]);
    int qoff = 0x0bb000 + g_version * 0x0100;
    for (int i = 0; i < g_questCount; i++) {
        strcpy(g_quests[i], QUEST_NAMES[i]);
        fseek(g_fp, qoff + i * 4, SEEK_SET);
        fread(&g_questFlag[i], sizeof(int), 1, g_fp);
        g_newQuestFlag[i] = g_questFlag[i];
    }
    return 1;
}

void save_changes() {
    if (!g_fp) return;
    if (g_newRupees != g_rupees) {
        fseek(g_fp, RUPEE[g_version], SEEK_SET);
        fwrite(&g_newRupees, sizeof(int), 1, g_fp);
    }
    for (int i = 0; i < g_itemCount; i++) {
        if (g_newQuant[i] != g_quant[i]) {
            fseek(g_fp, QUANT[g_version] + 8 * i, SEEK_SET);
            fwrite(&g_newQuant[i], sizeof(int), 1, g_fp);
        }
    }

    int wc = g_weaponCount;
    for (int i = 0; i < wc; i++) {
        if (g_newMod[i] != g_mod[i]) {
            fseek(g_fp, F_WPN[g_version] + 8 * i, SEEK_SET);
            fwrite(&g_newMod[i], sizeof(int), 1, g_fp);
        }
        if (g_newMod[i + MAX_ITEMS/2] != g_mod[i + MAX_ITEMS/2]) {
            fseek(g_fp, FV_WPN[g_version] + 8 * i, SEEK_SET);
            fwrite(&g_newMod[i + MAX_ITEMS/2], sizeof(int), 1, g_fp);
        }
    }
    for (int i = 0; i < g_bowCount; i++) {
        int idx = i + wc;
        if (g_newMod[idx] != g_mod[idx]) {
            fseek(g_fp, F_BOW[g_version] + 8 * i, SEEK_SET);
            fwrite(&g_newMod[idx], sizeof(int), 1, g_fp);
        }
        if (g_newMod[idx + MAX_ITEMS/2] != g_mod[idx + MAX_ITEMS/2]) {
            fseek(g_fp, FV_BOW[g_version] + 8 * i, SEEK_SET);
            fwrite(&g_newMod[idx + MAX_ITEMS/2], sizeof(int), 1, g_fp);
        }
    }
    for (int i = 0; i < g_shieldCount; i++) {
        int idx = i + wc + g_bowCount + 6;
        if (g_newMod[idx] != g_mod[idx]) {
            fseek(g_fp, F_SHLD[g_version] + 8 * i, SEEK_SET);
            fwrite(&g_newMod[idx], sizeof(int), 1, g_fp);
        }
        if (g_newMod[idx + MAX_ITEMS/2] != g_mod[idx + MAX_ITEMS/2]) {
            fseek(g_fp, FV_SHLD[g_version] + 8 * i, SEEK_SET);
            fwrite(&g_newMod[idx + MAX_ITEMS/2], sizeof(int), 1, g_fp);
        }
    }

    int qoff = 0x0bb000 + g_version * 0x0100;
    for (int i = 0; i < g_questCount; i++) {
        if (g_newQuestFlag[i] != g_questFlag[i]) {
            fseek(g_fp, qoff + i * 4, SEEK_SET);
            fwrite(&g_newQuestFlag[i], sizeof(int), 1, g_fp);
        }
    }
}

const char *translate(const char *raw) {
    for (int i = 0; i < 200; i++)
        if (strcmp(raw, RAW[i]) == 0) return DISP[i];
    return raw;
}

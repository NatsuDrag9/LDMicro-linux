/*
 * A library for storing parameters in a key file
 * 
 * This library is an analog to the windows freeze library
 * developed by Jonathan Westhues.
 * 
 * R Ramana, 2018
 */
#include "linuxUI.h"
#include "freezeLD.h"
#include <cstdlib>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

/*
 * store a window's position in the registry, or fail silently if the registry calls don't work
 */
void FreezeWindowPosF(HWID hwid, char *subKey, char *name)
{
    g_print("freezing");
    char* moveToKeyLocatin = (char *)malloc(strlen(subKey) + 35);
    if(!moveToKeyLocatin)
        return;
    sprintf(moveToKeyLocatin, "mkdir -p %s/%s", LDMICRO_REGISTER, subKey);
    system(moveToKeyLocatin);  
    sprintf(moveToKeyLocatin, "cd %s/%s", LDMICRO_REGISTER, subKey);
    if (-1 == system(moveToKeyLocatin))
        return;
    free(moveToKeyLocatin);

    char *keyName = (char *)malloc(strlen(name) + 30);
    if(!keyName)
        return;

    Key newKey;

    int val;
    g_print("get width");
    sprintf(keyName, "%s_width", name);
    std::ofstream Register(keyName, std::ios::binary | std::ios::trunc);
    if (!Register.is_open())
        return;
    gtk_window_get_size(GTK_WINDOW(hwid), &val, NULL);
    newKey.type = 'i';
    newKey.val.i = val;
    Register.write((char*) &newKey, sizeof(newKey));
    Register.close();

    g_print("get height");
    sprintf(keyName, "%s_height", name);
    Register.open(keyName, std::ios::binary | std::ios::trunc);
    if (!Register.is_open())
        return;
    gtk_window_get_size(GTK_WINDOW(hwid), NULL, &val);
    newKey.type = 'i';
    newKey.val.i = val;
    Register.write((char*) &newKey, sizeof(newKey));
    Register.close();

    g_print("get posX");
    sprintf(keyName, "%s_posX", name);
    Register.open(keyName, std::ios::binary | std::ios::trunc);
    if (!Register.is_open())
        return;
    gtk_window_get_position(GTK_WINDOW(hwid), &val, NULL);
    newKey.type = 'i';
    newKey.val.i = val;
    Register.write((char*) &newKey, sizeof(newKey));
    Register.close();
    
    g_print("get posY");
    sprintf(keyName, "%s_posY", name);
    Register.open(keyName, std::ios::binary | std::ios::trunc);
    if (!Register.is_open())
        return;
    gtk_window_get_position(GTK_WINDOW(hwid), NULL, &val);
    newKey.type = 'i';
    newKey.val.i = val;
    Register.write((char*) &newKey, sizeof(newKey));
    Register.close();

    g_print("get max");
    sprintf(keyName, "%s_maximized", name);
    Register.open(keyName, std::ios::binary | std::ios::trunc);
    if (!Register.is_open())
        return;
    newKey.type = 'b';
    newKey.val.b = gtk_window_is_maximized(GTK_WINDOW(hwid));
    Register.write((char*) &newKey, sizeof(newKey));
    Register.close();

    free(keyName);
    g_print("freezed");
}

static void Clamp(LONG *v, LONG min, LONG max)
{
    if(*v < min) *v = min;
    if(*v > max) *v = max;
}

/*
 * retrieve a window's position from the registry, or do nothing if there is no info saved
 */
void ThawWindowPosF(HWID hwid, char *subKey, char *name)
{
    char* moveToKeyLocatin = (char *)malloc(strlen(name) + 30);
    if(!moveToKeyLocatin)
        return;
    sprintf(moveToKeyLocatin, "cd %s/%s", LDMICRO_REGISTER, subKey);
    if (-1 == system(moveToKeyLocatin))
        return;
    free(moveToKeyLocatin);
    
    char *keyName = (char *)malloc(strlen(name) + 30);
    if(!keyName)
        return;

    Key newKey1,  newKey2;

    /// set size
    sprintf(keyName, "%s_width", name);
    std::ifstream Register(keyName, std::ios::binary);
    if (!Register.is_open())
        return;
    Register.read((char*) &newKey1, sizeof(newKey1));
    Register.close();

    sprintf(keyName, "%s_height", name);
    Register.open(keyName, std::ios::binary);
    if (!Register.is_open())
        return;
    Register.read((char*) &newKey2, sizeof(newKey2));
    Register.close();
    if (newKey1.type == 'i' && newKey2.type == 'i')
        gtk_window_resize(GTK_WINDOW(hwid), newKey1.val.i, newKey2.val.i);


    /// set position
    sprintf(keyName, "%s_posX", name);
    Register.open(keyName, std::ios::binary);
    if (!Register.is_open())
        return;
    Register.read((char*) &newKey1, sizeof(newKey1));
    Register.close();

    sprintf(keyName, "%s_posY", name);
    Register.open(keyName, std::ios::binary);
    if (!Register.is_open())
        return;
    Register.read((char*) &newKey2, sizeof(newKey2));
    Register.close();
    if (newKey1.type == 'i' && newKey2.type == 'i')
        gtk_window_move(GTK_WINDOW(hwid), newKey1.val.i, newKey2.val.i);


    sprintf(keyName, "%s_maximized", name);
    Register.open(keyName, std::ios::binary);
    if (!Register.is_open())
        return;
    Register.read((char*) &newKey1, sizeof(newKey1));
    Register.close();
    if  (newKey1.type == 'b')
        if (newKey1.val.b)
            gtk_window_maximize(GTK_WINDOW(hwid));


    /// gtk_window_move handles off-screen window placement

    free(keyName);
}

/*
 * store a DWORD setting in the registry
 */
void FreezeDWORDF(DWORD val, char *subKey, char *name)
{
    char* moveToKeyLocatin = (char *)malloc(strlen(name) + 30);
    if(!moveToKeyLocatin)
        return;
    sprintf(moveToKeyLocatin, "mkdir -p %s/%s", LDMICRO_REGISTER, subKey);
    system(moveToKeyLocatin);
    sprintf(moveToKeyLocatin, "cd %s/%s", LDMICRO_REGISTER, subKey);
    if (-1 == system(moveToKeyLocatin))
        return;
    free(moveToKeyLocatin);

    Key newKey;
    newKey.type = 'D';
    newKey.val.D = val;
    std::ofstream Register(name, std::ios::binary | std::ios::trunc);
    Register.write((char*) &newKey, sizeof(newKey));
    Register.close();
}

/*
 * retrieve a DWORD setting, or return the default if that setting is unavailable
 */
DWORD ThawDWORDF(DWORD val, char *subKey, char *name)
{
    char* moveToKeyLocatin = (char *)malloc(strlen(name) + 30);
    if(!moveToKeyLocatin)
        return val;
    sprintf(moveToKeyLocatin, "cd %s/%s", LDMICRO_REGISTER, subKey);
    if (-1 == system(moveToKeyLocatin))
        return val;
    free(moveToKeyLocatin);

    Key newKey;

    std::ifstream Register(name, std::ios::binary);
    Register.read((char*) &newKey, sizeof(newKey));
    Register.close();
    if(Register.bad())
        return val;

    if(newKey.type == 'D')
        return newKey.val.D;
    else
        return val;
}

/*
 * store a string setting in the registry
 */
void FreezeStringF(char *val, char *subKey, char *name)
{
    char* moveToKeyLocatin = (char *)malloc(strlen(name) + 30);
    if(!moveToKeyLocatin)
        return;
    sprintf(moveToKeyLocatin, "mkdir -p %s/%s", LDMICRO_REGISTER, subKey);
    system(moveToKeyLocatin);
    sprintf(moveToKeyLocatin, "cd %s/%s", LDMICRO_REGISTER, subKey);
    if (-1 == system(moveToKeyLocatin))
        return;
    free(moveToKeyLocatin);
    
    std::ofstream Register(name, std::ios::trunc);
    Register << strlen(val)+1 << "\n";
    Register << val;
    Register.close();
}

/*
 * retrieve a string setting, or return the default if that setting is unavailable
 */
void ThawStringF(char *val, int max, char *subKey, char *name)
{
    char* moveToKeyLocatin = (char *)malloc(strlen(name) + 30);
    if(!moveToKeyLocatin)
        return;
    sprintf(moveToKeyLocatin, "cd %s/%s", LDMICRO_REGISTER, subKey);
    if (-1 == system(moveToKeyLocatin))
        return;
    free(moveToKeyLocatin);

    std::ifstream Register(name);
    int l;
    Register >> l;
    if (l >= max)
        return;
    Register >> val;
}


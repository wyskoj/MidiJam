//
// Created by Jacob on 3/16/2026.
//

#ifndef MIDIJAM_DLSCOLLECTIONWRAPPER_H
#define MIDIJAM_DLSCOLLECTIONWRAPPER_H

#include <windows.h>
#include <dmusici.h>

/**
 * Wraps an IDirectMusicScript and its associated loader and performance context.
 *
 * Despite the name suggesting a DLS collection, this class manages a
 * IDirectMusicScript object. The name is likely a misnomer in the original source.
 * See DirectMusicSystem::LoadDlsCollection for context.
 *
 * Original struct layout (sizeof = 0x10):
 *   +0x00  void**                    vtable
 *   +0x04  IDirectMusicScript*       pScript
 *   +0x08  IDirectMusicLoader*       pLoader
 *   +0x0C  IDirectMusicPerformance8* pPerformance
 */
class DlsCollectionWrapper
{
public:
    IDirectMusicScript*       pScript;
    IDirectMusicLoader8*      pLoader;
    IDirectMusicPerformance8* pPerformance;

    /**
     * Stores the performance, loader, and script pointers.
     * Does not take ownership — caller is responsible for COM reference counts.
     * @param pPerformance  The DirectMusic performance object.
     * @param pLoader       The DirectMusic loader object.
     * @param pScript       The DirectMusic script object to wrap.
     */
    DlsCollectionWrapper(
        IDirectMusicPerformance8* pPerformance,
        IDirectMusicLoader8*      pLoader,
        IDirectMusicScript*       pScript);

    /**
     * Calls Cleanup().
     */
    ~DlsCollectionWrapper();

    /**
     * Releases the loader's reference to the script, releases the script,
     * and zeroes all pointers.
     */
    void Cleanup();
};

static_assert(sizeof(DlsCollectionWrapper) == 12, "DlsCollectionWrapper layout mismatch");

#endif // MIDIJAM_DLSCOLLECTIONWRAPPER_H
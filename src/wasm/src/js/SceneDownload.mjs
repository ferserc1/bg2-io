
import fs from 'fs';

import { downloadFile } from './downloadUtils.mjs';

// Returns the scene json
export const downloadSceneFile = async (url,dstPath) => {
    const result = await downloadFile(url, dstPath);
    console.log(result);
    throw new Error("Not implemented");
}

// Donwload the scene and all the resources to dstPath, and returns an index with the
// downloaded files. The progressCallback is called several times during download.
export const downloadScene = async (url, dstPath, progressCallback = null) => {
    // Create destination directory
    await fs.promises.mkdir(dstPath, { recursive: true });
    const sceneFile = await downloadSceneFile(url,dstPath);
    console.log(sceneFile);
    return null;
}

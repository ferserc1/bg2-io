
import fs from 'fs';
import { URL } from 'url';
import path from 'path';

import { downloadFile } from './downloadUtils.mjs';
import Bg2ioWrapper from './Bg2ioWrapper.mjs';
import Bg2ioModule from './Bg2ioModule.cjs';




function resolveUrl(fileUrl, baseUrl) {
    if (/^https?\:\/\//i.test(fileUrl)) {
        const urlData = new URL(fileUrl);
        const localPath = path.basename(urlData.pathname);
        return {
            fileUrl,
            localPath
        }
    }
    else {
        const urlData = new URL(baseUrl);
        urlData.pathname = path.join(urlData.pathname, fileUrl);
        const localPath = path.basename(urlData.pathname);
        return {
            fileUrl: urlData.href,
            localPath
        }
    }
}

export const getSceneResources = (sceneData, baseUrl) => {
    const result = {};

    const resolveResource = (comp,attrib) => {
        const resolvedUrl = resolveUrl(comp[attrib], baseUrl);
        if (!result[resolvedUrl.fileUrl]) {
            result[resolvedUrl.fileUrl] = resolvedUrl.localPath;
        }
        comp[attrib] = resolvedUrl.localPath;
    }

    const getResources = (node) => {
        node.components?.forEach(comp => {
            switch (comp.type) {
            case 'Environment':
                resolveResource(comp,'equirectangularTexture');
                break;
            case 'Drawable':
                const drwName = comp.name;
                comp.name += '.bg2';
                resolveResource(comp,'name');
                comp.name = drwName + '.vwglb';
                resolveResource(comp,'name');
                comp.name = drwName;
                break;
            }
        });
        node.children?.forEach(child => {
            getResources(child, baseUrl);
        })
    }

    sceneData.scene.forEach(child => getResources(child));

    return result;
}

// Returns the scene json
export const downloadSceneFile = async (url,dstPath) => {
    const sceneFilePath = await downloadFile(url, dstPath);
    const sceneFile = await fs.promises.readFile(sceneFilePath, {encoding: 'utf-8'});
    const sceneJson = JSON.parse(sceneFile);
    return sceneJson;
}

let g_bg2ioModule = null;
let g_bg2ioWrapper = null;

export const getBg2FileResources = async (filePath, baseUrl, result = {}) => {
    if (g_bg2ioModule === null) {
        g_bg2ioModule = await Bg2ioModule();
        g_bg2ioWrapper = new Bg2ioWrapper({ instance: g_bg2ioModule  });
    }
    const buffer = await fs.promises.readFile(filePath);
    const jsonFile = g_bg2ioWrapper.loadBg2FileAsJson(buffer);
    jsonFile.materials.forEach(mat => {
        [
            'diffuse',
            'metallic',
            'roughness',
            'ambientOcclussion',
            'normal',
            'height'
        ].forEach(attrib => {
            const file = mat[attrib];
            if (typeof(file) === 'string') {
                if (/^http/i.test(file)) {
                    result[file] = file.substring(file.lastIndexOf('/') + 1);
                    mat[attrib] = result[file];
                }
                else {
                    result[path.join(baseUrl,file)] = file.substring(file.lastIndexOf('/'));
                    mat[attrib] = result[file];
                }
            }
        })
    });

    // Write file to save changes in material paths
    const modifiedFileBuffer = g_bg2ioWrapper.getBg2BufferFromJson(jsonFile);
    await fs.promises.writeFile(filePath, modifiedFileBuffer);

    return result;
}

// Donwload the scene and all the resources to dstPath, and returns an index with the
// downloaded files. The progressCallback is called several times during download.
export const downloadScene = async (sceneFileUrl, dstPath, progressCallback = null) => {
    // Create destination directory
    await fs.promises.mkdir(dstPath, { recursive: true });
    
    // Get the base URL without the scene file name
    const baseUrl = sceneFileUrl.substring(0, sceneFileUrl.lastIndexOf('/'));

    // Download scene file
    const sceneFile = await downloadSceneFile(sceneFileUrl,dstPath);
    
    // Get file resources
    const resources = getSceneResources(sceneFile, baseUrl);

    // Download scene direct resources
    const indirectResources = {}
    let downloadedModelFiles = {};
    for (const remoteUrl in resources) {
        const ext = remoteUrl.substring(remoteUrl.lastIndexOf('.'));
        try {
            if (ext === ".bg2" || ext === '.vwglb') {
                const urlData = new URL(remoteUrl);
                const fileId = path.parse(urlData.pathname).name;
                if (downloadedModelFiles[fileId]) {
                    //continue;
                }
                else {
                    downloadedModelFiles[fileId] = remoteUrl;
                }

            }
            const localFilePath = await downloadFile(remoteUrl,dstPath);
            console.log(localFilePath);
            if (ext === ".bg2" || ext === '.vwglb') {
                // add indirect resources to indirectResources object
                await getBg2FileResources(localFilePath, baseUrl, indirectResources);
            } 
        }
        catch (err) {
            // Ignore not found errors in bg2 or vwglb 3d models, because the
            // downloader will try to download both, but only one of them will
            // be available
            if (err.code !== 404 || (ext !== '.bg2' && ext !== '.vwglb')) {
                console.error(err);
            }
        }
    }

    // TODO: Download scene indirect resources
    for  (const remoteUrl in indirectResources) {
        try {
            const localFilePath = await downloadFile(remoteUrl, dstPath);
            console.info(`Downloaded: ${localFilePath}`);
        }
        catch (err) {
            if (err.code === 404) {
                console.warn(`Not found: ${remoteUrl}`);
            }
        }
    }

    return sceneFile;
}

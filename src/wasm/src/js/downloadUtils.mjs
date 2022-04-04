

import fs from 'fs';
import http from 'http';
import https from 'https';
import path from 'path';
import url from 'url';

export const downloadFile = async (fileUrl,dstPath) => {
    let downloadProtocol = /https\:/i.test(fileUrl) ? https : (/http\:/i.test(fileUrl) ? http : null);
    if (!downloadProtocol) {
        throw new Error("Unsupported URL protocol. Accepted protocols are http or https");
    }

    return new Promise((resolve,reject) => {
        const fileName = path.basename(url.parse(fileUrl).pathname);
        const dstFilePath = path.join(dstPath,fileName);
        const file = fs.createWriteStream(dstFilePath);
        downloadProtocol.get(fileUrl, (response) => {
            if (response.statusCode !== 200) {
                const err = new Error(response.statusMessage);
                err.code = response.statusCode;
                file.close(() => {
                    fs.unlink(dstFilePath, () => reject(err));
                })
            }
            else {
                response.pipe(file);
                file.on('finish', () => {
                    file.close(() => {
                        resolve(dstFilePath);
                    })
                })
            }
        }).on('error', err => {
            file.close(() => {
                fs.unlink(dstFilePath, () => reject(err));
            })
        });
    })
}

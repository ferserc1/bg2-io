
let g_bg2ioModuleLoadingPromise = null;
const loadBg2ioModule = () => {
    if (!g_bg2ioModuleLoadingPromise) {
        g_bg2ioModuleLoadingPromise = new Promise((resolve) => {
            const script = document.createElement('script');
            script.type = 'text/javascript';
            script.src = 'bg2io.js';
            let loaded = false;
            script.onload = script.onreadystatechange = () => {
                if (!loaded) {
                    loaded = true;
                    resolve();
                }
            }
            document.head.appendChild(script);
        });
    }
    return g_bg2ioModuleLoadingPromise;
}

export default class Bg2ioWrapper {
    constructor({ debug = false } = {}) {
        this._instance = null;
        this._debug = debug;
    }

    async init() {
        await loadBg2ioModule();
        this._instance = await Bg2ioModule();
        return this._instance;
    }

    get instance() {
        return this._instance;
    }

    loadBg2File(arrayBuffer) {
        const buffer = new Uint8Array(arrayBuffer);
        const cPtr = this.instance._malloc(buffer.length * buffer.BYTES_PER_ELEMENT);
        this.instance.HEAPU8.set(buffer, cPtr);
        const bg2File = this._instance._loadBg2File(cPtr, buffer.length, this._debug ? 1 : 0);
        return bg2File;
    }

    getBg2FileHeader(file) {
        const headerPtr = this._instance._getBg2FileHeader(file, this._debug ? 1 : 0);
        const charData = new Uint8Array(this.instance.HEAPU8.buffer, headerPtr, 4);
        const numberOfPlist = new Int32Array(this.instance.HEAP32.buffer, headerPtr + 4, 1)[0];
        return {
            endianess: charData[0],
            version: {
                major: charData[1],
                minor: charData[2],
                revision: charData[3]
            },
            numberOfPlist
        }
    }

    freeBg2File(file) {
        this._instance._freeBg2File(file, this._debug ? 1 : 0);
    }

    // async bg2json(arrayBuffer) {
    //     const buffer = new Uint8Array(arrayBuffer);
    //     const cPtr = this.instance._malloc(buffer.length * buffer.BYTES_PER_ELEMENT);
    //     this.instance.HEAPU8.set(buffer, cPtr);
    //     const jsonStringPtr = this.instance._loadBg2Buffer(cPtr, buffer.length, this._debug ? 1 : 0);
    //     const jsonString = this.getString(jsonStringPtr);
    //     this.instance._free(cPtr);
    //     return JSON.parse(jsonString);
    // }

    getString(structPtr) {
        const strSize = new Int32Array(this.instance.HEAPU8.buffer, structPtr, 1)[0];
        const strPtr = new Int32Array(this.instance.HEAPU8.buffer, structPtr + 4)[0];
        const strData = new Uint8Array(this.instance.HEAPU8.buffer, strPtr, strSize);
        const str = new TextDecoder().decode(strData);
        this.instance._freeString(structPtr, this._debug ? 1 : 0);
        return str;
    }
}

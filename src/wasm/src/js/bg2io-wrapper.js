
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

    getMaterialsData(file) {
        const materialStringPtr = this._instance._getMaterialStringRef(file, this._debug ? 1 : 0);
        if (materialStringPtr) {
            const materialString = this.getStringRef(materialStringPtr);
            return JSON.parse(materialString);
        }
        else {
            return {}
        }
    }

    getComponentData(file) {
        const componentStringPtr = this._instance._getComponentStringRef(file, this._debug ? 1 : 0);
        if (componentStringPtr) {
            const componentStringRef = this.getStringRef(componentStringPtr);
            return JSON.parse(componentStringRef);
        }
        else {
            return {}
        }
    }

    freeBg2File(file) {
        this._instance._freeBg2File(file, this._debug ? 1 : 0);
    }
    
    getString(structPtr) {
        const strSize = new Int32Array(this.instance.HEAPU8.buffer, structPtr, 1)[0];
        const strPtr = new Int32Array(this.instance.HEAPU8.buffer, structPtr + 4)[0];
        const strData = new Uint8Array(this.instance.HEAPU8.buffer, strPtr, strSize);
        const str = new TextDecoder().decode(strData);
        this.instance._freeString(structPtr, this._debug ? 1 : 0);
        return str;
    }

    getStringRef(structPtr) {
        const strSize = new Int32Array(this.instance.HEAPU8.buffer, structPtr, 1)[0];
        const strPtr = new Int32Array(this.instance.HEAPU8.buffer, structPtr + 4)[0];
        const strData = new Uint8Array(this.instance.HEAPU8.buffer, strPtr, strSize);
        const str = new TextDecoder().decode(strData);
        this.instance._freeStringRef(structPtr, this._debug ? 1 : 0);
        return str;
    }
}

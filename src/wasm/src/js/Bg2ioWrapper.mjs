
export default class Bg2ioWrapper {
    constructor({ instance, debug = false } = {}) {
        this._instance = instance;
        this._debug = debug;
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

    loadBg2FileAsJson(arrayBuffer) {
        const bg2File = this.loadBg2File(arrayBuffer);
        const header = this.getBg2FileHeader(bg2File);
        const components = this.getComponentData(bg2File);
        const materials = this.getMaterialsData(bg2File);
        const joints = this.getJointData(bg2File);
        const polyList = []
        for (let i = 0; i < header.numberOfPlist; ++i) {
            polyList.push(this.getPolyList(bg2File, i));
        }
        this.freeBg2File(bg2File);
        return { header, components, materials, joints, polyList }
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

    getJointData(file) {
        const jointStringPtr = this._instance._getJointStringRef(file, this._debug ? 1 : 0);
        if (jointStringPtr) {
            const jointStringRef = this.getStringRef(jointStringPtr);
            return JSON.parse(jointStringRef);
        }
        else {
            return {}
        }
    }

    getPolyList(file,index) {
        const plistPtr = this._instance._getPolyList(file, index, this._debug ? 1 : 0);
        if (plistPtr) {
            /*
            typedef struct Bg2ioFloatArrayT {
                int length;         4 bytes
                float * data;       4 bytes
            } Bg2ioFloatArray;

            typedef struct Bg2ioIntArrayT {
                int length;         4 bytes
                int * data;         4 bytes
            } Bg2ioIntArray;

            typedef struct Bg2ioPolyListT {
                char * name;                    4 bytes
                char * matName;                 4 bytes
                char * groupName;               4 bytes
                int visible;                    4 bytes
                Bg2ioFloatArray vertex;         8 bytes
                Bg2ioFloatArray normal;         8 bytes
                Bg2ioFloatArray texCoord0;      8 bytes
                Bg2ioFloatArray texCoord1;      8 bytes
                Bg2ioFloatArray texCoord2;      8 bytes
                Bg2ioIntArray index;            8 bytes
            } Bg2ioPolyList;            total: 64 bytes (16 elements)
            */
            const structData = new Int32Array(this.instance.HEAPU8.buffer, plistPtr, 16);
            const namePtr = structData[0];
            const matNamePtr = structData[1];
            const groupNamePtr = structData[2];
            const visible = structData[3];
            const vertexCount = structData[4];
            const vertexPtr = structData[5];
            const normalCount = structData[6];
            const normalPtr = structData[7];
            const texCoord0Count = structData[8];
            const texCoord0Ptr = structData[9];
            const texCoord1Count = structData[10];
            const texCoord1Ptr = structData[11];
            const texCoord2Count = structData[12];
            const texCoord2Ptr = structData[13];
            const indexCount = structData[14];
            const indexPtr = structData[15];

            const result = {
                name: this.getStringFromCStr(namePtr),
                matName: this.getStringFromCStr(matNamePtr),
                groupName: this.getStringFromCStr(groupNamePtr),
                visible: visible === 1,
                vertex: this.getFloatArray(vertexPtr, vertexCount),
                normal: this.getFloatArray(normalPtr, normalCount),
                texCoord0: this.getFloatArray(texCoord0Ptr, texCoord0Count),
                texCoord1: this.getFloatArray(texCoord1Ptr, texCoord1Count),
                texCoord2: this.getFloatArray(texCoord2Ptr, texCoord2Count),
                index: this.getIntArray(indexPtr, indexCount)
            }

            return result;
        }
        else {
            return null;
        }
    }

    getFloatArray(ptr,count) {
        if (ptr) {
            const data = new Float32Array(this.instance.HEAPU8.buffer, ptr, count);
            return data;
        }
        else {
            return [];
        }
    }

    getIntArray(ptr,count) {
        if (ptr) {
            const data = new Int32Array(this.instance.HEAPU8.buffer, ptr, count);
            return data;
        }
        else {
            return []
        }
    }

    // Get a JS string from a c string
    getStringFromCStr(ptr) {
        if (!ptr) {
            return "";
        }

        const structPtr = this._instance._getStringStructRefFromCStr(ptr, this._debug ? 1 : 0);
        return this.getStringRef(structPtr);
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

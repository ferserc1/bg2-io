import Bg2ioWrapper from '../../bin/wasm/bg2io/Bg2ioWrapper.mjs';
import Bg2ioModule from '../../bin/wasm/bg2io/Bg2ioModule.cjs';

import fs from 'fs';

const instance = await Bg2ioModule();

const wrapper = new Bg2ioWrapper({ instance, debug: true });
if (process.argv.length<3) {
    throw new Error("You must to specify a bg2 file to convert");
}

const modelPath = process.argv[2];
const buffer = fs.readFileSync(modelPath);
const bg2File = wrapper.loadBg2File(buffer);
const header = wrapper.getBg2FileHeader(bg2File);

console.log(header);

const components = wrapper.getComponentData(bg2File);
console.log(components);

const material = wrapper.getMaterialsData(bg2File);
console.log(material);

const joints = wrapper.getJointData(bg2File);
console.log(joints);

for (let i = 0; i<header.numberOfPlist; ++i) {
    const plist = wrapper.getPolyList(bg2File, i);
    console.log(plist);
}

wrapper.freeBg2File(bg2File);


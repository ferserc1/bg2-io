import Bg2ioWrapper from '../../bin/wasm/bg2io/Bg2ioWrapper.mjs';
import Bg2ioModule from '../../bin/wasm/bg2io/Bg2ioModule.cjs';

import fs from 'fs';

const instance = await Bg2ioModule();

const wrapper = new Bg2ioWrapper({ instance, debug: true });
if (process.argv.length<3) {
    throw new Error("You must to specify a bg2 file to copy");
}

const modelPath = process.argv[2];
const buffer = fs.readFileSync(modelPath);

const jsonFile = wrapper.loadBg2FileAsJson(buffer);
console.log(jsonFile);

const fileBuffer = wrapper.getBg2BufferFromJson(jsonFile);
if (fileBuffer) {
    fs.writeFileSync(modelPath + ".copy.bg2", fileBuffer);
}

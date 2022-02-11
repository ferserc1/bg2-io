
import Bg2ioWrapper from '../../bin/wasm/bg2io/Bg2ioBrowser';

const wrapper = await Bg2ioWrapper({ debug: true, wasmPath: "bg2io" });

const modelFile = "cubes.bg2";
const response = await fetch(`../../resources/${modelFile}`);
if (!response.ok) {
    throw new Error("Could not load bg2 model file");
}
const data = await response.arrayBuffer();
const jsonData = wrapper.loadBg2FileAsJson(data);
console.log(jsonData);

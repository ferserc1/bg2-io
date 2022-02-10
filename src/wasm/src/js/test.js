
import Bg2ioWrapper from "./bg2io-wrapper";

const wrapper = new Bg2ioWrapper({ debug: true });
const instance = await wrapper.init();

const modelFile = "sphere.bg2";
const response = await fetch(`../../resources/${modelFile}`);
if (response.ok) {
    const data = await response.arrayBuffer();
    const bg2File = wrapper.loadBg2File(data);
    const header = wrapper.getBg2FileHeader(bg2File);
    console.log(header);
}
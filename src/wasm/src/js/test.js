
import Bg2ioWrapper from "./bg2io-wrapper";

const wrapper = new Bg2ioWrapper({ debug: true });
const instance = await wrapper.init();

const modelFile = "cubes.bg2";
const response = await fetch(`../../resources/${modelFile}`);
if (response.ok) {
    const data = await response.arrayBuffer();
    const bg2File = wrapper.loadBg2File(data);
    const header = wrapper.getBg2FileHeader(bg2File);
    console.log(header);

    const components = wrapper.getComponentData(bg2File);
    console.log(components);
    
    const material = wrapper.getMaterialsData(bg2File);
    console.log(material);

    const joints = wrapper.getJointData(bg2File);
    console.log(joints);

    wrapper.freeBg2File(bg2File);
}
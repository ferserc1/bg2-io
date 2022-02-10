
import loadBg2ioWrapper from '../../bin/wasm/bg2io/Bg2ioBrowser';

const wrapper = await loadBg2ioWrapper({ debug: true, wasmPath: "bg2io" });

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

    for (let i = 0; i<header.numberOfPlist; ++i) {
        const plist = wrapper.getPolyList(bg2File, i);
        console.log(plist);
    }

    wrapper.freeBg2File(bg2File);

    console.log("Done");
}
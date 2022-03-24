import { downloadScene } from "../../bin/wasm/bg2io/SceneDownload.mjs";


if (process.argv.length<4) {
    throw new Error("You must to specify a .vitscnj scene url to download and a destination path");
}
const sceneUrl = process.argv[2];
const dstPath = process.argv[3];

const result = await downloadScene(sceneUrl, dstPath, progress => console.log(progress));

console.log(result);
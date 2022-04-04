import { downloadScene } from "../../bin/wasm/bg2io/SceneDownload.mjs";


if (process.argv.length<4) {
    console.error("You must to specify a .vitscnj scene url to download and a destination path");
    process.exit(-1);
}
const sceneUrl = process.argv[2];
const dstPath = process.argv[3];

const result = await downloadScene(sceneUrl, dstPath, (resource, dstPath, progress, total, err) => {
    if (err) {
        console.error(resource, dstPath, `${progress}/${total}`, err.message);
    }
    else {
        console.log(resource,dstPath, `${progress}/${total}`);
    }
});

if (!result) {
    console.error("Error downloading scene file. Check remote URL and write permissions on destination path");
    process.exit(-2);
}

console.log(JSON.stringify(result, "", "  "));
console.log(`Scene downloaded at ${result.mainSceneFilePath}`);

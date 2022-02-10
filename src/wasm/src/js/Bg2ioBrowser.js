import Bg2ioWrapper from './Bg2ioWrapper';

let g_bg2ioModuleLoadingPromise = null;
const loadBg2ioModule = (path) => {
    if (!g_bg2ioModuleLoadingPromise) {
        g_bg2ioModuleLoadingPromise = new Promise((resolve) => {
            const script = document.createElement('script');
            script.type = 'text/javascript';
            script.src = path;
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

export default async ({ debug = false, wasmPath = '' } = {}) => {
    if (wasmPath !== "" && !/\/$/.test(wasmPath)) {
        wasmPath += '/';
    }
    await loadBg2ioModule(wasmPath + 'bg2io.js');
    const instance = await Bg2ioModule();
    return new Bg2ioWrapper({ instance, debug });
}

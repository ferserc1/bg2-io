
const loadScript = (path) => {
    return new Promise((resolve) => {
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
    })
}


loadScript('bg2io.js').then(async () => {
    const instance = await Bg2ioModule();

    instance._sayHello();

    instance.ccall("sayHello");
});

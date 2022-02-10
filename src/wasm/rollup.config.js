import copy from 'rollup-plugin-copy';

export default [
    {
        input: 'src/js/Bg2ioBrowser.js',

        watch: {
            include: "./**"
        },

        output: {
            file: '../../bin/wasm/bg2io/Bg2ioBrowser.js',
            format: 'es',
            sourcemap: 'inline'
        },

        plugins: [
            copy({
                targets: [
                    { src: "build/bg2io.js", dest: "../../bin/wasm/bg2io" },
                    { src: "build/bg2io.wasm", dest: "../../bin/wasm/bg2io" },

                    // bg2io for Node.js
                    { src: "build/Bg2ioModule.cjs", dest: "../../bin/wasm/bg2io" },
                    { src: "build/Bg2ioModule.wasm", dest: "../../bin/wasm/bg2io" },
                    { src: "src/js/Bg2ioWrapper.mjs", dest: "../../bin/wasm/bg2io" }
                ]
            })
        ]
    },

    {
        input: 'example-browser.js',

        watch: {
            include: "./**"
        },

        output: {
            file: '../../bin/wasm/example-browser.js',
            format: 'es',
            sourcemap: 'inline'
        },

        plugins: [
            copy({
                targets: [
                    { src: "index.html", dest: "../../bin/wasm" }
                ]
            })
        ]
    }
]

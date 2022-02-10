import copy from 'rollup-plugin-copy';

export default [
    {
        input: 'src/js/bg2io-wrapper.js',

        watch: {
            include: "./**"
        },

        output: {
            file: '../../bin/wasm/bg2io-wrapper.js',
            format: 'es',
            sourcemap: 'inline'
        },

        plugins: [
            copy({
                targets: [
                    { src: "src/html/index.html", dest: "../../bin/wasm" }
                ]
            })
        ]
    },

    {
        input: 'src/js/test.js',

        watch: {
            include: "./**"
        },

        output: {
            file: '../../bin/wasm/test.js',
            format: 'es',
            sourcemap: 'inline'
        }
    }
]

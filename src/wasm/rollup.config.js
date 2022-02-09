import copy from 'rollup-plugin-copy';

export default [
    {
        input: 'src/js/wrapper.js',

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
    }
]

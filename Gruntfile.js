var shell = require('shelljs');

module.exports = function (grunt) {
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        connect: {
            server: {
                options: {
                    base: '__html',
                    livereload: 8002,
                    port: 8000
                }
            }
        },
        watch: {
            files: ['**/*.re', '**/*.md', '**/*.png', '**/*.jpg', '**/*.erb', '!__html/*', '!__html/**/*', '!book/layouts/layout.html.erb'],
            options: {
                livereload: 8002
            }
        }
    });

    grunt.event.on('watch', function (action, filepath, target) {
        if (filepath.match(/(.*)(?:\.([^.]+$))/)[2] == "re" && action != "changed")
            return
        shell.exec("sh build.sh html")
    });

    grunt.loadNpmTasks('grunt-contrib-connect');
    grunt.loadNpmTasks('grunt-contrib-watch');

    grunt.registerTask('default', ['connect', 'watch']);
    grunt.registerTask('start', '', function () {
        if (process.platform == "darwin")
            shell.exec('open editor-manual.sublime-project');
        else
            shell.exec('editor-manual.sublime-project');
        grunt.task.run('connect', 'watch');
    });
};
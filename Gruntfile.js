var shell = require('shelljs');

module.exports = function (grunt) {
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        connect: {
            server: {
                options: {
                    base: '__web',
                    livereload: 8002,
                    port: 8000
                }
            }
        },
        watch: {
            files: ['unityprojects/**/**.cs','book/**/*.re', 'book/**/*.md', 'book/**/*.png', 'book/**/*.jpg', 'book/**/*.erb', '!__html/*', '!__html/**/*', '!book/layouts/layout.html.erb'],
            options: {
                livereload: 8002
            }
        }
    });

    grunt.event.on('watch', function (action, filepath, target) {
        var match =  filepath.match(/(.*)(?:\.([^.]+$))/);
        var file_name = match[1]
        var extension =match[2]

        if ((extension == "re" || extension == "md") && action != "changed") return;
        
       shell.exec("sh build.sh web_watch " + filepath)
    });

    grunt.loadNpmTasks('grunt-contrib-connect');
    grunt.loadNpmTasks('grunt-contrib-watch');

    grunt.registerTask('default',function () {
        
         shell.exec("sh build.sh web")
         grunt.task.run('connect', 'watch');
    });
    grunt.registerTask('start', '', function () {
        if (process.platform == "darwin")
            shell.exec('open editor-manual.sublime-project');
        else
            shell.exec('editor-manual.sublime-project');
        grunt.task.run('default');
    });
};
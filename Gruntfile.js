module.exports = function(grunt) {
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    connect: {
      server: {
        options: {
          base: './book/__html',
          livereload: 8002,
          port: 8000
        }
      }
    },
    watch: {
      files: ['**/*.re', '**/*.png', '**/*.jpg', '!**/*.erb',
        '!book/__html/*', '!book/__html/**/*'
      ],
      tasks: ['exec:build_html'],
      options: {
        livereload: 8002
      }
    },
    exec: {
      build_html: 'sh build.sh html'
    }
  });

  grunt.loadNpmTasks('grunt-contrib-connect');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-exec');

  grunt.registerTask('default', ['connect', 'watch']);
};

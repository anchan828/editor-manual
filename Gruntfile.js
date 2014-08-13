module.exports = function(grunt) {
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    connect: {
      site: {
      }
    },
    watch: {
      files: ['**/*.re','**/*.png','**/*.jpg','!book/__build/**/*'],
      tasks: ['exec:build_html'],
      options: {
        livereload: true
      }
    },
    exec: {
        build_html: 'sh build.sh html'
    }
  });

  grunt.loadNpmTasks('grunt-contrib-connect');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-exec');

  grunt.registerTask('default', ['connect','watch']);
};
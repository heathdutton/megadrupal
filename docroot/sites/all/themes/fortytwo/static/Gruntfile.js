'use strict';

module.exports = function(grunt) {
  grunt.initConfig({
    notify: {
      watch_sass: {
        options: {
          message: 'SASS finished compiling'
        }
      }
    },
    watch: {
      css: {
        files: ['sass/**/*.sass', 'Gruntfile.js'],
        tasks: ['compass:watch', 'notify:watch_sass'],
        options: {
          spawn: false,
          reload: true,
          atBegin: true
        }
      }
    },
    compass: {
      clean: {
        options: {
          clean: true,
          watch: true,
          sassDir: 'sass',
          cssDir: 'css'
        }
      },
      watch: {
        options: {
          sourcemap: false,
          sassDir: 'sass',
          cssDir: 'css',
          watch: false
        }
      }
    }
  });

  require('load-grunt-tasks')(grunt);

  grunt.registerTask('default', ['watch']);
};

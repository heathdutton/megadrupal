module.exports = function (grunt) {
  "use strict";
  var neat = require('node-neat').includePaths;
  // Config...
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    watch: {
      scripts: {
        options: {
          livereload:true
        },
        files: ['./js/**/*.js'],
        tasks: ['jshint']
      },
      css: {
        files: ['./sass/**/*.scss'],
        tasks: ['sass:dev'],
        options: {
          spawn: false
        }
      },
      livereload: {
        options: {
          livereload:true
        },
        files:[
          './css/**/*.css',
        ]
      }
    },
    jshint: {
      options: {
        jshintrc: '.jshintrc'
      },
      all: [
        'js/src/**/*.js',
        '!js/scripts.gen.js'
      ]
    },
    uglify: {
      prod: {
        options: {
          mangle: true,
          compress: true
        },
        files: {
          'js/scripts.gen.js': ['js/scripts.js']
        }
      }
    },
    sass: {
      options: {
        includePaths: neat
      },
      prod: {
        options: {
          outputStyle: 'compressed'
        },
        files: {
          './css/style.css': './sass/style.scss',
          './css/print.css': './sass/print.scss',
          './css/wysiwyg.css': './sass/wysiwyg.scss'
        }
      },
      dev: {
        options: {
          outputStyle: 'expanded'
        },
        files: {
          './css/style.css': './sass/style.scss',
          './css/print.css': './sass/print.scss',
          './css/wysiwyg.css': './sass/wysiwyg.scss'
        }
      }
    }
  });
  // Load tasks...
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-sass');
  // Task aliases and tasks
  grunt.registerTask('prod', [
    'uglify:prod',
    'sass:prod'
  ]);
  // Default task.
  grunt.registerTask('default', 'watch');
};

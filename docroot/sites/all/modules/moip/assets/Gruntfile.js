module.exports = function(grunt) {

  grunt.initConfig({
    compass: {
      options: {
        config: 'config.rb'
      },
      dist: {
        options: {
          environment: 'production',
          outputStyle: 'compressed',
        }
      },
      dev: {
        options: {
          outputStyle: 'expanded' //nested, expanded, compact, compressed
        }
      },
    },
    coffee: {
      dist: {
        expand: true,
        flatten: false,
        cwd: 'coffee',
        src: ['moip.coffee'],
        dest: 'js/',
        ext: '.min.js'
      },
      dev: {
        expand: true,
        flatten: true,
        cwd: 'coffee',
        src: ['moip.coffee'],
        dest: 'js/',
        ext: '.js'
      },
    },
    watch: {
      css: {
        files: ['sass/*.scss'],
        tasks: ['compass:dev']
      },
      scripts: {
        files: ['coffee/moip.coffee'],
        tasks: ['coffee:dev', 'jshint']
      }
    },
    jshint: {
      all: ['js/moip.js']
    },
    uglify: {
      options: {
        mangle: false
      },
      my_target: {
        files: {
          'js/moip.min.js': ['js/moip.js']
        }
      }
    }
  });

  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-contrib-compass');
  grunt.loadNpmTasks('grunt-contrib-coffee');
  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-uglify');

  grunt.registerTask('dist', ['compass:dist', 'coffee:dist', 'uglify']);

};
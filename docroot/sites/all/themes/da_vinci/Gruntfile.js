module.exports = function(grunt) {
  grunt.initConfig({
    sass_globbing: {
      main: {
        files: {
          'sass/maps/_components.sass': 'sass/partials/components/**/*[.sass,.scss]',
          'sass/maps/_libs.sass': 'sass/partials/lib/*[.sass,.scss]',
          'sass/maps/_base.sass': 'sass/partials/base/*[.sass,.scss]',
          'sass/maps/_regions.sass': 'sass/partials/regions/*[.sass,.scss]',
          'sass/maps/_content.sass': 'sass/partials/content/**/*.sass',
        },
        options: {
          useSingleQuotes: false
        }
      }
    },
    sass: {
      dist: {
        files: [{
          expand: true,
          cwd: 'sass',
          src: ['*.sass'],
          dest: 'css',
          ext: '.css'
        }]
      },
      options: {
        style: 'expanded'
      }
    },
    watch: {
      sass: {
        files: ['sass/**/**{.scss,.sass}'],
        tasks: ['default']
      },
      options: {
        livereload: true,
        spawn: false
      }
    },
    cssmin: {
      options: {
        shorthandCompacting: false,
        roundingPrecision: -1
      },
      target: {
        files: {
          'css/main.css': ['css/main.css'],
          'css/libs.css': ['css/libs.css'],
          'css/panel.css': ['css/panel.css']
        }
      }
    }
  });
  grunt.loadNpmTasks('grunt-sass');
  grunt.loadNpmTasks('grunt-sass-globbing');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-contrib-cssmin');
  grunt.registerTask('dist', ['sass_globbing:main','sass','cssmin']);
  grunt.registerTask('default', ['dist', 'watch']);
}
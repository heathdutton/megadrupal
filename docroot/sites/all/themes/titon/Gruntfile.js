module.exports = function(grunt) {
  var theme_name = 'titon';

  var global_vars = {
    theme_name: theme_name,
    theme_path: '../' + theme_name,
    theme_css: 'css',
    theme_scss: 'scss',
    theme_templates: 'templates'
  }

  grunt.initConfig({
    global_vars: global_vars,
    pkg: grunt.file.readJSON('package.json'),

    sass: {
      dist: {
        options: {
          outputStyle: 'compressed',
          includePaths: ['<%= global_vars.theme_scss %>', require('node-bourbon').includePaths]
        },
        files: {
          '<%= global_vars.theme_css %>/<%= global_vars.theme_name %>.css': '<%= global_vars.theme_scss %>/toolkit.scss'
        }
      }
    },

    watch: {
      grunt: { files: ['Gruntfile.js'] },

      sass: {
        files: '<%= global_vars.theme_scss %>/**/*.scss',
        tasks: ['sass'],
        options: {
          livereload: true
        }
      },

      theme_templates: {
        files: '<%= global_vars.theme_templates %>/*.tpl.php',
        options: {
          livereload: true
        }
      }
    }
  });

  grunt.loadNpmTasks('grunt-sass');
  grunt.loadNpmTasks('grunt-contrib-watch');

  grunt.registerTask('build', ['sass']);
  grunt.registerTask('default', ['build', 'watch']);
}
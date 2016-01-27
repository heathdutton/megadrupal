module.exports = function (grunt) {
  require('time-grunt')(grunt);
  grunt.loadNpmTasks('grunt-cssbeautifier');
  grunt.loadNpmTasks('grunt-strip-css-comments');
  grunt.loadNpmTasks('grunt-notify');
  grunt.loadNpmTasks('grunt-contrib-cssmin');
  grunt.loadNpmTasks('grunt-csscomb');
  grunt.initConfig({
    pkg: grunt.file.readJSON("package.json"),

    // Define paths.
    paths: {
      sass: 'source/sass',
      devCSS: 'css',
      prodCSS: 'source/deploy/styles',
    }, // paths

    uglify: {
      global: {
        files: {
          "js/site.min.js": ['source/js/*.js']
        }
      }
    }, // uglify

    cssbeautifier: {
      global: {
        files: {
          src: ['css/*.css']
        },
        options: {
          indent: '  ',
          openbrace: 'end-of-line',
          autosemicolon: false
        }
      }
    }, // cssbeautifier

    stripCssComments: {
      global: {
        files: [{
          expand: true,
          src: ['css/*.css']
        }]
      }
    }, // stripCssComments

    sass: {
      global: {
        options: {
          sourceMap: true,
          sourceComments: false,
          outputStyle: 'nested'
        },
        // I suspect this method slows down grunt
        // @todo - test with named files.
        files: [{
          expand: true,
          cwd: '<%= paths.sass %>/',
          src: ['**/*.scss'],
          dest: '<%= paths.devCSS %>/',
          ext: '.css'
        },
        ],
      }
    }, // sass

    watch: {
      options: {
        livereload: true
      },
      site: {
        files: ['templates/**/*.tpl.php', 'js/**/*.{js,json}', 'css/*.css', 'images/**/*.{png,jpg,jpeg,gif,webp,svg}']
      },
      js: {
        files: ['source/js/*.js'],
        tasks: ["uglify"]
      },
      css: {
        files: ["source/sass/**/*.scss"],
        tasks: ["sass"]
      },
    }, // watch

    csscomb: {
      options: {
        config: 'csscomb.json'
      },
      dynamic_mappings: {
        expand: true,
        cwd: 'css',
        src: ['*.css', '!*.min.css'], // Pattern(s) to match.
        dest: 'css'
      }
    },

    // Grunt notify
    notify_hooks: {
      options: {
        enabled: true,
        max_jshint_notifications: 5,
        title: "Gratis LibSass",
        success: true,
        duration: 50
      }
    } // notify

  });
  require("load-grunt-tasks")(grunt);
  grunt.task.run('notify_hooks');
  grunt.registerTask("default", ["sass", "watch"]);
  // Grunt format command (run before code commit)
  grunt.registerTask("format", ["csscomb", "stripCssComments",  "cssbeautifier"]);
};

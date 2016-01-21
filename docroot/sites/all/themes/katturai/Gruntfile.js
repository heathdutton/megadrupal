module.exports = function(grunt) {

  // Project configuration.
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    uglify: {
      options: {
        banner: '/*! <%= pkg.name %> <%= grunt.template.today("yyyy-mm-dd") %> */\n'
      },
      build: {
        src: 'js/*.js',
        dest: 'js/<%= pkg.name %>.min.js'
      }
    },
    jshint: {
      files: ['Gruntfile.js', 'js/**/*.js'],
      options: {
        // options here to override JSHint defaults
        globals: {
          jQuery: true,
          console: true,
          module: true,
          document: true
        }
      }
    },
    csslint: {
      options: {
        csslintrc: '.csslintrc'
      },
      all: [ 'css/*.css' ]
    },
    compass: {
      dist: {
        options: {
          sassDir: 'sass',
          cssDir: 'css',
          config: 'config.rb'
        }
      }
    },
    shell: {
      all: {
        command: 'drush cc all'
      }
    },
    watch: {
      options: {
        livereload: true
      },
      js: {
        files: ['<%= jshint.files %>'],
        tasks: ['jshint']
      },
      sass: {
        files: ['sass/**/*.scss'],
        tasks: ['compass:dist']
      },
      css: {
        files: ['css/{,**/}*.css'],
        tasks: ['csslint']
      },
      html: {
         files: ['*.html']
      },
      registry: {
        files: ['*.info', '{,**}/*.{php,inc,css}',],
        tasks: ['shell'],
        // options: {
        //   livereload: false
        // }
      },
    }
  });

  // Required plugins.
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-csslint');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-contrib-compass');
  grunt.loadNpmTasks('grunt-shell');

  // Default task(s).
  grunt.registerTask('default', ['watch']);
  // grunt.registerTask('watch', ['uglify'], ['compass:dist']);
};

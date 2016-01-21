'use strict'

module.exports = (grunt) ->
  # Project configuration.
  grunt.initConfig

    # Metadata.
    pkg: grunt.file.readJSON('package.json')
    banner:
      '/**!\n' +
      ' * <%= pkg.title || pkg.name %> - <%= grunt.template.today("yyyy-mm-dd") %>\n' +
      ' * <%= pkg.homepage %>\n' +
      ' * Copyright (c) <%= grunt.template.today("yyyy") %> <%= pkg.author %>\n' +
      ' * License: <%= pkg.license %>\n' +
      ' */\n'


    # Task configuration.
    clean:
      files: ['<%= concat.dist.dest %>']

    concat:
      options:
        banner: '<%= banner %>'

      dist:
        src: ['src/<%= pkg.name %>.js']
        dest: 'js/<%= pkg.name %>.js'

    uglify:
      options:
        banner: '<%= banner %>'

      dist:
        src: '<%= concat.dist.dest %>'
        dest: 'js/<%= pkg.name %>.min.js'

    qunit:
      files: ['test/**/*.html']

    jshint:
      options:
        jshintrc: true

      src:
        src: ['src/{,**/}*.js']

      test:
        src: ['test/{,**/}*.js']

    watch:
      src:
        files: '<%= jshint.src.src %>'
        tasks: [
          'jshint:src'
          'qunit'
        ]

      test:
        files: '<%= jshint.test.src %>'
        tasks: [
          'jshint:test'
          'qunit'
        ]


  # These plugins provide necessary tasks.
  grunt.loadNpmTasks 'grunt-contrib-clean'
  grunt.loadNpmTasks 'grunt-contrib-concat'
  grunt.loadNpmTasks 'grunt-contrib-uglify'
  grunt.loadNpmTasks 'grunt-contrib-qunit'
  grunt.loadNpmTasks 'grunt-contrib-jshint'
  grunt.loadNpmTasks 'grunt-contrib-watch'

  # Default task.
  grunt.registerTask 'default', [
    'jshint'
    'qunit'
    'concat'
    'uglify'
    'clean'
  ]

  # Default task.
  grunt.registerTask 'test', [
    'jshint'
    'qunit'
  ]

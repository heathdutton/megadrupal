README.txt
==========
Uglify2 is a Drupal 7 module that uses Uglify2 (https://github.com/mishoo/UglifyJS2) to optimize your cached JavaScript files.

INSTALLATION & CONFIGURATION
============================
1) Install the module.
2) Go to /admin/settings/performance, enable the service. Enable js aggregation if it's not already enabled.

LIMITATIONS & TROUBLESHOOTING
============================= 
Uglify2 module can operate in 2 different modes as displayed under Preferred Processing Method in settings:
  1) Send JavaScript file contents to the API
  2) Send JavaScript file paths to the API (Requires your site to be publicly accessible)
  Second mode reads the contents of the JavaScript file, sends them to the actual API, parses the response and replaces the compiled
code with the original. If set to use the 3rd mode, or the length of the JavaScript file is over 200K under 2nd mode, the module
sends the public path of the file to the API instead of file content and processes the response. 
This link describes the API methods: http://marijnhaverbeke.nl/uglifyjs

CHANGELOG
==========
7.x-2.0
==================
* Created the sub-module

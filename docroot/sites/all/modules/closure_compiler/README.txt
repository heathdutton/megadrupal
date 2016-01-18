README.txt
==========
Closure Compiler is a Drupal 7 module that provides a framework for other sub-modules to optimize your cached JavaScript files.

INSTALLATION & CONFIGURATION
============================
1) Install the module and one or more sub-modules.
2) Go to /admin/settings/performance, enable the service. Enable js aggregation if it's not already enabled.
3) If you would like to compile locally using Java-based Google Closure Compiler Application or YUI Compressor:
  * Ensure you have Java runtimes installed (version 6 or later required, free download here: http://www.java.com/en/download/)
  * Download the Google Closure Compiler Application (http://closure-compiler.googlecode.com/files/compiler-latest.zip) and 
    unzip the contents under the module directory (closure_compiler). Make sure compiler.jar file is in the same directory as
    closure_compiler.module file.
  * Go to /admin/settings/performance and check local compiling status. The module will let you select the local compiling option only 
    if your environment passes the checks. Select "Compile locally via Java based compiler" option. 

LIMITATIONS & TROUBLESHOOTING
============================= 
  Closure Compiler module looks at the cached JavaScript folder (public://js) and processes files on cron run. It does no optimization
  itself, but calls other modules (e.g. google_closure_compiler) to do the optimization.

CHANGELOG
==========
7.x-2.0 - 7.x-2.1
==================
* Added JSMin-PHP as an option, for those who are not able to run java locally and do not wish to use
  external services

7.x-1.0 - 7.x-2.0
==================
* Split the module into the framework, and a separate google_closure_compiler sub-module
* Add additional sub-modules that use UglifyJS2 and YUI Compressor

6.x-0.x-dev - 7.x-1.0
==================
* Keeps (and links to) the source JS file, in order to preserve licenses
* Added a more detailed status page
* Separated single file compilation from the main cron task, to make it easier to use from other modules
* Added status reporting
* Modularized code
* Fixed display of local closure compiler status
* Neater integration into system_performance_settings form
* Application of coder review module
* Initial port of latest 6.x-0.x-dev version (2011-04-18) to work in Drupal 7

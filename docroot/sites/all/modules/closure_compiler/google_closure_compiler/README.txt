README.txt
==========
Google Closure Compiler is a Drupal 7 module that uses Google's Closure Compiler to optimize your cached JavaScript files.

INSTALLATION & CONFIGURATION
============================
1) Install the module.
2) Go to /admin/settings/performance, enable the service. Enable js aggregation if it's not already enabled.
3) If you would like to compile locally using Java-based Google Closure Compiler Application:
  * Ensure you have Java runtimes installed (version 6 or later required, free download here: http://www.java.com/en/download/)
  * Download the Google Closure Compiler Application (http://closure-compiler.googlecode.com/files/compiler-latest.zip) and 
    unzip the contents under the module directory (closure_compiler/google_closure_compiler). Make sure compiler.jar file is in
    the same directory as the google_closure_compiler.module file.
  * Go to /admin/settings/performance and check local compiling status. The module will let you select the local compiling option only 
    if your environment passes the checks. Select "Compile locally via Java based compiler" option. 

LIMITATIONS & TROUBLESHOOTING
============================= 
Google Closure Compiler module can operate in 3 different modes as displayed under Preferred Processing Method in settings:
  1) Compile locally via Java based compiler
  2) Send JavaScript file contents to the API
  3) Send JavaScript file paths to the API (Requires your site to be publicly accessible by Google)
  First mode requires Java 6+ to be installed, compiler.jar file present in the module directory and PHP shell_exec function to be 
executable in your environment without issues. If running the module under IIS, you might need to give read and execute permissions
on C:\Windows\System32\cmd.exe to the IUSR_<machine_name> user. If your environment fails to pass the checks at any time, the module
sets the preferred method to the 2nd mode. This mode reads the contents of the JavaScript file, sends them to the actual API, parses 
the response and replaces the compiled code with the original. If set to use the 3rd mode, or the length of the JavaScript file is 
over 200K under 2nd mode, the module sends the public path of the file to the API instead of file content and processes the response. 
Here's a good reference link describing API methods: http://code.google.com/closure/compiler/docs/api-tutorial1.html
  Please note that ADVANCED_OPTIMIZATIONS compilation level tends to be very unstable and breaks Drupal JavaScript most of the time. 

CHANGELOG
==========
7.x-2.0
==================
* Created the sub-module by splitting the original closure_compiler module

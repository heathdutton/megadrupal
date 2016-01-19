README.txt
==========
YUI Compressor is a Drupal 7 module that uses Yahoo's YUI Compressor script to optimize your cached JavaScript files.

INSTALLATION & CONFIGURATION
============================
1) Install the module.
2) Go to /admin/settings/performance, enable the service. Enable js aggregation if it's not already enabled.
3) If you would like to compile locally using Java-based Yahoo YUI Compressor Application:
  * Ensure you have Java runtimes installed (free download here: http://www.java.com/en/download/)
  * Download the Yahoo YUI Compressor Application (https://github.com/yui/yuicompressor/downloads or use git) and 
    unzip the contents. Build using `ant`. Place the result as yuicompressor.jar under the module directory (closure_compiler/yui_compressor).
  * Go to /admin/settings/performance and check local compiling status. The module will let you select the local compiling option only 
    if your environment passes the checks. Select "Compile locally via Java based compiler" option. 

LIMITATIONS & TROUBLESHOOTING
============================= 
Yahoo YUI Compressor module can operate in 1 mode as displayed under Preferred Processing Method in settings:
  1) Compile locally via Java based compiler
  First mode requires Java 6+ to be installed, yuicompressor.jar file present in the module directory and PHP shell_exec function to be 
executable in your environment without issues. If running the module under IIS, you might need to give read and execute permissions
on C:\Windows\System32\cmd.exe to the IUSR_<machine_name> user.

CHANGELOG
==========
7.x-2.0
==================
* Created the sub-module

Devel extended logging
----------------------

If you're a "tail -f" kind of debugger, you may find this module useful. It 
adds a number of functions that write to the debug log file:

- ddi(): Indents debug messages based on the depth of the call stack
- dds(): Logs just the attribute/key names of an object/array.
- ddb(): Logs just the function names from the debug_backtrace() function 
         call stack
- ddf(): Logs the name of the current function, indented with ddi()
- ddu(): Logs a message and moves cursor one line up
- ddt(): Writes debug messages with code execution timestamps included

See the function comment blocks for full documentation on usage.


Using the debug functions early during the bootstrap sequence
-------------------------------------------------------------

Normally the debug functions are only available after devel_ext_log has been
included during the bootstrap sequence. If you're logging things in the core,
this may not be early enough. 

To use the debugging function that early, add this to your Drupal's index.php,
just after the define() for DRUPAL_ROOT:

require_once(DRUPAL_ROOT . '/sites/all/modules/devel_ext_log/devel_ext_log.module');

If you use drush, and want to use the debug function, you also need to add an
include to drush code. One place to do so is in DRUSH/includes/bootstrap.inc
_drush_bootstrap_select_drupal_site(), after $drupal_root has been
defined:

require_once($drupal_root . '/sites/all/modules/devel_ext_log/devel_ext_log.module');


Using the debug functions from the SimpleTest framework
-------------------------------------------------------

In order to debug print from SimpleTests, you need to hardcode the target file
name defined in devel_ext_log.module:ddi(). Otherwise the temporary file used
will be the temporary file of the SimpleTest sandbox, and not the one you're
looking at. There are instructions in comments of the mentioned function on
how to do this.
Load JavaScript asynchronously using the most browser compatible method. The
element for each JavaScript file to be loaded asynchronously will be generated
dynamically after the window load event using the following basic method:

(function() {
  var s = document.createElement('script');
  s.type = 'text/javascript';
  s.async = true;
  s.src = script.data;
  var x = document.getElementsByTagName('script')[0];
  x.parentNode.insertBefore(s, x);
})();


### How to load a JavaScript file asynchronously

Using this module, you can specify a script to be loaded asynchronously in two
basic ways:

1. By adding "async_js" => TRUE to the options array in drupal_add_js().
2. By calling async_js_add_js("path/to/your/script.js") which bypasses
drupal_add_js(), gets you the same result, and may be a touch faster.


### Additional functionality

In addition to loading scripts asynchronously, this module includes the
following functionality:

- Dependencies: Specify dependencies at the individual script level. Simply add
the following key/value pair to the options array in either drupal_add_js() or
async_js_add_js().

  "async_dependencies" => array("path/to/script1.js", "path/to/script2.js")

NOTE: The path you use to reference the dependency must correspond exactly to
the path used to actually load the dependency.

- Callback functions: Specify callback functions to be fired once the script 
has loaded. This can be done by adding the following key/value pair to the
options array in either drupal_add_js() or async_js_add_js(). 

  "async_callback" => array("your_function_name", "another_function_name")

NOTE: All callback functions must exist in the global scope.

- Fade-in effect: When specifying a script to be loaded asynchronously, you can
specify html elements to fade in after a delay and in unison. This can be done
by adding the following key/value pair to the options array in either
drupal_add_js() or async_js_add_js().

  "async_fade" => array(".array", "#of", ".jQuery", "#selectors")

The fade-in effect will be applied universally to all elements defined in this
way on a single page. The default delay is 1 second. This can be changed by
editing the conf variable "async_js_timeout".

- Containers: Specify a container in which to append your script element. To do
this, add the following key/value pair to the options array in either
drupal_add_js() or async_js_add_js().

  "async_container" => ".jQuery #selector"

- Final callback: You may add a final callback function to be fired after all
asynchronous scripts have been successfully loaded by editing the
"async_js_final_callback" conf variable. NOTE: Your callback function must
exist in the global scope.


### Similar modules

- Async script shim is a similar, minimally maintained module for Drupal 8.
(https://drupal.org/project/async_script_shim)

- Script.JS uses the $script.js library. (https://drupal.org/project/scriptjs)

- LABjs uses the LABjs library. (https://drupal.org/project/labjs)

- Advanced CSS/JS Aggregation. (https://drupal.org/project/advagg)


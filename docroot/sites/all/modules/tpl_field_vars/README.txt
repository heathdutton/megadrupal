Provides super themer-friendly template variables for entity fields.


Project info at:
http://drupal.org/project/tpl_field_vars

See also:
http://greenash.net.au/thoughts/2012/05/introducing-the-drupal-template-field-variables-module/


Basic usage:

Add the following code to your theme's template.php file, for using
this module with nodes:

/**
 * Preprocessor for node.tpl.php template file.
 */
function YOURTHEME_preprocess_node(&$vars) {
  tpl_field_vars_preprocess($vars, $vars['node'], array(
    'cleanup' => TRUE,
    'debug' => TRUE,
  ));
}

Or this code, for users:

/**
 * Preprocessor for user-profile.tpl.php template file.
 */
function YOURTHEME_preprocess_user_profile(&$vars) {
  tpl_field_vars_preprocess($vars, $vars['elements']['#account'], array(
    'cleanup' => TRUE,
    'debug' => TRUE,
  ));
}

Or this code, for taxonomy terms:

/**
 * Preprocessor for taxonomy-term.tpl.php template file.
 */
function YOURTHEME_preprocess_taxonomy_term(&$vars) {
  tpl_field_vars_preprocess($vars, $vars['term'], array(
    'cleanup' => TRUE,
    'debug' => TRUE,
  ));
}

Then, create a template file for the entity you're working with
(e.g. create a 'node--page.tpl.php' file).

Be sure to clear your cache after adding any of the above code, and
also after creating new template files.

Load the relevant page in your browser (e.g. http://yoursite.local/node/1),
and you should see this module's debug output. The debug output will
tell you all the variables that have been made available in your
template, and the structure of each variable (any given variable may
be a simple string value, or a series of nested arrays).

Add the variables to your template file, as you see fit. When you no
longer need the debug output, change the relevant:

  'debug' => TRUE,

line in template.php to read:

  'debug' => FALSE,

and the debug output will no longer show.

That's about it. Happy theming!


Advanced usage:

Apart from within Drupal's main entity preprocessing functions, you
can also make use of Template Field Variables from anywhere else you
want in your custom code. All you need to do, is call tpl_field_vars()
and provide the entity object in question. All the variables you
need will be in the array returned by this function.

For example, if you have a custom block that needs to output some node
fields, you can call tpl_field_vars() from your block's callback
function, like so:

/**
 * My custom block callback
 */
function MYMODULE_MYBLOCK_CALLBACK() {
  $node = node_load(1);
  
  $vars = tpl_field_vars($node);
  
  $block['content'] = array(
    '#theme' => 'MYMODULE_MYBLOCK',
    '#myfield1' => !empty($vars['myfield1']) ? $vars['myfield1'] : '',
    '#myfield2' => !empty($vars['myfield2']) ? $vars['myfield2'] : '',
  );
  
  return $block;
}

And you can then make use of the passed-in variables in your block's
template.

A block callback is just one example - you can use Template Field
Variables wherever you want. In fact, you could even use it in custom
code that doesn't pass the values to a template at all - you might
simply prefer to work with Template Field Variables values in your
custom code, because it's easier than working with Drupal's nested
field arrays.

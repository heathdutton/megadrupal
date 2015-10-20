<?php
/**
 * @file
 * Hooks that you can implement into your own modules to extend the functionality
 * of the phpembed input filter.
 *
 * Allowing any PHP or Drupal function to be called is extremely dangerous and
 * frankly, not necessary.  All we need are a couple to get some really nice
 * additional functionality into content fields.  We will define a base set of
 * functions here and allow other modules to add their own as they see fit.
 */

/**
 * Implements hook_phpembed_FUNCTION_validate_args(&$args).
 *
 * Required.
 *
 * It's pretty bad practice to simply let end users run PHP code on your site,
 * which is basically what this module lets you do.  In order for a php function
 * to be run, this hook must exist.
 *
 * Don't just create the function and leave it empty.  Ensure the function you
 * want to allow for embedding gets properly sanitized and formatted data.
 */
function hook_phpembed_FUNCTION_validate_args(&$args) {
  foreach ($args as $key => &$arg) {
    switch ($key) {
      // Expects an integer
      case 0:
        $arg = (int)$arg;
        break;

      // Must be alphanumerics only
      case 1:
        $arg = preg_replace('/[^a-z0-9]/i', '', $arg);
        break;
    }
  }

  // Remove any extra args that may have been passed to prevent php errors.
  $args = array_slice($args, 0, 3);
}

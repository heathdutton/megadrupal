<?php
/**
 * @file
 * Debugging functions
 */

$plugin = array (
  'title' => t('Debugging'),
  'description' => t('Library for debugging.'),
  'xu_type' => 'library'
);

/**
 * Print a variable to the 'message' area of the page.
 *
 * @todo Add log level support
 *
 * @param $input
 *  The variable to return
 * @param null $name
 *  An optional name to render.
 * @param bool $die
 *  Whether to die after rendering. Defaults to FALSE.
 */
function xu_debug($input, $name = NULL, $die = FALSE) {
  if (user_access('access xu debugging') || user_access('access devel information')) {
    $export = xu_krumo($input, TRUE, $name, $die);
    drupal_set_message($export);
  }
}

/**
 * Wrapper around xuKrumo.
 *
 * This is based on Devel's kprint_r(), however it ALWAYS calls krumo, even for
 * strings and other values, as dprint_r is less reliable. We also use a modified
 * krumo object (xuKrumo) so we get the right output.
 *
 * This function requires devel to be installed, or the Krumo class, for it to work.
 *
 * @todo Remove the dependency on Devel
 *
 * @param $input
 * @param bool $return
 * @param null $name
 * @param bool $die
 *
 * @return mixed|string
 */
function xu_krumo($input, $return = FALSE, $name = NULL, $die = FALSE) {
  if (user_access('access xu debugging') || user_access('access devel information')) {
    if(class_exists('Krumo')) {
      $output = $return ? (isset($name) ? $name .' => ' : '') . xu_ob('xu_krumo_dump', $input) : xu_krumo_dump($input);
      if ($die == TRUE){
        die('xu called die');
      }
      else {
        return $output;
      }
    }
    else {
      drupal_set_message('xu could not output debug information because Krumo was not available');
    }
  }
}

/**
 * Return something, output buffered
 *
 * This is used by xu to dynamically pass debug information back to a variable
 *
 * @see krumo_ob in the devel module
 *
 * @param $callback
 *  The callback to pass the something into
 * @param $object
 *  The something
 *
 * @return string
 */
function xu_ob($callback, $object) {
  ob_start();
  if (function_exists($callback)) {
    call_user_func($callback, $object);
  }
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

/**
 * Dump a variable via xuKrumo
 *
 * @param $input
 */
function xu_krumo_dump($input) {
  xu_include('xuKrumo.class.php', 'lib');

  $xk = new xuKrumo();
  $xk->dump($input);

  return TRUE;
}

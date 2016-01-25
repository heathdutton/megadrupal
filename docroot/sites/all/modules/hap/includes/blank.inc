<?php

/**
 * Implements hook_hap_info().
 */
function hap_blank_hap_info() {
  return array(
    'name' => 'Blank',
    'description' => 'This is a page distiller utility.',
    'hooks' => array('menu'),
  );
}



/******************************************************************************
 * Drupal hooks
 *****************************************************************************/
/**
 * Implements hook_menu().
 */
function hap_blank_menu() {
  $items['hap/blank'] = array(
    'page callback' => 'hap_blank_page',
    'access arguments' => array('access content'),
    'file' => 'blank.inc',
    'file path' => hap_path(),
    );

  return $items;
}

/**
 * Prints a blank template by bypassing page.tpl.php.
 * This function uses the http GET variables to determine how to operate.
 *
 * @see hap_blank_url().
 *
 * For example: if you want to call:
 * @code
 * theme('my_awesome_theme', array('nid' => 43, 'op' => 'selected'));
 * @endcode
 * Then the request url should be
 * "hap/blank/?callback=theme&vars[hook]=my_awesome_theme&vars[nid]=43&vars[op]=selected".
 * Do not bother with calling the function directly like:
 * "callback=theme_my_awesome_theme", as you would then have to manage the files
 * that need to be included yourself.
 *
 * This function can also simply print the active handler of a given path. For
 * instance loading the page contents of 'node/32' only requires the path to
 * read:
 * "hap/blank/?path=node/32
 *
 * Note that aside from 'callback', 'include', and 'path' the names of the variables is
 * irrelevant. The order of the arguments is however important in respect to
 * the function signature.
 */
function hap_blank_page($arguments = NULL) {
  if (!$arguments) {
    $arguments = drupal_get_query_parameters();
  }

  $page = '';

  if (isset($arguments['include'])) {
    call_user_func_array('module_load_include', $arguments['include']);
    unset($arguments['include']);
  }

  if (isset($arguments['callback'])) {
    $callback = $arguments['callback'];
    unset($arguments['callback']);
    $page = call_user_func_array($callback, $arguments);
  }
  else if($arguments['path']) {
    $page = menu_execute_active_handler($arguments['path'], FALSE);
  }


  // ensure that we're returning a renderable array
  if (is_string($page)) {
    $page = array('#markup' => $page);
  }

  $build = array(
    '#theme_wrappers' => array('html'),
    'page' => array(array('#markup' => theme('status_messages')), $page),
    );

  print render($build);

  // Do not just exit. Rather let Drupal run it's closure functions to
  // complete the request. Some modules depend on this.
  return NULL;
}


/**
 * Builds query parameters and path for hap/blank.
 *
 * @param $callback
 *   The callback to execute. Or an internal drupal path bound for
 *   menu_execute_active_handler(). The callback will only be envoked when the
 *   $arguments is an array.
 *   @see menu_execute_active_handler().
 *
 * @param $arguments
 *   An array of arguments to pass to the callback. Some callbacks require
 *   array argument to be keyed, such as theme().
 *
 * @param $include
 *   An array of of arguments bound for module_load_include().
 *   @see module_load_include().
 *
 * @return
 *   A keyed array of 'callback', 'arguments', or 'path'.
 */
function hap_blank_url($callback, $arguments = NULL, $include = NULL) {
  return array(
    'path' => 'hap/blank',
    'options' => array('query' => hap_blank_arguments($callback, $arguments, $include)));
}

/**
 * Prepares an array for hap_blank_page() arguments. Typically used in
 * hook_menu implementations.
 */
function hap_blank_arguments($callback, $arguments = NULL, $include = NULL) {
  // If arguments are provided
  if (isset($arguments) && is_array($arguments)) {
    $query['callback'] = $callback;

    if ($include) {
      $query['include'] = $include;
    }

    if (!empty($arguments)) {
      $query[] = $arguments;
    }
  }
  else {
    $query['path'] = $callback;
  }

  return $query;
}

<?php

/**
 * @file
 * Contains the 'url path alias' default argument plugin.
 */

/**
 * Default argument plugin to convert a nid to its url path
 */
class views_plugin_argument_default_url_path extends views_plugin_argument_default {
  /**
   * Returns the default argument.
   */
  function get_argument() {
    // By default provide a views default argument of 'all'.
    // If this value is received, the filter will be ignored; i.e, "all values".
    $return = 'all';

    // Grab the last value from the url.
    $arg = check_plain(array_pop(arg()));

    // Is it already the nid?
    if (ctype_digit($arg)) {
      $return = $arg;
    }
    // Is it a path alias?
    elseif ($nid = drupal_lookup_path('source', $arg)) {
      $return = $nid;
    }
    return $return;
  }
}

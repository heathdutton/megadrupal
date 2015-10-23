<?php

/**
 * @file Views Plugin class implementation.
 */

/**
 * Views Argument plugin to return the current path as default argument.
 */
class views_menu_reference_plugin_argument_current_path extends views_plugin_argument_default {

  function option_definition() {
    return parent::option_definition();
  }

  function get_argument() {
    // Return the current path (/node/1235, ...)
    return current_path();
  }

}

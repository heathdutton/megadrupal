<?php

/**
 * @file
 * Handler for default argument value for Node Registration ID from URL.
 */

class views_handler_argument_default_node_registration extends views_plugin_argument_default {
  function get_argument() {
    $registration = menu_get_object('node_registration');
    if ($registration) {
      return $registration->registration_id;
    }
  }
}

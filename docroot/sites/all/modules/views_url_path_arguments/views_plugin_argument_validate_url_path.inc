<?php

/**
 * @file
 * Contains the 'url path alias' argument validator plugin.
 */

/**
 * Validate whether an argument is an acceptable node.
 */
class views_plugin_argument_validate_url_path extends views_plugin_argument_validate {
  /**
   * Perform the validation of an argument.
   */
  function validate_argument($argument) {
    // Default to returning false.
    $return = FALSE;

    // Is it the views 'all' argument value?
    if ($argument == 'all') {
      $return = TRUE;
    }
    // Is it already a nid?
    elseif (is_numeric($argument)) {
      // Node access isn't technically necessary for a single nid as views
      // permissions would not allow any data exposure.
      // That said, node access gives an expected experience as this validate
      // plugin will return FALSE if they don't have access.
      $result = db_select('node', 'n')
        ->addTag('node_access')
        ->condition('nid', $argument, '=')
        ->countQuery()
        ->execute()
        ->fetchField();

      if ($result->fetchCol()) {
        $return = TRUE;
      }
    }
    // Is it a path alias?
    elseif ($path = drupal_lookup_path('source', $argument)) {
      // Honor node_access so we don't inadvertently disclose the node title.
      $nid = substr($path, 5);
      $title = db_select('node', 'n')
        ->addTag('node_access')
        ->condition('nid', $nid, '=')
        ->fields('n', array('title'))
        ->execute()
        ->fetchField();
      if ($title) {
        $return = TRUE;

        // Convert the path alias to a nid.
        $this->argument->argument = $nid;

        // Save the title() handlers some work.
        $this->argument->validated_title = check_plain($title);
      }
    }

    return $return;
  }
}

<?php

/**
 * Shows the similarity of the node.
 *
 * @ingroup views_filter_handlers
 *
 */
class views_related_content_handler_filter_author extends views_handler {
  function options_validate(&$form, &$form_state) {
  }

  function get_value_options() {
  }

  function query() {
    if (arg(0) == 'node' && is_numeric(arg(1))) {
      $nodeid = arg(1);
    }

    if (isset($nodeid)) {
      $result = db_select('node', 'n')
        ->fields('n')
        ->condition('nid', $nodeid,'=');

      $result = $result->execute();
      return $result->fetchAssoc();
    }
  }
}

<?php

/**
 * @file
 * Definition of og_simplestats_handler_field_node_count.
 */

/**
 * Field handler to display the total number of posts (nodes) in a group.
 */
class og_simplestats_handler_field_node_count extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
  }
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  function render($values) {
    $nid = $this->get_value($values, 'nid');
    return og_simplestats_get_stats($nid);
  }
}

<?php

/**
 * @file
 * Views handler for Match module
 */

class match_handler_field_match_title extends views_handler_field_custom {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function pre_render($values) {
    $nids = array();
    foreach ($values as $row) {
      $nids[] = $row->{$this->aliases['nid']};
    }

    if (!empty($nids)) {
      $this->nodes = node_load_multiple($nids);
    }
  }

  function render($values) {
    if (isset($this->nodes[$values->{$this->aliases['nid']}])) {
      $node = $this->nodes[$values->{$this->aliases['nid']}];
      $title = match_title($node, TRUE);
      return l($title, 'node/' . $node->nid);
    }
  }
}

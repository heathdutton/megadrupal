<?php

class role_indicators_plugin_row extends views_plugin_row {
  // Basic properties that let the row style follow relationships.
  var $base_table = 'taxonomy_term_data';
  var $base_field = 'tid';

  // Stores the terms loaded with pre_render.
  var $terms = array();


  function pre_render($values) {
    $nids = array();
    foreach ($values as $row) {
      $tids[] = $row->{$this->field_alias};
    }
    $this->terms = taxonomy_term_load_multiple($tids);
  }

  function render($row) {
    $row->term = $this->terms[$row->{$this->field_alias}];
    return parent::render($row);
  }
}

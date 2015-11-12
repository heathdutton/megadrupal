<?php

/**
 * @file
 * Field handler for sense data.
 */

/**
 * Field handler for pull info off the sense data array.
 */
class lingwo_senses_views_handler_field_data extends views_handler_field {
  var $data_property = NULL;

  function construct() {
    parent::construct();

    $this->additional_fields['data'] = array('table' => 'lingwo_senses_revisions', 'field' => 'data');

    if (!empty($this->definition['data property'])) {
      $this->data_property = $this->definition['data property'];
    }
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['data'];
  }

  function render($values) {
    $data = unserialize($values->{$this->field_alias});
    $parts = explode('][', $this->data_property);

    foreach ($parts as $part) {
      if (is_array($data) && isset($data[$part])) {
        $data = $data[$part];
      }
      else {
        $data = NULL;
        break;
      }
    }

    if (!empty($data)) {
      if (is_array($data)) {
        $data = implode(', ', $data);
      }
      // TODO: should we convert '\n' to '<br />' for examples
      return check_plain($data);
    }
  }
}


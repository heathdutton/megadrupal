<?php

/**
 *
 * @ingroup views_filter_handlers
 */
class views_related_content_handler_filter_field_reference extends views_handler_filter_in_operator {
  function options_validate(&$form, &$form_state) {
  }

  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    
    $listNodeReference = $this->_get_entityreferences_settings();
    foreach ($listNodeReference as $key => $value) {
      $this->value_options[$key] = $value;
    }

    return $this->value_options;
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


  function _get_entityreferences_settings() {
    $child_ids = $field_names = $settings = array();

    // Get all entityreference field names
    $query = db_select('field_config', 'f');
    $query->fields('f', array('field_name'));
    $query->condition('f.type', 'entityreference');
    $query->distinct();
    $rows = $query->execute();

    foreach ($rows as $row) {
      $field_name = $row->field_name;
      $field_names[$field_name] = $this->_field_alias($field_name);
    }

    return $field_names;
  }

  function _field_alias($machine_name) {
    $temp = str_replace('_', ' ', $machine_name);
    $temp = ucwords(strtolower($temp));
    return $temp;
  }
}

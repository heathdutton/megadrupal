<?php

/**
 *
 * @ingroup views_filter_handlers
 */
class views_related_content_handler_filter_term extends views_handler_filter_in_operator {
  function options_validate(&$form, &$form_state) {
  }

  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $listTaxonomy = taxonomy_get_vocabularies();
    foreach ($listTaxonomy as $key => $value) {
      $this->value_options[$value->machine_name] = $value->name;
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
}

<?php

/**
 * Field handler to present unsoft delete a node.
 */
class entity_soft_delete_handler_link_undelete extends views_handler_field_node_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
  }

  function access() {
    return user_access('undelete soft deleted content');
  }

  function render_link($data, $values) {
    $function_history = array();
    cf_error_append_history($function_history, __FUNCTION__);

    if (!entity_soft_delete_can_undelete($values, NULL, $function_history)) {
      return;
    }

    $results = entity_soft_delete_get_entity_bundle_settings(array($values), $function_history);

    if (count($results) == 0){
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = 'node/' . $values->nid . '/undelete';
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t("undelete");

    return $text;
  }
}

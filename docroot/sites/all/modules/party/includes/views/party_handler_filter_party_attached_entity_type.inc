<?php
/**
 * @file Filter by attached entity type.
 */

class party_handler_filter_party_attached_entity_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Attached entity types');
      $entity_types = entity_get_info();
      foreach ($entity_types as $type => $info) {
        // Entities with a CRM controller class have data sets.
        if (isset($info['crm controller class'])) {
          $options[$type] = $info['label'];
        }
      }
      asort($options);
      $this->value_options = $options;
    }
  }
}

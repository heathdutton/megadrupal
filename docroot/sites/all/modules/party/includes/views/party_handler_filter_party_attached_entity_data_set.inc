<?php
/**
 * @file Filter by attached entity data set.
 */

class party_handler_filter_party_attached_entity_data_set extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Attached entity data sets');
      $data_sets = party_get_data_set_info();

      foreach ($data_sets as $data_set => $info) {
        $options[$data_set] = $info['label'];
      }
      asort($options);
      $this->value_options = $options;
    }
  }
}

<?php

/**
 * @file
 * Definition of views_handler_filter_entity_gallery_type.
 */

/**
 * Filter by entity gallery type.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_entity_gallery_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Gallery types');
      $types = entity_gallery_type_get_types();
      $options = array();
      foreach ($types as $type => $info) {
        $options[$type] = t($info->name);
      }
      asort($options);
      $this->value_options = $options;
    }
  }
}

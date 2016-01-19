<?php
/**
 * @file
 * Provide handler to show resources.
 */

class views_handler_filter_by_resources extends views_handler_filter_in_operator  {
  function get_value_options() {
    if (!isset($this->value_options)) {
      //get the node type that holds the resources for instances of this field type
      //we don't know for which instance this view is being shown. so show data for all resource typs
      $field_name = str_replace('_nid', '', $this->field);
      $result = db_select('field_config_instance', 'f')->fields('f', array('data'))
        ->condition('field_name', $field_name)->execute();
      $resoruces_ct = array();
      foreach ($result as $record) {
        $data = unserialize($record->data);
        $resources_ct[] = $data['widget']['settings']['resource_booking_node'];
      }
      if (empty($resources_ct)) {
        $options[] = t('No resource type found');
      }
      else {
        $options = db_select('node', 'n')->fields('n', array('nid', 'title'))
          ->condition('status', 1)->condition('type', $resources_ct, 'IN')
          ->orderby('type')
          ->execute()->fetchAllKeyed();
      }
      $this->value_options = $options;
    }
  }
}

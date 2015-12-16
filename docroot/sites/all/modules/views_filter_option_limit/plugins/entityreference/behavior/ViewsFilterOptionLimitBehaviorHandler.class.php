<?php

/**
 * Entityreference behaviour handler class for views option limit.
 *
 * If enabled, provides a second filter for the field using our Views handler.
 */
class ViewsFilterOptionLimitBehaviorHandler extends EntityReference_BehaviorHandler_Abstract {

  public function views_data_alter(&$data, $field) {
    $entity_info = entity_get_info($field['settings']['target_type']);
    $field_name = $field['field_name'] . '_target_id';
    foreach ($data as $table_name => &$table_data) {
      if (isset($table_data[$field_name])) {
        // This doesn't clash with reference_option_limit module, because that
        // never got round to getting support for Views with entityreference
        // fields.
        $limit_option_pseudo_field_name = $field_name . '_option_limit';

        $data[$table_name][$limit_option_pseudo_field_name] = array();

        // Copy properties from the original.
        foreach (array('group', 'title', 'title short', 'help') as $key) {
          $data[$table_name][$limit_option_pseudo_field_name][$key] = $table_data[$field_name][$key];
        }

        $data[$table_name][$limit_option_pseudo_field_name]['title'] .= ' ' . t('(with restricted options)');

        $data[$table_name][$limit_option_pseudo_field_name]['filter'] = array();

        // Copy properties from the original filter definition.
        foreach (array('field', 'table', 'field_name', 'additional fields') as $key) {
          $data[$table_name][$limit_option_pseudo_field_name]['filter'][$key] = $table_data[$field_name]['filter'][$key];
        }

        $data[$table_name][$limit_option_pseudo_field_name]['filter']['handler'] = 'views_filter_option_limit_handler_filter_options_limit';
      }
    }
  }
}

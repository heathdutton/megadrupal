<?php
/**
 * @file at_cm.hooks.inc
 */

/**
 * Implements hook_init()
 */
function at_cm_init() {
  if ($api_key = variable_get('at_cm.api_key', '')) {
    at_container('container')->offsetSet('cm.null', '');
    at_container('container')->offsetSet('cm.auth', $api_key);
  }
}

/**
 * Implements hook_field_info()
 */
function at_cm_field_info() {
  return array(
    'at_cm_list' => array(
      'label' => t('CampaignMonitor List'),
      'description' => t('This field stores varchar text in the database.'),
      'settings' => array('max_length' => 255),
      'instance_settings' => array('text_processing' => 0),
      'default_widget' => 'text_textfield',
      'default_formatter' => 'text_default',
    ),
  );
}

/**
 * Implements hook_field_validate().
 */
function at_cm_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    $list_id = reset($item);
    if (!empty($list_id)) {
      if (!at_cm_list_is_valid(reset($item))) {
          $errors[$field['field_name']][$langcode][$delta][] = array(
            'error' => "at_cm_list_id_invalid",
            'message' => t('Invalid list ID'),
          );
      }
    }
  }
}

/**
 * Implements hook_field_load()
 */
function at_cm_field_load($entity_type, $entities, $field, $instances, $langcode, &$items) {
  return text_field_load($entity_type, $entities, $field, $instances, $langcode, $items);
}

/**
 * Implements hook_field_settings_form().
 */
function at_cm_field_settings_form($field, $instance, $has_data) {
  return text_field_settings_form($field, $instance, $has_data);
}

/**
 * Implements hook_field_is_empty().
 */
function at_cm_field_is_empty($item, $field) {
  return text_field_is_empty($item, $field);
}

/**
 * Implements hook_options_list().
 */
function at_cm_options_list($field, $instance, $entity_type, $entity) {
  return at_cm_lists_options();
}

/**
 * Implements hook_field_widget_info_alter().
 */
function at_cm_field_widget_info_alter(&$info) {
  $widgets = array('options_select', 'options_buttons');
  foreach ($widgets as $widget) {
    $info[$widget]['field types'] = array_merge($info[$widget]['field types'], array('at_cm_list'));
  }
}

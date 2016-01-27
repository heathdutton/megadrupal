<?php
/**
 * Implements hook_field_info().
 */
function flag_entity_field_info() {
  return array(
    'flag_entity_fields_flag' => array(
      'label' => t('Flag it'),
      'description' => t("This field stores entity flaged status"),
      'settings' => array('flagable_type' => ''),
      'instance_settings' => array(),
      'default_widget' => 'options_select',
      'default_formatter' => 'flag_entity_fields_flag',
    )
  );
}

/**
 * Implements hook_field_validate().
 * @todo: validate
 */
function flag_entity_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  
}

/**
 * Implements hook_field_is_empty().
 */
function flag_entity_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function flag_entity_field_widget_info() {
  return array(
    'flag_entity_fields_flag' => array(
      'label' => t('Flag'),
      'field types' => array('flag_entity_fields_flag'),
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function flag_entity_field_settings_form($field, $instance, $has_data) {
  global $user;
  // Get proper values for 'allowed_values_function', which is a core setting.
  $flags = flag_get_flags($instance['entity_type'], $instance['bundle'], $user);
  dsm($flags);
  $options = array();
  foreach ($flags as $type => $flag) {
    if ($flag->show_on_form) {
      $options += array($type => $flag->title);
    }
  }
  $form['flagable_type'] = array(
    '#type' => 'select',
    '#title' => t('Flagable type'),
    '#default_value' => '',
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t('The flagable type which supplies the options for this field.'),
    '#disabled' => $has_data,
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function flag_entity_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  global $user;
  $flags = flag_get_flags($element['#entity_type'], $element['#bundle'], $user);
  $options = array();
  foreach ($flags as $type => $flag) {
    if ($flag->show_on_form) {
      $options += array($type => $flag->title);
    }
  }
  $element = $element + array(
    'flagable' => array(
      '#type' => 'value',
      '#value' => 1,
    )
  );

  return $element;
}

/**
 * Implements hook_field_widget_info_alter().
 */
function flag_entity_field_widget_info_alter(&$info) {
  $info['options_select']['field types'][] = 'flag_entity_fields_flag';
  $info['options_buttons']['field types'][] = 'flag_entity_fields_flag';
}

/**
 * Implements hook_options_list().
 */
function flag_entity_options_list($field) {
  $function = !empty($field['settings']['options_list_callback']) ? $field['settings']['options_list_callback'] : 'flag_entity_allowed_values';
  return $function($field);
}

/**
 * Returns the set of valid terms for a taxonomy field.
 *
 * @param $field
 *   The field definition.
 * @return
 *   The array of valid terms for this field, keyed by term id.
 */
function flag_entity_allowed_values($field) {
  $options = array();
  foreach ($field['settings'] as $tree) {
    
  }
  return $options;
}

/**
 * Implements hook_field_presave().
 */
function flag_entity_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($field['type'] == 'flag_entity_fields_flag') {
    
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function flag_entity_field_formatter_info() {
  return array(
    'flag_entity_fields_flag' => array(
      'label' => t('Flag'),
      'field types' => array('flag_entity_fields_flag'),
    )
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function flag_entity_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];
  $links = flag_link($entity_type, $entity);
  switch ($display['type']) {
    case 'flag_entity_fields_flag':
      if ($links) {
        foreach ($links as $type => $link) {
          $element[] = array(
            '#theme' => 'links',
            '#links' => $links,
            '#attributes' => array('class' => array('links', 'inline', $type))
          );
        }
      }
      else {
        $links = array(
          'flag-wishlist' => array(
            'title' => '<input class="bnt_add-wishlist" type="button" value="" />',
            'html' => TRUE,
          ),
        );
        $element[] = array(
          '#theme' => 'links',
          '#links' => $links,
          '#attributes' => array('class' => array('links', 'inline', 'not-logged-in', 'disabled'))
        );
      }
      break;
  }
  return $element;
}

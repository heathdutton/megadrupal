<?php

/**
 * @file
 * Defines and handles the fields required by the Group module.
 */

// Fields type defines.
define('GCC_THEME_FIELD_TYPE_THEME', 'gcc_theme');

// Fields name defines.
define('GCC_THEME_FIELD_THEME', 'field_gcc_theme');

/* Declaration and Management of the fields required by Group Core */

/**
 * Implements hook_field_info().
 */
function gcc_theme_field_info() {

  return array(

    GCC_THEME_FIELD_TYPE_THEME => array(

      'label' => t('Choose a custom theme'),
      'description' => t('This field allow to assigne a custom theme to the group.'),
      'default_widget' => 'gcc_theme_select_widget',
      'default_formatter' => 'gcc_theme_hidden_formatter',
      'no_ui' => TRUE,
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function gcc_theme_field_is_empty($item, $field) {

  switch ($field['type']) {

    case GCC_THEME_FIELD_TYPE_THEME:
      if (isset($item['value']) && !empty($item['value']) && $item['value'] != '__none') {
        return FALSE;
      }
      break;
  }

  return TRUE;
}

/* Widgets */

/**
 * Implements hook_field_widget_info().
 */
function gcc_theme_field_widget_info() {

  return array(

    'gcc_theme_select_widget' => array(

      'label' => t('Select List'),
      'field types' => array(GCC_THEME_FIELD_TYPE_THEME),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function gcc_theme_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  switch ($instance['widget']['type']) {

    case 'gcc_theme_select_widget':
      $themes = list_themes();
      module_load_include('inc', 'system', 'system.admin');
      uasort($themes, 'system_sort_modules_by_info_name');

      $options = array('__none' => t('Use sidewide theme'));
      foreach ($themes as $key => $value) {
        if ($value->status) {
          $options[$key] = check_plain($value->info['name']);
        }
      }

      $element += array(

        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : '',
      );

      $element = array('value' => $element);
      break;
  }

  return $element;
}

/* Formatters */

/**
 * Implements hook_field_formatter_info().
 */
function gcc_theme_field_formatter_info() {

  return array(

    'gcc_theme_hidden_formatter' => array(

      'label' => t('Hidden'),
      'field types' => array(GCC_THEME_FIELD_TYPE_THEME),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function gcc_theme_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  switch ($display['type']) {

    case 'gcc_theme_hidden_formatter':
      break;
  }

  return $element;
}

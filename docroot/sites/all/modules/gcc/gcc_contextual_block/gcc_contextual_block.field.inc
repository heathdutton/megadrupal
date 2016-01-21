<?php

/**
 * @file
 * Defines and handles the fields required by the Group Contextual Block module.
 */

// Fields type defines.
define('GCC_CONTEXTUAL_BLOCK_FIELD_TYPE', 'gcc_contextual_block');

// Fields name defines.
define('GCC_CONTEXTUAL_BLOCK_FIELD', 'field_gcc_contextual_block');

/* Declaration and Management of the fields required by Group Contextual Block */

/**
 * Implements hook_field_info().
 */
function gcc_contextual_block_field_info() {

  return array(

    GCC_CONTEXTUAL_BLOCK_FIELD_TYPE => array(

      'label' => t('Choose a custom block layout'),
      'description' => t('This field allow to configure a custom block layout to the group.'),
      'default_widget' => 'gcc_contextual_block_hidden_widget',
      'default_formatter' => 'gcc_contextual_block_hidden_formatter',
      'no_ui' => TRUE,
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function gcc_contextual_block_field_is_empty($item, $field) {

  switch ($field['type']) {

    case GCC_CONTEXTUAL_BLOCK_FIELD_TYPE:
      if (isset($item['config']) && !empty($item['config'])) {
        return FALSE;
      }
      break;
  }

  return TRUE;
}

/**
 * Implements hookf_field_presave().
 */
function gcc_contextual_block_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {

  switch ($field['type']) {

    case GCC_CONTEXTUAL_BLOCK_FIELD_TYPE:
      foreach ($items as &$item) {

        if (!isset($item['data'])) {
          $item['data'] = '';
        }

        if (isset($item['config'])) {
          $item['data'] = serialize($item['config']);
        }
      }
      break;
  }
}

/**
 * Implements hook_field_load().
 */
function gcc_contextual_block_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {

  switch ($field['type']) {

    case GCC_CONTEXTUAL_BLOCK_FIELD_TYPE:
      foreach ($entities as $id => $entity) {
        foreach ($items[$id] as &$item) {

          if (isset($item['data'])) {
            $item['config'] = unserialize($item['data']);
          }
        }
      }
      break;
  }
}

/* Widgets */

/**
 * Implements hook_field_widget_info().
 */
function gcc_contextual_block_field_widget_info() {

  return array(

    'gcc_contextual_block_hidden_widget' => array(

      'label' => t('Hidden'),
      'field types' => array(GCC_CONTEXTUAL_BLOCK_FIELD_TYPE),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function gcc_contextual_block_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  return $element;
}

/* Formatters */

/**
 * Implements hook_field_formatter_info().
 */
function gcc_contextual_block_field_formatter_info() {

  return array(

    'gcc_contextual_block_hidden_formatter' => array(

      'label' => t('Hidden'),
      'field types' => array(GCC_CONTEXTUAL_BLOCK_FIELD_TYPE),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function gcc_contextual_block_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  switch ($display['type']) {

    case 'gcc_contextual_block_hidden_formatter':
      break;
  }

  return $element;
}

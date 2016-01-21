<?php

/**
 * @file
 * Defines and handles the fields required by the Group module.
 */

// Fields type defines.
define('GCC_NODE_ACCESS_FIELD_TYPE_PRIVATE', 'gcc_private');

// Fields name defines.
define('GCC_NODE_ACCESS_FIELD_PRIVATE', 'field_gcc_private');

/* Declaration and Management of the fields required by Group Core */

/**
 * Implements hook_field_info().
 */
function gcc_node_access_field_info() {

  return array(

    GCC_NODE_ACCESS_FIELD_TYPE_PRIVATE => array(

      'label' => t('Private content'),
      'description' => t('This field allow to make a content private.'),
      'default_widget' => 'gcc_node_access_select_widget',
      'default_formatter' => 'gcc_node_access_hidden_formatter',
      'no_ui' => TRUE,
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function gcc_node_access_field_is_empty($item, $field) {

  switch ($field['type']) {

    case GCC_NODE_ACCESS_FIELD_TYPE_PRIVATE:
      if (isset($item['value']) && !empty($item['value'])) {
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
function gcc_node_access_field_widget_info() {

  return array(

    'gcc_node_access_select_widget' => array(

      'label' => t('Select List'),
      'field types' => array(GCC_NODE_ACCESS_FIELD_TYPE_PRIVATE),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function gcc_node_access_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  switch ($instance['widget']['type']) {

    case 'gcc_node_access_select_widget':
      $element += array(

        '#type' => 'select',
        '#options' => array(
          GCC_NODE_ACCESS_PUBLIC => t('Public'),
          GCC_NODE_ACCESS_PRIVATE => t('Private'),
        ),
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
function gcc_node_access_field_formatter_info() {

  return array(

    'gcc_node_access_hidden_formatter' => array(

      'label' => t('Hidden'),
      'field types' => array(GCC_NODE_ACCESS_FIELD_TYPE_PRIVATE),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function gcc_node_access_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  switch ($display['type']) {

    case 'gcc_node_access_hidden_formatter':
      break;
  }

  return $element;
}

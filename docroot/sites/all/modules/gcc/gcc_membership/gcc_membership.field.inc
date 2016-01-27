<?php

/**
 * @file
 * Defines and handles the fields required by the Group module.
 */

// Fields type defines.
define('GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_GROUP', 'gcc_membership_group');
define('GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_TYPE', 'gcc_membership_type');

// Fields name defines.
define('GCC_MEMBERSHIP_FIELD_MEMBERSHIP_GROUP', 'field_gcc_membership_group');
define('GCC_MEMBERSHIP_FIELD_MEMBERSHIP_TYPE', 'field_gcc_membership_type');

/* Declaration and Management of the fields required by Group Core */

/**
 * Implements hook_field_info().
 */
function gcc_membership_field_info() {

  return array(

    GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_GROUP => array(

      'label' => t('Use as Group Membership Information Set'),
      'description' => t('This field allow to transform an entity into a group Membership Information Set.'),
      'default_widget' => 'gcc_membership_hidden_widget',
      'default_formatter' => 'gcc_membership_hidden_formatter',
      'no_ui' => TRUE,
    ),
    GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_TYPE => array(

      'label' => t('Membership Informations Set'),
      'description' => t('This field allow to choose what type of membership information set to use.'),
      'default_widget' => 'gcc_membership_membership_type_widget',
      'default_formatter' => 'gcc_membership_membership_type_formatter',
      'no_ui' => TRUE,
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function gcc_membership_field_is_empty($item, $field) {

  switch ($field['type']) {

    case GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_GROUP:
      if (isset($item['entity_type'], $item['entity_id']) && !empty($item['entity_id'])) {
        return FALSE;
      }
      break;

    case GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_TYPE:
      if (isset($item)) {

        list($entity_type, $bundle) = explode(':', $item) + array(NULL, NULL);
        if (is_string($entity_type) && is_string($bundle)) {
          return FALSE;
        }
      }
      break;
  }

  return TRUE;
}

/**
 * Implements hook_field_presave().
 */
function gcc_membership_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {

  switch ($field['type']) {

    case GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_TYPE:
      foreach ($items as &$item) {

        if (!is_array($item)) {

          list($entity_type, $bundle) = explode(':', $item) + array(NULL, NULL);
          $item = array();
          $item['entity_type'] = $entity_type;
          $item['bundle'] = $bundle;
        }
      }
      break;
  }
}

/* Widgets */

/**
 * Implements hook_field_widget_info().
 */
function gcc_membership_field_widget_info() {

  return array(

    'gcc_membership_hidden_widget' => array(

      'label' => t('Hidden'),
      'field types' => array(GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_GROUP),
    ),

    'gcc_membership_membership_type_widget' => array(

      'label' => t('Select List'),
      'field types' => array(GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_TYPE),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function gcc_membership_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  switch ($instance['widget']['type']) {

    case 'gcc_membership_hidden_widget':
      // DO NOTHING.
      break;

    case 'gcc_membership_membership_type_widget':
      $list = gcc_membership_get_membership_type();

      $options = array();
      foreach ($list as $type) {
        $options[$type['entity_type'] . ':' . $type['bundle']] = $type['label'];
      }

      $element += array(

        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($items[$delta]['entity_type'], $items[$delta]['bundle']) ? $items[$delta]['entity_type'] . ':' . $items[$delta]['bundle'] : '',
      );
      break;
  }

  return $element;
}

/* Formatters */

/**
 * Implements hook_field_formatter_info().
 */
function gcc_membership_field_formatter_info() {

  return array(

    'gcc_membership_hidden_formatter' => array(

      'label' => t('Hidden'),
      'field types' => array(GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_GROUP),
    ),

    'gcc_membership_membership_type_formatter' => array(

      'label' => t('Default'),
      'field types' => array(GCC_MEMBERSHIP_FIELD_TYPE_MEMBERSHIP_TYPE),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function gcc_membership_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  switch ($display['type']) {

    case 'gcc_membership_hidden_formatter':
      break;

    case 'gcc_membership_membership_type_formatter':
      break;
  }

  return $element;
}

<?php

/**
 * @file
 * Defines and handles the fields required by the Group module.
 */

// Fields type defines.
define('GCC_MENU_FIELD_TYPE_MENU', 'gcc_menu');

// Fields name defines.
define('GCC_MENU_FIELD_MENU', 'field_gcc_menu');

/* Declaration and Management of the fields required by Group Core */

/**
 * Implements hook_field_info().
 */
function gcc_menu_field_info() {

  return array(

    GCC_MENU_FIELD_TYPE_MENU => array(

      'label' => t('Create menus'),
      'description' => t('This field allow to create menu in the group.'),
      'default_widget' => 'gcc_menu_widget',
      'default_formatter' => 'gcc_menu_hidden_formatter',
      'no_ui' => TRUE,
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function gcc_menu_field_is_empty($item, $field) {

  switch ($field['type']) {

    case GCC_MENU_FIELD_TYPE_MENU:
      return FALSE;
  }

  return TRUE;
}

/**
 * Implements hook_field_delete().
 */
function gcc_menu_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {

  switch ($field['type']) {

    case GCC_MENU_FIELD_TYPE_MENU:
      foreach ($items as $item) {
        menu_delete($item);
      }
      break;
  }
}

/**
 * Implements hook_field_update().
 */
function gcc_menu_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  switch ($field['type']) {

    case GCC_MENU_FIELD_TYPE_MENU:
      $current_menus = array();
      foreach ($items as $item) {
        $current_menus[] = $item['menu_name'];
      }

      // Compare the original field values with the ones that are being saved.
      // Use $entity->original to check this when possible.
      // But if it isn't available,
      // create a bare-bones entity and load its previous values instead.
      if (isset($entity->original)) {
        $original = $entity->original;
      }
      else {
        $original = entity_create_stub_entity($entity_type, array(
          $id,
          $vid,
          $bundle,
        ));
        field_attach_load($entity_type, array($id => $original), FIELD_LOAD_CURRENT, array('field_id' => $field['id']));
      }

      if (!empty($original->{$field['field_name']}[$langcode])) {
        foreach ($original->{$field['field_name']}[$langcode] as $original_item) {
          if (isset($original_item['menu_name']) && !in_array($original_item['menu_name'], $current_menus)) {
            menu_delete($original_item);
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
function gcc_menu_field_widget_info() {

  return array(

    'gcc_menu_widget' => array(

      'label' => t('Menu widget'),
      'field types' => array(GCC_MENU_FIELD_TYPE_MENU),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function gcc_menu_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  switch ($instance['widget']['type']) {

    case 'gcc_menu_widget':
      // DO NOTHING.
      break;
  }

  return $element;
}

/* Formatters */

/**
 * Implements hook_field_formatter_info().
 */
function gcc_menu_field_formatter_info() {

  return array(

    'gcc_menu_hidden_formatter' => array(

      'label' => t('Hidden'),
      'field types' => array(GCC_MENU_FIELD_TYPE_MENU),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function gcc_menu_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  switch ($display['type']) {

    case 'gcc_menu_hidden_formatter':
      break;
  }

  return $element;
}

<?php

function _commerce_product_dimensions_get_type($id) {
  return 'product_with_dimensions_' . $id;
}

function _commerce_product_dimensions_get_id($type) {
  return (int) substr($type, strlen('product_with_dimensions_'));
}

function commerce_product_dimensions_line_item_type_form($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['mode'] = array(
    '#type' => 'select',
    '#title' => t('Dimensions'),
    '#options' => dimension_get_modes_select(),
    '#default_value' => DIMENSION_MODE_LENGTH,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function commerce_product_dimensions_line_item_type_form_submit($form, &$form_state) {
  $line_item_type = array(
    'name' => $form_state['values']['name'],
    'mode' => $form_state['values']['mode'],
  );
  if (drupal_write_record('commerce_product_dimensions_line_item_type', $line_item_type)) {
    drupal_set_message(t('Line item type %name created.', array('%name' => $line_item_type['name'])));

    $line_item_type['type'] = _commerce_product_dimensions_get_type($line_item_type['id']);
    commerce_line_item_types_reset();

    // Notify the field API that a new bundle has been created.
    field_attach_create_bundle('commerce_line_item', $line_item_type['type']);

    // Load the full line item type array.
    $line_item_type = commerce_line_item_type_load($line_item_type['type']);

    // Configure the new line item type with default fields.
    commerce_line_item_configure_line_item_type($line_item_type);
    variable_set('menu_rebuild_needed', TRUE);
  }
  $form_state['redirect'] = 'admin/commerce/config/line-items';
}

/**
 * Implements hook_commerce_line_item_type_info().
 */
function commerce_product_dimensions_commerce_line_item_type_info() {
  $line_item_types = array();

  $records = db_select('commerce_product_dimensions_line_item_type', 'l')
    ->fields('l')
    ->execute()
    ->fetchAll();

  foreach ($records as $record) {
    $type = _commerce_product_dimensions_get_type($record->id);
    $line_item_types[$type] = array(
      'type' => $type,
      'name' => $record->name,
      // 'description' => t('References a product with length dimension and displays it with the SKU as the label.'),
      'product' => TRUE,
      'add_form_submit_value' => t('Add product'),
      'base' => 'commerce_product_line_item',
      'callbacks' => array(
        'configuration' => 'commerce_product_dimensions_line_item_configuration',
      ),
      'dimension_mode' => $record->mode,
    );
  }

  return $line_item_types;
}

function commerce_product_dimensions_line_item_configuration($line_item_type) {
  $type = $line_item_type['type'];
  $name = $line_item_type['name'];
  $mode = $line_item_type['dimension_mode'];

  // Create the product reference field for the line item type.
  commerce_product_reference_create_instance('commerce_product', 'commerce_line_item', $type, $name);

  // Create basic fields
  $field_defs = array(
    'commerce_display_path' => array(
      'field' => array(
        'type' => 'text',
        'cardinality' => 1,
        'entity_types' => array('commerce_line_item'),
        'translatable' => FALSE,
        'locked' => TRUE,
      ),
      'instance' => array(
        'entity_type' => 'commerce_line_item',
        'bundle' => $type,
        'label' => t('Display path'),
        'required' => TRUE,
        'settings' => array(),
        'widget' => array(
          'type' => 'text_textfield',
          'weight' => 0,
        ),
        'display' => array(
          'display' => array(
            'label' => 'hidden',
            'weight' => 0,
          ),
        ),
      ),
    ),
    $type => array(
      'field' => array(
        'type' => 'dimension',
        'settings' => array(
          'mode' => $mode,
        ),
        'cardinality' => 1,
        'entity_types' => array('commerce_line_item'),
        'translatable' => FALSE,
        'locked' => FALSE,
      ),
      'instance' => array(
        'entity_type' => 'commerce_line_item',
        'bundle' => $type,
        'label' => t('Product Dimension'),
        'required' => TRUE,
        'settings' => _dimension_default_field_instance_settings(),
        'widget' => array(
          'type' => 'dimension_widget',
          'weight' => 0,
        ),
        'display' => array(
          'display' => array(
            'label' => 'hidden',
            'weight' => 0,
          ),
        ),
        'commerce_cart_settings' => array(
          'field_access' => TRUE,
        ),
      ),
    ),
  );
  foreach ($field_defs as $field_name => $field_def) {
    // Look for or add a display path textfield to the product line item type.
    commerce_activate_field($field_name);
    field_cache_clear();

    $field = field_info_field($field_name);
    $instance = field_info_instance('commerce_line_item', $field_name, $type);

    if (empty($field)) {
      $field = $field_def['field'];
      $field['field_name'] = $field_name;
      field_create_field($field);
    }

    if (empty($instance)) {
      $instance = $field_def['instance'];
      $instance['field_name'] = $field_name;
      field_create_instance($instance);
    }
  }
}

function commerce_product_dimensions_entity_presave($entity, $entity_type) {
  if ($entity_type == 'commerce_line_item' && strpos($entity->type, 'product_with_dimensions_') === 0) {
  }
}

function commerce_product_dimensions_commerce_entity_create_alter($entity_type, $entity) {
  if ($entity_type == 'commerce_line_item' && strpos($entity->type, 'product_with_dimensions_') === 0) {
    $entity->data['context']['add_to_cart_combine'] = FALSE;
  }
}

function commerce_product_dimensions_commerce_line_item_type_delete($type) {
  $type = strtr($type, array('-' => '_'));
  $line_item_type = commerce_line_item_type_load($type);

  $count = db_query("SELECT line_item_id FROM {commerce_line_item} WHERE type = :line_item_type",
    array(':line_item_type' => $line_item_type['type']))
    ->rowCount();
  // Don't allow deletion of line item types that have line items already.
  if ($count > 0) {
    drupal_set_title(t('Cannot delete the %name line item type', array('%name' => $line_item_type['name'])), PASS_THROUGH);
    return format_plural($count,
      'There is 1 line item of this type. It cannot be deleted.',
      'There are @count line items of this type. It cannot be deleted.'
    );
  }

  return drupal_get_form('commerce_product_dimensions_line_item_type_delete_form', $line_item_type);
}

/**
 * Form callback: confirmation form for deleting a line item type.
 *
 * @param $line_item_type
 *   The line item type array to be deleted.
 *
 * @see confirm_form()
 */
function commerce_product_dimensions_line_item_type_delete_form($form, &$form_state, $line_item_type) {
  $form_state['line_item_type'] = $line_item_type;

  $form['#submit'][] = 'commerce_product_dimensions_line_item_type_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete the %name line item type?', array('%name' => $line_item_type['name'])),
    'admin/commerce/config/line-items',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_product_dimensions_line_item_type_delete_form().
 */
function commerce_product_dimensions_line_item_type_delete_form_submit($form, &$form_state) {
  $line_item_type = $form_state['line_item_type'];

  commerce_product_dimensions_line_item_type_delete($line_item_type['type']);

  drupal_set_message(t('The line item type %name has been deleted.', array('%name' => $line_item_type['name'])));
  watchdog('commerce_line_item', 'Deleted line item type %name.', array('%name' => $line_item_type['name']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/config/line-items';
}

function commerce_product_dimensions_line_item_type_delete($type, $skip_reset = FALSE) {
  $id = _commerce_product_dimensions_get_id($type);

  db_delete('commerce_product_dimensions_line_item_type')
    ->condition('id', $id)
    ->execute();

  // Rebuild the menu to get rid of this type's product add menu item.
  if (!$skip_reset) {
    commerce_line_item_types_reset();
    menu_rebuild();
  }

  // Notify the field API that this bundle has been destroyed.
  field_attach_delete_bundle('commerce_line_item', $type);
}

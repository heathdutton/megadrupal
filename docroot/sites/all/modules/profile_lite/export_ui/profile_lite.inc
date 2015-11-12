<?php

/**
 * @file
 * CTools Export UI Plugin Implementation.
 */

$plugin = array(
  // The name of the table as found in the schema in hook_install. This
  // must be an exportable type with the 'export' section defined.
  'schema' => 'profile_lite_categories',

  // The access permission to use. If not provided it will default to
  // 'administer site configuration'
  'access' => 'administer profile lite categories',

  // You can actually define large chunks of the menu system here. Nothing
  // is required here. If you leave out the values, the prefix will default
  // to admin/structure and the item will default to the plugin name.
  'menu' => array(
    'menu prefix' => 'admin/config/people/accounts',
    'menu item' => 'profile-lite',
    // Title of the top level menu. Note this should not be translated,
    // as the menu system will translate it.
    'menu title' => 'Manage categories',
    // Description of the top level menu, which is usually needed for
    // menu items in an administration list. Will be translated
    // by the menu system.
    'menu description' => 'Categorize user fields.',
  ),

  // These are required to provide proper strings for referring to the
  // actual type of exportable. "proper" means it will appear at the
  // beginning of a sentence.
  'title singular' => t('category'),
  'title singular proper' => t('Category'),
  'title plural' => t('categories'),
  'title plural proper' => t('Categories'),

  // This will provide you with a form for editing the properties on your
  // exportable, with validate and submit handler.
  //
  // The item being edited will be in $form_state['item'].
  //
  // The submit handler is only responsible for moving data from
  // $form_state['values'] to $form_state['item'].
  //
  // All callbacks will accept &$form and &$form_state as arguments.
  'form' => array(
    'settings' => 'profile_lite_category_form',
    //'validate' => 'profile_lite_category_form_validate',
  ),
);

/**
 * UGC User Access Record add form.
 */
function profile_lite_category_form(&$form, &$form_state) {
  unset($form['info']);
  $category = isset($form_state['item']) && is_object($form_state['item']) ? $form_state['item'] : new stdClass();

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Category Label'),
    '#default_value' => empty($category->label) ? '' : $category->label,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine Name'),
    '#default_value' => empty($category->name) ? '' : $category->name,
    '#disabled' => !empty($category->name),
    '#machine_name' => array(
      'exists' => '_profile_lite_category_exists',
      'source' => array('label'),
    ),
    '#required' => TRUE,
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => empty($category->weight) ? 0 : $category->weight,
  );

  $form['fields'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Fields'),
  );
  foreach (field_info_instances('user', 'user') as $field => $info) {
    $form['fields'][$field]['selected'] = array(
      '#type' => 'checkbox',
      '#title' => t($info['label']),
      '#default_value' => !empty($category->fields[$field]),
      '#element_validate' => array('profile_lite_fields_element_validate'),
    );
    $form['fields'][$field]['weight'] = array(
      //'#type' => 'weight',
      '#type' => 'value',
      '#title' => t('Weight'),
      '#default_value' => empty($category->fields[$field]['weight']) ? $info['widget']['weight'] : $category->fields[$field]['weight'],
    );
  }
}

/**
 * Element validator for a fields checkbox.
 */
function profile_lite_fields_element_validate($element, &$form_state, $form) {
  $field_name = $element['#parents'][1];
  $field = &$form_state['values']['fields'][$field_name];
  if (empty($field['selected'])) {
    $field_name = $element['#parents'][1];
    unset($form_state['values']['fields'][$field_name]);
  }
  else {
    unset($field['selected']);
  }
}


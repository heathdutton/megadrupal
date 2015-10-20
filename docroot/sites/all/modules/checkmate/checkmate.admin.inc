<?php
/**
 * @file
 * Checkmate admin pages.
 */

/**
 * Generates the checkmate_item type editing form.
 */
function checkmate_item_type_form($form, &$form_state, $checkmate_item_type, $op = 'edit') {

  if ($op == 'clone') {
    $checkmate_item_type->label .= ' (cloned)';
    $checkmate_item_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $checkmate_item_type->label,
    '#description' => t('The human-readable name of this checkmate-item type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($checkmate_item_type->type) ? $checkmate_item_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $checkmate_item_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'checkmate_item_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this checkmate-item type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($checkmate_item_type->description) ? $checkmate_item_type->description : '',
    '#description' => t('Description about the checkmate-item type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save checkmate-item type'),
    '#weight' => 40,
  );

  if (!$checkmate_item_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete checkmate-item type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('checkmate_item_type_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing checkmate_item_type.
 */
function checkmate_item_type_form_submit(&$form, &$form_state) {
  $checkmate_item_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  checkmate_item_type_save($checkmate_item_type);

  // Redirect user back to list of checkmate_item_type.
  $form_state['redirect'] = 'admin/structure/checkmate-item-types';
}

/**
 * Submit handler for deleting a checkmate_item_type.
 */
function checkmate_item_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/checkmate-item-types/' . $form_state['checkmate_item_type']->type . '/delete';
}

/**
 * Checkmate Item type delete form.
 */
function checkmate_item_type_form_delete_confirm($form, &$form_state, $checkmate_item_type) {
  $form_state['checkmate_item_type'] = $checkmate_item_type;
  $form['checkmate_item_type_id'] = array(
    '#type' => 'value',
    '#value' => entity_id('checkmate_item_type', $checkmate_item_type),
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete checkmate-item type %title?', array(
        '%title' => entity_label('checkmate_item_type', $checkmate_item_type),
      )),
    'checkmate_item/' . entity_id('checkmate_item_type', $checkmate_item_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Checkmate Item type delete form submit handler.
 */
function checkmate_item_type_form_delete_confirm_submit($form, &$form_state) {
  $checkmate_item_type = $form_state['checkmate_item_type'];
  checkmate_item_type_delete($checkmate_item_type);

  watchdog('checkmate_item_type', '@type: deleted %title.', array('@type' => $checkmate_item_type->type, '%title' => $checkmate_item_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $checkmate_item_type->type, '%title' => $checkmate_item_type->label)));

  $form_state['redirect'] = 'admin/content/structure/checkmate-item-types';
}

/**
 * Page to select checkmate Item Type to add new checkmate Item.
 */
function checkmate_item_admin_add_page() {
  $items = array();
  foreach (checkmate_item_types() as $check_item_type_key => $check_item_type) {
    $items[] = l(entity_label('checkmate_item_type', $check_item_type), 'admin/content/checkmate-item/add/' . $check_item_type_key);
  }
  return array(
    'list' => array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#title' => t('Select type of checkmate-item to create.'),
    ),
  );
}

/**
 * Add new checkmate Item page callback.
 */
function checkmate_item_add($type) {
  $checkmate_item_type = checkmate_item_types($type);

  $checkmate_item = entity_create('checkmate_item', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('checkmate_item_type', $checkmate_item_type))));

  $output = drupal_get_form('checkmate_item_form', $checkmate_item);

  return $output;
}

/**
 * Checkmate Item Form.
 */
function checkmate_item_form($form, &$form_state, $checkmate_item) {
  $form_state['checkmate_item'] = $checkmate_item;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $checkmate_item->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $checkmate_item->description,
  );
  $form['fields'] = array();
  field_attach_form('checkmate_item', $checkmate_item, $form['fields'], $form_state);

  $form['checkmate_item_advanced'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['checkmate_item_advanced']['checkmate_groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Checkmate item groups'),
  );

  $form['checkmate_item_advanced']['checkmate_groups']['check_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#required' => TRUE,
    '#default_value' => $checkmate_item->check_group,
    '#autocomplete_path' => 'checkmate_group/autocomplete',
  );

  $form['checkmate_item_advanced']['checkmate_groups']['check_subgroup'] = array(
    '#type' => 'textfield',
    '#title' => t('Subgroup'),
    '#required' => TRUE,
    '#default_value' => $checkmate_item->check_subgroup,
    '#autocomplete_path' => 'checkmate_group/autocomplete',
  );

  $form['checkmate_item_advanced']['checkmate_callback'] = array(
    '#type' => 'fieldset',
    '#title' => t('Checkmate item callback'),
  );

  $callbacks = module_invoke_all('checkmate_check_callbacks');
  $callback_options = array();
  foreach ($callbacks as $callback_key => $callback) {
    $callback_options[$callback_key] = $callback['label'];
  }
  asort($callback_options);
  $form['checkmate_item_advanced']['checkmate_callback']['callback'] = array(
    '#type' => 'select',
    '#options' => $callback_options,
    '#title' => t('Callback function'),
    '#required' => TRUE,
    '#default_value' => $checkmate_item->callback,
    '#ajax' => array(
      'callback' => 'checkmate_item_form_js',
      'wrapper' => 'checkmate_callback_arguments_wrapper',
      'method' => 'replace',
    ),
  );


  $form['checkmate_item_advanced']['checkmate_callback']['callback_arguments_wrapper'] = array(
    '#type' => 'container',
    '#id' => 'checkmate_callback_arguments_wrapper',
    '#tree' => TRUE,
  );

  if (!$form_state['rebuild']) {
    drupal_add_js('
      jQuery(document).ready(function($) {
        $("#edit-callback").change();
      });', 'inline'
    );
  }
  elseif (isset($form_state['values']['callback'])) {
    $selected_callback = $form_state['values']['callback'];
    $arguments = isset($callbacks[$selected_callback]['arguments']) ? $callbacks[$selected_callback]['arguments'] : array();

    if ($checkmate_item->callback == $selected_callback) {
      $argument_values = unserialize($checkmate_item->parameters);
      foreach ($arguments as $key => &$argument) {
        if (isset($argument_values[$key])) {
          $argument['#default_value'] = $argument_values[$key];
        }
      }
    }

    $form['checkmate_item_advanced']['checkmate_callback']['callback_arguments_wrapper']['arguments'] = $arguments;

    if (empty($arguments)) {
      $form['checkmate_item_advanced']['checkmate_callback']['callback_arguments_wrapper']['#attributes'] = array('style' => 'display:none;');
    }
    else {
      $form['checkmate_item_advanced']['checkmate_callback']['callback_arguments_wrapper']['#attributes'] = array('style' => 'display:block;');
    }
  }

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $checkmate_item->uid,
  );

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save checkmate-item'),
    '#submit' => $submit + array('checkmate_item_form_submit'),
  );

  // Show Delete button if we edit checkmate-item.
  $checkmate_item_id = entity_id('checkmate_item', $checkmate_item);
  if (!empty($checkmate_item_id) && checkmate_item_access('edit', $checkmate_item)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('checkmate_item_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'checkmate_item_form_validate';

  return $form;
}

/**
 * Checkmate Item Form ajax callback.
 */
function checkmate_item_form_js($form, &$form_state) {
  return $form['checkmate_item_advanced']['checkmate_callback']['callback_arguments_wrapper'];
}

/**
 * Checkmate Item Form validate callback.
 * Needs to be here for the entity api but I have no use for it.
 */
function checkmate_item_form_validate($form, $form_state) {
}

/**
 * Checkmate Item submit handler.
 */
function checkmate_item_form_submit($form, &$form_state) {
  if ($form_state['submitted'] && !form_get_errors()) {
    $checkmate_item = $form_state['checkmate_item'];

    entity_form_submit_build_entity('checkmate_item', $checkmate_item, $form, $form_state);
    $arguments = isset($form_state['values']['callback_arguments_wrapper']['arguments']) ? $form_state['values']['callback_arguments_wrapper']['arguments'] : array();
    $checkmate_item->parameters = serialize($arguments);
    checkmate_item_save($checkmate_item);

    if (isset($_GET['entity_type']) && isset($_GET['entity_id']) && isset($_GET['field_name'])) {
      $entity = entity_load_single($_GET['entity_type'], $_GET['entity_id']);
      if ($entity && isset($entity->$_GET['field_name'])) {
        $entity_wrapper = entity_metadata_wrapper('node', node_load($_GET['entity_id']));
        $checkmate_items = $entity_wrapper->$_GET['field_name']->value();
        $checkmate_items = array_filter($checkmate_items, 'checkmate_item_is_null');
        $checkmate_items[] = $checkmate_item;
        $entity_wrapper->$_GET['field_name']->set($checkmate_items);
        $entity_wrapper->save();
      }
    }
    $form_state['redirect'] = 'admin/content/checkmate-item';
  }
}

/**
 * Used in array_filter to filter out NULL values.
 */
function checkmate_item_is_null($item) {
  return isset($item);
}

/**
 * Checkmate Item delete submit handler.
 */
function checkmate_item_form_submit_delete($form, &$form_state) {
  if (isset($_GET['destination'])) {
    unset($_GET['destination']);
  }
  $checkmate_item = $form_state['checkmate_item'];
  $checkmate_item_id = entity_id('checkmate_item', $checkmate_item);
  $form_state['redirect'] = 'admin/content/checkmate-item/' . $checkmate_item_id . '/delete';
}

/**
 * Delete confirmation form.
 */
function checkmate_item_delete_form($form, &$form_state, $checkmate_item) {
  $form_state['checkmate_item'] = $checkmate_item;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['checkmate_item_type_id'] = array(
    '#type' => 'value',
    '#value' => entity_id('checkmate_item', $checkmate_item),
  );
  $checkmate_item_uri = entity_uri('checkmate_item', $checkmate_item);
  return confirm_form($form,
    t('Are you sure you want to delete checkmate-item %title?', array('%title' => entity_label('checkmate_item', $checkmate_item))),
    $checkmate_item_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function checkmate_item_delete_form_submit($form, &$form_state) {
  $checkmate_item = $form_state['checkmate_item'];
  checkmate_item_delete($checkmate_item);

  drupal_set_message(t('checkmate-item %title deleted.', array('%title' => entity_label('checkmate_item', $checkmate_item))));

  $form_state['redirect'] = 'admin/content/checkmate-item';
}

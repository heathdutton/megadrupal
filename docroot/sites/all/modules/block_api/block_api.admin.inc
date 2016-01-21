<?php

/**
 * @file
 * Block API administration pages.
 */

/**
 * Form constructor to create a new block instance.
 */
function block_api_create_form($form, &$form_state) {
  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrative block description'),
    '#required' => TRUE,
  );
  $form['delta'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 32,
    '#required' => TRUE,
    '#machine_name' => array(
      'source' => array('admin_title'),
      'exists' => 'block_api_load',
    ),
  );
  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('Block type'),
    '#required' => TRUE,
    '#options' => array(),
  );
  foreach (block_api_get_info() as $type => $info) {
    // Add the option.
    $form['type']['#options'][$type] = $info['title'];
    // Add the block type description and entire block info to the about to be
    // expanded sub-element for the option.
    $form['type'][$type]['#description'] = $info['description'];
    $form['type'][$type]['#block_api'] = $info;
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add block'),
  );
  return $form;
}

/**
 * Form submission handler for block_api_create_form().
 */
function block_api_create_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);

  $type = $form_state['values']['type'];
  $block_info = $form['type'][$type]['#block_api'];
  $delta = $form_state['values']['delta'];

  // Save the block instance.
  $instance = $form_state['values'];
  $instance += $block_info;
  $instance = (object) $instance;
  block_api_save($instance);

  // @see block_add_block_form_submit()
  $query = db_insert('block')->fields(array(
    'visibility', 'pages', 'custom', 'title', 'module', 'theme', 'status', 'weight', 'delta', 'cache',
  ));
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $query->values(array(
        'visibility' => 0,
        'pages' => '',
        'custom' => 0,
        'title' => '',
        'module' => 'block_api',
        'theme' => $theme->name,
        'status' => 0,
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();

  drupal_set_message(t('The block has been created.'));

  $form_state['redirect'] = 'admin/structure/block/manage/block_api/' . $delta;
}

/**
 * Form constructor to confirm the deletion of a block instance.
 */
function block_api_delete_form($form, &$form_state, $module, $delta) {
  $instance = block_api_load($delta);
  $form['#block_api'] = $instance;
  $form['type'] = array('#type' => 'value', '#value' => $instance->type);
  $form['module'] = array('#type' => 'value', '#value' => $instance->module);
  $form['delta'] = array('#type' => 'value', '#value' => $instance->delta);

  return confirm_form($form,
    t('Are you sure you want to delete the block %name?', array('%name' => $instance->admin_title)),
    'admin/structure/block',
    NULL,
    t('Delete')
  );
}

/**
 * Form submission handler for block_api_delete_form().
 */
function block_api_delete_form_submit($form, &$form_state) {
  $instance = $form['#block_api'];

  block_api_delete($instance->delta);

  drupal_set_message(t('The block %name has been removed.', array('%name' => $instance->admin_title)));
  cache_clear_all();

  $form_state['redirect'] = 'admin/structure/block';
}

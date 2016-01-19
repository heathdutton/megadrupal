<?php

/**
 * @file
 * Defines the administration pages for the Widget Block module.
 */

/**
 * Display the widget block addition form.
 *
 * @see widget_block_add_block_form_submit()
 */
function widget_block_add_block_form($form, &$form_state) {
  // Load the block.admin include file.
  module_load_include('inc', 'block', 'block.admin');
  // Build the block configuration form.
  $form = block_admin_configure($form, $form_state, 'widget_block', NULL);

  // Other modules should be able to use hook_form_block_add_block_form_alter()
  // to modify this form, so add a base form ID.
  $form_state['build_info']['base_form_id'] = 'block_add_block_form';

  // Prevent default block validate and submit from being automatically added
  // because of the base form ID by providing these handlers manually.
  $form['#validate'] = array();
  $form['#submit'] = array('widget_block_add_block_form_submit');

  return $form;
}

/**
 * Form submit handler for "widget_block_add_block_form".
 */
function widget_block_add_block_form_submit($form, &$form_state) {
  // Set the block delta to the widget ID.
  $delta = $form_state['values']['delta'] = $form_state['values']['widget_block']['id'];
  // Save the widget block.
  widget_block_block_save(NULL, $form_state['values']);
  //
  // The following code was copied from block.admin.inc and is used for
  // creating a new block.
  //
  // Create the block.
  $query = db_insert('block')->fields(array(
    'visibility',
    'pages',
    'custom',
    'title',
    'module',
    'theme',
    'status',
    'weight',
    'delta',
    'cache',
  ));
  // Iterate through the available themes.
  foreach (list_themes() as $theme) {
    if ($theme->status) {
      $query->values(array(
        'visibility' => (int) $form_state['values']['visibility'],
        'pages' => trim($form_state['values']['pages']),
        'custom' => (int) $form_state['values']['custom'],
        'title' => $form_state['values']['title'],
        'module' => $form_state['values']['module'],
        'theme' => $theme->name,
        'status' => 0,
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();
  // Create the block roles.
  $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    $query->values(array(
      'rid' => $rid,
      'module' => $form_state['values']['module'],
      'delta' => $delta,
    ));
  }
  $query->execute();
  // Notify the administrator about the created block.
  drupal_set_message(t('The block has been created.'));
  // Ensure all cache is cleared. This is needed because the blocks will need
  // to be rebuild.
  cache_clear_all();
  // Redirect to the block overview page.
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Confirm deletion of a widget block.
 */
function widget_block_delete_block_form($form, &$form_state, $delta = 0) {
  // Try to retrieve the widget block configuration.
  if (($widget_block_config = widget_block_get_config($delta))) {
    // Attach widget block delta to the form.
    $form['delta'] = array(
      '#type' => 'value',
      '#value' => $widget_block_config['id'],
    );
    // Build the confirm form related variables.
    $question = t('Are you sure you want to delete the "%name" block?', array('%name' => $widget_block_config['name']));
    // Generat the confirm form.
    return confirm_form($form, $question, 'admin/structure/block', NULL, t('Delete'), t('Cancel'));
  }
  // Notify about the invalid widget block ID.
  drupal_set_message(t('Invalid widget block ID'), 'error');
  // Redirect to the overview page.
  drupal_goto('admin/structure/block');
}

/**
 * Form submit handler for "widget_block_delete_block_form".
 */
function widget_block_delete_block_form_submit($form, &$form_state) {
  // Get the delta attached to the form.
  $delta = $form_state['values']['delta'];
  // Remove the block.
  db_delete('block')
      ->condition('module', 'widget_block')
      ->condition('delta', $delta)
      ->execute();
  // Remove the block roles.
  db_delete('block_role')
      ->condition('module', 'widget_block')
      ->condition('delta', $delta)
      ->execute();
  // Remove the widget block configuration.
  widget_block_del_config($delta);
  // Redirect to the overview page.
  $form_state['redirect'] = 'admin/structure/block';
}

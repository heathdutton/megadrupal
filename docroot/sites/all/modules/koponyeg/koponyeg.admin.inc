<?php

/**
 * @file
 * Admin interface for configure and add koponyeg blocks.
 *
 * @author Dániel Kalmár <kalm.dani@gmail.com>
 */


/**
 * Build the koponyeg block configuration form for add new weather block.
 * This code comes from the menu block module admin file. Thanks for the developers.
 */
function koponyeg_add_block_form($form, $form_state) {
  module_load_include('inc', 'block', 'block.admin');
  $form = block_admin_configure($form, $form_state, 'koponyeg', NULL);
  // Other modules should be able to use hook_form_block_add_block_form_alter()
  // to modify this form, so add a base form ID.
  $form_state['build_info']['base_form_id'] = 'block_add_block_form';
  // Prevent block_add_block_form_validate/submit() from being automatically
  // added because of the base form ID by providing these handlers manually.
  $form['#validate'] = array();
  $form['#submit'] = array('koponyeg_add_block_form_submit');

  return $form;
}

/**
 * Save the new weather block.
 */
function koponyeg_add_block_form_submit($form, &$form_state) {
  // Determine the delta of the new block.
  $block_ids = variable_get('koponyeg_block_ids', array());
  $delta = empty($block_ids) ? 1 : max($block_ids) + 1;

  // Save the new array of blocks IDs.
  $block_ids[] = $delta;
  variable_set('koponyeg_block_ids', $block_ids);

  // Save the block into the blocks table.
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $region = !empty($form_state['values']['regions'][$theme->name]) ? $form_state['values']['regions'][$theme->name] : BLOCK_REGION_NONE;

      $fields = array(
        'visibility' => (int) $form_state['values']['visibility'],
        'pages' => trim($form_state['values']['pages']),
        'custom' => (int) $form_state['values']['custom'],
        'title' => $form_state['values']['title'],
        'module' => $form_state['values']['module'],
        'theme' => $theme->name,
        'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
        'status' => (int) ($region != BLOCK_REGION_NONE),
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_CACHE_GLOBAL,
      );

      db_insert('block')
        ->fields($fields)
        ->execute();
    }
  }

  variable_set('koponyeg_forecast_region_' . $delta, $form_state['values']['koponyeg_forecast_region']);

  foreach (array_filter($form_state['values']['roles']) as $rid) {
    $fields = array(
      'rid' => $rid,
      'module' => $form_state['values']['module'],
      'delta' => $delta,
    );

    db_insert('block_role')
      ->fields($fields)
      ->execute();
  }

  drupal_set_message(t('The block has been created.'));
  cache_clear_all();

  $form_state['redirect'] = 'admin/structure/block';
  return;
}

/**
 * Build the block delete confirmation block.
 */
function koponyeg_block_delete($form, &$form_state, $delta = 0) {
  $form['delta'] = array('#type' => 'hidden', '#value' => $delta);
  return confirm_form($form, t('Are you sure you want to delete this block?'), 'admin/structure/block', NULL, t('Delete'), t('Cancel'));
}

/**
 * Deletion of Koponyeg blocks.
 */
function koponyeg_block_delete_submit($form, &$form_state) {
  // Remove the deleted weather block id.
  $delta = $form_state['values']['delta'];
  $block_ids = variable_get('koponyeg_block_ids', array());
  unset($block_ids[array_search($delta, $block_ids)]);
  variable_set('koponyeg_block_ids', $block_ids);
  variable_del('koponyeg_forecast_region_' . $delta);
  // Delete the block from the proper tables.
  db_delete('block')
    ->condition('module', 'koponyeg')
    ->condition('delta', $delta)
    ->execute();
  db_delete('block_role')
    ->condition('module', 'koponyeg')
    ->condition('delta', $delta)
    ->execute();

  drupal_set_message(t('The block has been removed.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
  return;
}

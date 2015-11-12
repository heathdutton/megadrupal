<?php
/**
 * @file
 * Bclock Upload admin form.
 */

/**
 * Builds Module configuration form.
 *
 * Blocks count selector.
 */
function block_upload_settings_form($form, &$form_state) {
  $form['block_upload_blocks_count'] = array(
    '#type' => 'select',
    '#title' => t('Blocks count'),
    '#options' => drupal_map_assoc(range(1, 10)),
    '#default_value' => variable_get('block_upload_blocks_count', 1),
  );
  return system_settings_form($form);
}

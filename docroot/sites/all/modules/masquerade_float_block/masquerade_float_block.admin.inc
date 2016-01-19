<?php

/**
 * @file
 * This file contain admin UI page callback.
 */

/**
 * Menu callback.
 *
 * Add admin page for Masquerade float block.
 */
function masquerade_float_block_admin_page_form($form, &$form_state) {
  $form['masquerade_float_block_visible'] = array(
    '#title'         => t('Enabled'),
    '#description'   => t('Check to enable the Masquerade float block.
      The default Masquerade block position will not be affected.'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('masquerade_float_block_visible', 1),
  );

  return system_settings_form($form);
}

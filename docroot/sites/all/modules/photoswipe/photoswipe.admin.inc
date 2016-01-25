<?php

/**
 * @file
 * Administrative page callbacks for the photoswipe module.
 */

function photoswipe_admin_settings($form, &$form_state) {
  $form['photoswipe_always_load_non_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load PhotoSwipe on all non-admin pages'),
    '#default_value' => variable_get('photoswipe_always_load_non_admin', FALSE),
    '#description' => t('Useful if you want to use photoswipe elsewhere by just adding the <code>.photoswipe</code> CSS class.')
  );

  return system_settings_form($form);
}

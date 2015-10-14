<?php

/**
 * Implements hook_admin()
 *
 * @return $form array
 */
function commerce_ajax_basket_link_admin() {
  $form = array();

  $form['commerce_ajax_basket_link_replace_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace submit'),
    '#description' => t('If you want the confirmation message to replace submit
      button, leave this checked.'),
    '#default_value' => variable_get('commerce_ajax_basket_link_replace_submit', 1)
  );

  $form['commerce_ajax_basket_link_default_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Return default system messages'),
    '#description' => t('If you want to return the default Drupal system messages,
      rather than a simple confirmation, check this option.'),
    '#default_value' => variable_get('commerce_ajax_basket_link_default_messages', 0)
  );

  return system_settings_form($form);
}

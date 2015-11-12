<?php
/**
 * @file
 * Administration forms for PathInfo.
 */

/**
 * Form constructor.
 */
function pathinfo_config_form($form, &$form_state) {

  $form['pathinfo_footer_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Path Info details in page footer.'),
    '#default_value' => variable_get('pathinfo_footer_display', TRUE),
  );

  $t_arguments = array(
    '!drupal_api' => l(
      t('Drupal API reference'),
      'http://api.drupal.org/api/drupal',
      array('external' => TRUE)
    ),
    '!drupalize_me' => l(
      t('drupalize.me'),
      'http://api.drupalize.me',
      array('external' => TRUE)
    ),
  );

  $form['pathinfo_link_functions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link functions to their documentation in the !drupal_api or !drupalize_me where possible.', $t_arguments),
    '#default_value' => variable_get('pathinfo_link_functions', TRUE),
  );

  return system_settings_form($form);
}

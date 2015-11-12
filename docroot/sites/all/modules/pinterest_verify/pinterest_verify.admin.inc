<?php
/**
 * @file pinterest_verify.admin.inc
 * Settings form for the pinterest_verify module.
 */

/**
 * Settings form callback.
 */
function pinterest_verify_settings() {
  $form['pinterest'] = array(
    '#type' => 'fieldset',
    '#markup' => t('Configure your pinterest web site verification settings.'),
  );

  $form['pinterest']['pinterest_verify_method'] = array(
    '#type' => 'radios',
    '#title' => t('Verification method'),
    '#options' => array(
      PINTEREST_VERIFY_META => t('Use a meta tag.'),
      PINTEREST_VERIFY_PAGE => t('Use a verification page.'),
    ),
    '#default_value' => variable_get('pinterest_verify_method', PINTEREST_VERIFY_META),
  );

  $form['pinterest']['pinterest_verify_meta'] = array(
    '#type' => 'textfield',
    '#title' => t('Meta Tag'),
    '#description' => t('Enter the random looking <em>content</em> string from the verification meta tag pinterest provided you, e.g: <em>a9730d3ec6fcb86d646230fb4a90155</em>'),
    '#default_value' => variable_get('pinterest_verify_meta', ''),
  );

  $form['pinterest']['pinterest_verify_page_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification Page URL'),
    '#description' => t('Enter the verification filename pinterest has provided you, e.g: <em>pinterest-a4288.html</em>'),
    '#default_value' => variable_get('pinterest_verify_page_url', ''),
  );

  $form['pinterest']['pinterest_verify_page_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Verification Page Content'),
    '#description' => t('Paste the content of the verification file pinterest has provided you.'),
    '#default_value' => variable_get('pinterest_verify_page_content', ''),
  );

  $form['#submit'][] = 'pinterest_verify_settings_menu_rebuild';

  return system_settings_form($form);
}

/**
 * Extra submit handler to rebuild the menu when page verification is chosen.
 *
 * @todo: Maybe find a less heavy way of doing this.
 */
function pinterest_verify_settings_menu_rebuild($form, $form_state) {
  // Only do this if the page url was changed.
  if ($form_state['values']['pinterest_verify_method'] == PINTEREST_VERIFY_PAGE
    && !empty($form_state['values']['pinterest_verify_page_url'])
    && $form_state['values']['pinterest_verify_page_url'] != variable_get('pinterest_verify_page_url', '')
  ) {
    menu_rebuild();
    drupal_set_message(t('The menu router has been rebuilt.'));
  }
}

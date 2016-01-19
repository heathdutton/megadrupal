<?php
/**
 * @file acquia_mobile_redirect.admin.inc
 * Settings UI for the module.
 */

/**
 * Form callback for the settings form.
 */
function acquia_mobile_redirect_settings() {
  $form['acquia_mobile_redirect_mobile'] = array(
    '#title' => t('Mobile URL'),
    '#description' => t('Enter the URL that mobile devices should redirect to, if they visit the site via another URL.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('acquia_mobile_redirect_mobile', ''),
  );

  $form['acquia_mobile_redirect_tablet'] = array(
    '#title' => t('Tablet URL'),
    '#description' => t('Enter the URL that tablet devices should redirect to, if they visit the site via another URL.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('acquia_mobile_redirect_tablet', ''),
  );

  $form['acquia_mobile_redirect_desktop'] = array(
    '#title' => t('Desktop URL'),
    '#description' => t('Enter the URL that desktop computers should redirect to, if they visit the site via another URL.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('acquia_mobile_redirect_desktop', ''),
  );

  $form['acquia_mobile_redirect_override'] = array(
    '#title' => t('Override variable'),
    '#description' => t('If your site uses a global $_GET variable (e.g.: ?device=mobile) to override device detection, enter its name here. E.g: if you can override url switching with <em>?device=mobile</em>, enter <strong>device</strong> in this field.'),
    '#type' => 'textfield',
    '#size' => 16,
    '#default_value' => variable_get('acquia_mobile_redirect_override', ''),
  );

  return system_settings_form($form);
}

/**
 * Validate handler for the settings form.
 */
function acquia_mobile_redirect_settings_validate($form, &$form_state) {
  // Make sure the URLs are just that.
  if (!empty($form_state['values']['acquia_mobile_redirect_mobile']) && !valid_url($form_state['values']['acquia_mobile_redirect_mobile'])) {
    form_set_error('acquia_mobile_redirect_mobile', t('Mobile URL %url is not a valid URL', array('%url' => $form_state['values']['acquia_mobile_redirect_mobile'])));
  }
  if (!empty($form_state['values']['acquia_mobile_redirect_tablet']) && !valid_url($form_state['values']['acquia_mobile_redirect_tablet'])) {
    form_set_error('acquia_mobile_redirect_tablet', t('Tablet URL %url is not a valid URL', array('%url' => $form_state['values']['acquia_mobile_redirect_tablet'])));
  }
  if (!empty($form_state['values']['acquia_mobile_redirect_desktop']) && !valid_url($form_state['values']['acquia_mobile_redirect_desktop'])) {
    form_set_error('acquia_mobile_redirect_desktop', t('Desktop URL %url is not a valid URL', array('%url' => $form_state['values']['acquia_mobile_redirect_desktop'])));
  }
}

<?php

/**
 * Sunlight APIs settings form.
 */
function sunlight_admin() {
  $form['sunlight_apikey'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Sunlight API key'),
    '#default_value' => variable_get('sunlight_apikey', ''),
    '#description'   => t('Enter your <a href="https://sunlightfoundation.com/api/accounts/register/">Sunlight API key</a>.'),
    '#required'      => TRUE,
  );
  return system_settings_form($form);
}

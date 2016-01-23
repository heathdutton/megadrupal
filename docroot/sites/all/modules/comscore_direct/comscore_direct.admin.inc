<?php
/**
 * @file
 * Common settings for comScore Direct tag. hansdotcom@gmail.com.
 * comscore_direct_form.
 *
 */
function comscore_direct_form() {
  global $base_url;
  $form = array();
  $form['comscore_direct_id'] = array(
      '#title' => t('comScore site id'),
      '#type' => 'textfield',
      '#description' => t('comScore ID (required)'),
      '#default_value' => variable_get('comscore_direct_id', ''),
      '#required' => TRUE,
  );
  $form['comscore_direct_genre'] = array(
      '#title' => t('comScore Genre of content'),
      '#type' => 'textfield',
      '#description' => t('Alphanumeric value used for client specific custom classification (optional)'),
      '#default_value' => variable_get('comscore_direct_genre', '')
  );
  $form['comscore_direct_package'] = array(
      '#title' => t('comScore Package'),
      '#type' => 'textfield',
      '#description' => t('Alphanumeric value for customized aggregation to reflect sections or site centric advertising packages (optional)'),
      '#default_value' => variable_get('comscore_direct_package', '')
  );
  $form['comscore_direct_clientseg'] = array(
      '#title' => t('comScore Client Segment ID'),
      '#type' => 'textfield',
      '#description' => t('Alphanumeric value for Publisher’s own segment for the machine the content asset is being served to (optional)'),
      '#default_value' => variable_get('comscore_direct_clientseg', '')
  );
  $form['comscore_direct_baseurl'] = array(
    '#title' => t('Base URL'),
    '#type' => 'textfield',
    '#description' => t('This is your site URL and will be used for the c4 parameter.  If empty, the default will be: %base_url', array('%base_url' => $base_url)),
    '#default_value' => variable_get('comscore_direct_baseurl', ''),
  );
  return system_settings_form($form);
}

/*
 * Form validation
 *
 */
function comscore_direct_form_validate($form, &$form_state) {
  $comscore_customer_id = $form_state['values']['comscore_direct_id'];
  if (!is_numeric($comscore_customer_id)) {
    form_set_error('mls_comscore_id', t('You must enter an integer for comScore customer code.'));
  }
  elseif ($comscore_customer_id <= 0) {
    form_set_error('mls_comscore_id', t('comScore customer code must be positive.'));
  }
}

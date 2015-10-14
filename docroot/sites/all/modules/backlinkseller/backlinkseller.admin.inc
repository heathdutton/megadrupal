<?php

/**
 * @file
 * Backlinkseller integration.
 */

/**
 * Default administration forms of the module.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array
 *   Backlinkseller basic settings form
 */
function backlinkseller_settings_form(array $form, array &$form_state) {
  _backlinkseller_get_debug_messeges();

  // BASICS.
  $form['basics'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['basics']['backlinkseller_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Backlinkseller ID'),
    '#description' => t('This ID you can find on the top of the Backlinkseller Code. (@example)', array(
      '@example' => '$BACKLINK_SELLER["ID"] = "0123456789.9876543210";',
    )),
    '#default_value' => variable_get('backlinkseller_id'),
    '#size' => 25,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#attributes' => array(
      'required' => 'required',
    ),
  );

  $form['basics']['backlinkseller_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('The URL which leads to your website.'),
    '#default_value' => variable_get('backlinkseller_site_url', url('', array('absolute' => TRUE))),
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['basics']['backlinkseller_html_block_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal Block ID'),
    '#description' => t('The value will be the id-Attribute of the Drupal block. Nothig should point to Backlinkseller.de'),
    '#default_value' => variable_get('backlinkseller_html_block_id', md5(time())),
    '#size' => 35,
    '#maxlength' => 255,
  );

  // HTML SETTINGS.
  $form['html'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['html']['backlinkseller_html_before_ads'] = array(
    '#type' => 'textfield',
    '#title' => t('Before text links'),
    '#description' => t('Custom HTML code which is added before all text links.'),
    '#default_value' => variable_get('backlinkseller_html_before_ads', '<div>'),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['html']['backlinkseller_html_after_ads'] = array(
    '#type' => 'textfield',
    '#title' => t('After text links'),
    '#description' => t('Custom HTML code which is added after all text links.'),
    '#default_value' => variable_get('backlinkseller_html_after_ads', '</div>'),
    '#size' => 60,
    '#maxlength' => 255,
  );

  // DEBUGING.
  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debugging'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['debug']['backlinkseller_debug_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('If you enable this option, all error messages will be displayed, so you can determine the cause of error easier.'),
    '#default_value' => variable_get('backlinkseller_debug_flag', 1),
    '#options' => drupal_map_assoc(array(0, 1)),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validate the backlinkseller id an website URL.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function backlinkseller_settings_form_validate(array $form, array &$form_state) {
  if (!preg_match('#([0-9]\.)+#', $form_state['values']['backlinkseller_id'])) {
    form_set_error('backlinkseller_id', t('Enter a valid ID please.'));
  }
  if (!valid_url($form_state['values']['backlinkseller_site_url'], TRUE)) {
    form_set_error('backlinkseller_site_url', t('Enter a valid URL please.'));
  }
}

/**
 * Save the default module settings.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function backlinkseller_settings_form_submit(array $form, array &$form_state) {
  $backlinkseller_id_parts = explode('.', $form_state['values']['backlinkseller_id']);
  variable_set('backlinkseller_affiliate_id', $backlinkseller_id_parts[0]);
  variable_set('backlinkseller_id', $form_state['values']['backlinkseller_id']);
  variable_set('backlinkseller_html_block_id', $form_state['values']['backlinkseller_html_block_id']);
  variable_set('backlinkseller_site_url', $form_state['values']['backlinkseller_site_url']);
  variable_set('backlinkseller_html_before_ads', $form_state['values']['backlinkseller_html_before_ads']);
  variable_set('backlinkseller_html_after_ads', $form_state['values']['backlinkseller_html_after_ads']);
  variable_set('backlinkseller_debug_flag', $form_state['values']['backlinkseller_debug_flag']);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Administration form for the affiliate block.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array
 *   Affiliate settings form
 */
function backlinkseller_affiliate_form(array $form, array &$form_state) {
  module_load_include('inc', 'backlinkseller');
  _backlinkseller_get_debug_messeges();

  $form['affiliate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Affiliate'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $module_path = drupal_get_path('module', 'backlinkseller');
  $img_path = $module_path . '/assets/images/';

  $form['affiliate']['backlinkseller_affiliate_banner'] = array(
    '#type' => 'radios',
    '#title' => t('Banner'),
    '#description' => t('Choose a banner for the affiliate program.'),
    '#default_value' => variable_get('backlinkseller_affiliate_banner', 1),
    '#options' => array(
      1 => theme('image', array('path' => $img_path . 'banner-1.gif', 'attributes' => array())),
      2 => theme('image', array('path' => $img_path . 'banner-2.gif', 'attributes' => array())),
      3 => theme('image', array('path' => $img_path . 'banner-3.gif', 'attributes' => array())),
    ),
  );

  $form['affiliate']['backlinkseller_affiliate_banner_alt'] = array(
    '#type' => 'textfield',
    '#title' => t('Banner alt-Attribute'),
    '#description' => t('This text will be displayed, when the Banner is not available'),
    '#default_value' => variable_get('backlinkseller_affiliate_banner_alt'),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['affiliate']['backlinkseller_affiliate_link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Link title-Attribute'),
    '#description' => t('This text will be displayed as tooltip.'),
    '#default_value' => variable_get('backlinkseller_affiliate_link_title'),
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['affiliate']['backlinkseller_affiliate_html_block_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal Block ID'),
    '#description' => t('The value will be the id-Attribute of the Drupal block. Nothig should point to Backlinkseller.de'),
    '#default_value' => variable_get('backlinkseller_affiliate_html_block_id', md5(time() + 60)),
    '#size' => 35,
    '#maxlength' => 255,
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Save the affiliate block settings.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function backlinkseller_affiliate_form_submit(array $form, array &$form_state) {
  $backlinkseller_affiliate_banner = variable_get('backlinkseller_affiliate_banner');

  if ($form_state['values']['backlinkseller_affiliate_banner'] != $backlinkseller_affiliate_banner) {
    $installation_root = DRUPAL_ROOT . base_path();
    $module_path = drupal_get_path('module', 'backlinkseller');
    $selected_banner = intval($form_state['values']['backlinkseller_affiliate_banner']);

    $source = $installation_root . $module_path . '/assets/images/banner-' . $selected_banner . '.gif';
    $target = 'public://' . md5('backlinkseller') . '.gif';

    file_unmanaged_copy($source, $target, FILE_EXISTS_REPLACE);
  }

  variable_set('backlinkseller_affiliate_html_block_id', $form_state['values']['backlinkseller_affiliate_html_block_id']);
  variable_set('backlinkseller_affiliate_banner', $form_state['values']['backlinkseller_affiliate_banner']);
  variable_set('backlinkseller_affiliate_banner_alt', $form_state['values']['backlinkseller_affiliate_banner_alt']);
  variable_set('backlinkseller_affiliate_link_title', $form_state['values']['backlinkseller_affiliate_link_title']);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Warn user if debug flag enabled.
 */
function _backlinkseller_get_debug_messeges() {
  if (variable_get('backlinkseller_debug_flag', 1) == 1) {
    drupal_set_message(t('Backlinkseller is in debug mode!'), 'warning');
  }
}

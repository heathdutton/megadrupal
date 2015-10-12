<?php
/**
 * @file
 * Provides theme settings for Fly Flower based themes when admin theme is not.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function fly_flower_form_system_theme_settings_alter(&$form, $form_state, $form_id = NULL) {
  // Work-around for a core bug affecting admin themes.
  // @see https://drupal.org/node/943212
  if (isset($form_id)) {
    return;
  }
  // Alter theme settings form.
  _fly_flower_settings_form($form, $form_state);
}

/**
 * Contains the form for the theme settings.
 *
 * @param array $form
 *   The form array, passed by reference.
 * @param array $form_state
 *   The form state array, passed by reference.
 */
function _fly_flower_settings_form(array &$form, array $form_state) {
  $form['cdrupal'] = array(
    '#type'   => 'vertical_tabs',
    '#prefix' => '<h2><small>' . t('Fly Flower Settings') . '</small></h2>',
    '#weight' => -11,
  );

  // cDrupal general.
  $form['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#group' => 'cdrupal',
  );
  // Socials network.
  $form['general_settings']['socials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Socials network'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Facebook.
  $form['general_settings']['socials']['facebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook'),
    '#default_value' => theme_get_setting('facebook'),
  );
  // Twitter.
  $form['general_settings']['socials']['twitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter'),
    '#default_value' => theme_get_setting('twitter'),
  );
  // Pinterest.
  $form['general_settings']['socials']['pinterest'] = array(
    '#type' => 'textfield',
    '#title' => t('Pinterest'),
    '#default_value' => theme_get_setting('pinterest'),
  );
  // Footer.
  $form['footer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer'),
    '#group' => 'cdrupal',
  );

  // Copyright text.
  $form['footer']['copyright'] = array(
    '#type' => 'textarea',
    '#title' => t('Copyright'),
    '#default_value' => theme_get_setting('copyright'),
  );

  // Credit.
  $form['footer']['credit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Don\'t not show theme author: Theme by <a href="http://cdrupal.com/"><strong>cDrupal</strong></a>'),
    '#default_value' => theme_get_setting('credit'),
  );

  // Wrap global setting fieldsets in vertical tabs.
  $form['general'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2><small>' . t('Override Global Settings') . '</small></h2>',
    '#weight' => -9,
  );
  $form['theme_settings']['#group'] = 'general';
  $form['logo']['#group'] = 'general';
  $form['favicon']['#group'] = 'general';

  // Do not add Bootstrap specific settings to global settings.
  if (empty($form_state['build_info']['args'][0])) {
    unset($form['general']['#prefix']);
    return;
  }
  // BootstrapCDN.
  $form['advanced']['bootstrap_cdn'] = array(
    '#type' => 'fieldset',
    '#title' => t('BootstrapCDN'),
    '#description' => t('Use !bootstrapcdn to serve the Bootstrap framework files. Enabling this setting will prevent this theme from attempting to load any Bootstrap framework files locally. !warning', array(
      '!bootstrapcdn' => l(t('BootstrapCDN'), 'http://bootstrapcdn.com', array(
        'external' => TRUE,
      )),
      '!warning' => '<div class="alert alert-info messages info"><strong>' . t('NOTE') . ':</strong> ' . t('While BootstrapCDN (content distribution network) is the preferred method for providing huge performance gains in load time, this method does depend on using this third party service. BootstrapCDN is under no obligation or commitment to provide guaranteed up-time or service quality for this theme. If you choose to disable this setting, you must provide your own Bootstrap source and/or optional CDN delivery implementation.') . '</div>',
    )),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['bootstrap_cdn']['bootstrap_cdn'] = array(
    '#type' => 'select',
    '#title' => t('BootstrapCDN version'),
    '#options' => drupal_map_assoc(array(
      '3.0.0',
      '3.0.1',
      '3.0.2',
      // Add 3.3.4 version by cDrupal.com
      '3.3.4',
    )),
    '#default_value' => theme_get_setting('bootstrap_cdn'),
    '#empty_option' => t('Disabled'),
    '#empty_value' => NULL,
  );
}

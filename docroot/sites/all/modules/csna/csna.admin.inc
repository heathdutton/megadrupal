<?php

/**
 * @file
 * Functionality for CSNA administration.
 */

/**
 * Menu callback: CSNA key or secrect config.
 */
function csna_admin_settings() {
  $providers = csna_providers();
  foreach ($providers as $provider_id => $provider) {
    $form[$provider_id] = array(
      '#type' => 'fieldset',
      '#title' => $provider['title'],
      '#description' => $provider['description'],
    );
    $form[$provider_id]['csna_' . $provider_id . '_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Key'),
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => variable_get('csna_' . $provider_id . '_key', ''),
    );
    $form[$provider_id]['csna_' . $provider_id . '_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Secret'),
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => variable_get('csna_' . $provider_id . '_secret', ''),
    );
  }

  // If no provider/sub-module was found, provide a default help text.
  if (!isset($form)) {
    $form['default_message'] = array(
      '#markup' => t('<strong>No CSNA third party API provider module has been enabled yet</strong>.<br/>Please enable <strong>at least a CSNA sub-module</strong> or any other <strong>CSNA API provider</strong> from the !modadminlink then feel free to browse this page again.<br/>See more information on !projectlink about <strong>supported providers</strong> and <strong>installation instructions</strong>.', array(
        '!modadminlink' => l(t('module administration page'), 'admin/modules'),
        '!projectlink' => l(t("module project's page"), 'http://drupal.org/project/csna', array('attributes' => array('target' => '_blank'))),
      )),
    );
    return $form;
  }
  else {
    return system_settings_form($form);
  }
}

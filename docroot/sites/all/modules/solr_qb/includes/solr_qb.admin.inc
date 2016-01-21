<?php
/**
 * @file
 * Admin forms.
 */

/**
 * Main settings form.
 */
function solr_qb_settings_form($form, &$form_state) {
  $drivers = module_invoke_all('solr_qb_driver');
  $options = array();
  foreach ($drivers as $key => $driver) {
    $options[$key] = $driver['name'];
  }
  $form['solr_qb_driver'] = array(
    '#title' => t('Used driver'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('solr_qb_driver', 'custom'),
  );

  // Custom connection settings.
  $custom = variable_get('solr_qb_custom_settings', array());
  $form['solr_qb_custom_settings'] = array(
    '#title' => t('Custom server settings'),
    '#type' => 'fieldset',
    '#states' => array(
      'visible' => array(
        ':input[name="solr_qb_driver"]' => array('value' => 'custom'),
      ),
    ),
    '#tree' => TRUE,
  );
  $form['solr_qb_custom_settings']['protocol'] = array(
    '#title' => t('Protocol'),
    '#type' => 'select',
    '#options' => array(
      'http' => 'http',
      'https' => 'https',
    ),
    '#default_value' => !empty($custom['protocol']) ? $custom['protocol'] : 'http',
  );
  $form['solr_qb_custom_settings']['host'] = array(
    '#title' => t('Host'),
    '#type' => 'textfield',
    '#description' => t('You can set authentication credentials as a part of the host URL. Example: @example', array('@example' => 'admin:admin@localhost')),
    '#default_value' => !empty($custom['host']) ? $custom['host'] : '',
  );
  $form['solr_qb_custom_settings']['path'] = array(
    '#title' => t('Solr path'),
    '#type' => 'textfield',
    '#default_value' => !empty($custom['path']) ? $custom['path'] : '/solr',
  );
  $form['solr_qb_custom_settings']['port'] = array(
    '#title' => t('Port'),
    '#type' => 'textfield',
    '#default_value' => !empty($custom['port']) ? $custom['port'] : 8983,
  );
  return system_settings_form($form);
}

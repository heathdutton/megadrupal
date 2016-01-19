<?php
/**
 * @file
 * Provides admin settings form(s)
 */

/**
 * Admin settings form.
 */
function foresight_admin() {
  $form = array();

  $form['foresight_noscript'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include non-javascript fallback'),
    '#default_value' => variable_get('foresight_noscript', TRUE),
    '#description' => t("Include a noscript tag to display the default image when JavaScript is not available."),
  );

  $form['bandwidth_test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bandwidth test'),
  );

  $form['bandwidth_test']['foresight_min_kbs_for_high_bandwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum speed'),
    '#field_suffix' => 'Kb/s',
    '#default_value' => variable_get('foresight_min_kbs_for_high_bandwidth', 300),
    '#description' => t("Minimum download speed required to pass bandwidth test."),
    '#size' => 5,
  );

  $form['bandwidth_test']['foresight_speed_test_kb'] = array(
    '#type' => 'select',
    '#title' => t('Download size'),
    '#default_value' => variable_get('foresight_speed_test_kb', 50),
    '#options' => array(
      25 => '25K',
      50 => '50K',
      100 => '100K',
      150 => '150K',
      200 => '200K',
    ),
    '#description' => t("The size of the image file downloaded to test bandwidth."),
  );

  $form['bandwidth_test']['foresight_speed_test_expires_minutes'] = array(
    '#type' => 'textfield',
    '#title' => t('Remember result for'),
    '#field_suffix' => t('minutes'),
    '#default_value' => variable_get('foresight_speed_test_expires_minutes', 30),
    '#description' => t("Minimum download speed required to pass bandwidth test."),
    '#size' => 5,
  );

  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debug'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['debug']['foresight_forced_pixel_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Force pixel ratio'),
    '#default_value' => variable_get('foresight_forced_pixel_ratio', ''),
    '#description' => t("Bypass pixel ratio detection and use fixed value."),
    '#size' => 5,
  );
  $form['debug']['foresight_forced_bandwidth'] = array(
    '#type' => 'select',
    '#title' => t('Force bandwidth'),
    '#options' => array(
      '' => 'Off',
      'high' => t('High'),
      'low' => t('Low'),
    ),
    '#default_value' => variable_get('foresight_forced_bandwidth', ''),
    '#description' => t("Bypass bandwidth detection and use fixed value."),
  );

  return system_settings_form($form);
}

<?php
/**
 * @file
 * Admin forms for deletemonitor
 */

/**
 * Settings form
 * @return mixed
 */
function deletemonitor_settings() {
  $form['deletemonitor_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Delete Monitor options'),
    '#default_value' => variable_get('deletemonitor_types', array()),
    '#options' => array('node' => t('Node types'), 'views' => t('Views'), 'imagestyles' => t('Image Styles')),
    '#description' => t("Select which ones you want to be monitored"),
  );

  return system_settings_form($form);
}

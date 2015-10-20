<?php

/**
 * @file
 * Administration page callbacks for the Jalali Date module.
 */

/**
 * Form builder. Configure Jalali Date settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function jalalidate_nodes_admin_settings() {

  // Pattern url
  if (class_exists('IntlDateFormatter')) {
    $url = 'http://userguide.icu-project.org/formatparse/datetime';
  }
  else {
    $url = 'http://ir2.php.net/manual/en/function.date.php';
  }

  $form['jalalidate_nodes_settings'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Jalali Date settings',
  );

  $form['jalalidate_nodes_settings']['jalalidate_nodes_format'] = array(
    '#type'   => 'select',
    '#title'  => t('Date format'),
    '#options'=> array(
        'short' => t('Short -> 1/2/1392'),
        'medium'=> t('Medium -> 1/2/1392 - 2:30'),
        'long'  => t('Long -> sunday - 1/2/1392 - 2:30'),
        'custom'=> t('Custom'),
    ),
    '#default_value' => variable_get('jalalidate_nodes_format'),
  );

  $form['jalalidate_nodes_settings']['jalalidate_nodes_custom'] = array(
    '#type'   => 'textfield',
    '#title'  => t('Custom'),
    // This #states rule says that the "custom" textfield should only
    // be shown if the "format" form element is set to "custom".
    '#states' => array(
      'visible' => array(
        ':input[name="jalalidate_nodes_format"]' => array('value' => 'custom'),
      ),
    ),
    '#default_value' => variable_get('jalalidate_nodes_custom'),
    '#description' => "See This <a href='" . $url . "'>page</a> for more information about String patterns",
  );

  $form['jalalidate_nodes_test'] = array(
    '#type' => 'submit',
    '#value' => t('Verify requirements'),
    '#submit' => array('jalalidate_verify_requirements'),
   );

  return system_settings_form($form);
}

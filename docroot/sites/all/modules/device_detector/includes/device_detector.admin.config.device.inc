<?php
/**
 * @file
 * CEMHUB module administration settings.
 */

/**
 * Form constructor for the CEM HUB registration.
 */
function device_detector_device_settings_form($form, &$form_state) {
  // Fieldset contating all desktop related settings.
  $form['dsktp_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Desktop settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['dsktp_details']['device_detector_dsktp_common_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Common Class Name/Names for Desktop Device'),
    '#description' => t("Add class names, comma(,) separated for multiple class names to be added to every page on body<br />eg. classA,classB...<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_dsktp_common_class_name', ''),
    '#required' => FALSE,
  );
  // Fieldset contating all device related settings other than desktop.
  $form['othr_dvc_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other Device settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['othr_dvc_details']['device_detector_othr_dvc_common_class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Common Class Name/Names for Devices other than Desktop'),
    '#description' => t("Add class names, comma(,) separated for multiple class names to be added to every page on body<br />eg. classA,classB...<br />*Note: Don't include any <b>Number</b> or <b>Special Character</b>(eg.'\/~`!@#$%^&*()_-+=\{}[]|;:\"<>,.?) in your class name.!!"),
    '#default_value' => variable_get('device_detector_othr_dvc_common_class_name', ''),
    '#required' => FALSE,
  );
  $form['othr_dvc_details']['device_detector_othr_dvc_as_class_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Device Name as class Name</b>'),
    '#description' => t("Check this box to add the device name as the class name to body"),
    '#default_value' => variable_get('device_detector_othr_dvc_as_class_name', 0),
    '#required' => FALSE,
  );
  $form['othr_dvc_details']['device_detector_othr_dvc_os_as_class_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Add Device OS Name as class Name</b>'),
    '#description' => t("Check this box to add the device OS name as the class name to body"),
    '#default_value' => variable_get('device_detector_othr_dvc_os_as_class_name', 0),
    '#required' => FALSE,
  );
  $form['#validate'][] = 'device_detector_device_settings_form_validate';
  $form['#submit'][] = 'device_detector_device_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Implementing custom validate handler for the system form.
 */
function device_detector_device_settings_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['device_detector_dsktp_common_class_name']) && !preg_match(DEVICE_DETECTOR_CLASS_NAME_FORMAT_RULES, $form_state['values']['device_detector_dsktp_common_class_name'])) {
    form_set_error('device_detector_dsktp_common_class_name', t('Please provide Common Class Name/Names for Desktop Device in required format'));
  }
  elseif (device_detector_str_starts_with($form_state['values']['device_detector_dsktp_common_class_name'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_dsktp_common_class_name'], ",")) {
    form_set_error('device_detector_dsktp_common_class_name', t("Don't include ',' at the beginning or end of Common Class Name/Names for Desktop Device."));
  }
  if (!empty($form_state['values']['device_detector_othr_dvc_common_class_name']) && !preg_match(DEVICE_DETECTOR_CLASS_NAME_FORMAT_RULES, $form_state['values']['device_detector_othr_dvc_common_class_name'])) {
    form_set_error('device_detector_othr_dvc_common_class_name', t('Please provide Common Class Name/Names for Devices other than Desktop in required format'));
  }
  elseif (device_detector_str_starts_with($form_state['values']['device_detector_othr_dvc_common_class_name'], ",") || device_detector_str_ends_with($form_state['values']['device_detector_othr_dvc_common_class_name'], ",")) {
    form_set_error('device_detector_othr_dvc_common_class_name', t("Don't include ',' at the beginning or end of Common Class Name/Names for Devices other than Desktop."));
  }
}

/**
 * Implementing custom submit handler for the system form.
 */
function device_detector_device_settings_form_submit($form, &$form_state) {
  // Setting "device_detector_is_dsktp_settings_chkd" variable value.
  variable_set('device_detector_is_dsktp_settings_chkd', ((!empty($form_state['values']['device_detector_dsktp_common_class_name'])) ? TRUE : FALSE));
  // Setting "device_detector_is_dvc_settings_chkd" variable value.
  if ((!empty($form_state['values']['device_detector_othr_dvc_common_class_name'])) || (($form_state['values']['device_detector_othr_dvc_as_class_name']) == 1) || (($form_state['values']['device_detector_othr_dvc_os_as_class_name']) == 1)) {
    variable_set('device_detector_is_dvc_settings_chkd', TRUE);
  }
}

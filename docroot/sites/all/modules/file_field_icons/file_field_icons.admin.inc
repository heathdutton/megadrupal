<?php

/**
 * Form to list and select icon packages.
 */
function file_field_icons_form($form, &$form_state) {
  $packages = array(
    'default' => 'Drupal default',
    'metro' => _file_field_icons_display_icons('metro'),
    'flat' => _file_field_icons_display_icons('flat'),
  );

  $custom_packages = _file_field_icons_get_custom_packages();
  if ($custom_packages) {
    $custom_packages_path = _file_field_icons_get_custom_packages_path();

    foreach($custom_packages as $custom_package_key => $custom_package_value) {
      $packages[$custom_package_key] = _file_field_icons_display_icons($custom_package_value, TRUE);
    }


  }

  $default_package = variable_get('file_field_icons_package', 'default');

  $form['package'] = array(
    '#type' => 'radios',
    '#title' => t('Available packages'),
    '#options' => $packages,
    '#default_value' => $default_package,
    '#description' => t('Select the package. To add custom packages see README.txt'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler to save the selected icon package.
 */
function file_field_icons_form_submit($form, &$form_state) {
  variable_set('file_field_icons_package', $form_state['values']['package']);

  drupal_set_message(t('Package \'!package\' set.', array('!package' => $form_state['values']['package'])));
}

/**
 * Display a list of file icons.
 */
function _file_field_icons_display_icons($package, $custom = FALSE) {
  if (!$custom) {
    $package_path = _file_field_icons_get_core_packages_path();
  }
  else {
    $package_path = _file_field_icons_get_custom_packages_path();
  }
  $package_path = $package_path . '/' . $package;

  if (is_dir($package_path)) {
    $get_icons = array(
      'text-plain.png',
      'application-pdf.png',
      'x-office-document.png',
      'x-office-spreadsheet.png',
    );

    foreach ($get_icons as $key => $value) {
      $icons[] = array('data' => theme('image', array('path' => $package_path . '/' . $value)));

    }

    return $package . theme('item_list', array('items' => $icons, 'attributes' => array('class' => 'inline')));
  }
}


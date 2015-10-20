<?php

/**
 * Configuration options for Headroom.js.
 * @param $form
 * @param $form_state
 * @return mixed
 */
function headroomjs_admin_settings_form($form, &$form_state) {
  if (!(libraries_get_path('headroomjs') && file_exists(libraries_get_path('headroomjs') . '/headroom.js'))) {
    drupal_set_message(t('Headroom.js plugin was not detected. See the !link for more information.', array('!link' => l('status report', 'admin/reports/status'))), 'error', FALSE);
  }

  $form['headroomjs_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Offset'),
    '#description' => t('The vertical offset (in pixels) before element is first unpinned.'),
    '#default_value' => variable_get('headroomjs_offset', 5),
    '#element_validate' => array('element_validate_integer')
  );

  $form['headroomjs_tolerance'] = array(
    '#type' => 'textfield',
    '#title' => t('Tolerance'),
    '#description' => t('The scroll tolerance (in pixels) before state changes.'),
    '#default_value' => variable_get('headroomjs_tolerance', 200),
    '#element_validate' => array('element_validate_integer')
  );

  $form['headroomjs_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Header Element'),
    '#description' => t('The HTML element that Headroom.js should attach to. Defaults to the header tag.'),
    '#default_value' => variable_get('headroomjs_selector', 'header')
  );

  $form['headroomjs_class_initial'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial Class'),
    '#description' => t('The CSS class applied to the HTML element when Headroom.js is initialized.'),
    '#default_value' => variable_get('headroomjs_class_initial', 'headroom')
  );

  $form['headroomjs_class_pinned'] = array(
    '#type' => 'textfield',
    '#title' => t('Pinned Class'),
    '#description' => t('The CSS class applied to the HTML element when the element is pinned.'),
    '#default_value' => variable_get('headroomjs_class_pinned', 'headroom--pinned')
  );

  $form['headroomjs_class_unpinned'] = array(
    '#type' => 'textfield',
    '#title' => t('Unpinned Class'),
    '#description' => t('The CSS class applied to the HTML element when the element is unpinned.'),
    '#default_value' => variable_get('headroomjs_class_unpinned', 'headroom--unpinned')
  );

  $form['headroomjs_minified'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Minified Files'),
    '#description' => t('Whether or not to load the minified versions of the javascript files. This is recommended.'),
    '#default_value' => variable_get('headroomjs_minified', TRUE)
  );

  return system_settings_form($form);
}
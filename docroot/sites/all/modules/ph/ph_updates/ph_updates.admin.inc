<?php
/**
 * @file
 * Updates feed administrative UI implementation.
 */

/**
 * Updates feed settings form.
 */
function ph_updates_admin_settings($form, &$form_state) {
  $format_options = array();
  foreach (ph_updates_get_format_info() as $name => $info) {
    $format_options[$name] = $info['label'];
  }
  $form['ph_updates_feed_format'] = array(
    '#type' => 'select',
    '#title' => t('Feed format'),
    '#options' => $format_options,
    '#default_value' => variable_get('ph_updates_feed_format'),
    '#required' => TRUE,
    '#description' => t('Select the feed format to output at the specified feed path.'),
  );
  $form['ph_updates_feed_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed path'),
    '#default_value' => variable_get('ph_updates_feed_path'),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
    '#element_validate' => array('ph_updates_feed_path_element_validate'),
    '#required' => TRUE,
    '#description' => t('Specify a suitable Drupal path for publishing updates feeds. A feed is accessed by appending to this feed path the project name and core API version, e.g. @path/%project/%core_api', array(
      '@path' => url(variable_get('ph_updates_feed_path', 'release-history'), array('absolute' => TRUE)),
      '%project' => 'mymodule',
      '%core_api' => '7.x',
    )),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'ph_updates_admin_settings_flush';

  return $form;
}

/**
 * Validates feed path input.
 */
function ph_updates_feed_path_element_validate($element, &$form_state) {
  $path = $element['#value'];
  if (!ph_updates_valid_feed_path($path)) {
    form_error($element, t('Specified feed path is not valid. The path must contain one or more segments joined by slashes ("/"). Each segment must contain only alphanumeric characters, dot ("."), underscores ("_"), and hyphens ("-").'));
  }
}

/**
 * Flushes cached information after saving settings.
 */
function ph_updates_admin_settings_flush() {
  // Rebuild feed data info.
  ph_updates_get_info(NULL, TRUE);
  rules_clear_cache();
}

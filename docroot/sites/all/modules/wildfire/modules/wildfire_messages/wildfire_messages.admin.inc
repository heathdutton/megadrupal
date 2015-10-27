<?php
/**
 * @file
 *  wildfire_messages.admin.inc
 *
 * Administrative pages for the Wildfire messages module.
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * Settings form for the content types page.
 */
function wildfire_settings_content_types_form($form, &$form_state) {
  $form = array();

  $form['wildfire_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types that may be emailed'),
    '#description' => t('Check all the node types that are allowed to be sent using the mailer.'),
    '#options' => wildfire_settings_content_types_options(),
    '#default_value' => variable_get('wildfire_content_types', array()),
  );

  return system_settings_form($form);
}

/**
 * Settings for the region settings page.
 */
function wildfire_settings_regions_form($form, &$form_state) {

  global $user;
  $formats = filter_formats($user);
  $options = array();

  foreach ($formats as $fid => $format) {
    $f_format = filter_format_load($fid);
    if (filter_access($f_format, $user)) {
      $options[$fid] = $format->name;
    }
  }

  $default_format_id = variable_get('wildfire_snippet_format_id', 0);

  if ($default_format_id == 0) {
    $default_format_id = filter_default_format();
  }

  $form['snippet_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Snippet format'),
  );
  $form['snippet_fieldset']['wildfire_snippet_format_id'] = array(
    '#type' => 'select',
    '#title' => t('Default format to use for snippet content'),
    '#description' => t('Defines what content is allowed in a snippet region based on the available formats.'),
    '#options' => $options,
    '#default_value' => variable_get('wildfire_snippet_format_id', $default_format_id),
  );

  return system_settings_form($form);

}

/**
 * Provides the checkbox options for choosing content types for use with mailer.
 *
 * @return array
 *    An associative array whose keys are the machine-readable content types
 *    currently on the system and whose values are the human-readable content
 *    types.
 */
function wildfire_settings_content_types_options() {
  $nodetypes = node_type_get_types();
  $returntypes = array();

  foreach ($nodetypes as $nodetype => $val) {
    $returntypes[$nodetype] = $val->name;
  }

  return $returntypes;
}

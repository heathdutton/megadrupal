<?php

/**
 * @file
 * Admin page callbacks for the Webform Hints module.
 */

/**
 * Menu callback; displays the Webform hints module settings page.
 *
 * @ingroup forms
 * @see webform_hints_admin_settings_submit()
 */
function webform_hints_admin_settings($form, &$form_state) {
  // Query the database for all webform nodes.
  $options = array();
  $result = db_query('SELECT w.nid, n.title FROM {webform} w, {node} n WHERE w.nid = n.nid');
  foreach ($result as $webform) {
    $options[$webform->nid] = $webform->title;
  }

  if (empty($options)) {
    drupal_set_message(t('You have yet to add any Webforms to this site. Once you do, you can configure which forms will receive automatic placeholders here. <a href="@admin-url">Create your first Webform</a> to get started.', array('@admin-url' => url('node/add/webform'))), 'warning');
  }

  // Build checkbox list of results with ability to apply/remove Webform Hints.
  $form['webform_hints_forms'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Webforms'),
    '#default_value' => variable_get('webform_hints_forms', array()),
    '#options' => $options,
    '#description' => t('Select the webform nodes you would like to apply the "Webform Hints" effect to. On the selected nodes, the titles of textfields, textareas, select fields, and email fields will appear as placeholder text within the respective fields. The labels of these fields will be visible only to screenreaders.'),
  );

  $form['webform_hints_reverse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply "Webform Hints" effect to all webforms except those selected'),
    '#default_value' => variable_get('webform_hints_reverse', 0),
    '#description' => t('By default the "Webform Hints" effect is applied only to selected webforms. Check this if you want the effect to be applied to all webforms except those selected above.'),
  );

  $form['webform_hints_required'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#title' => t('Required indicator'),
    '#default_value' => variable_get('webform_hints_required', WEBFORM_HINTS_DEFAULT_REQUIRED_INDICATOR),
    '#description' => t('This is appended to the hint of a required label. It can be left blank. The element can also be themed using the .required class (see <a href="http://www.w3.org/TR/WCAG/#visual-audio-contrast">WCAG</a>).'),
  );

  $form['webform_hints_legacy_support'] = array(
    '#type' => 'checkbox',
    '#title' => t('Legacy Browser Support'),
    '#default_value' => variable_get('webform_hints_legacy_support', ''),
    '#description' => t('Webform hints uses the HTML5 "placeholder" attribute for setting default/placeholder values on webform inputs. This module includes support for legacy browsers (like IE 9 and below) by enabling this option. This requires downloading and installing the Form Defaults jQuery plugin by Jason Palmer. See README.txt for installation instructions.'),
  );

  $form['webform_hints_use_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use description instead of label'),
    '#default_value' => variable_get('webform_hints_use_description', FALSE),
    '#description' => t('Selecting this option will use each component\'s description value as the hint rather than the label. The description will otherwise be hidden while the label will remain visible to end users. The required indicator (setting above) will not be attached to this hint, as the label will contain a required marker.'),
  );

  $form = system_settings_form($form);

  return $form;
}

/**
 * Form validations for system settings.
 */
function webform_hints_admin_settings_validate($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['webform_hints_legacy_support'] && !webform_hints_form_defaults_installed()) {
    form_set_error('webform_hints_legacy_support', t('The Form Defaults jQuery plugin needs to be installed to enable legacy support.'));
  }
}

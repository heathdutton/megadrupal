<?php

/**
 * @file
 *
 * Parish bulletin administration pages.
 */

/**
 * Parish bulletin configuration form definition.
 */
function parish_bulletin_configuration_form() {
  $form['parish_bulletin_import_on'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically import bulletins'),
    '#description' => t('Enable to have bulletins automatically added to this site.'),
    '#default_value' => variable_get('parish_bulletin_import_on', 0),
  );

  $form['parish_bulletin_import_day'] = array(
    '#type' => 'select',
    '#title' => t('Import Day of Week'),
    '#options' => array(
        0 => t('Sunday'),
        1 => t('Monday'),
        2 => t('Tuesday'),
        3 => t('Wednesday'),
        4 => t('Thursday'),
        5 => t('Friday'),
        6 => t('Saturday'),
    ),
    '#title' => t('Automatically import bulletins'),
    '#description' => t('The day of the week during which bulletins should be imported. For most parishes, this should be set to Saturday.'),
    '#default_value' => variable_get('parish_bulletin_import_day', 6),
  );

  $form['parish_bulletin_import_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Bulletin import URL'),
    '#description' => t('The location of your bulletin PDF file for download.'),
    '#default_value' => variable_get('parish_bulletin_import_url', ''),
    '#size' => 80,
    '#maxlength' => 128,
    '#element_validate' => array('_parish_bulletin_import_url_validate'),
    '#states' => array(
      'invisible' => array(
        'input[name="parish_bulletin_import_on"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['parish_bulletin_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Auto-generated bulletin node title'),
    '#description' => t('Automatically-imported bulletin node titles (which are only visible in administrative content listings) are set to this value. If you need to use tokens in bulletin titles, consider using the !auto_nodetitle module for bulletin node title customization.', array('!auto_nodetitle' => l('Automatic Nodetitles', 'https://www.drupal.org/project/auto_nodetitle', array('external' => TRUE)))),
    '#default_value' => variable_get('parish_bulletin_title', t('Bulletin')),
    '#size' => 60,
    '#maxlength' => 255,
    '#states' => array(
      'invisible' => array(
        'input[name="parish_bulletin_import_on"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['parish_bulletin_verify_ssl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verify SSL certificates'),
    '#description' => t("Leave this enabled unless you're having trouble downloading bulletins due to HTTP error 0 or otherwise know what you're doing."),
    '#default_value' => variable_get('parish_bulletin_verify_ssl', TRUE),
    '#states' => array(
      'invisible' => array(
        'input[name="parish_bulletin_import_on"]' => array('checked' => FALSE),
      ),
    ),
  );

  return system_settings_form($form);
}

function parish_bulletin_configuration_form_validate($form, &$form_state) {
  // If the 'Automatically import bulletins' option is checked, ensure that
  // the bulletin import URL is filled in.
  if ($form_state['values']['parish_bulletin_import_on'] && empty($form_state['values']['parish_bulletin_import_url'])) {
    form_set_error('parish_bulletin_import_url', 'To enable parish bulletin importing, please enter the URL where your bulletins may be downloaded.');
  }
}

/**
 * Validation for the parish bulletin import URL.
 */
function _parish_bulletin_import_url_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    // Make sure the URL is valid.
    if (!valid_url($element['#value'], TRUE)) {
      form_set_error($element['#name'], 'Please enter a valid URL. (Example: <em>http://www.church.org/bulletins/xyz.pdf</em>)');
    }
    // Make sure the URL is for a PDF file.
    elseif (strpos($element['#value'], '.pdf') === FALSE) {
      form_set_error($element['#name'], 'The URL must be for a PDF file.');
    }
  }
}

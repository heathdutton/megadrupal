<?php
/**
 * @file
 * Admin pages for the "Out of date" module.
 */

/**
 * Admin configuration form for the outofdate module.
 */
function outofdate_admin_form() {
  $form = array();


  $form['global'] = array();
  $form['global']['outofdate_content_age'] = array(
    '#type' => 'select',
    '#title' => t('Content age'),
    '#description' => t('Display the "Out of date" message on content older than this.'),
    '#options' => outofdate__get_interval_options(variable_get('outofdate_content_age', OUTOFDATE_DEFAULT_AGE)),
    '#default_value' => variable_get('outofdate_content_age', OUTOFDATE_DEFAULT_AGE),
  );
  $form['global']['outofdate_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#description' => t('Content substitutions available: <br />@interval - the age of the content<br />@contenttype - the human-readable name of the content-type.<br />@contenttype_lower - the human-readable name of the content-type, in lower-case.'),
    '#default_value' => variable_get('outofdate_message', OUTOFDATE_DEFAULT_MESSAGE),
  );

  // @TODO: show where the interval or message has been overridden for a
  // particular content-type.

  return system_settings_form($form);
}


/**
 * Get the options available
 *
 * @param Int $current_setting
 *   The current setting (time in seconds).
 *
 * @return Array
 *   A keyed array of options to use in FormAPI.
 */
function outofdate__get_interval_options($current_setting) {
  $options = array(
        3600 => t('1 hour'),
       86400 => t('1 day'),
      604800 => t('1 week'),
     2592000 => t('1 month'),
     7776000 => t('3 months'),
    15552000 => t('6 months'),
    31536000 => t('1 year'),
  );
  
  // If the current setting isn't one of the options, it's been customised
  // (e.g. through drush vset). Add the current setting as an additional
  // option, so the user can maintain their previous choice.
  if (!array_key_exists($current_setting, $options)) {
    $options = array(
      t('Default choices') => $options,
      t('Custom setting') => array(
        $current_setting => format_interval($current_setting),
      ),
    );
  }

  return $options;
}

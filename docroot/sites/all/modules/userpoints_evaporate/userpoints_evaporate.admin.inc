<?php
// $Id: userpoints_evaporate.admin.inc,v 1.1 2010/12/21 01:04:33 cafuego Exp $
/**
 * Display the settings form.
 */
function userpoints_evaporate_admin_settings() {

  drupal_set_title(t('Evaporation'));

  $options = array(
    USERPOINTS_EVAPORATE_HOUR => t('Once an hour'),
    USERPOINTS_EVAPORATE_DAY => t('Once a day'),
    USERPOINTS_EVAPORATE_WEEK => t('Once a week'),
    USERPOINTS_EVAPORATE_MONTH => t('Once a month'),
  );

  // Use the API. APIs are win.
  $terms = userpoints_get_categories();

  // Grab existing settings.
  $settings = variable_get('userpoints_evaporate', array());

  $form['userpoints_evaporate'] = array(
    '#tree' => TRUE,
  );

  // Duplicate the evaporate settings for each term, and add an on/off radio button.
  //
  foreach ($terms as $tid => $term) {
    // Load default settings, so as to avoid PHP warnings.
    if (empty($settings[$tid])) {
      $settings[$tid] = _userpoints_evaporate_default_settings();
    }

    $translation = userpoints_translation();
    $translation['%term'] = $translation['!term'] = $term;

    $form['userpoints_evaporate'][$tid] = array(
      '#type' => 'fieldset',
      '#title' => t('!term !points', $translation),
      '#description' => t('Control evaporation settings for %term !points', $translation),
      '#collapsible' => TRUE,
      '#collapsed' => ($settings[$tid]['enabled']) ? FALSE : TRUE,
    );

    $form['userpoints_evaporate'][$tid]['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => 'Enabled',
      '#description' => t('Control whether or not %term !points should evaporate.', $translation),
      '#options' => array(0, 1),
      '#default_value' => $settings[$tid]['enabled'],
    );

    $form['userpoints_evaporate'][$tid]['interval'] = array(
      '#title' => 'Interval',
      '#description' => t('How quickly do %term !points evaporate?', $translation),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $settings[$tid]['interval'],
    );

    $form['userpoints_evaporate'][$tid]['number'] = array(
      '#title' => 'Number',
      '#description' => t('How many %term !points evaporate at once?', $translation),
      '#type' => 'select',
      '#options' => array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 =>8, 9 => 9, 10 => 10),
      '#default_value' => $settings[$tid]['number'],
    );

    $form['userpoints_evaporate'][$tid]['description'] = array(
      '#title' => 'Description',
      '#description' => t('An optional transaction description to be added in "My points" when %term !points are evaporated.', $translation),
      '#type' => 'textfield',
      '#default_value' => $settings[$tid]['description'],
    );

    $form['userpoints_evaporate'][$tid]['nonegative'] = array(
      '#type' => 'checkbox',
      '#title' => 'No Negative',
      '#description' => t('If checked, %term !points will not evaporate if the total would end up being a negative number.', $translation),
      '#options' => array(0, 1),
      '#default_value' => $settings[$tid]['nonegative'],
    );

    $form['userpoints_evaporate'][$tid]['inactive'] = array(
      '#title' => 'Inactive',
      '#type' => 'fieldset',
    );

    $form['userpoints_evaporate'][$tid]['inactive']['enabled'] = array(
      '#title' => 'Inactive Users Only',
      '#description' => t('Should users be inactive for a certain time before they start losing %term !points?', $translation),
      '#type' => 'checkbox',
      '#options' => array(0, 1),
      '#default_value' => $settings[$tid]['inactive']['enabled'],
    );


    $form['userpoints_evaporate'][$tid]['inactive']['delta'] = array(
      '#title' => 'Inactivity Interval',
      '#description' => t('Enter a positive number.'),
      '#type' => 'textfield',
      '#size' => 3,
      '#default_value' => $settings[$tid]['inactive']['delta'],
    );

    $form['userpoints_evaporate'][$tid]['inactive']['unit'] = array(
      '#title' => 'Inactivity Unit',
      '#type' => 'select',
      '#options' => array(
        USERPOINTS_EVAPORATE_HOUR => t('Hour(s)'),
        USERPOINTS_EVAPORATE_DAY => t('Day(s)'),
        USERPOINTS_EVAPORATE_WEEK => t('Week(s)'),
        USERPOINTS_EVAPORATE_MONTH => t('Month(s)'),
      ),
      '#default_value' => $settings[$tid]['inactive']['unit'],
    );
  }

  return system_settings_form($form);
}

/**
 * Validation handler for the settings form.
 */
function userpoints_evaporate_admin_settings_validate($form, &$form_state) {
  $terms = userpoints_get_categories();

  foreach ($terms as $tid => $term) {
    if (!empty($form_state['values']['userpoints_evaporate'][$tid]['inactive']['enabled'])) {
      if (!is_numeric($form_state['values']['userpoints_evaporate'][$tid]['inactive']['delta']) || intval($form_state['values']['userpoints_evaporate'][$tid]['inactive']['delta']) < 1) {
        form_set_error('userpoints_evaporate][' . $tid . '][inactive][delta', t('Please enter a positive number'));
      }
    }
  }
}

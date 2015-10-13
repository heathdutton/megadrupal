<?php
// $Id: almanac.admin.inc,v 1.1 2011/02/18 10:13:31 nhwebworker Exp $
/**
 * @file
 * Administration callbacks.
 */

/**
 * Custom form for the administration of the almanac module.
 * @see almanac_admin_validate(), system_settings_form()
 */
function almanac_admin() {
  $settings = almanac_settings();

  $form['almanac'] = array('#tree' => TRUE);
  $form['almanac']['location'] = array(
    '#type' => 'fieldset',
    '#title' => t('Location'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['almanac']['location']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $settings['location']['name'],
    '#size' => 60,
    '#maxlength' => 128,
    '#description' => t("The name of the location for which data will be computed."),
    '#required' => TRUE,
  );
  $form['almanac']['location']['latitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#default_value' => $settings['location']['latitude'],
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => t("Location latitude in decimal notation. Use minus sign (-) for southern hemisphere."),
    '#required' => TRUE,
  );
  $form['almanac']['location']['longitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#default_value' => $settings['location']['longitude'],
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => t("Location logitude in decimal notation. Use minus sign (-) for eastern hemisphere."),
    '#required' => TRUE,
  );

  $form['almanac']['location']['timezone'] = array(
    '#type' => 'select',
    '#title' => t('Location Time Zone'),
    '#options' => system_time_zones(),
    '#default_value' => $settings['location']['timezone'],
    '#description' => t("Time zone of location. This is used to display all dates for this location."),
    '#required' => TRUE,
  );

  $form['almanac']['datetime'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dates and times'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = array();
  foreach (system_get_date_types() as $type) {
    $options[$type['type']] = $type['title'];
  }

  $form['almanac']['datetime']['block'] = array(
    '#type' => 'select',
    '#title' => t('Block format'),
    '#options' => $options,
    '#default_value' => $settings['datetime']['block'],
    '#description' => t("The date time format to use in the block."),
    '#required' => TRUE,
  );
  $form['almanac']['datetime']['page'] = array(
    '#type' => 'select',
    '#title' => t('Page format'),
    '#options' => $options,
    '#default_value' => $settings['datetime']['page'],
    '#description' => t("The date time format to use in the page."),
    '#required' => TRUE,
  );
  $form['almanac']['datetime']['date'] = array(
    '#type' => 'select',
    '#title' => t('Date format'),
    '#options' => $options,
    '#default_value' => $settings['datetime']['date'],
    '#description' => t("The date format. Used by moon phases."),
    '#required' => TRUE,
  );
  $form['almanac']['datetime']['time'] = array(
    '#type' => 'select',
    '#title' => t('Time format'),
    '#options' => $options,
    '#default_value' => $settings['datetime']['time'],
    '#description' => t("The time format. Widespread use, especially for rise and set times."),
    '#required' => TRUE,
  );

  $options = array('hours' => t('Hours'), 'minutes' => t('Minutes'), 'seconds' => t('Seconds'));
  $form['almanac']['datetime']['period'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Period components'),
    '#options' => $options,
    '#default_value' => $settings['datetime']['period'],
    '#description' => t("Used for the page display of periods."),
    '#required' => TRUE,
  );
  $form['almanac']['datetime']['period_short'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Short period components'),
    '#options' => $options,
    '#default_value' => $settings['datetime']['period_short'],
    '#description' => t("Used for the block display of periods."),
    '#required' => TRUE,
  );
  $form['almanac']['moon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Moon'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['almanac']['moon']['full'] = array(
    '#type' => 'textfield',
    '#title' => t('Known full moon date'),
    '#default_value' => $settings['moon']['full'],
    '#size' => 25,
    '#maxlength' => 25,
    '#description' => t("Set base date that we know was a full moon. Eg: 'July 15 2011 06:40 UTC'"),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Validate admin settings form.
 */
function almanac_admin_validate($form, &$form_state) {
  $longitude = $form_state['values']['almanac']['location']['longitude'];
  $latitude = $form_state['values']['almanac']['location']['latitude'];
  if (!is_numeric($longitude) || !is_numeric($latitude)) {
    form_set_error('almanac_coordinates', t('You must enter a numeric value from -180 to 180 for longitude and -90 to 90 for latitude'));
  }
  if (abs($longitude) > 180 || abs($latitude) > 90) {
    form_set_error('almanac_coordinates', t('You must enter a numeric value from -180 to 180 for longitude and -90 to 90 for latitude'));
  }
  //TODO: full moon validation
}

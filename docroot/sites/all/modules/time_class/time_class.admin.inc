<?php

/**
 * @file
 * Main file for the Time Class module.
 */

define('TIME_CLASS_VAR_PREFIX', 'time_class_');

/**
 * Form constructor for the Time Class configuration page form.
 */
function time_class_form($form, $form_state) {
  $defaults = variable_get('time_class', array());

  if (!isset($form_state['custom_events_cnt'])) {
    if (!empty($defaults['custom_events'])) {
      $new_class_count = 0;
      foreach ($defaults['custom_events'] as $value) {
        if (is_array($value)) {
          $new_class_count++;
        }
        $form_state['custom_events_cnt'] = $new_class_count;
      }
    }
    else {
      $form_state['custom_events_cnt'] = 1;
    }
  }

  $form['time_class'] = array(
    '#type' => 'fieldset',
    '#title' => t('Time Class Settings'),
    '#tree' => TRUE,
    '#description' => t('Select the appropriate classes:'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['time_class']['season'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a season class'),
    '#description' => t('It allows you to add season class,for example, "autumn".'),
    '#default_value' => $defaults['season'],
  );
  $form['time_class']['hemisphere'] = array(
    '#type' => 'radios',
    '#title' => t('Your hemisphere:'),
    '#description' => t("Choose earth's hemisphere."),
    '#default_value' => $defaults['hemisphere'],
    '#states' => array(
      'visible' => array(
        ':input[name="time_class[season]"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
    '#options' => array(0 => t('North'), 1 => t('South')),
    '#prefix' => '<div style="padding-left: 20px;">',
    '#suffix' => '</div>',
  );
  $form['time_class']['month'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a month class'),
    '#description' => t('It allows you to add month class,for example, "june".'),
    '#default_value' => $defaults['month'],
  );
  $form['time_class']['day_of_week'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a day of week class'),
    '#description' => t('It allows you to add day of the week class,for example, "monday".'),
    '#default_value' => $defaults['day_of_week'],
  );
  $form['time_class']['time_of_day'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a time of day class'),
    '#description' => t('It allows you to add time of day class,for example, "morning".'),
    '#default_value' => $defaults['time_of_day'],
  );
  $form['time_class']['events'] = array(
    '#type' => 'fieldset',
    '#title' => t('Events Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['time_class']['events']['start_end'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#description' => t('You can set the number of days before and after the selected date during which will be displayed the selected class(-es).<br>If these fields are not filled, the class will only be displayed on the relevant date.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['time_class']['events']['start_end']['start_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Start in'),
    '#size' => 3,
    '#description' => '',
    '#default_value' => $defaults['events']['start_end']['start_date'],
    '#prefix' => '<div style="float: left; display: inline-block; margin-right: 15px;">',
    '#suffix' => '</div>',
  );
  $form['time_class']['events']['start_end']['end_date'] = array(
    '#type' => 'textfield',
    '#title' => t('End after'),
    '#size' => 3,
    '#description' => '',
    '#default_value' => $defaults['events']['start_end']['end_date'],
    '#prefix' => '<div style="float: left; display: inline-block;">',
    '#suffix' => '</div>',
  );
  $form['time_class']['events']['valentines_day'] = array(
    '#type' => 'checkbox',
    '#title' => t("Add a Valentine's day class"),
    '#description' => t('It allows you to add Valentine\'s day class.'),
    '#default_value' => $defaults['events']['valentines_day'],
    '#prefix' => '<div style="clear: both;">',
    '#suffix' => '</div>',
  );
  $form['time_class']['events']['christmas'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a Christmas day class'),
    '#description' => t('It allows you to add Christmas class.'),
    '#default_value' => $defaults['events']['christmas'],
  );
  $form['time_class']['events']['new_year'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a New Year day class'),
    '#description' => t('It allows you to add New Year class.'),
    '#default_value' => $defaults['events']['new_year'],
  );

  $form['time_class']['custom_events'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Events Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div id="custom-events-wrapper">',
    '#suffix' => '</div>',
  );

  for ($i = 0; $i < $form_state['custom_events_cnt']; $i++) {

    // Set default variables.
    $class_name = '';
    $start_day = '';
    $end_day = '';
    $status = FALSE;

    // Get values from database if exist.
    if (isset($defaults['custom_events'][$i]) && is_array($defaults['custom_events'][$i])) {
      $class_name = $defaults['custom_events'][$i]['class_name'];
      $start_day = $defaults['custom_events'][$i]['start_day'];
      $end_day = $defaults['custom_events'][$i]['end_day'];
      $status = $defaults['custom_events'][$i]['status'];
    }

    // Set default variables for elements title.
    $fieldset_title = $class_name ? t('@class Class Settings', array('@class' => $class_name)) : t('Add Custom Class');
    $remove_class_title = $class_name ? t('Remove @class', array('@class' => $class_name)) : t('Remove custom class');

    $form['time_class']['custom_events'][$i] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($fieldset_title),
      '#description' => t('You can set the start and end dates.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );
    $form['time_class']['custom_events'][$i]['class_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom Class'),
      '#description' => t('Enter your custom class name.'),
      '#default_value' => $class_name,
    );
    $form['time_class']['custom_events'][$i]['start_day'] = array(
      '#type' => 'textfield',
      '#title' => t('Start Date'),
      '#size' => 9,
      '#attributes' => array(
        'class' => array('datepicker'),
      ),
      '#default_value' => $start_day,
      '#prefix' => '<div style="float: left; display: inline-block; margin-right: 15px;">',
      '#suffix' => '</div>',
    );
    $form['time_class']['custom_events'][$i]['end_day'] = array(
      '#type' => 'textfield',
      '#title' => t('End Date'),
      '#size' => 9,
      '#attributes' => array(
        'class' => array('datepicker'),
      ),
      '#default_value' => $end_day,
      '#prefix' => '<div style="float: left; display: inline-block;">',
      '#suffix' => '</div>',
    );
    $form['time_class']['custom_events'][$i]['status'] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($remove_class_title),
      '#description' => t('Select to remove the custom class.'),
      '#default_value' => $status,
      '#prefix' => '<div style="clear: both">',
      '#suffix' => '</div>',
    );
  }

  $form['time_class']['custom_events']['add_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add another class'),
    '#submit' => array('time_class_add_more_form_add'),
    '#ajax' => array(
      'wrapper' => 'custom-events-wrapper',
      'callback' => 'time_class_add_more_form_update',
    ),
  );

  $form['#attached']['library'][] = array('system', 'ui.datepicker');
  $form['#attached']['js'][] = drupal_get_path('module', 'time_class') . '/js/time_class.js';
  $form['#validate'][] = 'time_class_validate';
  $form['#submit'][] = 'time_class_submit';

  return system_settings_form($form);
}

/**
 * Add more button submit callback.
 */
function time_class_add_more_form_add($form, &$form_state) {
  $new_class_count = 1;
  foreach ($form_state['values']['time_class']['custom_events'] as $value) {
    if (is_array($value)) {
      $new_class_count++;
    }
  }
  $form_state['custom_events_cnt'] = $new_class_count;
  $form_state['rebuild'] = TRUE;
}

/**
 * Add more button ajax callback.
 */
function time_class_add_more_form_update($form, $form_state) {
  return $form['time_class']['custom_events'];
}

/**
 * Form validation handler for time_class_form().
 */
function time_class_validate($form, &$form_state) {
  // Checking if number is entered.
  if (!is_numeric($form_state['values']['time_class']['events']['start_end']['start_date'])) {
    form_set_error('start_date', t('Value in field "Start Date" is wrong. It should be a number.'));
  }

  // Checking if number is entered.
  if (!is_numeric($form_state['values']['time_class']['events']['start_end']['end_date'])) {
    form_set_error('end_date', t('Value in field "End Date" is wrong. It should be a number.'));
  }

  // Validation for custom events.
  foreach ($form_state['values']['time_class']['custom_events'] as $key => $value) {
    if (is_array($value)) {
      foreach ($value as $k => $v) {
        if (!empty($v)) {
          $today = strtotime(date('d.m.y'));

          // Check for empty fields in custom event item.
          if (empty($value['class_name']) || empty($value['start_day']) || empty($value['end_day'])) {
            form_set_error('time_class][custom_events][' . $key . '][class_name', t('All fields must be filled.'));
            form_set_error('time_class][custom_events][' . $key . '][start_day');
            form_set_error('time_class][custom_events][' . $key . '][end_day');
          }
          // Check that the end date is more than the start date.
          elseif (strtotime($value['end_day']) <= strtotime($value['start_day'])) {
            form_set_error('time_class][custom_events][' . $key . '][start_day', t('Value in field "Start Date" can not be larger than the value in "End Date".'));
            form_set_error('time_class][custom_events][' . $key . '][end_day');
          }
          // Check that the end date is more than the current date.
          if ($today > strtotime($value['end_day'])) {
            form_set_error('time_class][custom_events][' . $key . '][end_day', t('You can not set a "End Date" is less than the current date.'));
          }
        }
        else {
          // Check for empty fields in previous custom event item.
          // So that the user does not create a large number of items.
          if (($k != 'status') && ($k === reset($value))) {
            form_set_error('time_class][custom_events][' . $key . '][class_name', t('Please, fill previous item.'));
            form_set_error('time_class][custom_events][' . $key . '][start_day');
            form_set_error('time_class][custom_events][' . $key . '][end_day');
          }
        }
      }
    }
  }
}

/**
 * Form submit handler for time_class_form().
 */
function time_class_submit($form, &$form_state) {
  $new_values = array();
  foreach ($form_state['values']['time_class']['custom_events'] as $key => $value) {
    if (is_array($value)) {
      if ($value['status'] == 1) {
        // Remove selected custom event item.
        unset($form_state['values']['time_class']['custom_events'][$key]);
      }
      elseif (!empty($value['class_name']) && !empty($value['start_day']) && !empty($value['end_day'])) {
        // Set new values from custom event item into the temporary array.
        $new_values[] = $value;
      }
    }
  }

  // Update 'custom_events' array.
  $form_state['values']['time_class']['custom_events'] = array_values($new_values);
  // Update 'time_class' values in database.
  variable_set('time_class', $form_state['values']['time_class']);
}

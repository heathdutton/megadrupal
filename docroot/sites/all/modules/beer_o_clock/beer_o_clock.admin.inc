<?php

/**
 * @file
 * Provides the administration settings for Beer O'Clock.
 */

/**
 * The administration settings form
 */
function beer_o_clock_settings($form, $form_state) {
  // Sunday is day zero to co-incide with PHP.net
  $form['beer_o_clock_day'] = array(
    '#type' => 'select',
    '#title' => "Beer O'Clock Day of the week",
    '#description' => "The day on which Beer O'Clock is on",
    '#options' => array(
      0 => t('Sunday'),
      1 => t('Monday'),
      2 => t('Tuesday'),
      3 => t('Wednesday'),
      4 => t('Thursday'),
      5 => t('Friday'),
      6 => t('Saturday'),
    ),
    '#default_value' => variable_get('beer_o_clock_day', 5),
    '#required' => TRUE,
  );
  $form['beer_o_clock_hour'] = array(
    '#type' => 'select',
    '#title' => t("Beer O'Clock Hour of the day"),
    '#description' => t("The time that Beer O'Clock starts - in 24 hour time format"),
    '#options' => range(0, 23),
    '#default_value' => variable_get('beer_o_clock_hour', 16),
    '#required' => TRUE,
  );
  $form['beer_o_clock_message'] = array(
    '#title' => t("Message to display when it is Beer O'Clock"),
    '#type' => 'textarea',
    '#description' => t("You can use most common HTML tags"),
    '#default_value' => variable_get('beer_o_clock_message', t('It sure is!')),
  );
  $form['beer_o_clock_not_message'] = array(
    '#title' => t("Message to display when it is not Beer O'Clock"),
    '#type' => 'textarea',
    '#description' => t("You can use most common HTML tags"),
    '#default_value' => variable_get('beer_o_clock_not_message', t('Not yet buddy!')),
  );

  return system_settings_form($form);
}

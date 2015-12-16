<?php
/**
 * @file
 * Contact notes admin settings include file.
 */

function contact_centre_admin_settings($form, &$form_state) {
  $form = array();

  $options_yes_no = array(
    0 => t('No'),
    1 => t('Yes'),
  );

  $date_formats = system_get_date_types();
  $date_types = array();
  foreach ($date_formats as $key => $date_format) {
    $date_types[$key] = $date_format['title'] . ' (' . format_date(REQUEST_TIME, $key) . ')';
  }

  $form['contact_centre_show_unopened'] = array(
    '#type' => 'radios',
    '#title' => t('Show "unopened messages" count on user log in?'),
    '#default_value' => variable_get('contact_centre_show_unopened', 1),
    '#options' => $options_yes_no,
    '#description' => t('If set to yes, any user with administer contact centre permission will see a count of unopened messages when they login.'),
  );

  $form['contact_centre_show_profile'] = array(
    '#type' => 'radios',
    '#title' => t('Show "unopened messages" count on user profile?'),
    '#default_value' => variable_get('contact_centre_show_profile', 0),
    '#options' => $options_yes_no,
    '#description' => t('If set to yes, any user with administer contact centre permission will see a count of unopened messages on their profile page.'),
  );

  $form['contact_centre_date_format'] = array(
    '#type' => 'select',
    '#title' => t('Date display format'),
    '#default_value' => variable_get('contact_centre_date_format', 'medium'),
    '#options' => $date_types,
    '#description' => t('Select the format for date display in the contact centre.'),
  );

  $form['contact_centre_messages_per_page'] = array(
    '#type' => 'select',
    '#title' => t('Number of messages per page to show in contact centre'),
    '#default_value' => variable_get('contact_centre_messages_per_page', 15),
    '#options' => array(
      5   => '5',
      10  => '10',
      15  => '15',
      20  => '20',
      25  => '25',
    ),
  );

  return system_settings_form($form);
}

<?php
/**
 * @file
 * Administration settings for Seen.
 *
 * @copyright Copyright(c) 2014 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */


/**
 * Form callback for the Seen settings form.
 */
function seen_settings_form($form, &$form_state) {

  if (variable_get('statistics_enable_access_log', 0) == 0) {
    drupal_set_message(t('Access log statistics are currently disabled. Seen depends on the Access Log to track page views. !link.', array('!link' => l('Visit the Statistics settings to enable the access log', 'admin/config/system/statistics'))), 'warning');
  }

  $form['seen_show_all_users'] = array(
    '#type' => 'select',
    '#title' => t('Show all users when listing views'),
    '#default_value' => variable_get('seen_show_all_users', 0),
    '#options' => array(
      0 => t('No, show only users who viewed'),
      1 => t('Yes, show all users'),
    ),
    '#required' => TRUE,
  );
  $form['seen_list_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of rows to show on Seen pages'),
    '#default_value' => check_plain(seen_get_listing_row_limit()),
    '#maxlength' => 12,
    '#length' => 12,
    '#description' => t('Defaults to 25, if not set.'),
  );
  $form['seen_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Date format'),
    '#default_value' => check_plain(variable_get('seen_date_format', seen_default_date_format())),
    '#description' => t('A PHP Date format string. see !link', array(
      '!link' => l('http://au1.php.net/manual/en/function.date.php', 'http://au1.php.net/manual/en/function.date.php'),
    )),
  );
  $form['seen_tick'] = array(
    '#type' => 'textfield',
    '#title' => t('Tick mark'),
    '#default_value' => variable_get('seen_tick', '&#10004;'),
    '#description' => t('HTML escaped code to use to indicate a record has been viewed.'),
  );

  return system_settings_form($form);
}



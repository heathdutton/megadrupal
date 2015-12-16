<?php

/**
 * @file
 * Administrative page callbacks for Service Links module.
 */

/**
 * Configuration form for the general settings for Social Stats module.
 */
function social_stats_general_settings_form($form, &$form_state) {
  global $base_url;  
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js("jQuery(document).ready(function(){
      jQuery('.pickadate').datepicker({
        dateFormat: 'dd/mm/yy',
        autoSize: true,
        inline: true,
      });
    });",
  'inline');

  $form['basic_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
    '#weight' => -1,
  );

  $form['basic_settings']['social_stats_url_root'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Root'),
    '#description' => t('Root of the URL to check against social media. For instance, "https://mysite.com" (no trailing slash). This is useful for fetching production site stats from a different environment.'),
    '#default_value' => variable_get('social_stats_url_root', $base_url),
    '#size' => 60,
    '#maxlength' => 254,
  );

  $form['basic_settings']['social_stats_options'] = array(
    '#type' => 'radios',
    '#title' => t('Select options'),
    '#options' => array('Start Date', 'Offset',),
    '#default_value' => variable_get('social_stats_options', ''),
  );

  $form['basic_settings']['social_stats_start_date'] = array(
    '#title' => t('Start Date (MM/DD/YYYY)'),
    '#type' => 'textfield',
    '#maxlength' => 20,
    '#attributes' => array('class' => array('pickadate')),
    '#default_value' => variable_get('social_stats_start_date', '01/01/1970'),
    '#description' => t('The oldest date from which the statistics should be retrieved.'),
    '#states' => array(
      'visible' => array(
        ':input[name="social_stats_options"]' => array('value' => 0),
      ),
    ),
  );

  $form['basic_settings']['social_stats_date_offset'] = array(
    '#title' => t('Date Offset'),
    '#type' => 'textfield',
    '#maxlength' => 20,
    '#size' => 20,
    '#default_value' => variable_get('social_stats_date_offset', '-100 days'),
    '#description' => t('The days offset from which the stats should be retrieved.'),
    '#states' => array(
      'visible' => array(
        ':input[name="social_stats_options"]' => array('value' => 1),
      ),
    ),
  );
  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron configuration'),
    '#weight' => 0,
  );
  $form['configuration']['social_stats_cron_interval'] = array(
    '#type' => 'select',
    '#title' => t('Cron interval'),
    '#description' => t('Time after which social data should be collected.'),
    '#default_value' => variable_get('social_stats_cron_interval', 60 * 60 * 24),
    '#options' => array(
      60 => t('1 minute'),
      300 => t('5 minutes'),
      3600 => t('1 hour'),
      60 * 60 * 6 => t('6 hours'),
      60 * 60 * 12 => t('12 hours'),
      60 * 60 * 24 => t('1 day'),
      60 * 60 * 24 * 7 => t('1 week'),
      60 * 60 * 24 * 7 * 2 => t('2 weeks'),
      60 * 60 * 24 * 7 * 4 => t('1 month'),
    ),
  );

  $form['configuration']['social_stats_cron_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron duration'),
    '#description' => t('Time (in secs) for which the queue should execute.'),
    '#default_value' => variable_get('social_stats_cron_duration', 300),
    '#size' => 3,
    '#maxlength' => 3,
  );

  $form['#submit'] = array('social_stats_general_settings_form_submit');

  return system_settings_form($form);
}

/**
 * Submit function to adjust the next cron execution time if the value changes.
 */
function social_stats_general_settings_form_submit($form, &$form_state) {
  $orig_interval = variable_get('social_stats_cron_interval', 0);
  $new_interval = $form_state['values']['social_stats_cron_interval'];
  if ($orig_interval != $new_interval) {
    variable_set('social_stats_next_execution', REQUEST_TIME + $new_interval);
  }
}

/**
 * Configuration form for listing of the content types against the social media.
 */
function social_stats_content_types_form() {
  $node_types = node_type_get_types();
  $i = 0;
  foreach ($node_types as $types) {
    $form['social_stats'][$i] = array(
      '#type' => 'fieldset',
      '#title' => filter_xss_admin($types->name),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['social_stats'][$i]['social_stats_' . $types->type] = array(
      '#type' => 'checkboxes',
      '#options' => drupal_map_assoc(array(
        t('Facebook'),
        t('Twitter'),
        t('Google Plus'),
        t('LinkedIn'),
      )),
      '#default_value' => variable_get('social_stats_' . $types->type, array(0)),
    );
    $i++;
  }
  return system_settings_form($form);
}

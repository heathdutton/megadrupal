<?php
/**
 * @file
 * Provides a settings form for the Tables Size module.
 */


/**
 * Builds the Table Size settings form.
 */
function table_size_admin_settings_form($form, $form_state) {
  $settings = LazyVars::get('table_size_cron_settings', NULL);

  $form['data'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $form['data']['run'] = array(
    '#title' => t('The Table Size cron routine should'),
    '#type' => 'radios',
    '#options' => TableSizeUtil::getRunOptions(),
    '#default_value' => $settings ? $settings['run'] : TableSizeCron::RUN_NEVER,
  );

  $form['data']['min'] = array(
    '#title' => t('Interval start'),
    '#title_display' => 'before',
    '#type' => 'textfield',
    '#default_value' => $settings ? $settings['min'] : '02:15',
    '#id' => 'table-size-min',
    '#size' => 4,
  );

  $form['data']['max'] = array(
    '#title' => t('Interval end'),
    '#title_display' => 'after',
    '#type' => 'textfield',
    '#default_value' => $settings ? $settings['max'] : '05:15',
    '#id' => 'table-size-max',
    '#size' => 4,
  );

  $form['data']['email'] = array(
    '#title' => t('Email'),
    '#type' => 'textfield',
    '#default_value' => $settings ? $settings['email'] : $GLOBALS['user']->mail,
    '#size' => 30,
    '#required' => TRUE,
    '#description' => t('On every Table Size cron routine, the module will send a status report to this email address.'),
  );

  $langs = language_list();
  $language_options = array();
  foreach ($langs as $k => $l) {
    $language_options[$k] = $l->name . ' (' . $l->native . ')';
  }
  $form['data']['language'] = array(
    '#title' => t('Language'),
    '#type' => 'select',
    '#options' => $language_options,
    '#default_value' => $settings ? $settings['language'] : $GLOBALS['language']->language,
  );

  $form['actions'] = array(
    '#type' => 'container',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $path = drupal_get_path('module', 'table_size');
  if (file_exists('sites/all/libraries/base/jquery-ui-timepicker/jquery.ui.timepicker.js')) {
    $form['#attached'] = array(
      'js' => array(
        'misc/ui/jquery.ui.widget.min.js',
        'misc/ui/jquery.ui.position.min.js',
        'sites/all/libraries/base/jquery-ui-timepicker/jquery.ui.timepicker.js',
        $path . '/js/admin_settings.js',
      ),
      'css' => array(
        'sites/all/libraries/base/jquery-ui-timepicker/jquery.ui.timepicker.css',
        $path . '/css/admin_settings.css',
      ),
    );
  }
  $form['#attached']['css'][] = $path . '/css/admin_settings.css';

  return $form;
}

/**
 * Ajax callback for the hour min element.
 */
function table_size_admin_settings_form_ajax($form, $form_state) {
  return $form['data']['max'];
}


/**
 * Validation handler for the Table Size settings form.
 */
function table_size_admin_settings_form_validate(&$form, $form_state) {
  $v = $form_state['values']['data'];
  $v['min'] = ltrim($v['min'], ' 0');
  $v['max'] = ltrim($v['max'], ' 0');

  $min_parts = explode(':', $v['min']);
  $max_parts = explode(':', $v['max']);
  if (count($min_parts) != 2) {
    form_set_error('data][min', t('Invalid time.'));
  }
  if (count($max_parts) != 2) {
    form_set_error('data][max', t('Invalid time.'));
  }

  if (!form_get_errors() && $min_parts[0]*60 + $min_parts[1] >= $max_parts[0]*60 + $max_parts[1]) {
    form_set_error('data][min', t('Invalid interval.'));
    $form['data']['max']['#attributes']['class'][] = 'error';
  }

  if (!valid_email_address($v['email'])) {
    form_set_error('data][email', t('Invalid email.'));
  }
}

/**
 * Submit handler for the Table Size settings form.
 */
function table_size_admin_settings_form_submit($form, $form_state) {
  $v = $form_state['values']['data'];
  LazyVars::set('table_size_cron_settings', $v);
  drupal_set_message(t('The alert settings were successfully saved.'));
}
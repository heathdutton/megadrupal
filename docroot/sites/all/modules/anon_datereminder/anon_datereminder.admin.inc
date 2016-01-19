<?php
/**
 * @file
 * Forms for administrative settings.
 */

/**
 * Build form to set anon_datereminder adminstrative settings.
 *
 * @return array
 *   The form for settings.
 */
function anon_datereminder_settings_form() {
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  $form = array();

  $form['anon_datereminder_cron_frequency'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron frequency in minutes'),
    '#size' => 5,
    '#default_value' => variable_get('anon_datereminder_cron_frequency',
      ANON_DATEREMINDER_CRON_FREQUENCY),
    '#description' => t('How often does cron run, in minutes? This affects when reminders are sent. Reminders will be sent for anything due before the next expected cron run.'),
  );

  $form['anon_datereminder_max_reminders'] = array(
    '#type' => 'textfield',
    '#title' => t('Max reminders'),
    '#size' => 3,
    '#default_value' => variable_get('anon_datereminder_max_reminders',
      ANON_DATEREMINDER_MAX_REMINDERS),
    '#description' => t('Maximum number of reminders a visitor can request for one node'),
  );

  $form['anon_datereminder_min_add'] = array(
    '#type' => 'textfield',
    '#title' => t('Add this many at once'),
    '#size' => 2,
    '#default_value' => variable_get('anon_datereminder_min_add', 1),
    '#description' => t('Allow visitor to add this many reminders at once'),
  );

  $form['anon_datereminder_retain_expired'] = array(
    '#type' => 'textfield',
    '#title' => t('Days to retain reminder'),
    '#description' => t('How long after last occurence to keep record of reminders'),
    '#size' => 3,
    '#default_value' => variable_get('anon_datereminder_retain_expired', 14),
  );

  $form['anon_datereminder_display'] = array(
    '#type' => 'select',
    '#description' => t('Where should reminders appear when viewing a node?'),
    '#default_value' => variable_get('anon_datereminder_display', 'block'),
    '#title' => t('Reminder display'),
    '#options' => array(
      'block' => t('Create block'),
      'node' => t('Show reminder form inline on node'),
    ),
  );

  $form['anon_datereminder_theme'] = array(
    '#type' => 'select',
    '#description' => t('How would you like for the form to be displayed'),
    '#default_value' => variable_get('anon_datereminder_theme', 'fields'),
    '#title' => t('Reminder theme'),
    '#options' => array(
      'fields' => t('Display as standard fields'),
      'table' => t('Display fields in a table'),
    ),
  );

  module_load_include('inc', 'anon_datereminder', 'includes/anon_datereminder_form');
  $leads = _anon_datereminder_leadtime_list();
  $boxes = array();
  foreach ($leads as $l => $lbl) {
    $boxes["_l_$l"] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($lbl),
      '#default_value' => TRUE,
    );
  }
  $form['anon_datereminder_leadtime_list'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Specify allowed lead times'),
    '#description' => t('Check to retain'),
  );
  $form['anon_datereminder_leadtime_list'] += $boxes;

  $nval = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Add new lead time'),
    '#description' => t('Enter new lead time to add to the list'),
  );
  $nval['_added'] = array(
    '#type' => 'textfield',
    '#size' => 5,
  );
  $nval['_units'] = array(
    '#type' => 'select',
    '#description' => t('Select units'),
    '#options' => array(
      1 => t('Seconds'),
      60 => t('Minutes'),
      3600 => t('Hours'),
      86400 => t('Days'),
      604800 => t('Weeks'),
    ),
    '#default_value' => 3600,
  );
  $form['anon_datereminder_leadtime_list'][] = $nval;

  $form = system_settings_form($form);

  // Note: We do this *after* calling system_settings_form() so that
  // menu_rebuild() is called after the normal system
  // processing is done. (This is because anon_datereminder_menu generates
  // different menus depending on anon_datereminder_as_tab.)
  $form['#submit'][] = 'menu_rebuild';
  return $form;
}

/**
 * Validate the administrative settings.
 *
 * @param array $form
 *   Form just submitted
 * @param array $form_state
 *   And state thereof
 */
function anon_datereminder_settings_form_validate($form, &$form_state) {
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  $vals = $form_state['values'];
  $cron_rate = $vals['anon_datereminder_cron_frequency'];
  if (!is_numeric($cron_rate) || $cron_rate < 5) {
    form_set_error('anon_datereminder_cron_frequency',
            t('Cron frequency needs to be a number and at least 5'));
  }

  $maxrem = $vals['anon_datereminder_max_reminders'];
  if (!is_numeric($maxrem) || $maxrem < 1) {
    form_set_error('anon_datereminder_max_reminders',
            t('Must be a positive number'));
  }

  $minadd = $vals['anon_datereminder_min_add'];
  if (!is_numeric($minadd) || $minadd < 1) {
    form_set_error('anon_datereminder_min_add',
            t('Must be a positive number'));
  }

  $retdays = $vals['anon_datereminder_retain_expired'];
  if (!is_numeric($retdays) || (intval($retdays) < 0)) {
    form_set_error('anon_datereminder_retain_expired',
            t('Retention days must be non-negative numeric'));
  }

  $leads = array();
  $nv = $vals['_added'];
  if (isset($nv) && $nv != '' && $nv > 0) {
    if (!is_numeric($nv)) {
      form_set_error('_added',
              t('New lead time must be numeric'));
      unset($nv);
    }
    else {
      $nv *= $vals['_units'];
    }
  }
  else {
    unset($nv);
  }

  foreach ($vals as $l => $v) {
    if (drupal_substr($l, 0, 3) == '_l_') {
      // This is one of our lead times.
      unset($form_state['values'][$l]);
      if ($v) {
        $leads[] = drupal_substr($l, 3);
      }
    }
  }
  if (isset($nv)) {
    $leads[] = $nv;
  }
  rsort($leads, SORT_NUMERIC);
  $form_state['values']['anon_datereminder_leadtime_list'] = implode(',', $leads);

  unset($form_state['values']['_added']);
  unset($form_state['values']['_units']);
}

/**
 * Build form to configure email message to send.
 *
 * @returns array
 *   The email form.
 */
function anon_datereminder_email_form() {
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  module_load_include('inc', 'anon_datereminder', 'includes/mailer');
  $form = array();

  $form['anon_datereminder_mail_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail sender'),
    '#size' => 30,
    '#default_value' => _anon_datereminder_email_from(),
    '#description' => t('Address reminders should be from. Leave blank to use system default.'),
  );

  $form['anon_datereminder_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Reminder subject'),
    '#size' => 60,
    '#default_value' => _anon_datereminder_email_subject(),
    '#description' => t('Email subject. Use tokens.'),
  );

  $form['anon_datereminder_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Reminder body'),
    '#default_value' => _anon_datereminder_email_body(),
    '#description' => t('Body of reminder email. Use tokens.'),
  );

  $form['anon_datereminder_token_tree'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Available tokens'),
    '#theme' => 'token_tree',
    '#token_types' => array('anon_datereminder', 'node'),
  );

  $form = system_settings_form($form);
  return $form;
}

<?php
/**
 * @file
 * Form definition and handling function for OAI Harvester module
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implements hook_form().
 *
 * The add repository form object
 *
 * @return array
 *   The FAPI definition
 */
function oaiharvester_repository_add_form() {
  global $user;

  $form['name'] = array(
    '#title' => t('Name of repository'),
    '#type' => 'textfield',
    '#description' => 'The name of the harvestable OAI-PMH repository',
  );

  $form['oai_provider_url'] = array(
    '#title' => t('URL of repository'),
    '#type' => 'textfield',
    '#description' => 'The URL of the harvestable OAI-PMH repository',
  );

  $form['type'] = array(
    '#title' => t('Type of repository'),
    '#type' => 'radios',
    '#options' => array(
      OAIHARVESTER_PROVIDERTYPE_SERVER => t('Real OAI-PMH server'),
      OAIHARVESTER_PROVIDERTYPE_CACHE => t('Files in the local filesystem'),
    ),
    '#default_value' => OAIHARVESTER_PROVIDERTYPE_SERVER,
    '#description' => 'The type of repository',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit and validate'),
  );

  return $form;
} // oaiharvester_repository_add_form

/**
 * Validate the new repository form
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function oaiharvester_repository_add_form_validate($form, &$form_state) {
  if (empty($form_state['values']['name'])
    || $form_state['values']['name'] == '') {
    form_set_error('name', t('Name shouldn\'t be empty!'));
  }
  if (empty($form_state['values']['oai_provider_url'])
    || $form_state['values']['oai_provider_url'] == '') {
    form_set_error('oai_provider_url', t('URL shouldn\'t be empty!'));
  }
  else {
    $oai_provider_url = $form_state['values']['oai_provider_url'];
    $repository = _oaiharvester_provider_get_by_url($oai_provider_url);
    if ($repository !== FALSE) {
      form_set_error('oai_provider_url',
        t('This provider has been registered as !name. Provide an another URL!',
          array('!name' => l(
            $repository->name,
            'admin/xc/harvester/repository/' . $repository->provider_id
      ))));
    }
  }

  $form_state['redirect'] = 'admin/xc/harvester/repository/list';
} // oaiharvester_repository_add_form_validate

/**
 * Handle post-validation form submission of new repository form
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function oaiharvester_repository_add_form_submit($form, &$form_state) {
  global $user;

  $name = $form_state['values']['name'];
  $url  = $form_state['values']['oai_provider_url'];
  $type = $form_state['values']['type'];
  if (!isset($type) || $type == '') {
    $type = OAIHARVESTER_PROVIDERTYPE_SERVER;
  }

  $repository = (object) array(
    'name' => $name,
    'oai_provider_url' => $url,
    'type' => $type,
    'created_at' => format_date(time(), 'custom', 'Y-m-d H:i:s'),
    'updated_at' => format_date(time(), 'custom', 'Y-m-d H:i:s'),
    'user_id' => $user->uid
  );

  // Error at page http://xc-bui.carli.illinois.edu/xc-7x-1.x/drupal-7.17/admin/xc/harvester/repository/2/revalidate
  $ret_val = drupal_write_record('oaiharvester_providers', $repository);
  if ($ret_val == SAVED_NEW) {
    $msg = t('Successfully added repository "%name"', array('%name' => $repository->name));
    xc_log_info('harvester', $msg);
    drupal_set_message($msg);
    $form_state['redirect'] = 'admin/xc/harvester/repository/' . $repository->provider_id . '/revalidate';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new repository.'));
  }
  menu_rebuild();
}

/**
 * Form to edit (modify) an existing repository record
 *
 * @param $form_state
 * @param $repository
 * @return unknown_type
 */
function oaiharvester_repository_edit_form($form, &$form_state, $provider_id) {
  $repository = oaiharvester_repository_load($provider_id);
  $form = oaiharvester_repository_add_form();
  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $repository->provider_id
  );
  $form['name']['#value'] = $repository->name;
  $form['oai_provider_url']['#value'] = $repository->oai_provider_url;
  $form['type']['#default_value'] = $repository->type;

  $form['submit']['#value'] = t('Save and validate');
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
} // oaiharvester_repository_edit_form

/**
 * Form submission function for editing (modifying) a repository. After successfull
 * modification it redirects the user to revalidation
 *
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function oaiharvester_repository_edit_form_submit($form, &$form_state) {
  $values = $form_state['input'];

  $repository = (object) array(
    'provider_id' => $values['id'],
    'name' => $values['name'],
    'oai_provider_url' => $values['oai_provider_url'],
    'type' => $values['type'],
    'updated_at' => format_date(time(), 'custom', 'Y-m-d H:i:s')
  );

  if (empty($repository->type)) {
    $repository->type = OAIHARVESTER_PROVIDERTYPE_SERVER;
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    xc_log_info('harvester', 'do delete repository...');
    $sql = 'DELETE FROM {oaiharvester_providers} WHERE provider_id = %d';
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql, $repository->provider_id) */
    $result = db_delete('oaiharvester_providers')
              ->condition('provider_id', $repository->provider_id)
              ->execute();

    if ($result == 1) { // repository is deleted
      $msg = t('Repository %name has been removed', array('%name' => $repository->name));
      xc_log_info('harvester', $msg);
      drupal_set_message($msg);
      $form_state['redirect'] = 'admin/xc/harvester/repository/list';
    }
    else {
      $msg = t('Unexpected error. The remove repository command reported the deletion of %count repository.',
        array('%count' => $result));
      xc_log_error('harvester', $msg);
      drupal_set_message($msg, 'error');
    }
  }
  elseif ($form_state['clicked_button']['#value'] == t('Save and validate')) {
    $result = drupal_write_record('oaiharvester_providers', $repository, 'provider_id');

    if ($result == SAVED_UPDATED) { // repository is updated
      $msg = t('%name modified', array('%name' => $repository->name));
      xc_log_info('harvester', $msg);
      drupal_set_message($msg);
      $form_state['redirect'] = 'admin/xc/harvester/repository/' . $repository->provider_id . '/revalidate';
    }
    else {
      $msg = t('Unexpected error. Failed to modify repository.');
      xc_log_error('harvester', $msg);
      drupal_set_message($msg, 'error');
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Something else happened.'));
  }

  menu_rebuild();
} // oaiharvester_repository_edit_form_submit

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function oaiharvester_schedule_multiform($form, &$form_state = NULL, $schedule = NULL) {
  drupal_add_css(drupal_get_path('module', 'oaiharvester') . '/css/oaiharvester.css');

  // TODO: check
  $step = isset($form_state['values'])
        ? (int) $form_state['storage']['step']
        : (version_compare(VERSION, '6.14', '>=') ? 1 : 0);
  $form_state['storage']['step'] = $step + 1;

  switch ($step) {
    case 1:
      drupal_set_title(t('Step 1: Select repository and schedules #%t',
        array('%t' => $step)));
      $form = oaiharvester_schedule_step1_form();

      if ($schedule != NULL) {
        _oaiharvester_copy_step1_values_to_form($form, $schedule);
      }
      break;

    case 2:
      if (!isset($form_state['storage']['recurrence'])) {
        _oaiharvester_copy_values_to_storage($form_state);
      }
      drupal_set_title(t('Step 2: Name harvest schedule'));
      $schedule_id = !empty($form_state['storage']['schedule_id'])
                   ? $form_state['storage']['schedule_id']
                   : NULL;
      $form = oaiharvester_schedule_step2_form(NULL, $schedule_id,
        $form_state['storage']['provider_id'],
        $form_state['storage']['recurrence']);

      if ($schedule != NULL) {
        _oaiharvester_copy_step2_values_to_form($form, $schedule);
      }
      break;
  }

  $button_name = ($step < 2) ? t('Next') : t('Submit');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $button_name,
  );

  return $form;
}

function oaiharvester_schedule_multiform_validate($form, &$form_state) {
  switch ($form_state['storage']['step']) {
    case 2:
      if (empty($form_state['values']['recurrence'])) {
        form_set_error('recurrence', t('You should select the frequency of harvest'));
        $form_state['storage']['step'] = $form_state['storage']['step'] - 1;
      }
      break;
    case 3:
      $formats = $form_state['values']['format_id'];
      if (count($form['format_id']['#options']) > 1) {
        $formats_keys = array_keys($formats);
        if ((empty($formats) || (count($formats) == 1 && empty($formats_keys[0])))) {
          form_set_error('format_id', t('You should select a format'));
          $form_state['storage']['step'] = $form_state['storage']['step'] - 1;
        }
      }
      if (empty($form_state['values']['parsing_mode'])) {
        form_set_error('parsing_mode', t('You should select a parsing mode'));
        $form_state['storage']['step'] = $form_state['storage']['step'] - 1;
      }
      break;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function oaiharvester_schedule_multiform_submit($form, &$form_state) {
  global $user;

  $values = $form_state['values'];

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $schedule_id = $values['schedule_id'];
    $schedule = _oaiharvester_schedule_get($schedule_id);
    _oaiharvester_schedule_delete($schedule->harvest_schedule_id);
    _oaiharvester_schedule_steps_delete($schedule->harvest_schedule_id);
    drupal_set_message(t('%name removed', array('%name' => $schedule->schedule_name)));
    unset($form_state['storage']);
    $form_state['redirect'] = 'admin/xc/harvester/schedule/list';
    return;
  }

  if ($form_state['storage']['step'] < 3) {
    $form_state['rebuild'] = TRUE;
    return;
  }

  $storage  = $form_state['storage'];

  if (isset($values['schedule_id'])) {
    $schedule_id = $values['schedule_id'];
  }

  $schedule = new stdClass();
  $schedule->provider_id          = $storage['provider_id'];
  $schedule->recurrence           = $storage['recurrence'];
  $schedule->hour                 = $storage['hour'];
  $schedule->minute               = $storage['minute'];
  $schedule->day_of_week          = $storage['day_of_week'];
  $schedule->start_date           = $storage['start_date'];
  $schedule->end_date             = $storage['end_date'];
  $schedule->schedule_name        = $values['schedule_name'];
  $schedule->parsing_mode         = $values['parsing_mode'];
  $schedule->is_cacheable         = $values['is_cacheable'];
  $schedule->max_request          = (int) $values['max_request'];
  $schedule->skip_main_task       = $values['skip_main_task'];
  // $schedule->do_defrbrize         = $values['do_defrbrize'];
  $schedule->notify_email_address = $values['notify_email_address'];
  $schedule->created_date         = format_date(time(), 'custom', 'Y-m-d H:i:s');
  $schedule->created_by           = $user->name;
  $type = 'create_record';
  if (!empty($schedule_id)) {
    $type = 'update_record';
    $schedule->harvest_schedule_id = $schedule_id;
    drupal_write_record('oaiharvester_harvester_schedules', $schedule, 'harvest_schedule_id');
  }
  else {
    drupal_write_record('oaiharvester_harvester_schedules', $schedule);
  }
  $form_state['values']['schedule_id'] = $schedule->harvest_schedule_id;

  $sets = $values['set_id'];
  $formats = $values['format_id'];
  if ($type == 'create_record') {
    if (!empty($formats)) {
      foreach ($formats as $format_id) {
        if (!empty($sets)) {
          foreach ($sets as $set_id) {
            _oaiharvester_add_schedule_step($schedule->harvest_schedule_id, $format_id, $set_id);
          }
        }
        else {
          _oaiharvester_add_schedule_step($schedule->harvest_schedule_id, $format_id, NULL);
        }
      }
    }
    else {
      _oaiharvester_add_schedule_step($schedule->harvest_schedule_id, NULL, NULL);
    }
  }
  else { // 'update_record'
    $stored_steps = _oaiharvester_schedule_steps_get_atomic($schedule->harvest_schedule_id);
    $last_ran = $stored_steps[0]->last_ran;

    $new_steps = array();
    if (!empty($formats)) {
      foreach ($formats as $format_id) {
        if (!empty($sets)) {
          foreach ($sets as $set_id) {
            $new_steps[] = _oaiharvester_create_step($schedule->harvest_schedule_id, $format_id, $set_id, $last_ran);
          }
        }
        else {
          $new_steps[] = _oaiharvester_create_step($schedule->harvest_schedule_id, $format_id, NULL, $last_ran);
        }
      }
    }
    else {
      $new_steps[] = _oaiharvester_create_step($schedule->harvest_schedule_id, NULL, NULL, $last_ran);
    }
    $deletable = _oaiharvester_array_diff2($stored_steps, $new_steps);
    $additions = _oaiharvester_array_diff2($new_steps, $stored_steps);
    if (count($deletable) > 0) {
      foreach ($deletable as $step) {
        _oaiharvester_schedule_step_delete_object($step);
      }
    }
    if (count($additions) > 0) {
      foreach ($additions as $step) {
        _oaiharvester_add_schedule_step_object($step);
      }
    }
  }

  //unset($form_state['storage']);
  $form_state['redirect'] = 'admin/xc/harvester/schedule/' . $schedule->harvest_schedule_id;
  menu_rebuild();
}

/**
 * Create a form for adding create a schedule, step 1
 *
 * @return (array)
 *   The Drupal FAPI form array
 */
function oaiharvester_schedule_step1_form() {
  // the list of providers
  $provider_options = oaiharvester_provider_get_all_names();

  // the list of weekdays
  $weekday_options = array(
    1 => t('Monday'),
    2 => t('Tuesday'),
    3 => t('Wednesday'),
    4 => t('Thursday'),
    5 => t('Friday'),
    6 => t('Saturday'),
    0 => t('Sunday'),
  );
  // the list of recurrences
  $recurrences = array('Daily', 'Hourly', 'Weekly');
  // the list of hours
  $hour_options = array(
    '00:00',
    '01:00',
    '02:00',
    '03:00',
    '04:00',
    '05:00',
    '06:00',
    '07:00',
    '08:00',
    '09:00',
    '10:00',
    '11:00',
    '12:00',
    '13:00',
    '14:00',
    '15:00',
    '16:00',
    '17:00',
    '18:00',
    '19:00',
    '20:00',
    '21:00',
    '22:00',
    '23:00',
  );
  // the list of minutes
  $minute_options = array();
  for ($i = 0; $i < 60; $i++) {
    $minute_options[] = $i;
  }

  $form['provider_id'] = array(
    '#title' => t('Select repository'),
    '#type' => 'select',
    '#description' => 'The name of the harvestable OAI-PMH repository',
    '#options' => $provider_options,
  );

  $form['schedule'] = array(
    '#title' => t('Schedule'),
    '#type' => 'fieldset',
    '#description' => 'Select the frequency of harvest',
  );

  // hourly schedule
  $form['schedule']['hourly'] = array(
    '#title' => t('Hourly'),
    '#type' => 'fieldset',
  );
  $form['schedule']['hourly']['recurrence'] = array(
    '#title' => t('Hourly'),
    '#type' => 'radio',
    '#return_value' => $recurrences[1],
    '#post_render' => array('xc_util_delete_div_around'),
  );
  $form['schedule']['hourly']['minute'] = array(
    '#type' => 'select',
    '#title' => 'Run at',
    '#prefix' => ' &mdash; ',
    '#suffix' => ' minutes past the hour',
    '#options' => $minute_options,
    '#post_render' => array('xc_util_delete_div_around'),
  );

  // daily schedule
  $form['schedule']['daily'] = array(
    '#title' => t('Daily'),
    '#type' => 'fieldset',
  );
  $form['schedule']['daily']['recurrence'] = array(
    '#title' => t('Daily'),
    '#type' => 'radio',
    '#return_value' => $recurrences[0],
    '#post_render' => array('xc_util_delete_div_around'),
  );
  $form['schedule']['daily']['daily_hour'] = array(
    '#type' => 'select',
    '#title' => 'Run at',
    '#prefix' => ' &mdash; ',
    '#suffix' => ' each day',
    '#options' => $hour_options,
    '#post_render' => array('xc_util_delete_div_around'),
  );

  // weekly schedule
  $form['schedule']['weekly'] = array(
    '#title' => t('Weekly'),
    '#type' => 'fieldset',
  );
  $form['schedule']['weekly']['recurrence'] = array(
    '#title' => t('Weekly'),
    '#type' => 'radio',
    '#return_value' => $recurrences[2],
    '#post_render' => array('xc_util_delete_div_around'),
  );
  $form['schedule']['weekly']['day_of_week'] = array(
    '#type' => 'select',
    '#title' => 'Run every',
    '#prefix' => ' &mdash; ',
    '#options' => $weekday_options,
    '#post_render' => array('xc_util_delete_div_around'),
  );
  $form['schedule']['weekly']['weekly_hour'] = array(
    '#type' => 'select',
    '#title' => 'at',
    '#prefix' => ' ',
    '#options' => $hour_options,
    '#post_render' => array('xc_util_delete_div_around'),
  );

  $form['start_date'] = array(
    '#title' => t('Start date'),
    '#type' => 'date',
    '#default_value' => 0,
    '#post_render' => array('xc_util_delete_div_around'),
  );

  $form['end_date'] = array(
    '#title' => t('End date'),
    '#type' => 'date',
    '#default_value' => 0,
    '#post_render' => array('xc_util_delete_div_around'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
  );

  return $form;
} // oaiharvester_schedule_step1_form

/**
 * Get the step 2 form as raw array
 * @param $arg1
 * @param $harvest_schedule_id The schedule ID
 * @param $provider_id The data provider ID
 * @return The form array
 */
function oaiharvester_schedule_step2_form($arg1, $harvest_schedule_id, $provider_id = NULL, $recurrence = NULL) {
  global $user;

  $sets_options = array();
  $formats_options = array();

  // setting default values for schedule name and notify email address
  $default_notify_email_address = $user->mail;
  $provider = oaiharvester_repository_load($provider_id);
  if (!empty($harvest_schedule_id)) {
    $schedule = oaiharvester_schedule_load($harvest_schedule_id);
    if (isset($schedule->schedule_name)) {
      $schedule_name = $schedule->schedule_name;
    }
    else {
      $schedule_name = $provider->name . ' - ' . $schedule->recurrence;
    }
    if (isset($schedule->notify_email_address)) {
      $default_notify_email_address = $schedule->notify_email_address;
    }
  }
  elseif (!empty($recurrence)) {
    $schedule_name = $provider->name . ' - ' . $recurrence;
  }
  else {
    $schedule_name = $provider->name;
  }

  // get the list of sets
  $result = _oaiharvester_sets_get_by_provider($provider_id);
  $sets_options[''] = '-- Select a set --';
  foreach ($result as $data) {
    $sets_options[$data->set_id] = $data->display_name;
  }

  // get the list of formats
  $result = _oaiharvester_metadataformats_get_by_provider($provider_id);
  $formats_options[''] = '-- Select a format --';
  foreach ($result as $data) {
    $formats_options[$data->format_id] = $data->name;
  }
  $parsing_mode_options = array(
    'dom' => 'DOM processing (slower, but more robust)',
    'regex' => 'Regular expression based (quicker, but less robust)',
  );

  $form['schedule_name'] = array(
    '#title' => t('Schedule name'),
    '#type' => 'textfield',
    '#description' => 'The name of the schedule',
    '#default_value' => $schedule_name,
  );

  if (!empty($harvest_schedule_id)) {
    $form['schedule_id'] = array(
      '#type' => 'hidden',
      '#value' => $harvest_schedule_id,
    );
  }

  $form['notify_email_address'] = array(
    '#title' => t('Contact email'),
    '#type' => 'textfield',
    '#description' => 'Email of the contact person',
    '#default_value' => $default_notify_email_address,
  );

  $form['set_id'] = array(
    '#title' => t('Choose set'),
    '#type' => 'select',
    '#options' => $sets_options,
    '#description' => 'The set to harvest in this schedule',
  );
  if (count($sets_options) > 1) {
    $form['set_id']['#multiple'] = TRUE;
    $form['set_id']['#attributes']['size'] = 5;
  }

  $form['format_id'] = array(
    '#title' => t('Choose Metadata Format'),
    '#type' => 'select',
    '#options' => $formats_options,
    '#description' => 'The metadata prefix to harvest in this schedule',
  );
  if (count($formats_options) > 1) {
    $form['format_id']['#multiple'] = TRUE;
    $form['format_id']['#attributes']['size'] = 5;
  }

  $form['parsing_mode'] = array(
    '#title' => t('Choose Parsing Mode'),
    '#type' => 'select',
    '#options' => $parsing_mode_options,
    '#description' => 'How to process the XML response comes from data provider?',
    '#attributes' => array('size' => 3),
    '#default_value' => isset($schedule->parsing_mode) ? $schedule->parsing_mode : 'regex',
  );

  $form['is_cacheable'] = array(
    '#title' => t('Do harvester cache responses?'),
    '#type' => 'radios',
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#description' => 'Do the Drupal Toolkit cache the HTTP responses? It
      is useful only in testing phase, but could be misleading in a production
      server, because it does not reflect the changes on the OAI-PMH server.
      If you don\'t want to use the cache more, set it to now, and delete
      the cache directory. It is the default file directory\'s
      oaiharvester_http_cache subdirectory.',
    '#attributes' => array('size' => 3),
    '#default_value' => isset($schedule->is_cacheable) ? $schedule->is_cacheable : 0,
  );

  $form['max_request'] = array(
    '#title' => t('Limiting OAI-PMH request'),
    '#type' => 'textfield',
    '#size' => 4,
    '#description' => 'Use this field to limit the OAI-PMH requests. If you limit the requests,
      the harvester will not fetch all information, only the first some records. The number of records
      per requests is controlled by the data provider, we only could controll the number of requests.
      To fetch all records could be time consuming, and if you only want to test the toolkit or your
      indexing settings, it is good, if you can start with only a limited number of records.
      The default 0 value means no limit. Use other value only for testing reasons.',
    '#default_value' => isset($schedule->max_request) ? $schedule->max_request : 0,
  );

  $form['skip_main_task'] = array(
    '#title' => t('Skip main task, and run only additional steps (if any)'),
    '#type' => 'checkbox',
    '#description' => 'Skip harvest and runs only the processing of the harvested records with the help of other modules implemented the hook_oaiharvester_additional_harvest_steps() hook.',
    '#default_value' => isset($schedule->skip_main_task) ? $schedule->skip_main_task : 0,
  );

  /*
   $form['do_defrbrize'] = array(
   '#title' => t('Run \'preparing metadata for search\' step?'),
   '#type' => 'radios',
   '#options' => array(0 => t('No'), 1 => t('Yes')),
   '#description' => 'If \'Yes\' after harvesting it starts preparing
   index structure for search, i.e. merge work and expression fields into
   correspondent manifestation.',
   '#default_value' => 0,
   );
   */

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Schedule'),
  );

  return $form;
} // oaiharvester_schedule_step2_form

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function oaiharvester_schedule_resettime_form($form, &$form_state, $schedule) {

  $id = $schedule->harvest_schedule_id;

  return confirm_form(
    array(
    'schedule_id' => array(
      '#type' => 'hidden',
      '#default_value' => $id,
    ),
  ),
    t('Are you sure, that you would like to reset the last run date?'),
    'admin/xc/harvester/schedule/' . $id, // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Reset time'),
    t('Cancel')
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function oaiharvester_schedule_resettime_form_submit($form, &$form_state) {
  $schedule_id = $form_state['values']['schedule_id'];
  $schedule    = _oaiharvester_schedule_get($schedule_id);
  drupal_set_message(t('reseting time of last ran for schedule %name',
    array('%name' => $schedule->schedule_name)));
  $steps = _oaiharvester_schedule_steps_get($schedule_id);

  foreach ($steps as $step) {
    drupal_set_message(t('reseting time of last ran for step of %set set in %format format.',
      array(
        '%format' => $step->name,
        '%set' => $step->set_spec,
    )));
    _oaiharvester_schedule_step_reset($step->step_id);
  }
  $form_state['redirect'] = 'admin/xc/harvester/schedule/' . $schedule_id;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function oaiharvester_schedule_emptyqueue_form($form, &$form_state) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => '<p>' . t('Are you sure, that you would like to empty the harvest queue? (Currently there are %count items in queue)', array('%count' => _oaiharvester_queue_count())) . '<p>',
    '#default_value' => t('Yes'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function oaiharvester_schedule_emptyqueue_form_submit($form, &$form_state) {
  $db_affected_rows = _oaiharvester_queue_delete_all();
  drupal_set_message(t('Removed %rows items from the harvester queue.',
    array('%rows' => $db_affected_rows)));
  $form_state['redirect'] = 'admin/xc/harvester/schedule';
}

function oaiharvester_schedule_unlock_form() {
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => '<p>' . t('Are you sure, that you would like to unlock harvests?') . '<p>',
    '#default_value' => t('Yes'),
  );

  return $form;
}

function oaiharvester_schedule_unlock_form_submit($form, &$form_state) {
  variable_set('oaiharvester_processing_cron', 'NOT_RUNNING');
  drupal_set_message(t('Harvests were unlocked by setting the oaiharvester_processing_cron variable to default.'));
  $form_state['redirect'] = 'admin/xc/harvester/schedule';
}

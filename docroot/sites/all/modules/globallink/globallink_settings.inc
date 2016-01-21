<?php

/**
 * Returns form for globallink_pd_settings page.
 */
function globallink_settings_page() {
  $array = array();

  $array[] = drupal_get_form('globallink_pd_settings');
  $array[] = drupal_get_form('globallink_adaptor_settings');

  return $array;
}

/**
 * Renders form for globallink_pd_settings.
 */
function globallink_pd_settings() {
  $form = array();

  $form['globallink_pd_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('GlobalLink Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['globallink_pd_settings']['globallink_pd_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' => variable_get('globallink_pd_url', ''),
    '#required' => TRUE,
    '#size' => 50
  );

  $form['globallink_pd_settings']['globallink_pd_username'] = array(
    '#type' => 'textfield',
    '#title' => t('User Id'),
    '#default_value' => variable_get('globallink_pd_username', ''),
    '#required' => TRUE,
    '#size' => 20
  );

  $form['globallink_pd_settings']['globallink_pd_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => TRUE,
    '#size' => 20
  );

  $form['globallink_pd_settings']['globallink_pd_projectid'] = array(
    '#type' => 'textfield',
    '#title' => t('Project Short Code(s)'),
    '#description' => t('Enter comma separated codes for multiple projects.'),
    '#default_value' => variable_get('globallink_pd_projectid', ''),
    '#required' => TRUE,
    '#size' => 50
  );

  $form['globallink_pd_settings']['globallink_pd_submissionprefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Submission Name Prefix'),
    '#default_value' => variable_get('globallink_pd_submissionprefix', 'DRU_'),
    '#required' => FALSE,
    '#size' => 20
  );

  $form['globallink_pd_settings']['globallink_pd_classifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Classifier'),
    '#default_value' => variable_get('globallink_pd_classifier', 'XML'),
    '#required' => TRUE,
    '#size' => 10
  );

  $form['globallink_pd_settings']['globallink_pd_max_target'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Target Count'),
    '#default_value' => variable_get('globallink_pd_max_target', '99999'),
    '#required' => TRUE,
    '#size' => 10
  );

  $form['globallink_pd_settings']['submit_pd_test'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Test Settings'),
    '#suffix' => '</div>'
  );

  return $form;
}

/**
 * Handles validation of globallink_pd_settings form.
 */
function globallink_pd_settings_validate(&$form, &$form_state) {
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_common');

  $project_short_code_char_arr = str_split(str_replace(' ', '', $form_state['values']['globallink_pd_projectid']));

  foreach ($project_short_code_char_arr as $val) {
    if (!preg_match('/[a-zA-Z0-9\,_]/', $val)) {
      form_set_error('', t('Only enter comma seperated GlobalLink Project Short Code.'));

      return FALSE;
    }

    if ($val === strtolower($val) && !is_numeric($val) && $val != ',' && $val != '_' && $val != '') {
      form_set_error('', t('GlobalLink Project Short Code is in lowercase.'));

      return FALSE;
    }
  }

  $project_short_code_arr = explode(',', str_replace(' ', '', $form_state['values']['globallink_pd_projectid']));

  foreach ($project_short_code_arr as $val) {
    if (count(array_keys($project_short_code_arr, $val)) > 1) {
      form_set_error('', t('GlobalLink Project Short Code has duplicates.'));

      return FALSE;
    }
  }

  $value = $form_state['values']['globallink_pd_max_target'];

  if (!is_numeric($value)) {
    form_set_error('globallink_pd_max_target', t('Max Target Count is not a number.'));

    return FALSE;
  }
  elseif ($value < 1) {
    form_set_error('globallink_pd_max_target', t('Max Target Count should be greater than 1.'));

    return FALSE;
  }

  $prefix_len = strlen($form_state['values']['globallink_pd_submissionprefix']);

  if ($prefix_len > 118) {
    form_set_error('globallink_pd_max_target', t('Submission Name cannot be longer than 118 characters. Currently ' . $prefix_len . ' characters long.'));

    return FALSE;
  }

  if (isset($form_state['values']['globallink_pd_password']) && $form_state['values']['globallink_pd_password'] != '') {
    try {
      $pd_obj = new ProjectDirector();

      $url = strrev($form_state['values']['globallink_pd_url']);

      if (ord($url) == 47) {
        $url = substr($url, 1);
      }

      $r_url = strrev($url);

      $pd_obj->url = $r_url;
      $pd_obj->username = $form_state['values']['globallink_pd_username'];
      $pd_obj->password = $form_state['values']['globallink_pd_password'];
      $pd_obj->projectShortCode = $form_state['values']['globallink_pd_projectid'];
      $pd_obj->submissionPrefix = $form_state['values']['globallink_pd_submissionprefix'];
      $pd_obj->classifier = $form_state['values']['globallink_pd_classifier'];
      $pd_obj->maxTargetCount = $form_state['values']['globallink_pd_max_target'];

      $success = globallink_test_pd_connectivity($pd_obj);

      if (!is_bool($success)) {
        form_set_error('', t($success));
      }
    }
    catch (SoapFault $se) {
      watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
      form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
    }
    catch (Exception $e) {
      watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
      form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
    }
  }
}

/**
 * Handles submission of globallink_pd_settings form.
 */
function globallink_pd_settings_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  switch ($op) {
    case t('Save PD Settings'):
      foreach ($form_state['values'] as $key => $value) {
        if (is_array($value) && isset($form_state['values']['array_filter'])) {
          $value = array_keys(array_filter($value));
        }

        variable_set($key, $value);
      }

      globallink_save_project_names();
      drupal_set_message(t('The configuration options have been saved.'));

      break;
    case t('Save and Test Settings'):
      try {
        foreach ($form_state['values'] as $key => $value) {
          if (is_array($value) && isset($form_state['values']['array_filter'])) {
            $value = array_keys(array_filter($value));
          }

          variable_set($key, $value);
        }

        globallink_save_project_names();
        drupal_set_message(t('Settings Saved and Connection Test Successful.'));
      }
      catch (SoapFault $se) {
        watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
        form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
      }
      catch (Exception $e) {
        watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
        form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
      }

      break;
  }
}

/**
 * Renders form for globallink_adaptor_settings.
 */
function globallink_adaptor_settings() {
  $form = array();

  $form['globallink_adaptor_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Adaptor Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['globallink_adaptor_settings']['globallink_pager_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Dashboard Pager Limit'),
    '#default_value' => variable_get('globallink_pager_limit', 10),
    '#required' => TRUE,
    '#size' => 20,
  );

  $form['globallink_adaptor_settings']['globallink_enable_preview'] = array(
    '#type' => 'radios',
    '#title' => t('Enable Preview For Receive Translations'),
    '#default_value' => variable_get('globallink_enable_preview', 1),
    '#options' => array(t('No'), t('Yes')),
  );

  $form['globallink_adaptor_settings']['globallink_implementation_type'] = array(
    '#type' => 'radios',
    '#title' => t('Node/Field Translation Filter Implementation'),
    '#default_value' => variable_get('globallink_implementation_type', 0),
    '#description' => t('Changing this value will remove and reload the contents on the Dashboard based on the selected value. Please make sure that there are no active submissions.'),
    '#options' => array(t('Standard'), t('Using Hook (Refer documentation and hook_sample.php)')),
  );

  $form['globallink_adaptor_settings']['globallink_publish_node'] = array(
    '#type' => 'radios',
    '#title' => t('Publish Translated Content'),
    '#default_value' => variable_get('globallink_publish_node', 0),
    '#options' => array(t('No'), t('Yes'), t('Use Source Content Setting')),
  );

  $form['globallink_adaptor_settings']['globallink_cron_type'] = array(
    '#type' => 'radios',
    '#title' => t('Automatic Update Status'),
    '#default_value' => variable_get('globallink_cron_type', 0),
    '#options' => array(t('Disabled'), t('Drupal Cron'), t('External Cron (globallink_cron.php)')),
  );

  $form['globallink_adaptor_settings']['globallink_proxy_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy URL'),
    '#default_value' => variable_get('globallink_proxy_url', ''),
    '#required' => FALSE,
    '#size' => 50,
  );

  $form['globallink_adaptor_settings']['submit_general_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Adaptor Settings')
  );

  return $form;
}

/**
 * Exports log.
 */
function globallink_view_log_submit($form, &$form_state) {
  $from_date = $form_state['input']['log_view_date'];
  $from_date = globallink_convert_date_to_timestamp($from_date) / 1000;

  $destination = 'temporary://' . 'globallink_log.html';
  $my_file_obj = file_unmanaged_save_data(globallink_get_log_data($from_date), $destination, FILE_EXISTS_REPLACE);

  file_transfer($my_file_obj, array(
    'Content-Type' => 'application/htm',
    'Content-Disposition' => 'inline; filename="' . 'globallink_log.html' . '"',
    'Content-Length' => filesize($my_file_obj),
  ));
}

/**
 * Gets log data from specific date in the form of HTML.
 *
 * @param string $from_date
 *   The timestamp.
 *
 * @return string
 *   HTML representation of log data.
 */
function globallink_get_log_data($from_date) {
  $html = '<html><head><title>GlobalLink Log</title><style>.datagrid table { border-collapse: collapse; text-align: left; width: 100%; } .datagrid {font: normal 12px/150% Arial, Helvetica, sans-serif; background: #fff; overflow: hidden; border: 1px solid #006699; -webkit-border-radius: 3px; -moz-border-radius: 3px; border-radius: 3px; }.datagrid table td, .datagrid table th { padding: 3px 10px; }.datagrid table thead th {background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #006699), color-stop(1, #00557F) );background:-moz-linear-gradient( center top, #006699 5%, #00557F 100% );filter:progid:DXImageTransform.Microsoft.gradient(startColorstr="#006699", endColorstr="#00557F");background-color:#006699; color:#FFFFFF; font-size: 15px; font-weight: bold; border-left: 1px solid #0070A8; } .datagrid table thead th:first-child { border: none; }.datagrid table tbody td { color: #00496B; border-left: 1px solid #E1EEF4;font-size: 12px;font-weight: normal; }.datagrid table tbody .alt td { background: #E1EEF4; color: #00496B; }.datagrid table tbody td:first-child { border-left: none; }.datagrid table tbody tr:last-child td { border-bottom: none; }</style></head><body>';

  $result = db_select('globallink_log', 'tl')
  ->fields('tl')
  ->condition('timestamp', $from_date, '>=')
  ->execute();

  $html .= '<div class="datagrid"><table>';
  $html .= '<thead><tr><th>Date Time</th><th>Log Severity</th><th>Type</th><th>Message</th></tr></thead><tbody>';

  $i = 0;

  foreach ($result as $row) {
    $date = format_date($row->timestamp, 'custom', 'Y-m-d H:i:s');
    $mod = $i % 2;

    if ($mod == 0) {
      $html .= '<tr><td style="width:10%;">' . $date . '</td><td style="width:10%;">' . $row->severity . '</td><td style="width:10%;">' . $row->type . '</td><td style="width:70%; text-style:justify;">' . $row->message . '</td></tr>';
    }
    else {
      $html .= '<tr class="alt"><td style="width:10%;">' . $date . '</td><td style="width:10%;">' . $row->severity . '</td><td style="width:10%;">' . $row->type . '</td><td style="width:70%; text-style:justify;">' . $row->message . '</td></tr>';
    }

    $i++;
  }

  $html .= '</tbody></table></div>';
  $html .= '</body></html>';

  return $html;
}

/**
 * Validates adaptor settings.
 */
function globallink_adaptor_settings_validate($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Save Adaptor Settings')) {
    $value = $form_state['values']['globallink_pager_limit'];

    if (!is_numeric($value)) {
      form_set_error('globallink_pager_limit', t('Dahsboard Pager Limit is not a number.'));
    }
    elseif ($value < 1) {
      form_set_error('globallink_pager_limit', t('Dahsboard Pager Limit should be greater than 1.'));
    }

    $value = $form_state['values']['globallink_implementation_type'];

    if ($value != variable_get('globallink_implementation_type')) {
      if (globallink_pending_submission_exists()) {
        form_set_error('', t('Active submission exists. Please make sure there are no active submissions before changing this value.'));
      }
    }
  }
}

/**
 * Submits adaptor settings.
 */
function globallink_adaptor_settings_submit($form, &$form_state) {
  $old_value = variable_get('globallink_implementation_type');
  $old_preview = variable_get('globallink_enable_preview', 1);

  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Save Adaptor Settings')) {
    foreach ($form_state['values'] as $key => $value) {
      if (is_array($value) && isset($form_state['values']['array_filter'])) {
        $value = array_keys(array_filter($value));
      }

      variable_set($key, $value);
    }

    $value = $form_state['values']['globallink_implementation_type'];

    if ($value != $old_value) {
      db_truncate('globallink_core')->execute();
    }

    $pvalue = $form_state['values']['globallink_enable_preview'];

    if ($pvalue != $old_preview) {
      $_SESSION['globallink_globalLink_arr'] = array();

      unset($_SESSION['globallink_globalLink_arr_last_refreshed']);
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Gets project director details.
 *
 * @return object
 *   The project director object.
 */
function globallink_get_project_director_details() {
  $pd4 = new ProjectDirector();

  $url = strrev(variable_get('globallink_pd_url', ''));

  if (ord($url) == 47) {
    $url = substr($url, 1);
  }

  $r_url = strrev($url);

  $pd4->url = $r_url;
  $pd4->username = variable_get('globallink_pd_username', '');
  $pd4->password = variable_get('globallink_pd_password', '');
  $pd4->projectShortCode = variable_get('globallink_pd_projectid', '');
  $pd4->submissionPrefix = variable_get('globallink_pd_submissionprefix', '');
  $pd4->classifier = variable_get('globallink_pd_classifier', 'XML');
  $pd4->maxTargetCount = variable_get('globallink_pd_max_target', '99999');

  return $pd4;
}

/**
 * Saves project names.
 */
function globallink_save_project_names() {
  $pd4 = globallink_get_project_director_details();

  $projects_arr = globallink_get_user_pd_projects($pd4);
  $project_short_code = $pd4->projectShortCode;

  $var_arr = array();

  if ($project_short_code != '') {
    $proj_code_arr = explode(',', $project_short_code);

    foreach ($proj_code_arr as $proj_code) {
      if (isset($projects_arr[$proj_code])) {
        $var_arr[$proj_code] = $projects_arr[$proj_code];
      }
    }
  }

  variable_set('globallink_pd_projects', $var_arr);
}

/**
 * Validates project director details.
 *
 * @param object $pd4
 *   The project director object.
 *
 * @return bool
 *   TRUE if the project director details are valid.  FALSE otherwise.
 */
function globallink_validate_project_director_details($pd4) {
  if (empty($pd4->url)) {
    form_set_error('', t('GlobalLink URL is undefined.'));

    return FALSE;
  }
  elseif (empty($pd4->username)) {
    form_set_error('', t('GlobalLink User Id is undefined.'));

    return FALSE;
  }
  elseif (empty($pd4->password)) {
    form_set_error('', t('GlobalLink Password is undefined.'));

    return FALSE;
  }
  elseif (empty($pd4->projectShortCode)) {
    form_set_error('', t('GlobalLink Project Code is undefined.'));

    return FALSE;
  }
  elseif (empty($pd4->classifier)) {
    form_set_error('', t('GlobalLink Classifier is undefined.'));

    return FALSE;
  }
  elseif (empty($pd4->maxTargetCount)) {
    form_set_error('', t('GlobalLink Max Target Count is undefined.'));

    return FALSE;
  }
  elseif (!is_numeric($pd4->maxTargetCount)) {
    form_set_error('', t('GlobalLink Max Target Count is not a number.'));

    return FALSE;
  }

  return TRUE;
}

/**
 * Wraps globallink_convert_date_to_timestamp.
 *
 * @param string $from_date
 *   The desired log date.
 */
function globallink_view_log($from_date) {
  $from_date = globallink_convert_date_to_timestamp($from_date);
}

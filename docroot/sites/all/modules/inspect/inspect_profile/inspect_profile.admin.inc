<?php
/**
 * @file
 * Drupal Inspect Profile module administration layer
 */

//D8:use Drupal\Inspect\Inspect;

/**
 * Exits if AJAX request.
 *
 * @see InspectProfile_Statistics::make()
 *
 * @param integer $from_timestamp
 * @param integer $to_timestamp
 * @param array|NULL $options
 *   Default: NULL
 * @return InspectProfile_Statistics|NULL
 */
function inspect_profile_statistics_make($from_timestamp = 1, $to_timestamp = 0, $options = NULL) {
  $report = NULL;
  // Requires elevated user privilege to fend against flooding; report generation is heavy performance-wise.
  if (user_access('administer site configuration')) {
    module_load_include('inc', 'inspect_profile', 'InspectProfile_Statistics');
    $report = InspectProfile_Statistics::make($from_timestamp, $to_timestamp, $options);
  }
  if (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
    exit;
  }
  return $report;
}

/**
 * Lists .
 *
 * @return array
 */
function _inspect_profile_statistics_list() {
  $header = array(
    array('data' => t('Id'), 'field' => 'stats.sid', 'sort' => 'desc'),
    array('data' => t('From'), 'field' => 'stats.from_timestamp', 'sort' => 'desc'),
    array('data' => t('To'), 'field' => 'stats.to_timestamp', 'sort' => 'desc'),
    t('Length'), // Time span.
    array('data' => t('Filter'), 'field' => 'stats.profile_filter_name', 'sort' => 'asc'),
    array('data' => t('Completed'), 'field' => 'stats.completed'),
    array('data' => t('Aborted'), 'field' => 'stats.aborted'),
    array('data' => t('Failed'), 'field' => 'stats.failed'),
  );
  $rows = array();

  try {
    $query = db_select('inspect_profile_statistics', 'stats')->extend('PagerDefault')->extend('TableSort');
    $query->fields('stats', array('sid', 'from_timestamp', 'to_timestamp', 'profile_filter_name', 'completed', 'aborted', 'failed'))
      ->limit(50);

    // Table header sorting doesn't seem to build combined order.
    if (empty($_GET['sort'])) {
      $query->orderBy('stats.to_timestamp', 'desc')
        ->orderBy('stats.from_timestamp', 'desc')
        ->orderBy('stats.profile_filter_name', 'asc');
    }
    else {
      $query->orderByHeader($header);
    }
    $result = $query->execute();

    foreach ($result as $stat) {
      // Skip empty reports.
      if (!$stat->completed && !$stat->aborted && !$stat->failed) {
        continue;
      }
      $url = 'admin/reports/inspect_profile/statistics/download/' . $stat->sid;
      $title = array('attributes' => array('title' => t('Download report #!id', array('!id' => $stat->sid))));
      $rows[] = array('data' =>
        array(
          l($stat->sid, $url, $title),
          l(format_date($stat->from_timestamp, 'custom', 'Y-m-d H:i'), $url, $title),
          l(format_date($stat->to_timestamp - 1, 'custom', 'Y-m-d H:i'), $url, $title),
          l(
            !(($diff = $stat->to_timestamp - $stat->from_timestamp) % 86400) ? (($days = $diff / 86400) . ' ' . ($days > 1 ? t('days') : t('day'))) :
              _inspect_profile_statistics_date_diff($diff)
            ,
            $url,
            $title
          ),
          l($stat->profile_filter_name, $url, $title),
          l($stat->completed, $url, $title),
          l($stat->aborted, $url, $title),
          l($stat->failed, $url, $title),
        ),
      );
    }
  }
  catch (Exception $xc) {
    Inspect::trace($xc, array('type' => 'inspect_profile', 'severity' => WATCHDOG_ERROR));
  }

  return array(
    //'whatever' => array(
    //  '#markup' => '<div>hello</div>'
    //),
    'inspect_profile_statistics_list_table' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#attributes' => array('id' => 'admin-inspect-profile-statistics-list'),
      '#empty' => t('No reports generated.'),
    ),
    'inspect_profile_statistics_list_pager' => array('#theme' => 'pager'),
  );
}

/**
 * @param integer $diff
 * @return string
 */
function _inspect_profile_statistics_date_diff($diff) {
  $s = '';
  if (($days = round(floor($diff / 86400)))) {
    $s .= $days . ' ' . ($days > 1 ? t('days') : t('day'));
    $diff -= $days * 86400;
  }
  if (($hrs = round(floor($diff / 3600)))) {
    $s .= ' ' . $hrs . ' ' . ($hrs > 1 ? t('hours') : t('hour'));
  }
  return trim($s);
}

/**
 * Push report file to download.
 *
 * @param integer $report_id
 */
function inspect_profile_statistics_report_download($report_id) {
  module_load_include('inc', 'inspect_profile', 'InspectProfile_Statistics');
  InspectProfile_Statistics::reportDownload($report_id);
}

/**
 * Defines configuration form fields.
 *
 * Uses drupal_get_form().
 *
 * @param array $form
 * @param array &$form_state
 * @return array
 *   - the return value of system_settings_form()
 */
function _inspect_profile_admin_form($form, &$form_state) {
  if (!isset($form['#attributes'])) {
    $form['#attributes'] = array();
  }
  $form['#attributes']['autocomplete'] = 'off';

  $form['#attached']['css'][] = drupal_get_path('module', 'inspect_profile') . '/inspect_profile.admin.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'inspect_profile') . '/inspect_profile.admin.js';

  $form['stat_simple'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simple event statistics'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'inspect_profile_statsimplefileshutdown' => array(
      '#type' => 'checkbox',
      '#title' => t('File simple event statistics at shutdown.'),
      '#default_value' => variable_get('inspect_profile_statsimplefileshutdown', FALSE),
    ),
  );

  $profile_filter_list = $cron_profile_filters = $cron_profile_filter_options = array();
  if (($profile_filters = variable_get('inspect_profile_profilefilters', ''))
    && ($profile_filter_list = drupal_json_decode($profile_filters))) {
    
    // Cron profile filters.
    $cron_profile_filter_options = array_combine(array_keys($profile_filter_list), array_keys($profile_filter_list));
    if (($cron_profile_filters = variable_get('inspect_profile_statscronprofilefilters', array()))) {
      if (($values = drupal_json_decode($cron_profile_filters))) {
        $cron_profile_filters = $cron_profile_filter_options;
        foreach ($cron_profile_filters as $name) {
          if (!in_array($name, $values, TRUE)) {
            $cron_profile_filters[$name] = 0;
          }
        }
      }
      else {
        $cron_profile_filters = array();
      }
    }
  }

  $form['stat_profile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Profile statistics'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'inspect_profile_statseveritythreshold' => array(
      '#type' => 'select',
      '#title' => t('Severity threshold'),
      '#description' => t('A profile is considered failed when it contains an event of that watchdog severity (or worse).'),
      '#options' => array(
        WATCHDOG_ALERT => t('alert'), // 1
        WATCHDOG_CRITICAL => t('critical'), // 2
        WATCHDOG_ERROR => t('error'), // 3
        WATCHDOG_WARNING => t('warning'), // 4
        WATCHDOG_NOTICE => t('notice'), // 5
        WATCHDOG_INFO => t('info'), // 6
        WATCHDOG_DEBUG => t('debug'), // 7
      ),
      '#default_value' => variable_get('inspect_profile_statseveritythreshold', WATCHDOG_ERROR),
    ),
    'inspect_profile_statoffsethrs' => array(
      '#type' => 'textfield',
      '#title' => t('Day statistics offset (hours)'),
      '#description' => t('Default zero - generate day statistics from 00:00:00 to 23:59:59 o\'clock.'),
      '#default_value' => variable_get('inspect_profile_statoffsethrs', 0),
      '#size' => 2,
    ),
    'inspect_profile_statscrondays' => array(
      '#type' => 'select',
      '#title' => t('Cron generates statistical reports of the previous N days'),
      '#description' => t('Will only generate once every day; if cron runs more than once a day it only checks that the reports exist.'),
      '#options' => range(0, 15),
      '#default_value' => variable_get('inspect_profile_statscrondays', 0),
      '#prefix' => '<hr/><h4>' . t('Report generation') . '</h4>',
    ),
    'inspect_profile_statscronprofilefilters' => array(
      '#type' => 'checkboxes',
      '#title' => t('Cron generates for each of these profile filters'),
      '#options' => $cron_profile_filter_options,
      '#default_value' => $cron_profile_filters,
    ),
    'inspect_profile_statserrortranslator' => array(
      '#type' => 'textfield',
      '#title' => t('Error code translatation function (if such exists)'),
      '#description' => t('The function must, given only an error code as argument, return an array which has the buckets \'module\' and \'name\'.'),
      '#default_value' => variable_get('inspect_profile_statserrortranslator', ''),
    ),
    'inspect_profile_statsnow' => array(
      '#type' => 'markup',
      '#markup' => '<div id="inspect-profile-statsnow">
<div class="form-item form-type-textfield form-item-inspect-profile-statsfrom">
<label for="edit-inspect-profile-statsfrom">' . t('From time') . '</label>
<input type="text" id="edit-inspect-profile-statsfrom" name="inspect_profile_statsfrom" value="" size="19" maxlength="19" class="form-text" placeholder="' . t('YYYY-MM-DD HH:II:SS') . '" />
<div class="">' . t('Empty: today\'s morning at 0 o\'clock.') . '</div>
</div>
<div class="form-item form-type-textfield form-item-inspect-profile-statsto">
<label for="edit-inspect-profile-statsto">' . t('To time') . '</label>
<input type="text" id="edit-inspect-profile-statsto" name="inspect_profile_statsto" value="" size="19" maxlength="19" class="form-text" placeholder="' . t('YYYY-MM-DD HH:II:SS') . '" />
<div class="">' . t('Empty: 24 hours after From time.') . '</div>
</div>
<div class="form-actions form-wrapper"><input type="button" id="inspect_profile_statsnow" name="inspect_profile_statsnow" value="' . t('Generate') . '" class="form-submit"></div>
</div>',
      '#prefix' => '<hr/><h4>' . t('Generate reports now') . '</h4>',
    ),
    'inspect_profile_statsfilesnotwin' => array(
      '#type' => 'checkbox',
      '#title' => t('Don\'t make files Windows compatible (omit UTF-8 byte order marker, and carriage return by newlines).'),
      '#default_value' => variable_get('inspect_profile_statsfilesnotwin', FALSE),
      '#prefix' => '<hr/><h4>' . t('Report files') . '</h4>',
    ),
  );
  $form['stat_profile']['inspect_profile_statscrondays']['#options'][0] = t('Turned off');

  $form['inspect_profile_profilefilters'] = array(
    '#type' => 'textarea',
    '#default_value' => $profile_filters,
    '#maxlength' => 10000,
    '#attributes' => array('style' => array('display:none;')),
  );
  $form['profile_filters'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Profile filters'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'add' => array(
      '#type' => 'fieldset',
      '#title' => t('Add filter'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Filter name'),
        '#default_value' => '',
        '#maxlength' => 64,
        '#size' => 60,
      ),
      'profiles' => array(
        '#type' => 'textarea',
        '#title' => t('Profile names'),
        '#description' => t('Newline- or pipe-separated. To exclude: prefix the whole filter with an exclamation mark.'),
        '#default_value' => '',
        '#rows' => 1,
      ),
    ),
  );
  if ($profile_filter_list) {
    $form['profile_filters']['list'] = array(
      '#type' => 'fieldset',
      '#title' => t('Existing filters'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    foreach ($profile_filter_list as $name => $filter) {
      $form['profile_filters']['list'][$name] = array(
        '#type' => 'textarea',
        '#title' => $name,
        '#default_value' => $filter,
        '#rows' => 1,
      );
      if ($filter{0} === '!') {
        $form['profile_filters']['list'][$name]['#description'] = '- ' . t('excluding') . ' -';
      }
    }
  }

  //module_load_include('inc', 'inspect_profile', 'InspectProfile_Statistics');
  //InspectProfile_Statistics::make(1380232800, 1380578400, 'Personnel forms using the Employment selector');

  //variable_set('inspect_profile_profilefilters', '{"login":"login, get user employment","Economy forms":"create acc-orders|create acc-questions","Personnel forms using the Employment selector":"!login, get user employment|create acc-orders|create acc-questions|create benefits-wo-emp|create mass-report|create recovery-wo-emp|create quest-wo-emp|create tax-rate-wo-emp","Personnel forms that dont use the Employment selector":"create benefits-wo-emp|create mass-report|create recovery-wo-emp|create quest-wo-emp|create tax-rate-wo-emp"}');
  //variable_set('inspect_profile_statserrortranslator', 'kk_seb_error_get');

  //InspectProfile_Statistics::make(1380492000, 0);

  //$filter = '!login, get user employment|create quest-wo-emp|create recovery-wo-emp|create benefits-wo-emp|create tax-rate-wo-emp';
  //$filter = '!login, get user employment|create acc-orders|create acc-questions|create benefits-wo-emp|create mass-report|create recovery-wo-emp|create quest-wo-emp|create tax-rate-wo-emp';
  //$filter = 'login, get user employment';
  //$filter = 'create acc-orders|create acc-questions';
  //$filter = 'create benefits-wo-emp|create mass-report|create recovery-wo-emp|create quest-wo-emp|create tax-rate-wo-emp';

  //echo inspect_get(InspectProfile_Statistics::make(1380492000, 0, $filter));
  //echo inspect_get(InspectProfile_Statistics::make(1380492000, 0, '!login, get user employment'));
  //echo inspect_get(InspectProfile_Statistics::make(1380492000, 0, '!login, get user employment|create quest-wo-emp|create recovery-wo-emp|create benefits-wo-emp|create tax-rate-wo-emp'));

  /*echo inspect_get(InspectProfile_Statistics::make(1380492000, 1380578400, $filter));
  //sleep(5);
  echo inspect_get(InspectProfile_Statistics::make(1380578400, 1380664800, $filter));
  //sleep(5);*/
  //echo inspect_get(InspectProfile_Statistics::make(1380492000, 1380664800, 'Personnel forms using the Employment selector'));
  //echo inspect_get(InspectProfile_Statistics::make(1380232800, 1380578400, 'Personnel forms using the Employment selector'));
  //echo inspect_get(InspectProfile_Statistics::make(1380492000, 1380664800, 'Absence'));

  $form['#submit'][] = '_inspect_profile_admin_form_submit';
  return system_settings_form($form);
}

/**
 * @param array $form
 * @param array &$form_state
 */
function _inspect_profile_admin_form_submit($form, &$form_state) {
  //inspect($form_state['values']);
  $values =& $form_state['values'];

  $save = array();
  if (($existing = $values['inspect_profile_profilefilters'])) {
    $save = drupal_json_decode($existing);
  }
  // New?
  $profile_filters =& $values['profile_filters'];
  if (($name = trim($profile_filters['add']['name'])) && ($profiles = trim($profile_filters['add']['profiles']))) {
    $save[ $name ] = preg_replace('/^\|*([^\|].*[^\|])\|*$/', '$1',
      preg_replace('/\|+/', '|',
        preg_replace('/\s*\|\s*/', '|',
          str_replace(array("\r", "\n"), array('', '|'),
            $profiles))));
  }
  // Any existing cleared?
  if (array_key_exists('list', $profile_filters)) {
    foreach ($profile_filters['list'] as $name => $profiles) {
      if (!trim($profiles)) {
        unset($save[$name]);
      }
      else { // Update.
        $save[$name] = preg_replace('/^\|*([^\|].*[^\|])\|*$/', '$1',
          preg_replace('/\|+/', '|',
            preg_replace('/\s*\|\s*/', '|',
              str_replace(array("\r", "\n"), array('', '|'),
                $profiles))));
      }
    }
  }
  $values['inspect_profile_profilefilters'] = json_encode($save);

  // Turn sudding checkboxes array into json.
  if (($cron_profile_filters_checkboxes = $values['inspect_profile_statscronprofilefilters'])) {
    $cron_profile_filters = array();
    foreach ($cron_profile_filters_checkboxes as $name => $val) {
      if ($val && array_key_exists($name, $save)) {
        $cron_profile_filters[] = $name;
      }
    }
    $values['inspect_profile_statscronprofilefilters'] = $cron_profile_filters ? drupal_json_encode($cron_profile_filters) : NULL;
  }

  // Do not save the list.
  unset($values['profile_filters']);
}

<?php

/**
 * @file
 * Admin functions.
 */

/**
 * Language check in one step immediately.
 */
function langcheck_check_instant_start() {
  $sandbox = array();
  langcheck_check_initialize($sandbox);
  if ($sandbox['results']['count_languages']) {
    $settings = variable_get('langcheck_settings', _langcheck_default_values('langcheck_settings'));
    $settings['check_batch']['num_of_files'] = 0;
    $settings['check_batch']['num_of_langs'] = 0;
    langcheck_check_doit($sandbox, $settings);
    langcheck_check_result_summary($sandbox);
    variable_set('langcheck_check_batch_result', $sandbox['results']);
  }
  drupal_goto('admin/reports/langcheck');
}

/**
 * Batch process starter.
 */
function langcheck_check_batch_start() {
  $batch = array(
    'operations' => array(
      array('langcheck_check_batch_process', array()),
    ),
    'finished' => 'langcheck_check_batch_finished',
    'title' => t('Translation status check process'),
    'init_message' => t('Translation status check is starting.'),
    //'progress_message' => t('Remaining @remaining of @total.'),
    'progress_message' => '',
    'error_message' => t("Projects' translations checks has encountered an error."),
    'file' => drupal_get_path('module', 'langcheck') . '/langcheck.inc'
  );
  batch_set($batch);
  batch_process('admin/reports/langcheck');
}

/**
 * Batch process callback.
 */
function langcheck_check_batch_process(&$context) {
  if (!array_key_exists('status', $context['sandbox'])) {
    $context['sandbox']['status'] = 'initialize';
  }
  switch ($context['sandbox']['status']) {
    case 'initialize':
      langcheck_check_initialize($context['sandbox']);
      if ($context['sandbox']['results']['count_languages'] == 0) {
        $context['finished'] = 1;
        return;
      }
      $context['message'] = t('Initializing');
      $context['sandbox']['status'] = 'check';
      break;
    case 'check':
      $settings = variable_get('langcheck_settings', _langcheck_default_values('langcheck_settings'));
      langcheck_check_doit($context['sandbox'], $settings);
      if ($context['sandbox']['results']['count_modules'] <= $context['sandbox']['current_module']) {
        $context['message'] = t('Preparing the result to display.');
      }
      else {
        $context['message'] = t('Current module is %module, remaining @remaining of @total.', array('%module' => $context['sandbox']['modules'][$context['sandbox']['current_module']], '@remaining' => $context['sandbox']['results']['count_modules'] - $context['sandbox']['current_module'], '@total' => $context['sandbox']['results']['count_modules']));
      }
      break;
  }
  if ($context['sandbox']['current_module'] == $context['sandbox']['results']['count_modules']) {
    langcheck_check_result_summary($context['sandbox']);
    $context['results'] = $context['sandbox']['results'];
    $context['finished'] = 1;
  }
  else {
    $context['finished'] = $context['sandbox']['current_module'] / $context['sandbox']['results']['count_modules'];
  }
}

/**
 * Batch process finished callback.
 */
function langcheck_check_batch_finished($success, $results, $operations) {
  // @TODO Error handling. $success.
  variable_set('langcheck_check_batch_result', $results);
}

/**
 * Menu callback; displays results of the batch.
 *
 * @return string
 *    Themed HTML output of the stored langcheck check result.
 */
function langcheck_check_batch_result_page() {
  $results = variable_get('langcheck_check_batch_result', NULL);
  $output = '';
  if ($results === NULL) {
    $output .= t('Stored result not fund.') . ' ';
    if (user_access('start a new language check')) {
      $output = langcheck_check_batch_result_page_msg_create_new();
    }
    else {
      $output .= t('Please contact with the site administrators.');
    }
    return $output;
  }
  $languages = language_list();
  if (count($languages) == 1 AND array_key_exists('en', $languages)) {
    return t('Only the English language is enabled. Check was not necessary.');
  }
  $output .= t('The result was created %created ago.', array('%created' => format_interval(REQUEST_TIME - $results['time_start'])));
  if (user_access('start a new language check')) {
    $output .= ' ' . langcheck_check_batch_result_page_msg_create_new();
  }
  return $output . theme('langcheck_check_batch_result', $results);
}

/**
 * Returns the localized message to be displayed above the results.
 */
function langcheck_check_batch_result_page_msg_create_new() {
  return t('To create a new one with batch process <a href="!href-batch">click here</a> (recommended), or to create a new one in one step <a href="!href-instant">click here</a>.', array('!href-batch' => url('admin/reports/langcheck/start-batch'), '!href-instant' => url('admin/reports/langcheck/start-instant')));
}

/**
 * Cleans up the list by removing exclused directories.
 *
 * @param array $files
 *    Return array of _potx_explore_dir() function.
 * @param array $exclusions
 *    Array containing directory paths; most likely the union of the list that
 *    was given on the admin UI and langcheck_default_exclusions()'s output.
 */
function langcheck_remove_exclusions(&$files, $exclusions) {
  if (!count($exclusions)) {
    return;
  }
  $exclusions_count = count($exclusions);
  foreach ($files as $key => $filepath) {
    $i = 0;
    while ($i < $exclusions_count AND !preg_match('!^' . preg_quote($exclusions[$i], '!') . '(/|$)!', $filepath)) {
      $i++;
    }
    if ($i < $exclusions_count) {
      unset($files[$key]);
    }
  }
}

/**
 * Default exclusions.
 */
function langcheck_default_exclusions() {
  $exclusions = array(
    // @TODO This array needs to be filled.
    'print' => array('tcpdf', 'dompdf'),
    'geshifilter' => array('geshi', 'geshi-extra'),
    'fckeditor' => array('fckeditor'),
  );
  $return = array();
  foreach ($exclusions as $module => $paths) {
    if (module_exists($module)) {
      $root = drupal_get_path('module', $module);
      foreach ($paths as $p) {
        $return[] = "{$root}/{$p}";
      }
    }
  }
  return $return;
}

/**
 * Set up defaults for the language checking (batch) process.
 *
 * @param array $sandbox
 *    The $context['sandbox'] when running as a batch process.
 */
function langcheck_check_initialize(&$sandbox) {
  $languages = locale_language_list();
  unset($languages['en']);
  $sandbox['languages'] = array_keys($languages);
  $sandbox['modules'] = array_keys(module_list(FALSE, TRUE, TRUE));
  sort($sandbox['modules']);

  $default = array(
    'total' => 0,
    'untranslated' => 0,
    'state' => 0,
  );
  $sandbox['results'] = array(
    'time_start' => REQUEST_TIME,
    'time_end' => NULL,
    'count_languages' => count($sandbox['languages']),
    'count_modules' => count($sandbox['modules']),
    'data_module' => array(),
    'sum_module' => array_fill_keys($sandbox['languages'], $default),
    'sum_language' => array_fill_keys($sandbox['modules'], $default),
    'languages' => $sandbox['languages'],
    'language_names' => $languages,
    'excluded_modules' => array(),
  );
  foreach ($sandbox['modules'] as $module) {
    $sandbox['results']['data_module'][$module] = array_fill_keys($sandbox['languages'], $default);
  }
  $sandbox['current_language'] = 0;
  $sandbox['current_module'] = 0;
  $sandbox['current_module_files'] = array();
}

/**
 * Handles the actual language checking.
 *
 * @param array $sandbox
 *    The $context['sandbox'] when running as a batch process.
 *    @see langcheck_check_initialize()
 * @param array $settings
 *    Optional; use the values entered on the admin UI if not given.
 */
function langcheck_check_doit(&$sandbox, $settings = NULL) {
  module_load_include('inc', 'potx');
  global $_potx_store;
  potx_status('set', POTX_STATUS_SILENT);
  if (!$settings) {
    $settings = variable_get('langcheck_settings', _langcheck_default_values('langcheck_settings'));
  }
  $exclusions = array_merge(langcheck_default_exclusions(), $settings['check_batch']['exclusions']);
  $processed_languages = 0;
  $processed_files = 0;
  while (
    (!$settings['check_batch']['num_of_files'] || ($processed_files < $settings['check_batch']['num_of_files']))
    &&
    (!$settings['check_batch']['num_of_langs'] || ($processed_languages < min($settings['check_batch']['num_of_langs'], $sandbox['results']['count_languages'])))
    &&
    $sandbox['current_module'] < $sandbox['results']['count_modules']
  ) {
    $module = $sandbox['modules'][$sandbox['current_module']];
    $path = drupal_get_path('module', $module);
    $strip_prefix = 1 + drupal_strlen($path);
    if (!count($sandbox['current_module_files'])) {
      $sandbox['current_module_files'] = _potx_explore_dir($path . '/');
      langcheck_remove_exclusions($sandbox['current_module_files'], $exclusions);
      if (!count($sandbox['current_module_files'])) {
        $sandbox['results']['excluded_modules'][] = $module;
      }
    }
    $files = ($settings['check_batch']['num_of_files']) ? array_slice($sandbox['current_module_files'], 0, $settings['check_batch']['num_of_files'] - $processed_files) : $sandbox['current_module_files'];
    $sandbox['current_language'] = 0;
    while (
      (!$settings['check_batch']['num_of_langs'] || ($processed_languages < min($settings['check_batch']['num_of_langs'], $sandbox['results']['count_languages'])))
      &&
      $sandbox['current_language'] < $sandbox['results']['count_languages']
    ) {
      foreach ($files as $file) {
        _potx_process_file($file, $strip_prefix);
      }
      $langcode = $sandbox['languages'][$sandbox['current_language']];
      _potx_build_files(POTX_STRING_RUNTIME, POTX_BUILD_SINGLE, 'general', '_potx_save_string', '_potx_save_version', '_potx_get_header', $langcode, $langcode);
      $lines = explode("\n", $_potx_store['general']['strings']);
      $total = $untranslated = 0;
      foreach ($lines as $lineno => $line) {
        if (drupal_substr($line, 0, 7) == 'msgid "') {
          $total++;
        }
        elseif ((drupal_substr($line, 0, 9) == 'msgstr ""') || (drupal_substr($line, 0, 12) == 'msgstr[1] ""')) {
          if (isset($lines[$lineno + 1]) && ($lines[$lineno + 1] == '')) {
            $untranslated++;
          }
        }
      }
      $sandbox['results']['data_module'][$module][$langcode]['total'] += $total;
      $sandbox['results']['data_module'][$module][$langcode]['untranslated'] += $untranslated;
      $sandbox['results']['sum_language'][$module]['total'] += $total;
      $sandbox['results']['sum_language'][$module]['untranslated'] += $untranslated;
      $sandbox['results']['sum_module'][$langcode]['total'] += $total;
      $sandbox['results']['sum_module'][$langcode]['untranslated'] += $untranslated;
      unset($GLOBALS['_potx_store']['general'], $GLOBALS['_potx_strings']);
      $processed_languages++;
      $sandbox['current_language']++;
    }
    $processed_files += count($files);
    if ($sandbox['current_language'] == $sandbox['results']['count_languages']) {
      $sandbox['current_module_files'] = array_slice($sandbox['current_module_files'], count($files));
    }
    if (!count($sandbox['current_module_files']) AND $sandbox['current_language'] == $sandbox['results']['count_languages']) {
      $sandbox['current_module']++;
    }
  }
}

/**
 * Calculates the summaries.
 */
function langcheck_check_result_summary(&$sandbox) {
  foreach ($sandbox['results']['sum_language'] as $module => $detail) {
    if ($detail['total']) {
      $sandbox['results']['sum_language'][$module]['state'] = ($detail['total'] - $detail['untranslated']) / $detail['total'];
    }
  }
  foreach ($sandbox['results']['sum_module'] as $langcode => $detail) {
    if ($detail['total']) {
      $sandbox['results']['sum_module'][$langcode]['state'] = ($detail['total'] - $detail['untranslated']) / $detail['total'];
    }
  }
  foreach ($sandbox['results']['data_module'] as $module => $langs) {
    foreach ($langs as $langcode => $detail) {
      if ($detail['total']) {
        $sandbox['results']['data_module'][$module][$langcode]['state'] = ($detail['total'] - $detail['untranslated']) / $detail['total'];
      }
    }
  }
  $sandbox['results']['time_end'] = REQUEST_TIME;
}

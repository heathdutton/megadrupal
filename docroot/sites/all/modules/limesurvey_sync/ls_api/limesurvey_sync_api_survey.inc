<?php
/**
 * @file
 * API functions for the ls_survey module
 */

/**
 * Extract surveys with their properties from the LimeSurvey database.
 *
 * This function extracts surveys with their properties from the LimeSurvey
 * and returns them as an php array variable. It extracts all survey or only
 * a specific survey, depending on the parameter $sid.
 *
 * @param $ls_sid
 *   An integer, corresponding to the survey reference ID or 'all' to return
 *   to all surveys stored on the LimeSurvey database keyed by the survey ID.
 * @param $reset
 *   A boolean, set it to TRUE to reload cached datas. Defalt is FALSE.
 *
 * @return
 *   An multidimensionnal array, corresponding to surveys with their
 *   properties from the LimeSurvey database.
 */
function limesurvey_sync_survey_properties($ls_sid = NULL, $reset = FALSE) {
  // Cache datas.
  static $all_surveys = array();
  if ($reset || empty($all_surveys)) {
    $all_surveys = array();

    // Load file to connect to the LimeSurvey tables.
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
    // Load file for the drupal formated url.
    module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');

    // Set the previous db.
    $previous_db = limesurvey_sync_active_db_name();
    limesurvey_sync_db_close();

    // Load all existing drupal surveys.
    $surveys_query = db_query("SELECT n.nid AS node_nid, ls_sid, ls_lang, nt.type AS answ_type
    FROM {node} n
    JOIN {ls_survey} lss ON lss.nid = n.nid AND lss.vid = n.vid
    JOIN {node_type} nt ON lss.ls_orig_type = nt.orig_type");
    // Don't restrict to module = ls_answ : the assocation will still works if
    // the ls_answ module is disabled.
    while ($surveys_row = $surveys_query->fetchAssoc()) {
      $nodes_survey[$surveys_row['ls_sid']][$surveys_row['ls_lang']]['nid_survey'] = $surveys_row['node_nid'];
      $nodes_survey[$surveys_row['ls_sid']][$surveys_row['ls_lang']]['answ_type'] = $surveys_row['answ_type'];
    }

    // Retrieve the database version.
    $ls_gs = limesurvey_sync_ls_global_settings();
    $db_version = $ls_gs['dbversion'];
    if (empty($db_version)) {
      // return FALSE if the synchronization is not set.
      return FALSE;
    }

    limesurvey_sync_db_open();
    // Load all surveys properties from the LimeSurvey database.
    $desquery = db_query('SELECT surveys.*, surveys_lang.* FROM {' . limesurvey_sync_table('surveys') . '} surveys '
    . 'RIGHT JOIN {' . limesurvey_sync_table('surveys_languagesettings') . '} surveys_lang ON surveys_lang.surveyls_survey_id = surveys.sid '
    . 'ORDER BY active DESC, sid ASC, surveyls_language ASC, surveyls_title ASC');
    // Iterate each survey.
    foreach ($desquery as $surveys) {
      $sid = check_plain($surveys->sid);
      $lang = check_plain($surveys->surveyls_language);

      // Save datas into a php array variable.
      // Populate the non language specific properties.
      if (!isset($all_surveys[$sid])) {
        // Make sure the ls connexion is active for db_table_exists().
        limesurvey_sync_db_open();
        $all_surveys[$sid] = array(
          'active' => (check_plain($surveys->active) == 'Y'),
          // The 'tokens_table' key is set below.
          'tokenanswerspersistence' => (check_plain($surveys->tokenanswerspersistence) == 'Y'),
          // For old versions of LS (dbversion < 145) : set to TRUE if it not
          // exists, to allow to edit answers.
          'alloweditaftercompletion' => (isset($surveys->alloweditaftercompletion)) ? (check_plain($surveys->alloweditaftercompletion) == 'Y') : TRUE,
          // The showwelcome has been added on dbversion = 145 to.
          'showwelcome' => (isset($surveys->showwelcome)) ? (check_plain($surveys->showwelcome) == 'Y') : TRUE,
          // The 'tokenlength' colomn has been added since LimeSurvey DBversion >= 141.
          'tokenlength' => (isset($surveys->tokenlength)) ? check_plain($surveys->tokenlength) : 15,
          'autoredirect' => (check_plain($surveys->autoredirect) == 'Y'),
          'default_language' => check_plain($surveys->language),
          'additional_languages' => check_plain(trim($surveys->additional_languages)),
          'anonymized' => (check_plain($surveys->anonymized) == 'Y'),
          'date_answer' => (check_plain($surveys->datestamp) == 'Y'),
          'ipaddr' => (check_plain($surveys->ipaddr) == 'Y'),
          'refurl' => (check_plain($surveys->refurl) == 'Y'),
          // 'publicstatistics' colomn has been added since LimeSurvey
          // DBversion >= 131.
          'publicstatistics' => (isset($surveys->publicstatistics)) ? (check_plain($surveys->publicstatistics) == 'Y') : FALSE,
          // Note : bd version is the same for all surveys.
          'db_version' => check_plain($db_version),
          // 'startdate' colomn has been added since LimeSurvey DBversion >= 129.
          // 'startdate' and 'expires' are NULL by default. Change them to string.
          // Convert both colomns has timestamp.
          'startdate' => (empty($surveys->startdate)) ? '' : limesurvey_sync_fixdate_ls_to_drupal(check_plain($surveys->startdate)),
          'expires' => (empty($surveys->expires)) ? '' : limesurvey_sync_fixdate_ls_to_drupal(check_plain($surveys->expires)),
        );

        // Check for disabled survey.
        $time = REQUEST_TIME;
        $survey_has_started = (empty($all_surveys[$sid]['startdate']) || $time >= $all_surveys[$sid]['startdate']);
        $survey_not_finished = (empty($all_surveys[$sid]['expires']) || $time < $all_surveys[$sid]['expires']);
        $survey_is_actived = $all_surveys[$sid]['active'];
        $is_available = ($survey_has_started && $survey_not_finished && $survey_is_actived);
        $all_surveys[$sid]['is_available'] = $is_available;

        // Check if http_referer method is available.
        $http_referer = FALSE;
        if ($all_surveys[$sid]['autoredirect'] && !$all_surveys[$sid]['showwelcome']) {
          // Check if it is a one page survey.
          switch ($surveys->format) {
            case 'A': // All in one.
              $http_referer = TRUE;
              break;
            case 'G': // Group by group.
              // Check if there is only one group.
              limesurvey_sync_db_open();
              $query_group = db_query('SELECT DISTINCT gid FROM {' . limesurvey_sync_table('groups') . '} WHERE sid = :sid', array(':sid' => $sid));
              $http_referer = ($query_group->rowCount() <= 1);
              break;
            case 'S': // Question by question
              // Check if there is only one question.
              limesurvey_sync_db_open();
              $query_question = db_query('SELECT DISTINCT qid FROM {' . limesurvey_sync_table('questions') . '} WHERE sid = :sid', array(':sid' => $sid));
              $http_referer = ($query_question->rowCount() <= 1);
              break;
          }
          // Fix for a bug from the LimeSurvey software:
          // The welcome screen may be shown even if it should not on various
          // conditions. We disable the http_referrer method for such cases.
          // More informations on http://drupal.org/node/1717844#comment-6490200
          // It is hard to precise from wich version this bug had appeared :
          // - the v1.91+ build 120302 (dbVersion 146) was free of this bug.
          // - the v1.92+ build 120725 (dbVersion ?) mets this bug
          //   (http://bugs.limesurvey.org/view.php?id=6418).
          // - the v1.92+ build 120801 (dbVersion 155.6) mets this bug
          //   (http://bugs.limesurvey.org/view.php?id=6497)
          // - the v2.00+ build 120930 (dbVersion ?) incompletly fix it :
          //   mets this bug for non 'all-in-one' survey formats
          // - the v2.00+ build 121019 (dbVersion 163) mets this bug for non
          //   'all-in-one' survey formats
          //   (http://bugs.limesurvey.org/view.php?id=6782).
          // - the v2.00+ build 121031 (dbVersion 163) definitively fix it
          //   (http://bugs.limesurvey.org/view.php?id=6782#c21800).
          // - the v2.00+ build 121115 set the dbVersion to 164.
          // If the http_referrer method is not available, others less
          // specific methods will be used.
          $dbversion_bug_started = 146; // Arbitraty set to this dbversion (may be less).
          $dbversion_bug_fixeded = 164; // Does not exist yet...
          if ($db_version >= 163 && $surveys->format == 'A') {
            // No need to fix this issue.
          }
          elseif ($db_version > $dbversion_bug_started && $db_version < $dbversion_bug_fixeded) {
            $http_referer = FALSE;
          }
        }
        $all_surveys[$sid]['http_referer'] = $http_referer;
      }

      // Populate the language specific properties.

      // Handle back url from LS, comparing to the drupal url() version.
      // A drupal db connection may be usefull for url or variable if cache is
      // empty.
      limesurvey_sync_db_close();
      $ls_settings = variable_get('ls_settings', array('ls_path' => ''));
      $url_lang = check_plain($surveys->surveyls_url);
      $back_url_survey = FALSE;
      if (!empty($url_lang)) {
        // Convert relative to absolute url if needed and format the url into the drupal way.
        $url_lang = limesurvey_sync_rel2abs($url_lang, limesurvey_sync_rel2abs($ls_settings['ls_path'] . '/'));
        $back_url_survey = limesurvey_sync_drupal_formated_url($url_lang);
      }
      $correct_back_url = $type = FALSE;
      if (!empty($nodes_survey[$sid][$lang]['answ_type'])) {
        $type = $nodes_survey[$sid][$lang]['answ_type'];
        $path = 'backfromsurvey/' . $type;
        $options = variable_get('ls_answ_options_' . $type, FALSE);
        $iframes = array('iframe', 'colorbox', 'lightbox2', 'modalframe');
        if ($options && in_array($options['ls_redirect'], $iframes)) {
          $path .= '/back';
        }
        $correct_back_url = url($path, array('absolute' => TRUE));
      }
      // Switch back to the 'ls' connection.
      limesurvey_sync_db_open();

      // Populate the language specific properties.
      $all_surveys[$sid]['#lang'][$lang] = array(
        'title' => check_plain($surveys->surveyls_title),
        'description' => filter_xss($surveys->surveyls_description),
        'back_url' => check_plain($surveys->surveyls_url),
        'back_to_drupal' => ($back_url_survey && $back_url_survey == $correct_back_url),
        'correct_back_url' => $correct_back_url,
        // Note that, in most cases, nid_survey and type should not be retreive
        // from this function because it does not check if the ls_answ_module
        // is enabled and uses the current available value, not the node one
        // (which may be not the same on desynchronized answers). It is used
        // only for not breaking informations in case of the ls_answ module
        // would not be enabled.
        'nid_survey' => (!empty($nodes_survey[$sid][$lang]['nid_survey'])) ? $nodes_survey[$sid][$lang]['nid_survey'] : FALSE,
        'type' => $type,
      );
    }

    // Look for the token table.
    limesurvey_sync_db_open();
    foreach ($all_surveys as $sid => $survey) {
      $all_surveys[$sid]['tokens_table'] = db_table_exists(limesurvey_sync_table('tokens', $sid));
      $all_surveys[$sid]['can_edit_answer'] = ($all_surveys[$sid]['tokens_table'] && $all_surveys[$sid]['tokenanswerspersistence'] && $all_surveys[$sid]['alloweditaftercompletion']);
    }

    limesurvey_sync_db_close($previous_db);
  }

  // Deal with request :
  if ($ls_sid == 'all') {
    return $all_surveys;
  }
  elseif (isset($all_surveys[$ls_sid])) {
    return $all_surveys[$ls_sid];
  }
  else {
    return FALSE;
  }
  //
  // Example of the returned datas :
  // $all_surveys = Array
  // (
  //   [75158] => Array
  //   (
  //     [active] => TRUE
  //     [tokens_table] => TRUE
  //     [tokenanswerspersistence] => TRUE
  //     [alloweditaftercompletion] => TRUE
  //     [showwelcome] => TRUE
  //     [tokenlength] => 15
  //     [autoredirect] => TRUE
  //     [default_language] => fr
  //     [additional_languages] =>
  //     [anonymized] => TRUE
  //     [date_answer] => TRUE
  //     [ipaddr] => FALSE
  //     [refurl] => FALSE
  //     [publicstatistics] => TRUE
  //     [db_version] => TRUE
  //     [startdate] => ''
  //     [expires] => ''
  //     [is_available] => TRUE
  //     [can_edit_answer] => TRUE
  //     [http_referer] => TRUE
  //     [#lang] => Array
  //     (
  //       [fr] => Array
  //       (
  //         [title] => survey1
  //         [description] => this survey do this
  //         [back_url] => ./../site/?q=backfromsurvey/answ_type1
  //         [back_to_drupal] => TRUE
  //         [correct_back_url] => http://www.example.com/site/?q=backfromsurvey/answ_type1
  //         [nid_survey] => 12
  //         [type] => answ_type1
  //       )
  //
  //     )
  //
  //  )
  //
  //
  //  [39326] => Array
  //  (
  //     [active] => TRUE
  //     [tokens_table] => TRUE
  //     [tokenanswerspersistence] => TRUE
  //     [alloweditaftercompletion] => TRUE
  //     [showwelcome] => TRUE
  //     [tokenlength] => 15
  //     [autoredirect] => TRUE
  //     [default_language] => fr
  //     [additional_languages] =>
  //     [anonymized] => TRUE
  //     [date_answer] => TRUE
  //     [ipaddr] => FALSE
  //     [refurl] => FALSE
  //     [publicstatistics] => TRUE
  //     [db_version] => TRUE
  //     [startdate] => ''
  //     [expires] => ''
  //     [is_available] => TRUE
  //     [can_edit_answer] => TRUE
  //     [http_referer] => TRUE
  //     [#lang] => Array
  //     (
  //       [fr] => Array
  //       (
  //         [title] => survey3 (fr)
  //         [description] => blablabla
  //         [back_url] => ./../site/?q=backfromsurvey/answ_type2
  //         [back_to_drupal] => TRUE
  //         [correct_back_url] => http://www.example.com/site/?q=backfromsurvey/answ_type2
  //         [nid_survey] => 5
  //         [type] => answ_type2
  //       )
  //
  //       [es] => Array
  //       (
  //         [title] => survey3 (es)
  //         [description] => blablabla
  //         [back_url] => ./../site/
  //         [back_to_drupal] => FALSE
  //         [correct_back_url] => http://www.example.com/site/?q=backfromsurvey/answ_type3
  //         [nid_survey] => 6
  //         [type] => answ_type3
  //       )
  //
  //       [en] => Array
  //       (
  //         [title] => survey3 (en)
  //         [description] => blablabla
  //         [back_url] =>
  //         [back_to_drupal] => FALSE
  //         [correct_back_url] => http://www.example.com/site/?q=backfromsurvey/answ_type1
  //         [nid_survey] => 7
  //         [type] => answ_type4
  //       )
  //
  //     )
  //   )
  //
  // )
}

/**
 * Helper for building url to the LimeSurvey survey local settings page.
 *
 * Returns the correct anchor for a specific language tab for a defined survey
 * ID.
 *
 * @param $sid
 *   An integer, corresponding to the survey reference ID. Default is NULL.
 * @param $lang
 *   A string, corresponding to a specific language. Default is empty.
 *
 * @return
 *   A string, the corresponding anchor value.
 */
function limesurvey_sync_localsettings_anchor($sid = NULL, $lang = '') {
  // Retrieve the survey properties.
  $survey_properties = limesurvey_sync_survey_properties($sid);
  // If only one language, no specific anchor.
  if (empty($survey_properties['additional_languages'])) {
    // Target the end url textfield.
    return 'url_' . $survey_properties['default_language'];
  }
  // Handle default language.
  if ($lang == $survey_properties['default_language']) {
    return 'edittxtele0';
  }
  $anchor_id = 0;
  $add_languages = explode(' ', $survey_properties['additional_languages']);
  foreach ($add_languages as $add_lang) {
    $anchor_id++;
    if (drupal_strtolower($lang) == drupal_strtolower($add_lang)) {
      return 'edittxtele' . $anchor_id;
    }
  }
  // If the language is not available, go to the default tab.
  return 'edittxtele0';
}

/**
 * Overwrite the survey end url for the correct value.
 *
 * The survey end url must match variable values depending one conditions
 * (iframe, autoimport, etc...). This function overwrites the LimeSurvey end
 * url in order to match the correct value.
 *
 * @param $node_survey
 *   The node survey object.
 * @param $correct_url
 *   A string, the correct url to save into the LimeSurvey database.
 *   english.
 *
 * @return
 *   A boolean, TRUE if succeed.
 */
function limesurvey_sync_save_endurl($node_survey, $correct_url) {
  // Set the previous db.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $previous_db = limesurvey_sync_db_open();
  $success1 = db_update(limesurvey_sync_table('surveys_languagesettings'))
    ->fields(array('surveyls_url' => $correct_url))
    ->condition('surveyls_survey_id', $node_survey->ls_sid)
    ->condition('surveyls_language', $node_survey->ls_lang)
    ->execute();
  $success2 = db_update(limesurvey_sync_table('surveys'))
    ->fields(array('autoredirect' => 'Y'))
    ->condition('sid', $node_survey->ls_sid)
    ->execute();
  limesurvey_sync_db_close($previous_db);

  // Reset the survey properties in order to reflect those changes.
  limesurvey_sync_survey_properties(NULL, TRUE);
  return ($success1 && $success2);
}

/**
 * Convert the LimeSurvey language value to its Drupal language value.
 *
 * Use as sources :
 * - the _locale_get_predefined_list() drupal function,
 * - and the getLanguageData() LimeSurvey function.
 *
 * @param $ls_lang
 *   A string, the LimeSurvey language id.
 * @param $return_all
 *   A boolean, if TRUE, return the convert array with all languages. Default
 *   is FALSE.
 *
 * @return
 *   A string, the corresponding Drupal language id or FALSE if not finded.
 */
function limesurvey_sync_lang_convert($ls_lang, $return_all = FALSE) {
  // Here an array variable, the key is the LimeSurvey language id and the
  // value is the corresponding Drupal language id.
  $convert = array(
    'af' => 'af', //array('Afrikaans'),
    'am' => 'am', //array('Amharic', '????'),
    'ar' => 'ar', //array('Arabic', /* Left-to-right marker "?" */ '???????', LANGUAGE_RTL),
    'be' => 'be', //array('Belarusian', '??????????'),
    'bg' => 'bg', //array('Bulgarian', '?????????'),
    'bs' => 'bs', //array('Bosnian', 'Bosanski'),
    'ca' => 'ca', //array('Catalan', 'Català'),
    'cs' => 'cs', //array('Czech', 'Ceština'),
    'cs-informal' => 'cs', //array('Czech', 'Ceština'), Czech informal
    'cy' => 'cy', //array('Welsh', 'Cymraeg'),
    'da' => 'da', //array('Danish', 'Dansk'),
    'de' => 'de', //array('German', 'Deutsch'),
    'de-informal' => 'de', //array('German', 'Deutsch'), 'German (informal)'
    'el' => 'el', //array('Greek', '????????'),
    'en' => 'en', //array('English'),
    'es' => 'es', //array('Spanish', 'Español'),
    'es-CL' => 'es', //array('Spanish', 'Español'), Spanish (Chile)
    'es-MX' => 'es', //array('Spanish', 'Español'), Spanish (Mexico)
    'et' => 'et', //array('Estonian', 'Eesti'),
    'eu' => 'eu', //array('Basque', 'Euskera'),
    'fa' => 'fa', //array('Persian', /* Left-to-right marker "?" */ '?????', LANGUAGE_RTL),
    'fi' => 'fi', //array('Finnish', 'Suomi'),
    'fr' => 'fr', //array('French', 'Français'),
    'ie' => 'ga', //array('Irish', 'Gaeilge'),
    'gl' => 'gl', //array('Galician', 'Galego'),
    'he' => 'he', //array('Hebrew', /* Left-to-right marker "?" */ '?????', LANGUAGE_RTL),
    'hi' => 'hi', //array('Hindi', '??????'),
    'hr' => 'hr', //array('Croatian', 'Hrvatski'),
    'hu' => 'hu', //array('Hungarian', 'Magyar'),
    'id' => 'id', //array('Indonesian', 'Bahasa Indonesia'),
    'is' => 'is', //array('Icelandic', 'Íslenska'),
    'it' => 'it', //array('Italian', 'Italiano'),
    'it-informal' => 'it', //array('Italian', 'Italiano'), 'Italian (informal)'
    'ja' => 'ja', //array('Japanese', '???'),
    'ko' => 'ko', //array('Korean', '???'),
    'lt' => 'lt', //array('Lithuanian', 'Lietuviu'),
    'lv' => 'lv', //array('Latvian', 'Latviešu'),
    'mk' => 'mk', //array('Macedonian', '??????????'),
    'ms' => 'ms', //array('Malay', 'Bahasa Melayu'),
    'mt' => 'mt', //array('Maltese', 'Malti'),
    'nl' => 'nl', //array('Dutch', 'Nederlands'),
    'nl-informal' => 'nl', //array('Dutch', 'Nederlands'), 'Dutch (informal)'
    'nb' => 'nb', //array('Norwegian Bokmål', 'Bokmål'),
    'nn' => 'nn', //array('Norwegian Nynorsk', 'Nynorsk'),
    'pa' => 'pa', //array('Punjabi'),
    'pl' => 'pl', //array('Polish', 'Polski'),
    'pt' => 'pt', //array('Portuguese, International'), OR 'pt-pt' => array('Portuguese, Portugal', 'Português'),
    'pt-BR' => 'pt-br', //array('Portuguese, Brazil', 'Português'),
    'ro' => 'ro', //array('Romanian', 'Româna'),
    'ru' => 'ru', //array('Russian', '???????'),
    'si' => 'si', //array('Sinhala', '?????'),
    'sk' => 'sk', //array('Slovak', 'Slovencina'),
    'sl' => 'sl', //array('Slovenian', 'Slovenšcina'),
    'sq' => 'sq', //array('Albanian', 'Shqip'),
    'sr' => 'sr', //array('Serbian', '??????'),
    'sr-Latn' => 'sr', //array('Serbian', '??????'), 'Serbian (Latin)'
    'sv' => 'sv', //array('Swedish', 'Svenska'),
    'swh' => 'sw', //array('Swahili', 'Kiswahili'),
    'th' => 'th', //array('Thai', '???????'),
    'tr' => 'tr', //array('Turkish', 'Türkçe'),
    'ur' => 'ur', //array('Urdu', /* Left-to-right marker "?" */ '????', LANGUAGE_RTL),
    'vi' => 'vi', //array('Vietnamese', 'Ti?ng Vi?t'),
    'zh-Hans' => 'zh-hans', //array('Chinese, Simplified', '????'),
    'zh-Hant-HK' => 'zh-hant', //array('Chinese, Traditional', '????'), (Hong Kong)
    'zh-Hant-TW' => 'zh-hant', //array('Chinese, Traditional', '????'), (Taiwan)
  );
  if ($return_all) {
    return $convert;
  }
  if (isset($convert[$ls_lang])) {
    $drupal_lang = $convert[$ls_lang];
  }
  else {
    return FALSE;
  }
  // Note : The LimeSurvey site may use languages that are not installed on the
  // Drupal site. In such case, trying to translate the corresponding id fail,
  // returning the Drupal language id.
  return $drupal_lang;
}

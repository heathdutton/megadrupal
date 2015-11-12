<?php
/**
 * @file
 * API functions for the ls_answ module : importing answer
 */

// Code from that file should be check on each new LimeSurvey build. That why
// it is not perfectly formated using the Drupal coding standard rules.
// Help to adapt code from LimeSurvey to Drupal :
// Replace "db_table_name()" in LS by "limesurvey_sync_table()".
// Replace "$connect->getOne()" in LS by "db_query()->fetchField()" and adapt
// arguements.

/**
 * Return a formated date.
 *
 * @param $dateformatphp
 *   A string, the php format, for instance : 'd.m.Y'.
 * @param $date
 *   A string. The english formated date.
 *
 * @return
 *   A string. The converted date.
 */
function limesurvey_sync_date_time_converter($dateformatphp, $date) {
  $value = (date($dateformatphp, strtotime($date)));
  return $value;
}

/**
 * This function strips any content between and including <style>  & <javascript> tags.
 *
 * @param $content
 *   The string to clean.
 *
 * @return
 *   The cleaned string.
 */
function limesurvey_sync_answ_strip_javascript($content) {
  // From common_functions.php (1.92+) build 120613.
  $search = array(
    '@<script[^>]*?>.*?</script>@si', // Strip out javascript
    '@<style[^>]*?>.*?</style>@siU'    // Strip style tags properly
    /*               ,'@<[\/\!]*?[^<>]*?>@si',            // Strip out HTML tags
    '@<![\s\S]*?--[ \t\n\r]*>@'         // Strip multi-line comments including CDATA
    */,
  );
  $text = preg_replace($search, '', $content);
  return $text;
}

/**
 * This function sanitizes integer values.
 *
 * @param $integer
 *   The value to be sanitized.
 * @param $min
 *   A numeric. The minimal allowed value.
 * @param $max
 *   A numeric. The maximal allowed value.
 *
 * @return
 *   The cleaned string.
 */
function limesurvey_sync_answ_sanitize_int($integer, $min = '', $max = '') {
  // From limesurvey/classes/core/sanitize.php line 231 (1.92) build 120311.
  // Make int int!
  $int = preg_replace("#[^0-9]#", "", $integer);
  if ((($min != '') && ($int < $min)) || (($max != '') && ($int > $max))) {
    return FALSE;
  }
  if ($int == '') {
    return NULL;
  }
  return $int;
}

/**
 * Return the survey default language.
 *
 * The original function name on the LimeSurvey software is
 * getBaseLanguageFromSurveyID.
 *
 * @param $surveyid
 *   A numeric. The survey ID.
 *
 * @return
 *   A string, the default language.
 */
function limesurvey_sync_answ_get_base_language_from_survey_id($surveyid) {
  // From common_functions.php (1.92+) build 120613.
  static $ls_sync_cache = array();
  // $surveyid=(int)($surveyid);  // tester de retirer le commentaire
  if (!isset($ls_sync_cache[$surveyid])) {
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    $survey_properties = limesurvey_sync_survey_properties($surveyid);
    $surveylanguage = $survey_properties['default_language'];
    if (empty($surveylanguage)) {
      $surveylanguage = 'en';
    }
    $ls_sync_cache[$surveyid] = $surveylanguage;
  }
  else {
    $surveylanguage = $ls_sync_cache[$surveyid];
  }
  return $surveylanguage;
}


/**
 * This function sanitizes the language code.
 *
 * @param $codetosanitize
 *   A string. the language code ('en', 'de', etc...).
 *
 * @return
 *   The cleaned string.
 */
function limesurvey_sync_answ_sanitize_languagecode($codetosanitize) {
  // LS fonction name is sanitize_languagecode().
  return preg_replace('/[^a-z0-9-]/i', '', $codetosanitize);
}

/**
 * This function generates an array containing the fieldcode, and matching data in the same order as the activate script
 *
 * @param $surveyid
 *   A numeric or a string. The survey ID.
 * @param $style
 *   A string. 'short' (default) or 'full' - full creates extra information
 *   like default values.
 * @param $force_refresh
 *   A boolean. Forces to refresh the array.
 * @param $questionid
 *   Limit to a certain qid only (for question preview). Default is FALSE.
 * @param $s_question_language
 *   A string. The LimeSurvey language code.
 * @param $dp_langcode
 *   A string. The Drupal language code.
 *
 * @return
 *   A multidimentional array.
 */
function limesurvey_sync_answ_create_field_map($surveyid, $style = 'full', $force_refresh = FALSE, $questionid = FALSE, $s_question_language = NULL, $dp_langcode = NULL) {
  // LS fonction name is createFieldMap().
  // From common_functions.php (1.92+) build 120613,
  // and from application/helpers/common_helper.php (2.05+) build 140212.
  // Add t('Answer properties') as group_name value for token table values.
  // $clang->langcode becomes $langcode

  //global $dbprefix, $connect, $globalfieldmap, $clang, $a_duplicate_qid_s;
  static $globalfieldmap = array();
  $surveyid = limesurvey_sync_answ_sanitize_int($surveyid);

  //Get list of questions
  if (is_null($s_question_language)) {
    //if (isset($_SESSION['s_lang'])) {
    //  $s_question_language = $_SESSION['s_lang'];
    //}
    //else {
    $s_question_language = limesurvey_sync_answ_get_base_language_from_survey_id($surveyid);
    //}
  }
  $s_question_language = limesurvey_sync_answ_sanitize_languagecode($s_question_language);
  //if ($langcode != $s_question_language) {
  //  SetSurveyLanguage($surveyid, $s_question_language);
  //}
  $s_lang = ( ! empty($s_question_language)) ? $s_question_language : limesurvey_sync_answ_get_base_language_from_survey_id($surveyid);
  //$s_lang = $clang->langcode;
  $langcode = $s_lang;

  //checks to see if fieldmap has already been built for this page.
  if (isset($globalfieldmap[$surveyid][$style][$s_lang]) && $force_refresh == FALSE) {
    return $globalfieldmap[$surveyid][$style][$s_lang];
  }
  //if (isset($_SESSION['fieldmap-' . $surveyid . $s_lang]) && !$force_refresh) {
  //  return $_SESSION['fieldmap-' . $surveyid . $s_lang];
  //}

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $ls_v2 = (limesurvey_sync_lsversion() != '1.x');
  // Set the previous db connection name.
  $previous_db = limesurvey_sync_active_db_name();
  // Load translation strings before opening the new connection.
  // This needs the Drupal db connection.
  limesurvey_sync_db_close();

  // The content is static, translate it now using the correct language.
  $t_lang = (!empty($dp_langcode)) ? array('langcode' => $dp_langcode) : array();

  $t_answer_properties = t('Answer properties', array(), $t_lang);
  $t_token = t("Token", array(), $t_lang);
  $t_date_last_action = t("Date last action", array(), $t_lang);
  $t_date_started = t("Date started", array(), $t_lang);
  $t_ip_adress = t("IP address", array(), $t_lang);
  $t_referrer_url = t("Referrer URL", array(), $t_lang);

  $t_other = t('Other', array(), $t_lang);
  $t_comment = t('Comment', array(), $t_lang);
  $t_scale_1 = t('Scale 1', array(), $t_lang);
  $t_scale_2 = t('Scale 2', array(), $t_lang);
  $t_rank_s = t('Rank %s', array(), $t_lang);
  $t_other_comment = t('Other comment', array(), $t_lang);

  // Load base lang Now for the same raison.
  $base_language = limesurvey_sync_answ_get_base_language_from_survey_id($surveyid);

  $fieldmap["id"] = array(
    "fieldname" => "id",
    'sid' => $surveyid,
    'type' => "id",
    "gid" => "",
    "qid" => "",
    "aid" => "",
  );
  if ($style == "full") {
    $fieldmap["id"]['title'] = "";
    $fieldmap["id"]['question'] = t("Response ID", array(), $t_lang);
    $fieldmap["id"]['group_name'] = $t_answer_properties;
  }

  $fieldmap["submitdate"] = array(
    "fieldname" => "submitdate",
    'type' => "submitdate",
    'sid' => $surveyid,
    "gid" => "",
    "qid" => "",
    "aid" => "",
  );
  if ($style == "full") {
    $fieldmap["submitdate"]['title'] = "";
    $fieldmap["submitdate"]['question'] = t("Date submitted", array(), $t_lang);
    $fieldmap["submitdate"]['group_name'] = $t_answer_properties;
  }

  $fieldmap["lastpage"] = array(
    "fieldname" => "lastpage",
    'sid' => $surveyid,
    'type' => "lastpage",
    "gid" => "",
    "qid" => "",
    "aid" => "",
  );
  if ($style == "full") {
    $fieldmap["lastpage"]['title'] = "";
    $fieldmap["lastpage"]['question'] = t("Last page", array(), $t_lang);
    $fieldmap["lastpage"]['group_name'] = $t_answer_properties;
  }

  $fieldmap["startlanguage"] = array(
    "fieldname" => "startlanguage",
    'sid' => $surveyid,
    'type' => "startlanguage",
    "gid" => "",
    "qid" => "",
    "aid" => "",
  );
  if ($style == "full") {
    $fieldmap["startlanguage"]['title'] = "";
    $fieldmap["startlanguage"]['question'] = t("Start language", array(), $t_lang);
    $fieldmap["startlanguage"]['group_name'] = $t_answer_properties;
  }

  limesurvey_sync_db_open();

  //Check for any additional fields for this survey and create necessary fields (token and datestamp and ipaddr)
  $pquery = "SELECT anonymized, datestamp, ipaddr, refurl FROM {" . limesurvey_sync_table('surveys') . "} WHERE sid=:sid";
  $presult = db_query($pquery, array(':sid' => $surveyid)); //Checked
  while ($prow = $presult->fetchAssoc()) {
    if ($prow['anonymized'] == "N") {
      $fieldmap["token"] = array(
        "fieldname" => "token",
        'sid' => $surveyid,
        'type' => "token",
        "gid" => "",
        "qid" => "",
        "aid" => "",
      );
      if ($style == "full") {
        $fieldmap["token"]['title'] = "";
        $fieldmap["token"]['question'] = $t_token;
        $fieldmap["token"]['group_name'] = $t_answer_properties;
      }
    }
    if ($prow['datestamp'] == "Y") {
      $fieldmap["datestamp"] = array(
        "fieldname" => "datestamp",
        'type' => "datestamp",
        'sid' => $surveyid,
        "gid" => "",
        "qid" => "",
        "aid" => "",
      );
      if ($style == "full") {
        $fieldmap["datestamp"]['title'] = "";
        $fieldmap["datestamp"]['question'] = $t_date_last_action;
        $fieldmap["datestamp"]['group_name'] = $t_answer_properties;
      }
      $fieldmap["startdate"] = array(
        "fieldname" => "startdate",
        'type' => "startdate",
        'sid' => $surveyid,
        "gid" => "",
        "qid" => "",
        "aid" => "",
      );
      if ($style == "full") {
        $fieldmap["startdate"]['title'] = "";
        $fieldmap["startdate"]['question'] = $t_date_started;
        $fieldmap["startdate"]['group_name'] = $t_answer_properties;
      }

    }
    if ($prow['ipaddr'] == "Y") {
      $fieldmap["ipaddr"] = array(
        "fieldname" => "ipaddr",
        'type' => "ipaddress",
        'sid' => $surveyid,
        "gid" => "",
        "qid" => "",
        "aid" => "",
      );
      if ($style == "full") {
        $fieldmap["ipaddr"]['title'] = "";
        $fieldmap["ipaddr"]['question'] = $t_ip_adress;
        $fieldmap["ipaddr"]['group_name'] = $t_answer_properties;
      }
    }
    // Add 'refurl' to fieldmap.
    if ($prow['refurl'] == "Y") {
      $fieldmap["refurl"] = array(
        "fieldname" => "refurl",
        'type' => "url",
        'sid' => $surveyid,
        "gid" => "",
        "qid" => "",
        "aid" => "",
      );
      if ($style == "full") {
        $fieldmap["refurl"]['title'] = "";
        $fieldmap["refurl"]['question'] = $t_referrer_url;
        $fieldmap["refurl"]['group_name'] = $t_answer_properties;
      }
    }
  }

  // Collect all default values once so don't need separate query for each question with defaults
  // First collect language specific defaults
  $defaults_query = "SELECT a.qid, a.sqid, a.scale_id, a.specialtype, a.defaultvalue"
        . " FROM {" . limesurvey_sync_table('defaultvalues') . "} AS a, {" . limesurvey_sync_table('questions') . "} AS b"
        . " WHERE a.qid = b.qid"
        . " AND a.language = b.language"
        . " AND a.language = :a_language"
        . " AND b.same_default=:b_same_default"
        . " AND b.sid = :b_sid";
  $default_results = db_query($defaults_query, array(':a_language' => $s_lang, ':b_same_default' => 0, ':b_sid' => $surveyid));

  $default_values = array(); // indexed by question then subquestion
  while ($dv = $default_results->fetchAssoc()) {
    if ($dv['specialtype'] != '') {
      $sq = $dv['specialtype'];
    }
    else {
      $sq = $dv['sqid'];
    }
    $default_values[$dv['qid'] . '~' . $sq] = $dv['defaultvalue'];
  }

  // Now overwrite language-specific defaults (if any) base language values for each question that uses same_defaults=1
  //$base_language = limesurvey_sync_answ_get_base_language_from_survey_id($surveyid);
  $defaults_query = "SELECT a.qid, a.sqid, a.scale_id, a.specialtype, a.defaultvalue"
        . " FROM {" . limesurvey_sync_table('defaultvalues') . "} AS a, {" . limesurvey_sync_table('questions') . "} AS b"
        . " WHERE a.qid = b.qid"
        . " AND a.language = b.language"
        . " AND a.language = :a_language "
        . " AND b.same_default=:b_same_default"
        . " AND b.sid =:b_sid";
  $default_results = db_query($defaults_query, array(':a_language' => $base_language, ':b_same_default' => 1, ':b_sid' => $surveyid));

  while ($dv = $default_results->fetchAssoc()) {
    if ($dv['specialtype'] != '') {
      $sq = $dv['specialtype'];
    }
    else {
      $sq = $dv['sqid'];
    }
    $default_values[$dv['qid'] . '~' . $sq] = $dv['defaultvalue'];
  }

  $qtypes = limesurvey_sync_answ_getqtypelist('', 'array', $s_lang, $dp_langcode);

  $aquery_data = array(
    ':questions_sid' => $surveyid,
    ':questions_language' => $s_lang,
    ':questions_parent_qid' => 0,
    ':groups_language' => $s_lang,
  );
  $aquery = "SELECT * "
        . " FROM {" . limesurvey_sync_table('questions') . "} AS questions, {" . limesurvey_sync_table('groups') . "} AS groups"
        . " WHERE questions.gid=groups.gid AND "
        . " questions.sid=:questions_sid AND "
        . " questions.language=:questions_language AND "
        . " questions.parent_qid=:questions_parent_qid AND "
        . " groups.language=:groups_language ";
  if ($questionid !== FALSE) {
    $aquery .= " and questions.qid=:questions_qid ";
    $aquery_data = array_merge($aquery_data, array(':questions_qid' => $questionid));
  }
  $aquery .= " ORDER BY group_order, question_order";
  $aresult = db_query($aquery, $aquery_data); //Checked

  $question_seq = -1; // this is incremental question sequence across all groups
  $group_seq = -1;
  $_groupOrder = -1;

  while ($arow = $aresult->fetchAssoc()) {
    ++$question_seq;

    // fix fact taht group_order may have gaps
    if ($_groupOrder != $arow['group_order']) {
      $_groupOrder = $arow['group_order'];
      ++$group_seq;
    }

    // Conditions indicators are obsolete with EM.  However, they are so tightly coupled into LS code that easider to just set values to 'N' for now and refactor later.
    $conditions = 'N';
    $usedinconditions = 'N';

    // Field identifier
    // GXQXSXA
    // G=Group  Q=Question S=Subquestion A=Answer Option
    // If S or A don't exist then set it to 0
    // Implicit (subqestion intermal to a question type ) or explicit qubquestions/answer count starts at 1

    // Types "L", "!" , "O", "D", "G", "N", "X", "Y", "5","S","T","U","*"
    $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}";

    if ($qtypes[$arow['type']]['subquestions'] == 0 && $arow['type'] != "R" && $arow['type'] != "|") {
      if (isset($fieldmap[$fieldname])) {
        $a_duplicate_qid_s[$arow['qid']] = array(
          'fieldname' => $fieldname,
          'question' => $arow['question'],
          'gid' => $arow['gid'],
        );
      }
      $fieldmap[$fieldname] = array(
        "fieldname" => $fieldname,
        'type' => "{$arow['type']}",
        'sid' => $surveyid,
        "gid" => $arow['gid'],
        "qid" => $arow['qid'],
        "aid" => "",
      );
      if ($style == "full") {
        $fieldmap[$fieldname]['title'] = $arow['title'];
        $fieldmap[$fieldname]['question'] = $arow['question'];
        $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
        $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
        $fieldmap[$fieldname]['hasconditions'] = $conditions;
        $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
        $fieldmap[$fieldname]['questionSeq'] = $question_seq;
        $fieldmap[$fieldname]['groupSeq'] = $group_seq;
        if (isset($default_values[$arow['qid'] . '~0'])) {
          $fieldmap[$fieldname]['defaultvalue'] = $default_values[$arow['qid'] . '~0'];
        }
      }
      switch ($arow['type']) {
        case "L": //RADIO LIST
        case "!": //DROPDOWN LIST
          if (!$ls_v2) {
            $fieldmap[$fieldname]['other'] = $arow['other']; // so that base variable knows whether has other value  // Removed into build121101.
          }
          if ($arow['other'] == "Y") {
            $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}other";
            if (isset($fieldmap[$fieldname])) {
              $a_duplicate_qid_s[$arow['qid']] = array(
                'fieldname' => $fieldname,
                'question' => $arow['question'],
                'gid' => $arow['gid'],
              );
            }

            $fieldmap[$fieldname] = array(
              "fieldname" => $fieldname,
              'type' => $arow['type'],
              'sid' => $surveyid,
              "gid" => $arow['gid'],
              "qid" => $arow['qid'],
              "aid" => "other",
            );
            // dgk bug fix line above. aid should be set to "other" for export to append to the field name in the header line.
            if ($style == "full") {
              $fieldmap[$fieldname]['title'] = $arow['title'];
              $fieldmap[$fieldname]['question'] = $arow['question'];
              $fieldmap[$fieldname]['subquestion'] = $t_other;
              $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
              $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
              $fieldmap[$fieldname]['hasconditions'] = $conditions;
              $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
              $fieldmap[$fieldname]['questionSeq'] = $question_seq;
              $fieldmap[$fieldname]['groupSeq'] = $group_seq;
              if (!$ls_v2) {
                $fieldmap[$fieldname]['other'] = $arow['other']; // Removed into build121101.
              }
              if (isset($default_values[$arow['qid'] . '~other'])) {
                $fieldmap[$fieldname]['defaultvalue'] = $default_values[$arow['qid'] . '~other'];
              }
            }
          }
          break;
        case "O": //DROPDOWN LIST WITH COMMENT
          $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}comment";
          if (isset($fieldmap[$fieldname])) {
            $a_duplicate_qid_s[$arow['qid']] = array(
              'fieldname' => $fieldname,
              'question' => $arow['question'],
              'gid' => $arow['gid'],
            );
          }

          $fieldmap[$fieldname] = array(
            "fieldname" => $fieldname,
            'type' => $arow['type'],
            'sid' => $surveyid,
            "gid" => $arow['gid'],
            "qid" => $arow['qid'],
            "aid" => "comment",
          );
          // dgk bug fix line below. aid should be set to "comment" for export to append to the field name in the header line. Also needed set the type element correctly.
          if ($style == "full") {
            $fieldmap[$fieldname]['title'] = $arow['title'];
            $fieldmap[$fieldname]['question'] = $arow['question'];
            $fieldmap[$fieldname]['subquestion'] = $t_comment;
            $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
            $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
            $fieldmap[$fieldname]['hasconditions'] = $conditions;
            $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
            $fieldmap[$fieldname]['questionSeq'] = $question_seq;
            $fieldmap[$fieldname]['groupSeq'] = $group_seq;
          }
          break;
      }
    }
    // For Multi flexi question types
    elseif ($qtypes[$arow['type']]['subquestions'] == 2 && $qtypes[$arow['type']]['answerscales'] == 0) {
      //MULTI FLEXI
      $abrows = limesurvey_sync_answ_get_sub_questions($surveyid, $arow['qid'], $s_lang);
      //Now first process scale=1
      $answerset = array();
      $answer_list = array();
      foreach ($abrows as $key => $abrow) {
        if ($abrow['scale_id'] == 1) {
          $answerset[] = $abrow;
          $answer_list[] = array(
            'code' => $abrow['title'],
            'answer' => $abrow['question'],
          );
          unset($abrows[$key]);
        }
      }
      reset($abrows);
      foreach ($abrows as $abrow) {
        foreach ($answerset as $answer) {
          $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}{$abrow['title']}_{$answer['title']}";
          if (isset($fieldmap[$fieldname])) {
            $a_duplicate_qid_s[$arow['qid']] = array(
              'fieldname' => $fieldname,
              'question' => $arow['question'],
              'gid' => $arow['gid'],
            );
          }
          $fieldmap[$fieldname] = array(
            "fieldname" => $fieldname,
            'type' => $arow['type'],
            'sid' => $surveyid,
            "gid" => $arow['gid'],
            "qid" => $arow['qid'],
            "aid" => $abrow['title'] . "_" . $answer['title'],
            "sqid" => $abrow['qid'],
          );
          if ($abrow['other'] == "Y") {
            $alsoother = "Y";
          }
          if ($style == "full") {
            $fieldmap[$fieldname]['title'] = $arow['title'];
            $fieldmap[$fieldname]['question'] = $arow['question'];
            $fieldmap[$fieldname]['subquestion1'] = $abrow['question'];
            $fieldmap[$fieldname]['subquestion2'] = $answer['question'];
            $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
            $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
            $fieldmap[$fieldname]['hasconditions'] = $conditions;
            $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
            $fieldmap[$fieldname]['questionSeq'] = $question_seq;
            $fieldmap[$fieldname]['groupSeq'] = $group_seq;
            $fieldmap[$fieldname]['preg'] = $arow['preg'];
            $fieldmap[$fieldname]['answerList'] = $answer_list;
          }
        }
      }
      unset($answerset);
    }
    elseif ($arow['type'] == "1") {
      $abrows = limesurvey_sync_answ_get_sub_questions($surveyid, $arow['qid'], $s_lang);
      foreach ($abrows as $abrow) {
        $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}{$abrow['title']}#0";
        if (isset($fieldmap[$fieldname])) {
          $a_duplicate_qid_s[$arow['qid']] = array(
            'fieldname' => $fieldname,
            'question' => $arow['question'],
            'gid' => $arow['gid'],
          );
        }
        $fieldmap[$fieldname] = array(
          "fieldname" => $fieldname,
          'type' => $arow['type'],
          'sid' => $surveyid,
          "gid" => $arow['gid'],
          "qid" => $arow['qid'],
          "aid" => $abrow['title'],
          "scale_id" => 0,
        );
        if ($style == "full") {
          $fieldmap[$fieldname]['title'] = $arow['title'];
          $fieldmap[$fieldname]['question'] = $arow['question'];
          $fieldmap[$fieldname]['subquestion'] = $abrow['question'];
          $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
          $fieldmap[$fieldname]['scale'] = $t_scale_1;
          $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
          $fieldmap[$fieldname]['hasconditions'] = $conditions;
          $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
          $fieldmap[$fieldname]['questionSeq'] = $question_seq;
          $fieldmap[$fieldname]['groupSeq'] = $group_seq;
        }

        $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}{$abrow['title']}#1";
        if (isset($fieldmap[$fieldname])) {
          $a_duplicate_qid_s[$arow['qid']] = array(
            'fieldname' => $fieldname,
            'question' => $arow['question'],
            'gid' => $arow['gid'],
          );
        }
        $fieldmap[$fieldname] = array(
          "fieldname" => $fieldname,
          'type' => $arow['type'],
          'sid' => $surveyid,
          "gid" => $arow['gid'],
          "qid" => $arow['qid'],
          "aid" => $abrow['title'],
          "scale_id" => 1,
        );
        if ($style == "full") {
          $fieldmap[$fieldname]['title'] = $arow['title'];
          $fieldmap[$fieldname]['question'] = $arow['question'];
          $fieldmap[$fieldname]['subquestion'] = $abrow['question'];
          $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
          $fieldmap[$fieldname]['scale'] = $t_scale_2;
          $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
          $fieldmap[$fieldname]['hasconditions'] = $conditions;
          $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
          $fieldmap[$fieldname]['questionSeq'] = $question_seq;
          $fieldmap[$fieldname]['groupSeq'] = $group_seq;
        }
      }
    }

    elseif ($arow['type'] == "R") {
      //MULTI ENTRY
      $slots = db_query("select count(code) from {" . limesurvey_sync_table('answers') . "} where qid=:quid and language=:language", array(':quid' => "{$arow['qid']}", ':language' => "'{$s_lang}'"))->fetchField();
      for ($i = 1; $i <= $slots; $i++) {
        $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}$i";
        if (isset($fieldmap[$fieldname])) {
          $a_duplicate_qid_s[$arow['qid']] = array(
            'fieldname' => $fieldname,
            'question' => $arow['question'],
            'gid' => $arow['gid'],
          );
        }
        $fieldmap[$fieldname] = array(
          "fieldname" => $fieldname,
          'type' => $arow['type'],
          'sid' => $surveyid,
          "gid" => $arow['gid'],
          "qid" => $arow['qid'],
          "aid" => $i,
        );
        if ($style == "full") {
          $fieldmap[$fieldname]['title'] = $arow['title'];
          $fieldmap[$fieldname]['question'] = $arow['question'];
          $fieldmap[$fieldname]['subquestion'] = sprintf($t_rank_s, $i);
          $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
          $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
          $fieldmap[$fieldname]['hasconditions'] = $conditions;
          $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
          $fieldmap[$fieldname]['questionSeq'] = $question_seq;
          $fieldmap[$fieldname]['groupSeq'] = $group_seq;
        }
      }
    }
    elseif ($arow['type'] == "|") {
      $abquery = "SELECT value FROM {" . limesurvey_sync_table('question_attributes')
                . "} WHERE attribute=:attribute AND qid=:qid";
      $abresult = db_query($abquery, array(':attribute' => 'max_num_of_files', ':qid' => $arow['qid']));
      $abrow = $abresult->fetchAssoc();

      $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}";
      $fieldmap[$fieldname] = array(
        "fieldname" => $fieldname,
        'type' => $arow['type'],
        'sid' => $surveyid,
        "gid" => $arow['gid'],
        "qid" => $arow['qid'],
        "aid" => '',
      );
      if ($style == "full") {
        $fieldmap[$fieldname]['title'] = $arow['title'];
        $fieldmap[$fieldname]['question'] = $arow['question'];
        $fieldmap[$fieldname]['max_files'] = $abrow['value'];
        $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
        $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
        $fieldmap[$fieldname]['hasconditions'] = $conditions;
        $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
        $fieldmap[$fieldname]['questionSeq'] = $question_seq;
        $fieldmap[$fieldname]['groupSeq'] = $group_seq;
      }
      $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}" . "_filecount";
      $fieldmap[$fieldname] = array(
        "fieldname" => $fieldname,
        'type' => $arow['type'],
        'sid' => $surveyid,
        "gid" => $arow['gid'],
        "qid" => $arow['qid'],
        "aid" => "filecount",
      );
      if ($style == "full") {
        $fieldmap[$fieldname]['title'] = $arow['title'];
        $fieldmap[$fieldname]['question'] = "filecount - " . $arow['question'];
        //$fieldmap[$fieldname]['subquestion']=$t_comment;
        $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
        $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
        $fieldmap[$fieldname]['hasconditions'] = $conditions;
        $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
        $fieldmap[$fieldname]['questionSeq'] = $question_seq;
        $fieldmap[$fieldname]['groupSeq'] = $group_seq;
      }
    }
    else {
      //MULTI ENTRY
      $abrows = limesurvey_sync_answ_get_sub_questions($surveyid, $arow['qid'], $s_lang);
      foreach ($abrows as $abrow) {
        $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}{$abrow['title']}";
        if (isset($fieldmap[$fieldname])) {
          $a_duplicate_qid_s[$arow['qid']] = array(
            'fieldname' => $fieldname,
            'question' => $arow['question'],
            'gid' => $arow['gid'],
          );
        }
        $fieldmap[$fieldname] = array(
          "fieldname" => $fieldname,
          'type' => $arow['type'],
          'sid' => $surveyid,
          'gid' => $arow['gid'],
          'qid' => $arow['qid'],
          'aid' => $abrow['title'],
          'sqid' => $abrow['qid'],
        );
        if ($style == "full") {
          $fieldmap[$fieldname]['title'] = $arow['title'];
          $fieldmap[$fieldname]['question'] = $arow['question'];
          $fieldmap[$fieldname]['subquestion'] = $abrow['question'];
          $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
          $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
          $fieldmap[$fieldname]['hasconditions'] = $conditions;
          $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
          $fieldmap[$fieldname]['questionSeq'] = $question_seq;
          $fieldmap[$fieldname]['groupSeq'] = $group_seq;
          $fieldmap[$fieldname]['preg'] = $arow['preg'];
          if (isset($default_values[$arow['qid'] . '~' . $abrow['qid']])) {
            $fieldmap[$fieldname]['defaultvalue'] = $default_values[$arow['qid'] . '~' . $abrow['qid']];
          }
        }
        if ($arow['type'] == "P") {
          $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}{$abrow['title']}comment";
          if (isset($fieldmap[$fieldname])) {
            $a_duplicate_qid_s[$arow['qid']] = array(
              'fieldname' => $fieldname,
              'question' => $arow['question'],
              'gid' => $arow['gid'],
            );
          }
          $fieldmap[$fieldname] = array(
            "fieldname" => $fieldname,
            'type' => $arow['type'],
            'sid' => $surveyid,
            "gid" => $arow['gid'],
            "qid" => $arow['qid'],
            "aid" => $abrow['title'] . "comment",
          );
          if ($style == "full") {
            $fieldmap[$fieldname]['title'] = $arow['title'];
            $fieldmap[$fieldname]['question'] = $arow['question'];
            $fieldmap[$fieldname]['subquestion'] = $t_comment;
            $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
            $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
            $fieldmap[$fieldname]['hasconditions'] = $conditions;
            $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
            $fieldmap[$fieldname]['questionSeq'] = $question_seq;
            $fieldmap[$fieldname]['groupSeq'] = $group_seq;
          }
        }
      }
      if ($arow['other'] == "Y" && ($arow['type'] == "M" || $arow['type'] == "P")) {
        $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}other";
        if (isset($fieldmap[$fieldname])) {
          $a_duplicate_qid_s[$arow['qid']] = array(
            'fieldname' => $fieldname,
            'question' => $arow['question'],
            'gid' => $arow['gid'],
          );
        }
        $fieldmap[$fieldname] = array(
          "fieldname" => $fieldname,
          'type' => $arow['type'],
          'sid' => $surveyid,
          "gid" => $arow['gid'],
          "qid" => $arow['qid'],
          "aid" => "other",
        );
        if ($style == "full") {
          $fieldmap[$fieldname]['title'] = $arow['title'];
          $fieldmap[$fieldname]['question'] = $arow['question'];
          $fieldmap[$fieldname]['subquestion'] = $t_other;
          $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
          $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
          $fieldmap[$fieldname]['hasconditions'] = $conditions;
          $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
          $fieldmap[$fieldname]['questionSeq'] = $question_seq;
          $fieldmap[$fieldname]['groupSeq'] = $group_seq;
          $fieldmap[$fieldname]['other'] = $arow['other'];
        }
        if ($arow['type'] == "P") {
          $fieldname = "{$arow['sid']}X{$arow['gid']}X{$arow['qid']}othercomment";
          if (isset($fieldmap[$fieldname])) {
            $a_duplicate_qid_s[$arow['qid']] = array(
              'fieldname' => $fieldname,
              'question' => $arow['question'],
              'gid' => $arow['gid'],
            );
          }
          $fieldmap[$fieldname] = array(
            "fieldname" => $fieldname,
            'type' => $arow['type'],
            'sid' => $surveyid,
            "gid" => $arow['gid'],
            "qid" => $arow['qid'],
            "aid" => "othercomment",
          );
          if ($style == "full") {
            $fieldmap[$fieldname]['title'] = $arow['title'];
            $fieldmap[$fieldname]['question'] = $arow['question'];
            $fieldmap[$fieldname]['subquestion'] = $t_other_comment;
            $fieldmap[$fieldname]['group_name'] = $arow['group_name'];
            $fieldmap[$fieldname]['mandatory'] = $arow['mandatory'];
            $fieldmap[$fieldname]['hasconditions'] = $conditions;
            $fieldmap[$fieldname]['usedinconditions'] = $usedinconditions;
            $fieldmap[$fieldname]['questionSeq'] = $question_seq;
            $fieldmap[$fieldname]['groupSeq'] = $group_seq;
            $fieldmap[$fieldname]['other'] = $arow['other'];
          }
        }
      }
    }
    // Relevance and grelevance added sinc v1.92RC2.
    if (isset($fieldmap[$fieldname])) {
      $fieldmap[$fieldname]['relevance'] = $arow['relevance'];
      $fieldmap[$fieldname]['relevance'] = (isset($arow['relevance'])) ? $arow['relevance'] : '';
      $fieldmap[$fieldname]['grelevance'] = (isset($arow['grelevance'])) ? $arow['grelevance'] : '';
      $fieldmap[$fieldname]['questionSeq'] = $question_seq;
      $fieldmap[$fieldname]['groupSeq'] = $group_seq;
      $fieldmap[$fieldname]['preg'] = $arow['preg'];
      $fieldmap[$fieldname]['other'] = $arow['other'];
      $fieldmap[$fieldname]['help'] = $arow['help'];
    }
    else {
      --$question_seq; // didn't generate a valid $fieldmap entry, so decrement the question counter to ensure they are sequential
    }

  }
  limesurvey_sync_db_close($previous_db);
  if (isset($fieldmap)) {
    $globalfieldmap[$surveyid][$style][$langcode] = $fieldmap;
    //$_SESSION['fieldmap-' . $surveyid . $langcode]=$fieldmap;
    return $fieldmap;
  }
}


/**
 * Return a formated value depending on field.
 *
 * For example : 'YES' instead of 'Y'.
 *
 * @param $fieldcode
 *   A string. Fieldcode reference.
 * @param $value
 *   The correponding value for that fielcode.
 * @param $surveyid
 *   A numeric or a string. The survey ID.
 * @param $s_lang
 *   A string. The language code.
 * @param $dp_langcode
 *   A string. The Drupal language code.
 * @param $format
 *   A string. Not used on this module.
 * @param $dateformatphp
 *   A string, the php format, for instance : 'd.m.Y'.
 * @param $style
 *   A string. 'short' (default) or 'full' - full creates extra information
 *   like default values.
 *
 * @return
 *   A string. The formated value.
 */
function limesurvey_sync_answ_getextendedanswer($fieldcode, $value, $surveyid, $s_lang, $dp_langcode = NULL, $format = '', $dateformatphp = 'd.m.Y', $style = 'full') {
  // LS fonction name is getExtendedAnswer().
  // From common_functions.php (1.92+) build 120613,
  // and from application/helpers/common_helper.php (2.05+) build 140212.
  // Add $surveyid, $s_lang and style in arguements !
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $ls_v2 = (limesurvey_sync_lsversion() != '1.x');
  // Set the previous db connection name.
  $previous_db = limesurvey_sync_active_db_name();
  // Set it to Drupal for the t() function.
  limesurvey_sync_db_close();
  // use Survey base language if s_lang isn't set in _SESSION (when browsing answers)
  //$s_lang = limesurvey_sync_answ_get_base_language_from_survey_id($surveyid);
  //if  (!isset($action) || (isset($action) && $action!='browse') )
  //{
  //  if (isset($_SESSION['s_lang'])) $s_lang = $_SESSION['s_lang'];  //This one does not work in admin mode when you browse a particular answer
  //}

  //Fieldcode used to determine question, $value used to match against answer code
  //Returns NULL if question type does not suit
  if (($ls_v2 && strpos($fieldcode, "{$surveyid}X") === 0) || (!$ls_v2 && substr_count($fieldcode, "X") > 1)) {
    $fieldmap = limesurvey_sync_answ_create_field_map($surveyid, 'full', FALSE, FALSE, $s_lang, $dp_langcode); // add arguements to keep the language.
    if (isset($fieldmap[$fieldcode])) {
      $fields = $fieldmap[$fieldcode];
    }
    else {
      return FALSE;
    }
    // Added into the 7.x-1.6 release.
    // If it is a comment field there is nothing to convert here
    if ($fields['aid'] == 'comment') return $value;

    $t_lang = (!empty($dp_langcode)) ? array('langcode' => $dp_langcode) : array();

    //Find out the question type
    $this_type = $fields['type'];
    switch ($this_type) {
      case 'D':
        if (trim($value) != '') {
          $value = limesurvey_sync_date_time_converter($dateformatphp, $value);
        }
        break;
        // Added into 7.x-1.3 from build 130611.
      case 'N':
        if (trim($value) != '') {
          if (strpos($value, ".") !== FALSE) {
            $value = rtrim(rtrim($value, "0"), ".");
          }
          limesurvey_sync_db_open();
          $nattributes = db_query("SELECT value FROM {" . limesurvey_sync_table('question_attributes') . "} WHERE attribute = :attribute AND qid = :qid", array(':attribute' => 'num_value_int_only', ':qid' => $fields['qid']))->fetchField();
          limesurvey_sync_db_close();
          if ($nattributes) {
            $value = number_format($value, 0, '', '');
          }
        }
        break;
      case "L":
      case "!":
      case "O":
      case "^":
      case "I":
      case "R":
        limesurvey_sync_db_open();
        $query = "SELECT code, answer FROM {" . limesurvey_sync_table('answers') . "} WHERE qid=:qid AND code=:code AND scale_id=:scale_id AND language=:language";
        $result = db_query($query, array(':qid' => $fields['qid'], ':code' => $value, ':scale_id' => 0, ':language' => $s_lang)); //Checked
        while ($row = $result->fetchAssoc()) {
          $this_answer = $row['answer'];
        } // while
        // Load the Drupal db connection for the t() function.
        limesurvey_sync_db_close();

        if ($value == "-oth-") {
          $this_answer = t("Other", array(), $t_lang);
        }
        break;
      case "M":
      case "J":
      case "P":
        switch ($value) {
          case "Y":
            $this_answer = t("Yes", array(), $t_lang);
            break;
        }
        break;
      case "Y":
        switch ($value) {
          case "Y":
            $this_answer = t("Yes", array(), $t_lang);
            break;
          case "N":
            $this_answer = t("No", array(), $t_lang);
            break;
          default:
            $this_answer = t("No answer", array(), $t_lang);
        }
        break;
      case "G":
        switch ($value) {
          case "M":
            $this_answer = t("Male", array(), $t_lang);
            break;
          case "F":
            $this_answer = t("Female", array(), $t_lang);
            break;
          default:
            $this_answer = t("No answer", array(), $t_lang);
        }
        break;
      case "C":
        switch ($value) {
          case "Y":
            $this_answer = t("Yes", array(), $t_lang);
            break;
          case "N":
            $this_answer = t("No", array(), $t_lang);
            break;
          case "U":
            $this_answer = t("Uncertain", array(), $t_lang);
            break;
        }
        break;
      case "E":
        switch ($value) {
          case "I":
            $this_answer = t("Increase", array(), $t_lang);
            break;
          case "D":
            $this_answer = t("Decrease", array(), $t_lang);
            break;
          case "S":
            $this_answer = t("Same", array(), $t_lang);
            break;
        }
        break;
      case "F":
      case "H":
      case "1":
        $query = "SELECT answer FROM {" . limesurvey_sync_table('answers') . "} WHERE qid=:qid AND code=:code AND language=:language";
        $query_data = array(
          ':qid' => $fields['qid'],
          ':code' => $value,
          ':language' => $s_lang,
        );
        if (isset($fields['scale_id'])) {
          $query .= " AND scale_id=:scale_id";
          $query_data = array_merge($query_data, array(':scale_id' => $fields['scale_id']));
        }
        limesurvey_sync_db_open();
        $result = db_query($query, $query_data); //Checked
        while ($row = $result->fetchAssoc()) {
          $this_answer = $row['answer'];
        } // while
        limesurvey_sync_db_close();
        if ($value == "-oth-") {
          $this_answer = t("Other", array(), $t_lang);
        }
        break;
      case "|": //File upload
        if (drupal_substr($fieldcode, -9) == 'filecount') {
          $this_answer = t("File count", array(), $t_lang);
        }
        else {
          //Show the filename, size, title and comment -- no link!
          $files = json_decode($value);
          $value = '';
          if (is_array($files)) {
            foreach ($files as $file) {
              $value .= $file->name .
                                    ' (' . $file->size . 'KB) ' .
                                    strip_tags($file->title) .
                                    ' - ' . strip_tags($file->comment) . "<br/>";
            }
          }
        }
        break;
      default:
    } // switch
  }
  limesurvey_sync_db_close($previous_db);
  // Add from the LimeSurvey_sync module : Handle 'submidate' and other ls
  // standard fields into the limesurvey_sync_answ_import_single_answer() function.
  switch ($fieldcode) { // Introduced into the v(2.00+) build 121115.
    case 'submitdate':
    case 'startdate':
    case 'datestamp':
      if (trim($value) != '') {
        // Handle by the module.
        //$dateformatdetails = getDateFormatDataForQID(array('date_format'=>''), $iSurveyID);
        //$value=convertDateTimeFormat($sValue,"Y-m-d H:i:s",$dateformatdetails['phpdate'].' H:i:s');
      }
      break;
  }
  if (isset($this_answer)) {
    if ($format != 'INSERTANS') {
      return $this_answer . " [$value]";
    }
    else {
      if (strip_tags($this_answer) == "") {
        switch ($this_type) { // for questions with answers beeing
          // answer code, it is safe to return the
          // code instead of the blank stripped answer
          case "A":
          case "B":
          case "C":
          case "E":
          case "F":
          case "H":
          case "1":
          case "M":
          case "P":
          case "!":
          case "5":
          case "L":
          case "O":
            return $value;
            break;
          default:
            return strip_tags($this_answer);
            break;
        }
      }
      else {
        return strip_tags($this_answer);
      }
    }
  }
  else {
    return $value;
  }
}

/**
 * Returns list of question types available in LimeSurvey.
 *
 * Explanation of questiontype array:
 * description : Question description.
 * subquestions : 0= Does not support subquestions x=Number of subquestion
 * scales.
 * answerscales : 0= Does not need answers x=Number of answer scales (usually
 * 1, but e.g. for dual scale question set to 2).
 * assessable : 0=Does not support assessment values when editing answerd
 * 1=Support assessment values.
 *
 * @param $selected_code
 *   A sring. Value of the Question Type (defaults to "T").
 * @param $return_type
 *   A string. Type of output from this function (defaults to selector).
 * @param $s_lang
 *   A string. The LimeSurvey language code.
 * @param $dp_langcode
 *   A string. The Drupal language code.
 *
 * @return
 *   depending on $return_type param, returns a straight "array" of question
 *   types, or an <option></option> list.
 */
function limesurvey_sync_answ_getqtypelist($selected_code = "T", $return_type = "selector", $s_lang = 'en', $dp_langcode = NULL) {
  // From the getqtypelist() function, common_functions.php (1.92+) build 120613.
  // From the getQuestionTypeList() function, application/helpers/common_helper.php
  // (2.05+) build 140212.
  //global $publicurl;
  //global $sourcefrom, $clang;

  //if (!isset($clang))
  //{
  //$clang = new limesurvey_lang("en");
  //}
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  // Retrieve the previous db name.
  $previous_db = limesurvey_sync_active_db_name();
  // Set to the Drupal database for t() function;
  limesurvey_sync_db_close();

  $t_lang = (!empty($dp_langcode)) ? array('langcode' => $dp_langcode) : array();

  $group['Arrays'] = t('Arrays', array(), $t_lang);
  $group['MaskQuestions'] = t("Mask questions", array(), $t_lang);
  $group['SinChoiceQues'] = t("Single choice questions", array(), $t_lang);
  $group['MulChoiceQues'] = t("Multiple choice questions", array(), $t_lang);
  $group['TextQuestions'] = t("Text questions", array(), $t_lang);


  $qtypes = array(
    "1" => array(
      'description' => t("Array dual scale", array(), $t_lang),
      'group' => $group['Arrays'],
      'subquestions' => 1,
      'assessable' => 1,
      'hasdefaultvalues' => 0,
      'answerscales' => 2,
    ),
    "5" => array(
      'description' => t("5 Point Choice", array(), $t_lang),
      'group' => $group['SinChoiceQues'],
      'subquestions' => 0,
      'hasdefaultvalues' => 0,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "A" => array(
      'description' => t("Array (5 Point Choice)", array(), $t_lang),
      'group' => $group['Arrays'],
      'subquestions' => 1,
      'hasdefaultvalues' => 0,
      'assessable' => 1,
      'answerscales' => 0,
    ),
    "B" => array(
      'description' => t("Array (10 Point Choice)", array(), $t_lang),
      'group' => $group['Arrays'],
      'subquestions' => 1,
      'hasdefaultvalues' => 0,
      'assessable' => 1,
      'answerscales' => 0,
    ),
    "C" => array(
      'description' => t("Array (Yes/No/Uncertain)", array(), $t_lang),
      'group' => $group['Arrays'],
      'subquestions' => 1,
      'hasdefaultvalues' => 0,
      'assessable' => 1,
      'answerscales' => 0,
    ),
    "D" => array(
      'description' => t("Date", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 1,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "E" => array(
      'description' => t("Array (Increase/Same/Decrease)", array(), $t_lang),
      'group' => $group['Arrays'],
      'subquestions' => 1,
      'hasdefaultvalues' => 0,
      'assessable' => 1,
      'answerscales' => 0,
    ),
    "F" => array(
      'description' => t("Array", array(), $t_lang),
      'group' => $group['Arrays'],
      'subquestions' => 1,
      'hasdefaultvalues' => 0,
      'assessable' => 1,
      'answerscales' => 1,
    ),
    "G" => array(
      'description' => t("Gender", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 0,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "H" => array(
      'description' => t("Array by column", array(), $t_lang),
      'group' => $group['Arrays'],
      'hasdefaultvalues' => 0,
      'subquestions' => 1,
      'assessable' => 1,
      'answerscales' => 1,
    ),
    "I" => array(
      'description' => t("Language Switch", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'hasdefaultvalues' => 0,
      'subquestions' => 0,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "K" => array(
      'description' => t("Multiple Numerical Input", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'hasdefaultvalues' => 1,
      'subquestions' => 1,
      'assessable' => 1,
      'answerscales' => 0,
    ),
    "L" => array(
      'description' => t("List (Radio)", array(), $t_lang),
      'group' => $group['SinChoiceQues'],
      'subquestions' => 0,
      'hasdefaultvalues' => 1,
      'assessable' => 1,
      'answerscales' => 1,
    ),
    "M" => array(
      'description' => t("Multiple Options", array(), $t_lang),
      'group' => $group['MulChoiceQues'],
      'subquestions' => 1,
      'hasdefaultvalues' => 1,
      'assessable' => 1,
      'answerscales' => 0,
    ),
    "N" => array(
      'description' => t("Numerical Input", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 1,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "O" => array(
      'description' => t("List with comment", array(), $t_lang),
      'group' => $group['SinChoiceQues'],
      'subquestions' => 0,
      'hasdefaultvalues' => 1,
      'assessable' => 1,
      'answerscales' => 1,
    ),
    "P" => array(
      'description' => t("Multiple Options With Comments", array(), $t_lang),
      'group' => $group['MulChoiceQues'],
      'subquestions' => 1,
      'hasdefaultvalues' => 1,
      'assessable' => 1,
      'answerscales' => 0,
    ),
    "Q" => array(
      'description' => t("Multiple Short Text", array(), $t_lang),
      'group' => $group['TextQuestions'],
      'subquestions' => 1,
      'hasdefaultvalues' => 1,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "R" => array(
      'description' => t("Ranking", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 0,
      'assessable' => 1,
      'answerscales' => 1,
    ),
    "S" => array(
      'description' => t("Short Free Text", array(), $t_lang),
      'group' => $group['TextQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 1,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "T" => array(
      'description' => t("Long Free Text", array(), $t_lang),
      'group' => $group['TextQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 1,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "U" => array(
      'description' => t("Huge Free Text", array(), $t_lang),
      'group' => $group['TextQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 1,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "X" => array(
      'description' => t("Text display", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 0,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "Y" => array(
      'description' => t("Yes/No", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 0,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "!" => array(
      'description' => t("List (Dropdown)", array(), $t_lang),
      'group' => $group['SinChoiceQues'],
      'subquestions' => 0,
      'hasdefaultvalues' => 1,
      'assessable' => 1,
      'answerscales' => 1,
    ),
    ":" => array(
      'description' => t("Array (Numbers)", array(), $t_lang),
      'group' => $group['Arrays'],
      'subquestions' => 2,
      'hasdefaultvalues' => 0,
      'assessable' => 1,
      'answerscales' => 0,
    ),
    ";" => array(
      'description' => t("Array (Texts)", array(), $t_lang),
      'group' => $group['Arrays'],
      'subquestions' => 2,
      'hasdefaultvalues' => 0,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "|" => array(
      'description' => t("File upload", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 0,
      'assessable' => 0,
      'answerscales' => 0,
    ),
    "*" => array(
      'description' => t("Equation", array(), $t_lang),
      'group' => $group['MaskQuestions'],
      'subquestions' => 0,
      'hasdefaultvalues' => 0,
      'assessable' => 0,
      'answerscales' => 0,
    ),
  );
  asort($qtypes);
  if ($return_type == "array") {
    // switch back to the previous db.
    limesurvey_sync_db_close($previous_db);
    return $qtypes;
  }
  if ($return_type == "group") {
    foreach ($qtypes as $qkey => $qtype) {
      $newq_type[$qtype['group']][$qkey] = $qtype;
    }


    $qtypeselecter = "";
    foreach ($newq_type as $group => $members) {
      $qtypeselecter .= '<optgroup label="' . $group . '">';
      foreach ($members as $type_code => $type_properties) {
        $qtypeselecter .= "<option value='$type_code'";
        if ($selected_code == $type_code) {
          $qtypeselecter .= " selected='selected'";
        }
        $qtypeselecter .= ">{$type_properties['description']}</option>\n";
      }
      $qtypeselecter .= '</optgroup>';
    }
    // switch back to the previous db.
    limesurvey_sync_db_close($previous_db);
    return $qtypeselecter;

  }
  $qtypeselecter = "";
  foreach ($qtypes as $type_code => $type_properties) {
    $qtypeselecter .= "<option value='$type_code'";
    if ($selected_code == $type_code) {
      $qtypeselecter .= " selected='selected'";
    }
    $qtypeselecter .= ">{$type_properties['description']}</option>\n";
  }
  // switch back to the previous db.
  limesurvey_sync_db_close($previous_db);
  return $qtypeselecter;
}

/**
 * Return Return an array of subquestions for a given sid/qid.
 *
 * For example : 'YES' instead of 'Y'. Note : The limesurvey_sync_db_open()
 * function must be called before calling this function.
 *
 * @param $sid
 *   A numeric or a string. The survey ID.
 * @param $qid
 *   An integer. The question ID.
 * @param $s_language
 *   A string. Language of the subquestion text.
 *
 * @return
 *   The array of subquestions.
 */
function limesurvey_sync_answ_get_sub_questions($sid, $qid, $s_language) {
  // The LS name of this function is getSubQuestions().
  // From common_functions.php (1.92+) build 120613,
  // and from application/helpers/common_helper.php (2.05+) build 140212.
  //global $dbprefix, $connect, $clang;
  static $subquestions = array();

  if (!isset($subquestions[$sid])) {
    $subquestions[$sid] = array();
  }
  if (!isset($subquestions[$sid][$s_language])) {
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
    // Note : current_db is already 'ls' here.
    $previous_db = limesurvey_sync_db_open();
    $sid = limesurvey_sync_answ_sanitize_int($sid);
    $query = "SELECT sq.*, q.other FROM {" . limesurvey_sync_table('questions') . "} as sq, {" . limesurvey_sync_table('questions') . "} as q"
    . " WHERE sq.parent_qid=q.qid AND q.sid=:q_sid"
    . " AND sq.language=:sq_language "
    . " AND q.language=:q_language "
    . " ORDER BY sq.parent_qid, q.question_order,sq.scale_id , sq.question_order";
    $result = db_query($query, array(':q_sid' => $sid, ':sq_language' => $s_language, ':q_language' => $s_language)); //Checked

    while ($row = $result->fetchAssoc()) {
      $resultset[$row['parent_qid']][] = $row;
    }
    limesurvey_sync_db_close($previous_db);
    $subquestions[$sid][$s_language] = $resultset;
  }
  if (isset($subquestions[$sid][$s_language][$qid])) {
    return $subquestions[$sid][$s_language][$qid];
  }
  return array();
}

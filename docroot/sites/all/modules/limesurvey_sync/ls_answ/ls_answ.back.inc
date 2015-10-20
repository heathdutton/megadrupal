<?php
/**
 * @file
 * Return page from the LimeSurvey site, after answer completion.
 */

/**
 * Handle synchronization on coming back from the LimeSurvey site to Drupal.
 *
 * Try to retreive the edited answer via several method. At the end, redirect
 * to a specific page (it can be the synchronized answer node view page) or the
 * front page if it fails. Note that permission access to this page is set to
 * 'access content' : this page should not print informations without testing
 * them on rights.
 *
 * @param $type
 *   A string, the answer content type type.
 * @param $back
 *   A boolean, TRUE if the previous page was the iframe page or FALSE if it
 *   was the LimeSurvey form page. Default is FALSE.
 */
function ls_answ_back_from_ls($type, $back = FALSE) {

  // Load options.
  module_load_include('inc', 'ls_survey', 'ls_survey.type');
  $options = variable_get('ls_answ_options_' . $type, ls_survey_answ_default_options());

  // Load the associated survey node.
  module_load_include('inc', 'ls_answ', 'ls_answ');
  $node_survey = ls_answ_find_survey($type);

  // Load LS survey properties
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($node_survey->ls_sid);

  // Set default redirection.
  $end_url = (!empty($options['ls_end_url'])) ? $options['ls_end_url'] : '<front>';
  $iframes = array('iframe', 'colorbox', 'lightbox2', 'modalframe');
  // Check if auto_sync is set.
  $allowed_auto_import = ($survey_properties['date_answer'] && !$survey_properties['anonymized']);
  $editable_surveys = ($survey_properties['active'] && $survey_properties['is_available']);
  if ($options['ls_auto_import'] && $allowed_auto_import && $editable_surveys) {
    $node = FALSE;
    // Select a method to retrieve the fresh edited answer.
    if ($survey_properties['tokens_table'] && ($survey_properties['tokenanswerspersistence'] || $survey_properties['alloweditaftercompletion'])) {
      if ($survey_properties['http_referer'] && !in_array($options['ls_redirect'], $iframes) && !$back) {
        // Search datas into the http_referer value.
        // The http_referer method should work only if :
        // - "Show welcome screen?" is set to No,
        // - and "Automatically load URL when survey complete?" is set to Yes,
        // - and it is a one page survey.
        $node = ls_answ_back_referer($node_survey, $type, $options, $survey_properties);
      }
      if (!$node) {
        // Try on user informations just before editing the anwer.
        $node = ls_answ_back_user($node_survey, $type, $options, $survey_properties);
      }
    }
    if (!$node) {
      // Try to load the ls updated answer on LimeSurvey, 40s ago.
      $node = ls_answ_back_time_ago($node_survey, $type, $options, $survey_properties, 40);
    }
    // If the node has been identified, proceed to the auto sync.
    if ($node) {
      $sync_node = ls_answ_sync_node($node);
      if (isset($sync_node->ls_status) && $sync_node->ls_status == LS_STATUS_COMPLETED && $sync_node->ls_sync == LS_SYNC_SYNCHRONIZED) {
        // If we display the answers data view page :
        // Note that anonymous users can view own contribution, depending on
        // permissions setting.
        if ($options['ls_standing_by_status']) {
          // Assume that the status will be changed to 1 after synchronization.
          $sync_node->status = NODE_PUBLISHED;
        }
        if ($options['ls_end'] == 'own' && node_access('view', $sync_node)) {
          $end_url = 'node/' . $sync_node->nid;
        }
      }
    }
  }

  // Redirect to the answer node view page or a specific page or the front
  // page.
  drupal_goto($end_url);
}

/**
 * Try to retrieve the edited answer on last edited answers on LS tables.
 *
 * For datestamp surveys only. Load all anwsers edited x seconds ago.
 *
 * @param $node_survey
 *   An object. The survey node.
 * @param $type
 *   A string. The answers content type.
 * @param $options
 *   An array. The answers content type options (drupal variable).
 * @param $survey_properties
 *   An array. The LimeSurvey survey property.
 * @param $seconds_ago
 *   A numeric. The number of second ago. Default is 60seconds.
 *
 * @return
 *   The edited node or FALSE if it fails.
 */
function ls_answ_back_time_ago($node_survey, $type, $options, $survey_properties, $seconds_ago = 60) {
  // Try to retreive answers completed one min ago:
  $time = REQUEST_TIME;
  $time_ago = limesurvey_sync_fixdate_drupal_to_ls($time - $seconds_ago);
  $on['submitdate > :submitdate'] = array(':submitdate' => $time_ago);
  $order = 'submitdate DESC';
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
  $ls_answers = limesurvey_sync_answ_extract_answers($node_survey->ls_sid, $on, 0, $order);
  if ($ls_answers && count($ls_answers) == 1) {
    // An answer match ! Prepare the node.
    module_load_include('inc', 'ls_answ', 'ls_answ');
    $where['id = :id'] = array(':id' => $ls_answers[0]['ls_aid']);
    $only_new_nodes = FALSE; // Return the node if already exists.
    $nodes = ls_answ_prepare_import($node_survey->ls_sid, $type, $where, $only_new_nodes);
    if (!empty($nodes) && count($nodes) == 1) {
      $node = $nodes[0];
      if (!empty($node->nid)) {
        // The node does already exists in drupal.
        $editing_stage = ($node->ls_status == LS_STATUS_EDITING && (empty($options['ls_validuntil']) || ($time < ($node->ls_last_import + $options['ls_validuntil']))));
        if ($editing_stage) {
          return $node;
        }
      }
      else {
        // The answer should be created
        return $node;
      }
    }
  }
  return FALSE;
}

/**
 * Try to retrieve the edited answer from the http_referer.
 *
 * The last submittetd page of a LimeSurvey form should redirect to this page.
 * This function tries to synchronize the answer, retriving the answer
 * references from the http referrer.
 *
 * @param $node_survey
 *   An object. The survey node.
 * @param $type
 *   A string. The answers content type.
 * @param $options
 *   An array. The answers content type options (drupal variable).
 * @param $survey_properties
 *   An array. The LimeSurvey survey property.
 *
 * @return
 *   The edited node or FALSE if it fails.
 */
function ls_answ_back_referer($node_survey, $type, $options, $survey_properties) {
  // Search datas into the http_referer value.
  // The http_referer method should work only if :
  // - "Show welcome screen?" is set to No,
  // - and "Automatically load URL when survey complete?" is set to Yes,
  // - and it is a one page survey.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
  $extract = limesurvey_sync_answ_extract_referer();
  if ($extract && is_array($extract)) {
    // The http referer contain sensible datas !
    $ls_sid = $extract['ls_sid'];
    $ls_lang = (!empty($extract['ls_lang'])) ? $extract['ls_lang'] : $survey_properties['default_language'];

    // Check if the referer url match the request (don't auto sync
    // desynchronized ls_sid or ls_lang).
    $referer_match_type = ($ls_sid == $node_survey->ls_sid && $ls_lang == $node_survey->ls_lang);
    if ($referer_match_type && !empty($extract['ls_token'])) {
      // Token is provided : we can retrieve the answer on drupal for token
      // persistent surveys and non empty answers.
      $only_new_nodes = FALSE;
      module_load_include('inc', 'ls_answ', 'ls_answ');
      $nodes = ls_answ_prepare_import($ls_sid, $type, array("token = :token" => array(':token' => $extract['ls_token'])), $only_new_nodes);
      if (!empty($nodes) && count($nodes) == 1) {
        $node = $nodes[0];
        if (!empty($node->nid)) {
          // The node does already exists in drupal.
          $time = REQUEST_TIME;
          $editing_stage = ($node->ls_sync == LS_STATUS_EDITING && (empty($options['ls_validuntil']) || ($time < ($node->ls_last_import + $options['ls_validuntil']))));
          if ($editing_stage) {
            return $node;
          }
        }
        else {
          // The answer should be created
          return $node;
        }
      }
    }
  }
  return FALSE;
}

/**
 * Try to retrieve the edited answer using token persistence.
 *
 * For connected users only. On token answer persistence, the answer node is
 * saved just before editing. Search on thoses nodes.
 *
 * @param $node_survey
 *   An object. The survey node.
 * @param $type
 *   A string. The answers content type.
 * @param $options
 *   An array. The answers content type options (drupal variable).
 * @param $survey_properties
 *   An array. The LimeSurvey survey property.
 *
 * @return
 *   The edited node or FALSE if it fails.
 */
function ls_answ_back_user($node_survey, $type, $options, $survey_properties) {

  // Try to load the most recent editing answer.
  global $user;
  $time = REQUEST_TIME;
  $expire_date = $time - $options['ls_validuntil'];
  $answers_query = db_query("SELECT n.nid AS node_nid
  FROM {node} n
  JOIN {node_revision} r ON n.nid = r.nid AND n.vid = r.vid
  JOIN {ls_answ} lsa ON lsa.nid = r.nid AND lsa.vid = r.vid
  WHERE ls_sid = :ls_sid AND ls_lang = :ls_lang AND ls_status = :ls_status AND ls_sync = :ls_sync AND r.uid = :ruid AND ls_last_import > :ls_last_import
  ORDER BY ls_last_import DESC", array(':ls_sid' => $node_survey->ls_sid, ':ls_lang' => $node_survey->ls_lang, ':ls_status' => LS_STATUS_EDITING, ':ls_sync' => LS_SYNC_UNSYNCHRONIZED, ':ruid' => $user->uid, ':ls_last_import' => $expire_date));
  if ($answers_query->rowCount() == 1) {
    $result = $answers_query->fetchAssoc();
    return node_load($result['node_nid']);
  }
  return FALSE;
}

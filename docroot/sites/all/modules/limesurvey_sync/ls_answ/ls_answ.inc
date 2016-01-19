<?php
/**
 * @file
 * fonctions for the ls_answ module.
 */

/**
 * Return the survey node associated to the content type value provided.
 *
 * The survey node associated to the answers content type provides default
 * values for new answer nodes.
 *
 * @param $type
 *   A node object or string that indicates the node type to evaluate.
 *
 * @return
 *   An object. The survey node associated to this answers content type or
 *   FALSE if there is no associated survey.
 */
function ls_answ_find_survey($type) {
  if (empty($type)) {
    return FALSE;
  }
  $info = node_type_get_type($type);
  if ($info && $info->module == 'ls_answ') {
    module_load_include('inc', 'ls_survey', 'ls_survey');
    return ls_survey_find_survey($info->orig_type);
  }
  else {
    return FALSE;
  }
}

/**
 * Return the survey node associated to a survey ID and language provided.
 *
 * This way to find the associated survey to should only be used for answers
 * that have not already been imported (new answers).
 *
 * @param $ls_sid
 *   A numeric. The survey ID reference.
 * @param $ls_lang
 *   A string. The lang value of the survey.
 * @param $reset
 *   A boolean. to reset the cache. Default is FALSE.
 *
 * @return
 *   An object. The associated survey node or FALSE if there is no associated
 *   survey.
 */
function ls_answ_find_current_survey($ls_sid, $ls_lang = '', $reset = FALSE) {

  static $surveys = array();

  if (empty($surveys) || $reset) {
    if ($reset) {
      $surveys = array();
    }

    // Build a cache, storing all survey nid keyed by sid and lang.
    $survey_query = db_query('SELECT n.nid AS node_nid, ls_sid, ls_lang
    FROM {ls_survey} lss INNER JOIN {node} n ON n.nid = lss.nid AND n.vid = lss.vid
    ORDER BY ls_sid, ls_lang, node_nid');
    while ($survey_row = $survey_query->fetchAssoc()) {
      $nid = $survey_row['node_nid'];
      $sid = $survey_row['ls_sid'];
      $lang = $survey_row['ls_lang'];
      if (!empty($nid) && !empty($sid) && !empty($lang)) {
        $surveys[$sid][$lang] = $nid;
      }
    }
  }

  // Deal with request:
  if (empty($ls_sid)) {
    return FALSE;
  }
  elseif (isset($surveys[$ls_sid][$ls_lang])) {
    return node_load($surveys[$ls_sid][$ls_lang]);
  }
  elseif (isset($surveys[$ls_sid]) && empty($ls_lang) && count($surveys[$ls_sid]) == 1) {
    // If ls_lang is not provided and there is only one language on the survey.
    return node_load(current($surveys[$ls_sid]));
  }
  else {
    return FALSE;
  }
}

/**
 * Synchronize an answer node between the LimeSurvey database and Drupal.
 *
 * Main function of the ls_answ module. It analyses an answer node, comparing
 * it to its reference into the Drupal site and the LimeSurvey database
 * (looking for desynchronized troubles), updates it, importing the answer
 * values into Drupal (into the node body) and setting the modifyed datas :
 * ls_status, ls_sync and ls_last_import values. It returns an array resuming
 * changes make during the synchronization process.
 *
 * @param $to_sync
 *   The answer answer node object to synchronize. if the nid is not provided,
 *   the answer will be create. Note that the full node object is not needed,
 *   only the ls_field need to be filled.
 * @param $force_sync
 *   A Boolean. Force the importation. If TRUE, the import will be procceed
 *   even if it is not needed, the answers on editing stage will be reset to
 *   virgin stage, even if the expiration period is not sold out. Default is
 *   FALSE.
 * @param $force_desync
 *   A Boolean. Force the importation on desynchronized answers. Default is FALSE.
 *
 * @return
 *   A multidimensional array, listing only modifyied nodes. Primary key is
 *   'ls_status', 'ls_sync', 'not_needed' or 'not_allowed'. Secondary key is
 *   the ls_status or ls_sync modified value (available values for ls_status
 *   are LS_STATUS_VIRGIN, LS_STATUS_EDITING, LS_STATUS_COMPLETED), or
 *   'no_update' or 'no_import'. Third key is the modified nid
 *   value. Ultimate value is the node title.
 */
function ls_answ_sync_node($to_sync, $force_sync = FALSE, $force_desync = FALSE) {

  if (is_numeric($to_sync)) {
    $to_sync = node_load($to_sync);
  }
  if (empty($to_sync)) {
    return array();
  }

  // If the type is not provided and the corresponding answer does not already
  // exists : set the type to the current survey node set to the corresponding
  // ls_sid and ls_lang values if exists.
  if (empty($to_sync->type) && isset($to_sync->ls_sid, $to_sync->ls_aid, $to_sync->ls_token)) {
    // Load all existing nodes.
    $list = ls_answ_list_protected_ref($to_sync->ls_sid, $to_sync->ls_aid, $to_sync->ls_token);
    if (!$list) {
      $current_node_survey = ls_answ_find_current_survey($to_sync->ls_sid, $to_sync->ls_lang);
      if ($current_node_survey) {
        module_load_include('inc', 'ls_survey', 'ls_survey');
        $answ_type = ls_survey_find_answ_type($current_node_survey->ls_orig_type);
        if ($answ_type) {
          $to_sync->type = $answ_type->type;
          module_load_include('inc', 'node', 'node.pages');
          node_object_prepare($to_sync);
        }
      }
    }
  }

  // Check if the type is valid.
  if (empty($to_sync->type) || !ls_answ_type_is_answer($to_sync->type)) {
    return array();
  }

  if (!isset($list)) {
    $list = ls_answ_list_protected_ref($to_sync->ls_sid, $to_sync->ls_aid, $to_sync->ls_token);
  }

  // Load the associated survey if not provided, caching datas.
  static $node_survey_cached = array();
  if (isset($node_survey_cached[$to_sync->type])) {
    $node_survey = $node_survey_cached[$to_sync->type];
  }
  else {
    $node_survey = ls_answ_find_survey($to_sync->type);
    $node_survey_cached[$to_sync->type] = $node_survey;
  }

  // Load the associated options survey if not provided.
  static $options_cached = array();
  if (isset($options_cached[$to_sync->type])) {
    $options = $options_cached[$to_sync->type];
  }
  else {
    // Load file for the ls_survey_answ_default_options() function.
    module_load_include('inc', 'ls_survey', 'ls_survey.type');
    $options = variable_get('ls_answ_options_' . $to_sync->type, ls_survey_answ_default_options());
    $options_cached[$to_sync->type] = $options;
  }

  // Load the survey properties.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($to_sync->ls_sid);

  // Set default variables.
  $changes = $rules = $messages = array();
  $need_update = $create_node = $import = $publishing = FALSE;
  $watchdog_type = WATCHDOG_NOTICE;

  // Check for desynchronization, checking ordered by gravity.
  $ls_sync = ls_answ_check_sync($to_sync, $node_survey);

  $need_update = ($to_sync->ls_sync != $ls_sync || ($ls_sync == LS_SYNC_DESYNCHRONIZED_ANSWER && $to_sync->ls_status == LS_STATUS_VIRGIN));
  if ($need_update) {
    $changes = array('ls_sync' => $ls_sync);
    // Don't let the ls_status to "virgin" for a desynchronized answer,
    // arbitrarily change it to "editing".
    if ($ls_sync == LS_SYNC_DESYNCHRONIZED_ANSWER && $to_sync->ls_status == LS_STATUS_VIRGIN) {
      $changes['ls_status'] = LS_STATUS_EDITING;
    }
    $futur_to_sync = $to_sync;
    $futur_to_sync->ls_sync = $changes['ls_sync'];
    if ($ls_sync > LS_SYNC_SYNCHRONIZED_REVISION) {
      $create_node = ($force_desync && in_array($ls_sync, array(LS_SYNC_DESYNCHRONIZED_LS_LANG, LS_SYNC_DESYNCHRONIZED_LANG)));
      $rules[] = 'ls_answ_sync_desynchronized';
      $watchdog_type = WATCHDOG_ALERT;
    }
    else {
      $rules[] = 'ls_answ_sync_resynchronized';
    }
  }
  // Handle desynchronized nodes.
  if ($ls_sync > LS_SYNC_SYNCHRONIZED_REVISION) {
    // The answer is desynchronized.
    $desynchronized_mode = TRUE;
  }
  else {
    // The answer is not desynchronized.
    $desynchronized_mode = FALSE;
  }

  $not_allowed_desync_import = array(
    LS_SYNC_SYNCHRONIZED_REVISION,
    LS_SYNC_DESYNCHRONIZED_TOKEN,
    LS_SYNC_DESYNCHRONIZED_ANSWER,
    LS_SYNC_DESYNCHRONIZED_SID,
    LS_SYNC_DESYNCHRONIZED_SURVEY,
    LS_SYNC_DESYNCHRONIZED_SERVER,
  );

  // Check the import status.
  $ls_sync_final = (isset($changes['ls_sync'])) ? $changes['ls_sync'] : $to_sync->ls_sync;
  if (!in_array($ls_sync_final, $not_allowed_desync_import)) {

    $time = REQUEST_TIME;
    // If the node is being edited, calculate if the edition is over.
    if ($to_sync->ls_status == LS_STATUS_EDITING && !empty($options['ls_validuntil'])) {
      $expire_date = $to_sync->ls_last_import + $options['ls_validuntil'];
      $edition_has_expired = ($time > $expire_date);
    }
    else {
      $edition_has_expired = FALSE;
    }

    // Check if an corresponding answer exists on the LimeSurvey database.
    if (!empty($to_sync->ls_aid) || !empty($to_sync->ls_token)) {
      if (!empty($to_sync->ls_aid)) {
        $where['id = :id'] = array(':id' => $to_sync->ls_aid);
      }
      else {
        $where['token = :token'] = array(':token' => $to_sync->ls_token);
      }
      // Check the reference into the LimeSurvey answers table.
      module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
      $all_answers = limesurvey_sync_answ_extract_answers($to_sync->ls_sid, $where, 0, 'id DESC');
      if ($all_answers !== FALSE) {
        $answered = count($all_answers);
        if ($answered > 0) {
          // Load the first entry into the LS answer table.
          $row_answer = $all_answers[0];
        }
      }
      else {
        $answered = FALSE;
      }
    }
    else {
      $answered = FALSE;
    }

    // Compare references between drupal and LimeSurvey.

    if ($answered === FALSE) {
      // The answer has no references in the Drupal site. it corresponds to no
      // answer. It corresponds to : ls_aid == 0 and ls_token == ''.
      // It can be a new answer which doesn't use tokens.
      $answer_edition_expired = ($to_sync->ls_status == LS_STATUS_EDITING && ($edition_has_expired  || ((!$desynchronized_mode && $force_sync) || ($desynchronized_mode && $force_desync))));
      $virgin_answer_needs_update = ($to_sync->ls_status != LS_STATUS_VIRGIN || (!$desynchronized_mode && $to_sync->ls_sync != LS_SYNC_UNSYNCHRONIZED));
      $need_import = $answer_edition_expired && $virgin_answer_needs_update;

      if ($need_import) {
        $need_update = $publishing = TRUE;
        // Don't update the $to_sync->ls_last_import value because we wants this
        // answer to be still marked as expired.
        $changes['ls_status'] = LS_STATUS_VIRGIN;
        if (!$desynchronized_mode) {
          $changes['ls_sync'] = LS_SYNC_UNSYNCHRONIZED;
        }
        $watchdog_type = WATCHDOG_ALERT;
        // For the extract translation module.
        $extract_t = t('Virgin answer settings reseted');
        $messages[] = array(
          'message' => 'Virgin answer settings reseted',
          'arguments' => array(),
        );
      }

    }
    elseif ($answered == 0) {
      // The answer exists in the Drupal site but it does not exist in
      // LimeSurvey site : it can correspond to a virgin answer or a
      // desynchronized answer (already imported in the past).
      // Note : ls_answ_import() function doesn't provide the $to_sync->body
      // properties for memory usage raisons, but datas from this function have
      // no chance to arrive here.
      $empty_body = (empty($to_sync->body[ls_answ_body_language($to_sync)][0]['value']));
      $filled_answer = ($to_sync->ls_aid != 0 && !$empty_body);

      if ($filled_answer) {
        // The answer has specific answer id which has no reference into the
        // LimeSurevey database : the answer is desynchronized.
        // Handle above.
      }
      else {
        // ls_aid is 0 but ls_token is not empty : is it a virgin answer asked
        // for editing.
        // It can be a new answer which uses tokens.
        $answer_edition_expired = ($to_sync->ls_status == LS_STATUS_EDITING && ($edition_has_expired  || ((!$desynchronized_mode && $force_sync) || ($desynchronized_mode && $force_desync))));
        $virgin_answer = ($to_sync->ls_status != LS_STATUS_COMPLETED);
        $virgin_answer_need_update = ($virgin_answer && $to_sync->ls_status != LS_STATUS_VIRGIN || ((!$desynchronized_mode && $to_sync->ls_sync != LS_SYNC_UNSYNCHRONIZED) || ($desynchronized_mode && $force_desync)));

        $need_import = ($answer_edition_expired && $virgin_answer_need_update);
        if ($need_import) {
          $need_update = $publishing = TRUE;
          // Don't update the $to_sync->ls_last_import value because we wants this
          // answer to be still marked as expired;
          if (!$desynchronized_mode) {
            $changes['ls_sync'] = LS_SYNC_UNSYNCHRONIZED;
          }
          $changes['ls_status'] = LS_STATUS_VIRGIN;
          // Reset LimeSurvey fields.
          $changes['ls_lang'] = $node_survey->ls_lang;
          $changes['ls_aid'] = 0;
          $changes['ls_token'] = '';
          // Delete the token invitation if it exists.
          if ($survey_properties['tokens_table']) {
            limesurvey_sync_answ_delete_token($to_sync->ls_sid, $to_sync->ls_token);
          }
          // For the extract translation module.
          $extract_t = t('The answer is switched back to the virgin status');
          $messages[] = array(
            'message' => 'The answer is switched back to the virgin status',
            'arguments' => array(),
          );
        }
      }

    }
    else {
      // The LimeSurvey form has been filled !

      // check if the lang has changed :
      if ($to_sync->ls_lang != $row_answer['ls_lang']) {
        $to_sync->ls_lang = $row_answer['ls_lang'];
        // Check for desynchronized lang.
        return ls_answ_sync_node($to_sync, $force_sync, $force_desync);
      }
      // Check if it is completed or not and if it needs an update.
      $answer_completed = (!empty($row_answer['submitdate']) && $row_answer['submitdate'] != 'N');

      $answer_edition_expired = ($answer_completed || ($to_sync->ls_status == LS_STATUS_EDITING && ($edition_has_expired  || ((!$desynchronized_mode && $force_sync) || ($desynchronized_mode && $force_desync)))));
      $completed_answer_needs_update = ($answer_completed && ($to_sync->ls_status != LS_STATUS_COMPLETED || (!$desynchronized_mode && $to_sync->ls_sync != LS_SYNC_SYNCHRONIZED)));
      $answer_edition_needs_updapte = (!$answer_completed && ($to_sync->ls_status != LS_STATUS_EDITING || (!$desynchronized_mode && $to_sync->ls_sync != LS_SYNC_SYNCHRONIZED)));

      $options_have_changed = ($to_sync->ls_status != LS_STATUS_VIRGIN && $to_sync->ls_last_import < $options['ls_options_last_import'] && !$desynchronized_mode);
      $answer_edited_outsite_from_drupal = (($to_sync->ls_last_import < limesurvey_sync_fixdate_ls_to_drupal($row_answer['submitdate'])) && !$desynchronized_mode);
      $ls_field_changed = ($to_sync->ls_aid != $row_answer['ls_aid'] || $to_sync->ls_token != $row_answer['ls_token']);

      $need_import = ($answer_edition_expired && ($completed_answer_needs_update || $answer_edition_needs_updapte)) || ($ls_field_changed || $options_have_changed || $answer_edited_outsite_from_drupal || (!$desynchronized_mode && $force_sync) || ($desynchronized_mode && $force_desync));

      if ($need_import) {
        $need_update = TRUE;
        $create_node = (!$desynchronized_mode || $force_desync);
        $import = $publishing = TRUE;
        if (!$desynchronized_mode) {
          $changes['ls_sync'] = LS_SYNC_SYNCHRONIZED;
        }
        $changes['ls_status'] = (!$answer_completed) ? LS_STATUS_EDITING : LS_STATUS_COMPLETED;
        // Update the values from the ls table :
        $changes['ls_aid'] = $row_answer['ls_aid'];
        $changes['ls_lang'] = $row_answer['ls_lang'];
        $changes['ls_token'] = $row_answer['ls_token'];
        $rules[] = (!$answer_completed) ? 'ls_answ_sync_editing' : 'ls_answ_sync_completed';
        // For the extract translation module.
        $extract_t = t('The answer is being edited');
        $extract_t = t('The answer is being edited and has been imported');
        $extract_t = t('The answer has just been edited');
        $extract_t = t('The answer has just been imported');
        $messages[] = array(
          'message' => (!$answer_completed) ? ((!empty($to_sync->nid)) ? 'The answer is being edited' : 'The answer is being edited and has been imported') : ((!empty($to_sync->nid)) ? 'The answer has just been edited' : 'The answer has just been imported'),
          'arguments' => array(),
        );
      }

    }
  }

  // Ok, now proceed to the node save.
  if ($need_update) {

    $rev = (!empty($to_sync->vid)) ? $to_sync->vid : NULL;
    $node = (!empty($to_sync->nid)) ? node_load($to_sync->nid, $rev, TRUE) : FALSE;

    if ($node || $create_node) {

      // Create the node if not exist.
      if (!$node) {
        // On desynchronization mode, this situation may concern
        // desynchronized_ls_lang and desynchronized_lang when importing nodes.
        $node = (array) $to_sync;
        // Break the relationship between the 2 node objects : now we can edit
        // $node without editing $to_sync.
        $node = (object) $node;
        module_load_include('inc', 'node', 'node.pages');
        node_object_prepare($node);
      }
      else {
        // The node_load() function overwrite new values on the ls fields, we
        // import them back.
        $changed_ls_fields = array('ls_sid', 'ls_aid', 'ls_token', 'ls_lang');
        foreach ($changed_ls_fields as $ls_field) {
          $node->$ls_field = $to_sync->$ls_field;
        }
      }

      // Handle desynchronized revisions
      if ($ls_sync_final == LS_SYNC_SYNCHRONIZED_REVISION && !$options['ls_keep_revisions']) {
        if (ls_answ_clean_delete_answer($node, 'vid')) {
          $changes['ls_sync'] = LS_SYNC_DESYNCHRONIZED_ANSWER;
          $changes['ls_last_import'] = $time;
          // Don't let the ls_status to "virgin" for a desynchronized answer,
          // arbitrarily change it to "editing".
          if ($changes['ls_status'] == LS_STATUS_VIRGIN || $node->ls_sync == LS_STATUS_VIRGIN) {
            $changes['ls_status'] = LS_STATUS_EDITING;
          }
        }
      }

      // And update it.
      foreach ($changes as $field => $value) {
        $node->$field = $value;
      }

      $node->revision = $options['ls_revision'];
      // Prepare message(s).
      if ($options['ls_revision'] && !empty($messages)) {
        $logs = array();
        foreach ($messages as $message) {
          // Translation strings are declared into the $extract_t variable.
          $logs[] = t($message['message'], $message['arguments']);
        }
        $node->log = implode(' ' . t('and') . ' ', $logs);
      }

      // Saving a node revision will mark it as the current node. We prevent
      // this behavior.
      // Check if the node is the current revision:
      $current_node = (!empty($to_sync->vid)) ? db_query('SELECT 1 FROM {node} WHERE nid = :nid AND vid = :vid', array(':nid' => $to_sync->nid, ':vid' => $to_sync->vid))->fetchField() : TRUE;
      if ($current_node) {
        // Save or create the node !

        // Update node status to "unpublished", depending on survey options.
        if ($publishing && $options['ls_standing_by_status']) {
          $node->status = ($node->ls_status == LS_STATUS_COMPLETED) ? NODE_PUBLISHED : NODE_NOT_PUBLISHED;
        }

        // Import answers data form the LimeSurvey database and store it into
        // the body field as an html table.
        if ($import) {
          $answer = limesurvey_sync_answ_import_answer($node, $node->ls_aid, $options);
          $body_lang = ls_answ_body_language($node);
          $node->body = array(
            $body_lang => array(
              array(
                'format' => $options['ls_format'],
                'value' => $answer['body'],
                'summary' => text_summary($answer['body']),
                'safe_value' => $answer['body'],
                'safe_summary' => text_summary($answer['body']),
              ),
            ),
          );
          $node->ls_last_import = $time;
        }

        // Save or create the node !
        node_save($node);
      }
      else {
        // Edit the revision.
        db_update('ls_answ')
        ->fields($changes)
        ->condition('vid', $node->vid)
        ->execute();
      }

      // Watchdog the changes.
      if (!empty($messages)) {
        foreach ($messages as $message) {
          $url_node = 'node/' . $node->nid;
          $title_node = ($node->title == '') ? $url_node : $node->title;
          // Translation strings are declared into the $extract_t variable.
          watchdog('ls_answ', $message['message'], $message['arguments'], $watchdog_type, l($title_node, $url_node));
        }
      }

      // Custom events for the rules module.
      if (!empty($rules) && module_exists('rules')) {
        foreach ($rules as $rule) {
          rules_invoke_event($rule, $node);
          rules_invoke_event($rule . '--' . $node->type, $node);
        }
      }
      return $node;

    }
    else {
      return 'not_allowed';
    }
  }
  else {
    return 'not_needed';
  }
}

/**
 * Provide informations on nodes, searching by LS answer references.
 *
 * Answer references are answer ID and answer token and answer ID has priority
 * on the token value. This function retrieves the corresponding node (if it
 * exists) from sid, answer id and token, and informs if the reference is
 * shared by several nodes or types, if it is obsolet or if it is a
 * desynchronized token.
 *
 * @param $ls_sid
 *   A numeric. The survey ID.
 * @param $ls_aid
 *   A numeric. The answer ID. Set it to 0 for searching via the ls_token
 *   value.
 * @param $ls_token
 *   A string. The answer token value (optional). Default is empty.
 * @param $reset
 *   A boolean. to reset the answers protected references. Default is FALSE.
 *
 * @return
 *   A multidimentional array or FALSE if the provided references has no
 *   matching answer nodes on the Drupal site. The array primary keys are
 *   'obsolet' (boolean, corresping to an revision entry if TRUE),
 *   'desync_token' (boolean), 'n_types' (a numeric, the number of types
 *   register with this reference), 'n_nodes' (a numeric, the number of active
 *   nodes register with this reference), 'type' (a multidimentional array
 *   storing the type name, the nids values, the vids values, some ls_fields
 *   values; only actual revisions are stored for non obsolet references. See
 *   an example at the end of this function.
 */
function ls_answ_list_protected_ref($ls_sid, $ls_aid, $ls_token = '', $reset = FALSE) {

  // Cache datas.
  static $list = array();

  if (empty($list) || $reset) {
    // Create the $list vaiable.
    $list = array();
    // LEFT JOIN allows to select revisions to.
    // Don't filter by the type because a ls_aid value could be shared by more
    // than one answers content type.
    $answ_rev_query = db_query('SELECT ls_sid, ls_aid, ls_token, ls_sync, ls_last_import, ls_status, ls_lang, n.nid AS node_nid, n.vid AS node_vid, n.type AS node_type,
    nr.vid AS node_revision_vid, nr.title AS nr_title
    FROM {ls_answ} lsa
    JOIN {node_revision} nr ON lsa.vid = nr.vid
    JOIN {node} n ON nr.nid = n.nid');
    $keep = $tested = array();

    while ($answ_rev = $answ_rev_query->fetchAssoc()) {
      // A unique ls_aid entry could be both the current ls_aid or an absolet
      // ls_aid : first list the ls_aid to be tested, then proceed to the
      // evaluation.
      $current_rev = ($answ_rev['node_vid'] == $answ_rev['node_revision_vid']);

      if (!empty($answ_rev['ls_sid']) && (!empty($answ_rev['ls_aid']) || !empty($answ_rev['ls_token']))) {
        // A token is protected as long as ls_aid is not set.
        if (!empty($answ_rev['ls_token']) && empty($answ_rev['ls_aid'])) {
          $list[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['current_rev'] = $current_rev;
          $list[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['ls_sync'] = $answ_rev['ls_sync'];
          $list[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['ls_last_import'] = $answ_rev['ls_last_import'];
          $list[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['ls_status'] = $answ_rev['ls_status'];
          $list[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['ls_lang'] = $answ_rev['ls_lang'];
          $list[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['title'] = $answ_rev['nr_title'];
          if ($current_rev) {
            // Store protected values into the $keep variable.
            if (!isset($keep[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']])) {
              $keep[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']] = 0;
            }
            $keep[$answ_rev['ls_sid']]['ls_token'][$answ_rev['ls_token']]++;
          }
        }
        if (!empty($answ_rev['ls_aid'])) {
          $list[$answ_rev['ls_sid']]['ls_aid'][$answ_rev['ls_aid']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['current_rev'] = $current_rev;
          $list[$answ_rev['ls_sid']]['ls_aid'][$answ_rev['ls_aid']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['ls_sync'] = $answ_rev['ls_sync'];
          $list[$answ_rev['ls_sid']]['ls_aid'][$answ_rev['ls_aid']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['ls_last_import'] = $answ_rev['ls_last_import'];
          $list[$answ_rev['ls_sid']]['ls_aid'][$answ_rev['ls_aid']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['ls_status'] = $answ_rev['ls_status'];
          $list[$answ_rev['ls_sid']]['ls_aid'][$answ_rev['ls_aid']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['ls_lang'] = $answ_rev['ls_lang'];
          $list[$answ_rev['ls_sid']]['ls_aid'][$answ_rev['ls_aid']]['type'][$answ_rev['node_type']][(string) $answ_rev['node_nid']][(string) $answ_rev['node_revision_vid']]['title'] = $answ_rev['nr_title'];
          if ($current_rev) {
            // Store protected values into the $keep variable.
            $keep[$answ_rev['ls_sid']]['ls_aid'][$answ_rev['ls_aid']] = 1;
          }
        }
      }
    }

    // Add the keys : 'obsolet', 'n_types' and 'n_nodes', 'desync_token'.
    foreach ($list as $sid => $datas) {
      foreach ($datas as $aid_token => $data) {
        foreach ($data as $aid_token_value => $types) {
          $valid = (!empty($aid_token_value));
          if ($valid) {
            $protected = (isset($keep[$sid][$aid_token]) && array_key_exists($aid_token_value, $keep[$sid][$aid_token]));
            $obsolet = ($valid && !$protected);
            $desync_token = ($aid_token == 'ls_token' && $protected && $keep[$sid][$aid_token][$aid_token_value] > 1);
            $n_types = 0;
            foreach ($types['type'] as $type => $nids) {
              if ($n_types == 0) {
                $n_nodes = 0;
              }
              $n_types++;
              foreach ($nids as $nid => $vids) {
                $n_nodes++;
                foreach ($vids as $vid => $info) {
                  if (!$obsolet && !$info['current_rev']) {
                    // Delete the old revision, except for the obsolet
                    // references, because only revisions will be marked as
                    // DESYNCHRONIZED_REVISION; non obsolet won't be edited,
                    // so we delete them.
                    unset($list[$sid][$aid_token][$aid_token_value]['type'][$type][$nid][$vid]);
                  }
                }
              }
            }
            $add = array(
              'obsolet' => $obsolet,
              'desync_token' => $desync_token,
              'n_types' => $n_types,
              'n_nodes' => $n_nodes,
            );
            $list[$sid][$aid_token][$aid_token_value] = ls_answ_array_merge_recursive_keep_num_keys($list[$sid][$aid_token][$aid_token_value], $add);
          }
        }
      }
    }
  }

  // Deal with request :
  if ((empty($ls_sid) || !isset($list[$ls_sid])) || (empty($ls_aid) && empty($ls_token))) {
    return FALSE;
  }
  elseif (empty($ls_aid) && !empty($ls_token)) {
    return (isset($list[$ls_sid]['ls_token'][$ls_token])) ? $list[$ls_sid]['ls_token'][$ls_token] : FALSE;
  }
  elseif (!empty($ls_aid) && empty($ls_token)) {
    return (isset($list[$ls_sid]['ls_aid'][$ls_aid])) ? $list[$ls_sid]['ls_aid'][$ls_aid] : FALSE;
  }
  elseif (!empty($ls_aid) && !empty($ls_token)) {
    if (isset($list[$ls_sid]['ls_aid'][$ls_aid]) && ! isset($list[$ls_sid]['ls_token'][$ls_token])) {
      return $list[$ls_sid]['ls_aid'][$ls_aid];
    }
    elseif (!isset($list[$ls_sid]['ls_aid'][$ls_aid]) && isset($list[$ls_sid]['ls_token'][$ls_token])) {
      return $list[$ls_sid]['ls_token'][$ls_token];
    }
    elseif (isset($list[$ls_sid]['ls_aid'][$ls_aid]) && isset($list[$ls_sid]['ls_token'][$ls_token])) {
      // ls_aid has priority on ls_token.
      return $list[$ls_sid]['ls_aid'][$ls_aid];
    }
    else {
      return FALSE;
    }
  }
}

/**
 * Provide the ls_sync value.
 *
 * This function provides the ls_sync value depending on de properties ls_sid,
 * ls_aid, ls_token et the associated survey node. Note that this function
 * doesn't not check the actual ls_sync value, it will be done outside of this
 * function. Controls ordered by severity.
 *
 * @param $node
 *   The answer node object.
 * @param $node_survey
 *   The associated node object (optional). Default is NULL. Providing the
 *   survey node avoid to load it twice.
 *
 * @return
 *   A numeric, The corresponding ls_sync value. Strict comparision on the
 *   returned value is needed because this function may return 0.
 */
function ls_answ_check_sync($node, $node_survey = NULL) {
  // Check the server connection.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $all_survey_properties = limesurvey_sync_survey_properties('all');
  if ($all_survey_properties === FALSE) {
    return LS_SYNC_DESYNCHRONIZED_SERVER;
  }

  if (empty($node->ls_sid)) {
    return LS_SYNC_DESYNCHRONIZED_SID;
  }

  $survey_properties = limesurvey_sync_survey_properties($node->ls_sid);
  if (empty($survey_properties)) {
    return LS_SYNC_DESYNCHRONIZED_SURVEY;
  }
  $list = FALSE;
  if (!empty($node->ls_aid) || !empty($node->ls_token)) {
    // Check for desynchronized answer
    $on = array();
    $test_desync_answ = FALSE;
    if (!empty($node->ls_aid)) {
      $on['id = :id'] = array(':id' => $node->ls_aid);
      $test_desync_answ = TRUE;
    }
    elseif ($survey_properties['tokens_table'] && $survey_properties['tokenanswerspersistence']) {
      $on['token = :token'] = array(':token' => $node->ls_token);
      $test_desync_answ = TRUE;
    }
    if ($test_desync_answ) {
      module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
      $answers = limesurvey_sync_answ_extract_answers($node->ls_sid, $on, 4);
      if ($answers !== FALSE) {
        $n_answ = count($answers);
        if (!empty($node->ls_aid) && $n_answ == 0) {
          return LS_SYNC_DESYNCHRONIZED_ANSWER;
        }
      }
    }
    $list = ls_answ_list_protected_ref($node->ls_sid, $node->ls_aid, $node->ls_token);
    if ($list && ($list['n_types'] > 1 || !isset($list['type'][$node->type]))) {
      return LS_SYNC_DESYNCHRONIZED_TYPE;
    }
    elseif ($list && ($list['n_nodes'] > 1 || !isset($list['type'][$node->type][$node->nid]))) {
      return LS_SYNC_DESYNCHRONIZED_NODE;
    }
    elseif ($list && $list['desync_token']) {
      return LS_SYNC_DESYNCHRONIZED_TOKEN;
    }
  }

  if (empty($node->ls_lang) || empty($survey_properties['#lang'][$node->ls_lang])) {
    return LS_SYNC_DESYNCHRONIZED_LANG;
  }
  if (empty($node_survey)) {
    $node_survey = ls_answ_find_survey($node->type);
  }
  if ($node_survey->ls_sid != $node->ls_sid) {
    return LS_SYNC_DESYNCHRONIZED_LS_SID;
  }
  if ($node_survey->ls_lang != $node->ls_lang) {
    return LS_SYNC_DESYNCHRONIZED_LS_LANG;
  }

  // The node is not desynchronized.
  if ($list && $list['obsolet']) {
    return LS_SYNC_SYNCHRONIZED_REVISION;
  }

  return ($node->ls_status == LS_STATUS_COMPLETED) ? LS_SYNC_SYNCHRONIZED : LS_SYNC_UNSYNCHRONIZED;
}

/**
 * Prepare importation from LimeSurvey, creating answers nodes to synchronize.
 *
 * This function load first all answers from the LimeSurvey database for a
 * specific survey ID and retrieve the one that are alredy save. it returns an
 * array of nodes corresponding to each entry on the LimeSurvey database.
 *
 * @param $ls_sid
 *   A numeric. The survey ID reference.
 * @param $type_provided
 *   A string. The answer content type that should be associate to the answer
 *   entry. If it not provided and the answer does not already exist, the
 *   current associated survey will be used.
 * @param $where
 *   A array. The where clause of the sql query on the LimeSurvey table. It
 *   should be let empty.
 * @param $only_new_nodes
 *   A boolean. If TRUE, only return node to be created (not the existing one).
 *   Default is TRUE.
 * @param $include_desync_rev
 *   A boolean. If TRUE, only return the desynchronized revisions. Default is
 *   FALSE.
 *
 * @return
 *   A numerically indexed array. Each value is the node object corresponding
 *   to an entry on the LimeSurvey answer table.
 */
function ls_answ_prepare_import($ls_sid, $type_provided = '', $where = array(), $only_new_nodes = TRUE, $include_desync_rev = FALSE) {

  static $import_nodes = array();
  if (!empty($import_nodes)) {
    return $import_nodes;
  }

  // Collect answers from the LimeSurvey database.
  // Load file for the limesurvey_sync_answ_extract_answers()
  // function.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
  $ls_answers = limesurvey_sync_answ_extract_answers($ls_sid, $where);
  if ($ls_answers === FALSE) {
    return;
  }
  // Manage batch datas.

  // Iterate answers from the LimeSurvey database.
  foreach ($ls_answers as $row_answer) {
    $node = array();
    $node['ls_sid'] = $ls_sid;
    $node['ls_aid'] = $row_answer['ls_aid'];
    $node['ls_token'] = $row_answer['ls_token'];
    $node['ls_lang'] = $row_answer['ls_lang'];
    if (!empty($type_provided)) {
      $node['type'] = $type_provided;
    }

    // Load all existing types and nodes for this references.
    $list = ls_answ_list_protected_ref($node['ls_sid'], $node['ls_aid'], $node['ls_token']);
    // Try to retrieve an existing node :
    if ($list) {
      // This entry does already exist into the drupal database, we are going
      // to update all associated nodes.
      if (!$only_new_nodes || $include_desync_rev) {
        foreach ($list['type'] as $type => $nids) {
          foreach ($nids as $nid => $vids) {
            foreach ($vids as $vid => $infos) {
              // Don't import the new desynchronized type for performance
              // raisons, as long has we have set "don't create node" on
              // desynchronized types in the ls_answ_sync_node(() function.
              if (empty($type_provided) || isset($list['type'][$type_provided])) {
                $node['nid'] = $nid;
                $node['vid'] = $vid;
                $node['ls_sync'] = $infos['ls_sync'];
                $node['ls_last_import'] = $infos['ls_last_import'];
                $node['ls_status'] = $infos['ls_status'];
                $node['ls_lang'] = $infos['ls_lang'];
                $node['title'] = $infos['title'];
                if ((!$only_new_nodes && !$list['obsolet']) || ($include_desync_rev && $list['obsolet'])) {
                  $import_nodes[] = (object) $node;
                }
              }
            }
          }
        }
      }
    }
    else {
      // This entry does not exist into the drupal database, we are going to
      // import it.

      // If the type is not provided and the corresponding answer does not already
      // exists : set the type to the current survey node set to the corresponding
      // ls_sid and ls_lang values if exists.
      if (empty($type_provided)) {
        $current_node_survey = ls_answ_find_current_survey($node['ls_sid'], $node['ls_lang']);
        if ($current_node_survey) {
          module_load_include('inc', 'ls_survey', 'ls_survey');
          $answ_type = ls_survey_find_answ_type($current_node_survey->ls_orig_type);
          if ($answ_type) {
            $node['type'] = $answ_type->type;
          }
        }
      }

      if (!empty($node['type'])) {
        // We ask for edition.
        $time = REQUEST_TIME;
        $node['ls_status'] = LS_STATUS_EDITING;
        // Manage the author node. Default is anonymous.
        module_load_include('inc', 'ls_survey', 'ls_survey.type');
        $options = variable_get('ls_answ_options_' . $node['type'], ls_survey_answ_default_options());
        global $user;
        $node['uid'] = (is_numeric($options['ls_author'])) ? $options['ls_author'] : $user->uid;
        $node['ls_last_import'] = $time;
        $node['ls_sync'] = LS_SYNC_UNSYNCHRONIZED;
        // Create the node object :
        $node_object = (object) $node;
        // Handle the language setting.
        if (module_exists('locale') && variable_get('language_content_type_' . $node['type'], 0)) {
          // Set the node language.
          // Make sure this language is enabled.
          // Set the node->body to NULL
          $node->body = NULL;
          $node_language = ls_answ_validlang_dp_nodelanguage($node);
          if (!empty($node_language)) {
            $node->language = $node_language;
          }
        }

        $import_nodes[] = $node_object;
      }
    }
  } // End of the while loop of the selected LimeSurvey answer references.

  return $import_nodes;
}

/**
 * Import or update answer nodes from the LimeSurvey database.
 *
 * This function is independent or can be used by the batch API.
 *
 * @param $ls_sid
 *   A numeric. The survey ID reference.
 * @param $type
 *   A string. The answer content type that should be associate to the answer
 *   entry. If it not provided and the answer does not already exist, the
 *   current associated survey will be used.
 * @param $where
 *   A array. The where clause of the sql query on the LimeSurvey table. It
 *   should be let empty.
 * @param $limit
 *   A numeric. The number of nodes to create. On batch operations it will be
 *   overwriten. Default is 0, corresponding to all answers stored on the
 *   LimeSurvey table.
 * @param $batch
 *   A Boolean. Set to TRUE when using this function on batch operations.
 *   Default is FALSE.
 * @param $only_new_nodes
 *   A Boolean. Import only new answers. Default is TRUE.
 * @param $include_desync_rev
 *   A Boolean. Include desynchronized revisions. Default is FALSE.
 * @param $force_sync
 *   A Boolean. Force the importation. If TRUE, the import will be procceed
 *   even if it is not needed, the answers on editing stage will be reset to
 *   virgin stage, even if the expiration period is not sold out. Default is
 *   FALSE.
 * @param $force_desync
 *   A Boolean. Force the importation on desynchronized answers. Default is FALSE.
 * @param $verbose
 *   A Boolean. If TRUE, print updated or imported answers. Note that it will
 *   only used during the finished batch. Default is FALSE.
 * @param $context
 *   An array. Used for batch operations.
 *
 * @return
 *   A multidimensional array storing the synchronization results. The primary
 *   keys are 'import' or 'update', the secondary key is a string,
 *   concatenating several datas as 'old status' . ';' . 'new_statut' . '_' .
 *   'old sync status' . ';' . 'new sync status'. Third level is a numeric
 *   array providing the corresponding nid an title.
 */
function ls_answ_import($ls_sid, $type = '', $where = array(), $limit = 0, $batch = FALSE, $only_new_nodes = TRUE, $include_desync_rev = FALSE, $force_sync = FALSE, $force_desync = FALSE, $verbose = FALSE, &$context = array()) {

  // Collect answers from the LimeSurvey database.
  $nodes = ls_answ_prepare_import($ls_sid, $type, $where, $only_new_nodes, $include_desync_rev);
  if (empty($nodes)) {
    if ($batch) {
      $context['finished'] = 1;
      $context['results']['ls_sync'] = array();
      $context['results']['ls_verbose'] = $verbose;
    }
    return;
  }
  $count_answ = count($nodes);

  // Manage batch datas.
  if ($batch) {
    // We decide that we can safely process 50 nodes at a time without a
    // timeout.
    $limit = 50;
    if (!isset($context['sandbox']['progress'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['current_node'] = 0;
      $context['sandbox']['max'] = $count_answ - 1;
      $context['sandbox']['ls_sync'] = array();
    }
    // Handle batch stuff.
    $key = $context['sandbox']['current_node'];
    $ls_sync = $context['sandbox']['ls_sync'];
  }
  else {
    $key = 0;
    $ls_sync = array();
    if ($limit == 0) {
      $limit = $count_answ;
    }
    // For no batch feature : load from newest to oldest.
    $nodes = array_reverse($nodes);
  }

  // Iterate answers from the LimeSurvey database.
  for ($i = 0; $i < $limit; $i++) {
    if (isset($nodes[$key])) {
      // Import the answer !
      $sync_node = ls_answ_sync_node($nodes[$key], $force_sync, $force_desync);
      $ls_sync = ls_answ_sync_results($nodes[$key], $sync_node, $ls_sync);
    }
    $key++;

    if ($batch) {
      // Update our progress information.
      $context['sandbox']['progress']++;
      $context['sandbox']['current_node'] = $key;
      $context['sandbox']['ls_sync'] = $ls_sync;
      if (!empty($nodes[$key]->title)) {
        $context['message'] = t('Now processing %node', array('%node' => $nodes[$key]->title));
      }
    }
  } // End of the while loop of the selected LimeSurvey answer references.

  if ($batch) {
    $context['results']['ls_sync'] = $ls_sync;
    $context['results']['ls_verbose'] = $verbose;
    // Inform the batch engine that we are not finished, and provide an
    // estimation of the completion level we reached.
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
    else {
      $context['finished'] = 1;
    }
  }
  else {
    return $ls_sync;
  }
}

/**
 * Batch operation callback, synchronizing several answer nodes from LimeSurvey.
 *
 * @param $ls_sid
 *   A numeric. The survey ID reference.
 * @param $type
 *   A string. The answer content type that should be associate to the answer
 *   entry. If it not provided and the answer does not already exist, the
 *   current associated survey will be used.
 * @param $where
 *   A array. The where clause of the sql query on the LimeSurvey table. It
 *   should be let empty.
 * @param $only_new_nodes
 *   A Boolean. Import only new answers. Default is TRUE.
 * @param $include_desync_rev
 *   A Boolean. Include desynchronized revisions. Default is FALSE.
 * @param $force_sync
 *   A Boolean. Force the synchronization. Default is FALSE.
 * @param $force_desync
 *   A Boolean. Force the importation on desynchronized answers. Default is FALSE.
 * @param $verbose
 *   A Boolean. If TRUE, print updated or imported answers. Note that it will
 *   only used during the finished batch. Default is FALSE.
 */
function ls_answ_batch_import($ls_sid, $type = '', $where = array(), $only_new_nodes = TRUE, $include_desync_rev = FALSE, $force_sync = FALSE, $force_desync = FALSE, $verbose = FALSE) {
  $operations = array();
  $operations[] = array('ls_answ_import', array($ls_sid, $type, $where, 0, TRUE, $only_new_nodes, $include_desync_rev, $force_sync, $force_desync, $verbose));
  $batch = array(
    'operations' => $operations,
    'finished' => 'ls_answ_batch_finished',
    'title' => t('Processing answers importation'),
    'init_message' => t('Answers importation is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Answers importation has encountered an error.'),
    'file' => drupal_get_path('module', 'ls_answ') . '/ls_answ.inc',
  );
  batch_set($batch);
}

/**
 * Synchronize nodes between the LimeSurvey database and Drupal.
 *
 * This function can be used in a simple way or through a bath operation.
 *
 * @param $where
 *   A array. The where clause of the sql query selecting answer nodes to be
 *   synchronized. Keys are the field value concatenate to its reference into
 *   the database layer system (in a nutshell : "n.uid = :n.uid") and the
 *   values are the correspondind field value to filter with. Default is empty,
 *   corresponding to all answer nodes the current user is allowed to access.
 * @param $limit
 *   A numeric. the number of nodes to create. On batch operations it will be
 *   overwriten. Default is 0, corresponding to all answers stored on the
 *   Drupal database.
 * @param $batch
 *   A Boolean. Format the query for batch operations. Default is FALSE.
 * @param $force_sync
 *   A Boolean. Force the synchronization. Default is FALSE.
 * @param $force_desync
 *   A Boolean. Force the importation on desynchronized answers. Default is FALSE.
 * @param $access_bypass
 *   A Boolean. Select answer nodes including non allowed nodes for the current
 *   user, used for mass updates, like "synchronize all answers from a survey".
 *   Default is FALSE.
 * @param $verbose
 *   A Boolean. If TRUE, print updated or imported answers. Note that it will
 *   only used during the finished batch. Default is FALSE.
 * @param $context
 *   An array. Reference for batch operations. Default is empty.
 *
 * @return
 *   A multidimensional array storing the synchronization results. The primary
 *   keys are 'import' or 'update', the secondary key is a string,
 *   concatenating several datas as 'old status' . ';' . 'new_statut' . '_' .
 *   'old sync status' . ';' . 'new sync status'. Third level is a numeric
 *   array providing the corresponding nid an title.
 */
function ls_answ_sync($where = array(), $limit = 0, $batch = FALSE, $force_sync = FALSE, $force_desync = FALSE, $access_bypass = FALSE, $verbose = FALSE, &$context = array()) {

  // Select nodes to be synchronized.
  $query = db_select('node', 'n')->fields('n', array('nid', 'vid', 'type', 'title', 'changed'));
  $query->join('node_revision', 'r', 'n.nid = r.nid AND n.vid = r.vid');
  $query->join('ls_answ', 'lsa', 'lsa.nid = r.nid AND lsa.vid = r.vid');
  $query->fields('lsa', array('ls_status', 'ls_sync', 'ls_last_import', 'ls_sid', 'ls_lang', 'ls_aid', 'ls_token'));
  if (!$access_bypass) {
    $query->addTag('node_access');
  }

  // Handle specific conditions.
  foreach ($where as $field => $select) {
    $query->condition($field, $select);
  }

  $count_answ = $query->countQuery()->execute()->fetchField();

  // Manage batch datas.

  if ($batch) {
    // We decide that we can safely process 5 nodes at a time without a timeout.
    $limit = 50;
    if (!isset($context['sandbox']['progress'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['current_node'] = 0;
      $context['sandbox']['max'] = $count_answ;
      $context['sandbox']['ls_sync'] = array();
    }
    // Handle batch stuff.
    $ls_sync = $context['sandbox']['ls_sync'];
    $query
      ->condition('n.nid', $context['sandbox']['current_node'], '>')
      ->orderBy('n.nid', 'ASC')
      ->range(0, $limit);

  }
  else {
    $ls_sync = array();
    if ($limit != 0) {
      // Order by the most recent updated nodes.
      $query->range(0, $limit)
      ->orderBy('ls_last_import', 'DESC');
    }
  }

  $result = $query->execute();

  $ls_sync_node = array();
  // Iterate each answer node to test.
  while ($to_sync = $result->fetchObject()) {

    // Synchronize the answer !
    $sync_node = ls_answ_sync_node($to_sync, $force_sync, $force_desync);
    $ls_sync = ls_answ_sync_results($to_sync, $sync_node, $ls_sync);
    // Store some result for post-processing in the finished callback.
    $context['results'][] = check_plain($to_sync->title);

    if ($batch) {
      // Update our progress information.
      $context['sandbox']['progress']++;
      $context['sandbox']['current_node'] = $to_sync->nid;
      $context['sandbox']['ls_sync'] = $ls_sync;
      if (!$access_bypass && !empty($nodes[$key]->title)) {
        $context['message'] = t('Now processing %node', array('%node' => $to_sync->title));
      }
    }

  } // End of the while loop of the selected drupal answer references.

  if ($batch) {
    $context['results']['ls_sync'] = $ls_sync;
    $context['results']['ls_verbose'] = $verbose;
    // Inform the batch engine that we are not finished, and provide an
    // estimation of the completion level we reached.
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
    else {
      $context['finished'] = 1;
    }
  }
  else {
    return $ls_sync;
  }
}

/**
 * Batch operation callback, synchronizing several answer nodes from Drupal.
 *
 * @param $where
 *   A array. The where clause of the sql query selecting answer nodes to be
 *   synchronized. Keys are the field value concatenate to its reference into
 *   the database layer system (in a nutshell : "n.uid = :n.uid") and the
 *   values are the correspondind field value to filter with. Default is empty,
 *   corresponding to all answer nodes the current user is allowed to access.
 * @param $force_sync
 *   A Boolean. Force the synchronization. Default is FALSE.
 * @param $force_desync
 *   A Boolean. Force the importation on desynchronized answers. Default is FALSE.
 * @param $access_bypass
 *   A Boolean. Select answer nodes including non allowed nodes for the current
 *   user, used for mass updates, like "synchronize all answers from a survey".
 *   Default is FALSE.
 * @param $verbose
 *   A Boolean. If TRUE, print updated or imported answers. Note that it will
 *   only used during the finished batch. Default is FALSE.
 */
function ls_answ_batch_sync($where = array(), $force_sync = FALSE, $force_desync = FALSE, $access_bypass = FALSE, $verbose = FALSE) {
  $operations = array();
  $operations[] = array('ls_answ_sync', array($where, 0, TRUE, $force_sync, $force_desync, $access_bypass, $verbose));
  $batch = array(
    'operations' => $operations,
    'finished' => 'ls_answ_batch_finished',
    'title' => t('Processing answers synchronization'),
    'init_message' => t('Answers synchronization is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Answers synchronization has encountered an error.'),
    'file' => drupal_get_path('module', 'ls_answ') . '/ls_answ.inc',
  );
  batch_set($batch);
}

/**
 * Batch 'finished' callback
 */
function ls_answ_batch_finished($success, $results, $operations) {
  if ($success) {
    // Because we have used the access bypass, don't explicit how many
    // nodes have been updated.
    $message = ls_answ_display_sync_results($results['ls_sync'], $results['ls_verbose']);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred');
  }
  drupal_set_message($message);
}

/**
 * Merge synchronization results of multiple answer nodes.
 *
 * @param $node
 *   The answer node object before the synchronization. Note that the variable
 *   may not store all node properties (body, changes, etc...).
 * @param $sync_node
 *   The answer $node object or a string ('not_allowed' or 'not_needed') if the
 *   synchronization fails.
 * @param $ls_sync
 *   A multidimensional array storing previous results of this function. The
 *   actual result is going to be merged to the provided results. Default is
 *   an empty array.
 *
 * @return
 *   A multidimensional array storing the synchronization results. The primary
 *   keys are 'import' or 'update', the secondary key is a string,
 *   concatenating several datas as 'old status' . ';' . 'new_statut' . '_' .
 *   'old sync status' . ';' . 'new sync status'. Third level is a numeric
 *   array providing the corresponding nid and title.
 */
function ls_answ_sync_results($node, $sync_node, $ls_sync = array()) {
  $import_update = (empty($node->nid)) ? 'import' : 'update';
  $nid = (!empty($sync_node->nid)) ? $sync_node->nid : ((!empty($node->nid)) ? $node->nid : '');
  $vid = (!empty($sync_node->vid)) ? $sync_node->vid : ((!empty($node->vid)) ? $node->vid : '');
  $ls_aid = (!empty($sync_node->ls_aid)) ? $sync_node->ls_aid : ((!empty($node->ls_aid)) ? $node->ls_aid : '');
  $title = (!empty($sync_node->title)) ? $sync_node->title : ((!empty($node->title)) ? $node->title : '');
  $no_changes = array(
    'not_needed',
    'not_allowed',
  );

  if (in_array($sync_node, $no_changes)) {
    $ls_sync[$import_update][$sync_node][] = array(
      'nid' => $nid,
      'title' => $title,
      'vid' => $vid,
      'ls_aid' => $ls_aid,
    );
  }
  else {
    if ($import_update == 'import') {
      $change_status = $sync_node->ls_status;
      $change_sync = $sync_node->ls_sync;
    }
    else {
      $change_status = ($sync_node->ls_status != $node->ls_status) ? $node->ls_status . ';' . $sync_node->ls_status : '';
      $change_sync = ($sync_node->ls_sync != $node->ls_sync) ? $node->ls_sync . ';' . $sync_node->ls_sync : '';
    }

    $ls_sync[$import_update][$change_status . '_' . $change_sync][] = array(
      'nid' => $nid,
      'title' => $title,
      'vid' => $vid,
      'ls_aid' => $ls_aid,
    );
  }
  return $ls_sync;
}

/**
 * Display synchronization results.
 *
 * @param $ls_sync
 *   A multimentional array, formated by the ls_answ_sync_results() function.
 * @param $verbose
 *   A boolean. List all tested answer nodes as link if TRUE. Default is FALSE.
 * @param $send_message
 *   A boolean. Send message if TRUE. Default is FALSE.
 *
 * @return
 *   An html indented list of result.
 */
function ls_answ_display_sync_results($ls_sync, $verbose = FALSE, $send_message = FALSE) {

  if (empty($ls_sync)) {
    return t('No answer to synchronize.');
  }

  $n_answers = 0;
  $no_changes = array(
    'not_needed',
    'not_allowed',
  );
  $type_message = 'status';
  $display = $current_vids = array();
  $i = 0;
  foreach ($ls_sync as $import_update => $array_changes) {
    $n_import_update = 0;
    if (!isset($display[$i])) {
      $display[$i] = array();
    }
    ksort($array_changes);
    $j = 0;
    foreach ($array_changes as $change => $datas) {
      if (!isset($display[$i]['children'][$j])) {
        $display[$i]['children'][$j] = array();
      }
      $n_changed = count($datas);
      $n_import_update += $n_changed;
      if (in_array($change, $no_changes)) {
        if ($change == 'not_needed') {
          if ($import_update == 'import') {
            $output = format_plural(
              $n_changed,
              '1 answer does not need to be imported',
              "%count answers don't need to be imported",
              array('%count' => $n_changed)
            );
            $output_one_answ = t('The answer does not need to be imported');
            $display[$i]['children'][$j] = array_merge($display[$i]['children'][$j], array('data' => $output));
          }
          else { // The $import_update value is 'update'.
            $output = format_plural(
              $n_changed,
              '1 answer does not need to be updated',
              "%count answers don't need to be updated",
              array('%count' => $n_changed)
            );
            $output_one_answ = t('The answer does not need to be updated');
            $display[$i]['children'][$j] = array_merge($display[$i]['children'][$j], array('data' => $output));
          }
        }
        elseif ($change == 'not_allowed') {
          $type_message = 'warning';
          if ($import_update == 'import') {
            $output = format_plural(
              $n_changed,
              '1 answer is not allowed to be imported',
              '%count answers are not allowed to be imported',
              array('%count' => $n_changed)
            );
            $output_one_answ = t('The answer is not allowed to be imported');
            $display[$i]['children'][$j] = array_merge($display[$i]['children'][$j], array('data' => $output));
          }
          else {
            $output = format_plural(
              $n_changed,
              '1 answer is not allowed to be updated',
              '%count answers are not allowed to be updated',
              array('%count' => $n_changed)
            );
            $output_one_answ = t('The answer is not allowed to be updated');
            $display[$i]['children'][$j] = array_merge($display[$i]['children'][$j], array('data' => $output));
          }
        }
      }
      else {
        // Retreive ls_sync and ls_field values.
        $ls_field = explode('_', $change);
        $ls_status = $ls_field[0];
        $ls_sync = $ls_field[1];
        if ($import_update == 'import') {
          $correct_status = ($ls_status == LS_STATUS_COMPLETED);
          $correct_sync = ($ls_sync == LS_SYNC_SYNCHRONIZED);
          if ($correct_status && $correct_sync) {
            $output = format_plural(
              $n_changed,
              '1 answer has been correctly imported',
              '%count answers have been correctly imported',
              array('%count' => $n_changed)
            );
            $output_one_answ = t('The answer has been correctly imported');
            $display[$i]['children'][$j] = array_merge($display[$i]['children'][$j], array('data' => $output));
          }
          else {
            if ($ls_sync > LS_SYNC_SYNCHRONIZED_REVISION) {
              $type_message = 'warning';
            }
            $ouput = format_plural(
              $n_changed,
              'On 1 imported answer,',
              'On %count imported answers,',
              array('%count' => $n_changed)
            );
            $ls_status_ouput = ($correct_status) ? '' : t("the @field '%value' has been set", array('@field' => drupal_strtolower(ls_answ_display_sync_headers('ls_status')), '%value' => drupal_strtolower(ls_answ_display_sync_fields('ls_status', $ls_status))));
            $ls_sync_ouput = ($correct_sync) ? '' : t("the @field '%value' has been set", array('@field' => drupal_strtolower(ls_answ_display_sync_headers('ls_sync')), '%value' => drupal_strtolower(ls_answ_display_sync_fields('ls_sync', $ls_sync))));
            $and = (!empty($ls_status_ouput) && !empty($ls_sync_ouput)) ? ' ' . t('and') . ' ' : '';
            $ouput .= ' ' . $ls_status_ouput . $and . $ls_sync_ouput;
            $display[$i]['children'][$j] = array_merge($display[$i]['children'][$j], array('data' => $ouput));
            $output_one_answ = t('Importing the answer,') . ' ' . $ls_status_ouput . $and . $ls_sync_ouput;
          }

        }
        else { // The $import_update value is 'update'.
          $no_change_status = ($ls_status === '');
          $no_change_sync = ($ls_sync === '');
          if ($no_change_status && $no_change_sync) {
            $display[$i]['children'][$j] = array_merge($display[$i]['children'][$j], array('data' => format_plural(
              $n_changed,
              '1 answer has been updated with no change on the @status and the @sync',
              '%count answers have been updated with no change on the @status and the @sync',
              array('%count' => $n_changed, '@status' => drupal_strtolower(ls_answ_display_sync_headers('ls_status')), '@sync' => drupal_strtolower(ls_answ_display_sync_headers('ls_sync')))
            )));
            $output_one_answ = t('The answer has been updated with no change on the @status and the @sync', array('@status' => drupal_strtolower(ls_answ_display_sync_headers('ls_status')), '@sync' => drupal_strtolower(ls_answ_display_sync_headers('ls_sync'))));
          }
          else {
            // Retreive old and new ls_sync and ls_field values.
            $changes_status = ($no_change_status) ? FALSE : explode(';', $ls_status);
            $changes_sync = ($no_change_sync) ? FALSE : explode(';', $ls_sync);
            if ($changes_sync && $changes_sync[1] > LS_SYNC_SYNCHRONIZED_REVISION) {
              $type_message = 'warning';
            }
            $ouput = format_plural(
              $n_changed,
              'On 1 updated answer,',
              'On %count updated answers,',
              array('%count' => $n_changed)
            );
            $ls_status_ouput = (!$changes_status) ? '' : t("the @field has been changed from '%old' to '%new'", array('@field' => drupal_strtolower(ls_answ_display_sync_headers('ls_status')), '%old' => drupal_strtolower(ls_answ_display_sync_fields('ls_status', $changes_status[0])), '%new' => drupal_strtolower(ls_answ_display_sync_fields('ls_status', $changes_status[1]))));
            $ls_sync_ouput = (!$changes_sync) ? '' : t("the @field has been changed from '%old' to '%new'", array('@field' => drupal_strtolower(ls_answ_display_sync_headers('ls_sync')), '%old' => drupal_strtolower(ls_answ_display_sync_fields('ls_sync', $changes_sync[0])), '%new' => drupal_strtolower(ls_answ_display_sync_fields('ls_sync', $changes_sync[1]))));
            $and = (!empty($ls_status_ouput) && !empty($ls_sync_ouput)) ? ' ' . t('and') . ' ' : '';
            $ouput .= ' ' . $ls_status_ouput . $and . $ls_sync_ouput;
            $display[$i]['children'][$j] = array_merge($display[$i]['children'][$j], array('data' => $ouput));
            $output_one_answ = t('Updating the answer,') . ' ' . $ls_status_ouput . $and . $ls_sync_ouput;
          }
        }
      }

      // Count the answers :
      $n_answers += count($datas);
      $output_one_answ_answ = '';

      if ($verbose) {
        // Print the nodes.
        $links = $link_entries = array();
        foreach ($datas as $answer) {
          $link = '';
          if (!empty($answer['nid'])) {
            // Save current revision vid into the variable $current_vids.
            if (!isset($current_vids[$answer['nid']])) {
              $node_answer = node_load($answer['nid'], NULL, TRUE);
              if ($node_answer) {
                $current_vids[$answer['nid']]['vid'] = $node_answer->vid;
                $access = node_access('view', $node_answer);
                $current_vids[$answer['nid']]['title'] = ($access) ? l($node_answer->title, 'node/' . $node_answer->nid) : t('access not allowed');
              }
            }
            $revision = (!empty($answer['vid']) && $current_vids[$answer['nid']]['vid'] != $answer['vid']);
            $url = (!$revision) ? ('node/' . $answer['nid']) : ('node/' . $answer['nid'] . '/revision/' . $answer['vid'] . '/view');
            $link = ($access) ? l($answer['title'], $url) : t('access not allowed');
          }
          if (!empty($link)) {
            $links[$answer['nid']][$answer['vid']] = $link;
          }
          elseif (!empty($answer['ls_aid'])) {
            // Nid is empty, corresponding to an answer ID not imported, as
            // examples : because it correpond to an uncompleted answer or a
            // desynchronized language.
            $link_entries[$answer['ls_aid']] = t('Concerning the answer entry %aid.', array('%aid' => $answer['ls_aid']));
          }
        }

        // Sort and display answers nodes, revisions and answer entries.
        if (!empty($links)) {
          // Sort and display answers nodes, revisions.
          ksort($links);
          $k = 0;
          foreach ($links as $nid => $answer_vids) {
            if (!empty($answer_vids)) {
              ksort($answer_vids);
              if (!isset($display[$i]['children'][$j]['children'][$k])) {
                $display[$i]['children'][$j]['children'][$k] = array();
              }
              // If the current node is in the list, set it now to make it
              // appear on the top.
              $current_vid = $current_vids[$nid]['vid'];
              if (isset($answer_vids[$current_vid])) {
                // This is the current node version.
                $display[$i]['children'][$j]['children'][$k] = array_merge($display[$i]['children'][$j]['children'][$k], array('data' => $answer_vids[$current_vid]));
                $output_one_answ_answ = $answer_vids[$current_vid];
                // Delete the current vid entry to avoid dooblon.
                unset($answer_vids[$current_vid]);
              }

              // If the list contains revisions, set them now.
              $n_revisions = count($answer_vids);
              if ($n_revisions > 0) {
                $revision_title = format_plural($n_revisions,
                  'one revision from the current node !title :',
                  '@count revisions from the current node !title :',
                  array('!title' => $current_vids[$nid]['title'])
                );
                $display[$i]['children'][$j]['children'][$k] = array_merge($display[$i]['children'][$j]['children'][$k], array('data' => $revision_title));
                $output_one_answ_answ = t('The revision from the current node !title :', array('!title' => $current_vids[$nid]['title']));
              }

              $l = 0;
              foreach ($answer_vids as $vid => $answer_link) {
                // This is an old node version.
                if (!isset($display[$i]['children'][$j]['children'][$k]['children'][$l])) {
                  $display[$i]['children'][$j]['children'][$k]['children'][$l] = array();
                }
                $display[$i]['children'][$j]['children'][$k]['children'][$l] = array_merge($display[$i]['children'][$j]['children'][$k]['children'][$l], array('data' => t('!answer (rev %rev)', array('!answer' => $answer_link, '%rev' => $vid))));
                $output_one_answ_answ .= ' ' . t('!answer (rev %rev)', array('!answer' => $answer_link, '%rev' => $vid));
                $l++;
              }
            }
            $k++;
          }
        }
        elseif (!empty($link_entries)) {
          // Display answer entries.
          ksort($link_entries);
          $display[$i]['children'][$j]['children'] = $link_entries;
          reset($link_entries);
          $output_one_answ_answ = current($link_entries);
        }
      }

      $j++;

    }
    if ($import_update == 'import') {
      $display[$i] = array_merge($display[$i], array('data' => format_plural(
        $n_import_update,
        'On 1 answer to be imported :',
        'On %count answers to be imported :',
        array('%count' => $n_import_update)
      )));
    }
    else { // The $import_update value is 'update'.
      $display[$i] = array_merge($display[$i], array('data' => format_plural(
        $n_import_update,
        'On 1 answer to be updated :',
        'On %count answers to be updated',
        array('%count' => $n_import_update)
      )));
    }

    $i++;
  }

  if ($n_answers > 1) {
    $message = (!empty($display)) ? theme('item_list', array('items' => $display)) : FALSE;
  }
  else {
    // If there is only one answer, don't display a list.
    $message = $output_one_answ . ((!empty($output_one_answ_answ)) ? ' (' . $output_one_answ_answ . ')' : '');
  }
  if ($send_message && $message) {
    drupal_set_message(filter_xss($message), $type_message);
  }

  return $message;
}

/**
 * Delete answer entries on the LimeSurvey table, checking for protected entries.
 *
 * This function check if the answer entry is not protected before deleting it.
 * For example, if an answer as an aswer ID set to 25, a new revision is
 * created to mark the node as unpublished, then the revision is deleted : the
 * actual revision still uses the answer ID 25 : the old revision will be
 * deleted on the drupal site but this answer ID must be kept on the
 * LimeSurvey database. Group queries.
 *
 * @param $node
 *   The answer node object to delete, or a string : the answer content type
 *   to delete.
 * @param $to_delete
 *   A string. Available values are 'nid' when deleting a node, 'vid' when
 *   deleting an revision, and 'type' : to delete all obsolet entries of a
 *   specific answer content type..
 *
 * @return
 *   A numeric. The number of entries deleted on the LimeSurvey table.
 */
function ls_answ_clean_delete_answer($node, $to_delete) {

  $answ_rev_sql = 'SELECT ls_sid, ls_aid, ls_token, n.nid AS node_nid, n.vid AS node_vid, type,
  node_revision.vid AS node_revision_vid
  FROM {node_revision} node_revision
  JOIN {ls_answ} lsa ON lsa.vid = node_revision.vid
  JOIN {node} n ON node_revision.nid = n.nid WHERE 1 = 1';

  $delete = $already_deleted_ls_aid  = $already_deleted_ls_token = array();
  switch ($to_delete) {
    case 'vid':
      $delete[] = node_load($node, $node->vid);
      break;

    case 'nid':
      $answ_rev_sql .= ' AND n.nid = :nnid';
      $answ_rev_query = db_query($answ_rev_sql, array(':nnid' => $node->nid));
      while ($answ_rev = $answ_rev_query->fetchObject()) {
        $delete[] = $answ_rev;
      }
      break;

    case 'type':
      $answ_rev_sql .= " AND n.type = :ntype";
      $answ_rev_query = db_query($answ_rev_sql, array(':ntype' => $node));
      while ($answ_rev = $answ_rev_query->fetchObject()) {
        $delete[] = $answ_rev;
      }
      break;

    default:
      return;
  }

  // Load file for the limesurvey_sync_answ_delete() function.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');

  $revisions_deleted = 0;
  $delete_current_rev = ($to_delete == 'nid');
  foreach ($delete as $to_test) {
    $list = ls_answ_list_protected_ref($to_test->ls_sid, $to_test->ls_aid, $to_test->ls_token, TRUE);
    $not_shared = ($to_delete == 'vid' || ($to_delete == 'nid' && $list['n_nodes'] <= 1 && isset($list['type'][$to_test->type][$to_test->node_nid])) || ($to_delete == 'type' && $list['n_types'] <= 1 && isset($list['type'][$to_test->type])));
    $obsolet = $list['obsolet'];
    $already_done = (isset($already_deleted_ls_aid[$to_test->ls_sid][$to_test->ls_aid]) || isset($already_deleted_ls_token[$to_test->ls_sid][$to_test->ls_token]));
    if (!$already_done && $not_shared && ($obsolet || $delete_current_rev)) {
      // Processed to the deletion.
      limesurvey_sync_answ_delete($to_test->ls_sid, $to_test->ls_token, $to_test->ls_aid);
      $already_deleted_ls_aid[$to_test->ls_sid][$to_test->ls_aid] = 1;
      $already_deleted_ls_token[$to_test->ls_sid][$to_test->ls_token] = 1;
      $revisions_deleted++;
    }
  }
  return $revisions_deleted;
}

/**
 * Anonymize the token table on the Limesurvey database.
 *
 * Note that this function only delete datas exported via drupal. Group queries
 * to avoid several identical database update queries for performance raisons,
 * or to avoid errors messages when a survey has already been deleted from the
 * LimeSurvey database.
 *
 * @param $on
 *   An array. The where database query.
 *
 * @return
 *   A numeric. The number of entries to be updated (return an increment
 *   number whereas the entry to delete was existing or not before the update
 *   queries).
 */
function ls_answ_delete_exported_users($on) {
  $where_field = '';
  $where_value = array();
  foreach ($on as $field => $select) {
    $where_field .= ' AND ' . $field;
    $where_value = array_merge($where_value, $select);
  }

  // It's a mass update : no db_rewrite_sql().
  $answers_query = db_query('SELECT ls_sid, ls_token
  FROM {ls_answ} lsa
  JOIN {node} n ON n.nid = lsa.nid AND n.vid = lsa.vid
  WHERE 1 = 1' . $where_field, $where_value);
  $to_test = array();
  while ($answers = $answers_query->fetchAssoc()) {
    if (!empty($answers['ls_token'])) {
      $to_test[$answers['ls_sid']][$answers['ls_token']] = 1;
    }
  }
  // Load surveys datas to query only existing survey tables.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  // Load all existing survey IDs.
  $survey_properties = limesurvey_sync_survey_properties();
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
  // Processed to the test and deletions.
  $answers_updated = 0;
  foreach ($to_test as $ls_sid => $datas) {
    if (!array_key_exists($ls_sid, $survey_properties)) {
      // The survey has been deleted on the LimeSurvey site. Associated tables
      // should not exist anymore.
      continue;
    }
    foreach ($datas as $ls_fields) {
      if ($survey_properties[$ls_sid]['tokens_table']) {
        limesurvey_sync_answ_anonymize_tokens($ls_sid, $ls_fields['ls_token']);
        $answers_updated++;
      }
    }
  }
  return $answers_updated;
}

/**
 * Provide a automatic generated title for answer nodes.
 *
 * This function generates unique titles, by generating a random number. Title
 * can be generate adding a number or a letter or comparing it with a other
 * title.
 *
 * @param $type
 *   A string. The type value of an answsers content type.
 * @param $method
 *   A string. Allow to choose how the title is going to be formated. Possible
 *   values are : '' (empty) by default, 'add_letter', 'change_prefix', 'no_title'. The
 *   ls_answ module only use the default method. Other method are for other
 *   LimeSurvey synchronization modules (ls_diff as a example).
 * @param $ref_title
 *   A string. Title to be compared with the generated title. Default is empty.
 * @param $ref_prefix
 *   A string. Prefix of the $ref_title. Default is empty.
 * @param $prefix
 *   A string. Prefix of the generated title. Default is empty.
 *
 * @return
 *   A string. The unique generated title.
 */
function ls_answ_create_title($type, $method = '', $ref_title = '', $ref_prefix = '', $prefix = '') {

  $thousands_separator = '&#8239;';
  // "narrow non breaking space", cf http://david.latapie.name/?p=4474
  // @TODO it may be interesting to use "narrow non breaking space" instead of
  // a simple space when formating numbers.

  module_load_include('inc', 'ls_survey', 'ls_survey.type');
  $variable_name = 'ls_answ_options_' . $type;
  $options = variable_get($variable_name, ls_survey_answ_default_options());
  $numbermin = $options['ls_suffix_min'];
  $numbermax = $options['ls_suffix_max'];
  if (empty($prefix)) {
    $prefix = $options['ls_prefix'];
  }
  $method_number = $options['ls_suffix_method'];
  if (empty($options['ls_prefix'])) {
    $method == 'no_title';
  }
  if ($method == 'no_title') {
    return '';
  }
  $title_list = array();
  $ref_title_number = '';
  $i = 0;
  $j = 0;
  if ($ref_title != '' && preg_match('/([ 0-9]+)$/', trim($ref_title), $matches)) {
    $ref_title_number = str_replace(' ', '', $matches[0]);
  }

  // Extracts all existings titles.
  // Order by title may improve the search via the array_key_exist() function.
  $query_title = db_query("SELECT DISTINCT node_revision.title AS title, node_revision.nid, node.type, node.nid
  FROM {node_revision} node_revision JOIN {node} node ON node_revision.nid = node.nid WHERE type = :type
  ORDER BY title", array(':type' => array($type)));

  // Select the pattern for extrating the number or the letter included into nodes title.
  $pattern_title = '/([ 0-9]+)$/';
  if ($method == 'add_letter') {
    $add_letter_prefix_title = str_replace($ref_prefix, $prefix, $ref_title);
    $pattern_title = "/^" . $add_letter_prefix_title . "[ ]?([A-Za-z]+)$/";
  }

  // Load all numbers|letters into the $title_list variable, as
  // keys.
  while ($result_title = $query_title->fetchAssoc()) {
    if (preg_match($pattern_title, trim($result_title['title']), $matches)) {
      $title_list[str_replace(' ', '', $matches[1])] = 1;
      $j++;
    }
  }
  switch ($method) {
    case 'add_letter':
      if ($ref_title_number != '') {
        // Generate a unused unique letter.
        // "i", "o" and "l" letters are ignored because they may be confusing.
        $pattern = "abcdefghjkmnpqrstuvwxyz";
        $length = 1;
        $isvalidletter = FALSE;
        $j = 0;
        while ($isvalidletter == FALSE) {
          $newletter = drupal_strtoupper(ls_answ_randomkey($length, $pattern));
          if (!isset($title_list[$newletter])) {
            $isvalidletter = TRUE;
          }
          $j++;
          // The system can adapt itself if the 24 letters are sold out.
          $length = ($j <= 24) ? 1 : ceil(($j / 24));
        }
        return ($add_letter_prefix_title . " " . $newletter);
        break;
      }

    case 'change_prefix':
      if ($ref_title_number != '') {
        return (str_replace($ref_prefix, $prefix, $ref_title));
      }
      break;

    default:
      if ($method_number == 'random') {
        // Generate a unique random number.
        $isvalidnumber = FALSE;
        while ($isvalidnumber == FALSE) {
          $newnumber = mt_rand($numbermin, $numbermax);
          if (!isset($title_list[$newnumber])) {
            $isvalidnumber = TRUE;
          }
          $i++;
          // The system can adapt itself if 75% of the interval are sold out.
          if ($i > (($numbermax - $numbermin) * 0.75)) {
            $numbermin = $options['ls_suffix_min'] = $numbermax;
            $numbermax = $options['ls_suffix_max'] = (int) (str_repeat('9', (drupal_strlen($i) + 1)));
            // Update the answers content type options.
            variable_set($variable_name, $options);
          }
        }
      }
      else {
        // Increment number
        ksort($title_list);
        $all_numbers = array_keys($title_list);
        $newnumber = ((count($all_numbers) > 0) ? max($all_numbers) : 0) + 1;
      }
      // Generate the title as "Answer #1 254" (for example).
      return ($prefix . number_format($newnumber, 0, ',', ' '));
  }
}

/**
 * Generate a random value.
 *
 * This function generates unique titles, by generating a random number. Title
 * can be generate adding a number or a letter or comparing it with a other
 * title.
 *
 * @param $length
 *   A numeric. The lenth of the random value.
 * @param $pattern
 *   A string. The caractere to use for generating the random sting. Default
 *   is "23456789abcdefghijkmnpqrstuvwxyz".
 *
 * @return
 *   A string. The unique generated key.
 */
function ls_answ_randomkey($length, $pattern = "23456789abcdefghijkmnpqrstuvwxyz") {
  $patternlength = drupal_strlen($pattern) - 1;
  for ($i = 0; $i < $length; $i++) {
    if (isset($key)) {
      $key .= $pattern{rand(0, $patternlength)};
    }
    else {
      $key = $pattern{rand(0, $patternlength)};
    }
  }
  return $key;
}

/**
 * Extract the letter token from a title.
 *
 * As an example, if 'answer #1 234 A' is provided, it returns 'A'.
 *
 * @param $string
 *   A string. The title to explore.
 *
 * @return
 *   A string. The extracted letter or empty if it fails.
 */
function ls_answ_extract_letter($string) {
  if (preg_match("/.+[ ]?([A-Za-z]+)$/", trim($string), $matches)) {
    return $matches[1];
  }
  return '';
}

/**
 * Provide an empty answer node.
 *
 * Pay attention that $type value should be controled to be an valid answer
 * content type before using this function.
 *
 * @param $type
 *   A string. The answer content type value.
 *
 * @return
 *   The new answer node object.
 */
function ls_answ_new_node($type) {
  // Create default answ_node.
  $node_answ_virgin = new stdClass();
  $node_answ_virgin->type = $type;
  module_load_include('inc', 'node', 'node.pages');
  node_object_prepare($node_answ_virgin);
  return $node_answ_virgin;
}

/**
 * Provide the display message corresponding to the ls_sync value.
 *
 * For synchronized answers : to use it only if the ls_sync value has changed.
 *
 * @param $node
 *   The answer node object.
 * @param $node_survey
 *   The associated node object. Default is NULL. providing the survey node
 *   avoid to load it twice.
 *
 * @return
 *   An array : keys are 'message' for the untranslated message and
 *   'arguments'. Returned values should by used throught the t() or the
 *   watchdog() functions.
 */
function ls_answ_sync_message($node, $node_survey = NULL) {
  if (empty($node_survey)) {
    $node_survey = ls_answ_find_survey($node->type);
  }
  $ref_answ = (!empty($node->ls_aid)) ? $node->ls_aid : (!empty($node->ls_token)) ? $node->ls_token : '';
  if (is_numeric($node->ls_sync) && ($node->ls_sync == LS_SYNC_UNSYNCHRONIZED || $node->ls_sync == LS_SYNC_SYNCHRONIZED)) {
    // For the extract translation module.
    $extract_t = t('This answer ID (%aid) is resynchronized again.');
    $messages[] = array(
      'message' => 'This answer ID (%aid) is resynchronized again.',
      'arguments' => array('%aid' => $node->ls_aid),
    );
  }
  elseif (empty($node->ls_sync)) {
    return FALSE;
  }
  else {
    switch ($node->ls_sync) {
      case LS_SYNC_DESYNCHRONIZED_SERVER:
        // For the extract translation module.
        $extract_t = t('This answer is desynchronized because the connection to your LimeSurvey database fails. Check the <a href="@url">LimeSurvey Sync settings</a> page in order to fix it.');
        $message = 'This answer is desynchronized because the connection to your LimeSurvey database fails. Check the <a href="@url">LimeSurvey Sync settings</a> page in order to fix it.';
        $arg = array(
          '@url' => url('admin/config/media/ls_sync'),
        );
        break;
      case LS_SYNC_DESYNCHRONIZED_SID:
        // For the extract translation module.
        $extract_t = t('This answer is desynchronized because its survey ID is empty');
        $message = 'This answer is desynchronized because its survey ID is empty';
        $arg = array();
        break;
      case LS_SYNC_DESYNCHRONIZED_SURVEY:
        // For the extract translation module.
        $extract_t = t('This answer survey ID (%sid) is desynchronized, it does not exist anymore on your LimeSurvey site. Check the <a href="@url">associated survey edit page</a> in order to fix it.');
        $message = 'This answer survey ID (%sid) is desynchronized, it does not exist anymore on your LimeSurvey site. Check the <a href="@url">associated survey edit page</a> in order to fix it.';
        $arg = array(
          '%sid' => $node->ls_sid,
          '@url' => url('node/' . $node_survey->nid . '/edit'),
        );
        break;
      case LS_SYNC_DESYNCHRONIZED_ANSWER:
        // For the extract translation module.
        $extract_t = t('This answer ID (%aid) is desynchronized, it does not exist anymore on your LimeSurvey site.');
        $message = 'This answer ID (%aid) is desynchronized, it does not exist anymore on your LimeSurvey site.';
        $arg = array('%aid' => $node->ls_aid);
        break;
      case LS_SYNC_DESYNCHRONIZED_TOKEN:
        // For the extract translation module.
        $extract_t = t('This answer token (%token) is desynchronized, it is not unique into the LimeSurvey database.');
        $message = 'This answer token (%token) is desynchronized, it is not unique into the LimeSurvey database.';
        $arg = array('%token' => $node->ls_token);
        break;
      case LS_SYNC_DESYNCHRONIZED_TYPE:
        // For the extract translation module.
        $extract_t = t('This answer ID (%aid) is desynchronized because it exists in more than one answers content type (current is %type).');
        $message = 'This answer ID (%aid) is desynchronized because it exists in more than one answers content type (current is %type).';
        $arg = array(
          '%aid' => $ref_answ,
          '%type' => $node->type,
        );
        break;
      case LS_SYNC_DESYNCHRONIZED_NODE:
        // For the extract translation module.
        $extract_t = t('This answer ID (%aid) is desynchronized because it exists in more than one answers node (current is %nid).');
        $message = 'This answer ID (%aid) is desynchronized because it exists in more than one answers node (current is %nid).';
        $arg = array(
          '%aid' => $ref_answ,
          '%nid' => $node->nid,
        );
        break;
      case LS_SYNC_DESYNCHRONIZED_LS_SID:
        // For the extract translation module.
        $extract_t = t('Pay attention that the survey <a href="@url">%title</a> associated to this answer actually uses a different survey ID (%sid_survey) on the LimeSurvey site. You can still edit the answer but you will find it under the %sid_answer survey ID reference on your Limesurvey site user interface.');
        $message = 'Pay attention that the survey <a href="@url">%title</a> associated to this answer actually uses a different survey ID (%sid_survey) on the LimeSurvey site. You can still edit the answer but you will find it under the %sid_answer survey ID reference on your Limesurvey site user interface.';
        $arg = array(
          '@url' => url('node/' . $node_survey->nid),
          '%title' => $node_survey->title,
          '%sid_survey' => $node_survey->ls_sid,
          '%sid_answer' => $node->ls_sid,
        );
        break;
      case LS_SYNC_DESYNCHRONIZED_LANG:
        // For the extract translation module.
        $extract_t = t('This survey language (%lang) is desynchronized, it does not exist anymore on your LimeSurvey site. Check the <a href="@url">associated survey edit page</a> in order to fix it. Default language will be used.');
        $message = 'This survey language (%lang) is desynchronized, it does not exist anymore on your LimeSurvey site. Check the <a href="@url">associated survey edit page</a> in order to fix it. Default language will be used.';
        $arg = array(
          '%lang' => $node->ls_lang,
          '@url' => url('node/' . $node_survey->nid . '/edit'),
        );
        break;
      case LS_SYNC_DESYNCHRONIZED_LS_LANG:
        // For the extract translation module.
        $extract_t = t('Pay attention that the survey <a href="@url">%title</a> associated to this answer actually uses a different default survey language (%lang_survey). Actual is %lang_answer and is still available.');
        $message = 'Pay attention that the survey <a href="@url">%title</a> associated to this answer actually uses a different default survey language (%lang_survey). Actual is %lang_answer and is still available.';
        $arg = array(
          '@url' => url('node/' . $node_survey->nid),
          '%title' => $node_survey->title,
          '%lang_survey' => $node_survey->ls_lang,
          '%lang_answer' => $node->ls_lang,
        );
        break;
      case LS_SYNC_SYNCHRONIZED_REVISION:
        // For the extract translation module.
        $extract_t = t('This answer ID (%aid) is synchronized, it corresponds to a revision from a valid answer.');
        $message = 'This answer ID (%aid) is synchronized, it corresponds to a revision from a valid answer.';
        $arg = array('%aid' => $node->ls_aid);
        break;
      default:
        // For the extract translation module.
        $extract_t = t('The synchronization status (%ls_sync) is unknowned.');
        $message = 'The synchronization status (%ls_sync) is unknowned.';
        $arg = array(
          '%ls_sync' => $node->ls_sync,
        );
    }
  }
  return array(
    'message' => $message,
    'arguments' => $arg,
  );
}

/**
 * Provide human readable value for the sync values.
 *
 * Used for dysplaying synchronization datas values, for example in the views
 * module.
 *
 * @param $field
 *   A string. The node answer synchronization field name.
 * @param $value
 *   The node answer synchronization correponding value.
 * @param $langcode
 *   A string, the language code to use to translate the values.
 *   Default is NULL, corresponding to the Drupal default language.
 *
 * @return
 *   A string. The formated value.
 */
function ls_answ_display_sync_fields($field, $value, $langcode = NULL) {
  if (empty($langcode)) {
    global $language;
    $langcode = isset($language->language) ? $language->language : 'en';
  }
  $t_arg = array();
  $t_lang = array('langcode' => $langcode);

  switch ($field) {

    case 'ls_status':
      if (is_numeric($value)) {
        switch ($value) {
          case LS_STATUS_EMPTY:
            return t('Not exist', $t_arg, $t_lang);
          case LS_STATUS_VIRGIN:
            return t('Virgin', $t_arg, $t_lang);
          case LS_STATUS_EDITING:
            return t('Editing', $t_arg, $t_lang);
          case LS_STATUS_COMPLETED:
            return t('Completed', $t_arg, $t_lang);
          default:
            return t('Unknown', $t_arg, $t_lang);
        }
      }
      else {
        return '';
      }

    case 'ls_sync':
      if (is_numeric($value)) {
        switch ($value) {
          case LS_SYNC_UNSYNCHRONIZED:
            return t('Unsynchronized', $t_arg, $t_lang);
          case LS_SYNC_SYNCHRONIZED:
            return t('Synchronized', $t_arg, $t_lang);
          case LS_SYNC_SYNCHRONIZED_REVISION:
            return t('Synchronized (old revision)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_LS_LANG:
            return t('Desynchronized (new survey language)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_LANG:
            return t('Desynchronized (language deleted)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_LS_SID:
            return t('Desynchronized (new survey ID)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_NODE:
            return t('Desynchronized (dobloon on nodes)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_TYPE:
            return t('Desynchronized (dobloon on types)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_TOKEN:
            return t('Desynchronized (token not unique)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_ANSWER:
            return t('Desynchronized (answer deleted)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_SURVEY:
            return t('Desynchronized (survey deleted)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_SID:
            return t('Desynchronized (empty survey ID)', $t_arg, $t_lang);
          case LS_SYNC_DESYNCHRONIZED_SERVER:
            return t('Desynchronized (server connection fails)', $t_arg, $t_lang);
          default:
            return t('Unknown');
        }
      }
      else {
        return '';
      }

    case 'ls_last_import':
      if (is_numeric($value)) {
        if ($value == 0) {
          return t('Never', $t_arg, $t_lang);
        }
        else {
          return format_date($value, 'small');
        }
      }
      else {
        return '';
      }

    case 'ls_sid':
      return (!empty($value)) ? number_format($value, 0, '.', ' ') : '';

    case 'ls_lang':
      // Explicit language ('en' changed to 'english').
      module_load_include('inc', 'ls_survey', 'ls_survey');
      return ls_survey_display_sync_fields('ls_lang', $value, $langcode);
      break;

    case 'ls_aid':
    case 'ls_token':

    default:
      // Display nothing for non ls_answ module nodes and for ls_aid == 0.
      return (!empty($value)) ? $value : '';
  }
  return '';
}

/**
 * Provide human readable value for the sync fields.
 *
 * Used for dysplaying synchronization datas fields name, for example in the
 * views module.
 *
 * @param $col
 *   A string. The node answer synchronization field name.
 *
 * @return
 *   A string. The formated field value.
 */
function ls_answ_display_sync_headers($col) {
  switch ($col) {
    case 'ls_status':
      return t('Answer status');
    case 'ls_sync':
      return t('Synchronization status');
    case 'ls_last_import':
      return t('Last import');
    case 'ls_sid':
      return t('Survey ID');
    case 'ls_lang':
      return t('Survey language');
    case 'ls_aid':
      return t('Answer ID');
    case 'ls_token':
      return t('Invitation token');
    default:
      return '';
  }
  return '';
}

/**
 * Provide human readable value for the sync fields descriptions.
 *
 * Used for dysplaying synchronization datas fields descriptions, for example
 * in the Rules module.
 *
 * @param $col
 *   A string. The node answer synchronization field name.
 *
 * @return
 *   A string. The formated field value.
 */
function ls_answ_display_sync_headers_description($col) {
  switch ($col) {
    case 'ls_status':
      return t('The status of the answer (completed, editing, virgin, empty)');
    case 'ls_sync':
      return t('The synchronization status of the answer (synchronized, desynchronized, unsynchronized)');
    case 'ls_last_import':
      return t('The date the answer was most recently imported from the LimeSurvey software.');
    case 'ls_sid':
      return t('The survey ID from the LimeSurvey software for this answer.');
    case 'ls_lang':
      return t('The language from the LimeSurvey software for this answer.');
    case 'ls_aid':
      return t('The answer ID from the LimeSurvey software for this answer.');
    case 'ls_token':
      return t('The token invitation value from the LimeSurvey software for this answer.');
    default:
      return '';
  }
  return '';
}

/**
 * Provide human readable value for the LimeSurvey standard fields values.
 *
 * Used for dysplaying 'submitdate', 'startlanguage', etc... values into the
 * answer display page.
 *
 * @param $field
 *   A string. The standard LimeSurvey field name.
 * @param $value
 *   The The standard LimeSurvey field correponding value.
 * @param $langcode
 *   A string, the language code to use to translate the values.
 *   Default is NULL, corresponding to the Drupal default language.
 *
 * @return
 *   A string. The formated value.
 */
function ls_answ_display_ls_standard_fields($field, $value, $langcode = NULL) {
  $standard_ls_fields = array(
    'id',
    'submitdate',
    'lastpage',
    'startlanguage',
    'token',
    'datestamp',
    'startdate',
    'ipaddr',
    'refurl',
  );

  $fields_date_to_format = array(
    'submitdate',
    'datestamp',
    'startdate',
  );

  if (in_array($field, $fields_date_to_format)) {
    // Format dates for ls_fields dates. As it is non dynamic datas, precise
    // the correct timezone.
    return limesurvey_sync_format_static_lsdate($value);
  }
  elseif (in_array($field, $standard_ls_fields)) {
    switch ($field) {
      case 'startlanguage':
        // Convert 'en' into 'English'.
        module_load_include('inc', 'ls_survey', 'ls_survey');
        return ls_survey_display_sync_fields('ls_lang', $value, $langcode);

      case 'ipaddr':
      case 'refurl':
        // Display them as urls.
        // An ip address (127.0.0.1) generates a buggy link :
        // example.com/127.0.0.1 ; Fix it.
        $http = (preg_match('#^(http|ftp)[s]?://.+#', $value)) ? '' : 'http://';
        return (!empty($value)) ? l($value, $http . $value, array('external' => TRUE)) : '';

      default:
        return $value;
    }
  }
  else {
    return $value;
  }
}

/**
 * Merge multidimensional arrays recursivly without overwriting numeric keys.
 *
 * Code from
 * http://www.php.net/manual/fr/function.array-merge-recursive.php#106985.
 * Used to merge synchronization results.
 *
 * @return
 *   An array. The merged array.
 */
function ls_answ_array_merge_recursive_keep_num_keys() {
  $arrays = func_get_args();
  $base = array_shift($arrays);

  foreach ($arrays as $array) {
    reset($base); // Important
    while (list($key, $value) = @each($array)) {
      if (is_array($value) && @is_array($base[$key])) {
        $base[$key] = ls_answ_array_merge_recursive_keep_num_keys($base[$key], $value);
      }
      else {
        $base[$key] = $value;
      }
    }
  }

  return $base;
}

/**
 * Return the LimeSurvey language id corresponding to an Drupal language.
 *
 * Several LimeSurvey languages may be availables for a single Drupal language.
 * As an example, a survey can store 'de-informal' and 'de' LimeSurvey
 * languages, both corresponding to the 'de' Drupal language. Providing the
 * optionals parameters 'Survey ID' and 'suggested LimeSurvey language' solves
 * such cases.
 *
 * @param $dp_lang
 *   A string, the Drupal language id.
 * @param $sid
 *   An integer, the LimeSurvey survey ID. Default is empty.
 * @param $suggested_ls_lang
 *   A string, a suggested LimeSurvey language ID. Default is empty.
 *
 * @return
 *   A string, the corresponding LimeSurvey language id, or FALSE if an error
 *   occurs.
 */
function ls_answ_lang_drupal_to_ls($dp_lang, $sid = '', $suggested_ls_lang = '') {
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $convert = limesurvey_sync_lang_convert('', TRUE);
  $ls_language = array();
  // A drupal language may match several LimeSurvey language. In such cases,
  // uses the optional parameter $ls_lang.
  foreach ($convert as $ls_langcode => $dp_langcode) {
    if ($dp_langcode == $dp_lang) {
      $ls_language[] = $ls_langcode;
    }
  }
  // Count how many LimeSurvey lang match for this Drupal lang.
  if (count($ls_language) == 0) {
    return FALSE;
  }
  elseif (count($ls_language) == 1) {
    return $ls_language[0];
  }
  else {
    // Several LimeSurvey languages are availables.
    // the survey stores 'de-informal' and 'de' languages as an example, both
    // corresponding to the 'de' Drupal language.
    if (empty($sid)) {
      // The correct language can not be guess.
      if (limesurvey_sync_lang_convert($suggested_ls_lang) == $dp_lang) {
        return $suggested_ls_lang;
      }
      // Or not.
      return FALSE;
    }
    // Retrieve all languages availables for this survey.
    $survey_properties = limesurvey_sync_survey_properties($sid);
    $avalaible_ls_langs = array_keys($survey_properties['#lang']);
    $ls_multiple_language = array();
    foreach ($avalaible_ls_langs as $potential_ls_lang) {
      $potential_dp_lang = limesurvey_sync_lang_convert($potential_ls_lang);
      if ($potential_dp_lang == $dp_lang) {
        if ($suggested_ls_lang == $potential_dp_lang) {
          return $suggested_ls_lang;
        }
        $ls_multiple_language[] = $potential_ls_lang;
      }
    }
    if (count($ls_multiple_language) == 0) {
      return FALSE;
    }
    elseif (count($ls_multiple_language) == 1) {
      return $ls_multiple_language[0];
    }
    else {
      // Several LimeSurvey languages are availables and suggested ls_lang does
      // not help.
      return FALSE;
    }
  }
}

/**
 * Return the correct LimeSurvey language ID value for an Drupal Language ID.
 *
 * Available languages are the ones existing for this survey on the LimeSurvey
 * software, even if thoses languages have not been imported to a Drupal Survey
 * node neither if the corresponding Drupal language is not enabled. It makes
 * sense because the translations are stored into the LimeSurvey software.
 * However, extend answer translations ('Y' to 'Oui' in french) will fail if
 * the correponding language is not enabled on Drupal, displaying the english
 * datas.
 *
 * @param $node
 *   An answer node object. At least, ls_sid, ls_lang, type and body must be
 *   provided.
 * @param $lang_dp_to_import
 *   A string, the Drupal language ID to use for importing the answer. Default
 *   is empty, corresponding to the Drupal language ID of the node->ls_lang
 *   value.
 *
 * @return
 *   A string, LimeSurvey language ID value to use for importing answers datas
 *   from the LimeSurvey database, if the provided language is not available,
 *   provide answers content type current language or if it fails, the
 *   LimeSurvey survey default language.
 */
function ls_answ_validlang_ls($node, $lang_dp_to_import = '') {
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  if (empty($lang_dp_to_import)) {
    $lang_dp_to_import = limesurvey_sync_lang_convert($node->ls_lang);
  }
  module_load_include('inc', 'ls_survey', 'ls_survey.type');
  $options = variable_get('ls_answ_options_' . $node->type, ls_survey_answ_default_options());
  $ls_import_lang = $options['ls_import_lang'];
  $empty_body = empty($node->body[ls_answ_body_language($node)][0]['value']);
  $overwrite = (!in_array($ls_import_lang, array('', 'ls_lang')));
  // Retreive the default survey node language
  $node_survey = ls_answ_find_survey($node->type);
  if ($overwrite) {
    $lang_to_test = ls_answ_lang_drupal_to_ls($ls_import_lang, $node->ls_sid, $node->ls_lang);
  }
  elseif ($empty_body) {
    $lang_to_test = limesurvey_sync_lang_convert($node_survey->ls_lang);
  }
  else { // if !$empty_body
    // The answer has been imported once, keep on the existing language.
    $lang_to_test = limesurvey_sync_lang_convert($node->ls_lang);
  }
  $sid = $node->ls_sid;
  $availables = array();
  $survey_properties = limesurvey_sync_survey_properties($sid);
  foreach (array_keys($survey_properties['#lang']) as $ls_lang) {
    $ls_lang_dp = limesurvey_sync_lang_convert($ls_lang);
    if (($overwrite && $ls_import_lang == $ls_lang_dp) || (!$overwrite && $ls_lang_dp == $lang_to_test)) {
      return $ls_lang;
    }
    $availables[$ls_lang] = $ls_lang_dp;
  }
  // The provided language does not exist on the LimeSurvey database.
  // Arbitraily returns the answer content type default language, or the
  // LimeSurvey default language if it fails again.
  $to_import_ls = array_search($lang_dp_to_import, $availables);
  $ls_lang_dp = limesurvey_sync_lang_convert($survey_properties['default_language']);
  if ($to_import_ls !== FALSE) {
    return $to_import_ls;
  }
  else {
    return $survey_properties['default_language'];
  }
}

/**
 * Return the correct Drupal language ID to use on import into the t() function.
 *
 * Available languages are the enabled Drupal languages.
 *
 * @param $lang_dp_to_import
 *   A string, the Drupal language ID to use for importing the answer.
 *
 * @return
 *   A string, LimeSurvey languagecode ID value to use into the t() function on
 *   importing answers datas from the LimeSurvey database, or NULL in all other
 *   cases.
 */
function ls_answ_validlang_dp_langcode($lang_dp_to_import) {
  if (!drupal_multilingual() || !module_exists('locale')) {
    // The answer node langcode doesn't need to be set.
    return NULL;
  }
  global $language;
  $current_language = isset($language->language) ? $language->language : 'en';

  // Retrieves the Drupal enabled languages.
  $languages = language_list('enabled');
  $available_dp_langs = array_keys($languages[1]);
  if (in_array($lang_dp_to_import, $available_dp_langs)) {
    return ($lang_dp_to_import != $current_language) ? $lang_dp_to_import : NULL;
  }
  // The provided language is not available on Drupal.
  return NULL;
}

/**
 * Return the correct Drupal node language ID to use for answer nodes.
 *
 * This function returns the node language ($node->language) from the core
 * Locale module. Note: if the answer import language is overwritten using
 * the ls_import_lang option, the language will fit to this new value,
 * ignoring the provided parameter.
 *
 * @param $node
 *   An answer node object. At least, ls_sid, ls_lang, type and body must be
 *   provided.
 * @param $lang_dp_to_import
 *   A string, the Drupal language ID to use for importing the answer. Default
 *   is empty, corresponding to the Drupal language ID of the node->ls_lang
 *   value.
 * @param $force
 *   A boolean, if TRUE returns the drupal language as it should be even if the
 *   multilingual option is not enabled. This is used for displaying
 *   information on the answers node type page. Default is FALSE.
 *
 * @return
 *   A string, LimeSurvey languagecode ID value to store into the
 *   $node->language variable, or NULL in all other cases.
 */
function ls_answ_validlang_dp_nodelanguage($node, $lang_dp_to_import = '', $force = FALSE) {
  if (!$force && (!drupal_multilingual() || !module_exists('locale') || !variable_get('language_content_type_' . $node->type, 0))) {
    // The answer node language doesn't need to be set.
    return NULL;
  }

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  if (empty($lang_dp_to_import)) {
    $lang_dp_to_import = limesurvey_sync_lang_convert($node->ls_lang);
  }
  // The lang may be overwritten.
  $lang_to_test = ls_answ_validlang_ls($node, $lang_dp_to_import);
  // Retrieves the Drupal enabled languages.
  $languages = language_list('enabled');
  $available_dp_langs = array_keys($languages[1]);
  if (in_array($lang_to_test, $available_dp_langs)) {
    return $lang_to_test;
  }
  if (in_array($lang_dp_to_import, $available_dp_langs)) {
    return $lang_dp_to_import;
  }
  // The provided language is not available on Drupal.
  return NULL;
}

/**
 * Provide the node_language key for the answer body field.
 *
 * I have created this function in order to update it depending on bug reports.
 * Actually, as far as I can see, the language value used into the example :
 * $node->body[$node->language][0]['value'] = 'This is a body text'; is always
 * 'und', even if the locale module or the content type core modules (or
 * multilingue custom modules) are enabled. The node language is stored into
 * the node table and its associated body field seems to be independant.
 *
 * @param $node
 *   The answer node object.
 *
 * @return
 *   A string. The language value
 */
function ls_answ_body_language($node) {
  // $body_lng = (empty($node->language)) ? LANGUAGE_NONE : $node->language;
  $body_lng = LANGUAGE_NONE;
  return $body_lng;
}

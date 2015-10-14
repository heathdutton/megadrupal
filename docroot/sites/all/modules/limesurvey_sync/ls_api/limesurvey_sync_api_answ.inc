<?php
/**
 * @file
 * API functions for the ls_answ module
 */

/**
 * Provide the url to the limeSurvey website for filling an answer.
 *
 * @param $node
 *   An object. The answer node.
 * @param $absolute
 *   A boolean. For absolute url. Default is TRUE.
 * @param $extraquery
 *   An array, more variables to add to the query. used by the colorbox module.
 *   Default is empty.
 *
 * @return
 *   A string. The url of the Limesurvey site answer editing form.
 */
function limesurvey_sync_answ_surveyurl($node, $absolute = TRUE, $extraquery = array()) {

  $ls_settings = variable_get('ls_settings', array('ls_path' => ''));
  $ls_url = $ls_settings['ls_path'] . '/index.php';
  if ($absolute) {
    module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
    // Convert relative to absolute url if needed and format the url into the
    // drupal way.
    $ls_url = limesurvey_sync_rel2abs($ls_url);
  }
  $query = array('absolute' => $absolute);

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $ls_v2 = (limesurvey_sync_lsversion() != '1.x');

  if (!empty($node->ls_sid)) {
    if ($ls_v2) {
      $ls_url .= '/' . $node->ls_sid;
    }
    else {
      $query['query']['sid'] = $node->ls_sid;
    }
  }

  // Strange behavior on urls on LS 2.0:
  // limesurvey/index.php/619887/tk-shkjg352icqhaad/lang-fr returns 'page not
  // found'.
  // limesurvey/index.php/619887/tk-shkjg352icqhaad succeed but is in english.
  // limesurvey/index.php/619887/lang-fr/tk-shkjg352icqhaad succeed (fr)!
  // Url from http://docs.limesurvey.org/tiki-index.php?page=Optional+settings.
  // limesurvey/index.php/survey/index/sid/619887/token/shkjg352icqhaad/lang/fr
  // succeed (this is the link provided by the email invitation).
  // The email invitation provides the lang parameter even for default survey
  // language as limesurvey/index.php/survey/index/sid/619887/token/1234/lang/en.
  if (!empty($node->ls_lang)) {
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    $survey_properties = limesurvey_sync_survey_properties($node->ls_sid);
    if ($node->ls_lang != $survey_properties['default_language']) {
      if ($ls_v2) {
        $ls_url .= '/lang-' . $node->ls_lang;
      }
      else {
        $query['query']['lang'] = $node->ls_lang;
      }
    }
  }

  if (!empty($node->ls_token)) {
    if ($ls_v2) {
      $ls_url .= '/tk-' . $node->ls_token;
    }
    else {
      $query['query']['token'] = $node->ls_token;
    }
  }

  // Handle extraqueries :
  foreach ($extraquery as $key => $value) {
    $query['query'][$key] = $value;
  }

  return str_replace(' ', '%20', url($ls_url, $query));
}

/**
 * Retrieve LimeSurvey values from the http referrer.
 *
 * Extract pertinent datas from the http referer. It should be executed after
 * the Survey has been completed on the LimeSurvey interface.
 *
 * @return
 *   An array, listing the formated ls_sid, ls_token, ls_lang values or FALSE
 *   if it fails or TRUE if the referer url is the LimeSurvey site but the
 *   ls_sid value can not be found.
 */
function limesurvey_sync_answ_extract_referer() {


  $http_referer = $_SERVER['HTTP_REFERER'];
  $url_extract = @parse_url($http_referer);

  if (!isset($url_extract['scheme']) || !isset($url_extract['host']) || !isset($url_extract['path'])) {
    return FALSE;
  }
  $port = (!empty($url_extract['port'])) ? ':' . $url_extract['port'] : '';
  $referrer_base_url = drupal_strtolower(trim($url_extract['scheme'] . '://' . $url_extract['host'] . $port . $url_extract['path']));
  $expected_url = drupal_strtolower(trim(limesurvey_sync_answ_surveyurl(NULL, TRUE)));
  $ls_referer = (strpos($referrer_base_url, $expected_url) === 0);
  $data_referer = array();
  $ls_v2 = (limesurvey_sync_lsversion() != '1.x');
  if ($ls_v2) {
    // Examples LimeSurvey v2.x urls:
    // $referrer_base_url = http://example.com/limesurvey/index.php/168837/tk-3p5sqmi5mq5y4g3.
    // or if the http_referrer method is not available :
    // $referrer_base_url = http://example.com/limesurvey/index.php/survey/index.
    // $expected_url = http://example.com/limesurvey/index.php.
    if ($ls_referer) {
      $query_string = drupal_substr($referrer_base_url, drupal_strlen($expected_url));
      if ($pos_query2 = strpos('/survey/index', $query_string) === 0) {
        $query_string = drupal_substr($query_string, drupal_strlen('/survey/index'));
      }
      // $query = '/168837/tk-3p5sqmi5mq5y4g3'
      $query_array = explode('/', $query_string);
      foreach ($query_array as $arg) {
        if (is_numeric($arg)) {
          $data_referer['ls_sid'] = check_plain($arg);
        }
        elseif (preg_match('#^tk-([a-z0-9]+)$#', $arg, $matches)) {
          $data_referer['ls_token'] = $matches[1];
        }
        elseif (drupal_substr($arg, 0, 5) == 'lang-') {
          $data_referer['ls_lang'] = drupal_substr($arg, 5);
        }
      }
    }
  }
  else {
    // Examples LimeSurvey v1.x urls:
    // $referrer_base_url = http://example.com/limesurvey/index.php?sid=168837&token=3p5sqmi5mq5y4g3.
    // $expected_url = http://example.com/limesurvey/index.php
    if ($ls_referer) {
      if (isset($url_extract['query'])) {
        $url_query = $url_extract['query'];
        parse_str($url_query, $store);
        if (!empty($store['sid']) && is_numeric($store['sid'])) {
          $data_referer['ls_sid'] = check_plain($store['sid']);
        }
        if (!empty($store['token']) && preg_match('#^[a-z0-9]+$#', $store['token'])) {
          $data_referer['ls_token'] = check_plain($store['token']);
        }
        if (!empty($store['lang'])) {
          $data_referer['ls_lang'] = check_plain($store['lang']);
        }
      }
    }
  }

  if (!$ls_referer) {
    return FALSE;
  }
  elseif (isset($data_referer['ls_sid'])) {
    return $data_referer;
  }
  else {
    return TRUE;
  }
}

/**
 * Delete an answer entry into the LimeSurvey database.
 *
 * @param $sid
 *   A numeric. The survey ID reference.
 * @param $token
 *   A string. The answer token.
 * @param $aid
 *   A numeric. The answer ID reference. Default is 0.
 *
 * @return
 *   A Boolean. FALSE if there is no answer to delete or if an error occures.
 */
function limesurvey_sync_answ_delete($sid, $token, $aid = 0) {
  if (empty($sid) || (empty($aid) && empty($token))) {
    return FALSE;
  }

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $previous_db = limesurvey_sync_db_open();
  $delete = db_delete(limesurvey_sync_table('answer', $sid));
  if (!empty($aid)) {
    $delete->condition('id', $aid);
  }
  elseif (!empty($token)) {
    $delete->condition('token', $token);
  }
  $n_row_deleted = $delete->execute();
  limesurvey_sync_db_close($previous_db);
  return ($n_row_deleted === FALSE) ? FALSE : TRUE;
}

/**
 * Delete an invitation entry into the LimeSurvey database.
 *
 * @param $sid
 *   A numeric. The survey ID reference.
 * @param $token
 *   A string. The answer token.
 *
 * @return
 *   A Boolean. False if an error occures.
 */
function limesurvey_sync_answ_delete_token($sid, $token) {
  if (empty($sid) || empty($token)) {
    return FALSE;
  }
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $previous_db = limesurvey_sync_db_open();
  $n_row_deleted = db_delete(limesurvey_sync_table('tokens', $sid))
  ->condition('token', $token)
  ->execute();
  limesurvey_sync_db_close($previous_db);
  return ($n_row_deleted === FALSE) ? FALSE : TRUE;
}

/**
 * Delete user informations into the token table of the LimeSurvey database.
 *
 * @param $sid
 *   A numeric. The survey ID reference.
 * @param $token
 *   A string. The answer token.
 *
 * @return
 *   A Boolean. False if an error occures.
 */
function limesurvey_sync_answ_anonymize_tokens($sid, $token) {
  if (empty($sid) || empty($token)) {
    return FALSE;
  }
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $previous_db = limesurvey_sync_db_open();
  $n_row_updated = db_update(limesurvey_sync_table('tokens', $sid))
    ->fields(array(
    'firstname' => '',
    'lastname' => '',
    'email' => '',
  ))
    ->condition('token', $token)
    ->execute();
  limesurvey_sync_db_close($previous_db);
  return ($n_row_updated === FALSE) ? FALSE : TRUE;
}

/**
 * Provide a unique token (a generate random string) for a specific survey ID.
 *
 * Check into the LimeSurvey database (answers table and tokens table) and into
 * the Drupal {ls_answ} table for a unique random string.
 *
 * @param $sid
 *   A numeric. The survey ID reference.
 *
 * @return
 *   A string. A unique random string that never been used before.
 */
function limesurvey_sync_answ_create_token($sid) {

  if (empty($sid)) {
    return;
  }
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');

  // Find the token length :
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($sid);
  // The length of the generate random string. Deflaut value is 15.
  $tokenlength = (!empty($survey_properties['tokenlength'])) ? $survey_properties['tokenlength'] : 15;

  // Load all existing tokens into an array using each token as key.
  $existingtokens = array();

  // Load tokens from the LimeSurvey tokens table.
  $previous_db = limesurvey_sync_db_open();
  $result_alltoken_token = db_query('SELECT token FROM {' . limesurvey_sync_table('tokens', $sid) . '}');
  while ($row_alltoken_token = $result_alltoken_token->fetchAssoc()) {
    if ($row_alltoken_token['token'] != '') {
      $existingtokens[$row_alltoken_token['token']] = 1;
    }
  }

  // Load tokens from the LimeSurvey answers table, if token persistence is
  // actived.
  if ($survey_properties['tokenanswerspersistence']) {
    $result_alltoken_answers = db_query('SELECT token FROM {' . limesurvey_sync_table('answer', $sid) . '}');
    // Load all tokens from the LimeSurvey table responds.
    while ($row_alltoken_answers = $result_alltoken_answers->fetchAssoc()) {
      if ($row_alltoken_answers['token'] != '') {
        $existingtokens[$row_alltoken_answers['token']] = 1;
      }
    }
  }

  // Load tokens from the Drupal answers table.
  // No addTag('node_access') : it's a system function.
  // Need to connect to the Drupal database.
  limesurvey_sync_db_close();
  $result_alltoken_drupal = db_query('SELECT ls_token AS token FROM {ls_answ} lsa JOIN {node} n ON lsa.nid = n.nid AND lsa.vid = n.vid WHERE ls_sid = :ls_sid', array(':ls_sid' => $sid));
  while ($row_alltoken_drupal = $result_alltoken_drupal->fetchAssoc()) {
    if ($row_alltoken_drupal['token'] != '') {
      $existingtokens[$row_alltoken_drupal['token']] = 1;
    }
  }

  // All exisiting tokens are loaded.
  // Now generate a unique token.
  $isvalidtoken = FALSE;
  $token_quantity = count($existingtokens);
  // Load file or ls_answ_randomkey().
  module_load_include('inc', 'ls_answ', 'ls_answ');
  // Counter.
  $n = 1;
  while ($isvalidtoken == FALSE) {
    $newtoken = ls_answ_randomkey($tokenlength);
    if (!isset($existingtokens[$newtoken]) || $token_quantity == 0) {
      $isvalidtoken = TRUE;
    }
    // Dynamic adaptation if 75% of possible tokens are used.
    $n++;
    // 24 is 75% of the 32 available caracters.
    if ($n > (24 * $tokenlength)) {
      $tokenlength++;
    }
  }

  // Switch back to the previous connection, before the
  // limesurvey_sync_answ_create_token() function was called.
  limesurvey_sync_db_close($previous_db);

  return ($newtoken);
}

/**
 * Create or update an invitation in order to fill a LimeSurvey form.
 *
 * In order to access to the LimeSurvey site form, an unique an valid
 * invitation must be created or updated into the token table of the LimeSurvey
 * database.
 *
 * @param $type
 *   A strin. The type of the answer node.
 * @param $ls_sid
 *   A numeric. The survey ID reference.
 * @param $ls_lang
 *   A string. The survey language value.
 * @param $use_this_token
 *   A string. Force to use the provided token. Default is empty : a unique
 *   random token will be generate.
 * @param $author
 *   An string. The author username of the answer node.
 *
 * @return
 *   An string. The token corresponding to the new valid invitation or FALSE
 *   if an error occures.
 */
function limesurvey_sync_answ_set_invit($type, $ls_sid, $ls_lang, $use_this_token = '', $author = FALSE ) {
  if (empty($ls_sid)) {
    return FALSE;
  }
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $table_token = limesurvey_sync_table('tokens', $ls_sid);

  // Set previous db.
  $previous_db = limesurvey_sync_active_db_name();
  // Make sure we run on the Drupal database.
  limesurvey_sync_db_close();

  // Prepare the invitation.

  // Prepare dates.
  $time = REQUEST_TIME;
  $valid_from = limesurvey_sync_fixdate_drupal_to_ls($time);
  module_load_include('inc', 'ls_survey', 'ls_survey.type');
  $options = variable_get('ls_answ_options_' . $type, ls_survey_answ_default_options());
  $valid_until = (empty($options['ls_validuntil'])) ? NULL : limesurvey_sync_fixdate_drupal_to_ls($time + $options['ls_validuntil']);

  // Prepare author.
  // Load the survey properties.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($ls_sid);
  // Ultimate control on the survey properties for security raison if the
  // survey options are not correctly set.
  $export_user_info = ($options['ls_export_token'] && !$survey_properties['anonymized']);
  if ($export_user_info) {
    // Retrieve $account mail.
    $account = (!empty($author)) ? user_load_by_name($author) : FALSE;
    $lastname = ($account) ? $account->name : variable_get('anonymous', t('Anonymous'));
    $email = ($account) ? $account->mail : '';
  }
  else {
    $lastname = '';
    $email = '';
  }

  // Prepare the token, using the provided one or not.
  if (!empty($use_this_token)) {
    $token = $use_this_token;
    // Check if an invitation exist with the provided token.
    limesurvey_sync_db_open();
    $invit_exists = (db_query('SELECT 1 FROM {' . $table_token . '} WHERE token = :token', array(':token' => $token))->fetchField());
    limesurvey_sync_db_close();
  }
  else {
    // Or generate a unique random token.
    module_load_include('inc', 'ls_answ', 'ls_answ');
    $token = limesurvey_sync_answ_create_token($ls_sid);
    $invit_exists = FALSE;
  }

  // Prepare fields.
  // Code is an adpated version of the code from
  // limesurvey/admin/token.php line 2070
  $data = array(
    'firstname' => '',
    'lastname' => $lastname,
    'email' => $email,
    'emailstatus' => 'OK',
    'token' => $token,
    'language' => $ls_lang,
    'sent' => "N",
    'remindersent' => "N",
    'completed' => "N",
    'validfrom' => $valid_from,
    'validuntil' => $valid_until,
  );

  // In the token table of the LimeSurvey database schema, a new colomn has
  // been added about the 1.91+ version (exactly : in DBVersion = 145).
  // We adapt here the API to handle both versions.
  // We first retrieve the current LimeSurvey site database version.
  $col_usesleft_exists = ($survey_properties['db_version'] >= 145);

  if ($col_usesleft_exists) {
    $data['usesleft'] = 1;
  }

  // Create or update the invitation into the token table of the LimeSurvey
  // Database.
  if ($invit_exists) {
    // Update it.
    limesurvey_sync_db_open();
    $n_row_updated = db_update($table_token)
      ->fields($data)
      ->condition('token', $data['token'])
      ->execute();
    $success = ($n_row_updated === FALSE) ? FALSE : TRUE;
    limesurvey_sync_db_close();
  }
  else {
    // Or insert it.
    limesurvey_sync_db_open();
    $n_row_inserted = db_insert($table_token)
      ->fields($data)
      ->execute();
    $success = ($n_row_inserted === FALSE) ? FALSE : TRUE;
  }
  limesurvey_sync_db_close($previous_db);
  // Return the token from the valid invitation.
  return ($success) ? ($token) : FALSE;
}

/**
 * Extract selected answers from the LimeSurvey answers table.
 *
 * @param $ls_sid
 *   A numeric. The answer survey ID reference.
 * @param $on
 *   An array. The where database query. Default is empty.
 * @param $limit
 *   A numeric. the number of nodes to create. On batch operations it will be
 *   overwriten. Default is 0, corresponding to all answers stored on the
 *   LimeSurvey table.
 * @param $order
 *   A string. A custom order on query. Default is empty corresponding to
 *   order by id ASC. Set it as 'submitdate DESC' as an example.
 *
 * @return
 *   An array. The ->fetchAssoc() returned.
 */
function limesurvey_sync_answ_extract_answers($ls_sid, $on = array(), $limit = 0, $order = '') {
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($ls_sid);
  if (empty($survey_properties)) {
    // The survey does not exist.
    return FALSE;
  }

  // Load file for the limesurvey_sync functions.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $table_answer = limesurvey_sync_table('answer', $ls_sid);
  // Token field can exist even if tokenanswerspersistence is FALSE.
  $previous_db = limesurvey_sync_db_open();
  $token_field_query = db_query_range('SELECT answ.* FROM {' . $table_answer . '} AS answ', 0, 1);
  $token_field_row = $token_field_query->fetchAssoc();
  limesurvey_sync_db_close();
  $token_field_exists = is_array($token_field_row) ? (array_key_exists('token', $token_field_row)) : FALSE;
  $select_token = ($token_field_exists) ? ' token AS ls_token,' : '';

  $where_field = '';
  $where_value = array();
  foreach ($on as $field => $select) {
    // token filter should be check before calling the
    // limesurvey_sync_answ_extract_answers() function.
    if ($token_field_exists || drupal_substr($field, 0, 5) != 'token') {
      $where_field .= ' AND ' . $field;
      $where_value = array_merge($where_value, $select);
    }
    else {
      return FALSE;
    }
  }
  // Usefull for batching operations.
  $order_by = (empty($order)) ? ' ORDER BY id ASC' : ' ORDER BY ' . $order;

  $sql = 'SELECT id AS ls_aid, startlanguage AS ls_lang,' . $select_token . ' submitdate FROM {' . $table_answer . '} WHERE 1 = 1' . $where_field . $order_by;
  limesurvey_sync_db_open();
  if (!empty($limit)) {
    $answer_query = db_query_range($sql, 0, $limit, $where_value);
  }
  else {
    $answer_query = db_query($sql, $where_value);
  }

  if ($answer_query === FALSE) {
    limesurvey_sync_db_close($previous_db);
    return FALSE;
  }
  $all_answers = array();
  // Add an empty token field if not exist.
  while ($row_answer = $answer_query->fetchAssoc()) {
    $all_answers[] = ($token_field_exists) ? $row_answer : array_merge($row_answer, array('ls_token' => ''));
  }
  limesurvey_sync_db_close($previous_db);

  return $all_answers;
}

/**
 * Allow an answer on the Limesurvey site to be edited again.
 *
 * It it used when an answernode is edited and revision is FALSE. It imports
 * the new token on the answer table if needed.
 *
 * @param $ls_sid
 *   A numeric. The answer survey ID reference.
 * @param $ls_lang
 *   A string. The answer language value.
 * @param $ls_token
 *   A string. The answer token value.
 *
 * @return
 *   An Boolean. True if it succed or FALSE if an error occures.
 */
function limesurvey_sync_answ_reactivate_answer($ls_sid, $ls_aid, $ls_token) {
  if (empty($ls_aid) && empty($ls_token)) {
    return FALSE;
  }

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $table_answer = limesurvey_sync_table('answer', $ls_sid);
  // Load the survey properties.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($ls_sid);
  $update_fields = array(
    'lastpage' => 1,
    'token' => $ls_token,
  );
  // Set submitdate = NULL to mark it as uncompleted for bdversion < 145
  // (since dbversion 145, the alloweditafertcompletion and useleft properties
  // has been added).
  if ($survey_properties['db_version'] < 145) {
    $update_fields['submitdate'] = NULL;
  }
  $previous_db = limesurvey_sync_db_open();
  $n_row_updated = db_update($table_answer)
    ->fields($update_fields);
  if ($ls_aid != 0) {
    $n_row_updated->condition('id', $ls_aid);
  }
  else {
    $n_row_updated->condition('token', $ls_token);
  }
  $n_row_updated->execute();
  $success = ($n_row_updated === FALSE) ? FALSE : TRUE;
  limesurvey_sync_db_close($previous_db);
  return ($success);
}

/**
 * Duplicate the answer on the LimeSurvey site for answer node revision.
 *
 * It copies an answer into into the LimeSurvey database, returning the new
 * token associated to the copy. Original answer will be set to "uncompleted"
 * into LimeSurvey or will be deleted depending on survey options.
 *
 * @param $ls_sid
 *   A numeric. The answer survey ID reference.
 * @param $ls_lang
 *   A string. The answer language value.
 * @param $ls_token
 *   A string. The answer token value.
 * @param $open
 *   An boolean. To mark as uncompleted the original answer. Default is TRUE.
 *
 * @return
 *   An string. The token corresponding to the new answer or FALSE
 *   if an error occures.
 */
function limesurvey_sync_answ_duplicate_answer($ls_sid, $ls_aid, $ls_token, $open = TRUE) {

  // Load the survey properties.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($ls_sid);

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $table_answer = limesurvey_sync_table('answer', $ls_sid);
  // Set the previous db connection.
  $previous_db = limesurvey_sync_active_db_name();

  // Load the original answer datas from the LimeSurvey database.
  if (!empty($ls_aid) || !empty($ls_token)) {
    if (!empty($ls_aid)) {
      $where_field = 'id = :id';
      $where_value = array(':id' => $ls_aid);
    }
    else {
      $where_field = "token = :token";
      $where_value = array(':token' => $ls_token);
    }
    limesurvey_sync_db_open();
    $query_ls_data = db_query('SELECT answ.* FROM {' . $table_answer . '} AS answ WHERE ' . $where_field, $where_value);
    $ls_data = $query_ls_data->fetchAssoc();
    limesurvey_sync_db_close();
  }
  else {
    $ls_data = FALSE;
  }

  if (!$ls_data) {
    // There is no answer to duplicate.
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    $survey_properties = limesurvey_sync_survey_properties($ls_sid);
    if ($survey_properties['tokens_table']) {
      $final_token = ((empty($ls_token)) ? limesurvey_sync_answ_create_token($ls_sid) : $ls_token);
    }
    else {
      $final_token = '';
    }
    limesurvey_sync_db_close($previous_db);
    return array(
      'ls_token' => $final_token,
      'ls_aid' => 0,
    );
  }

  // Create a unique new token.
  $new_token = limesurvey_sync_answ_create_token($ls_sid);

  // Copy the answer to an new entry, importing the new token.
  $all_field = $all_data_format = $all_data = array();
  $time = REQUEST_TIME;
  foreach ($ls_data as $field => $data) {
    switch ($field) {

      case 'id':
        // Primary key.
        break;

      case 'token':
        // Pay attention on importing the new token.
        $all_data[$field] = $new_token;
        break;

      case 'submitdate':
        // Setting submitdate = NULL to allow the new answer to be edited !
        if ($survey_properties['db_version'] < 145) {
          $all_data[$field] = NULL;
        }
        break;

      case 'lastpage':
        // Set last page to 1 to open the answer to its first page.
        $all_data[$field] = ($open) ? 1 : $data;
        break;

      case 'datestamp':
        // Change the answer date (optional).
        $all_data[$field] = limesurvey_sync_fixdate_drupal_to_ls();
        break;

      default:
        // Handle all answer datas.
        $all_data[$field] = $data;
    }
  }

  limesurvey_sync_db_open();
  // Insert and retrieve the new answer ID.
  $new_aid = db_insert($table_answer)
    ->fields($all_data)
    ->execute();
  limesurvey_sync_db_close($previous_db);

  $answ_ref = array(
    'ls_token' => $new_token,
    'ls_aid' => $new_aid,
  );

  // Update the original answer to mark it as "uncompleted", setting the
  // submitdate to NULL.
  if ($open) {
    limesurvey_sync_db_open();
    $n_row_updated = db_update($table_answer)
      ->fields(array('submitdate' => NULL));
    if ($ls_aid != 0) {
      $n_row_updated->condition('id', $ls_aid);
    }
    else {
      $n_row_updated->condition('token', $ls_token);
    }
    $n_row_updated->execute();
    limesurvey_sync_db_close($previous_db);
  }

  return $answ_ref;
}

/**
 * Import one or more LimeSurvey answer(s) as an html table.
 *
 * Generate an html table displaying a corresponding answer id datas, options
 * may alter the results. The html able will be stores into the body field of
 * an answer node. It provides to the matching score if several answers are
 * provided ans how many answers where provided.
 *
 * @param $node
 *   A node object. It must contain at least ls_sid, ls_lang and for language
 *   troublesettings type, body.
 * @param $ls_aid_s
 *   A numeric or an array. If it is a numeric value, it is the answer ID. If
 *   it is an array : keys are the answer IDs and values are the associated
 *   header.
 * @param $options
 *   An array containg the keys 'ls_merge', 'ls_only_answered', 'ls_ignore_x'
 *   filled with a boolean value and the key 'ls_fields' storing fields to
 *   show in the html table (available values are 'id', 'submitdate',
 *   'lastpage', 'startlanguage', 'token', 'datestamp', 'startdate', 'ipaddr',
 *   'refurl'). It alters the html result.
 *
 * @return
 *   An array. Keys are : 'body' : the html table filled with the answer(s)
 *   questions and values; 'score', the matching score if several answer IDs
 *   are provided (in %) or FALSE if only one answer ID; and 'number' : the number of
 *   provided answer IDs. Or return FALSE if an error occurs.
 */
function limesurvey_sync_answ_import_answer($node, $ls_aid_s, $options = array('ls_merge' => TRUE, 'ls_only_answered' => FALSE, 'ls_ignore_x' => TRUE, 'ls_fields' => array()), $reset = FALSE) {
  // Inspired by LS : admin/browse.php, next to 'if ($subaction == "id")'.
  $dismatch_max_length_dispayed = 15;

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
  $ls_sid = $node->ls_sid;
  $ls_lang = $node->ls_lang;
  $surveyid = $ls_sid;
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $surveytable = limesurvey_sync_table('survey', $surveyid);
  $tokentable = limesurvey_sync_table('tokens', $surveyid);

  // Set the previous cb connection.
  $previous_db = limesurvey_sync_active_db_name();
  // switch to drupal for the t() function.
  limesurvey_sync_db_close();

  // Find languageS : one for the LimeSurvey datas and on for the Drupal t() function.
  // Set general language, the one from the node or the answers content type.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $lang_dp_to_import = ($options['ls_import_lang']) ? $options['ls_import_lang'] : limesurvey_sync_lang_convert($node->ls_lang);
  // Set the language for the LimeSurvey database
  $language = ls_answ_validlang_ls($node, $lang_dp_to_import);
  // Set the language for the Drupal t() function.
  $dp_langcode = ls_answ_validlang_dp_langcode($lang_dp_to_import);
  $t_lang = (!empty($dp_langcode)) ? array('langcode' => $dp_langcode) : array();
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_import');

  // Cache datas.
  static $fnames_cached = array();
  $fnames = array();
  if ($reset || empty($fnames_cached) || !isset($fnames_cached[$ls_sid][$language])) {
    if ($reset) {
      $fnames_cached = array();
    }
    // Load questions fields and properties.
    $fieldmap = limesurvey_sync_answ_create_field_map($surveyid, 'full', FALSE, FALSE, $language, $dp_langcode);

    // Cache datas.
    // Reformat questions fields and properties.
    foreach ($fieldmap as $field) {
      $question = $field['question'];
      $subquestion = '';
      if (isset($field['subquestion']) && $field['subquestion'] != '') {
        $subquestion = $field['subquestion'];
      }
      if (isset($field['subquestion1']) && isset($field['subquestion2'])) {
        $subquestion .= ' (' . $field['subquestion1'] . ':' . $field['subquestion2'] . ')';
      }
      if (isset($field['scale_id'])) {
        $question .= ' [' . $field['scale'] . ']';
      }
      $fnames_cached[$ls_sid][$language][] = array_merge(
        array(0 => $field['fieldname']),
        array(1 => $question),
        array('type' => $field['type']),
        array('subquestion' => $subquestion),
        array('group_name' => $field['group_name']),
        array('title' => $field['title'])
      );
    }
  }

  $fnames = $fnames_cached[$ls_sid][$language];

  // Create answer header if needed.
  if (is_numeric($ls_aid_s)) {
    $ls_aid_s = array($ls_aid_s => t('Answers', array(), $t_lang));
  }
  $head = array();
  $head[] = array('data' => t('Section', array(), $t_lang));
  $head[] = array('data' => t('Questions', array(), $t_lang));
  $head[] = array('data' => t('Extras', array(), $t_lang));
  // Count how many answers to analyze.
  $count_col = count($ls_aid_s);
  // Add a comparison colomn if many answers.
  if ($count_col > 1) {
    $head[] = array('data' => t('Matching', array(), $t_lang));
  }

  // Generate the $data variable containing questions and all answers datas.
  $data = array();
  foreach ($ls_aid_s as $aid => $title_answer) {
    $head[] = array('data' => $title_answer);
    // Note that the data value is incremented by each anwser.
    $data = limesurvey_sync_answ_import_single_answer($surveyid, $language, $aid, $fnames, $data, $options, $dp_langcode);
  }

  // Prepare datas into the rows variable for an html table.
  $rows = array();
  $score = $total_score = 0;
  $empty_cell = array(trim(t("No answer", array(), $t_lang) . ' []'), '', NULL);
  $previous_gn = $previous_q = $previous_sq = 10000;
  $new_tree = FALSE;

  // Extract datas.
  foreach ($data as $row_number => $cell) {
    // Bypass the "tree" key which stores informations for rowspan (no datas
    // answers). Notice the strict comparison.
    if ($row_number === 'tree') {
      continue;
    }

    // Initialization.
    $array_attribute = $matched_data = $matching_attribute = array();
    $dismatch = FALSE;
    $matching = '';

    // Bypass the non answered questions depending on display options.
    if (($options['ls_merge'] && $options['ls_only_answered']) &&
    ($data['tree'][$cell['ngroup_name']][$cell['nquestion']][$cell['nsubquestion']]['count_answered'] == 0
    || $data['tree'][$cell['ngroup_name']][$cell['nquestion']]['count_answered'] == 0
    || $data['tree'][$cell['ngroup_name']]['count_answered'] == 0
    )) {
      continue;
    }

    // Display the questions :
    $empty_answers = (array_diff(array_unique($cell['answers']), $empty_cell) == array());

    // If there are many answers : calculate the matching score.
    if ($count_col > 1) {
      // Don't compare 'Text display' questions and standard ls_fiels neither.
      $ignore_fields = ($cell['type'] == "X" || in_array($cell['fieldname'], $standard_ls_fields));
      // Ignore empty answers for the comparison : This is an arbitrary choice,
      // it improves the score sensibility.
      if (!$empty_answers && !$ignore_fields) {
        $total_score += $count_col;
      }
      // There are many answers :
      $unique_data = array_unique($cell['answers']);
      $count_unique = count($unique_data);
      if ($ignore_fields) {
        $matching = '';
        $array_attribute = array();
        $matching_attribute = array();
      }
      elseif ($count_unique == 1) {
        // All answers are matching !
        if ( !in_array($cell['answers'][0], $empty_cell)) {
          // And there are no empty answer.
          $matching =  t('match', array(), $t_lang);
          $array_attribute = array();
          $matching_attribute = array('class' => 'matched');
          $score += $count_col;
        }
      }
      elseif ($count_unique == $count_col) {
        // All answers are differents !
        $matching = t('not match', array(), $t_lang);
        $array_attribute = array();
        $matching_attribute = array('class' => 'notmatched');
      }
      else {
        // many different answers values.
        $matching =  t('partially match', array(), $t_lang);
        $matched_data = array_diff_assoc($cell['answers'], $unique_data);
        $matching_attribute = array('class' => 'dismatched');
        $dismatch = TRUE;
        $matched_data_count = array_count_values($cell['answers']);
        $n_matched_options = count(array_unique($matched_data));
        // Sort answers, the most frequent first.
        arsort($matched_data_count);
        $answer_summary = '';
        $dismatch_summary = array();
        foreach ($matched_data_count as $answer => $count) {
          $answer_summary = (in_array($answer, $empty_cell, TRUE)) ? t("No answer", array(), $t_lang) : ((drupal_strlen($answer) > $dismatch_max_length_dispayed) ? drupal_substr($answer, 0, $dismatch_max_length_dispayed) . ' [...]' : $answer);
          $dismatch_summary[] =  t('%answer_summary => @score%', array('%answer_summary' => $answer_summary, '@score' => number_format($count / $count_col * 100, 0, ',', ' ')), $t_lang);
          // Arbitray choice for the score algorithm : increment the score on
          // matching answers only.
          if ($count > 1 ) {
            $score += $count;
          }
        }
        $matching .= theme('item_list', array('items' => $dismatch_summary));
      }
    }

    if (!$options['ls_merge']) {
      // Show all questions or only the non empty answers.
      if (!$options['ls_only_answered'] || !$empty_answers) {
        $rows[(int) $row_number][] = array('data' => $cell['group_name']);
        $rows[(int) $row_number][] = array('data' => $cell['question']);
        $rows[(int) $row_number][] = array('data' => $cell['subquestion']);
      }
    }
    else {
      // Merge questions, subquestions and extra for greater readibility.
      // Use the "tree" key to calculate the rowspan value.
      $new_tree = FALSE;
      $count_option =  ($options['ls_only_answered']) ? 'count_answered' : 'count_all';
      if ($previous_gn != $cell['ngroup_name']) {
        $rowspan = ($data['tree'][$cell['ngroup_name']][$count_option] > 1) ? array('rowspan' => $data['tree'][$cell['ngroup_name']][$count_option]) : array();
        $rows[$row_number][] = array_merge(array('data' => $cell['group_name']), $rowspan );
        $new_tree = TRUE;
      }
      if ($previous_q != $cell['nquestion'] || $new_tree) {
        $rowspan = ($data['tree'][$cell['ngroup_name']][$cell['nquestion']][$count_option] > 1) ? array('rowspan' => $data['tree'][$cell['ngroup_name']][$cell['nquestion']][$count_option]) : array();
        $rows[$row_number][] = array_merge(array('data' => $cell['question']), $rowspan );
        $new_tree = TRUE;
      }
      if ($previous_sq != $cell['nsubquestion'] || $new_tree) {
        $rowspan = ($data['tree'][$cell['ngroup_name']][$cell['nquestion']][$cell['nsubquestion']][$count_option] > 1) ? array('rowspan' => $data['tree'][$cell['ngroup_name']][$cell['nquestion']][$cell['nsubquestion']][$count_option]) : array();
        $rows[$row_number][] = array_merge(array('data' => $cell['subquestion']), $rowspan );
      }

      $previous_gn = $cell['ngroup_name'];
      $previous_q = $cell['nquestion'];
      $previous_sq = $cell['nsubquestion'];
    }

    // Display the comparison colomn.
    if ($count_col > 1) {
      $rows[(int) $row_number][] = array_merge(array('data' => $matching), $matching_attribute);
    }

    // Iterate each answer to the question.
    foreach ($cell['answers'] as $single_answer) {
      if ($dismatch) {
        // Theme the machting datas on dismatched answers.
        $theme_answers = (in_array($single_answer, $matched_data)) ? array('class' => 'matched') : array('class' => 'notmatched');
      }
      else {
        $theme_answers = array();
      }
      if (!$options['ls_only_answered'] || !$empty_answers) {
        // Or only non empty answer.
        $rows[(int) $row_number][] =  array_merge(array('data' => $single_answer), $theme_answers);
      }
    }
  }

  // At the end, theme the result and format the score.
  if (empty($data)) {
    $output = format_plural($count_col, 'The answer is empty.', 'All @count answers are empty.', array(), $t_lang);
    $final_score = FALSE;
  }
  else {
    $output = theme('table', array('header' => $head, 'rows' => $rows));
    $final_score = ($score != 0) ? number_format($score / $total_score * 100, 1, ',', ' ') : 0;
    if ($final_score == 100.0) {
      // To avoid the coma.
      $final_score = 100;
    }
  }

  // make sure to return to the previous function.
  limesurvey_sync_db_close($previous_db);
  return array(
    'body' => $output,
    'score' => $final_score,
    'number' => $count_col,
  );
}

/**
 * Return an php array variable containing questions and answers informations.
 *
 * Format questions and answers informations of the answer ID provided,
 * returning them into a multidimensional array. Notice that function can be
 * called several times, incrementing the data variable by each new id answer
 * provided. This code is an adaptation from the LS file amin/browse.php, from
 * the line beguining by the comment 'SHOW INDIVIDUAL RECORD'.
 *
 * @param $surveyid
 *   A numeric. The survey ID reference.
 * @param $language
 *   A string. The answer language value.
 * @param $id
 *   A numeric. The answer ID.
 * @param $fnames
 *   An array containing the survey questions fields and properties.
 * @param $data
 *   A array. the return array of this function, used to incremente the
 *   generate variable.
 * @param $options
 *   An array containg the keys 'ls_merge', 'ls_only_answered', 'ls_ignore_x'
 *   filled with a boolean value and the key 'ls_fields' storing fields to
 *   show in the html table (available values are 'id', 'submitdate',
 *   'lastpage', 'startlanguage', 'token', 'datestamp', 'startdate', 'ipaddr',
 *   'refurl'). It alters the html result.
 * @param $dp_langcode
 *   A string. The Drupal language code.
 *
 * @return
 *   An array. a multidimentional array storings all datas (questions and
 *   answers).
 */
function limesurvey_sync_answ_import_single_answer($surveyid, $language, $id, $fnames, $data = array(), $options = array('ls_merge' => TRUE, 'ls_only_answered' => FALSE, 'ls_ignore_x' => TRUE, 'ls_fields' => array()), $dp_langcode = NULL) {

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $surveytable = limesurvey_sync_table('survey', $surveyid);
  $tokentable = limesurvey_sync_table('tokens', $surveyid);

  // Set the previous cb connection.
  $previous_db = limesurvey_sync_active_db_name();
  // Close it for the t() function
  limesurvey_sync_db_close();

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_import');
  $dateformatdetails['phpdate'] = 'j/n/Y - H:i';
  $empty_cell = array(trim(t("No answer") . ' []'), '', NULL, ' ');

  // Load file for the ls_answ_display_ls_standard_fields() function.
  module_load_include('inc', 'ls_answ', 'ls_answ');
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

  $nfncount = count($fnames) -1;

  $id = ($id < 1) ? 1 : $id;
  $row_number = 0;

  $previous_gn = $previous_q = $previous_sq = 100; // No 0.
  $gn = $q = $sq = 0;
  $new_tree = FALSE;
  $fix_db_case = (double) VERSION < 7.14;

  limesurvey_sync_db_open();
  $idresult = db_query('SELECT surveys.* FROM {' . $surveytable . '} AS surveys WHERE id = :id', array(':id' => $id));
  $idrow = $idresult->fetchAssoc();
  // No iteration (while) because id is a primary key, only one row will be
  // returned and the limesurvey_sync_answ_getextendedanswer() function below
  // needs the drupal connection in order to translate strings.
  limesurvey_sync_db_close();
  if ($idrow) {

    for ($i = 0; $i < $nfncount + 1; $i++) {

      // Ignore 'Text display' now to avoid rowspan errors.
      if ($options['ls_ignore_x'] && $fnames[$i]['type'] == "X") {
        continue;
      }

      // Ignore ls fields : answer ID, token, stardate, etc...
      if (in_array($fnames[$i][0], $standard_ls_fields) && !in_array($fnames[$i][0], $options['ls_fields'])) {
        continue;
      }

      // D7 return the lowercase colomn name : adapt here.
      // EDIT : Drupal 7.14 returns the sensitive case colomn name now.
      $index = ($fix_db_case) ? drupal_strtolower($fnames[$i][0]) : $fnames[$i][0];
      $cell_data = '';
      if (isset($fnames[$i]['type']) && $fnames[$i]['type'] == "|") {
        // This is a file : print the file name.
        // @TODO : Add a CCK field file to make it downloadable.
        $cell_data = htmlspecialchars(strip_tags(limesurvey_sync_answ_strip_javascript($fnames[$i]['index'])));
      }
      elseif (!in_array($fnames[$i][0], $standard_ls_fields) && !empty($idrow[$index])) {
        // @TODO : Performance may be improved on caching the getextendedanswer()
        // function or filtrering it with something like
        // && trim($idrow[$fnames[$i][0]]) != '' in special cases.
        $cell_data = htmlspecialchars(strip_tags(limesurvey_sync_answ_strip_javascript(limesurvey_sync_answ_getextendedanswer($fnames[$i][0], $idrow[$index], $surveyid, $language, $dp_langcode, '', $dateformatdetails['phpdate']))), ENT_QUOTES);
      }
      elseif (in_array($fnames[$i][0], $standard_ls_fields) && !empty($idrow[$index])) {
        $cell_data = ls_answ_display_ls_standard_fields($fnames[$i][0], htmlspecialchars(strip_tags(limesurvey_sync_answ_strip_javascript($idrow[$index])), ENT_QUOTES), $dp_langcode);
      }
      else {
        $cell_data = htmlspecialchars(strip_tags(limesurvey_sync_answ_strip_javascript($idrow[$index])), ENT_QUOTES);
      }
      $data[$row_number]['question'] = strip_tags(limesurvey_sync_answ_strip_javascript($fnames[$i][1]));
      $data[$row_number]['fieldname'] = strip_tags(limesurvey_sync_answ_strip_javascript($fnames[$i][0]));
      // Note : the [] for storing multiple answers.
      $data[$row_number]['answers'][] = trim($cell_data);

      $data[$row_number]['type'] = strip_tags(limesurvey_sync_answ_strip_javascript($fnames[$i]['type']));
      if (!$options['ls_only_answered'] && !$options['ls_merge']) {
        $data[$row_number]['group_name'] = strip_tags(limesurvey_sync_answ_strip_javascript($fnames[$i]['group_name']));
        $data[$row_number]['subquestion'] = strip_tags(limesurvey_sync_answ_strip_javascript($fnames[$i]['subquestion']));
      }
      else {
        $question = strip_tags(limesurvey_sync_answ_strip_javascript($fnames[$i][1]));

        // Handle tree group name.
        if ($previous_gn != $fnames[$i]['group_name']) {
          $gn++;
          $q = $sq = 0;
          $data['tree'][$gn]['name'] = $fnames[$i]['group_name'];
          $data['tree'][$gn]['count_all'] = $data['tree'][$gn]['count_answered'] = 0;
          $new_tree = TRUE;
        }
        // Handle tree question.
        if ($previous_q != $question || $new_tree) {
          $q++;
          $sq = 0;
          $data['tree'][$gn][$q]['name'] = $question;
          $data['tree'][$gn][$q]['count_all'] = $data['tree'][$gn][$q]['count_answered'] = 0;
          $new_tree = TRUE;
        }
        // Handle tree subquestion.
        if ($previous_sq != $fnames[$i]['subquestion'] || $new_tree) {
          $sq++;
          $data['tree'][$gn][$q][$sq]['name'] = $fnames[$i]['subquestion'];
          $data['tree'][$gn][$q][$sq]['count_all'] = $data['tree'][$gn][$q][$sq]['count_answered'] = 0;
        }

        // Handle tree answer.
        if (!isset($data['tree'][$gn][$q][$sq]['answer'])
        || ((! in_array($cell_data, $empty_cell))
        && !in_array($cell_data, $data['tree'][$gn][$q][$sq]['answer']))
        ) {
          $data['tree'][$gn][$q][$sq]['answer'][$sq] = $cell_data;
        }


        // Handle groupname and subquestion.
        $data[$row_number]['group_name'] = $fnames[$i]['group_name'];
        $data[$row_number]['subquestion'] = strip_tags(limesurvey_sync_answ_strip_javascript($fnames[$i]['subquestion']));
        $data[$row_number]['ngroup_name'] = $gn;
        $data[$row_number]['nquestion'] = $q;
        $data[$row_number]['nsubquestion'] = $sq;
        // Count all duplicate values :
        $data['tree'][$gn]['count_all']++;
        $data['tree'][$gn][$q]['count_all']++;
        $data['tree'][$gn][$q][$sq]['count_all']++;

        // Count all duplicate values for answered answers :
        $data['tree'][$gn][$q][$sq]['count_answered'] = count(array_diff($data['tree'][$gn][$q][$sq]['answer'], $empty_cell));
        $data['tree'][$gn][$q]['count_answered'] += $data['tree'][$gn][$q][$sq]['count_answered'];
        $data['tree'][$gn]['count_answered'] +=  $data['tree'][$gn][$q][$sq]['count_answered'];

        // Initialize for the next loop.
        $previous_gn = $fnames[$i]['group_name'];
        $previous_q = $question;
        $previous_sq = $fnames[$i]['subquestion'];
        $new_tree = FALSE;
      }
      $row_number++;
    }
  }
  limesurvey_sync_db_close($previous_db);
  return ($data);
}

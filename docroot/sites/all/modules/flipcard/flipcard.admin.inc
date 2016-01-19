<?php

/**
 * Admin settings.
 */
function flipcard_admin() {

  $form['flipcard_words_per_quiz'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of questions'),
    '#default_value' => variable_get('flipcard_words_per_quiz', 10),
    '#description' => t("Select the number of questions in a flipcard quiz.")
  );
  
  return system_settings_form($form);
}

/**
 * Page callback for admin page
*/
function flipcard_prepare_nodes() {
  $form = array();

  $form['info'] = array(
    '#item' => 'Prepare the nodes for flipcard use',
    '#description' => 'This preparation phase is needed to set up the nodes which are being tracked.'
  );

  $form['uid'] = array(
    '#type' => 'textfield',
    '#title' => t('Uid of user for whom results table should be initialised.'),
    '#required' => FALSE,
    '#default_value' => "1",
  );

  $form['prepare_nodes'] = array(
    '#type' => 'submit',
    '#value' => t('Prepare Nodes'),
    '#weight' => 5
  );

  return $form;
}

/**
 * Implements hook_variable_info().
 * 
 * See http://www.lullabot.com/blog/article/module-monday-variable
 */
function flipcard_variable_info() {
  // Change at: admin/config/system/variable/edit/flipcard_words_per_quiz
  // Use: variable_get('flipcard_words_per_quiz');
  $variables['flipcard_words_per_quiz'] = array(
    'title' => t('Number of words in a quiz'),
    'description' => t('Specify the number of words in a quiz.'),
    'default' => 10,
    'type' => 'number',
    'group' => 'Flipcard',
  );
  
  $variables['lingopolo_subsite_language'] = array(
    'title' => t('Language of subsite'),
    'description' => t('Specify the name of the subsite language.'),
    'default' => 'LANGUAGE NOT SET',
    'group' => 'Flipcard',
  );
  
  return $variables;
}

/**
 * Get the highest possible node id
 */
function flipcard_get_highest_nid() {
  $sql = "SELECT nid FROM {node} ORDER BY nid DESC";

  $result = db_query_range($sql, 0, 1);
  $nid = $result->fetchField();

  return $nid;
}

/**
 * Get the next highest user id
 *
 * @param $last_id
 *   The uid which the returned value should be higher than
 */
function flipcard_get_next_highest_uid($last_id) {
  $uid = $last_id;

  $sql = "SELECT uid FROM {users} WHERE uid > :last_id ORDER BY uid ASC";

  $result = db_query_range($sql, 0, 1, array(':last_id' => $last_id));

  if ($result) {
    $uid = $result->fetchField();
  }

  return $uid;
}

/**
 * Compare two values of shown cards (for use in sorting table)
 */
function compare_shown($a, $b) {
  return $a['shown'] < $b['shown'];
}

/**
 * Display a summary of student activity
 */
function flipcard_student_summary() {
  $sql = "SELECT uid, name, access FROM {users} ORDER BY access DESC";

  $result = db_query($sql);

  $html = "";
  if ($result->rowCount() > 0) {
    foreach ($result as $row) {
      $user_stats = flipcard_get_user_stats($row->uid);

      $dt = new DateTime('@' . $row->access);  // convert UNIX timestamp to PHP DateTime
      $access_time = $dt->format('Y-m-d H:i:s'); // output = 2012-08-15 00:00:00

      if ($user_stats->shown > 0) {
        $rows[] = array('uid' => $row->uid,
          'name' => l($row->name, 'user/' . $row->uid),
          'access' => $access_time,
          'shown' => $user_stats->shown,
          'correct' => $user_stats->correct);
      }
    }
  }

  // sort alphabetically by name
  //   usort($rows, 'compare_shown');

  $header = array('uid', 'Name', 'Access', 'Shown', 'Correct');

  $variables = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => false,
    'empty' => ''
  );

  $html .= theme_table($variables);

  return $html;
}

/**
 * Get the stats for a particular user
 */
function flipcard_get_user_stats($uid) {
  $sql = "SELECT SUM(shown) AS shown, SUM(correct) AS correct " .
    "FROM {flipcard_cards} " .
    "WHERE uid = :uid";

  $result = db_query($sql, array(':uid' => $uid));
  $row = $result->fetchObject();

  return $row;
}

/**
 * Get the number of flipcard nodes for the given user
 */
function flipcard_get_flipcard_count($uid) {
  $sql = "SELECT COUNT(nid) AS count " .
    "FROM {flipcard_cards} " .
    "WHERE uid = :uid";

  $result = db_query($sql, array(':uid' => $uid));
  $flipcard_count = $result->fetchField();

  return $flipcard_count;
}

/**
 * Implements hook_node_delete.
 *
 * When a node is deleted we need to make sure that it is also removed from the flipcard_cards
 */
function flipcard_node_delete($node) {
  flipcard_remove_node_stats($node->nid);
}

/**
 * Remove a particular node entry from the flipcard_cards table
 *
 * @param nid
 *   Node id of the node to remove
 */
function flipcard_remove_node_stats($nid) {
  db_delete('flipcard_cards')
  ->condition('nid', $nid)
  ->execute();
}

/**
 * Implements hook_user_delete().
 *
 * @param $account
 */
function flipcard_user_delete($account) {
  db_delete('flipcard_cards')
  ->condition('uid', $account->uid)
  ->execute();
}

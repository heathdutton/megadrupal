<?php

/**
 * @file
 * User Menu functions
 */

/**
 * Manage Planet Feeds
 */
function easyplanet_feeds_user($userid, $lid=NULL) {
  if ($userid == NULL) {
    global $user;
    $userid = $user->uid;
  }

  if (intval($lid) > 0) {
    $edit = db_select('easyplanet_feeds', 'uf')
      ->fields('uf')
      ->condition('uid', $userid)
      ->condition('lid', $lid)
      ->execute()
      ->fetch();
    $form = drupal_get_form('easyplanet_feeds_user_edit', $edit, $userid);
  }
  else {
    $form = drupal_get_form('easyplanet_feeds_user_edit', (object)array(), $userid);
    $form['title'] = array(
                           '#markup' => '<h2>User Feeds</h2>',
                           );
    $form['feed_list'] = array(
                               '#markup' => easyplanet_feeds_user_list($userid)
                               );
  }

  return $form;
}

/**
 * Build table of feeds
 */
function easyplanet_feeds_user_list($userid) {
  $query = db_select('easyplanet_feeds', 'uf');
  $query->join('aggregator_feed', 'af', 'af.fid=uf.fid');

  $feeds = $query
    ->fields('uf')
    ->fields('af', array('title'))
    ->condition('uid', $userid)
    ->execute();

  $rows = array();

  if (variable_get('easyplanet_user_edit', TRUE)) {
    $headers = array('Feed Title', 'Disabled', 'Frozen', 'Edit');
  }
  else {
    $headers = array('Feed Title', 'Disabled', 'Frozen');
  }

  foreach ($feeds as $feed) {
    $lid = strval($feed->lid);
    $disabled = ($feed->disabled == TRUE) ? 'X' : '';
    $frozen = ($feed->frozen == TRUE) ? 'X' : '';
    $edit = l(t('edit'), 'user/' . $userid . '/easyplanet/feeds/'. $lid);
    if (variable_get('easyplanet_user_edit', TRUE)) {
      array_push($rows, array($feed->title, $disabled, $frozen, $edit));
    }
    else {
      array_push($rows, array($feed->title, $disabled, $frozen));
    }
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add/Delete/Update Feeds
 */
function easyplanet_feeds_user_edit($form, &$form_state, $edit, $user) {
  // Check permissions from Settings
  if (!variable_get('easyplanet_user_edit', TRUE) && !variable_get('easyplanet_user_add', TRUE)) return NULL;

  $aggregator = db_select('aggregator_feed', 'af')->fields('af', array('title', 'url'))
    ->condition('fid', $edit->fid)->execute()->fetch();
  // $aggregator = db_fetch_object(db_query_range('SELECT title, url FROM {aggregator_feed} WHERE fid="%d"', array($edit['fid']), 0, 1));
  $feed_title = $aggregator->title;
  $feed_url = $aggregator->url;

  $form = array();

  $form['feed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feed'),
    '#collapsible' => TRUE,
    '#collapsed' => ($edit->lid > 0) ? FALSE : TRUE,
    '#tree' => FALSE,
  );
  $form['feed']['lid'] = array(
    '#type' => 'hidden',
    '#value' => $edit->lid,
  );
  $form['feed']['feed_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed Title'),
    '#default_value' => $feed_title,
    '#description' => t('The Title of the feed to be aggregated.'),
  );
  $form['feed']['feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed URL'),
    '#default_value' => $feed_url,
    '#description' => t('The URL of the feed to be aggregated.'),
  );
  $form['feed']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#default_value' => $edit->disabled,
    '#description' => t('Whether or not the blog will be aggregated. (unchecke overrides frozen)'),
  );

  if (variable_get('easyplanet_user_add', TRUE) && $edit->lid == 0) {
    $form['feed']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Add',
    );
  }
  elseif (variable_get('easyplanet_user_edit', TRUE) && $edit->lid > 0) {
    $form['feed']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Update',
    );
  }

  if (variable_get('easyplanet_user_edit', TRUE) && $edit->lid > 0) {
    $form['feed']['delete'] = array(
      '#type' => 'submit',
      '#value' => 'Delete',
    );
  }

  return $form;
}

/**
 * Do the Add/Delete/Update Modification to Feed
 */
function easyplanet_feeds_user_edit_submit($form, &$form_state) {
  global $user;
  $uid = $user->uid;
  $op = $form_state['values']['op'];
  // TODO: refactor for sane objects
  // TODO: move validation out to validation function
  // TODO: merge with feeds_admin.inc (frozen is the only missing thing)
  $lid = $form_state['values']['lid'];
  $feed_url = $form_state['values']['feed_url'];
  $feed_title = $form_state['values']['feed_title'];
  $username = $form_state['values']['username'];
  $disabled = $form_state['values']['disabled'];
  $feed = db_select('easyplanet_feeds', 'uf')->fields('uf', array('fid'))
    ->condition('lid', $lid)
    ->execute()
    ->fetch();
  $fid = $feed->fid;

  // Check Valid URL
  if (!valid_url($feed_url, TRUE)) {
    form_set_error('url', t('The URL %url is invalid. Please enter a fully-qualified URL, ex. http://www.example.com/feed.xml.', array('%url' => $form_state['values']['url'])));
    return NULL;
  }

  // Check for duplicates
  if ($op != 'Delete' && easyplanet_check_dupes($feed_url, $feed_title, $fid)) return NULL;

  // Check that user hasn't reached max feeds
  if ($op != 'Delete' && !easyplanet_check_limit($uid, $op)) return NULL;

  // Update
  if ($op == 'Update') {
    db_update('easyplanet_feeds')
      ->fields(array(
                     "uid" => $uid,
                     "disabled" => $disabled))
      ->condition('fid', $fid)
      ->execute();

    db_update('aggregator_feed')
      ->fields(array(
                     "url" => $feed_url,
                     "title" => $feed_title))
      ->condition('fid', $fid)
      ->execute();

    drupal_set_message(t("Updated: $feed_title"));
  }

  // Delete
  if ($op == 'Delete') {
    db_delete('easyplanet_feeds')->condition('lid', $lid)->execute();
    db_delete('aggregator_item')->condition('fid', $fid)->execute();
    db_delete('aggregator_feed')->condition('fid', $fid)->execute();
    db_delete('aggregator_category_feed')->condition('fid', $fid)->execute();
    drupal_set_message(t("Deleted: $feed_title"));
  }

  // Add
  if ($op == 'Add') {
    $fid = db_insert('aggregator_feed')
      ->fields(array(
                     'title' => $feed_title,
                     'url' => $feed_url,
                     'link' => '',
                     'refresh' => 900,
                     'block' => 5,
                     'description' => '',
                     'image' => ''))
      ->execute();
    // Assign category, if set.
    if ($cid = variable_get('easyplanet_feed_category', 0)) {
      db_insert('aggregator_category_feed')
        ->fields(array(
                       'fid' => $fid,
                       'cid' => $cid))
        ->execute();
    }
    db_insert('easyplanet_feeds')
      ->fields(array(
                     'fid' => $fid,
                     'uid' => $uid,
                     'disabled' => $disabled))
      ->execute();
    drupal_set_message(t("Added new feed: $feed_title"));
  }

  // Pull feed content if option is enabled
  if ($op != 'Delete') {
    if (variable_get('easyplanet_cron_edit', 0)) {
      module_load_include('module', 'aggregator', 'aggregator');
      $feed = db_select('aggregator_feed', 'af')
        ->fields('af')
        ->condition('fid', $fid)
        ->execute()
        ->fetch();
      aggregator_refresh($feed);
    }
  }

  drupal_goto('user/' . $user->uid . '/easyplanet/feeds/');
}

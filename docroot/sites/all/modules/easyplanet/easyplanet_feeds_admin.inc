<?php

/**
 * @file
 * Adminsitrative Menu functions
 */

/**
 * Manage Planet Feeds
 */
function easyplanet_feeds_admin($lid) {
  if (intval($lid) > 0) {
    $edit = db_select('easyplanet_feeds', 'uf')
      ->fields('uf')
      ->condition('lid', $lid)
      ->execute()->fetch();
    $form = drupal_get_form('easyplanet_feeds_admin_edit', $edit, TRUE);
  }
  else {
    $form = drupal_get_form('easyplanet_feeds_admin_edit', $edit, FALSE);
    $form['title'] = array(
                           '#markup' => '<h2>User Feeds</h2>'
                           );
    $form['feed_list'] = array(
                               '#markup' => easyplanet_feeds_admin_list()
                               );
  }
  return $form;
}

/**
 * Build table of feeds
 */
function easyplanet_feeds_admin_list() {
  $query = db_select('easyplanet_feeds', 'uf');
  $query->join('aggregator_feed', 'af', 'uf.fid=af.fid');
  $query->join('users', 'u', 'uf.uid=u.uid');

  $feeds = $query->fields('uf')
    ->fields('af', array('title'))
    ->fields('u', array('name'))
    ->orderBy('uf.uid', 'DESC')
    ->execute();

  $rows = array();

  $headers = array('User', 'Feed Title', 'Disabled', 'Frozen', 'Edit');

  foreach ($feeds as $feed) {
    $lid = strval($feed->lid);
    $username = $feed->name;
    $disabled = ($feed->disabled == TRUE) ? 'X' : '';
    $frozen = ($feed->frozen == TRUE) ? 'X' : '';
    $edit = l(t('edit'), 'admin/config/system/easyplanet/feeds/'. $lid);
    $title = $feed->title;
    array_push($rows, array($username, $title, $disabled, $frozen, $edit));
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add/Delete/Update Feeds
 */
function easyplanet_feeds_admin_edit($form, &$form_state, $edit, $individual = FALSE) {
  if (!$edit) {
    $edit = (object)array();
  }
  if ($individual) {
    $query = db_select('users', 'u');
    $query->join('easyplanet_feeds', 'uf', 'uf.uid=u.uid');
    $user = $query->fields('u', array('name'))
      ->condition('lid', $edit->lid)
      ->execute()->fetch();
    $username = $user->name;
  }
  $aggregator = db_select('aggregator_feed', 'af')
    ->fields('af', array('title', 'url'))
    ->condition('fid', $edit->fid)
    ->execute()->fetch();
  $feed_title = $aggregator->title;
  $feed_url = $aggregator->url;

  $form = array();

  $form['feed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add New Feed'),
    '#collapsible' => TRUE,
    '#collapsed' => !$individual,
    '#tree' => FALSE,
  );
  $form['feed']['lid'] = array(
    '#type' => 'hidden',
    '#value' => $edit->lid,
  );
  $form['feed']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#default_value' => $username,
    '#description' => t('User the blog will belong to.'),
    '#autocomplete_path' => 'user/autocomplete',
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
    '#maxlength' => 255,
    '#description' => t('The URL of the feed to be aggregated.'),
  );
  $form['feed']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#default_value' => $edit->disabled,
    '#description' => t('Whether or not the blog will be aggregated. (unchecke overrides frozen)'),
  );
  $form['feed']['frozen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Frozen'),
    '#default_value' => $edit->frozen,
    '#description' => t('Whether or not the blog will be aggregated. (unchecked overrides enabled) -- User can not modify this'),
  );

  $form['feed']['submit'] = array(
    '#type' => 'submit',
    '#value' => $individual ? 'Update' : 'Add'
  );

  if ($edit->lid > 0) {
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
function easyplanet_feeds_admin_edit_submit($form, &$form_state, $userid = NULL) {
  $op  = $form_state['values']['op'];
  // TODO: refactor for sane objects
  // TODO: move validation out to validation function
  $lid = $form_state['values']['lid'];
  $feed_url = $form_state['values']['feed_url'];
  $feed_title = $form_state['values']['feed_title'];
  $username = $form_state['values']['username'];
  $disabled = $form_state['values']['disabled'];
  $frozen = $form_state['values']['frozen'];
  $feed = db_select('easyplanet_feeds', 'uf')->fields('uf', array('fid'))
    ->condition('lid', $lid)
    ->execute()
    ->fetch();
  $fid = $feed->fid;

  // Get uid from  username
  $user = db_select('users', 'u')
    ->fields('u', array('uid'))
    ->condition('name', $username)
    ->execute()
    ->fetch();

  $uid = $user->uid;
  if (!isset($uid)) {
    form_set_error('user_404', t('Unknown user %username.', array('%username' => $username)));
    return NULL;
  }

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
                     "disabled" => $disabled,
                     "frozen" => $frozen))
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
                     'disabled' => $disabled,
                     'frozen' => $frozen))
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

  drupal_goto("admin/config/system/easyplanet/feeds");
}

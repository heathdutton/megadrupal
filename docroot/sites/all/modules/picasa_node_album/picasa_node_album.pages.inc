<?php

/**
 * @file
 * Page callbacks for the picasa_node_album module
 */

/**
 * Form to add albums to a node
 */
function picasa_node_album_add($form, $form_state, $nid) {
  global $user;

  $form = array();
  $form['picasa'] = array(
    '#type' => 'fieldset',
    '#title' => t('Picasa Album Management')
  );

  // Set permission vars.
  $remove_any = user_access('remove any albums');
  $remove = $remove_any ? $remove_any : user_access('remove own albums');

  // Initialize the query.
  $query = db_select('picasa_node_album', 'pna')
      ->fields('pna', array('nid', 'uid', 'album_id', 'title'))
      ->condition('pna.nid', $nid);

  // Restrict access to only own albums if permission is not sufficient.
  if (!$remove_any) {
    $query->condition('pna.uid', $user->uid);
  }

  // Run query.
  $result = $query->execute();

  // Add items to the form if there are results
  if ($result->rowCount() > 0) {
  $form['picasa']['remove_album']['#tree'] = TRUE;
    foreach ($result as $record) {
      $account = user_load($record->uid);

      if ($remove) {
        $form['picasa']['remove_album'][$record->album_id] = array(
          '#type' => 'checkbox',
          '#title' => t('Remove Album: %title (posted by %user)', array('%title' => $record->title, '%user' => $account->name))
        );
      }
      else {
        $form['picasa']['remove_album'][$record>album_id] = array(
          '#markup' => t('Album: %title (posted by %user)', array('%title' => $record->title, '%user' => $account->name))
        );
      }
    }
  }

  // Setup user account.
  $google_user = _picasa_node_album_get_user_google_account($user->uid);

  if (empty($google_user)) {
    $form['picasa']['google_user'] = array(
      '#markup' => '<div>' . l(t('Setup your default Picasa account.'), 'user/' . $user->uid . '/edit/picasa-node-album', array('query' => array('destination' => $_GET['q']))) . '</div>'
    );
  }

  $limit = variable_get('picasa_node_album_captions', 0);
  if (!empty($google_user)) {

    $form['picasa']['current_account'] = array(
      '#markup' => t('Picasa Account: <b>%gu</b>', array('%gu' => $google_user))
    );

    $form['picasa']['picasa_album'] = array(
      '#type' => 'textfield',
      '#title' => t('Add Picasa Album'),
      '#description' => t('Start typing the name of the Picasa album here.'),
      '#size' => 30,
      '#autocomplete_path' => 'ajax/picasa_albums'
    );

    $form['picasa']['submit'] = array('#type' => 'submit', '#value' => t('Update Albums'));
  }

  $form['picasa']['nid'] = array(
    '#type' => 'hidden',
    '#value' => $nid
  );

  $form['picasa']['uid'] = array(
    '#type' => 'hidden',
    '#value' => $user->uid
  );

  $google_user_field = _picasa_node_album_get_google_user_field();
  if ($google_user_field['#type'] != 'hidden') {
    $form['account'] = array(
      '#type' => 'fieldset',
      '#title' => t('Picasa Account')
    );

    $form['account']['google_user'] = $google_user_field;

    $form['account']['switch'] = array(
    '#type' => 'submit',
    '#value' => t('Go')
    );
  }
  else {
    $form['account']['current_account'] = NULL;
  }

  $form['#validate'][] = 'picasa_node_album_validate_handler';
  $form['#submit'][] = 'picasa_node_album_submit_handler';

  return $form;
}

/**
 * submit handler for node form
 */
function picasa_node_album_validate_handler(&$form, $form_state) {
  global $user;

  if (isset($form_state['values']['google_user'])) {
    $google_user = _picasa_node_album_get_user_google_account($user->uid);

    if ($google_user == $form_state['values']['google_user']) {
      form_set_error('google_user', t('%user is already the current Picasa account.', array('%user' => $google_user)));
      return;
    }

    $exclude_accounts = variable_get('picasa_node_album_exclude_accounts', '');
    $exploded = explode("\n", $exclude_accounts);

    if (count($exploded) > 0 && $exploded[0] != '') {
      $array = array();
      // Create a new array of all lower case values.
      foreach ($exploded as $each) {
        $array[] = drupal_strtolower($each);
      }

      // Compare lower case versions of the strings.
      if (in_array(drupal_strtolower($form_state['values']['google_user']), $array)) {
        form_set_error('google_user', t('You may not use this account.'));
        return;
      }
    }
  }
}

/**
 * submit handler for node form
 */
function picasa_node_album_submit_handler(&$form, $form_state) {
  global $user;

  // Load the node.
  $node = node_load($form_state['values']['nid']);

  // Process removals.
  if (isset($form_state['values']['remove_album']) && count($form_state['values']['remove_album']) > 0) {

    foreach ($form_state['values']['remove_album'] as $id => $value) {
      if ($value == 1) {
        db_query("DELETE FROM {picasa_node_album} WHERE nid = :nid AND album_id = :album_id", array(':nid' => $node->nid, ':album_id' => $id));
        drupal_set_message(t('Album removed successfully.'));
      }
    }
  }

  // Add an album.
  if (isset($form_state['values']['picasa_album'])) {

    // Get an array of albums
    $albums = _picasa_node_album_get_albums();

    if ($album_id = array_search($form_state['values']['picasa_album'], $albums)) {
      // Make sure album doesn't already exist
      $result = db_select('picasa_node_album', 'pna')
        ->fields('pna', array('nid'))
        ->condition('pna.nid', $node->nid)
        ->condition('pna.album_id', $album_id)
        ->execute();

      if ($result->rowCount() > 0) {
        form_set_error('picasa_album', t('That album already exists in this node.'));
        return;
      }
      // Insert record.
      $google_user = _picasa_node_album_get_user_google_account($user->uid);
      $result = db_query("INSERT INTO {picasa_node_album} (nid, uid, google_user, album_id, title, images) VALUES (:nid, :uid, :google_user, :album_id, :title, '')",
                          array(':nid' => $node->nid, ':uid' => $user->uid, ':google_user' => $google_user, ':album_id' => $album_id, ':title' => $form_state['values']['picasa_album']));
      drupal_set_message(t('Album added successfully.'));
    }
  }

  // Handle user switch
  if (isset($form_state['values']['google_user']) && !empty($form_state['values']['google_user'])) {
    picasa_node_album_user_edit_submit($form, $form_state);
  }
}

/**
 * Callback for user/%/edit/picasa
 */
function picasa_node_album_user_edit($form, $form_state, $uid) {
  // Accommodate 'me' module
  if ($uid == 'me') {
    global $user;
    $uid = $user->uid;
  }

  // Load account.
  $account = user_load($uid);

  // Load google user if one exists.
  $google_user = _picasa_node_album_get_user_google_account($account->uid);

  $form['text'] = array(
    '#markup' => t('Setting your Google/Picasa username here will allow you to add albums to content.')
  );

  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $account->uid
  );

  $form['google_user'] = _picasa_node_album_get_google_user_field();
  $form['google_user']['#default_value'] = $google_user;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set Picasa User')
  );

  $form['#validate'][] = 'picasa_node_album_validate_handler';

  return $form;
}

/**
 * Implements hook_submit().
 */
function picasa_node_album_user_edit_submit(&$form, $form_state) {
  // Query for a google username for this user.
  $result = db_select('picasa_node_album_users', 'pnau')
    ->fields('pnau', array('google_user'))
    ->condition('pnau.uid', $form_state['values']['uid'])
    ->execute();

  // Update or insert depending if a record exists for this user.
  if ($result->rowCount() > 0) {
    $record = $result->fetchObject();
    db_query("UPDATE {picasa_node_album_users} SET google_user = :gu WHERE uid = :uid", array(':gu' => $form_state['values']['google_user'], ':uid' => $form_state['values']['uid']));
  }
  else {
    db_query("INSERT INTO {picasa_node_album_users} (uid, google_user) VALUES (:uid, :gu)", array(':uid' => $form_state['values']['uid'], ':gu' => $form_state['values']['google_user']));
  }

  // Tell user the good news.
  drupal_set_message(t('Your Picasa settings have been updated.'));
}

/**
 * Autocomplete callback for get albums.
 */
function picasa_node_album_js_albums($string = '') {
  // Get albums for this account.
  $albums = _picasa_node_album_get_albums('text');
  $matches = array();
  if (is_array($albums)) {
    // Run through albums and append the $matches array if $string is found.
    foreach ($albums as $album) {
      if (stripos($album, $string) !== FALSE) {
        $matches[$album] = check_plain($album);
      }
    }
  }
  // Output JSON
  drupal_json_output($matches);
}

/**
 * Page callback for an individual album view
 */
function picasa_node_album_detail($nid) {
  $album_id = arg(3);

  if (empty($album_id)) {
    $albums = _picasa_node_album_get_list($nid);
    return theme('picasa_node_album_list', array('nid' => $nid, 'albums' => $albums));
  }

  // load up album
  $album = array();
  $cache_min = variable_get('picasa_node_album_cache_min', 60);

  $result = db_select('picasa_node_album', 'pna')
    ->fields('pna', array('uid', 'google_user', 'album_id', 'title', 'added', 'updated', 'images'))
    ->condition('nid', $nid)
    ->condition('album_id', $album_id)
    ->execute();

  if ($result->rowCount() > 0) {
    $record = $result->fetchObject();

    $account = user_load($record->uid);
    $album['postedby'] = t('posted by') . ' ' . l($account->name, 'user/' . $account->uid, array('html' => TRUE));
    $album['author'] = l($account->name, 'user/' . $account->uid, array('html' => TRUE));
    $album['title'] = $record->title;
    $album['uid'] = $account->uid;

    drupal_set_title($record->title);

    $cache_expires = $record->updated + $cache_min;
    if (empty($record->images) || time() > $cache_expires) {
      $album['images'] = _picasa_node_album_get_images($record->google_user, $record->album_id);
    }
    else {
      $album['images'] = unserialize($record->images);
    }

    $node_url = url('node/' . $nid);
    return theme('picasa_node_album_detail', array( 'nid' => $nid,
                                                    'node_url' => $node_url,
                                                    'title' => $album['title'],
                                                    'uid' => $album['uid'],
                                                    'author' => $album['author'],
                                                    'postedby' => $album['postedby'],
                                                    'images' => $album['images']
                                                  )
                                                );
  }

  drupal_not_found();
  return;
}

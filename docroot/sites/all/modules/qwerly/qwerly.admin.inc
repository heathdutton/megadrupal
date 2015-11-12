<?php
/*
 * @file
 * Renders the settings page for Qwerly
 * If action == save, it expects incoming data and redirects out.
 * If action == process_existing, it adds old users to the process queue
 * Called via qwerly_menu.
 */
function qwerly_settings_page($form, $form_state, $action = false) {
  //  Settings defined herein
  //
  //    api_key               The Qwerly API key to use
  //    cache_time            How long between cache updates
  //    update_indefinitely   Wether to indefinitely update the per-user cache or stop after 4
  //    max_requests          The maximum number of requests per cron-run
  //    new_email_action      What to do when a user alters his email - discard existing or append?

  $settings = qwerly_get_settings();

  $form = array(
    'logo' => array(
      '#markup' => '<h1 style="background:url(http://qwerly.com/images/logo.png) no-repeat left;padding-left:140px;height:47px;line-height:47px;">Settings</h1>'
    ),
    'api_key' => array(
      '#title' => t('API Key'),
      '#description' => t('The API Key provided by Qwerly at <a href="@url">@url</a>', array('@url' => 'http://dev.qwerly.com/apps/mykeys')),
      '#type' => 'textfield',
      '#default_value' => $settings->api_key,
    ),

    'cache_time' => array(
      '#title' => t('Cache Lifetime'),
      '#description' => t('How long in seconds should a cache be deemed \'alive\' before it is updated. Ideally 86400 ~ 1000000.'),
      '#type' => 'textfield',
      '#default_value' => $settings->cache_time,
    ),

    'update_indefinitely' => array(
      '#title' => t('Update caches indefinitely?'),
      '#description' => t('By default the Qwerly API module updates the cache for a user indefinitely, as new data may become available or existing data may be updated.'
                        . 'However, when there are large numbers of users in your database, this can cause problems. Untick this to stop updating after the initial data is recieved.'),
      '#type' => 'checkbox',
      '#return_value' => 1,
      // not sure why it has to be done this way, but it's the only way that works.
      '#attributes' => ($settings->update_indefinitely ? array('checked' => 'checked') : array()),
    ),

    'max_requests' => array(
      '#title' => t('Maximum number of requests per cron-run'),
      '#description' => t('The Qwerly API module updates it caches using a cron-job, which requests new data for all expired caches. However, with large numbers of users this can result in a lot of'
                        . ' requests per update. Set this number to > 0 to limit it. Note that if you limit it too severely, it can cause the update process to never clear the queue'),
      '#type' => 'textfield',
      '#default_value' => $settings->max_requests,
    ),

    'new_email_action' => array(
      '#title' => t('Action on updated email'),
      '#description' => t('Define what Qwerly API should do in the event of the user changing their local account email.'),
      '#type' => 'radios',
      '#options' => array(
        'clear' => t('Clear existing data and start afresh with this new email address. Loses all cached data!'),
        'append' => t('Keep existing data, appending new data to the existing dataset'),
      ),
      '#default_value' => $settings->new_email_action,
      '#title_display' => 'before',
    ),
    'actions' => array(
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save settings'),
      ),
      'process_existing' => array(
        '#markup' => l(t('Add all existing users to queue'), 'admin/config/people/qwerly/process_existing', array('attributes' => array('class' => array('button',),),))
      ),
    ),
  );

  $settings = (array) $settings;
  return $form;
}


function qwerly_settings_page_validate($form, $form_state) {
  $settings = qwerly_get_settings();
  $post = $form_state['values'];

  // validate API key
  $key = $post['api_key'];
  if ($settings->api_key != $key) {
    // make a test request
    $qwerly = new Qwerly($key);
    $qwerly->twitter('qwerly');

    if ($qwerly->last_request->error) {
      form_set_error('api_key', t('The provided API key would not allow a valid request: :request', array(':request' => $qwerly->last_request->error,)));
    }
  }

  // cache time
  $time = $post['cache_time'];
  if ($time != $settings->cache_time && (!is_numeric($time) || $time < 30)) {
    form_set_error('cache_time', t('The cache time needs to be a number of seconds. Eg. 86400'));
  }

  // max requests
  $max_requests = $post['max_requests'];
  if ($max_requests != $settings->max_requests) {
    if (!is_numeric($max_requests)) {
      form_set_error('max_requests', t('The max requests needs to be numeric'));
    }
    elseif ($max_requests > 0 && $max_requests < 5) {
      drupal_set_message(t('The max requests has been saved, but is probably too low to keep up with the queue.'), 'warning');
    }
  }

  $action = $post['new_email_action'];
  if ($action != $settings->new_email_action && !in_array($action, array('clear', 'append',))) {
    form_set_error('new_email_action', t('Strange error.'));
  }

}

/**
 * Handles saving all data posted from qwerly_settings_page.
 * Ensures all values are sane.
 */
function qwerly_settings_page_submit($form, $form_state) {
  // get current settings.
  $settings = qwerly_get_settings();
  $post = $form_state['values'];

  $key = $post['api_key'];
  if ($settings->api_key != $key) {
    $settings->api_key = $key;
  }

  // cache time
  $time = $post['cache_time'];
  if ($time != $settings->cache_time) {
    $settings->cache_time = $time;
  }

  // update indefinitely
  $settings->update_indefinitely = isset($post['update_indefinitely']) && $post['update_indefinitely'];

  // max requests
  $max_requests = $post['max_requests'];
  if ($max_requests != $settings->max_requests) {
    $settings->max_requests = $max_requests;
  }

  // new email action
  $action = $post['new_email_action'];
  if ($action != $settings->new_email_action) {
    $settings->new_email_action = $action;
  }

  drupal_set_message(t('Qwerly settings saved'));
  qwerly_save_settings($settings);
}

function qwerly_process_existing() {
  // get all drupal UID's not already in the cache
  $uids = db_query('SELECT `uid` FROM {users} WHERE `uid` NOT IN(SELECT `uid` FROM {qwerly_data})')->fetchAll();

  // distribute them evenly across the next |uids| seconds
  // stops the next cache update being mammoth if no max_requests was set,
  // as no more 60 existing users will expire each minute (+6 requests)

  // add them to the database
  foreach ($uids as $i => $uid) {
    db_insert('qwerly_data')
      ->fields(array(
        'uid' => $uid->uid,
        'expires' => time() + $i,
        'stage' => 0,
        'data' => json_encode(new stdClass),
      ))
      ->execute();
  }

  drupal_set_message(t('Added :uids existing users to the Qwerly cache.', array(':uids' => count($uids),)));

  return drupal_get_form('qwerly_settings_page');
}

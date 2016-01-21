<?php
function bookmark_user_page($account = FALSE) {
  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  $query = db_select('bookmark', 'b');
  $query->addField('b', 'bid');
  $query->condition('b.uid', $account->uid);
  $query->addTag('bookmark_access');
  $query->addMetaData('account', $account);
  $query->addMetaData('op', 'view');
  $query->orderBy('b.created', 'DESC');
  $bids = $query->execute()->fetchCol();

  if (!empty($bids)) {
    $bookmarks = bookmark_load_multiple($bids);
    $build = bookmark_view_multiple($bookmarks);
  }
  else {
    $build['no_bookmarks'] = array(
      '#markup' => t('No bookmarks available.'),
    );
  }

  return $build;
}

function bookmark_page_view($bookmark) {
  //$hash = bookmark_url_get_hash($bookmark->url);
  //drupal_goto("url/$hash");
  return bookmark_view($bookmark, 'full');
}

function bookmark_form($form, $form_state, $bookmark = NULL) {
  if (!isset($bookmark)) {
    $bookmark = new stdClass();
    $bookmark->uid = $GLOBALS['user']->uid;
    $bookmark->language = LANGUAGE_NONE;
    $bookmark->url = '';
    $bookmark->title = '';
    $bookmark->status = BOOKMARK_PUBLIC;
  }

  if (!isset($form_state['bookmark'])) {
    bookmark_object_prepare($bookmark);
    $form_state['bookmark'] = $bookmark;
  }
  else {
    $bookmark = $form_state['bookmark'];
  }

  $form['#attributes']['class'][] = 'bookmark-form';
  $form['#attributes']['class'][] = 'bookmark-form';

  // Basic bookmark information.
  // These elements are just values so they are not even sent to the client.
  foreach (array('bid', 'uid', 'created', 'language') as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => isset($bookmark->$key) ? $bookmark->$key : NULL,
    );
  }

  // Changed must be sent to the client, for later overwrite error checking.
  $form['changed'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($bookmark->changed) ? $bookmark->changed : NULL,
  );

  $form['url'] = array(
    '#type' => module_exists('elements') ? 'urlfield' : 'textfield',
    '#title' => t('URL'),
    '#default_value' => $bookmark->url,
    '#maxlength' => NULL,
    '#element_validate' => array('bookmark_url_element_validate'),
    '#element_validate' => array('bookmark_url_element_validate'),
    '#required' => TRUE,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $bookmark->title,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Public bookmark'),
    '#default_value' => $bookmark->status,
    '#return_value' => BOOKMARK_PUBLIC,
    '#weight' => 50,
  );

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : 'bookmarks',
    '#weight' => 5,
  );

  //$form['#validate'][] = 'bookmark_form_validate';
  $form['#submit'] = array();

  field_attach_form('bookmark', $bookmark, $form, $form_state, $bookmark->language);
  return $form;
}

function bookmark_url_element_validate($element, &$form_state) {
  if (!valid_url($element['#value'], TRUE)) {
    form_error($element, t('The URL %url is invalid.', array('%url' => $element['#value'])));
  }
}

function bookmark_form_submit($form, &$form_state) {
  $bookmark = bookmark_form_submit_build_bookmark($form, $form_state);
  bookmark_save($bookmark);

  $title = entity_label($bookmark);
  watchdog('bookmark', 'Bookmark saved %title.', array('%title' => $title), WATCHDOG_NOTICE);
  drupal_set_message(t('Bookmark %title has been saved.', array('%title' => $title)));

  if ($bookmark->bid) {
    $form_state['values']['bid'] = $bookmark->bid;
    $form_state['bid'] = $bookmark->bid;
    $form_state['redirect'] = 'bookmarks';
  }
  else {
    // In the unlikely case something went wrong on save, the node will be
    // rebuilt and node form redisplayed the same way as in preview.
    drupal_set_message(t('The bookmark could not be saved.'), 'error');
    $form_state['rebuild'] = TRUE;
  }

  // Clear the page and block caches.
  cache_clear_all();
}

function bookmark_form_submit_build_bookmark($form, &$form_state) {
  //unset($form_state['submit_handlers']);
  //form_execute_handlers('submit', $form, $form_state);

  $bookmark = $form_state['bookmark'];
  entity_form_submit_build_entity('bookmark', $bookmark, $form, $form_state);

  //node_submit($node);
  foreach (module_implements('bookmark_submit') as $module) {
    $function = $module . '_bookmark_submit';
    $function($bookmark, $form, $form_state);
  }

  return $bookmark;
}

function bookmark_delete_form($form, &$form_state, $bookmark) {
  $form['#bookmark'] = $bookmark;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['bid'] = array(
    '#type' => 'value',
    '#value' => $bookmark->bid,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the bookmark for %url?', array('%url' => $bookmark->url)),
    'bookmark/' . $bookmark->bid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function bookmark_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $bookmark = bookmark_load($form_state['values']['bid']);
    $title = entity_label($bookmark);
    bookmark_delete($form_state['values']['bid']);
    watchdog('bookmark', 'Bookmark: deleted %title.', array('%title' => $title));
    drupal_set_message(t('Bookmark %title has been deleted.', array('%title' => $title)));
  }

  $form_state['redirect'] = 'bookmarks';
}

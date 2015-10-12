<?php

/**
 * @file
 * Menu callbacks, form callbacks and helpers.
 */

/**
 * Render a instagram_feeds import form on node/id/import pages.
 */
function instagram_feeds_import_tab_form($form, &$form_state, $node) {
  $form = array();
  $form['#feed_nid'] = $node->nid;
  if (isset($_GET['destination'])) {
    $form['#redirect'] = $_GET['destination'];
  }
  else {
    $form['#redirect'] = 'node/' . $node->nid;
  }
  $form['#redirect'] = 'node/' . $node->nid;
  $form = confirm_form($form, check_plain(t('Import content from Instagram Feed?')), 'node/' . $node->nid,
      t('Import content from Instagram Feed?'), t('Import'), t('Cancel'),
      'confirm feeds update');
  return $form;
}


/**
 * Submit handler for instagram_feeds_import_tab_form().
 */
function instagram_feeds_import_tab_form_submit($form, &$form_state) {
  $tasks = instagram_feeds_get_tasks($form['#feed_nid']);
  $batch = array(
    'title' => t('Processing Instagram Import Batch'),
    'init_message' => t('Starting import.'),
    'progress_message' => t('Processed @current out of @total.'),
  );
  $urls_to_import = instagram_feeds_urls_from_feed($form['#feed_nid']);
  $form_state['redirect'] = $form['#redirect'];

  foreach ($tasks as $task) {
    $batch['operations'][] = array('instagram_feeds_queue', array($task));
  }
  foreach ($urls_to_import as $url) {
    $batch['operations'][] = array(
      'instagram_feeds_queue',
      array(
        array(
          'type' => 'instagram_feeds_batch_import',
          'data' => array($url),
        ),
      ),
    );
  }
  batch_set($batch);
}

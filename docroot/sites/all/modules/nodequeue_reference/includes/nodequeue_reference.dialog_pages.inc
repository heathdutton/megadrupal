<?php

/**
 * @file
 * Page callbacks related to references dialog implementation of nodequeue_reference
 */

/**
 * Page callback for our redirect page.
 * In all cases, the callback passes values to the main widnow,
 * that set/override the field value of the nodequeue reference field.
 */
function nodequeue_reference_dialog_redirect_page($qid) {
  $queues = nodequeue_load_queues(array($qid));

  if (isset($queues[$qid]->qid) && references_dialog_in_dialog() && isset($_GET['references-dialog-close'])) {
    drupal_add_js(drupal_get_path('module', 'references_dialog') . '/js/references-dialog-child.js');
    drupal_add_js(array(
      'ReferencesDialog' => array(
        'entity_id' => $queues[$qid]->qid,
        'title' => $queues[$qid]->title,
        'entity_type' => 'not an entity actually',
      ),
    ), 'setting');
  }

  return '';
}

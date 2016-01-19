<?php

/**
 * @file keypic.pages.inc
 *
 * Functions for KeyPic pages
 */

/**
 *  Page callback for KeyPic spam reporting page.
 */
function keypic_report_spam_page_callback($form_id, $token) {
  $select = db_select('keypic_log', 'kpc');
  $select->condition('kpc.token', $token);
  $select->addExpression('COUNT(*)', 'counter');
  if ($log = $select->execute()->fetchObject()) {
    module_load_include('php', 'keypic', 'Keypic');
    Keypic::setFormID($form_id);
    Keypic::reportSpam($token);

    switch ($log->entity_type) {
      case 'comment':
        drupal_goto('admin/content/comment');
        break;
      case 'user':
        drupal_goto('admin/people');
        break;
    }
  }
  
  return drupal_not_found();
}

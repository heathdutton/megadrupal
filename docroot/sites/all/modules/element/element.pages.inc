<?php

/**
 * @file
 * Page callbacks for the Element module.
 */

/**
 * Page callback to provide a simple revision history.
 */
function element_revision_overview($element) {
  $revisions = element_revision_list($element);
  $header = array(t('Revision'), array('data' => t('View'), 'colspan' => 2));
  $rows = array();
  foreach ($revisions as $revision) {
    $row = array();
    $operations = array();
    $row[] = t('!date by !username!log', array(
      '!date' => l(format_date($revision->changed, 'short'), "element/$element->id"), 
      '!username' => theme('username', array('account' => $revision)),
      '!log' => !empty($revision->log) ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : '',
    ));
    $row[] = l(t('View revision'), "element/$element->id/$revision->vid");
    $rows[] = $row;
  }
  $build['element_revisions_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );
  return $build;
}

/**
 * Page callback to render a revision.
 */
function element_view_revision($element) {
  $rev_account = user_load($element->uid);
  $message = t('Revised by @name: @log', array('@name' => $rev_account->name, '@log' => $element->log));
  drupal_set_message($message, 'status');
  return entity_view('element', array($element->id => $element), 'full');
}

/**
 * Page callback to provide a simple Devel debugging page.
 */
function element_devel($element) {
  if (function_exists('kdevel_print_object')) {
    return kdevel_print_object($element);
  }
  else {
    return t('Sorry, function kdevel_print_object() appears to not exist.');
  }
}

<?php

/**
 * Implements hook_gr_info().
 */
function hook_gr_info() {
  return array(
    'name' => t('Own nodes'),
    'needs-data' => 'node',
  );
}

/**
 * Implements hook_gr_queue().
 */
function hook_gr_queue() {
  $results = db_select('user', 'u')
    ->fields('u', array('uid'))
    ->execute()->fetchCol();

  return array_unique($results);
}

/**
 * Implements hook_gr_worker().
 */
function hook_gr_worker($uid) {
  $recipient = user_load($uid);
  $items = entity_load('node', FALSE, array('uid' => $recipient->uid));

  if ($recipient && $items) {
    $params = array();
    $params['gentle_reminder']['items'] = $items;

    // Render items.
    $templates = gentle_reminder_mail_edit_text('hook');
    $item_template = $templates['item_body'];
    
    foreach ($items as $key => $item) {
      $params['gentle_reminder']['rendered_items'][$key] = token_replace($item_template, array('node' => $item));
    }

    gentle_reminder_send_mail($recipient, 'hook', $params);
  }
}

/**
 * Implements hook_gr_template().
 */
function hook_gr_template() {
  return array(
    'subject' => t('You have some unread messages on [site:name]'),
    'body' => t("Hi [gentle_reminder:recipient:name],\n
You have [gentle_reminder:items:count] of smthg:\n[gentle_reminder:items]"),
    'item_body' => t("* [node:title]\n"),
  );
}

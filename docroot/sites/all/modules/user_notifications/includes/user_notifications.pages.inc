<?php

/**
 * Page callback: Displays add content links for available content types.
 *
 * Redirects to user-notifications/add/[type] if only one content type is available.
 *
 * @see node_menu()
 */
function user_notifications_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  // Bypass the node/add listing if only one content type is available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  return theme('user_notifications_add_list', array('content' => $content));
}

<?php

/**
 * Implementation of hook_panels_content_types().
 */
function guestbook_guestbook_user_panels_content_types() {
  $items['guestbook_user'] = array(
    'title' => t('User guestbook'),
    'content_types' => 'guestbook_admin_content_types_guestbook_user',
    'single' => TRUE,
    'render callback' => 'guestbook_content_guestbook_user',
    'title callback' => 'guestbook_admin_title_guestbook_user',
  );
  return $items;
}

/**
 * Return all content types available.
 */
function guestbook_admin_content_types_guestbook_user() {
  return array(
    'user' => array(
      'title' => t('User guestbook'),
      'icon' => 'icon_user.png',
      'path' => panels_get_path('content_types/user'),
      'description' => t('The guestbook of a user.'),
      'required context' => new panels_required_context(t('User'), 'user'),
      'category' => t('User context'),
    ),
  );
}

/**
 * Render the guestbook of a user.
 */
function guestbook_content_guestbook_user($subtype, $conf, $panel_args, $context) {
  if (!$context->data || !user_access('access user guestbooks')) {
    return;
  }

  $block = new stdClass();
  $block->module = 'guestbook';
  $block->subject = t('Guestbook');
  $block->content = guestbook_page($context->data, NULL, NULL, FALSE);
  return $block;
}

/**
 * Return administrative pane title.
 */
function guestbook_admin_title_guestbook_user($conf, $context) {
  return t('"@s" guestbook', array('@s' => $context->identifier));
}


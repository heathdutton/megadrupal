<?php

/**
 * @file
 * Themes hooks and callback functions
 */

/**
 * Themes user notification unread count.
 */
function theme_user_notifications_unread_count_link(&$vars) {
  $link_text = $vars['link_text'];

	$count = user_notifications_unread_count();
	if (empty($count) && variable_get('user_notifications_block_hide_link')) {
		return;
	}

	$inbox = $link_text ? $link_text : t('Notifications');
	if ($count > 0) {
		$inbox .= ' (<strong>'. format_plural($count, '1 new', '@count new') .'</strong>)';
	}
	return l($inbox, 'user-notifications', array('html' => true, 'attributes' => array('title' => t("Notifications and messages from other users"))));
}

/**
 * Themes date headers on notifications page
 */
function theme_user_notifications_date_header(&$vars) {
  $date = $vars['date'];
  $format = $vars['format'];
	return '<h3 class="notification-date-header">'. format_date($date, 'custom', $format) .'</h3>';
}

/**
 * Returns HTML for a list of available user notification types for user
 * notification creation.
 *
 * @param $variables
 *   An associative array containing:
 *   - content: An array of content types.
 *
 * @ingroup themeable
 */
function theme_user_notifications_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="user-notifications-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    $output = '<p>' . t('You have not created any user notification types yet.') . '</p>';
  }
  return $output;
}

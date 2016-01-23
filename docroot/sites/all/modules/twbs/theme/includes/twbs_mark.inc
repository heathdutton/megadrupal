<?php

/**
 * Returns HTML for a marker for new or updated content.
 *
 * @param $variables
 *   An associative array containing:
 *   - type: Number representing the marker type to display. See MARK_NEW,
 *     MARK_UPDATED, MARK_READ.
 */
function twbs_mark($variables) {
  global $user;

  $type = $variables['type'];

  switch ($type) {
    case MARK_NEW:
      $message = t('New');
      $modifier = 'success';
      break;
    case MARK_UPDATED:
      $message = t('Updated');
      $modifier = 'primary';
      break;
    case MARK_READ:
      $message = t('Read');
      $modifier = 'info';
      break;
    default:
      $message = check_plain($variables['type']);
      $modifier = 'default';
  }

  if ($user->uid) {
    return " <span class=\"label label-${modifier}\">${message}</span>";
  }
}

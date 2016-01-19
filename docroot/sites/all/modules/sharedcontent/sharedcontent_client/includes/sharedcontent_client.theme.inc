<?php

/**
 * @file
 * Theme file stub for sharedcontent.
 */

/**
 * Render a linked content.
 *
 * Take care of how the content should be linked.
 *
 * @todo Replace string replacement with token system.
 */
function theme_sharedcontent_view_linked_content($variables) {
  $content = $variables['content'];
  $before = '';
  $after = '';

  if (!empty($content['#url'])) {
    $before = '<a href="' . $content['#url'] . '" class="sharedcontent-origin">';
    $after = '</a>';
  }

  return format_string($content['#content'], array(
    '!open_link' => $before,
    '!close_link' => $after,
    '!article_classes' => implode(' ', $content['#article_classes']),
  ));
}

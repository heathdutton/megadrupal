<?php

/**
 * Returns HTML for the update page.
 *
 * Note: this function is not themeable.
 *
 * @param $variables
 *   An associative array containing:
 *   - content: The page content to show.
 *   - show_messages: Whether to output status and error messages.
 *     FALSE can be useful to postpone the messages to a subsequent page.
 */
function twbs_update_page($variables) {
  drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  return theme('maintenance_page', $variables);
}

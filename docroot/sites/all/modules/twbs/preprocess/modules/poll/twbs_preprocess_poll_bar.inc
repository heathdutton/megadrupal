<?php

/**
 * Preprocess the poll_bar theme hook.
 *
 * Inputs: $title, $votes, $total_votes, $voted, $block
 *
 * @see poll-bar.tpl.php
 * @see poll-bar--block.tpl.php
 * @see theme_poll_bar()
 */
function twbs_preprocess_poll_bar(&$variables) {
  if ($variables['block']) {
    $variables['theme_hook_suggestions'][] = 'poll_bar__block';
  }
  $variables['title'] = check_plain($variables['title']);
  $variables['percentage'] = round($variables['votes'] * 100 / max($variables['total_votes'], 1));
}

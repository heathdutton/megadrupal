<?php

/**
 * Themes the voting form for a poll.
 *
 * Inputs: $form
 */
function twbs_preprocess_poll_vote(&$variables) {
  $form = $variables['form'];
  $variables['choice'] = drupal_render($form['choice']);
  $variables['title'] = check_plain($form['#node']->title);
  $variables['vote'] = drupal_render($form['vote']);
  $variables['rest'] = drupal_render_children($form);
  $variables['block'] = $form['#block'];
  if ($variables['block']) {
    $variables['theme_hook_suggestions'][] = 'poll_vote__block';
  }
}

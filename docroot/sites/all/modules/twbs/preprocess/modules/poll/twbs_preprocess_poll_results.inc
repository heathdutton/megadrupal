<?php

/**
 * Preprocess the poll_results theme hook.
 *
 * Inputs: $raw_title, $results, $votes, $raw_links, $block, $nid, $vote. The
 * $raw_* inputs to this are naturally unsafe; often safe versions are
 * made to simply overwrite the raw version, but in this case it seems likely
 * that the title and the links may be overridden by the theme layer, so they
 * are left in with a different name for that purpose.
 *
 * @see poll-results.tpl.php
 * @see poll-results--block.tpl.php
 */
function twbs_preprocess_poll_results(&$variables) {
  $variables['links'] = theme('links__poll_results', array('links' => $variables['raw_links']));
  if (isset($variables['vote']) && $variables['vote'] > -1 && user_access('cancel own vote')) {
    $elements = drupal_get_form('poll_cancel_form', $variables['nid']);
    $variables['cancel_form'] = drupal_render($elements);
  }
  $variables['title'] = check_plain($variables['raw_title']);

  if ($variables['block']) {
    $variables['theme_hook_suggestions'][] = 'poll_results__block';
  }
}

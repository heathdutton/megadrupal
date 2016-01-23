<?php

/**
 * @file
 * Drush commands for Scoville.
 */

/**
 * Implements hook_drush_command().
 */
function scoville_drush_command() {
  return array(
    'scoville-run' => array(
      'callback' => 'scoville_drush_run',
      'description' => dt('Recalculate Scoville hotness scores for nodes.'),
      'arguments' => array(),
    ),
  );
}

/**
 * Drush callback to do a Scoville run.
 *
 * We basically just do scoville_run() plus display a friendly message to the
 * human.
 */

function scoville_drush_run() {
  $run_stats = scoville_run();
  if ($run_stats) {
    drush_log(dt('Scoville run successfully completed. Cooled: @cool, inserted: @ins, heated: @heat, zeroed: @zero.', array(
      '@cool' => $run_stats['cooled'],
      '@heat' => $run_stats['heated'],
      '@ins' => $run_stats['inserted'],
      '@zero' => $run_stats['zeroed'],
    )), 'success');
  }
  else {
    drush_log(dt('Scoville run failed.'), 'error');
  }
}

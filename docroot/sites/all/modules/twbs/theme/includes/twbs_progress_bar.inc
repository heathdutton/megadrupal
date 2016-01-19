<?php

/**
 * Returns HTML for a progress bar.
 *
 * Note that the core Batch API uses this only for non-JavaScript batch jobs.
 *
 * @param $variables
 *   An associative array containing:
 *   - percent: The percentage of the progress.
 *   - message: A string containing information to be displayed.
 */
function twbs_progress_bar($variables) {
  $output = '';

  $percent = $variables['percent'];
  $message = $variables['message'];

  $output .= '<div id="progress" class="progress">';
  $output .= '<div class="progress-bar" role="progressbar" aria-valuenow="' . $percent . '" aria-valuemin="0" aria-valuemax="100" style="width: ' . $percent . '%;">';
  $output .= '<span class="sr-only">' . $percent . '%</span>';
  $output .= '</div>';
  $output .= '<div class="percentage">' . $percent . '%</div>';
  $output .= '<div class="message">' . $message . '</div>';
  $output .= '</div>';

  return $output;
}

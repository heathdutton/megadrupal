<?php

/**
 * Returns HTML for a user signature.
 *
 * @param $variables
 *   An associative array containing:
 *   - signature: The user's signature.
 *
 * @ingroup themeable
 */
function twbs_user_signature($variables) {
  $signature = $variables['signature'];
  $output = '';

  if ($signature) {
    $output .= '<div class="clear">';
    $output .= '<div>—</div>';
    $output .= $signature;
    $output .= '</div>';
  }

  return $output;
}

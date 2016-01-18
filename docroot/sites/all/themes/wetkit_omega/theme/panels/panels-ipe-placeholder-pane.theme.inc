<?php
/**
 * @file
 * panels-ipe-placeholder-pane.theme.inc
 */


/**
 * Theme the 'placeholder' pane, which is shown on an active IPE.
 *
 * @param array $variables
 *   The array used to modify the panels ipe placeholder pane.
 */
function wetkit_omega_panels_ipe_placeholder_pane($variables) {
  $region_title = $variables['region_title'];

  $output = '<div class="panels-ipe-placeholder-content">';
  $output .= "<h4>$region_title</h4>";
  $output .= '</div>';
  return $output;
}

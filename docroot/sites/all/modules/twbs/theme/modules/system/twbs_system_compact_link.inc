<?php

/**
 * Returns HTML for a link to show or hide inline help descriptions.
 *
 * @ingroup themeable
 */
function twbs_system_compact_link() {
  $output = '<div class="compact-link">';
  if (system_admin_compact_mode()) {
    $output .= l(t('Show descriptions'), 'admin/compact/off', array('attributes' => array('title' => t('Expand layout to include descriptions.')), 'query' => drupal_get_destination()));
  }
  else {
    $output .= l(t('Hide descriptions'), 'admin/compact/on', array('attributes' => array('title' => t('Compress layout by hiding descriptions.')), 'query' => drupal_get_destination()));
  }
  $output .= '</div>';

  return $output;
}

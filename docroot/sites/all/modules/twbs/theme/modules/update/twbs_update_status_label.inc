<?php

/**
 * Returns HTML for a label to display for a project's update status.
 *
 * @param array $variables
 *   An associative array containing:
 *   - status: The integer code for a project's current update status.
 *
 * @see update_calculate_project_data()
 * @ingroup themeable
 */
function twbs_update_status_label($variables) {
  switch ($variables['status']) {
    case UPDATE_NOT_SECURE:
      return '<span class="security-error">' . t('Security update required!') . '</span>';

    case UPDATE_REVOKED:
      return '<span class="revoked">' . t('Revoked!') . '</span>';

    case UPDATE_NOT_SUPPORTED:
      return '<span class="not-supported">' . t('Not supported!') . '</span>';

    case UPDATE_NOT_CURRENT:
      return '<span class="not-current">' . t('Update available') . '</span>';

    case UPDATE_CURRENT:
      return '<span class="current">' . t('Up to date') . '</span>';

  }
}

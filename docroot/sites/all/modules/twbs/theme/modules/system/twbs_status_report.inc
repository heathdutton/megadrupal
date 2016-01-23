<?php

/**
 * Returns HTML for the status report.
 *
 * @param $variables
 *   An associative array containing:
 *   - requirements: An array of requirements.
 *
 * @ingroup themeable
 */
function twbs_status_report($variables) {
  $requirements = $variables['requirements'];
  $severities = array(
    REQUIREMENT_INFO => array(
      'title' => t('Info'),
      'class' => 'active',
      'icon' => '&nbsp;',
    ),
    REQUIREMENT_OK => array(
      'title' => t('OK'),
      'class' => 'success',
      'icon' => '<i class="fa fa-fw fa-check"></i>',
    ),
    REQUIREMENT_WARNING => array(
      'title' => t('Warning'),
      'class' => 'warning',
      'icon' => '<i class="fa fa-fw fa-warning"></i>',
    ),
    REQUIREMENT_ERROR => array(
      'title' => t('Error'),
      'class' => 'danger',
      'icon' => '<i class="fa fa-fw fa-ban"></i>',
    ),
  );

  $rows = array();
  foreach ($requirements as $requirement) {
    if (empty($requirement['#type'])) {
      $severity = $severities[isset($requirement['severity']) ? (int) $requirement['severity'] : 0];

      $row = array();
      $row[] = $severity['icon'] . '<span class="sr-only">' . $severity['title'] . '</span>';
      $row[] = $requirement['title'];
      $row[] = $requirement['value'];
      $rows[] = array('data' => $row, 'class' => $severity['class']);

      if (!empty($requirement['description'])) {
        $row = array();
        $row[] = '&nbsp;';
        $row[] = array('data' => $requirement['description'], 'colspan' => 2);
        $rows[] = array('data' => $row, 'class' => $severity['class']);
      }
    }
  }

  return '<div class="table-responsive">' . theme('table', array('rows' => $rows)) . '</div>';
}

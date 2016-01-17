<?php
/**
 * @file
 * Theme and preprocess functions for breadcrumbs, messages, tabs..etc
 */

/**
 * Returns HTML for a list of maintenance tasks to perform.
 *
 * @param $variables
 *   An associative array containing:
 *   - items: An associative array of maintenance tasks.
 *   - active: The key for the currently active maintenance task.
 *
 * @ingroup themeable
 */
function restaurant_octal_task_list($variables) {
  $items = $variables['items'];
  $active = $variables['active'];

  $done = isset($items[$active]) || $active == NULL;
  $output = '<h2 class="element-invisible">Installation tasks</h2>';
  $output .= '<div class="steps-list">';

  $step = 1;
  foreach ($items as $k => $item) {
    if ($active == $k) {
      $class = 'active';
      $status = '(' . t('active') . ')';
      $done = FALSE;
    }
    else {
      $class = $done ? 'done' : '';
      $status = $done ? '(' . t('done') . ')' : '';
    }

    // Add a step class.
    $class .= ' step';

    $output .= '<div';
    $output .= ($class ? ' class="' . $class . '"' : '') . '>';
    $output .= '<h6 class="text-muted">' . st('Step @step', array('@step' => $step)) . '</h6>';
    $output .= '<h4>' . $item . '</h4>';
    $output .= ($status ? '<span class="element-invisible">' . $status . '</span>' : '');
    $output .= '</div>';

    $step++;
  }
  $output .= '</div>';
  return $output;
}
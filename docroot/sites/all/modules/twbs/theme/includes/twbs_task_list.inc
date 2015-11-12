<?php

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
function twbs_task_list($variables) {
  $items = $variables['items'];
  $active = $variables['active'];

  $done = isset($items[$active]) || $active == NULL;
  $output = '<h2 class="sr-only">Installation tasks</h2>';
  $output .= '<ol class="task-list">';

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
    $output .= '<li';
    $output .= ($class ? ' class="' . $class . '"' : '') . '>';
    $output .= $item;
    $output .= ($status ? '<span class="sr-only">' . $status . '</span>' : '');
    $output .= '</li>';
  }
  $output .= '</ol>';
  return $output;
}

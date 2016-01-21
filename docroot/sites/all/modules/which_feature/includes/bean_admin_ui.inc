<?php
/**
 * @file
 * Bean Admin UI module integration.
 */

/**
 * Implements hook_which_feature_preprocess_table_alter() on behalf of
 * bean_admin_ui.module
 */
function bean_admin_ui_which_feature_preprocess_table_alter(&$vars) {
  $menu = menu_get_item();
  if ('admin/structure/block-types' == $menu['path']) {
    $bean_types = which_feature_get_component('bean_type');

    $operations = array_pop($vars['header']);
    $vars['header'][] = array(
      'data' => t('Features'),
      'class' => array('which-feature'),
      'style' => 'width: 16%',
    );
    array_push($vars['header'], $operations);

    foreach ($vars['rows'] as $delta => &$row) {
      $column = array(
        'data' => '',
        'class' => array('which-feature'),
      );

      foreach ($row as $item) {
        if (preg_match('/block-types\/manage\/(.*?)\//', $item['data'], $matches)) {
          $delta = str_replace('-', '_', $matches[1]);
          if (isset($bean_types[$delta])) {
            $feature = $bean_types['#' . $bean_types[$delta]];
            $column['data'] = l($feature->info['name'], 'admin/structure/features/' . $feature->name);
          }
          break;
        }
      }

      $name = array_shift($row);
      $status = array_shift($row);
      array_unshift($row, $column);
      array_unshift($row, $name);
      array_unshift($row, $status);
    }
  }
}

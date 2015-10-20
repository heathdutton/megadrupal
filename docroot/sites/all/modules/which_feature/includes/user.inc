<?php
/**
 * @file
 * User module integration.
 */

/**
 * Implements hook_which_feature_preprocess_table_alter() on behalf of
 * user.module
 */
function user_which_feature_preprocess_table_alter(&$vars) {
  $menu = menu_get_item();
  if ('admin/people/permissions' == $menu['path']) {
    $permissions = which_feature_get_component('user_permission');

    $vars['header'][] = array(
      'data' => t('Feature'),
      'class' => array('which-feature'),
    );

    foreach ($vars['rows'] as $delta => &$row) {
      $column = array(
        'data' => '',
        'class' => array('which-feature'),
      );

      if (isset($row[1])) {
        if (preg_match('/1\[(.*?)\]/', $row[1]['data'], $matches)) {
          $permission = $matches[1];
          if (in_array($permission, array_keys($permissions))) {
            $feature = $permissions['#' . $permissions[$permission]];
            $column['data'] = l($feature->info['name'], 'admin/structure/features/' . $feature->name);
          }
        }
      }

      $row[] = $column;
    }
  }
}

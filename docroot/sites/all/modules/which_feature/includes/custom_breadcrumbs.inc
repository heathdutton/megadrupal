<?php
/**
 * @file
 * Custom Breadcrumbs module integration.
 */

/**
 * Implements hook_which_feature_preprocess_table_alter() on behalf of
 * custom_breadcrumbs.module
 */
function custom_breadcrumbs_which_feature_preprocess_table_alter(&$vars) {
  $menu = menu_get_item();
  if ('admin/structure/custom_breadcrumbs' == $menu['path']) {
    $component = 'custom_breadcrumb';

    $name = array_shift($vars['header']);
    array_unshift($vars['header'], array(
      'data' => t('Module'),
      'class' => array('which-feature'),
      'style' => 'width: 16%',
    ));
    array_unshift($vars['header'], $name);

    foreach ($vars['rows'] as $delta => &$row) {
      $column = array(
        'data' => '',
        'class' => array('which-feature'),
      );

      foreach ($row as $item) {
        if (is_string($item) && preg_match('/custom_breadcrumbs\/edit\/(.*?)\W/', $item, $matches)) {
          $custom_breadcrumb = custom_breadcrumbs_obj_load($matches[1]);
          if (isset($custom_breadcrumb->export_module)) {
            $module = features_get_info('module', $custom_breadcrumb->export_module);
            $column['data'] = $module->info['name'];
            if (isset($module->info['features'][$component]) && in_array($matches[1], $module->info['features'][$component])) {
              $column['data'] = l($column['data'], 'admin/structure/features/' . $module->name);
            }
          }
          break;
        }
      }

      $name = array_shift($row);
      array_unshift($row, $column);
      array_unshift($row, $name);
    }
  }
}

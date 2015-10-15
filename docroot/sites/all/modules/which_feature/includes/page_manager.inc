<?php
/**
 * @file
 * Page Manager module integration.
 */

/**
 * Implements hook_which_feature_preprocess_table_alter() on behalf of
 * page_manager.module
 */
function page_manager_which_feature_preprocess_table_alter(&$vars) {
  $menu = menu_get_item();
  if ('admin/structure/pages' == $menu['path']) {
    ctools_include('export');
    $component = 'page_manager_pages';
    $tasks = page_manager_get_tasks_by_type('page');

    $operations = array_pop($vars['header']);
    $vars['header'][] = array(
      'data' => t('Module'),
      'class' => array('which-feature'),
      'style' => 'width: 16%',
    );
    array_push($vars['header'], $operations);

    foreach ($vars['rows'] as $delta => &$row) {
      $column = array(
        'data' => '',
        'class' => array('which-feature'),
      );

      if (isset($tasks[$delta]) && isset($tasks[$delta]['module'])) {
        $module = features_get_info('module', $tasks[$delta]['module']);
        $column['data'] = $module->info['name'];
      }

      elseif (preg_match('/page-(.*)/', $delta, $matches)) {
        $page = ctools_export_crud_load($component, $matches[1]);
        if (isset($page->export_module)) {
          $module = features_get_info('module', $page->export_module);
          $column['data'] = $module->info['name'];

          if (isset($module->info['features'][$component]) && in_array($matches[1], $module->info['features'][$component])) {
            $column['data'] = l($column['data'], 'admin/structure/features/' . $module->name);
          }
        }
      }

      $operations = array_pop($row['data']);
      $row['data'][] = $column;
      array_push($row['data'], $operations);
    }
  }
}

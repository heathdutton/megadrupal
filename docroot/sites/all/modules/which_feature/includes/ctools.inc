<?php
/**
 * @file
 * Chaos tool suite (ctools) module integration.
 */

/**
 * Implements hook_which_feature_preprocess_table_alter() on behalf of
 * ctools.module
 */
function ctools_which_feature_preprocess_table_alter(&$vars) {
  if ('ctools-export-ui-list-items' == $vars['attributes']['id']) {
    $menu = menu_get_item();
    foreach (ctools_get_export_uis() as $plugin) {
      if (isset($plugin['menu']) && is_array($plugin['menu']) && strstr($menu['path'], $plugin['menu']['menu prefix'] . '/' . $plugin['menu']['menu item'])) {
        $operations = array_pop($vars['header']);
        $vars['header'][] = array(
          'data' => t('Module'),
          'class' => array('which-feature'),
          'style' => 'width: 16%',
        );
        array_push($vars['header'], $operations);

        foreach ($vars['rows'] as $delta => &$row) {
          $function = "{$plugin['module']}_which_feature_ctools_table_row";
          if (function_exists($function)) {
            $continue = $function($delta, $row);
            if (!$continue) {
              continue;
            }
          }

          $column = array(
            'data' => '',
            'class' => array('which-feature'),
          );

          $object = ctools_export_crud_load($plugin['schema'], $delta);
          if (isset($object->export_module)) {
            $module = features_get_info('module', $object->export_module);
            $column['data'] = $module->info['name'];
            if (isset($module->info['features'][$plugin['schema']]) && in_array($delta, $module->info['features'][$plugin['schema']])) {
              $column['data'] = l($column['data'], 'admin/structure/features/' . $module->name);
            }
          }

          $operations = array_pop($row['data']);
          $row['data'][] = $column;
          array_push($row['data'], $operations);
        }
        break;
      }
    }
  }
}

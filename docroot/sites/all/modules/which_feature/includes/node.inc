<?php
/**
 * @file
 * Node module integration.
 */

/**
 * Implements hook_which_feature_preprocess_table_alter() on behalf of
 * node.module
 */
function node_which_feature_preprocess_table_alter(&$vars) {
  $menu = menu_get_item();
  if ('admin/structure/types' == $menu['path']) {
    $content_types = node_type_get_types();

    $operations = array_pop($vars['header']);
    $vars['header'][] = array(
      'data' => t('Module'),
      'class' => array('which-feature'),
    );
    array_push($vars['header'], $operations);

    foreach ($vars['rows'] as $delta => &$row) {
      $column = array(
        'data' => '',
        'class' => array('which-feature'),
      );

      // @TODO - Deal with translation string.
      if (preg_match('/\(Machine name: (.*?)\)/', $row[0], $matches)) {
        if (isset($content_types[$matches[1]])) {
          $content_type = $content_types[$matches[1]];
          if (isset($content_type->module)) {
            $module = features_get_info('module', $content_type->module);
            $column['data'] = $module->info['name'];
            if (isset($module->info['features']['node']) && in_array($content_type->type, $module->info['features']['node'])) {
              $column['data'] = l($column['data'], 'admin/structure/features/' . $module->name);
            }
          }
        }
      }

      $name = array_shift($row);
      array_unshift($row, $column);
      array_unshift($row, $name);
    }
  }
}

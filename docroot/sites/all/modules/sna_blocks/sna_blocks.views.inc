<?php
/**
 * @file
 * Defines the View Style Plugins for Views Slideshow module.
 */

/**
 * Implements hook_views_plugins().
 */
function sna_blocks_views_plugins() {
  return array(
    'style' => array(
      'snablocks' => array(
        'title' => t('Simple Node Archive'),
        'help' => t('Display the results as a slideshow.'),
        'handler' => 'sna_blocks_plugin_style_snablocks',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses row class' => TRUE,
        'type' => 'normal',
        'path' => drupal_get_path('module', 'sna_blocks'),
        'theme' => 'sna_blocks',
        'theme path' => drupal_get_path('module', 'sna_blocks') . '/theme',
        'theme file' => 'sna_blocks.theme.inc',
      ),
    ),
  );
}

/**
 * Implements hook_views_query_alter().
 */
function sna_blocks_views_query_alter(&$view, &$query) {
  $field_name = '';
  // Find the views block having which have 'snablocks' style_plugin enabled.
  $style_options = sna_blocks_get_view_style_options($view->name, $view->current_display);
  if (!empty($style_options)) {
    $field_name = $style_options['field_name'];
  }
  if (!empty($field_name)) {
    $display_year = $display_month = '';

    $display_year = isset($view->args[0]) && $view->args[0] != '' && is_numeric($view->args[0]) ? $view->args[0] : '';
    $display_month = isset($view->args[1]) && $view->args[1] != '00' && is_numeric($view->args[1]) ? $view->args[1] : '';

    $format = $value = '';

    if ($field_name != 'node_created') {
      $field = 'sna_field_data_' . $field_name . '.' . $field_name . '_value';
      $placeholder = ':field_data_' . $field_name . '_' . $field_name . '_value';

      if (!empty($display_year)) {
        $format = 'Y';
        $value = $display_year;
      }
      if (!empty($display_month)) {
        $format = 'Y-m';
        $value = $display_year . '-' . $display_month;
      }

      $group = 0;
      // Check for a group.
      if (!isset($query->where[$group])) {
        $query->set_where_group('AND', $group);
      }

      $data_obj = new date_sql_handler(DATE_UNIX);

      $field_obj = field_info_field($field_name);
      if (!empty($field_obj) && !empty($field_obj['type'])) {
        $data_obj->date_type = $field_obj['type'];
      }
      $data_obj->db_timezone = date_get_timezone_db($field_obj['settings']['tz_handling']);
      $data_obj->local_timezone = date_get_timezone($field_obj['settings']['tz_handling']);

      $comp_date = new DateObject($value, date_default_timezone(), $format);
      $field = $data_obj->sql_field($field, NULL, $comp_date);
      $field = $data_obj->sql_format($format, $field);

      if (!empty($value) && $value != '') {
        $alias = 'sna_field_data_' . $field_name;
        $join = new views_join();
        $join->table = 'field_data_' . $field_name;
        $join->left_table = 'node';
        $join->field = 'entity_id';
        $join->left_field = 'nid';
        $join->type = 'LEFT';

        // Do the actual join.
        $query->table_queue[$field_name] = array(
          'num' => 1,
          'alias' => $alias,
          'table' => 'field_data_' . $field_name,
          'relationship' => 'node',
          'join' => $join,
        );
        $query->add_where_expression($group, "$field = $placeholder", array($placeholder => $value));
      }
    }
    else {
      if (!empty($display_year)) {
        $format = '%Y';
        $value = $display_year;
      }
      if (!empty($display_month)) {
        $format = '%Y-%m';
        $value = $display_year . '-' . $display_month;
      }
      if (!empty($value) && $value != '') {
        $query->where[1]['conditions'][] = array(
          'field' => "FROM_UNIXTIME(node.created, '" . $format . "') = '" . $value . "'",
          'value' => array(),
          'operator' => 'formula',
        );
      }
    }
  }
}

<?php
/**
 * @file
 * Preprocessing and theming functions for Views Maintenance.
 */

/**
 * Stores and returns field for comparing views.
 *
 * Also, initializes _views_maintenance_compare_by_order() with preferred order
 * if new field is "storage" or "status".
 *
 * @param string|null $header
 *   If value is not NULL it is saved for future calls without arguments.
 *
 * @return string|null
 *   Returns previously set value.
 */
function _views_maintenance_views_sort_criteria($new_field = NULL) {
  static $field = NULL;
  if (isset($new_field)) {
    $field = $new_field;

    // For fields sorted by predefined order initialize comparing function.
    // Views are sorted by attention required, ascending order means
    // view requiring immediate attention is first.
    switch ($new_field) {
      case 'storage':
        _views_maintenance_compare_by_order(NULL, NULL, array(t('Overridden'), t('Normal'), t('Default')));
        break;

      case 'status':
        _views_maintenance_compare_by_order(NULL, NULL, array('broken', 'unused', 'has-unused', 'has-maybe', 'ok'));
        break;
    }
  }
  return $field;
}

/**
 * Compares passed values using strcasecmp() or predefined order.
 *
 * Callback for uasort() within _views_maintenance_views_sort().
 *
 * Field for comparing is fetched from _views_maintenance_views_sort_criteria().
 * For name and type uses strcasecmp(), for storage and status compares values
 * by predefined order using _views_maintenance_compare_by_order(). Order
 * is initially set by _views_maintenance_views_sort_criteria().
 *
 * @param array $a
 * @param array $b
 *
 * @return int
 */
function _views_maintenance_views_sort_callback($a, $b) {
  $field = _views_maintenance_views_sort_criteria();
  switch ($field) {
    case 'type':
      $result = strcasecmp($a[$field], $b[$field]);
      break;

    case 'storage':
    case 'status':
      $result = _views_maintenance_compare_by_order($a[$field], $b[$field]);
      break;

    default:
      $result = 0;
      break;
  }

  if ($result == 0) {
    // For equal values (or sorting by name) return names comparison result.
    return strcasecmp($a['name'], $b['name']);
  }
  else {
    return $result;
  }
}

/**
 * Sorts views according to fields returned by tablesort.
 *
 * @param array $views
 *   List of views objects.
 */
function _views_maintenance_views_sort(&$views, $header) {
  $ts = tablesort_init($header);

  _views_maintenance_views_sort_criteria($ts['sql']);
  uasort($views, '_views_maintenance_views_sort_callback');

  if ($ts['sort'] == 'desc') {
    $views = array_reverse($views, TRUE);
  }
}

/**
 * Returns HTML for primary views table thead tag.
 *
 * Code was copied from theme_table().
 *
 * @param array $vars
 */
function theme_views_maintenance_views_thead($vars) {
  $ts = tablesort_init($vars['header']);
  $output = '<thead><tr>';
  foreach ($vars['header'] as $cell) {
    $cell = tablesort_header($cell, $vars['header'], $ts);
    $output .= _theme_table_cell($cell, TRUE);
  }
  $output .= "</tr></thead>\n";

  return $output;
}

/**
 * Prepares views info for output and includes required CSS/JS.
 *
 * @param array $vars
 */
function template_preprocess_views_maintenance_views_table(&$vars) {
  // Add primary views table header.
  $vars['views_header'] = array(
    array(
      'data' => '&nbsp;',
      'class' => 'views-mnt-view-toggle-all',
    ),
    array(
      'data' => t('View name'),
      'field' => 'name',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Type'),
      'field' => 'type',
    ),
    array(
      'data' => t('Storage'),
      'field' => 'storage',
    ),
    array(
      'data' => t('Status'),
      'field' => 'status',
    ),
    array(
      'data' => t('Actions'),
      'class' => 'views-mnt-view-links',
    ),
  );

  // Add headers for display tables.
  $vars['displays_header'] = array(
    t('Name'),
    t('Type'),
    t('Status'),
    t('Use case'),
    t('Description'),
    t('Actions'),
  );

  // Theme thead.
  $vars['thead'] = theme('views_maintenance_views_thead', array('header' => $vars['views_header']));

  if (isset($vars['views']) && is_array($vars['views'])) {
    // Sort views before replacing storage and status with themed values.
    _views_maintenance_views_sort($vars['views'], $vars['views_header']);

    // Convert display status info to table and prepare view info for printing.
    foreach ($vars['views'] as $view_id => &$view_info) {
      $rows = array();
      foreach ($view_info['displays'] as $display_id => $display_info) {
        // Convert associative arrays to table rows.
        $first = TRUE;
        foreach ($display_info['use_cases'] as $use_case) {
          // Description can be an array or HTML string.
          $description = '';
          if (!empty($use_case['description'])) {
            $description = theme('item_list', array('items' => array($use_case['description'])));
          }

          $use_case_row = array(
            theme('views_maintenance_display_status', array('status' => $use_case['status'])),
            isset($use_case['type']) ? $use_case['type'] : '',
            $description,
            !empty($use_case['links']) ? theme('item_list', array('items' => $use_case['links'])) : '',
          );

          if ($first) {
            // Add name and type to the first row of this display.
            array_unshift($use_case_row, array(
                'data' => check_plain($display_info['name']),
                'rowspan' => count($display_info['use_cases']),
                'class' => array('views-mnt-display-name'),
              ),
              array(
                'data' => check_plain($display_info['type']),
                'rowspan' => count($display_info['use_cases']),
                'class' => array('views-mnt-display-type'),
              ));
            $first = FALSE;
          }
          $rows[] = $use_case_row;
        }
      }
      $view_info['name'] = check_plain($view_info['name']);

      // Save displays table.
      $view_info['rows'] = $rows;
      $view_info['table'] = theme('table', array(
        'header' => $vars['displays_header'],
        'rows' => $view_info['rows'],
        'attributes' => array(
          'class' => array('views-mnt-display-table'),
        ),
      ));

      // Convert links array to string.
      $view_info['links'] = implode(' | ', $view_info['links']);

      // Theme view status and storage.
      $view_info['status'] = theme('views_maintenance_view_status', array('status' => $view_info['status']));
      $view_info['storage'] = theme('views_maintenance_view_storage', array('type' => $view_info['storage']));

      // Filter view description.
      $view_info['description'] = (trim($view_info['description']) != '') ? check_plain($view_info['description']) : NULL;
    }
  }

  // Include required JS and CSS.
  $module_path = drupal_get_path('module', 'views_maintenance');
  drupal_add_css($module_path . '/css/views-maintenance-admin.css');
  drupal_add_js($module_path . '/js/views-maintenance-admin.js');
}

/**
 * Returns HTML for view storage type.
 *
 * @param array $vars
 *
 * @return string
 */
function theme_views_maintenance_view_storage($vars) {
  // Convert storage type to our format and add class to highlight it.
  switch ($vars['type']) {
    case t('Default'):
      $storage = t('In code');
      $class = "ok";
      break;

    case t('Overridden'):
      $storage = $vars['type'];
      $class = "error";
      break;

    default: //t('Normal')
      $storage = t('Database');
      $class = "warning";
      break;
  }

  return '<span class="' . $class . '">' . $storage . '</span>';
}

/**
 * Returns HTML for display status.
 *
 * @param array $vars
 *
 * @return string
 */
function theme_views_maintenance_display_status($vars) {
  $attributes = array();
  switch ($vars['status']) {
    case 'broken':
      $attributes['class'] = 'error broken';
      $attributes['title'] = t("Display is broken! Immediate attention is required.");
      $value = t('Broken!');
      break;

    case 'ok':
      $attributes['class'] = 'ok';
      $attributes['title'] = t("Display is in use and shouldn't be removed.");
      $value = t('OK');
      break;

    case 'maybe':
      $attributes['class'] = 'warning';
      $attributes['title'] = t("Display is probably used somewhere, but we can't be sure.");
      $value = t('Probably unused');
      break;

    default: // unused
      $attributes['class'] = 'error';
      $attributes['title'] = t("Looks like display isn't used.");
      $value = t('Unused');
      break;
  }

  return '<span' . drupal_attributes($attributes) . '>' . $value . '</span>';
}

/**
 * Returns HTML for view status.
 *
 * @param array $vars
 *
 * @return string
 */
function theme_views_maintenance_view_status($vars) {
  $attributes = array();
  switch ($vars['status']) {
    case 'broken':
      $attributes['class'] = 'error broken';
      $attributes['title'] = t("View is broken! Immediate attention is required.");
      $value = t('Broken!');
      break;

    case 'ok':
      $attributes['class'] = 'ok';
      $attributes['title'] = t("All displays are used.");
      $value = t('OK');
      break;

    case 'has-maybe':
      $attributes['class'] = 'warning';
      $attributes['title'] = t("Some displays usage can't be detected.");
      $value = t('Probably unused displays');
      break;

    case 'has-unused':
      $attributes['class'] = 'error';
      $attributes['title'] = t("Some displays are unused and need your attention.");
      $value = t('Unused displays');
      break;

    default: // unused
      $attributes['class'] = 'error';
      $attributes['title'] = t("Looks like view isn't used anywhere and can be removed.");
      $value = t('Unused');
      break;
  }
  return '<span' . drupal_attributes($attributes) . '>' . $value . '</span>';
}

/**
 * Formats HTML string for emphasized display in a placeholder inside a sentence.
 *
 * @param array $vars
 *
 * @return string
 */
function theme_views_maintenance_html_placeholder($vars) {
  return '<em>' . $vars['html'] . '</em>';
}

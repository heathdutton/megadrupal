<?php

/**
 * @file
 * Theme functions used for normal file output.
 */

/**
 * Implements template_preprocess_hook() for dbtables_views.
 *
 * @param array $vars
 *   All the variables available for the theme.
 */
function template_preprocess_dbtables_view(&$vars) {
  $vars['dbtable_tabs'] = theme('dbtables_tabs', array('tab' => 1));
  $vars['table'] = '';
  $vars['pager'] = '';

  if (isset($vars['dbtable'])) {
    if (trim($vars['dbtable']['name']) != '') {
      drupal_set_title(t('Table') . ': ' . $vars['dbtable']['name']);
    }

    $table_name = $vars['dbtable']['name'];

    $query = db_select($table_name, 't');
    $query->fields('t');
    $r = $query->execute()->fetchAssoc();

    if (!empty($r)) {
      $header = array();
      $i = 0;
      foreach ($r as $key => $value) {
        if ($i == 0) {
          $header[$i]['sort'] = 'desc';
        }
        $header[$i]['data'] = $key;
        $header[$i]['field'] = $key;
        $i++;
      }

      $element = 0;
      $limit = 20;
      $page = isset($_GET['page']) ? $_GET['page'] : 0;
      $range_start = ($page * $limit);

      $quantity = $query->countQuery()->execute()->fetchField();
      $query->extend('PagerDefault')->limit($limit);
      $query->extend('TableSort')->orderByHeader($header);
      $query->range($range_start, $limit);
      $results = $query->execute();
      $rows = array();

      foreach ($results as $result) {
        foreach ($r as $key => $value) {
          $row[$key] = $result->{$key};
        }
        $rows[]['data'] = $row;
      }

      $variables = array(
        'header' => $header,
        'rows' => $rows,
      );
      $vars['table'] = theme('table', $variables);
      $tags = array();
      $parameters = array();
      pager_default_initialize($quantity, $limit);
      $vars['pager'] = theme('pager', $tags, $limit, $element, $parameters, $quantity);
    }
    else {
      $vars['table'] = t('Empty table.');
    }
  }
}

/**
 * Implements template_preprocess_hook() for dbtables_info.
 *
 * @param array $vars
 *   All the variables available for the theme.
 */
function template_preprocess_dbtables_info(&$vars) {
  $vars['dbtable_tabs'] = theme('dbtables_tabs', array('tab' => 0));
  $vars['dbtable_desc'] = '';
  $vars['dbtable_info'] = '';

  if (isset($vars['dbtable'])) {
    if (trim($vars['dbtable']['name']) != '') {
      drupal_set_title(t('Table') . ': ' . $vars['dbtable']['name']);
    }

    if (isset($vars['dbtable']['description']) && trim($vars['dbtable']['description']) != '') {
      $vars['dbtable_desc'] = '<p><b>' . t('Description') . '</b>: ' . $vars['dbtable']['description'] . '</p>';
    }

    $header = array(
      0 => t('Name'),
      1 => t('Datatype'),
      2 => t('Length/Set'),
      3 => t('Unsigned'),
      4 => t('Not Null'),
      5 => t('Default'),
      6 => t('Comment'),
    );

    $rows = array();
    foreach ($vars['dbtable']['fields'] as $key => $field) {
      $row[0] = $key;
      $row[1] = (isset($field['type'])) ? $field['type'] : '';
      $row[2] = (isset($field['length'])) ? $field['length'] : '';
      $row[3] = (isset($field['unsigned'])) ? $field['unsigned'] : '';
      $row[4] = (isset($field['not null'])) ? $field['not null'] : '';
      $row[5] = (isset($field['default'])) ? $field['default'] : '';
      $row[6] = (isset($field['description'])) ? $field['description'] : '';
      $rows[]['data'] = $row;
    }

    $variables = array(
      'header' => $header,
      'rows' => $rows,
    );
    $vars['dbtable_info'] = theme('table', $variables);
  }
}

/**
 * Implements template_preprocess_hook() for dbtables_tabs.
 *
 * @param array $vars
 *   All the variables available for the theme.
 */
function template_preprocess_dbtables_tabs(&$vars) {
  drupal_add_css(drupal_get_path('module', 'dbtables') . '/css/dbtables.css');
  $text0 = t('Info Table');
  $path0 = 'admin/content/dbtables/' . arg(3) . '/info';
  $text1 = t('View Table');
  $path1 = 'admin/content/dbtables/' . arg(3) . '/view';
  switch ($vars['tab']) {
    case '0':
      $vars['menu'][0] = '<li class="active">' . l($text0, $path0) . '</li>';
      $vars['menu'][1] = '<li>' . l($text1, $path1) . '</li>';
      break;

    case '1':
      $vars['menu'][0] = '<li>' . l($text0, $path0) . '</li>';
      $vars['menu'][1] = '<li class="active">' . l($text1, $path1) . '</li>';
      break;
  }
  $text2 = t('Change Table');
  $path2 = 'admin/content/dbtables';
  $vars['menu'][2] = '<li>' . l($text2, $path2) . '</li>';
}

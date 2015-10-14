<?php

/**
 * @file
 *
 */

/**
 * @param $vars
 */
function theme_path_metatags_ui_add_form($vars) {
  return _path_metatags_render_table($vars);
}

/**
 * Theme table with path metatags values.
 * @param $vars
 */
function theme_path_metatags_ui_edit_form($vars) {
  return _path_metatags_render_table($vars);
}

/**
 * @param $vars
 * @return string
 */
function _path_metatags_render_table($vars) {
  $form = $vars['form'];

  if (isset($form['metatags_table'])) {
    $rows = array();
    foreach (element_children($form['metatags_table']) as $value) {
      $row = array();
      $input = $form['metatags_table'][$value];
      if (isset($input['metatag']) && isset($input['metatag_value']) && isset($input['delete'])) {
        $row[] = render($input['metatag']);
        $row[] = render($input['metatag_value']);
        $row[] = render($input['delete']);
        $rows[] = $row;
      }
    }

    unset($form['metatags_table']);

    $rows[] = array(
      'data' => array(array('data' => render($form['more']), 'colspan' => 3))
    );

    $headers = array(t('Metatag'), t('Value'), t('Delete'));
    $output = theme('table', array(
      'header' => $headers,
      'rows' => $rows,
      'attributes' => array(
        'class' => array('metatags-tokens')
      ),
    ));

    // Build some other elements after table.
    $buttons = render($form['actions']);

    return drupal_render_children($form) . $output . $buttons;
  }
}

/**
 * Theming function for path_metatags_ui_metatags_list form.
 */
function theme_path_metatags_ui_metatags_list($vars) {

  $form = $vars['form'];
  $rows = array();
  foreach (element_children($form) as $path_id) {
    if (isset($form[$path_id]['weight'])) {
      $row = array();
      $row[] = render($form[$path_id]['title']);
      $row[] = render($form[$path_id]['name']);
      $row[] = render($form[$path_id]['path']);
      $row[] = render($form[$path_id]['actions']);
      $row[] = render($form[$path_id]['weight']);

      $class = 'enabled';
      if ($form[$path_id]['disabled']['#value'] == TRUE) {
        $class = 'disabled';
      }

      $rows[] = array(
        'data' => $row,
        'class' => array('draggable', $class),
      );
    }
  }

  if (empty($rows)) {
    $rows = array(array(
      'data' => array(array('data' => t('There are no created path metatags yet.'), 'colspan' => 5))
    ));
  }

  $header = array(t('Title'), t('Name'), t('Path'), t('Actions'), t('Weight'));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'path-metatags-ui-table-list')));
  drupal_add_tabledrag('path-metatags-ui-table-list', 'order', 'sibling', 'path-metatags-ui-table-weight');
  return $output . drupal_render_children($form);
}

/**
 * Theme the table for argument settings form.
 */
function theme_path_metatags_ui_form_step_arguments_selection_table($vars) {

  $header = array(
    array('data' => t('Argument')),
    array('data' => t('Position in path')),
    array('data' => t('Context assigned')),
    array('data' => t('Operations')),
  );

  $form = $vars['form'];
  ctools_include('modal');
  ctools_modal_add_js();

  $rows = array();
  foreach (element_children($form['argument']) as $key) {
    $row   = array();
    $row[] = check_plain($form['argument'][$key]['#keyword']);
    $row[] = check_plain($form['argument'][$key]['#position']);
    $row[] = $form['argument'][$key]['#context'] . ' &nbsp; ' . drupal_render($form['argument'][$key]['change']);
    $row[] = drupal_render($form['argument'][$key]['settings']) . drupal_render($form['argument'][$key]);
    $rows[]  = array('data' => $row);
  }

  if (!$rows) {
    $rows[] = array(array('data' => t('The path %path has no arguments to configure.', array('%path' => $form['#metatags-path'])), 'colspan' => 4));
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'path-metatags-ui-argument-table')
  ));
}

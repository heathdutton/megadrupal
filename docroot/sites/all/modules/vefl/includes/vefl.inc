<?php

/**
 * @file
 * Provides default layouts for views exposed form.
 */

/**
 * Implements hook_vefl_layouts().
 */
function vefl_vefl_layouts() {
  return array(
    'vefl_onecol' => array(
      'title' => t('Single column'),
      'regions' => array(
        'middle' => t('Middle column'),
      ),
      'module' => 'VEFL',
    ),
    'vefl_twocol' => array(
      'title' => t('Two column'),
      'regions' => array(
        'left' => t('Left side'),
        'right' => t('Right side'),
      ),
      'module' => 'VEFL',
    ),
    'vefl_threecol' => array(
      'title' => t('Three column'),
      'regions' => array(
        'left' => t('Left side'),
        'middle' => t('Middle column'),
        'right' => t('Right side'),
      ),
      'module' => 'VEFL',
    ),
  );
}

/**
 * A theme preprocess function for views_exposed_form.
 *
 * Adds $region_widgets array with separated by regions widgets.
 */
function vefl_views_exposed_form_preprocess(&$vars) {
  if (empty($vars['form']['#layout'])) {
    return;
  }
  $layout = $vars['form']['#layout'];
  $actions = array_keys(vefl_form_actions());

  // Adds additional action for BEF combined sort.
  if (!empty($vars['widgets']['sort-sort_bef_combine'])) {
    $actions[] = 'sort-sort_bef_combine';
  }
  // Ensure each region has an empty value.
  foreach ($layout['region_widgets'] as $region => $fields) {
    $vars['region_widgets'][$region] = array();
  }

  // Put fields to regions.
  foreach ($layout['region_widgets'] as $region => $fields) {
    foreach ($fields as $id) {
      if (!in_array($id, $actions) && !empty($vars['widgets'][$id]->widget)) {
        $vars['region_widgets'][$region][$id] = $vars['widgets'][$id];
      }
    }
  }
  // Put form actions to regions.
  foreach ($layout['region_widgets'] as $region => $fields) {
    foreach ($fields as $id) {
      if (in_array($id, $actions) && (!empty($vars['widgets'][$id]->widget) || !empty($vars[$id]))) {
        $widget = !empty($vars['widgets'][$id]->widget) ? $vars['widgets'][$id] : array('widget' => $vars[$id], 'id' => $id);
        $vars['region_widgets'][$region][$id] = (object) $widget;
      }
    }
  }
  $vars['classes_array'][] = 'vefl-layout';
  $vars['classes_array'][] = str_replace('_', '-', $layout['layout_id']);
  if (vefl_layout_module($layout['layout_id']) == 'vefl') {
    drupal_add_css(drupal_get_path('module', 'vefl') . '/css/vefl-layouts.css');
  }
}

/**
 * A theme preprocess function for views_exposed_widget.
 */
function vefl_preprocess_views_exposed_widget(&$vars) {
  $vars['widget']->id = str_replace('_', '-', $vars['widget']->id);
  $vars['classes_array'][] = 'views-widget-' . $vars['widget']->id;

  // Adds classes like in default views template.
  if ($vars['widget']->id == 'button') {
    $vars['classes_array'][] = 'views-submit-button';
  }
  if ($vars['widget']->id == 'reset-button') {
    $vars['classes_array'][] = 'views-reset-button';
  }

  // Adds widget theme suggestions.
  $field_name = str_replace('-', '_', $vars['widget']->id);
  if (substr($field_name, 0, 5) == 'edit_') {
    $field_name = substr($field_name, 5);
  }
  // @todo: add suggestion by views name and display.
  $vars['theme_hook_suggestions'][] = 'views_exposed_widget__' . $field_name;
}

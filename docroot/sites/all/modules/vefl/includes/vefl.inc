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
function vefl_preprocess_views_exposed_form(&$vars) {
  if (empty($vars['form']['#layout'])) {
    return;
  }
  $layout = $vars['form']['#layout'];
  $actions = array_keys(vefl_form_actions());

  // Ensure each region has an empty value.
  foreach ($layout['region_widgets'] as $region => $fields) {
    $vars['region_widgets'][$region] = '';
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
      if (in_array($id, $actions) && !empty($vars[$id])) {
        $widget = array('widget' => $vars[$id], 'id' => 'bef-' . $id);
        $vars['region_widgets'][$region][$id] = (object) $widget;
      }
    }
  }
  $vars['classes_array'][] = 'vefl-layout';
  $vars['classes_array'][] = str_replace('_', '-', $layout['layout']);
}

<?php

/**
 * @file
 * Theming functions for Formatter Field Preset module.
 */

/**
 * Create table from presets list form.
 *
 * @param array $vars
 *   Array that contains list form.
 * @return string
 *   Rendered output.
 */
function theme_formatter_field_preset_list_form($vars) {
  $header = array(t('Name'), t('Machine name'), t('Field'), t('Formatter'), t('Actions'));

  // Get form that should be rendered.
  $form = $vars['form'];

  $rows = array();
  foreach (element_children($form) as $object_name) {
    if (isset($form[$object_name]['machine_name'])) {

      // Build table row parts from form values.
      $row   = array();
      $row[] = render($form[$object_name]['name']);
      $row[] = render($form[$object_name]['machine_name']);
      $row[] = render($form[$object_name]['field']);
      $row[] = render($form[$object_name]['format']);
      $row[] = render($form[$object_name]['actions']);

      // Add class that shows status of exportable object: enabled or disabled.
      $class = 'enabled';
      if ($form[$object_name]['disabled']['#value'] == TRUE) {
        $class = 'disabled';
      }

      // Build table row from from values.
      $rows[] = array(
        'data' => $row,
        'class' => array($class),
      );
    }
  }

  // If no exportable objects in database we should show some empty text.
  if (empty($rows)) {
    $rows = array(array(
      'data' => array(array('data' => t('There are no presets yet.'), 'colspan' => count($header)))
    ));
  }

  // Render table with exportable objects.
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  // Return rendered table and rendered rest form parts.
  return $output . drupal_render_children($form);
}

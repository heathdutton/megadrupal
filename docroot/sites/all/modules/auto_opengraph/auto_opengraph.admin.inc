<?php
// $Id$

/**
 * @file
 *
 */

function auto_opengraph_settings_form() {
  
  $form['auto_opengraph_tags__default'] = _auto_opengraph_form(NULL, NULL, t('Default opengraph tag patterns'));
  $form['auto_opengraph_tags__front'] = _auto_opengraph_form('front', NULL, t('Overrides for front page'));
  $form['auto_opengraph_tags__front']['auto_opengraph_tags_render__front'] = array(
    '#type' => 'checkbox',
    '#title' => t('Output different tags for front page'),
    '#tree' => FALSE,
    '#default_value' => variable_get('auto_opengraph_tags_render__front', TRUE),
  );
  $form['auto_opengraph_write_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Also use og:description as meta description'),
    '#default_value' => variable_get('auto_opengraph_write_description', FALSE),
    '#description' => t('Useful for some search engines')
  );
  $form['auto_opengraph_style_images'] = array(
    '#type' => 'checkbox',
    '#title' => t('Style og:image according to Facebook specifications'),
    '#default_value' => variable_get('auto_opengraph_style_images', TRUE),
    '#description' => t('At least 50x50 pixels, max aspect ratio of 3:1')
  );

  $entity_types = _auto_opengraph_map_entities(TRUE);
  $auto_opengraph_entity_types = variable_get('auto_opengraph_entity_types', array('node', 'taxonomy_term'));
  $form['auto_opengraph_entity_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Auto opengraph entity types:'),
    '#options' => array_combine(array_keys($entity_types), array_keys($entity_types)),
    '#description' => t('Auto opengraph will manage og meta tags for all selected entity types.'),
    '#default_value' => $auto_opengraph_entity_types,
  );
  foreach ($auto_opengraph_entity_types as $type) {
    $name = ucfirst(str_replace('_', ' ', $type));
    $form[$type] = array(
      '#type' => 'fieldset',
      '#title' => t($name),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$type]["auto_opengraph_tags__{$type}"] = _auto_opengraph_form($type, NULL, t("Overrides for all $type entities"));

    // Form for each bundle
    $info = entity_get_info($type);
    if (count($info['bundles']) > 1) {
      foreach (element_children($info['bundles']) as $bundle) {
        $form[$type]["auto_opengraph_tags__{$type}__{$bundle}"] = _auto_opengraph_form($type, $bundle, t("Overrides for $bundle {$type}s"));
      }
    }

  }
  $form['array_filter'] = array('#type' => 'hidden');
  $form['#submit'] = array('auto_opengraph_form_preprocess');
  return system_settings_form($form);
}

function auto_opengraph_form_preprocess($form, &$form_state) {
  foreach($form_state['values'] as $field => $value) {
    if (stripos($field, 'auto_opengraph_tags__') === 0) {
      $val = $value['Basic'] + $value['Advanced'];
      variable_set($field, array_filter($val));
      unset($form_state['values'][$field]);
    }
  }
}
<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
$plugin = array(
  'title' => t('Provide a Heatmap.'),
  'description' => t('Select a layer that you want to be transformed into a Heatmap.'),
  'type' => 'map',
  'behavior' => array(
    'file' => 'openlayers_heatmaps_behavior.inc',
    'class' => 'openlayers_heatmaps_behavior',
    'parent' => 'openlayers_behavior',
  ),
);

/**
 * Zoom to Max Extent Behavior
 */
class openlayers_heatmaps_behavior extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  function options_init() {
    $options = parent::options_init();
    $options['layers'] = array();
    return $options;
  }

  /**
   * Form defintion for per map customizations.
   */
  function options_form($defaults) {
    $form = parent::options_form(array());

    // Only prompt for vector layers.
    $vector_layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if (isset($layer->data['vector']) && $layer->data['vector'] == TRUE) {
        $vector_layers[$id] = $name;
      }
    }

    foreach ($vector_layers as $id => $name) {

      $layer = openlayers_layer_load($id);

      $form['layers'][$id] = array(
        '#type' => 'fieldset',
        '#title' => t('Options for layer @layer', array('@layer' => $name)),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $form['layers'][$id]['layer'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable the Heatmap ?'),
        '#description' => t("If selected, the layer will be replaced by it's Heatmap") .
            '<br/>(' . check_plain($layer->name) .
            ': ' . check_plain($layer->description) . ')',
        '#default_value' => !empty($defaults['layers'][$id]['layer']) ? 1 : FALSE,
      );
      $form['layers'][$id]['radius'] = array(
        '#title' => t('Radius'),
        '#type' => 'textfield',
        '#description' => t('Select radius (pixels) to apply to Heatmap circles.'),
        '#default_value' => isset($defaults['layers'][$id]['radius']) ? $defaults['layers'][$id]['radius'] : 20,
        '#element_validate' => array('element_validate_integer_positive'),
      );
      $form['layers'][$id]['intensity'] = array(
        '#title' => t('Intensity'),
        '#type' => 'textfield',
        '#description' => t('Select the color intensity.'),
        '#default_value' => isset($defaults['layers'][$id]['intensity']) ? $defaults['layers'][$id]['intensity'] : 0.5,
        '#element_validate' => array('element_validate_integer_positive'),
      );
      $form['layers'][$id]['opacity'] = array(
        '#title' => t('Opacity'),
        '#type' => 'textfield',
        '#description' => t('Select the opacity.'),
        '#default_value' => isset($defaults['layers'][$id]['opacity']) ? $defaults['layers'][$id]['opacity'] : 0.5,
        '#element_validate' => array('element_validate_number'),
      );
    }

    return $form;
  }

  /**
   * Render.
   */
  function render(&$map) {
    // Add the heatmap.js library.
    if ($path = libraries_get_path('heatmap.js')) {
      drupal_add_js($path . '/src/heatmap.js');
      drupal_add_js($path . '/src/heatmap-openlayers.js');
    }
    drupal_add_js(drupal_get_path('module', 'openlayers_heatmaps') . '/openlayers_plugins/behaviors/openlayers_heatmaps_behavior.js');
    return $this->options;
  }
}

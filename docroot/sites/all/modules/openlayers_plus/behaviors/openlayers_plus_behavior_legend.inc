<?php
/**
 * @file
 * A block in a corner of a map that provides information on layers
 */

class openlayers_plus_behavior_legend extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    return array(
      'layers' => array(),
    );
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    $vector_layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if (isset($layer->data['vector']) && $layer->data['vector'] == TRUE) {
        $vector_layers[$id] = $name;

      }
    }

    $options['layers'] = array(
      '#title' => t('Layers'),
      '#type' => 'checkboxes',
      '#options' => $vector_layers,
      '#description' => t('Select layers to add legend.'),
      '#default_value' => isset($defaults['layers']) ? $defaults['layers'] : array(),
    );

    return $options;

  }


  /**
   * Render.
   */
  public function render(&$map) {
    $legend_title;
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_legend.js');
    drupal_add_css(drupal_get_path('module', 'openlayers_plus') . '/css/openlayers_plus.css');
    $options = array();
    $styles = openlayers_styles();
    uasort($map['layers'], 'openlayers_plus_layer_compare');
    foreach ($map['layers'] as $layer => $layer_data) {
      if (isset($map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]) && $map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]) {
        $selected_layer_style = $map['layer_styles'][$layer];
        if ($selected_layer_style == '0') {
          $selected_layer_style = "default";
        }
        else {
          $selected_layer_style = $map['layer_styles'][$layer];
        }
        $legend = array();

        if (!empty($map['layers'][$layer]['title'])) {
          $legend_title = $map['layers'][$layer]['title'];
        }
        else {
          $legend_title = $styles[$selected_layer_style]->title;
        }
        $legend[$layer]['title'] = $legend_title;
        $legend[$layer]['data'] = $styles[$selected_layer_style]->data;
        $options[$layer] = theme('openlayers_plus_legend',
            array(
              'legend' => $legend,
              'layer' => $layer_data,
              'layer_id' => $layer,
            ));
      }
    }
    return $options;
  }
}

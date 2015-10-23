<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */
/**
 * Draw Features behavior
 */
class openlayers_behavior_stylemap extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'hue' => '',
      'lightness' => 0,
      'saturation' => 0,
      'gamma' => 1,
    );
  }

  function options_form($defaults = array()) {
    $features = array(
      'hue' => t('Hue'),
      'lightness' => t('Lightness'),
      'saturation' => t('Saturation'),
      'gamma' => t('Gamma'),
    );
    $google_style_generator = l(t('Google Maps Styles Api'), 'http://gmaps-samples-v3.googlecode.com/svn/trunk/styledmaps/wizard/index.html');
    return array(
      'mode' => array(
        '#type' => 'radios',
        '#title' => t('Settings mode'),
        '#options' => array(
          'all' => t('Feature Type: all'),
          'specific' => t('Specific JSON string'),
        ),
        '#default_value' => isset($defaults['mode']) ? $defaults['mode'] : "all",
      ),
      'all' => array(
        '#type' => 'fieldset',
        '#states' => array(
          'visible' => array(
           ':input[name="behaviors[openlayers_behavior_stylemap][options_set][options][mode]"]' => array('value' => 'all'),
          ),
        ),
        array(
          'visibility' => array(
            '#title' => t('Visibility'),
            '#type' => 'select',
            '#description' => t('The visibility mode'),
            '#options' => array('on'=> t('On'), 'simplified' => t('Simplified'), 'off'=> t('Off')),
            '#default_value' => isset($defaults['all'][0]['visibility']) ? 
              $defaults['all'][0]['visibility'] : "simplified",
          ),
          'invert_lightness' => array(
            '#title' => t('Invert Lightness'),
            '#type' => 'checkbox',
            //'#options' => array('true'=> t('On'), 'simplified' => t('Simplified'), 'off'=> t('Off')),
            '#default_value' => isset($defaults['all'][0]['invert_lightness']) ? 
              $defaults['all'][0]['invert_lightness'] : 0,
          ),
          'hue' => array(
            '#title' => t('Hue'),
            '#type' => 'textfield',
            '#description' => t('The Hue color in hex format (e.g. #FFCC00)'),
            '#default_value' => isset($defaults['all'][0]['hue']) ? 
              $defaults['all'][0]['hue'] : "",
          ),
          'lightness' => array(
            '#type' => 'textfield',
            '#default_value' => (isset($defaults['all'][0]['lightness'])) ?
              $defaults['all'][0]['lightness'] : '0',
            '#title' => t('Lightness'),
            '#description' => t('The Lightness value (int between -100 and 100)'),
          ),
          'saturation' => array(
            '#title' => t('Saturation'),
            '#type' => 'textfield',
            '#description' => t('The Saturation value (int between -100 and 100)'),
            '#default_value' => isset($defaults['all'][0]['saturation']) ? 
              $defaults['all'][0]['saturation'] : "0",
          ),
          'gamma' => array(
            '#title' => t('Gamma'),
            '#type' => 'textfield',
            '#description' => t('The gamma value (int between 0 and 10)'),
            '#default_value' => isset($defaults['all'][0]['gamma']) ? 
              $defaults['all'][0]['gamma'] : "1",
          ),
        ),
      ),
      'specific' => array(
        '#type' => 'fieldset',
        '#states' => array(
          'visible' => array(
           ':input[name="behaviors[openlayers_behavior_stylemap][options_set][options][mode]"]' => array('value' => 'specific'),
          ),
        ),
        array(
          'json' => array(
            '#title' => t('Valid Json String'),
            '#type' => 'textarea',
            '#description' => t('a Valid Json string.<br />e.g. [ { "featureType": "water", "stylers": [ { "hue": "#00fff7" } ] } ]'),
            '#default_value' => isset($defaults['specific'][0]['json']) ? 
              $defaults['specific'][0]['json'] : "",
          ),
        ),
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers_stylemaps') . 
      '/includes/behaviors/openlayers_behavior_stylemap.js');
    return $this->options;
  }
}

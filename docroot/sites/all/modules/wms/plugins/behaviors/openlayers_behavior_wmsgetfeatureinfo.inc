<?php
// $Id$

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * WMSGetfeatureinfo Behavior
 */

$plugin = array(
    'openlayers_behavior_wmsgetfeatureinfo' => array(
        'title' => t('Get Feature Info'),
        'description' => t('Queries WMS server for more info about features on the map.'),
        'type' => 'layer',
        'behavior' => array(
          'file' => 'openlayers_behavior_wmsgetfeatureinfo.inc',
          'class' => 'openlayers_behavior_wmsgetfeatureinfo',
          'parent' => 'openlayers_behavior',
        ),
    ),
);


class openlayers_behavior_wmsgetfeatureinfo extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array();
  }

  function options_form($defaults) {
    $info_formats = array(
      'text/plain' => 'text/plain',
      'text/html' => 'text/html',
      'application/vnd.ogc.gml' => 'application/vnd.ogc.gml',
    );
    // TODO only return form when there is at least 1 WMS layer
    return array(
      'getfeatureinfo_usevisiblelayers' => array(
        '#title' => "Use All Visible Layers",
        '#type' => 'checkbox',
        '#description' => t('Query all visible WMS layers on the map.'),
        '#default_value' => isset($defaults['getfeatureinfo_usevisiblelayers']) ?
          $defaults['getfeatureinfo_usevisiblelayers'] : TRUE,
      ),
      'getfeatureinfo_layers' => array(
        '#title' => "Only Query",
        '#type' => 'select',
        '#options' => $this->map['layers'], // TODO filter this on WMS layers only
        '#description' => t("Select the layer from which points are pulled. This must be a WMS layer. 'Use Visible Layers' must be unchecked for this option to have effect"), //TODO Fix this in javascript
        '#default_value' => isset($defaults['getfeatureinfo_layers']) ?
          $defaults['getfeatureinfo_layers'] : NULL,
        '#states' => array(
          // Only show this field when the region is selected.
          'visible' => array(
            ':input[name="behaviors[openlayers_behavior_wmsgetfeatureinfo][options_set][options][getfeatureinfo_usevisiblelayers]"]' => array('checked' => FALSE),
          ),
        ),
      ),
      'getfeatureinfo_info_format' => array(
        '#title' => "Format ",
        '#type' => 'select',
        '#options' => $info_formats,
        '#description' => t("The format the server should return. text formats are displayed as-is. The Gml
          format is displayed in a Drupal table, and themeable."),
        '#default_value' => isset($defaults['getfeatureinfo_info_format']) ?
          $defaults['getfeatureinfo_info_format'] : NULL,
      ),
      'getfeatureinfo_htmlelement' => array(
        '#type' => 'textfield',
        '#description' => t("An HTML element (#id!) that will be filled
          with the query result. Ommit the # here. If you use the GetFeatureInfo block, the value should be 'getfeatureinfo'"),
        '#default_value' => isset($defaults['getfeatureinfo_htmlelement']) ?
          $defaults['getfeatureinfo_htmlelement'] : "getfeatureinfo",
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'wms') . '/plugins/behaviors/js/openlayers_behavior_wmsgetfeatureinfo.js');
    return $this->options;
  }
}


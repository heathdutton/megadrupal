<?php

/**
 * @file
 * Extension of the Views Plugin Style for D3 Map
 * Adapted from the GeoField Map views module and the OpenLayers Views module.
 */

class d3map_views_plugin_style extends views_plugin_style {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['data_source'] = array('default' => '');
    $options['height'] = array('default' => '500');
    $options['width'] = array('default' => '960');
    $options['filter'] = array('default' => 'state');
    $options['territories'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Get list of fields in this view & flag available geodata fields:
    $handlers = $this->display->handler->get_handlers('field');

    $fields = array();
    $fields_data = array();
    foreach ($handlers as $field_id => $handler) {
      $fields[$field_id] = $handler->ui_name();

      // ToDo declare a custom field type
      //if (!empty($handler->field_info['type']) && $handler->field_info['type'] == 'list_text') {
        // I currently allow any field type which means it is up to the user
        // to ensure it provides the correct values
        $fields_data[$field_id] = $handler->ui_name();
      //}
    }

    // Check whether we have a field we can work with:
    if (!count($fields_data)) {
      $form['error'] = array(
        '#markup' => t('Please add at least one List Text field to the view'),
      );
      return;
    }

    // Map preset.
    $form['data_source'] = array(
      '#type' => 'select',
      '#title' => t('Data Source'),
      '#description' => t('Which field contains data?'),
      '#options' => $fields_data,
      '#default_value' => $this->options['data_source'],
      '#required' => TRUE,
    );

    $form['height'] = array(
      '#title' => t('Map height'),
      '#type' => 'textfield',
      '#field_suffix' => t('px'),
      '#size' => 4,
      '#default_value' => $this->options['height'],
      '#required' => FALSE,
    );

    $form['width'] = array(
      '#title' => t('Map width'),
      '#type' => 'textfield',
      '#field_suffix' => t('px'),
      '#size' => 4,
      '#default_value' => $this->options['width'],
      '#required' => FALSE,
    );

    $form['filter'] = array(
      '#title' => t('Filter string'),
      '#description' => t('What is the name of the exposed views form filter? Ex. State'),
      '#type' => 'textfield',
      '#default_value' => $this->options['filter'],
      '#required' => FALSE,
    );

    $form['territories'] = array(
      '#title' => t('Show Territories'),
      '#type' => 'checkbox',
      '#description' => t('Show territories on US map display.'),
      '#default_value' => isset($this->options['territories']) ? $this->options['territories'] : FALSE,
    );

    $form['hide_empty'] = array(
      '#title' => t('Hide empty'),
      '#type' => 'checkbox',
      '#description' => t('Hide the D3 map if there are no results to display.'),
      '#default_value' => isset($this->options['hide_empty']) ? $this->options['hide_empty'] : TRUE,
    );
  }

  /**
   * Validate the options form.
   */
  function options_validate(&$form, &$form_state) {
    if (!is_numeric($form_state['values']['style_options']['height']) || $form_state['values']['style_options']['height'] < 0) {
      form_error($form['height'], t('Map height needs to be a positive number'));
    }
    if (!is_numeric($form_state['values']['style_options']['width']) || $form_state['values']['style_options']['width'] < 0) {
      form_error($form['width'], t('Map width needs to be a positive number'));
    }
  }

  /**
   * Renders D3 map view.
   */
  function render() {
    if (!empty($this->view->live_preview)) {
      return t('No preview available');
    }
    $data = array();
    $content = array();

    // Is there a data field selected?
    if ($this->options['data_source']) {
      $this->render_fields($this->view->result);
      foreach ($this->view->result as $id => $result) {
        $datafield = $this->get_field_value($id, $this->options['data_source']);

        if (!empty($datafield)) {
          // Set shape var to the rendered data field item
          $shape = $this->rendered_fields[$id][$this->options['data_source']];
          
          // Let modules modify the shape data.
          drupal_alter('d3map_views_alter_shape_data', $result, $shape);

          // If field is a taxonomy field use the tid value
          $field_value = (isset($datafield[0]['tid'])) ? $datafield[0]['tid'] : $shape;

          // Add this shape identifiers into the $data array for map rendering:
          $data[$shape] = $field_value;
        }
      }

      // Remove duplicate shape identifiers
      $data = array_unique($data);

      // If no data and hide_empty option is selected return nothing
      if (empty($data) && !empty($this->options['hide_empty'])) {
        return '';
      }

      // Set settings values and store these values as JS settings
      $d3map_views_settings = array(
        'width' => $this->options['width'], 
        'height' => $this->options['height'],
        'activePaths' => json_encode($data, JSON_FORCE_OBJECT),
        'mapPath' => drupal_get_path('module', 'd3map_views') . '/maps/us.json',
        'paramName' => $this->options['filter'],
        'showTerritories' => $this->options['territories'],
      );
      drupal_add_js(array('d3map_views' => $d3map_views_settings), 'setting');

      // Get the D3 include path from loading the D3 library
      $d3_include_path = d3map_views_d3_include_path();

      $content = array(
        '#markup' => theme('d3map_views_map', array(
          'width' => $this->options['width'] . 'px', 
          'height' => $this->options['height'] . 'px',
        )),
        '#attached' => array(
          'js' => array(
            $d3_include_path->uri,
            drupal_get_path('module', 'd3map_views') . '/js/d3map.js' => array('scope' => 'footer'),
          ),
          'css' => array(
            drupal_get_path('module', 'd3map_views') . '/css/d3map.css',
          ),
        ),
      );
    }
    return $content;
  }
}

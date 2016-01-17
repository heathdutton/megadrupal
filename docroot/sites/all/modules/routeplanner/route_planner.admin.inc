<?php
/**
 * @file
 * Administrative page callbacks for the route_planner module.
 */

/**
 * Define a form to set the fixed target point.
 *
 * @return
 *   The general settings form code stored in the $form variable, before
 *   converted to HTML.
 */
function route_planner_settings_form() {
  $form['route_planner'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Route Planner Settings'),
  );

  $form['route_planner']['route_planner_address'] = array(
    '#type'          => 'textfield',
    '#description'   => t('Your point of interesst or company address.'),
    '#title'         => t('Target address'),
    '#default_value' => variable_get('route_planner_address', 'Hamburg, Germany'),
  );
  $form['route_planner']['route_planner_address_end'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show an end point field.'),
    '#description'   => t('If checked the address block will have a end point field with the default address from your POI above.'),
    '#default_value' => variable_get('route_planner_address_end', FALSE),
  );
  $form['route_planner']['route_planner_unitsystem'] = array(
    '#type'          => 'select',
    '#description'   => t('Select your preferred unit system IMPERIAL or METRIC.'),
    '#title'         => t('Unit System'),
    '#options'       => array(
      0 => t('metric'),
      1 => t('imperial'),
    ),
    '#default_value' => variable_get('route_planner_unitsystem', 0),
  );

  $form['map-settings'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Map Settings'),
  );
  $form['map-settings']['route_planner_map_height'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Map Height'),
    '#description'   => t('A fixed height for example 300px.'),
    '#size'          => 10,
    '#default_value' => variable_get('route_planner_map_height', '300px'),
  );
  $form['map-settings']['route_planner_map_width'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Map Width'),
    '#description'   => t('A width value in % or px, for example 300px or 100%.'),
    '#size'          => 10,
    '#default_value' => variable_get('route_planner_map_width', '100%'),
  );
  $form['map-settings']['route_planner_map_zoom'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Zoom Level'),
    '#description'   => t('A value between 1 and 100 (a normal value is around 10).'),
    '#size'          => 10,
    '#default_value' => variable_get('route_planner_map_zoom', 10),
  );
  $form['map-settings']['route_planner_map_defaultui'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable Default UI Controls'),
    '#description'   => t('Disables all UI Controls in the map.'),
    '#default_value' => variable_get('route_planner_map_defaultui', TRUE),
  );
  $form['map-settings']['route_planner_map_zoomcontrol'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable ZoomControl'),
    '#default_value' => variable_get('route_planner_map_zoomcontrol', TRUE),
  );
  $form['map-settings']['route_planner_map_scrollwheel'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable ScrollWheel'),
    '#default_value' => variable_get('route_planner_map_scrollwheel', TRUE),
  );
  $form['map-settings']['route_planner_map_maptypecontrol'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Map Type Control'),
    '#default_value' => variable_get('route_planner_map_maptypecontrol', TRUE),
  );
  $form['map-settings']['route_planner_map_scalecontrol'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Scale Control'),
    '#default_value' => variable_get('route_planner_map_scalecontrol', TRUE),
  );
  $form['map-settings']['route_planner_map_draggable'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Mouse Drag'),
    '#default_value' => variable_get('route_planner_map_draggable', TRUE),
  );
  $form['map-settings']['route_planner_map_doubbleclick'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable Doubble Click Zoom'),
    '#default_value' => variable_get('route_planner_map_doubbleclick', TRUE),
  );
  $form['map-settings']['route_planner_map_streetviewcontrol'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Streetview Control'),
    '#default_value' => variable_get('route_planner_map_streetviewcontrol', TRUE),
  );
  $form['map-settings']['route_planner_map_overviewmapcontrol'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Overview Map'),
    '#default_value' => variable_get('route_planner_map_overviewmapcontrol', TRUE),
  );
  $form['map-settings']['route_planner_map_travelmode'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Travel Mode'),
    '#default_value' => variable_get('route_planner_map_travelmode', FALSE),
  );
  $form['map-style'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Map Style Settings'),
    '#description' => t('Optional Style Settings as described at <a target="_blank" href="@google_dev_url">Google DeveloperStyles</a>. You can find a lot of sample styles at <a target="_blank" href="@snazzymaps_url"> snazzymaps.com</a>.', array(
      '@google_dev_url' => url('https://developers.google.com/maps/documentation/javascript/styling#style_array_example'),
      '@snazzymaps_url' => url('http://snazzymaps.com/')
    )),
  );
  $form['map-style']['route_planner_map_style'] = array(
    '#type'          => 'textarea',
    '#title'         => 'Style Array',
    '#default_value' => variable_get('route_planner_map_style'),
  );
  return system_settings_form($form);
}

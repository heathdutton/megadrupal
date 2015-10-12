<?php

define('MAPBOXJS_PRESET_NAME_LENGTH', 128);

/**
 * Return a form for a mapbox.js map preset entity.
 *
 * @param $form
 * @param $form_state
 * @param null $preset
 *
 * @return mixed
 */
function mapboxjs_preset_form($form, &$form_state, MapboxjsPreset $preset = NULL, $op = 'edit') {
    if ($op == 'clone') {
    $preset->label .= ' (cloned)';
    $preset->name = '';
  }
  $form['#id'] = 'maboxjs_preset_form';
  $form_state['maboxjs_preset'] = $preset;

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($preset) ? $preset->label : '',
    '#description' => t('The human-readable name of this MapBox.js map preset.'),
    '#required' => TRUE,
    '#maxlength' => MAPBOXJS_PRESET_NAME_LENGTH,
    '#size' => 30,
  );
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'machine_name',
    '#description' => t(''),
    '#default_value' => isset($preset) ? $preset->name : '',
    '#disabled' => !empty($preset->name),
    '#machine_name' => array(
      'exists' => 'mapboxjs_get_presets',
      'source' => array('label')
    ),
  );
  // Provide preset administrator with control over the map container height.
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Mabox map height'),
    '#description' => t('Enter the height in pixels for this map.'),
    '#default_value' => isset($preset->height) ? $preset->height : '500',
    '#size' => '5',
    '#field_suffix' => t('px'),
    '#element_validate' => array('element_validate_integer_positive'),
  );
  // Set the lat/lon center points.
  $form['center'] = array(
    '#type' => 'fieldset',
    '#title' => t('Center point'),
    '#description' => t('Enter the lat/long center point of this map in decimal degrees.'),
    '#collapsible' => FALSE,
  );
  $form['center']['lat'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#default_value' => isset($preset->lat) ? $preset->lat : 0,
  );
  $form['center']['lon'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#default_value' => isset($preset->lon) ? $preset->lon : 0,
  );
  // Set default zoom level.
  $form['zoom'] = array(
    '#type' => 'select',
    '#title' => t('Zoom level'),
    '#description' => t('Select the default zoom level for this map.'),
    '#options' => drupal_map_assoc(array(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20)),
    '#default_value' => isset($preset->zoom) ? $preset->zoom : 8,
  );
  // Enable/disable zoom controls.
  $form['zoomer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide zoom controls'),
    '#default_value' => isset($preset->zoomer) ? $preset->zoomer : TRUE,
  );
  // Enable/disable fullscreen control.
  $form['fullscreen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide fullscreen map option'),
    '#default_value' => isset($preset->fullscreen) ? $preset->fullscreen : FALSE,
  );
  // Enable/disable map legend.
  $form['legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide a map legend, if included with MapBox tileset(s).'),
    '#default_value' => isset($preset->legend) ? $preset->legend : FALSE,
  );
  // Enable/disable layer toggle control.
  $form['layer_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide layer toggle controls'),
    '#description' => t('Select this option to present a layer switcher to toggle on/off the tileset layers provided above.'),
    '#default_value' => isset($preset->layer_toggle) ? $preset->layer_toggle : FALSE,
  );
  // Add an optional base tileset layer.
  $form['base_layer'] = array(
    '#type' => 'textfield',
    '#title' => t('Base tileset'),
    '#description' => t('Optionally, you can enter the URL of a base tileset, such as a terrain layer. This layer must support the <a href="https://github.com/mapbox/tilejson-spec">TileJSON</a> format.'),
    '#default_value' => isset($preset->base_layer) ? $preset->base_layer : '',
    '#element_validate' => array('mapboxjs_base_layer_validate'),
  );
  // Enable/disable tileset compositing.
  $form['composite'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable tileset compositing'),
    '#description' => t('By default, multiple tilesets will be returned by the MapBox API as a single, composited tileset. This improves performance and reduces the number of tile requests to the MapBox API. In certain cases, however, it is helpful to disable compositing for smoother transitions when toggling on/off one or more tilesets.'),
    '#default_value' => isset($preset->composite) ? $preset->composite : TRUE,
  );

  // Attach our locked link field.
  field_attach_form('mapboxjs_preset', $preset, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#value' => t('Save preset'),
    '#type' => 'submit',
  );

  // If map preset has been saved, display map preview below the form.
  // @TODO - Add better handling for invalid map previews.
  // @TODO - Add ajax-based preview that doesn't require saving the map preset.
  if ($form_state['op'] == 'edit') {
    $preview = mapboxjs_render_preset($form_state['mapboxjs_preset']);
    $preview['#prefix'] = '<h3>MapBox map preset preview</h3>';
    $form['#suffix'] = drupal_render($preview);
  }

  return $form;
}

/**
 * Helper function to validate base tileset URL.
 * @TODO - Consider adding this validation to mapboxjs_preset_form_validate().
 */
function mapboxjs_base_layer_validate($element, &$form_state, $form) {
  if (!empty($element['#value']) && !(valid_url($element['#value'], TRUE))) {
    form_error($element, t('Enter a valid URL or leave this field blank.'));
  }
}

/**
 * Validation handler for mapboxjs_preset_form().
 */
function mapboxjs_preset_form_validate($form, &$form_state) {
  // Validate lat/lon values.
  $lat = $form_state['values']['lat'];
  $lon = $form_state['values']['lon'];
  if (!empty($lat) & (!is_numeric($lat) || $lat > 90  || $lat < -90)) {
    form_set_error('lat', t('Enter the latitude of the center point as a decimal between -90 and 90.'));
  }
  if (!empty($lon) & (!is_numeric($lon) || $lon > 180  || $lon < -180)) {
    form_set_error('lon', t('Enter the longitude of the center point as a decimal between -180 and 180.'));
  }
  // Call to field_attach_form_validate() ensures that locked link field is validated.
  $preset = $form_state['mapboxjs_preset'];
  field_attach_form_validate('mapboxjs_preset', $preset, $form, $form_state);
  // @FIXME - For some reason, link field URL is not getting validated.
}

/**
 * Submit handler for mapboxjs_preset_form().
 *
 * @param $form
 * @param $form_state
 */
function mapboxjs_preset_form_submit($form, &$form_state) {

  $values = $form_state['values'];

  if ($form_state['mapboxjs_preset']->name) {
    $preset = $form_state['mapboxjs_preset'];
    foreach ($values as $key => $value) {
      $preset->{$key} = $value;
    }
  }
  else {
    $preset = entity_create('mapboxjs_preset', $values);
  }

  // Make sure that attached link field is saved with our custom entity.
  field_attach_submit('mapboxjs_preset', $preset, $form, $form_state);

  $preset->save();

  drupal_set_message(t('MapBox.js map preset saved.'));
}

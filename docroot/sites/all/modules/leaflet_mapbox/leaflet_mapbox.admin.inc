<?php

/**
 * @file
 * Leaflet MapBox module admin file.
 */

function leaflet_mapbox_configuration_form($form, &$form_state) {
  $form['leaflet_mapbox_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Map label'),
    '#required' => TRUE,
    '#default_value' => variable_get('leaflet_mapbox_label', ''),
    '#description' => t('Give your map a name, this will be the name shown on the display options form.'),
  );
  $form['leaflet_mapbox_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Map code'),
    '#required' => TRUE,
    '#default_value' => variable_get('leaflet_mapbox_code', ''),
    '#description' => t('This code is obtained from MapBox by clicking on the mapbox.js button after publishing your map'),
  );
  $form['leaflet_mapbox_zoomlevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Zoom Level'),
    '#required' => TRUE,
    '#default_value' => variable_get('leaflet_mapbox_zoomlevel', 2),
    '#description' => t('You must clear the site caches after changing this value or wait for the caches to expire before this change shows'),
  );
  $form['leaflet_mapbox_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Map description'),
    '#default_value' => variable_get('leaflet_mapbox_description', ''),
  );
  return system_settings_form($form);
}

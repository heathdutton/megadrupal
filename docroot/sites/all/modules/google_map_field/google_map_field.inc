<?php

// Taken from https://developers.google.com/maps/documentation/javascript/reference
$settings = array(
  'disableDoubleClickZoom' => array(
    'description' => 'Enables/disables zoom on double click. Disabled by default.',
    'default' => false
  ),
  'draggable' => array(
    'description' => 'If false, prevents the map from being dragged. Dragging is enabled by default.',
    'default' => true
  ),
  'keyboardShortcuts' => array(
    'description' => 'If false, prevents the map from being controlled by the keyboard. Keyboard shortcuts are enabled by default.',
    'default' => false
  ),
  'noClear' => array(
    'description' => 'If true, do not clear the contents of the Map div.',
    'default' => false
  ),
  'panControl' => array(
    'description' => 'The enabled/disabled state of the Pan control.',
    'default' => true
  ),
  'rotateControl' => array(
    'description' => 'The enabled/disabled state of the Rotate control.',
    'default' => false
  ),
  'scaleControl' => array(
    'description' => 'The initial enabled/disabled state of the Scale control.',
    'default' => true
  ),
  'scrollwheel' => array(
    'description' => 'If false, disables scrollwheel zooming on the map. The scrollwheel is enabled by default.',
    'default' => true
  ),
  'streetViewControl' => array(
    'description' => 'The initial enabled/disabled state of the Street View Pegman control. This control is part of the default UI, and should be set to false when displaying a map type on which the Street View road overlay should not appear (e.g. a non-Earth map type).',
    'default' => false
  ),
  'zoomControl' => array(
    'description' => 'The enabled/disabled state of the Zoom control.',
    'default' => true
  ),
);

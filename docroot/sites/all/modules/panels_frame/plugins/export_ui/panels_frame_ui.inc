<?php

/**
 * Abstract base plugin for Panels Frame Export UI.
 */
$plugin = array(
  'has menu' => FALSE,
  'handler' => array(
    'class' => 'panels_frame_ui',
    'parent' => 'ctools_export_ui',
  ),
);

<?php

// theme name
$base_theme = "wrappedsites";
// include template hooks
include_once(drupal_get_path('theme', $base_theme) .'/inc/template-hooks.inc');

// include custom functions
include_once(drupal_get_path('theme', $base_theme) .'/inc/template-functions.inc');

// include initialize theme settings
include_once(drupal_get_path('theme', $base_theme) .'/inc/template-init.inc');
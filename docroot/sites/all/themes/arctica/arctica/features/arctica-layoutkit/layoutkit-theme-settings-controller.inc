<?php

if (is_file($arctica_theme_path . '/features/arctica-layoutkit/arctica-easygrid/css/arctica-easy-grid.css')) {
  drupal_add_css($arctica_theme_path . '/features/arctica-layoutkit/arctica-easygrid/css/arctica-easy-grid.css');
}

if (theme_get_setting('hide_address_bar')) {
  drupal_add_js('if(window.addEventListener){window.addEventListener("load",function(){setTimeout(function(){window.scrollTo(0,0.9)},0)});}', 'inline');
}

if (theme_get_setting('ori_scale')) {
  drupal_add_js($arctica_theme_path . '/features/arctica-layoutkit/ori-scale.min.js', array('every_page' => TRUE, 'group' => JS_LIBRARY, 'weight' => -21));
}
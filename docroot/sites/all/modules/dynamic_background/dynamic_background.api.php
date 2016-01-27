<?php

/**
 * @file
 * Hooks provided by the dynamic background module.
 *
 */

/**
 * Sub-modules that provieds image selection to the css generation process have
 * to implement this hook.
 *
 * @param array $vars
 * @return array
 */
function hook_dynamic_background_css($vars) {
  return array(
    'image' => 'PATH TO IMAGE',
    'css_configuration' => array(
      'selector' => 'CSS SELECTOR',
      'css' => 'CSS OPTIONS',
    ),
    'image_style' => 'IMAGE STYLE NAME',
  );
}

/**
 * Sub-modules that provieds image selection to the css generation process have
 * to implement this hook. It's used to determind the default weight of the
 * sub-module.
 */
function hook_dynamic_background_weight() {
  return array(
    'weight' => 'WEIGHT OF THE CSS',
  );
}
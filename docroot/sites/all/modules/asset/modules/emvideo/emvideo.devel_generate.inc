<?php

/**
 * Implements hook_devel_generate().
 */
function emvideo_devel_generate($object, $field, $instance, $bundle) {
  // Get a random video from a random enabled provider
  $modules = module_implements('emvideo_generate');
  $module = $modules[array_rand($modules)];
  $url = module_invoke($module, 'emvideo_generate');
  return array(
    'url' => $url,
  );
}

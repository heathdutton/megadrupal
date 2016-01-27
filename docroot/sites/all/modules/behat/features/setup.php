<?php
function behat_feature_setup($name = '') {
  $setup = array(
    // Feature
    'example' => array(
      // Modules required by the feature
      'modules' => array(
        // e.g. 'panels', 'custom_module', ...
      ),
    ),
  );
  
  return (!empty($name)) ? $setup[$name] : $setup;
}
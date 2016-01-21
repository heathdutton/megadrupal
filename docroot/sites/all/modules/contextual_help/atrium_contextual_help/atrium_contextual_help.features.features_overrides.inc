<?php
/**
 * @file
 * atrium_contextual_help.features.features_overrides.inc
 */

/**
 * Implements hook_features_override_default_overrides().
 */
function atrium_contextual_help_features_override_default_overrides() {
  // This code is only used for UI in features. Exported alters hooks do the magic.
  $overrides = array();

  // Exported overrides for: panels_mini
  $overrides["panels_mini.oa_toolbar_panel.display|content|new-9"] = (object) array(
      'pid' => 'new-9',
      'panel' => 'contentmain',
      'type' => 'atrium_contextual_help_button',
      'subtype' => 'atrium_contextal_help_button',
      'shown' => TRUE,
      'access' => array(),
      'css' => array(
        'css_id' => '',
        'css_class' => 'pull-right',
      ),
      'cache' => array(),
      'extras' => array(),
      'position' => 6,
      'locks' => array(),
    );
  $overrides["panels_mini.oa_toolbar_panel.display|panels|contentmain|6"] = 'new-9';

 return $overrides;
}

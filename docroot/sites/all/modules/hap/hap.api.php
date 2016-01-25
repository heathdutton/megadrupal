<?php

/**
 * @file
 *   Example hooks and stuff.
 */

/**
 * Provide meta-information about your tool to systems such as the "Attractive
 * Installer". The "Attractive Installer" requires this hook to be implemented
 * by your tool in order to allow your tool to be enabled/disabled. "Enabling"
 * a tool only really means it is possible to implement its OWN hooks.
 * Otherwise, tools quietly sit in the includes directory without Drupal even
 * knowing about them.
 */
function hook_hap_info() {
  return array(
    // The Human-readable name for your tool.
    'name' => 'Crazy Horse',

    // A brief description about what your tool does.
    'description' => t('Does unimaginable things to your page displays.'),

    // Define the Drupal version this tool is compatible with. Defaults to the
    // current Drupal version.
    'core' => '7.x',

    // A human-readable version label for wherever tools are usually displayed,
    // such as the "Attractive Installer" page. Defaults to the current Drupal and
    // HAP version.
    'version' => '7.x-3.1',

    // The minimal php version your tool is compatible with. Defaults to
    // Drupal's current PHP minimum.
    'php' => '5.3',

    // This determines whether a tool should ALWAYS be enabled and bypasses the
    // user option to enable/disable it in the "Attractive Installer". This is
    // only really applicable to the "core" tool that contains global
    // utilities.
    'always enabled' => FALSE,

    // Defines which hooks this module utilizes.
    'hooks' => array('menu', 'theme', 'form_alter', 'page_alter'),

    // List module/tool dependencies. This will only ensure that
    // enabling/disabling a tool is possible when the dependency is met. It
    // does not ensure the depended tool is automatically included.
    'dependencies' => array(
      'modules' => array('panels'),
      'tools' => array('regex', 'mail'),
    ),
  );
}

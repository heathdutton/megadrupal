<?php
/**
 * @file
 * atrium_contextual_help.features.inc
 */

/**
 * Implements hook_default_panels_mini_alter().
 */
function atrium_contextual_help_default_panels_mini_alter(&$data) {
  if (isset($data['oa_toolbar_panel'])) {
    $data['oa_toolbar_panel']->display->content['new-9'] = (object) array(
          'pid' => 'new-9',
          'panel' => 'contentmain',
          'type' => 'atrium_contextual_help_button',
          'subtype' => 'atrium_contextal_help_button',
          'shown' => TRUE,
          'css' => array(
            'css_id' => '',
            'css_class' => 'pull-right',
          ),
          'access' => array(),
          'cache' => array(),
          'extras' => array(),
          'position' => 6,
          'locks' => array(),
          'configuration' => array(),
        ); /* WAS: '' */
    $data['oa_toolbar_panel']->display->panels['contentmain']['6'] = 'new-9'; /* WAS: '' */
  }
}

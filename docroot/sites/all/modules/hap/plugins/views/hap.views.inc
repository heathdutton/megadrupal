<?php

/**
 * Implements hook_views_plugins().
 */
function hap_views_plugins() {
  return array(
    'style' => array(
      'nah' => array(
        'title' => t('★ Nah..'),
        'help' => t('Omits wrapping divs for each row.'),
        'handler' => 'views_plugin_style',
        'theme' => 'hap_nah_views_plugin_style',
        'theme path' => drupal_get_path('module', 'hap') . '/plugins/views/style/nah',
        'theme file' => 'theme.inc',
        'uses row plugin' => TRUE,
        'uses row class' => FALSE,
        'uses grouping' => FALSE,
        'uses options' => FALSE,
        'type' => 'normal',
        // 'help topic' => 'hap-nah-views-plugin-style',
        // @todo Write some documentation explaining that if you are using this
        //  style plugin, you should probably also be responsible for
        // implementing some template overrides for the View's rows. Otherwise
        // you'll have a mess of fields rendered one after the other with no
        // grouping.. Unless you like that sort of thing.
      ),
    ),
  );

}

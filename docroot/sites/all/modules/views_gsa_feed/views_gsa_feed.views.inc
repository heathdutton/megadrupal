<?php
/**
 * @file
 * Views include file with views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function views_gsa_feed_views_plugins() {
  $path = drupal_get_path('module', 'views_gsa_feed');

  $style_defaults = array(
    'path' => $path . '/plugins',
    'parent' => 'views_gsa_feed',
    'theme' => 'views_gsa_feed',
    'theme path' => $path . '/theme',
    'theme file' => 'views_gsa_feed.theme.inc',
    'uses row plugin' => FALSE,
    'uses fields' => TRUE,
    'uses options' => TRUE,
    'type' => 'gsa_feed',
  );

  return array(
    'display' => array(
      'views_gsa_feed' => array(
        'title' => t('GSA Feed'),
        'help' => t('Create a feed to be indexed by an external Google Search Appliance.'),
        'path' => $path . '/plugins',
        'handler' => 'ViewsGsaFeedPluginDisplay',
        'parent' => 'feed',
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('gsa feed'),
        'help topic' => 'display-gsa-feed',
      ),
    ),
    'style' => array(
      'views_gsa_feed' => array(
        // This isn't really a display but is necessary so the file can
        // be included.
        'no ui' => TRUE,
        'handler' => 'ViewsGsaFeedPluginStyle',
        'path' => $path . '/plugins',
        'theme path' => $path . '/theme',
        'theme file' => 'views_gsa_feed.theme.inc',
        'type' => 'normal',
      ),
      'views_gsa_feed_xml' => array(
        'title' => t('GSA Feed'),
        'help' => t('Display the view feed.'),
        'handler' => 'ViewsGsaFeedPluginStyleXml',
        'headers' => array('Content-Type' => 'text/xml'),
        'additional themes' => array(
          'views_gsa_feed_xml_header' => 'style',
          'views_gsa_feed_xml_body' => 'style',
          'views_gsa_feed_xml_footer' => 'style',
        ),
        'additional themes base' => 'views_gsa_feed_xml',
      ) + $style_defaults,
    ),
  );
}

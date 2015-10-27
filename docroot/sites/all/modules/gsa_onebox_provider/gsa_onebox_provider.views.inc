<?php
/**
 * @file
 * Views include file with views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function gsa_onebox_provider_views_plugins() {
  $path = drupal_get_path('module', 'gsa_onebox_provider');

  $style_defaults = array(
    'path' => $path . '/plugins',
    'parent' => 'gsa_onebox_provider',
    'theme' => 'gsa_onebox_provider',
    'theme path' => $path . '/theme',
    'theme file' => 'gsa_onebox_provider.theme.inc',
    'uses row plugin' => FALSE,
    'uses fields' => TRUE,
    'uses options' => TRUE,
    'type' => 'gsa_onebox_provider',
  );

  return array(
    'display' => array(
      'gsa_onebox_provider' => array(
        'title' => t('GSA Onebox Provider'),
        'help' => t('Create a feed to act as a Google Search Appliance Onebox provider.'),
        'path' => $path . '/plugins',
        'handler' => 'GSAOneboxProviderPluginDisplay',
        'parent' => 'feed',
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('gsa onebox provider'),
        'help topic' => 'display-gsa-onebox-provider',
      ),
    ),
    'style' => array(
      'gsa_onebox_provider' => array(
        // This isn't really a display but is necessary so the file can
        // be included.
        'no ui' => TRUE,
        'handler' => 'GSAOneboxProviderPluginStyle',
        'path' => $path . '/plugins',
        'theme path' => $path . '/theme',
        'theme file' => 'gsa_onebox_provider.theme.inc',
        'type' => 'normal',
      ),
      'gsa_onebox_provider_xml' => array(
        'title' => t('GSA Onebox Provider'),
        'help' => t('Display the view feed.'),
        'handler' => 'GSAOneboxProviderPluginStyleXml',
        'headers' => array('Content-Type' => 'text/xml'),
        'additional themes' => array(
          'gsa_onebox_provider_xml_header' => 'style',
          'gsa_onebox_provider_xml_body' => 'style',
          'gsa_onebox_provider_xml_footer' => 'style',
        ),
        'additional themes base' => 'gsa_onebox_provider_xml',
      ) + $style_defaults,
    ),
  );
}

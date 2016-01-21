<?php

/**
 * @file
 * Contains an implementation of vCard items based on fields on a row plugin.
 */

/**
 * Implements hook_views_plugins().
 */
function views_vcards_views_plugins() {
  return array(
    'display' => array(
      'views_vcards_display' => array(
        'title' => t('vCard'),
        'handler' => 'views_vcards_plugin_display_vcard',
        'type' => 'feed',
        'admin' => t('vCard'),
        'contextual links locations' => array(),
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
      ),
    ),
    'style' => array(
      'views_vcards_style' => array(
        'title' => t('vCard'),
        'handler' => 'views_vcards_plugin_style_vcard',
        'uses options' => FALSE,
        'type' => 'feed',
        'theme' => 'views_vcards_view_vcard',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'views_vcards') . '/views/theme',
        'uses row plugin' => TRUE,
      ),
    ),
    'row' => array(
      'views_vcards_row' => array(
        'title' => t('vCards'),
        'handler' => 'views_vcards_plugin_row_vcard',
        'uses options' => TRUE,
        'type' => 'feed',
        'theme' => 'views_vcards_view_row_vcard',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'views_vcards') . '/views/theme',
        'uses fields' => TRUE,
      ),
    ),
  );
}

<?php

/**
 * @file
 * This file defines the CiviCRM Dashlet display plugin.
 */

/**
 * Implements hook_views_plugins().
 */
function views_civicrm_dashlets_views_plugins() {
  return array(
    'display' => array(
      'dashlet_view' => array(
        'title' => t('CiviCRM Dashlet'),
        'help' => t("Create a CiviCRM dashlet with this view."),
        'handler' => 'views_civicrm_dashlets_plugin_display_page',
        'theme' => 'views_view',
        'contextual links locations' => array('page'),
        'uses hook menu' => TRUE,
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'use more' => TRUE,
        'accept attachments' => TRUE,
        'admin' => t('Dashlet content'),
      ),
    ),
  );
}

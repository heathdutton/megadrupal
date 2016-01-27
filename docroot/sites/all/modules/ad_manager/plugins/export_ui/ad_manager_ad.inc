<?php

$plugin = array(
  'schema' => 'ad_manager_ad',
  'access' => 'administer ad manager',

  'menu' => array(
    'menu item' => 'ad-manager',
    'menu prefix' => 'admin/structure',
    'menu title' => 'Ad manager',
    'menu description' => 'Administer ads.',
  ),

  'title singular' => t('ad'),
  'title singular proper' => t('Ad'),
  'title plural' => t('ads'),
  'title plural proper' => t('Ads'),

  // Define the class to use as a handler for DART tags.
  'handler' => array(
    'class'   => 'ad_manager_ad_ui',
    'parent'  => 'ctools_export_ui',
  ),

  // Enable wizard since we use multi-step forms.
  'use wizard' => TRUE,

  // Define form_info for wizard.s
  'form info' => array(
    // TODO: enable Ajax:
    // 'ajax' => TRUE,
    // 'ajax render' => 'callback function',
    'show return' => FALSE,
    'order' => array(
      'ad_form' => t('Ad settings'),
      'provider_form' => t('Ad provider settings'),
    ),
    'forms' => array(
      'ad_form' => array(
        'form id' => 'ad_manager_ad_form'
      ),
      'provider_form' => array(
        'form id' => 'ad_manager_ad_provider_form'
      ),
    ),
  ),
);


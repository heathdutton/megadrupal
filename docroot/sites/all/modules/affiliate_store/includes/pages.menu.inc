<?php

/**
 * @file
 * Menu path for pages.
 */

/**
 * Implements menu passthrough hook.
 */
function _affiliate_store_pages_menu(&$items) {
  $items['affiliate-store'] = array(
    'title' => t('Store'),
    'title callback' => 'affiliate_store_title',
    'description' => 'Affiliate store front page.',
    'page callback' => 'affiliate_store_page',
    'access callback' => 'user_access',
    'access arguments' => array('access affiliate store product'),
    'file' => 'includes/pages.inc',
    'type' => MENU_SUGGESTED_ITEM,
  );
  $items['affiliate-store/term'] = array(
    'title' => 'List',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );
  $items['affiliate-store/term/%'] = array(
    'title' => t('Store'),
    'title callback' => 'affiliate_store_title',
    'description' => 'Affiliate store product category page.',
    'page callback' => 'affiliate_store_page',
    'page arguments' => array(2),
    'access callback' => 'user_access',
    'access arguments' => array('access affiliate store product'),
    'file' => 'includes/pages.inc',
    'type' => MENU_CALLBACK,
  );
}

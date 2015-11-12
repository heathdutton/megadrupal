<?php

/**
 * @file
 * Definitions of the default Commons browsing widget tabs.
 */

/**
 * Implementation of hook_default_commons_bw_ui().
 */
function commons_bw_ui_default_commons_bw_ui() {
  $tabs = array();

  if (module_exists('commons_groups')) {
    $tab = new stdClass;
    $tab->disabled = FALSE; /* Edit this to true to make a default tab disabled initially */
    $tab->api_version = 1;
    $tab->title = t('All');
    $tab->type = 'view';
    $tab->name = 'commons_bw_all';
    $tab->display = 'default';
    $tab->weight = -10;
    $tab->front = TRUE;
    $tab->bundle = 'post';
    $tabs['commons_all'] = $tab;
  }

  if (module_exists('commons_posts')) {
    $tab = new stdClass;
    $tab->disabled = FALSE; /* Edit this to true to make a default tab disabled initially */
    $tab->api_version = 1;
    $tab->title = t('Posts');
    $tab->type = 'view';
    $tab->name = 'commons_bw_posts';
    $tab->display = 'default';
    $tab->weight = -1;
    $tab->front = FALSE;
    $tab->bundle = 'post';
    $tabs['commons_posts'] = $tab;
  }

  if (module_exists('commons_q_a')) {
    $tab = new stdClass;
    $tab->disabled = FALSE; /* Edit this to true to make a default tab disabled initially */
    $tab->api_version = 1;
    $tab->title = t('Q & A');
    $tab->type = 'view';
    $tab->name = 'commons_bw_q_a';
    $tab->display = 'default';
    $tab->weight = 5;
    $tab->front = FALSE;
    $tab->bundle = 'question';
    $tabs['commons_q_a'] = $tab;
  }

  if (module_exists('commons_documents')) {
    $tab = new stdClass;
    $tab->disabled = FALSE; /* Edit this to true to make a default tab disabled initially */
    $tab->api_version = 1;
    $tab->title = t('Documents');
    $tab->type = 'view';
    $tab->name = 'commons_bw_documents';
    $tab->display = 'default';
    $tab->weight = 6;
    $tab->front = FALSE;
    $tab->bundle = 'document';
    $tabs['commons_documents'] = $tab;
  }

  if (module_exists('commons_wikis')) {
    $tab = new stdClass;
    $tab->disabled = FALSE; /* Edit this to true to make a default tab disabled initially */
    $tab->api_version = 1;
    $tab->title = t('Wikis');
    $tab->type = 'view';
    $tab->name = 'commons_bw_wikis';
    $tab->display = 'default';
    $tab->weight = 8;
    $tab->front = FALSE;
    $tab->bundle = 'wiki';
    $tabs['commons_wikis'] = $tab;
  }

  if (module_exists('commons_polls')) {
    $tab = new stdClass;
    $tab->disabled = FALSE; /* Edit this to true to make a default tab disabled initially */
    $tab->api_version = 1;
    $tab->title = t('Polls');
    $tab->type = 'view';
    $tab->name = 'commons_bw_polls';
    $tab->display = 'default';
    $tab->weight = 10;
    $tab->front = FALSE;
    $tab->bundle = 'poll';
    $tabs['commons_polls'] = $tab;
  }

  return $tabs;
}

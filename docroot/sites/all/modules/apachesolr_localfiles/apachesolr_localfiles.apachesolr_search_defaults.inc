<?php

/**
 * @file
 * apachesolr_search_defaults objects based on apachesolr_external_index module.
 */

/**
 * Implements hook_apachesolr_search_default_searchers().
 */
function apachesolr_localfiles_apachesolr_search_default_searchers() {
  $searchers = array();

  $searcher = new stdClass();
  $searcher->disabled = FALSE; /* Edit this to true to make a default searcher disabled initially */
  $searcher->api_version = 3;
  $searcher->page_id = 'localfiles_search';
  $searcher->label = t('Local files search');
  $searcher->description = '';
  $searcher->search_path = 'files/search';
  $searcher->page_title = t('Local files search');
  $searcher->env_id = APACHESOLR_LOCALFILES_ID; // defined in the .module
  $searcher->settings = array(
    'fq' => array(),
    'apachesolr_search_custom_enable' => 0,
    'apachesolr_search_search_type' => 'custom',
    'apachesolr_search_search_box' => 1,
    'apachesolr_search_per_page' => '15',
    'apachesolr_search_browse' => 'results',
    'apachesolr_search_spellcheck' => 0,
    'apachesolr_search_allow_user_input' => 0,
  );
  $searchers['localfiles_search'] = $searcher;

  return $searchers;
}


<?php

/**
 * @file
 * Provides default Views implementations for Fortissimmo accounts
 */

/**
 * Implementation of hook_views_default_views().
 */
function fortissimmo_views_default_views() {
  $accounts = fortissimmo_get_accounts();

  /*
   * View 'fortissimmo_<accountid>_property_detail'
   */
  if ($accounts) {
    foreach ($accounts as $accountid => $accountname) {
      $view = new view();
      $view->name = 'fortissimmo_' . $accountid . '_property_detail';
      $view->description = 'Default output for a Fortissimmo property detail page for account ' . check_plain($accountname);
      $view->tag = 'fortissimmo';
      $view->base_table = 'tbl' . $accountid . '_pand';
      $view->human_name = '';
      $view->core = 0;
      $view->api_version = '3.0';
      $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

      /* Display: Defaults */
      $handler = $view->new_display('default', 'Defaults', 'default');
      $handler->display->display_options['use_more_always'] = FALSE;
      $handler->display->display_options['access']['type'] = 'none';
      $handler->display->display_options['cache']['type'] = 'none';
      $handler->display->display_options['query']['type'] = 'views_query';
      $handler->display->display_options['exposed_form']['type'] = 'basic';
      $handler->display->display_options['pager']['type'] = 'some';
      $handler->display->display_options['pager']['options']['items_per_page'] = '1';
      $handler->display->display_options['pager']['options']['offset'] = '0';
      $handler->display->display_options['style_plugin'] = 'default';
      $handler->display->display_options['row_plugin'] = 'fields';
      /* Field: Fortissimmo Property: Title */
      $handler->display->display_options['fields']['title']['id'] = 'title';
      $handler->display->display_options['fields']['title']['table'] = 'tbl' . $accountid . '_pand';
      $handler->display->display_options['fields']['title']['field'] = 'title';
      $handler->display->display_options['fields']['title']['label'] = '';
      $handler->display->display_options['fields']['title']['fortissimmo_title_make_link'] = FALSE;
      $handler->display->display_options['fields']['title']['fortissimmo_title_reference_fallback'] = TRUE;
      /* Field: Fortissimmo Property: Description */
      $handler->display->display_options['fields']['description']['id'] = 'description';
      $handler->display->display_options['fields']['description']['table'] = 'tbl' . $accountid . '_pand';
      $handler->display->display_options['fields']['description']['field'] = 'description';
      $handler->display->display_options['fields']['description']['hide_empty'] = TRUE;
      $handler->display->display_options['fields']['description']['description_input_format'] = 'full_html';
      /* Field: Fortissimmo Property: Reference */
      $handler->display->display_options['fields']['reference']['id'] = 'reference';
      $handler->display->display_options['fields']['reference']['table'] = 'tbl' . $accountid . '_pand';
      $handler->display->display_options['fields']['reference']['field'] = 'reference';
      $handler->display->display_options['fields']['reference']['hide_empty'] = TRUE;
      /* Contextual filter: Fortissimmo Property: Fortissimmo ID */
      $handler->display->display_options['arguments']['fortissimmo']['id'] = 'fortissimmo';
      $handler->display->display_options['arguments']['fortissimmo']['table'] = 'tbl' . $accountid . '_pand';
      $handler->display->display_options['arguments']['fortissimmo']['field'] = 'fortissimmo';
      $handler->display->display_options['arguments']['fortissimmo']['default_action'] = 'not found';
      $handler->display->display_options['arguments']['fortissimmo']['exception']['title_enable'] = TRUE;
      $handler->display->display_options['arguments']['fortissimmo']['exception']['title'] = 'Alle';
      $handler->display->display_options['arguments']['fortissimmo']['title_enable'] = TRUE;
      $handler->display->display_options['arguments']['fortissimmo']['title'] = '%1';
      $handler->display->display_options['arguments']['fortissimmo']['breadcrumb_enable'] = TRUE;
      $handler->display->display_options['arguments']['fortissimmo']['breadcrumb'] = '%1';
      $handler->display->display_options['arguments']['fortissimmo']['default_argument_type'] = 'fixed';
      $handler->display->display_options['arguments']['fortissimmo']['summary']['number_of_records'] = '0';
      $handler->display->display_options['arguments']['fortissimmo']['summary']['format'] = 'default_summary';
      $handler->display->display_options['arguments']['fortissimmo']['summary_options']['items_per_page'] = '25';
      $handler->display->display_options['arguments']['fortissimmo']['specify_validation'] = TRUE;
      $translatables['fortissimmo_' . $accountid . '_property_detail'] = array(
        t('Defaults'),
        t('more'),
        t('Apply'),
        t('Reset'),
        t('Sort by'),
        t('Asc'),
        t('Desc'),
        t('Description'),
        t('Reference'),
        t('Alle'),
        t('%1'),
      );

      $views[$view->name] = $view;
    }
  }

  return $views;
}

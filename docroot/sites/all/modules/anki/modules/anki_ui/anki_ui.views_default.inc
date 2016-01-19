<?php
/**
 * @file
 * anki_ui.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function anki_ui_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'anki_cards';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'anki_cards';
  $view->human_name = 'Anki cards';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Flashcards';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'answer' => 'answer',
    'question' => 'question',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'answer' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'question' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No flashcards found!';
  /* Field: Anki: Card ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'anki_cards';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = 'Card id';
  /* Field: Anki: Question */
  $handler->display->display_options['fields']['question']['id'] = 'question';
  $handler->display->display_options['fields']['question']['table'] = 'anki_cards';
  $handler->display->display_options['fields']['question']['field'] = 'question';
  /* Contextual filter: Anki: Collection */
  $handler->display->display_options['arguments']['collection']['id'] = 'collection';
  $handler->display->display_options['arguments']['collection']['table'] = 'anki_cards';
  $handler->display->display_options['arguments']['collection']['field'] = 'collection';
  $handler->display->display_options['arguments']['collection']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['collection']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['collection']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['collection']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'anki/%';
  $handler->display->display_options['menu']['title'] = 'Browse';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['anki_cards'] = $view;

  return $export;
}

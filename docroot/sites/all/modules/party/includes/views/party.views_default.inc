<?php
/**
 * @file party.views_default.inc
 * Provides default views for the Party module.
 */

/**
 * Implements hook_views_default_views().
 */
function party_views_default_views() {
  // The main view of party pieces based on data sets.
  $view = new view;
  $view->name = 'party_attached_entities';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'party';
  $view->human_name = 'Party attached entities';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'party_attached_entity';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = t('There are no attached items to display.');
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Party: Party ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'party';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  /* Contextual filter: Party: Party ID */
  $handler->display->display_options['arguments']['pid']['id'] = 'pid';
  $handler->display->display_options['arguments']['pid']['table'] = 'party';
  $handler->display->display_options['arguments']['pid']['field'] = 'pid';
  $handler->display->display_options['arguments']['pid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['pid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['pid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['pid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['pid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['pid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['pid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['pid']['not'] = 0;

  // Get all defined data sets.
  $sets = party_get_data_set_info();
  //dsm($sets);
  // Make a display for each data set.
  foreach ($sets as $set_id => $set) {
    // Skip those data sets that request neither a piece nor it to be a view.
    // @todo: the second part of this if fails, but I've not yet had breakfast and I can't see why. fixme!
    if (!isset($set['piece']) || $set['piece']['maker'] != 'view') {
      continue;
    }

    $handler = $view->new_display('party_piece', $set['label'], 'party_piece_' . $set_id);
    $handler->display->display_options['data_set'] = $set_id;
    $handler->display->display_options['display_description'] = 'Party component - ' . $set['label'];
    $handler->display->display_options['party_piece_label'] = $set['label'];
    $handler->display->display_options['party_piece_path'] = $set['piece']['path'];
    $handler->display->display_options['defaults']['filters'] = FALSE;

    // Filter criterion: Party: Attached entity data set.
    $handler->display->display_options['filters']['entity_type']['id'] = 'data_set';
    $handler->display->display_options['filters']['entity_type']['table'] = 'party_attached_entity';
    $handler->display->display_options['filters']['entity_type']['field'] = 'data_set';
    $handler->display->display_options['filters']['entity_type']['value'] = array($set_id);
    $handler->display->display_options['filters']['entity_type']['group'] = 0;
  }

  $views[$view->name] = $view;

  return $views;
}

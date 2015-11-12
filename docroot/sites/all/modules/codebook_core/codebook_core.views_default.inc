<?php
/**
 * @file
 * codebook_core.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function codebook_core_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'codebook';
  $view->description = 'Top-level Codebook page.';
  $view->tag = 'codebook';
  $view->base_table = 'node';
  $view->human_name = 'Codebook';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Codebook';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'codebook_title' => 'codebook_title',
    'draggableviews' => 'draggableviews',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'codebook_title' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'draggableviews' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Footer: Global: Codebook node add links */
  $handler->display->display_options['footer']['codebook_core_node_add']['id'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['table'] = 'views';
  $handler->display->display_options['footer']['codebook_core_node_add']['field'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['codebook_types'] = array(
    'codebook_title' => 'codebook_title',
    'codebook_chapter' => 0,
    'codebook_ordinance' => 0,
    'codebook_resolution' => 0,
    'codebook_section' => 0,
  );
  $handler->display->display_options['footer']['codebook_core_node_add']['prepopulate_reference'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Sort criterion: Content: Title Number (field_codebook_doc_num) */
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['id'] = 'field_codebook_doc_num_value';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['table'] = 'field_data_field_codebook_doc_num';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['field'] = 'field_codebook_doc_num_value';
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'codebook_title' => 'codebook_title',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'codebook';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Codebook';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['codebook'] = $view;

  $view = new view();
  $view->name = 'codebook_footnotes';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'field_collection_item';
  $view->human_name = 'Footnotes';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['type'] = 'ol';
  $handler->display->display_options['style_options']['wrapper_class'] = '';
  $handler->display->display_options['row_plugin'] = 'entity';
  /* Relationship: Field collection item: Entity with the Title Footnotes (field_codebook_title_notes) */
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['id'] = 'field_codebook_title_notes_node';
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['field'] = 'field_codebook_title_notes_node';
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['label'] = 'Chapter';
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['required'] = TRUE;
  /* Field: Field collection item: Field collection item ID */
  $handler->display->display_options['fields']['item_id']['id'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['table'] = 'field_collection_item';
  $handler->display->display_options['fields']['item_id']['field'] = 'item_id';
  /* Sort criterion: Content: Title Footnotes (field_codebook_title_notes:delta) */
  $handler->display->display_options['sorts']['delta']['id'] = 'delta';
  $handler->display->display_options['sorts']['delta']['table'] = 'field_data_field_codebook_title_notes';
  $handler->display->display_options['sorts']['delta']['field'] = 'delta';
  $handler->display->display_options['sorts']['delta']['relationship'] = 'field_codebook_title_notes_node';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_codebook_title_notes_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';

  /* Display: Chapter Footnotes */
  $handler = $view->new_display('entity_view', 'Chapter Footnotes', 'eva_chapter_footnotes');
  $handler->display->display_options['display_description'] = 'Renders footnotes which are assigned to Chapters.';
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_chapter',
  );

  /* Display: Title Footnotes */
  $handler = $view->new_display('entity_view', 'Title Footnotes', 'eva_title_footnotes');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Title Footnotes';
  $handler->display->display_options['display_description'] = 'Renders footnotes which are assigned to Chapters included in a Title.';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Field collection item: Entity with the Title Footnotes (field_codebook_title_notes) */
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['id'] = 'field_codebook_title_notes_node';
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['field'] = 'field_codebook_title_notes_node';
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['label'] = 'Chapter';
  $handler->display->display_options['relationships']['field_codebook_title_notes_node']['required'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['id'] = 'field_codebook_title_target_id';
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['table'] = 'field_data_field_codebook_title';
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['field'] = 'field_codebook_title_target_id';
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['relationship'] = 'field_codebook_title_notes_node';
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['label'] = 'Title';
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Title Number (field_codebook_doc_num) */
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['id'] = 'field_codebook_doc_num_value';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['table'] = 'field_data_field_codebook_doc_num';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['field'] = 'field_codebook_doc_num_value';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['relationship'] = 'field_codebook_title_notes_node';
  /* Sort criterion: Content: Title Footnotes (field_codebook_title_notes:delta) */
  $handler->display->display_options['sorts']['delta']['id'] = 'delta';
  $handler->display->display_options['sorts']['delta']['table'] = 'field_data_field_codebook_title_notes';
  $handler->display->display_options['sorts']['delta']['field'] = 'delta';
  $handler->display->display_options['sorts']['delta']['relationship'] = 'field_codebook_title_notes_node';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_codebook_title_target_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1 Footnotes';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_title',
  );
  $handler->display->display_options['show_title'] = 1;
  $export['codebook_footnotes'] = $view;

  $view = new view();
  $view->name = 'codebook_sub_docs';
  $view->description = 'Displays the child douments of Codebook content.';
  $view->tag = 'codebook';
  $view->base_table = 'node';
  $view->human_name = 'Sub Docs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Chapters';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Footer: Global: Codebook node add links */
  $handler->display->display_options['footer']['codebook_core_node_add']['id'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['table'] = 'views';
  $handler->display->display_options['footer']['codebook_core_node_add']['field'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['label'] = 'codebook_core_node_add';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Chapters in this Title.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['id'] = 'field_codebook_title_target_id';
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['table'] = 'field_data_field_codebook_title';
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['field'] = 'field_codebook_title_target_id';
  $handler->display->display_options['relationships']['field_codebook_title_target_id']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Title Number (field_codebook_doc_num) */
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['id'] = 'field_codebook_doc_num_value';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['table'] = 'field_data_field_codebook_doc_num';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['field'] = 'field_codebook_doc_num_value';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_codebook_title_target_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';

  /* Display: Chapters on Titles */
  $handler = $view->new_display('entity_view', 'Chapters on Titles', 'eva_chapters');
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Codebook node add links */
  $handler->display->display_options['footer']['codebook_core_node_add']['id'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['table'] = 'views';
  $handler->display->display_options['footer']['codebook_core_node_add']['field'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['label'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['empty'] = TRUE;
  $handler->display->display_options['footer']['codebook_core_node_add']['codebook_types'] = array(
    'codebook_chapter' => 'codebook_chapter',
    'codebook_ordinance' => 0,
    'codebook_resolution' => 0,
    'codebook_section' => 0,
    'codebook_title' => 0,
  );
  $handler->display->display_options['footer']['codebook_core_node_add']['prepopulate_reference'] = 1;
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_title',
  );
  $handler->display->display_options['show_title'] = 1;

  /* Display: Sections on Chapters - Summary */
  $handler = $view->new_display('entity_view', 'Sections on Chapters - Summary', 'eva_sections_summary');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Sections';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Codebook node add links */
  $handler->display->display_options['footer']['codebook_core_node_add']['id'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['table'] = 'views';
  $handler->display->display_options['footer']['codebook_core_node_add']['field'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['label'] = 'codebook_core_node_add';
  $handler->display->display_options['footer']['codebook_core_node_add']['empty'] = TRUE;
  $handler->display->display_options['footer']['codebook_core_node_add']['codebook_types'] = array(
    'codebook_section' => 'codebook_section',
    'codebook_chapter' => 0,
    'codebook_ordinance' => 0,
    'codebook_resolution' => 0,
    'codebook_title' => 0,
  );
  $handler->display->display_options['footer']['codebook_core_node_add']['prepopulate_reference'] = 1;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Sections in this Chapter.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_codebook_chapter_target_id']['id'] = 'field_codebook_chapter_target_id';
  $handler->display->display_options['relationships']['field_codebook_chapter_target_id']['table'] = 'field_data_field_codebook_chapter';
  $handler->display->display_options['relationships']['field_codebook_chapter_target_id']['field'] = 'field_codebook_chapter_target_id';
  $handler->display->display_options['relationships']['field_codebook_chapter_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '#node-[nid]';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_codebook_chapter_target_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_chapter',
  );
  $handler->display->display_options['show_title'] = 1;

  /* Display: Sections on Chapters - Full */
  $handler = $view->new_display('entity_view', 'Sections on Chapters - Full', 'eva_sections_full');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'codebook__chapter';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'codebook__section';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'codebook_embed';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Sections in this Chapter.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_codebook_chapter_target_id']['id'] = 'field_codebook_chapter_target_id';
  $handler->display->display_options['relationships']['field_codebook_chapter_target_id']['table'] = 'field_data_field_codebook_chapter';
  $handler->display->display_options['relationships']['field_codebook_chapter_target_id']['field'] = 'field_codebook_chapter_target_id';
  $handler->display->display_options['relationships']['field_codebook_chapter_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_type'] = 'h2';
  $handler->display->display_options['fields']['title']['element_class'] = 'codebook__section-title';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_default_classes'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_type'] = 'p';
  $handler->display->display_options['fields']['edit_node']['element_class'] = 'codebook__edit';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['text'] = 'edit section';
  /* Field: Content: Description */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_type'] = 'div';
  $handler->display->display_options['fields']['body']['element_class'] = 'codebook__section-body';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_default_classes'] = FALSE;
  /* Field: Content: Exhibits */
  $handler->display->display_options['fields']['field_codebook_exhibits']['id'] = 'field_codebook_exhibits';
  $handler->display->display_options['fields']['field_codebook_exhibits']['table'] = 'field_data_field_codebook_exhibits';
  $handler->display->display_options['fields']['field_codebook_exhibits']['field'] = 'field_codebook_exhibits';
  $handler->display->display_options['fields']['field_codebook_exhibits']['element_type'] = 'div';
  $handler->display->display_options['fields']['field_codebook_exhibits']['element_class'] = 'codebook__section-exhibits';
  $handler->display->display_options['fields']['field_codebook_exhibits']['element_label_type'] = 'h3';
  $handler->display->display_options['fields']['field_codebook_exhibits']['element_label_class'] = 'codebook__section-subheader';
  $handler->display->display_options['fields']['field_codebook_exhibits']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_codebook_exhibits']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_codebook_exhibits']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_codebook_exhibits']['type'] = 'file_table';
  $handler->display->display_options['fields']['field_codebook_exhibits']['delta_offset'] = '0';
  /* Field: Content: Ordinances */
  $handler->display->display_options['fields']['field_codebook_ordinances']['id'] = 'field_codebook_ordinances';
  $handler->display->display_options['fields']['field_codebook_ordinances']['table'] = 'field_data_field_codebook_ordinances';
  $handler->display->display_options['fields']['field_codebook_ordinances']['field'] = 'field_codebook_ordinances';
  $handler->display->display_options['fields']['field_codebook_ordinances']['element_type'] = 'div';
  $handler->display->display_options['fields']['field_codebook_ordinances']['element_class'] = 'codebook__section-ordinances';
  $handler->display->display_options['fields']['field_codebook_ordinances']['element_label_type'] = 'h3';
  $handler->display->display_options['fields']['field_codebook_ordinances']['element_label_class'] = 'codebook__section-subheader';
  $handler->display->display_options['fields']['field_codebook_ordinances']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_codebook_ordinances']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['field_codebook_ordinances']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_codebook_chapter_target_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_chapter',
  );
  $handler->display->display_options['show_title'] = 1;
  $export['codebook_sub_docs'] = $view;

  return $export;
}

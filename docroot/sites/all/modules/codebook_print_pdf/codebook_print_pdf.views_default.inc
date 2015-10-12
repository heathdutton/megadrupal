<?php
/**
 * @file
 * codebook_print_pdf.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function codebook_print_pdf_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'codebook_pdf';
  $view->description = 'Used to generate listing of content in PDFs.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Codebook PDF';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'codebook_embed_pdf';
  $handler->display->display_options['row_options']['links'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Sort criterion: Content: Title Number (field_codebook_doc_num) */
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['id'] = 'field_codebook_doc_num_value';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['table'] = 'field_data_field_codebook_doc_num';
  $handler->display->display_options['sorts']['field_codebook_doc_num_value']['field'] = 'field_codebook_doc_num_value';
  /* Contextual filter: Content: Chapter (field_codebook_chapter) */
  $handler->display->display_options['arguments']['field_codebook_chapter_target_id']['id'] = 'field_codebook_chapter_target_id';
  $handler->display->display_options['arguments']['field_codebook_chapter_target_id']['table'] = 'field_data_field_codebook_chapter';
  $handler->display->display_options['arguments']['field_codebook_chapter_target_id']['field'] = 'field_codebook_chapter_target_id';
  $handler->display->display_options['arguments']['field_codebook_chapter_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_codebook_chapter_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_codebook_chapter_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_codebook_chapter_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_codebook_chapter_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'codebook_chapter' => 'codebook_chapter',
  );

  /* Display: Chapters on Titles - Summary */
  $handler = $view->new_display('entity_view', 'Chapters on Titles - Summary', 'eva_title_summary');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Chapters';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Title (field_codebook_title) */
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['id'] = 'field_codebook_title_target_id';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['table'] = 'field_data_field_codebook_title';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['field'] = 'field_codebook_title_target_id';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_title',
  );
  $handler->display->display_options['show_title'] = 1;

  /* Display: Chapters on Titles - Full */
  $handler = $view->new_display('entity_view', 'Chapters on Titles - Full', 'eva_title_full');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Title (field_codebook_title) */
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['id'] = 'field_codebook_title_target_id';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['table'] = 'field_data_field_codebook_title';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['field'] = 'field_codebook_title_target_id';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_codebook_title_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'codebook_chapter' => 'codebook_chapter',
  );
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_title',
  );

  /* Display: Sections on Chapters - Summary */
  $handler = $view->new_display('entity_view', 'Sections on Chapters - Summary', 'eva_chapter_summary');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Sections';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'codebook_section' => 'codebook_section',
  );
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_chapter',
  );
  $handler->display->display_options['show_title'] = 1;

  /* Display: Sections on Chapters - Full */
  $handler = $view->new_display('entity_view', 'Sections on Chapters - Full', 'eva_chapter_full');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'codebook_section' => 'codebook_section',
  );
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'codebook_chapter',
  );
  $export['codebook_pdf'] = $view;

  return $export;
}

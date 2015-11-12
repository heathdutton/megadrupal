<?php

function views_oai_pmh_views_default_views() {

  $view = new view();
  $view->name = 'oai_biblio';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Biblio OAI-PMH';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'dc:title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Biblio: Authors */
  $handler->display->display_options['fields']['biblio_authors']['id'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_authors']['field'] = 'biblio_authors';
  $handler->display->display_options['fields']['biblio_authors']['label'] = 'dc:creator';
  $handler->display->display_options['fields']['biblio_authors']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_authors']['biblio_label'] = 0;
  $handler->display->display_options['fields']['biblio_authors']['auth_category'] = '0';
  /* Field: Biblio: Corporate Authors */
  $handler->display->display_options['fields']['biblio_corp_authors']['id'] = 'biblio_corp_authors';
  $handler->display->display_options['fields']['biblio_corp_authors']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_corp_authors']['field'] = 'biblio_corp_authors';
  $handler->display->display_options['fields']['biblio_corp_authors']['label'] = 'dc:creator';
  $handler->display->display_options['fields']['biblio_corp_authors']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_corp_authors']['biblio_label'] = 1;
  $handler->display->display_options['fields']['biblio_corp_authors']['auth_category'] = '5';
  /* Field: Biblio: DOI */
  $handler->display->display_options['fields']['biblio_doi']['id'] = 'biblio_doi';
  $handler->display->display_options['fields']['biblio_doi']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_doi']['field'] = 'biblio_doi';
  $handler->display->display_options['fields']['biblio_doi']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['biblio_doi']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_doi']['biblio_label'] = 1;
  /* Field: Biblio: ISBN Number */
  $handler->display->display_options['fields']['biblio_isbn']['id'] = 'biblio_isbn';
  $handler->display->display_options['fields']['biblio_isbn']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_isbn']['field'] = 'biblio_isbn';
  $handler->display->display_options['fields']['biblio_isbn']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['biblio_isbn']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_isbn']['biblio_label'] = 1;
  /* Field: Biblio: Publisher */
  $handler->display->display_options['fields']['biblio_publisher']['id'] = 'biblio_publisher';
  $handler->display->display_options['fields']['biblio_publisher']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_publisher']['field'] = 'biblio_publisher';
  $handler->display->display_options['fields']['biblio_publisher']['label'] = 'dc:publisher';
  $handler->display->display_options['fields']['biblio_publisher']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_publisher']['biblio_label'] = 1;
  /* Field: Biblio: Secondary Title */
  $handler->display->display_options['fields']['biblio_secondary_title']['id'] = 'biblio_secondary_title';
  $handler->display->display_options['fields']['biblio_secondary_title']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_secondary_title']['field'] = 'biblio_secondary_title';
  $handler->display->display_options['fields']['biblio_secondary_title']['label'] = 'dc:source';
  $handler->display->display_options['fields']['biblio_secondary_title']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_secondary_title']['biblio_label'] = 0;
  /* Field: Biblio: URL */
  $handler->display->display_options['fields']['biblio_url']['id'] = 'biblio_url';
  $handler->display->display_options['fields']['biblio_url']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_url']['field'] = 'biblio_url';
  $handler->display->display_options['fields']['biblio_url']['label'] = 'dc:identifier';
  $handler->display->display_options['fields']['biblio_url']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_url']['biblio_label'] = 1;
  /* Field: Biblio: Year of Publication */
  $handler->display->display_options['fields']['biblio_year']['id'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_year']['field'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['label'] = 'dc:date';
  $handler->display->display_options['fields']['biblio_year']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_year']['biblio_label'] = 0;
  /* Field: Biblio: Abstract */
  $handler->display->display_options['fields']['biblio_abst_e']['id'] = 'biblio_abst_e';
  $handler->display->display_options['fields']['biblio_abst_e']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_abst_e']['field'] = 'biblio_abst_e';
  $handler->display->display_options['fields']['biblio_abst_e']['label'] = 'dc:description';
  $handler->display->display_options['fields']['biblio_abst_e']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_abst_e']['hide_empty'] = TRUE;
  /* Field: Biblio: Publication Language */
  $handler->display->display_options['fields']['biblio_lang']['id'] = 'biblio_lang';
  $handler->display->display_options['fields']['biblio_lang']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_lang']['field'] = 'biblio_lang';
  $handler->display->display_options['fields']['biblio_lang']['label'] = 'dc:language';
  $handler->display->display_options['fields']['biblio_lang']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_lang']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_lang']['biblio_label'] = 0;
  /* Field: Biblio: Keywords */
  $handler->display->display_options['fields']['biblio_keywords']['id'] = 'biblio_keywords';
  $handler->display->display_options['fields']['biblio_keywords']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_keywords']['field'] = 'biblio_keywords';
  $handler->display->display_options['fields']['biblio_keywords']['label'] = 'dc:subject';
  $handler->display->display_options['fields']['biblio_keywords']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['biblio_keywords']['biblio_label'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
      'biblio' => 'biblio',
  );

  /* Display: OAI-PMH */
  $handler = $view->new_display('views_oai_pmh', 'OAI-PMH', 'views_oai_pmh_biblio');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_oai_pmh_dc';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'dc';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'oai';

  return array('oai_biblio' => $view);

}
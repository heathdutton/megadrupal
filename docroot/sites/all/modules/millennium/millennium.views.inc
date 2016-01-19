<?php
/**
 * @file
 * Views handlers.
 */

/**
 * Implements hook_views_data().
 */
function millennium_views_data() {
  $data = array();
  $data['millennium_node_bib']['table']['group'] = t('Millennium');
  $data['millennium_node_bib']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
    'type' => 'INNER',
  );
  $data['millennium_node_bib']['cover_image'] = array(
    'field' => array(
      'real field' => 'biblio_data',
      'title' => t('Cover image'),
      'help' => t("Show the record cover image."),
      'handler' => 'millennium_handler_field_coverimage',
    ),
  );
  $data['millennium_node_bib']['type'] = array(
    'field' => array(
      'real field' => 'biblio_data',
      'title' => t('Type'),
      'help' => t("Show the record type."),
      'handler' => 'millennium_handler_field_type',
    ),
  );
  $data['millennium_node_bib']['year'] = array(
    'field' => array(
      'real field' => 'biblio_data',
      'title' => t('Year published'),
      'help' => t("Show the record's year published."),
      'handler' => 'millennium_handler_field_year',
    ),
  );
  $data['millennium_node_bib']['biblio_data'] = array(
    'field' => array(
      'real field' => 'biblio_data',
      'title' => t('Biblio data table'),
      'help' => t("Show the record's complete biblio data table."),
      'handler' => 'millennium_handler_field_biblio_data_table',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_handlers() to register all of the basic handlers().
 * views uses.
 */
function millennium_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'millennium') . '/views',
    ),
    'handlers' => array(
      // The name of my handler
      'millennium_handler_field_biblio_data_table' => array(
        'parent' => 'views_handler_field',
      ),
      'millennium_handler_field_coverimage' => array(
        'parent' => 'views_handler_field',
      ),
      'millennium_handler_field_type' => array(
        'parent' => 'views_handler_field',
      ),
      'millennium_handler_field_year' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implements hook_views_default_views().
 */
function millennium_views_default_views() {
  $views = array();

  // Exported view: millennium_book_wall
  $view = new view;
  $view->name = 'millennium_book_wall';
  $view->description = 'Millennium module: Wall of books';
  $view->tag = 'millennium';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to TRUE to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'New items';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['columns'] = '6';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Millennium: Cover image */
  $handler->display->display_options['fields']['cover_image']['id'] = 'cover_image';
  $handler->display->display_options['fields']['cover_image']['table'] = 'millennium_node_bib';
  $handler->display->display_options['fields']['cover_image']['field'] = 'cover_image';
  $handler->display->display_options['fields']['cover_image']['label'] = '';
  $handler->display->display_options['fields']['cover_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['cover_image']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['cover_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cover_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cover_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cover_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cover_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cover_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cover_image']['empty_zero'] = 0;
  /* Field: Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 1;
  $handler->display->display_options['fields']['title']['alter']['max_length'] = '40';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Millennium: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'millennium_node_bib';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  /* Field: Millennium: Year published */
  $handler->display->display_options['fields']['year']['id'] = 'year';
  $handler->display->display_options['fields']['year']['table'] = 'millennium_node_bib';
  $handler->display->display_options['fields']['year']['field'] = 'year';
  $handler->display->display_options['fields']['year']['label'] = '';
  $handler->display->display_options['fields']['year']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['year']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['year']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['year']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['year']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['year']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['year']['alter']['html'] = 0;
  $handler->display->display_options['fields']['year']['hide_empty'] = 0;
  $handler->display->display_options['fields']['year']['empty_zero'] = 0;
  /* Sort criterion: Node: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Filter: Node: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'millennium/bookwall';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Book wall';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'primary-links';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '6';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';

  $views[$view->name] = $view;

  return $views;
}

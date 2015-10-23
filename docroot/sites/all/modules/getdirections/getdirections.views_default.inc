<?php

/**
 * @file
 * Provides Views 3 integration for getdirections.
 *
 */


/**
 * Implements hook_views_default_views().
 */
function getdirections_views_default_views() {
  $views = array();

  if (module_exists('getlocations_fields')) {

    /**
    * getdirections node
    */
    $view = new view;
    $view->name = 'getdirections';
    $view->description = 'A link and block view to get directions for a getlocations-enabled node.';
    $view->tag = 'getdirections';
    $view->base_table = 'node';
    $view->human_name = 'getdirections';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['label'] = '';
    $handler->display->display_options['fields']['title']['exclude'] = TRUE;
    $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['title']['alter']['external'] = 0;
    $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['title']['alter']['html'] = 0;
    $handler->display->display_options['fields']['title']['link_to_node'] = 0;
    /* Field: Content: Nid */
    $handler->display->display_options['fields']['nid']['id'] = 'nid';
    $handler->display->display_options['fields']['nid']['table'] = 'node';
    $handler->display->display_options['fields']['nid']['field'] = 'nid';
    $handler->display->display_options['fields']['nid']['label'] = '';
    $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['text'] = 'Get directions to [title]';
    $handler->display->display_options['fields']['nid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['path'] = 'getdirections/location/to/[nid]';
    $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
    $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
    $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
    /* Contextual filter: Content: Nid */
    $handler->display->display_options['arguments']['nid']['id'] = 'nid';
    $handler->display->display_options['arguments']['nid']['table'] = 'node';
    $handler->display->display_options['arguments']['nid']['field'] = 'nid';
    $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
    $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
    $handler->display->display_options['arguments']['nid']['not'] = 0;
    /* Filter criterion: Content: Published */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'node';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 1;
    $handler->display->display_options['filters']['status']['group'] = 0;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
    /* Filter criterion: Getlocations Fields: Glid */
    $handler->display->display_options['filters']['glid']['id'] = 'glid';
    $handler->display->display_options['filters']['glid']['table'] = 'getlocations_fields';
    $handler->display->display_options['filters']['glid']['field'] = 'glid';
    $handler->display->display_options['filters']['glid']['operator'] = 'not empty';
    $handler->display->display_options['filters']['glid']['value']['value'] = '0';

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_1');

    $views[$view->name] = $view;


    /**
    * getdirections for multiple locations per node
    */








    /**
    * getdirections for taxonomy terms
    */

    $view = new view;
    $view->name = 'getdirections_term';
    $view->description = 'A link and block view to get directions for a getlocations-enabled taxonomy term.';
    $view->tag = 'getdirections';
    $view->base_table = 'taxonomy_term_data';
    $view->human_name = 'getdirections Term';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: Taxonomy term: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['label'] = '';
    $handler->display->display_options['fields']['name']['exclude'] = TRUE;
    $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['name']['alter']['external'] = 0;
    $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
    $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
    $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['name']['alter']['html'] = 0;
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['name']['hide_empty'] = 0;
    $handler->display->display_options['fields']['name']['empty_zero'] = 0;
    $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['name']['link_to_taxonomy'] = 0;
    /* Field: Taxonomy term: Term ID */
    $handler->display->display_options['fields']['tid']['id'] = 'tid';
    $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
    $handler->display->display_options['fields']['tid']['field'] = 'tid';
    $handler->display->display_options['fields']['tid']['label'] = '';
    $handler->display->display_options['fields']['tid']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['tid']['alter']['text'] = 'Get directions to [name]';
    $handler->display->display_options['fields']['tid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['tid']['alter']['path'] = 'getdirections/location_term/to/[tid]';
    $handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['tid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 1;
    $handler->display->display_options['fields']['tid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['tid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['tid']['hide_empty'] = 0;
    $handler->display->display_options['fields']['tid']['empty_zero'] = 0;
    $handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['tid']['format_plural'] = 0;
    /* Contextual filter: Taxonomy term: Term ID */
    $handler->display->display_options['arguments']['tid']['id'] = 'tid';
    $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_term_data';
    $handler->display->display_options['arguments']['tid']['field'] = 'tid';
    $handler->display->display_options['arguments']['tid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'taxonomy_tid';
    $handler->display->display_options['arguments']['tid']['default_argument_options']['term_page'] = 1;
    $handler->display->display_options['arguments']['tid']['default_argument_options']['node'] = 0;
    $handler->display->display_options['arguments']['tid']['default_argument_options']['limit'] = 0;
    $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['tid']['break_phrase'] = 0;
    $handler->display->display_options['arguments']['tid']['not'] = 0;
    /* Filter criterion: Taxonomy vocabulary: Machine name */
    $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
    $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
    $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
    $handler->display->display_options['filters']['machine_name']['value'] = array();

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_2');

    $views[$view->name] = $view;



    /**
    * getdirections user
    */
    $view = new view;
    $view->name = 'getdirections_user';
    $view->description = 'A link and block view to get directions for a location-enabled user';
    $view->tag = 'getdirections';
    $view->base_table = 'users';
    $view->human_name = 'getdirections User';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'access user profiles';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['label'] = '';
    $handler->display->display_options['fields']['name']['exclude'] = TRUE;
    $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['name']['alter']['external'] = 0;
    $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
    $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
    $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['name']['alter']['html'] = 0;
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['name']['hide_empty'] = 0;
    $handler->display->display_options['fields']['name']['empty_zero'] = 0;
    $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['name']['link_to_user'] = 0;
    $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
    $handler->display->display_options['fields']['name']['format_username'] = 1;
    /* Field: User: Uid */
    $handler->display->display_options['fields']['uid']['id'] = 'uid';
    $handler->display->display_options['fields']['uid']['table'] = 'users';
    $handler->display->display_options['fields']['uid']['field'] = 'uid';
    $handler->display->display_options['fields']['uid']['label'] = '';
    $handler->display->display_options['fields']['uid']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['text'] = 'Get directions to [name]';
    $handler->display->display_options['fields']['uid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['path'] = 'getdirections/location_user/to/[uid]';
    $handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['uid']['alter']['external'] = 0;
    $handler->display->display_options['fields']['uid']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['uid']['alter']['trim_whitespace'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['uid']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['uid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['uid']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['uid']['hide_empty'] = 0;
    $handler->display->display_options['fields']['uid']['empty_zero'] = 0;
    $handler->display->display_options['fields']['uid']['hide_alter_empty'] = 1;
    $handler->display->display_options['fields']['uid']['link_to_user'] = 0;
    /* Contextual filter: User: Uid */
    $handler->display->display_options['arguments']['uid']['id'] = 'uid';
    $handler->display->display_options['arguments']['uid']['table'] = 'users';
    $handler->display->display_options['arguments']['uid']['field'] = 'uid';
    $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
    $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
    $handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
    $handler->display->display_options['arguments']['uid']['not'] = 0;
    /* Filter criterion: User: Active */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'users';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = '1';
    $handler->display->display_options['filters']['status']['group'] = 0;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_1');

    $views[$view->name] = $view;

  }
  elseif (module_exists('location')) {

    /**
    * getdirections node
    */
    $view = new view;
    $view->name = 'getdirections';
    $view->description = 'A link and block view to get directions for a location-enabled node.';
    $view->tag = 'getdirections';
    $view->base_table = 'node';
    $view->human_name = 'getdirections';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['label'] = '';
    $handler->display->display_options['fields']['title']['exclude'] = TRUE;
    $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['title']['alter']['html'] = 0;
    $handler->display->display_options['fields']['title']['link_to_node'] = 0;
    /* Field: Content: Nid */
    $handler->display->display_options['fields']['nid']['id'] = 'nid';
    $handler->display->display_options['fields']['nid']['table'] = 'node';
    $handler->display->display_options['fields']['nid']['field'] = 'nid';
    $handler->display->display_options['fields']['nid']['label'] = '';
    $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['text'] = 'Get directions to [title]';
    $handler->display->display_options['fields']['nid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['path'] = 'getdirections/location/to/[nid]';
    $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
    /* Contextual filter: Content: Nid */
    $handler->display->display_options['arguments']['nid']['id'] = 'nid';
    $handler->display->display_options['arguments']['nid']['table'] = 'node';
    $handler->display->display_options['arguments']['nid']['field'] = 'nid';
    $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['nid']['exception']['title_enable'] = 1;
    $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
    $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['nid']['specify_validation'] = 1;
    $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
    $handler->display->display_options['arguments']['nid']['not'] = 0;
    /* Filter criterion: Content: Published */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'node';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 1;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
    /* Filter criterion: Location: Lid */
    $handler->display->display_options['filters']['lid']['id'] = 'lid';
    $handler->display->display_options['filters']['lid']['table'] = 'location';
    $handler->display->display_options['filters']['lid']['field'] = 'lid';
    $handler->display->display_options['filters']['lid']['operator'] = 'not empty';
    $handler->display->display_options['filters']['lid']['expose']['operator'] = FALSE;

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_1');
    $views[$view->name] = $view;


    /**
    * getdirections for multiple locations per node
    */
    $view = new view;
    $view->name = 'getdirections_multi';
    $view->description = 'A link and block view to get directions for use with multiple locations per node.';
    $view->tag = 'getdirections';
    $view->base_table = 'node';
    $view->human_name = 'getdirections_multi';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    $handler->display->display_options['row_options']['hide_empty'] = 0;
    $handler->display->display_options['row_options']['default_field_elements'] = 1;
    /* Field: Location: Latitude */
    $handler->display->display_options['fields']['latitude']['id'] = 'latitude';
    $handler->display->display_options['fields']['latitude']['table'] = 'location';
    $handler->display->display_options['fields']['latitude']['field'] = 'latitude';
    $handler->display->display_options['fields']['latitude']['label'] = '';
    $handler->display->display_options['fields']['latitude']['exclude'] = TRUE;
    $handler->display->display_options['fields']['latitude']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['external'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['latitude']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['latitude']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['html'] = 0;
    $handler->display->display_options['fields']['latitude']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['latitude']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['latitude']['hide_empty'] = 0;
    $handler->display->display_options['fields']['latitude']['empty_zero'] = 0;
    $handler->display->display_options['fields']['latitude']['hide_alter_empty'] = 0;
    $handler->display->display_options['fields']['latitude']['style'] = 'dd';
    /* Field: Location: Longitude */
    $handler->display->display_options['fields']['longitude']['id'] = 'longitude';
    $handler->display->display_options['fields']['longitude']['table'] = 'location';
    $handler->display->display_options['fields']['longitude']['field'] = 'longitude';
    $handler->display->display_options['fields']['longitude']['label'] = '';
    $handler->display->display_options['fields']['longitude']['exclude'] = TRUE;
    $handler->display->display_options['fields']['longitude']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['external'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['longitude']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['longitude']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['html'] = 0;
    $handler->display->display_options['fields']['longitude']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['longitude']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['longitude']['hide_empty'] = 0;
    $handler->display->display_options['fields']['longitude']['empty_zero'] = 0;
    $handler->display->display_options['fields']['longitude']['hide_alter_empty'] = 0;
    $handler->display->display_options['fields']['longitude']['style'] = 'dd';
    /* Field: Location: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'location';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['label'] = '';
    $handler->display->display_options['fields']['name']['exclude'] = TRUE;
    $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['name']['alter']['external'] = 0;
    $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['name']['alter']['html'] = 0;
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['name']['hide_empty'] = 0;
    $handler->display->display_options['fields']['name']['empty_zero'] = 0;
    $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
    /* Field: Global: Custom text */
    $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
    $handler->display->display_options['fields']['nothing']['table'] = 'views';
    $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
    $handler->display->display_options['fields']['nothing']['label'] = '';
    $handler->display->display_options['fields']['nothing']['alter']['text'] = 'To [name]';
    $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['nothing']['alter']['path'] = 'getdirections/latlon/to/[latitude],[longitude]/[name]';
    $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 1;
    $handler->display->display_options['fields']['nothing']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
    $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
    $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
    $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
    /* Sort criterion: Location: Distance / Proximity */
    $handler->display->display_options['sorts']['distance']['id'] = 'distance';
    $handler->display->display_options['sorts']['distance']['table'] = 'location';
    $handler->display->display_options['sorts']['distance']['field'] = 'distance';
    $handler->display->display_options['sorts']['distance']['origin'] = 'nid_arg';
    $handler->display->display_options['sorts']['distance']['nid_arg'] = 'nid';
    $handler->display->display_options['sorts']['distance']['nid_loc_field'] = 'field_location';
    /* Contextual filter: Content: Nid */
    $handler->display->display_options['arguments']['nid']['id'] = 'nid';
    $handler->display->display_options['arguments']['nid']['table'] = 'node';
    $handler->display->display_options['arguments']['nid']['field'] = 'nid';
    $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
    $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
    $handler->display->display_options['arguments']['nid']['not'] = 0;
    /* Filter criterion: Content: Published */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'node';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 1;
    $handler->display->display_options['filters']['status']['group'] = 0;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
    /* Filter criterion: Location: Lid */
    $handler->display->display_options['filters']['lid']['id'] = 'lid';
    $handler->display->display_options['filters']['lid']['table'] = 'location';
    $handler->display->display_options['filters']['lid']['field'] = 'lid';
    $handler->display->display_options['filters']['lid']['operator'] = 'not empty';
    $handler->display->display_options['filters']['lid']['expose']['operator'] = FALSE;

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block');

    $views[$view->name] = $view;


    /**
    * getdirections user
    */
    $view = new view;
    $view->name = 'getdirections_user';
    $view->description = 'A link and block view to get directions for a location-enabled user.';
    $view->tag = 'getdirections';
    $view->base_table = 'users';
    $view->human_name = 'getdirections_user';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['access']['type'] = 'none';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['label'] = '';
    $handler->display->display_options['fields']['name']['exclude'] = TRUE;
    $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['name']['alter']['html'] = 0;
    $handler->display->display_options['fields']['name']['hide_empty'] = 0;
    $handler->display->display_options['fields']['name']['empty_zero'] = 0;
    $handler->display->display_options['fields']['name']['link_to_user'] = 0;
    $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
    /* Field: User: Uid */
    $handler->display->display_options['fields']['uid']['id'] = 'uid';
    $handler->display->display_options['fields']['uid']['table'] = 'users';
    $handler->display->display_options['fields']['uid']['field'] = 'uid';
    $handler->display->display_options['fields']['uid']['label'] = '';
    $handler->display->display_options['fields']['uid']['alter']['alter_text'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['text'] = 'Get directions to [name]';
    $handler->display->display_options['fields']['uid']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['path'] = 'getdirections/location_user/to/[uid]';
    $handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['uid']['alter']['html'] = 0;
    $handler->display->display_options['fields']['uid']['hide_empty'] = 0;
    $handler->display->display_options['fields']['uid']['empty_zero'] = 0;
    $handler->display->display_options['fields']['uid']['link_to_user'] = 0;
    /* Contextual filter: User: Uid */
    $handler->display->display_options['arguments']['uid']['id'] = 'uid';
    $handler->display->display_options['arguments']['uid']['table'] = 'users';
    $handler->display->display_options['arguments']['uid']['field'] = 'uid';
    $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['uid']['exception']['title_enable'] = 1;
    $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
    $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
    $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['uid']['specify_validation'] = 1;
    $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
    $handler->display->display_options['arguments']['uid']['not'] = 0;
    /* Filter criterion: User: Active */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'users';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 1;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
    /* Filter criterion: Location: Lid */
    $handler->display->display_options['filters']['lid']['id'] = 'lid';
    $handler->display->display_options['filters']['lid']['table'] = 'location';
    $handler->display->display_options['filters']['lid']['field'] = 'lid';
    $handler->display->display_options['filters']['lid']['operator'] = 'not empty';
    $handler->display->display_options['filters']['lid']['expose']['operator'] = FALSE;

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_1');

    $views[$view->name] = $view;


    /**
    * getdirections user for multiple locations per user
    */
    $view = new view;
    $view->name = 'getdirections_multi_user';
    $view->description = 'A link and block view to get directions for use with multiple locations per user.';
    $view->tag = 'getdirections';
    $view->base_table = 'users';
    $view->human_name = 'getdirections_multi_user';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'getdirections_multi_user';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'access user profiles';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: Location: Latitude */
    $handler->display->display_options['fields']['latitude']['id'] = 'latitude';
    $handler->display->display_options['fields']['latitude']['table'] = 'location';
    $handler->display->display_options['fields']['latitude']['field'] = 'latitude';
    $handler->display->display_options['fields']['latitude']['label'] = '';
    $handler->display->display_options['fields']['latitude']['exclude'] = TRUE;
    $handler->display->display_options['fields']['latitude']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['external'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['latitude']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['latitude']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['latitude']['alter']['html'] = 0;
    $handler->display->display_options['fields']['latitude']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['latitude']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['latitude']['hide_empty'] = 0;
    $handler->display->display_options['fields']['latitude']['empty_zero'] = 0;
    $handler->display->display_options['fields']['latitude']['hide_alter_empty'] = 0;
    $handler->display->display_options['fields']['latitude']['style'] = 'dd';
    /* Field: Location: Longitude */
    $handler->display->display_options['fields']['longitude']['id'] = 'longitude';
    $handler->display->display_options['fields']['longitude']['table'] = 'location';
    $handler->display->display_options['fields']['longitude']['field'] = 'longitude';
    $handler->display->display_options['fields']['longitude']['label'] = '';
    $handler->display->display_options['fields']['longitude']['exclude'] = TRUE;
    $handler->display->display_options['fields']['longitude']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['external'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['longitude']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['longitude']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['longitude']['alter']['html'] = 0;
    $handler->display->display_options['fields']['longitude']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['longitude']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['longitude']['hide_empty'] = 0;
    $handler->display->display_options['fields']['longitude']['empty_zero'] = 0;
    $handler->display->display_options['fields']['longitude']['hide_alter_empty'] = 0;
    $handler->display->display_options['fields']['longitude']['style'] = 'dd';
    /* Field: Location: Name */
    $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
    $handler->display->display_options['fields']['name_1']['table'] = 'location';
    $handler->display->display_options['fields']['name_1']['field'] = 'name';
    $handler->display->display_options['fields']['name_1']['label'] = '';
    $handler->display->display_options['fields']['name_1']['exclude'] = TRUE;
    $handler->display->display_options['fields']['name_1']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['external'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['name_1']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['name_1']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['name_1']['alter']['html'] = 0;
    $handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['name_1']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['name_1']['hide_empty'] = 0;
    $handler->display->display_options['fields']['name_1']['empty_zero'] = 0;
    $handler->display->display_options['fields']['name_1']['hide_alter_empty'] = 0;
    /* Field: Global: Custom text */
    $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
    $handler->display->display_options['fields']['nothing']['table'] = 'views';
    $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
    $handler->display->display_options['fields']['nothing']['label'] = '';
    $handler->display->display_options['fields']['nothing']['alter']['text'] = 'To [name]';
    $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
    $handler->display->display_options['fields']['nothing']['alter']['path'] = 'getdirections/latlon/to/[latitude],[longitude]/[name]';
    $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 1;
    $handler->display->display_options['fields']['nothing']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
    $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
    $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
    $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
    $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
    /* Contextual filter: User: Uid */
    $handler->display->display_options['arguments']['uid']['id'] = 'uid';
    $handler->display->display_options['arguments']['uid']['table'] = 'users';
    $handler->display->display_options['arguments']['uid']['field'] = 'uid';
    $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
    $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
    $handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
    $handler->display->display_options['arguments']['uid']['not'] = 0;
    /* Filter criterion: User: Active */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'users';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 1;
    $handler->display->display_options['filters']['status']['group'] = 0;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
    /* Filter criterion: Location: Lid */
    $handler->display->display_options['filters']['lid']['id'] = 'lid';
    $handler->display->display_options['filters']['lid']['table'] = 'location';
    $handler->display->display_options['filters']['lid']['field'] = 'lid';
    $handler->display->display_options['filters']['lid']['operator'] = 'not empty';
    $handler->display->display_options['filters']['lid']['expose']['operator'] = FALSE;

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block');

    $views[$view->name] = $view;

  }
  elseif (module_exists('geofield')) {

    /**
    * getdirections node
    */
    $view = new view();
    $view->name = 'getdirections';
    $view->description = 'A link and block view to get directions for a geofield-enabled node.';
    $view->tag = 'getdirections';
    $view->base_table = 'node';
    $view->human_name = 'Getdirections';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['label'] = '';
    $handler->display->display_options['fields']['title']['exclude'] = TRUE;
    $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
    $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
    /* Field: Content: Nid */
    $handler->display->display_options['fields']['nid']['id'] = 'nid';
    $handler->display->display_options['fields']['nid']['table'] = 'node';
    $handler->display->display_options['fields']['nid']['field'] = 'nid';
    $handler->display->display_options['fields']['nid']['label'] = '';
    $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
    $handler->display->display_options['fields']['nid']['alter']['text'] = 'Get directions to [title]';
    $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['nid']['alter']['path'] = 'getdirections/location/to/[nid]';
    $handler->display->display_options['fields']['nid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
    /* Contextual filter: Content: Nid */
    $handler->display->display_options['arguments']['nid']['id'] = 'nid';
    $handler->display->display_options['arguments']['nid']['table'] = 'node';
    $handler->display->display_options['arguments']['nid']['field'] = 'nid';
    $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
    $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
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
    $handler->display->display_options['filters']['type']['value'] = array();

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_1');

    $views[$view->name] = $view;

    /**
    * getdirections user
    */
    $view = new view();
    $view->name = 'getdirections_user';
    $view->description = 'A link and block view to get directions for a geofield-enabled user';
    $view->tag = 'getdirections';
    $view->base_table = 'users';
    $view->human_name = 'Getdirections User';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'access user profiles';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['label'] = '';
    $handler->display->display_options['fields']['name']['exclude'] = TRUE;
    $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
    /* Field: User: Uid */
    $handler->display->display_options['fields']['uid']['id'] = 'uid';
    $handler->display->display_options['fields']['uid']['table'] = 'users';
    $handler->display->display_options['fields']['uid']['field'] = 'uid';
    $handler->display->display_options['fields']['uid']['label'] = '';
    $handler->display->display_options['fields']['uid']['alter']['alter_text'] = TRUE;
    $handler->display->display_options['fields']['uid']['alter']['text'] = 'Get directions to [name]';
    $handler->display->display_options['fields']['uid']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['uid']['alter']['path'] = 'getdirections/location_user/to/[uid]';
    $handler->display->display_options['fields']['uid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
    /* Contextual filter: User: Uid */
    $handler->display->display_options['arguments']['uid']['id'] = 'uid';
    $handler->display->display_options['arguments']['uid']['table'] = 'users';
    $handler->display->display_options['arguments']['uid']['field'] = 'uid';
    $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
    $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
    $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
    /* Filter criterion: User: Active */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'users';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = '1';
    $handler->display->display_options['filters']['status']['group'] = 1;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_1');

    $views[$view->name] = $view;

  }
  elseif (module_exists('geolocation')) {

    /**
    * getdirections node
    */
    $view = new view();
    $view->name = 'getdirections';
    $view->description = 'A link and block view to get directions for a geolocation-enabled node.';
    $view->tag = 'getdirections';
    $view->base_table = 'node';
    $view->human_name = 'Getdirections';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['label'] = '';
    $handler->display->display_options['fields']['title']['exclude'] = TRUE;
    $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
    $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
    /* Field: Content: Nid */
    $handler->display->display_options['fields']['nid']['id'] = 'nid';
    $handler->display->display_options['fields']['nid']['table'] = 'node';
    $handler->display->display_options['fields']['nid']['field'] = 'nid';
    $handler->display->display_options['fields']['nid']['label'] = '';
    $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
    $handler->display->display_options['fields']['nid']['alter']['text'] = 'Get directions to [title]';
    $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['nid']['alter']['path'] = 'getdirections/location/to/[nid]';
    $handler->display->display_options['fields']['nid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
    /* Contextual filter: Content: Nid */
    $handler->display->display_options['arguments']['nid']['id'] = 'nid';
    $handler->display->display_options['arguments']['nid']['table'] = 'node';
    $handler->display->display_options['arguments']['nid']['field'] = 'nid';
    $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
    $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
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
    $handler->display->display_options['filters']['type']['value'] = array();

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_1');

    $views[$view->name] = $view;

    /**
    * getdirections user
    */
    $view = new view();
    $view->name = 'getdirections_user';
    $view->description = 'A link and block view to get directions for a geolocation-enabled user';
    $view->tag = 'getdirections';
    $view->base_table = 'users';
    $view->human_name = 'Getdirections User';
    $view->core = 7;
    $view->api_version = '3';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Get driving directions';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'access user profiles';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['label'] = '';
    $handler->display->display_options['fields']['name']['exclude'] = TRUE;
    $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
    $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
    /* Field: User: Uid */
    $handler->display->display_options['fields']['uid']['id'] = 'uid';
    $handler->display->display_options['fields']['uid']['table'] = 'users';
    $handler->display->display_options['fields']['uid']['field'] = 'uid';
    $handler->display->display_options['fields']['uid']['label'] = '';
    $handler->display->display_options['fields']['uid']['alter']['alter_text'] = TRUE;
    $handler->display->display_options['fields']['uid']['alter']['text'] = 'Get directions to [name]';
    $handler->display->display_options['fields']['uid']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['uid']['alter']['path'] = 'getdirections/location_user/to/[uid]';
    $handler->display->display_options['fields']['uid']['alter']['alt'] = 'Get directions on a map';
    $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
    /* Contextual filter: User: Uid */
    $handler->display->display_options['arguments']['uid']['id'] = 'uid';
    $handler->display->display_options['arguments']['uid']['table'] = 'users';
    $handler->display->display_options['arguments']['uid']['field'] = 'uid';
    $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
    $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
    $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
    $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
    /* Filter criterion: User: Active */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'users';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = '1';
    $handler->display->display_options['filters']['status']['group'] = 1;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block_1');

    $views[$view->name] = $view;

  }

  return $views;
}

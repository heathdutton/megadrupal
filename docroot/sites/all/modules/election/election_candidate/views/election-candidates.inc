<?php
/**
 * @file
 * View listing candidates standing in an election, per election or per post.
 */

$view = new view();
$view->name = 'election_candidates';
$view->description = 'A list of candidates (intended for the public / all viewers of the election).';
$view->tag = 'election';
$view->base_table = 'election_candidate';
$view->human_name = 'Election candidates';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Candidates';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view published elections';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['slave'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['style_options']['row_class'] = 'election-candidate-row clearfix';
$handler->display->display_options['row_plugin'] = 'entity';
$handler->display->display_options['row_options']['view_mode'] = 'teaser';
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'No candidates found';
$handler->display->display_options['empty']['area']['content'] = 'No candidates found.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
/* Sort criterion: Election post: Title */
$handler->display->display_options['sorts']['title']['id'] = 'title';
$handler->display->display_options['sorts']['title']['table'] = 'election_post';
$handler->display->display_options['sorts']['title']['field'] = 'title';
/* Sort criterion: Global: Random */
$handler->display->display_options['sorts']['random']['id'] = 'random';
$handler->display->display_options['sorts']['random']['table'] = 'views';
$handler->display->display_options['sorts']['random']['field'] = 'random';
/* Contextual filter: Election: Election ID */
$handler->display->display_options['arguments']['election_id']['id'] = 'election_id';
$handler->display->display_options['arguments']['election_id']['table'] = 'election';
$handler->display->display_options['arguments']['election_id']['field'] = 'election_id';
$handler->display->display_options['arguments']['election_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['election_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['election_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['election_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['election_id']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['election_id']['validate']['type'] = 'numeric';
$handler->display->display_options['arguments']['election_id']['validate']['fail'] = 'empty';
/* Filter criterion: Election: Published or admin */
$handler->display->display_options['filters']['published_or_admin']['id'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['table'] = 'election';
$handler->display->display_options['filters']['published_or_admin']['field'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['group'] = 1;
/* Filter criterion: Election candidate: Published or admin */
$handler->display->display_options['filters']['published_or_admin_1']['id'] = 'published_or_admin_1';
$handler->display->display_options['filters']['published_or_admin_1']['table'] = 'election_candidate';
$handler->display->display_options['filters']['published_or_admin_1']['field'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin_1']['group'] = 1;
/* Filter criterion: Election candidate: Candidate status */
$handler->display->display_options['filters']['cstatus']['id'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['table'] = 'election_candidate';
$handler->display->display_options['filters']['cstatus']['field'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['operator'] = 'not in';
$handler->display->display_options['filters']['cstatus']['value'] = array(
  -2 => '-2',
);
$handler->display->display_options['filters']['cstatus']['group'] = 1;

/* Display: Candidates per election */
$handler = $view->new_display('embed', 'Candidates per election', 'embed_candidates_per_election');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'title',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['row_class'] = 'election-candidate-row clearfix';
$handler->display->display_options['style_options']['uses_fields'] = TRUE;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'entity';
$handler->display->display_options['row_options']['view_mode'] = 'teaser';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Standing for';
$handler->display->display_options['fields']['title']['link_to_post'] = 1;

/* Display: Candidates per post */
$handler = $view->new_display('embed', 'Candidates per post', 'embed_candidates_per_post');
$handler->display->display_options['defaults']['css_class'] = FALSE;
$handler->display->display_options['css_class'] = 'election-post-candidates';
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['defaults']['header'] = FALSE;
$handler->display->display_options['defaults']['empty'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Election post: Post ID */
$handler->display->display_options['arguments']['post_id']['id'] = 'post_id';
$handler->display->display_options['arguments']['post_id']['table'] = 'election_post';
$handler->display->display_options['arguments']['post_id']['field'] = 'post_id';
$handler->display->display_options['arguments']['post_id']['default_action'] = 'empty';
$handler->display->display_options['arguments']['post_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['post_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['post_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['post_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['post_id']['validate']['type'] = 'numeric';
$handler->display->display_options['arguments']['post_id']['validate']['fail'] = 'empty';

$views[$view->name] = $view;

<?php

/**
 * @file
 * Default view for CD Sunlight.
 */

/**
 * Implements hook_views_default_views().
 */
function cd_sunlight_views_default_views() {
  /*
   * View 'Congress'
   */
  $view = new view;
  $view->name = 'congress';
  $view->description = 'Current members of Congress.';
  $view->tag = '';
  $view->base_table = 'cd_sunlight_legislators';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Members of Congress';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['use_pager'] = 1;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = 'nothing';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['separator'] = ' ';
  $handler->display->display_options['row_options']['hide_empty'] = 1;

  /* Field: Congress: Picture */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['picture']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['picture']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['picture']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['picture']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['picture']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['picture']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['picture']['alter']['html'] = 0;
  $handler->display->display_options['fields']['picture']['hide_empty'] = 0;
  $handler->display->display_options['fields']['picture']['empty_zero'] = 0;
  $handler->display->display_options['fields']['picture']['format'] = '';
  /* Field: Congress: State */
  $handler->display->display_options['fields']['state']['id'] = 'state';
  $handler->display->display_options['fields']['state']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['state']['field'] = 'state';
  $handler->display->display_options['fields']['state']['label'] = '';
  $handler->display->display_options['fields']['state']['exclude'] = TRUE;
  $handler->display->display_options['fields']['state']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['state']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['state']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['state']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['state']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['state']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['state']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['state']['alter']['html'] = 0;
  $handler->display->display_options['fields']['state']['hide_empty'] = 0;
  $handler->display->display_options['fields']['state']['empty_zero'] = 0;
  /* Field: Congress: District */
  $handler->display->display_options['fields']['district']['id'] = 'district';
  $handler->display->display_options['fields']['district']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['district']['field'] = 'district';
  $handler->display->display_options['fields']['district']['label'] = '';
  $handler->display->display_options['fields']['district']['exclude'] = TRUE;
  $handler->display->display_options['fields']['district']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['district']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['district']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['district']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['district']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['district']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['district']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['district']['alter']['html'] = 0;
  $handler->display->display_options['fields']['district']['hide_empty'] = 1;
  $handler->display->display_options['fields']['district']['empty_zero'] = 1;
  $handler->display->display_options['fields']['district']['format_plural'] = 0;
  /* Field: Congress: Chamber */
  $handler->display->display_options['fields']['chamber']['id'] = 'chamber';
  $handler->display->display_options['fields']['chamber']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['chamber']['field'] = 'chamber';
  $handler->display->display_options['fields']['chamber']['label'] = '';
  $handler->display->display_options['fields']['chamber']['exclude'] = TRUE;
  $handler->display->display_options['fields']['chamber']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['chamber']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['chamber']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['chamber']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['chamber']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['chamber']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['chamber']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['chamber']['alter']['html'] = 0;
  $handler->display->display_options['fields']['chamber']['hide_empty'] = 0;
  $handler->display->display_options['fields']['chamber']['empty_zero'] = 0;
  /* Field: H3 */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'H3';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<h3>[state][district] [chamber]</h3>';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  /* Field: Congress: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  /* Field: Congress: First Name */
  $handler->display->display_options['fields']['firstname']['id'] = 'firstname';
  $handler->display->display_options['fields']['firstname']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['firstname']['field'] = 'firstname';
  $handler->display->display_options['fields']['firstname']['label'] = '';
  $handler->display->display_options['fields']['firstname']['exclude'] = TRUE;
  $handler->display->display_options['fields']['firstname']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['firstname']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['firstname']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['firstname']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['firstname']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['firstname']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['firstname']['alter']['html'] = 0;
  $handler->display->display_options['fields']['firstname']['hide_empty'] = 0;
  $handler->display->display_options['fields']['firstname']['empty_zero'] = 0;
  /* Field: Congress: Middle Name */
  $handler->display->display_options['fields']['middlename']['id'] = 'middlename';
  $handler->display->display_options['fields']['middlename']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['middlename']['field'] = 'middlename';
  $handler->display->display_options['fields']['middlename']['label'] = '';
  $handler->display->display_options['fields']['middlename']['exclude'] = TRUE;
  $handler->display->display_options['fields']['middlename']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['middlename']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['middlename']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['middlename']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['middlename']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['middlename']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['middlename']['alter']['html'] = 0;
  $handler->display->display_options['fields']['middlename']['hide_empty'] = 0;
  $handler->display->display_options['fields']['middlename']['empty_zero'] = 0;
  /* Field: Congress: Nickname */
  $handler->display->display_options['fields']['nickname']['id'] = 'nickname';
  $handler->display->display_options['fields']['nickname']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['nickname']['field'] = 'nickname';
  $handler->display->display_options['fields']['nickname']['label'] = '';
  $handler->display->display_options['fields']['nickname']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nickname']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nickname']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nickname']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nickname']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nickname']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nickname']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nickname']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nickname']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nickname']['hide_empty'] = 1;
  $handler->display->display_options['fields']['nickname']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nickname']['quotes'] = 1;
  /* Field: Congress: Last Name */
  $handler->display->display_options['fields']['lastname']['id'] = 'lastname';
  $handler->display->display_options['fields']['lastname']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['lastname']['field'] = 'lastname';
  $handler->display->display_options['fields']['lastname']['label'] = '';
  $handler->display->display_options['fields']['lastname']['exclude'] = TRUE;
  $handler->display->display_options['fields']['lastname']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['lastname']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['lastname']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['lastname']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['lastname']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['lastname']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['lastname']['alter']['html'] = 0;
  $handler->display->display_options['fields']['lastname']['hide_empty'] = 0;
  $handler->display->display_options['fields']['lastname']['empty_zero'] = 0;
  /* Field: Congress: Name Suffix */
  $handler->display->display_options['fields']['name_suffix']['id'] = 'name_suffix';
  $handler->display->display_options['fields']['name_suffix']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['name_suffix']['field'] = 'name_suffix';
  $handler->display->display_options['fields']['name_suffix']['label'] = '';
  $handler->display->display_options['fields']['name_suffix']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name_suffix']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name_suffix']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name_suffix']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name_suffix']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name_suffix']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name_suffix']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name_suffix']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name_suffix']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name_suffix']['empty_zero'] = 0;
  /* Field: Congress: Party */
  $handler->display->display_options['fields']['party']['id'] = 'party';
  $handler->display->display_options['fields']['party']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['party']['field'] = 'party';
  $handler->display->display_options['fields']['party']['label'] = '';
  $handler->display->display_options['fields']['party']['exclude'] = TRUE;
  $handler->display->display_options['fields']['party']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['party']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['party']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['party']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['party']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['party']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['party']['alter']['html'] = 0;
  $handler->display->display_options['fields']['party']['hide_empty'] = 0;
  $handler->display->display_options['fields']['party']['empty_zero'] = 0;
  /* Field: FULL NAME */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['ui_name'] = 'FULL NAME';
  $handler->display->display_options['fields']['nothing_2']['label'] = '';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '<h4 class="representative-fullname">[title] [firstname] [middlename] [nickname] [lastname] [name_suffix] [party]</h4>';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_2']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_2']['empty_zero'] = 0;
  /* Field: Congress: Webform */
  $handler->display->display_options['fields']['webform']['id'] = 'webform';
  $handler->display->display_options['fields']['webform']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['webform']['field'] = 'webform';
  $handler->display->display_options['fields']['webform']['label'] = '';
  $handler->display->display_options['fields']['webform']['exclude'] = TRUE;
  $handler->display->display_options['fields']['webform']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['webform']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['webform']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['webform']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['webform']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['webform']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['webform']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['webform']['alter']['html'] = 0;
  $handler->display->display_options['fields']['webform']['hide_empty'] = 1;
  $handler->display->display_options['fields']['webform']['empty_zero'] = 0;
  $handler->display->display_options['fields']['webform']['display_as_link'] = 1;
  $handler->display->display_options['fields']['webform']['display_as_favicon'] = 0;
  $handler->display->display_options['fields']['webform']['new_window'] = 1;
  /* Field: Congress: Facebook URL */
  $handler->display->display_options['fields']['facebook_url']['id'] = 'facebook_url';
  $handler->display->display_options['fields']['facebook_url']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['facebook_url']['field'] = 'facebook_url';
  $handler->display->display_options['fields']['facebook_url']['label'] = '';
  $handler->display->display_options['fields']['facebook_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['facebook_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['facebook_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['facebook_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['facebook_url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['facebook_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['facebook_url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['facebook_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['facebook_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['facebook_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['facebook_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['facebook_url']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['facebook_url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['facebook_url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['facebook_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['facebook_url']['display_as_link'] = 1;
  $handler->display->display_options['fields']['facebook_url']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['facebook_url']['new_window'] = 1;
  /* Field: Congress: Twitter Page */
  $handler->display->display_options['fields']['twitter_id']['id'] = 'twitter_id';
  $handler->display->display_options['fields']['twitter_id']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['twitter_id']['field'] = 'twitter_id';
  $handler->display->display_options['fields']['twitter_id']['label'] = '';
  $handler->display->display_options['fields']['twitter_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['twitter_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['twitter_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['twitter_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['twitter_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['twitter_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['twitter_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['twitter_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['twitter_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['twitter_id']['hide_empty'] = 1;
  $handler->display->display_options['fields']['twitter_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['twitter_id']['display_as_link'] = 1;
  $handler->display->display_options['fields']['twitter_id']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['twitter_id']['new_window'] = 1;
  /* Field: Congress: YouTube Page */
  $handler->display->display_options['fields']['youtube_url']['id'] = 'youtube_url';
  $handler->display->display_options['fields']['youtube_url']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['youtube_url']['field'] = 'youtube_url';
  $handler->display->display_options['fields']['youtube_url']['label'] = '';
  $handler->display->display_options['fields']['youtube_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['youtube_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['youtube_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['youtube_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['youtube_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['youtube_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['youtube_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['youtube_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['youtube_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['youtube_url']['hide_empty'] = 1;
  $handler->display->display_options['fields']['youtube_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['youtube_url']['display_as_link'] = 1;
  $handler->display->display_options['fields']['youtube_url']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['youtube_url']['new_window'] = 1;
  /* Field: Congress: BioGuide page */
  $handler->display->display_options['fields']['bioguide_id']['id'] = 'bioguide_id';
  $handler->display->display_options['fields']['bioguide_id']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['bioguide_id']['field'] = 'bioguide_id';
  $handler->display->display_options['fields']['bioguide_id']['label'] = '';
  $handler->display->display_options['fields']['bioguide_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['bioguide_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['bioguide_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['bioguide_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['bioguide_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['bioguide_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['bioguide_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['bioguide_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['bioguide_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['bioguide_id']['hide_empty'] = 1;
  $handler->display->display_options['fields']['bioguide_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['bioguide_id']['display_as_link'] = 1;
  $handler->display->display_options['fields']['bioguide_id']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['bioguide_id']['new_window'] = 1;
  /* Field: Congress: Govtrack page */
  $handler->display->display_options['fields']['govtrack_id']['id'] = 'govtrack_id';
  $handler->display->display_options['fields']['govtrack_id']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['govtrack_id']['field'] = 'govtrack_id';
  $handler->display->display_options['fields']['govtrack_id']['label'] = '';
  $handler->display->display_options['fields']['govtrack_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['govtrack_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['govtrack_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['govtrack_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['govtrack_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['govtrack_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['govtrack_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['govtrack_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['govtrack_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['govtrack_id']['hide_empty'] = 1;
  $handler->display->display_options['fields']['govtrack_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['govtrack_id']['display_as_link'] = 1;
  $handler->display->display_options['fields']['govtrack_id']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['govtrack_id']['new_window'] = 1;
  /* Field: Congress: OpenCongress page */
  $handler->display->display_options['fields']['congresspedia_url']['id'] = 'congresspedia_url';
  $handler->display->display_options['fields']['congresspedia_url']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['congresspedia_url']['field'] = 'congresspedia_url';
  $handler->display->display_options['fields']['congresspedia_url']['label'] = '';
  $handler->display->display_options['fields']['congresspedia_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['congresspedia_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['congresspedia_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['congresspedia_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['congresspedia_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['congresspedia_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['congresspedia_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['congresspedia_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['congresspedia_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['congresspedia_url']['hide_empty'] = 1;
  $handler->display->display_options['fields']['congresspedia_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['congresspedia_url']['display_as_link'] = 1;
  $handler->display->display_options['fields']['congresspedia_url']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['congresspedia_url']['new_window'] = 1;
  /* Field: Congress: OpenSecrets page */
  $handler->display->display_options['fields']['crp_id']['id'] = 'crp_id';
  $handler->display->display_options['fields']['crp_id']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['crp_id']['field'] = 'crp_id';
  $handler->display->display_options['fields']['crp_id']['label'] = '';
  $handler->display->display_options['fields']['crp_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['crp_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['crp_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['crp_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['crp_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['crp_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['crp_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['crp_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['crp_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['crp_id']['hide_empty'] = 1;
  $handler->display->display_options['fields']['crp_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['crp_id']['display_as_link'] = 1;
  $handler->display->display_options['fields']['crp_id']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['crp_id']['new_window'] = 1;
  /* Field: Congress: Progressive Punch Page */
  $handler->display->display_options['fields']['progressive_punch']['id'] = 'progressive_punch';
  $handler->display->display_options['fields']['progressive_punch']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['progressive_punch']['field'] = 'progressive_punch';
  $handler->display->display_options['fields']['progressive_punch']['label'] = '';
  $handler->display->display_options['fields']['progressive_punch']['exclude'] = TRUE;
  $handler->display->display_options['fields']['progressive_punch']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['progressive_punch']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['progressive_punch']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['progressive_punch']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['progressive_punch']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['progressive_punch']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['progressive_punch']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['progressive_punch']['alter']['html'] = 0;
  $handler->display->display_options['fields']['progressive_punch']['hide_empty'] = 0;
  $handler->display->display_options['fields']['progressive_punch']['empty_zero'] = 0;
  $handler->display->display_options['fields']['progressive_punch']['display_as_link'] = 1;
  $handler->display->display_options['fields']['progressive_punch']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['progressive_punch']['new_window'] = 1;
  /* Field: Congress: VoteSmart page */
  $handler->display->display_options['fields']['votesmart_id']['id'] = 'votesmart_id';
  $handler->display->display_options['fields']['votesmart_id']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['votesmart_id']['field'] = 'votesmart_id';
  $handler->display->display_options['fields']['votesmart_id']['label'] = '';
  $handler->display->display_options['fields']['votesmart_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['votesmart_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['votesmart_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['votesmart_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['votesmart_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['votesmart_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['votesmart_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['votesmart_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['votesmart_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['votesmart_id']['hide_empty'] = 1;
  $handler->display->display_options['fields']['votesmart_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['votesmart_id']['display_as_link'] = 1;
  $handler->display->display_options['fields']['votesmart_id']['display_as_favicon'] = 1;
  $handler->display->display_options['fields']['votesmart_id']['new_window'] = 1;
  /* Field: LINKS */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['ui_name'] = 'LINKS';
  $handler->display->display_options['fields']['nothing_3']['label'] = '';
  $handler->display->display_options['fields']['nothing_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '<div class="representative-links">[webform] [facebook_url] [twitter_id] [youtube_url] [bioguide_id] [govtrack_id] [congresspedia_url] [crp_id] [progressive_punch] [votesmart_id]</div>';
  $handler->display->display_options['fields']['nothing_3']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_3']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_3']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_3']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_3']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_3']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_3']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_3']['empty_zero'] = 0;
  /* Field: Congress: Phone */
  $handler->display->display_options['fields']['phone']['id'] = 'phone';
  $handler->display->display_options['fields']['phone']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['phone']['field'] = 'phone';
  $handler->display->display_options['fields']['phone']['label'] = '';
  $handler->display->display_options['fields']['phone']['exclude'] = TRUE;
  $handler->display->display_options['fields']['phone']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['phone']['alter']['text'] = 'p: [phone]';
  $handler->display->display_options['fields']['phone']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['phone']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['phone']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['phone']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['phone']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['phone']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['phone']['alter']['html'] = 0;
  $handler->display->display_options['fields']['phone']['hide_empty'] = 1;
  $handler->display->display_options['fields']['phone']['empty_zero'] = 0;
  /* Field: Congress: Fax */
  $handler->display->display_options['fields']['fax']['id'] = 'fax';
  $handler->display->display_options['fields']['fax']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['fields']['fax']['field'] = 'fax';
  $handler->display->display_options['fields']['fax']['label'] = '';
  $handler->display->display_options['fields']['fax']['exclude'] = TRUE;
  $handler->display->display_options['fields']['fax']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['fax']['alter']['text'] = 'f: [fax]';
  $handler->display->display_options['fields']['fax']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['fax']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['fax']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['fax']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['fax']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['fax']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['fax']['alter']['html'] = 0;
  $handler->display->display_options['fields']['fax']['hide_empty'] = 1;
  $handler->display->display_options['fields']['fax']['empty_zero'] = 0;
  /* Field: PHONES */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['ui_name'] = 'PHONES';
  $handler->display->display_options['fields']['nothing_4']['label'] = '';
  $handler->display->display_options['fields']['nothing_4']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = '<div class="representative-phones">[phone] [fax]</div>';
  $handler->display->display_options['fields']['nothing_4']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_4']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_4']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_4']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_4']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing_4']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_4']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_4']['empty_zero'] = 0;
  /* Field: ALL METADATA */
  $handler->display->display_options['fields']['representative_data']['id'] = 'representative_data';
  $handler->display->display_options['fields']['representative_data']['table'] = 'views';
  $handler->display->display_options['fields']['representative_data']['field'] = 'nothing';
  $handler->display->display_options['fields']['representative_data']['ui_name'] = 'ALL METADATA';
  $handler->display->display_options['fields']['representative_data']['label'] = '';
  $handler->display->display_options['fields']['representative_data']['alter']['text'] = '[nothing_2][nothing_3][nothing_4]';
  $handler->display->display_options['fields']['representative_data']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['representative_data']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['representative_data']['alter']['external'] = 0;
  $handler->display->display_options['fields']['representative_data']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['representative_data']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['representative_data']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['representative_data']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['representative_data']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['representative_data']['alter']['html'] = 0;
  $handler->display->display_options['fields']['representative_data']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['representative_data']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['representative_data']['hide_empty'] = 0;
  $handler->display->display_options['fields']['representative_data']['empty_zero'] = 0;

  /* Sort criterion: Congress: State */
  $handler->display->display_options['sorts']['state']['id'] = 'state';
  $handler->display->display_options['sorts']['state']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['sorts']['state']['field'] = 'state';
  /* Sort criterion: Congress: District */
  $handler->display->display_options['sorts']['district']['id'] = 'district';
  $handler->display->display_options['sorts']['district']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['sorts']['district']['field'] = 'district';

  /* Argument: Congress: State */
  $handler->display->display_options['arguments']['state']['id'] = 'state';
  $handler->display->display_options['arguments']['state']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['arguments']['state']['field'] = 'state';
  $handler->display->display_options['arguments']['state']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['state']['default_argument_type'] = 'fixed';
  /* Argument: Congress: District */
  $handler->display->display_options['arguments']['district']['id'] = 'district';
  $handler->display->display_options['arguments']['district']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['arguments']['district']['field'] = 'district';
  $handler->display->display_options['arguments']['district']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['district']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['district']['break_phrase'] = 1;
  $handler->display->display_options['arguments']['district']['not'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter: Congress: Party */
  $handler->display->display_options['filters']['party']['id'] = 'party';
  $handler->display->display_options['filters']['party']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['filters']['party']['field'] = 'party';
  $handler->display->display_options['filters']['party']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party']['expose']['label'] = 'Party';
  $handler->display->display_options['filters']['party']['expose']['identifier'] = 'party';
  $handler->display->display_options['filters']['party']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['party']['expose']['optional'] = 1;
  $handler->display->display_options['filters']['party']['expose']['single'] = 1;
  $handler->display->display_options['filters']['party']['expose']['remember'] = 1;
  /* Filter: Congress: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['title']['expose']['optional'] = 1;
  $handler->display->display_options['filters']['title']['expose']['single'] = 1;
  $handler->display->display_options['filters']['title']['expose']['remember'] = 1;
  /* Filter: Congress: State */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['exposed'] = TRUE;
  $handler->display->display_options['filters']['state']['expose']['label'] = 'State';
  $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
  $handler->display->display_options['filters']['state']['expose']['remember'] = 1;
  $handler->display->display_options['filters']['state']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['state']['expose']['optional'] = 1;
  $handler->display->display_options['filters']['state']['expose']['single'] = 1;
  $handler->display->display_options['filters']['state']['reduce_duplicates'] = 0;
  /* Filter: Congress: Gender */
  $handler->display->display_options['filters']['gender']['id'] = 'gender';
  $handler->display->display_options['filters']['gender']['table'] = 'cd_sunlight_legislators';
  $handler->display->display_options['filters']['gender']['field'] = 'gender';
  $handler->display->display_options['filters']['gender']['exposed'] = TRUE;
  $handler->display->display_options['filters']['gender']['expose']['label'] = 'Gender';
  $handler->display->display_options['filters']['gender']['expose']['identifier'] = 'gender';
  $handler->display->display_options['filters']['gender']['expose']['remember'] = 1;
  $handler->display->display_options['filters']['gender']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['gender']['expose']['optional'] = 1;
  $handler->display->display_options['filters']['gender']['expose']['single'] = 1;
  $handler->display->display_options['path'] = 'congress';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');

  $views[$view->name] = $view;

  $translatables['Congress'] = array(
    t('Defaults'),
    t('Members of Congress'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort By'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('.'),
    t(','),
    t('<h3>[state][district] [chamber]</h3>'),
    t('<h4 class="representative-fullname">[title] [firstname] [middlename] [nickname] [lastname] [name_suffix] [party]</h4>'),
    t('<div class="representative-links">[webform] [facebook_url] [twitter_id] [youtube_url] [bioguide_id] [govtrack_id] [congresspedia_url] [crp_id] [progressive_punch] [votesmart_id]</div>'),
    t('p: [phone]'),
    t('f: [fax]'),
    t('<div class="representative-phones">[phone] [fax]</div>'),
    t('[nothing_2][nothing_3][nothing_4]'),
    t('All'),
    t('Page'),
    t('Party'),
    t('Title'),
    t('State'),
    t('Gender'),
    t('Block'),
    t('Block - Current user'),
    t('Page - Current user'),
  );

  return $views;
}

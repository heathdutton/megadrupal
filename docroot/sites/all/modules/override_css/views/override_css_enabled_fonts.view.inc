<?php

/**
 * @file
 * Overzicht van alle enabled fonts.
 */

$view = new view;
$view->name = 'override_css_enabled_fonts';
$view->description = 'Overzicht van alle enabled fonts.';
$view->tag = '@font-your-face';
$view->base_table = 'fontyourface_font';
$view->human_name = 'Enabled fonts';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Beschikbare lettertypes';
$handler->display->display_options['css_class'] = 'view-fontyourface-browse';
$handler->display->display_options['use_more_text'] = 'meer';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'override css settings';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
$handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Opnieuw instellen';
$handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sorteer op';
$handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Oplopend';
$handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Aflopend';
$handler->display->display_options['exposed_form']['options']['autosubmit'] = 1;
$handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 0;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'grid';
$handler->display->display_options['style_options']['fill_single_line'] = 1;
$handler->display->display_options['row_plugin'] = 'fields';
/* Veld: Lettertype: Naam */
$handler->display->display_options['fields']['name_1']['id'] = 'name_1';
$handler->display->display_options['fields']['name_1']['table'] = 'fontyourface_font';
$handler->display->display_options['fields']['name_1']['field'] = 'name';
$handler->display->display_options['fields']['name_1']['label'] = '';
$handler->display->display_options['fields']['name_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['text'] = '<span>Edit</span>';
$handler->display->display_options['fields']['name_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['external'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name_1']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['name_1']['element_type'] = '0';
$handler->display->display_options['fields']['name_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['name_1']['element_wrapper_type'] = '0';
$handler->display->display_options['fields']['name_1']['element_default_classes'] = 0;
$handler->display->display_options['fields']['name_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['name_1']['empty_zero'] = 0;
$handler->display->display_options['fields']['name_1']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['name_1']['link_to_font'] = '0';
$handler->display->display_options['fields']['name_1']['display_as'] = '0';
/* Veld: Lettertype: Voorbeeldweergave */
$handler->display->display_options['fields']['preview']['id'] = 'preview';
$handler->display->display_options['fields']['preview']['table'] = 'fontyourface_font';
$handler->display->display_options['fields']['preview']['field'] = 'preview';
$handler->display->display_options['fields']['preview']['label'] = '';
$handler->display->display_options['fields']['preview']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['preview']['alter']['text'] = '<div class="fontyourface-preview enabled-[enabled_yes_no]">[preview]</div>';
$handler->display->display_options['fields']['preview']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['preview']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['preview']['alter']['external'] = 0;
$handler->display->display_options['fields']['preview']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['preview']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['preview']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['preview']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['preview']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['preview']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['preview']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['preview']['alter']['trim'] = 0;
$handler->display->display_options['fields']['preview']['alter']['html'] = 0;
$handler->display->display_options['fields']['preview']['element_type'] = '0';
$handler->display->display_options['fields']['preview']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['preview']['element_wrapper_type'] = '0';
$handler->display->display_options['fields']['preview']['element_default_classes'] = 0;
$handler->display->display_options['fields']['preview']['hide_empty'] = 0;
$handler->display->display_options['fields']['preview']['empty_zero'] = 0;
$handler->display->display_options['fields']['preview']['hide_alter_empty'] = 0;
/* Sorteercriterium: Lettertype: Naam */
$handler->display->display_options['sorts']['name']['id'] = 'name';
$handler->display->display_options['sorts']['name']['table'] = 'fontyourface_font';
$handler->display->display_options['sorts']['name']['field'] = 'name';
/* Filtercriterium: Lettertype: Ingeschakeld */
$handler->display->display_options['filters']['enabled']['id'] = 'enabled';
$handler->display->display_options['filters']['enabled']['table'] = 'fontyourface_font';
$handler->display->display_options['filters']['enabled']['field'] = 'enabled';
$handler->display->display_options['filters']['enabled']['value'] = '1';
$translatables['override_css_enabled_fonts'] = array(
  t('Defaults'),
  t('Beschikbare lettertypes'),
  t('meer'),
  t('Toepassen'),
  t('Opnieuw instellen'),
  t('Sorteer op'),
  t('Oplopend'),
  t('Aflopend'),
  t('<span>Edit</span>'),
  t('<div class="fontyourface-preview enabled-[enabled_yes_no]">[preview]</div>'),
);

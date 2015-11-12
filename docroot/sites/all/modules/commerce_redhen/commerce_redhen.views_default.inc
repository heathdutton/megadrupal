<?php

/**
 * @file
 * commerce_redhen.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_redhen_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'entity_reference_redhen_organizations';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'redhen_org';
  $view->human_name = 'Entity Reference Redhen Organizations';
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
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Organization: Relation: Affiliated with (redhen_org → redhen_contact) */
  $handler->display->display_options['relationships']['relation_redhen_affiliation_redhen_contact']['id'] = 'relation_redhen_affiliation_redhen_contact';
  $handler->display->display_options['relationships']['relation_redhen_affiliation_redhen_contact']['table'] = 'redhen_org';
  $handler->display->display_options['relationships']['relation_redhen_affiliation_redhen_contact']['field'] = 'relation_redhen_affiliation_redhen_contact';
  $handler->display->display_options['relationships']['relation_redhen_affiliation_redhen_contact']['r_index'] = '1';
  $handler->display->display_options['relationships']['relation_redhen_affiliation_redhen_contact']['entity_deduplication_left'] = 0;
  $handler->display->display_options['relationships']['relation_redhen_affiliation_redhen_contact']['entity_deduplication_right'] = 0;
  /* Relationship: Organization: Relation: Affiliated with (redhen_org → relation) */
  $handler->display->display_options['relationships']['relation_base_right_redhen_affiliation']['id'] = 'relation_base_right_redhen_affiliation';
  $handler->display->display_options['relationships']['relation_base_right_redhen_affiliation']['table'] = 'redhen_org';
  $handler->display->display_options['relationships']['relation_base_right_redhen_affiliation']['field'] = 'relation_base_right_redhen_affiliation';
  $handler->display->display_options['relationships']['relation_base_right_redhen_affiliation']['r_index'] = '0';
  $handler->display->display_options['relationships']['relation_base_right_redhen_affiliation']['entity_deduplication_left'] = 0;
  /* Relationship: User: Contact User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'redhen_contact_user';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'relation_redhen_affiliation_redhen_contact';
  /* Field: Organization: Organization Name */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'redhen_org';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = '';
  $handler->display->display_options['fields']['label']['element_label_colon'] = FALSE;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'label' => 'label',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $translatables['entity_reference_redhen_organizations'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Affiliated with'),
    t('User'),
    t('All'),
    t('Entity Reference'),
  );


  $export['entity_reference_redhen_organizations'] = $view;

  return $export;
}

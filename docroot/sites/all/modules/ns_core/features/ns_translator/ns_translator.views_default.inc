<?php
/**
 * @file
 * ns_translator.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function ns_translator_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'ns_translator';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'entity_translation';
  $view->human_name = 'ns_translator';
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
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'label' => 'label',
    'source' => 'source',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'label' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'source' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'Nothing more to translate!';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Field: Entity translation: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'entity_translation';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['link_to_entity'] = 0;
  /* Field: Entity translation: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'entity_translation';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Entity translation: Language */
  $handler->display->display_options['fields']['language']['id'] = 'language';
  $handler->display->display_options['fields']['language']['table'] = 'entity_translation';
  $handler->display->display_options['fields']['language']['field'] = 'language';
  $handler->display->display_options['fields']['language']['native_language'] = TRUE;
  /* Field: Entity translation: Translate link */
  $handler->display->display_options['fields']['translate_link']['id'] = 'translate_link';
  $handler->display->display_options['fields']['translate_link']['table'] = 'entity_translation';
  $handler->display->display_options['fields']['translate_link']['field'] = 'translate_link';
  $handler->display->display_options['fields']['translate_link']['label'] = 'Operations';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  /* Filter criterion: Entity translation: Entity type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['value'] = array(
    'node' => 'node',
  );
  $handler->display->display_options['filters']['entity_type']['group'] = 1;
  $handler->display->display_options['filters']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator_id'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['label'] = 'Entity type';
  $handler->display->display_options['filters']['entity_type']['expose']['operator'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['identifier'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['expose']['required'] = TRUE;
  /* Filter criterion: Entity translation: Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['operator'] = 'not in';
  $handler->display->display_options['filters']['language']['value'] = array(
    'und' => 'und',
  );
  $handler->display->display_options['filters']['language']['group'] = 1;
  /* Filter criterion: Entity translation: Language */
  $handler->display->display_options['filters']['language_1']['id'] = 'language_1';
  $handler->display->display_options['filters']['language_1']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['language_1']['field'] = 'language';
  $handler->display->display_options['filters']['language_1']['group'] = 1;
  $handler->display->display_options['filters']['language_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['language_1']['expose']['operator_id'] = 'language_1_op';
  $handler->display->display_options['filters']['language_1']['expose']['label'] = 'Source language';
  $handler->display->display_options['filters']['language_1']['expose']['operator'] = 'language_1_op';
  $handler->display->display_options['filters']['language_1']['expose']['identifier'] = 'language_1';
  /* Filter criterion: Entity translation: Translation exists */
  $handler->display->display_options['filters']['translation_exists']['id'] = 'translation_exists';
  $handler->display->display_options['filters']['translation_exists']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['translation_exists']['field'] = 'translation_exists';
  $handler->display->display_options['filters']['translation_exists']['operator'] = 'not in';
  $handler->display->display_options['filters']['translation_exists']['value'] = array(
    '***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
  );
  $handler->display->display_options['filters']['translation_exists']['group'] = 1;
  $handler->display->display_options['filters']['translation_exists']['exposed'] = TRUE;
  $handler->display->display_options['filters']['translation_exists']['expose']['operator_id'] = 'translation_exists_op';
  $handler->display->display_options['filters']['translation_exists']['expose']['label'] = 'Target language';
  $handler->display->display_options['filters']['translation_exists']['expose']['operator'] = 'translation_exists_op';
  $handler->display->display_options['filters']['translation_exists']['expose']['identifier'] = 'translation_exists';
  $handler->display->display_options['filters']['translation_exists']['expose']['required'] = TRUE;
  $handler->display->display_options['filters']['translation_exists']['entity_type'] = 'taxonomy_term';
  $handler->display->display_options['filters']['translation_exists']['use_filter'] = 1;
  $handler->display->display_options['filters']['translation_exists']['filter'] = 'entity_type';

  /* Display: Not translated */
  $handler = $view->new_display('panel_pane', 'Not translated', 'panel_pane_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Needs update */
  $handler = $view->new_display('panel_pane', 'Needs update', 'panel_pane_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Entity translation: Entity type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['value'] = array(
    'node' => 'node',
  );
  $handler->display->display_options['filters']['entity_type']['group'] = 1;
  $handler->display->display_options['filters']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator_id'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['label'] = 'Entity type';
  $handler->display->display_options['filters']['entity_type']['expose']['operator'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['identifier'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['expose']['required'] = TRUE;
  /* Filter criterion: Entity translation: Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['operator'] = 'not in';
  $handler->display->display_options['filters']['language']['value'] = array(
    'und' => 'und',
  );
  $handler->display->display_options['filters']['language']['group'] = 1;
  /* Filter criterion: Entity translation: Language */
  $handler->display->display_options['filters']['language_1']['id'] = 'language_1';
  $handler->display->display_options['filters']['language_1']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['language_1']['field'] = 'language';
  $handler->display->display_options['filters']['language_1']['group'] = 1;
  $handler->display->display_options['filters']['language_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['language_1']['expose']['operator_id'] = 'language_1_op';
  $handler->display->display_options['filters']['language_1']['expose']['label'] = 'Source language';
  $handler->display->display_options['filters']['language_1']['expose']['operator'] = 'language_1_op';
  $handler->display->display_options['filters']['language_1']['expose']['identifier'] = 'language_1';
  /* Filter criterion: Entity translation: Needs update */
  $handler->display->display_options['filters']['translate']['id'] = 'translate';
  $handler->display->display_options['filters']['translate']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['translate']['field'] = 'translate';
  $handler->display->display_options['filters']['translate']['value'] = '1';
  $handler->display->display_options['filters']['translate']['group'] = 2;
  /* Filter criterion: Entity translation: Translation status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'entity_translation';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';
  $handler->display->display_options['filters']['status']['group'] = 2;
  $handler->display->display_options['inherit_panels_path'] = '1';
  $translatables['ns_translator'] = array(
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
    t('Nothing more to translate!'),
    t('Label'),
    t('Created'),
    t('Language'),
    t('Operations'),
    t('Entity type'),
    t('Source language'),
    t('Target language'),
    t('Not translated'),
    t('View panes'),
    t('Needs update'),
  );
  $export['ns_translator'] = $view;

  return $export;
}

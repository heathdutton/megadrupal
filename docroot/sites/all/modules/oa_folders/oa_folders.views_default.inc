<?php
/**
 * @file
 * oa_folders.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function oa_folders_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'oa_folders';
  $view->description = 'Views for the Open Atrium Folders';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Open Atrium Folders';
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
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Folder icon */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Folder icon';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<i class="glyphicon glyphicon-folder-open icon-folder-close"></i>';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['nothing']['alter']['link_class'] = 'folder';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'reverse_field_oa_folder_parent_node';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = '';
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'oa_date';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'oa_date';
  $handler->display->display_options['fields']['changed']['oa_date'] = '0';
  /* Field: Content: Folder toolbox */
  $handler->display->display_options['fields']['field_folder_toolbox']['id'] = 'field_folder_toolbox';
  $handler->display->display_options['fields']['field_folder_toolbox']['table'] = 'node';
  $handler->display->display_options['fields']['field_folder_toolbox']['field'] = 'field_folder_toolbox';
  $handler->display->display_options['fields']['field_folder_toolbox']['label'] = '';
  $handler->display->display_options['fields']['field_folder_toolbox']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
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
    'oa_folder' => 'oa_folder',
  );

  /* Display: Sub folders */
  $handler = $view->new_display('panel_pane', 'Sub folders', 'subfolder_pane');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Parent folder (field_oa_folder_parent) */
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['id'] = 'field_oa_folder_parent_target_id';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['table'] = 'field_data_field_oa_folder_parent';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['field'] = 'field_oa_folder_parent_target_id';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['validate_options']['types'] = array(
    'oa_folder' => 'oa_folder',
  );
  $handler->display->display_options['arguments']['field_oa_folder_parent_target_id']['validate_options']['access'] = TRUE;
  $handler->display->display_options['pane_category']['name'] = 'Open Atrium Folders';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'field_oa_folder_parent_target_id' => array(
      'type' => 'context',
      'context' => 'entity:node.nid',
      'context_optional' => 1,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Parent folder (field_oa_folder_parent)',
    ),
  );

  /* Display: root folders */
  $handler = $view->new_display('panel_pane', 'root folders', 'root_pane');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Open Atrium Section (oa_section_ref) */
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['id'] = 'oa_section_ref_target_id';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['table'] = 'field_data_oa_section_ref';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['field'] = 'oa_section_ref_target_id';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['oa_section_ref_target_id']['validate_options']['types'] = array(
    'oa_section' => 'oa_section',
  );
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
    'oa_folder' => 'oa_folder',
  );
  /* Filter criterion: Content: Parent folder (field_oa_folder_parent) */
  $handler->display->display_options['filters']['field_oa_folder_parent_target_id']['id'] = 'field_oa_folder_parent_target_id';
  $handler->display->display_options['filters']['field_oa_folder_parent_target_id']['table'] = 'field_data_field_oa_folder_parent';
  $handler->display->display_options['filters']['field_oa_folder_parent_target_id']['field'] = 'field_oa_folder_parent_target_id';
  $handler->display->display_options['filters']['field_oa_folder_parent_target_id']['operator'] = 'empty';
  $handler->display->display_options['pane_category']['name'] = 'Open Atrium Folders';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'oa_section_ref_target_id' => array(
      'type' => 'context',
      'context' => 'entity:node.nid',
      'context_optional' => 1,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Open Atrium Section (oa_section_ref)',
    ),
  );

  /* Display: Folder content */
  $handler = $view->new_display('panel_pane', 'Folder content', 'content_pane');
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['row_class'] = 'folder-file-[fid]';
  $handler->display->display_options['style_options']['columns'] = array(
    'filemime' => 'filemime',
    'uri' => 'uri',
    'filename' => 'filename',
    'filesize' => 'filesize',
    'timestamp' => 'timestamp',
    'fid' => 'fid',
    'nid' => 'nid',
    'field_folder_toolbox' => 'field_folder_toolbox',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'filemime' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uri' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'filename' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'filesize' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'fid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_folder_toolbox' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are currently no files in this folder.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Attachments (field_oa_media:fid) */
  $handler->display->display_options['relationships']['field_oa_media_fid']['id'] = 'field_oa_media_fid';
  $handler->display->display_options['relationships']['field_oa_media_fid']['table'] = 'field_data_field_oa_media';
  $handler->display->display_options['relationships']['field_oa_media_fid']['field'] = 'field_oa_media_fid';
  $handler->display->display_options['relationships']['field_oa_media_fid']['label'] = 'file';
  $handler->display->display_options['relationships']['field_oa_media_fid']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: File: Mime type */
  $handler->display->display_options['fields']['filemime']['id'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filemime']['field'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['relationship'] = 'field_oa_media_fid';
  $handler->display->display_options['fields']['filemime']['label'] = '';
  $handler->display->display_options['fields']['filemime']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['filemime']['link_to_file'] = TRUE;
  $handler->display->display_options['fields']['filemime']['filemime_image'] = TRUE;
  /* Field: File: Path */
  $handler->display->display_options['fields']['uri']['id'] = 'uri';
  $handler->display->display_options['fields']['uri']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uri']['field'] = 'uri';
  $handler->display->display_options['fields']['uri']['relationship'] = 'field_oa_media_fid';
  $handler->display->display_options['fields']['uri']['label'] = '';
  $handler->display->display_options['fields']['uri']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uri']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uri']['file_download_path'] = TRUE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['relationship'] = 'field_oa_media_fid';
  $handler->display->display_options['fields']['filesize']['label'] = '';
  $handler->display->display_options['fields']['filesize']['exclude'] = TRUE;
  $handler->display->display_options['fields']['filesize']['element_label_colon'] = FALSE;
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['relationship'] = 'field_oa_media_fid';
  $handler->display->display_options['fields']['filename']['label'] = '';
  $handler->display->display_options['fields']['filename']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['filename']['alter']['text'] = '<div class="oa-folders-filename">[filename]</div>
<div class="oa-folders-filesize">[filesize]</div>';
  $handler->display->display_options['fields']['filename']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['filename']['alter']['path'] = '[uri]';
  $handler->display->display_options['fields']['filename']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['filename']['link_to_file'] = FALSE;
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['relationship'] = 'field_oa_media_fid';
  $handler->display->display_options['fields']['timestamp']['label'] = '';
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'oa_date';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'oa_date';
  $handler->display->display_options['fields']['timestamp']['oa_date'] = '0';
  /* Field: File: File ID */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['relationship'] = 'field_oa_media_fid';
  $handler->display->display_options['fields']['fid']['label'] = '';
  $handler->display->display_options['fields']['fid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['fid']['element_label_colon'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Folder toolbox */
  $handler->display->display_options['fields']['field_folder_toolbox']['id'] = 'field_folder_toolbox';
  $handler->display->display_options['fields']['field_folder_toolbox']['table'] = 'node';
  $handler->display->display_options['fields']['field_folder_toolbox']['field'] = 'field_folder_toolbox';
  $handler->display->display_options['fields']['field_folder_toolbox']['label'] = '';
  $handler->display->display_options['fields']['field_folder_toolbox']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: File: Name */
  $handler->display->display_options['sorts']['filename']['id'] = 'filename';
  $handler->display->display_options['sorts']['filename']['table'] = 'file_managed';
  $handler->display->display_options['sorts']['filename']['field'] = 'filename';
  $handler->display->display_options['sorts']['filename']['relationship'] = 'field_oa_media_fid';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'oa_folder' => 'oa_folder',
  );
  $handler->display->display_options['arguments']['nid']['validate_options']['access'] = TRUE;
  $handler->display->display_options['pane_category']['name'] = 'Open Atrium Folders';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'nid' => array(
      'type' => 'context',
      'context' => 'entity:node.nid',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Nid',
    ),
  );
  $export['oa_folders'] = $view;

  return $export;
}

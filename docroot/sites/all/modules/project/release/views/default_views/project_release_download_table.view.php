<?php

$view = new view();
$view->name = 'project_release_download_table';
$view->description = 'Provides tables for the latest release from the recommended branches, the latest releases from any supported branches, and devel';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Project release download table';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'path' => 'path',
  'field_release_version' => 'field_release_version',
  'uri' => 'uri',
  'uri_1' => 'uri_1',
  'filesize' => 'filesize',
  'filesize_1' => 'filesize_1',
  'extension' => 'extension',
  'extension_1' => 'extension',
  'timestamp' => 'timestamp',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'path' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_release_version' => array(
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
  'uri_1' => array(
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
  'filesize_1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'extension' => array(
    'align' => '',
    'separator' => ' | ',
    'empty_column' => 0,
  ),
  'extension_1' => array(
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
);
$handler->display->display_options['style_options']['override'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Title';
$handler->display->display_options['header']['area']['content'] = '<h4>Recommended releases</h4>';
$handler->display->display_options['header']['area']['format'] = '1';
/* Relationship: Content: Taxonomy terms on node */
$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
  'vocabulary_1' => 0,
  'vocabulary_5' => 0,
  'vocabulary_38' => 0,
  'vocabulary_54' => 0,
  'vocabulary_56' => 0,
  'vocabulary_6' => 0,
  'vocabulary_4' => 0,
  'vocabulary_44' => 0,
  'vocabulary_46' => 0,
  'vocabulary_3' => 0,
  'vocabulary_48' => 0,
  'vocabulary_50' => 0,
  'vocabulary_52' => 0,
  'vocabulary_58' => 0,
  'vocabulary_7' => 0,
  'vocabulary_2' => 0,
  'vocabulary_31' => 0,
  'vocabulary_32' => 0,
  'vocabulary_33' => 0,
  'vocabulary_34' => 0,
  'vocabulary_9' => 0,
);
/* Relationship: Content: Release files (field_release_files) */
$handler->display->display_options['relationships']['field_release_files_value']['id'] = 'field_release_files_value';
$handler->display->display_options['relationships']['field_release_files_value']['table'] = 'field_data_field_release_files';
$handler->display->display_options['relationships']['field_release_files_value']['field'] = 'field_release_files_value';
$handler->display->display_options['relationships']['field_release_files_value']['label'] = 'field collection item from field_release_files 1';
$handler->display->display_options['relationships']['field_release_files_value']['delta'] = '0';
/* Relationship: Field collection item: Release file (field_release_file:fid) */
$handler->display->display_options['relationships']['field_release_file_fid']['id'] = 'field_release_file_fid';
$handler->display->display_options['relationships']['field_release_file_fid']['table'] = 'field_data_field_release_file';
$handler->display->display_options['relationships']['field_release_file_fid']['field'] = 'field_release_file_fid';
$handler->display->display_options['relationships']['field_release_file_fid']['relationship'] = 'field_release_files_value';
$handler->display->display_options['relationships']['field_release_file_fid']['label'] = 'file from field_release_file 1';
/* Relationship: Content: Release files (field_release_files) */
$handler->display->display_options['relationships']['field_release_files_value_1']['id'] = 'field_release_files_value_1';
$handler->display->display_options['relationships']['field_release_files_value_1']['table'] = 'field_data_field_release_files';
$handler->display->display_options['relationships']['field_release_files_value_1']['field'] = 'field_release_files_value';
$handler->display->display_options['relationships']['field_release_files_value_1']['label'] = 'field collection item from field_release_files 2';
$handler->display->display_options['relationships']['field_release_files_value_1']['delta'] = '1';
/* Relationship: Field collection item: Release file (field_release_file:fid) */
$handler->display->display_options['relationships']['field_release_file_fid_1']['id'] = 'field_release_file_fid_1';
$handler->display->display_options['relationships']['field_release_file_fid_1']['table'] = 'field_data_field_release_file';
$handler->display->display_options['relationships']['field_release_file_fid_1']['field'] = 'field_release_file_fid';
$handler->display->display_options['relationships']['field_release_file_fid_1']['relationship'] = 'field_release_files_value_1';
$handler->display->display_options['relationships']['field_release_file_fid_1']['label'] = 'file from field_release_file 2';
/* Relationship: Project release: Supported releases */
$handler->display->display_options['relationships']['supported_releases_rel']['id'] = 'supported_releases_rel';
$handler->display->display_options['relationships']['supported_releases_rel']['table'] = 'field_data_field_release_project';
$handler->display->display_options['relationships']['supported_releases_rel']['field'] = 'supported_releases_rel';
$handler->display->display_options['relationships']['supported_releases_rel']['required'] = TRUE;
/* Field: Content: Path */
$handler->display->display_options['fields']['path']['id'] = 'path';
$handler->display->display_options['fields']['path']['table'] = 'node';
$handler->display->display_options['fields']['path']['field'] = 'path';
$handler->display->display_options['fields']['path']['exclude'] = TRUE;
$handler->display->display_options['fields']['path']['absolute'] = TRUE;
/* Field: Content: Version */
$handler->display->display_options['fields']['field_release_version']['id'] = 'field_release_version';
$handler->display->display_options['fields']['field_release_version']['table'] = 'field_data_field_release_version';
$handler->display->display_options['fields']['field_release_version']['field'] = 'field_release_version';
$handler->display->display_options['fields']['field_release_version']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['field_release_version']['alter']['path'] = '[path]';
$handler->display->display_options['fields']['field_release_version']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_release_version']['type'] = 'text_plain';
/* Field: File: Path */
$handler->display->display_options['fields']['uri']['id'] = 'uri';
$handler->display->display_options['fields']['uri']['table'] = 'file_managed';
$handler->display->display_options['fields']['uri']['field'] = 'uri';
$handler->display->display_options['fields']['uri']['relationship'] = 'field_release_file_fid';
$handler->display->display_options['fields']['uri']['exclude'] = TRUE;
$handler->display->display_options['fields']['uri']['file_download_path'] = TRUE;
/* Field: File: Path */
$handler->display->display_options['fields']['uri_1']['id'] = 'uri_1';
$handler->display->display_options['fields']['uri_1']['table'] = 'file_managed';
$handler->display->display_options['fields']['uri_1']['field'] = 'uri';
$handler->display->display_options['fields']['uri_1']['relationship'] = 'field_release_file_fid_1';
$handler->display->display_options['fields']['uri_1']['label'] = '';
$handler->display->display_options['fields']['uri_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['uri_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['uri_1']['file_download_path'] = TRUE;
/* Field: File: Size */
$handler->display->display_options['fields']['filesize']['id'] = 'filesize';
$handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
$handler->display->display_options['fields']['filesize']['field'] = 'filesize';
$handler->display->display_options['fields']['filesize']['relationship'] = 'field_release_file_fid';
$handler->display->display_options['fields']['filesize']['label'] = '';
$handler->display->display_options['fields']['filesize']['exclude'] = TRUE;
$handler->display->display_options['fields']['filesize']['element_label_colon'] = FALSE;
/* Field: File: Size */
$handler->display->display_options['fields']['filesize_1']['id'] = 'filesize_1';
$handler->display->display_options['fields']['filesize_1']['table'] = 'file_managed';
$handler->display->display_options['fields']['filesize_1']['field'] = 'filesize';
$handler->display->display_options['fields']['filesize_1']['relationship'] = 'field_release_file_fid_1';
$handler->display->display_options['fields']['filesize_1']['label'] = '';
$handler->display->display_options['fields']['filesize_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['filesize_1']['element_label_colon'] = FALSE;
/* Field: File: Extension */
$handler->display->display_options['fields']['extension']['id'] = 'extension';
$handler->display->display_options['fields']['extension']['table'] = 'file_managed';
$handler->display->display_options['fields']['extension']['field'] = 'extension';
$handler->display->display_options['fields']['extension']['relationship'] = 'field_release_file_fid';
$handler->display->display_options['fields']['extension']['label'] = 'Download';
$handler->display->display_options['fields']['extension']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['extension']['alter']['text'] = '[extension] <span class="filesize">([filesize])</span>';
$handler->display->display_options['fields']['extension']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['extension']['alter']['path'] = '[uri]';
$handler->display->display_options['fields']['extension']['extension_detect_tar'] = TRUE;
/* Field: File: Extension */
$handler->display->display_options['fields']['extension_1']['id'] = 'extension_1';
$handler->display->display_options['fields']['extension_1']['table'] = 'file_managed';
$handler->display->display_options['fields']['extension_1']['field'] = 'extension';
$handler->display->display_options['fields']['extension_1']['relationship'] = 'field_release_file_fid_1';
$handler->display->display_options['fields']['extension_1']['label'] = '';
$handler->display->display_options['fields']['extension_1']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['extension_1']['alter']['text'] = '[extension_1] <span class="filesize">([filesize_1])</span>';
$handler->display->display_options['fields']['extension_1']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['extension_1']['alter']['path'] = '[uri_1]';
$handler->display->display_options['fields']['extension_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['extension_1']['extension_detect_tar'] = TRUE;
/* Field: File: Upload date */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['relationship'] = 'field_release_file_fid';
$handler->display->display_options['fields']['timestamp']['label'] = 'Date';
$handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
$handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'Y-M-d';
/* Sort criterion: Taxonomy term: Weight */
$handler->display->display_options['sorts']['weight']['id'] = 'weight';
$handler->display->display_options['sorts']['weight']['table'] = 'taxonomy_term_data';
$handler->display->display_options['sorts']['weight']['field'] = 'weight';
$handler->display->display_options['sorts']['weight']['relationship'] = 'term_node_tid';
/* Sort criterion: Content: Version major (field_release_version_major) */
$handler->display->display_options['sorts']['field_release_version_major_value']['id'] = 'field_release_version_major_value';
$handler->display->display_options['sorts']['field_release_version_major_value']['table'] = 'field_data_field_release_version_major';
$handler->display->display_options['sorts']['field_release_version_major_value']['field'] = 'field_release_version_major_value';
$handler->display->display_options['sorts']['field_release_version_major_value']['order'] = 'DESC';
/* Sort criterion: Content: Version minor (field_release_version_minor) */
$handler->display->display_options['sorts']['field_release_version_minor_value']['id'] = 'field_release_version_minor_value';
$handler->display->display_options['sorts']['field_release_version_minor_value']['table'] = 'field_data_field_release_version_minor';
$handler->display->display_options['sorts']['field_release_version_minor_value']['field'] = 'field_release_version_minor_value';
$handler->display->display_options['sorts']['field_release_version_minor_value']['order'] = 'DESC';
/* Sort criterion: Content: Version patch (field_release_version_patch) */
$handler->display->display_options['sorts']['field_release_version_patch_value']['id'] = 'field_release_version_patch_value';
$handler->display->display_options['sorts']['field_release_version_patch_value']['table'] = 'field_data_field_release_version_patch';
$handler->display->display_options['sorts']['field_release_version_patch_value']['field'] = 'field_release_version_patch_value';
$handler->display->display_options['sorts']['field_release_version_patch_value']['order'] = 'DESC';
/* Sort criterion: Content: Version extra weight (field_release_version_ext_weight) */
$handler->display->display_options['sorts']['field_release_version_ext_weight_value']['id'] = 'field_release_version_ext_weight_value';
$handler->display->display_options['sorts']['field_release_version_ext_weight_value']['table'] = 'field_data_field_release_version_ext_weight';
$handler->display->display_options['sorts']['field_release_version_ext_weight_value']['field'] = 'field_release_version_ext_weight_value';
$handler->display->display_options['sorts']['field_release_version_ext_weight_value']['order'] = 'DESC';
/* Sort criterion: Content: Version extra (field_release_version_extra) */
$handler->display->display_options['sorts']['field_release_version_extra_value']['id'] = 'field_release_version_extra_value';
$handler->display->display_options['sorts']['field_release_version_extra_value']['table'] = 'field_data_field_release_version_extra';
$handler->display->display_options['sorts']['field_release_version_extra_value']['field'] = 'field_release_version_extra_value';
$handler->display->display_options['sorts']['field_release_version_extra_value']['order'] = 'DESC';
/* Sort criterion: Content: Version extra delta (field_release_version_ext_delta) */
$handler->display->display_options['sorts']['field_release_version_ext_delta_value']['id'] = 'field_release_version_ext_delta_value';
$handler->display->display_options['sorts']['field_release_version_ext_delta_value']['table'] = 'field_data_field_release_version_ext_delta';
$handler->display->display_options['sorts']['field_release_version_ext_delta_value']['field'] = 'field_release_version_ext_delta_value';
$handler->display->display_options['sorts']['field_release_version_ext_delta_value']['order'] = 'DESC';
/* Contextual filter: Content: Project (field_release_project) */
$handler->display->display_options['arguments']['field_release_project_target_id']['id'] = 'field_release_project_target_id';
$handler->display->display_options['arguments']['field_release_project_target_id']['table'] = 'field_data_field_release_project';
$handler->display->display_options['arguments']['field_release_project_target_id']['field'] = 'field_release_project_target_id';
$handler->display->display_options['arguments']['field_release_project_target_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['field_release_project_target_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['field_release_project_target_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['field_release_project_target_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['field_release_project_target_id']['summary_options']['items_per_page'] = '25';
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type']['id'] = 'project_type';
$handler->display->display_options['filters']['project_type']['table'] = 'node';
$handler->display->display_options['filters']['project_type']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type']['value'] = 'project_release';
$handler->display->display_options['filters']['project_type']['group'] = 1;
/* Filter criterion: Taxonomy term: Recommended (field_release_recommended) */
$handler->display->display_options['filters']['field_release_recommended_value']['id'] = 'field_release_recommended_value';
$handler->display->display_options['filters']['field_release_recommended_value']['table'] = 'field_data_field_release_recommended';
$handler->display->display_options['filters']['field_release_recommended_value']['field'] = 'field_release_recommended_value';
$handler->display->display_options['filters']['field_release_recommended_value']['relationship'] = 'term_node_tid';
$handler->display->display_options['filters']['field_release_recommended_value']['value'] = array(
  1 => '1',
);
$handler->display->display_options['filters']['field_release_recommended_value']['group'] = 1;
/* Filter criterion: Content: Update status (field_release_update_status) */
$handler->display->display_options['filters']['field_release_update_status_value']['id'] = 'field_release_update_status_value';
$handler->display->display_options['filters']['field_release_update_status_value']['table'] = 'field_data_field_release_update_status';
$handler->display->display_options['filters']['field_release_update_status_value']['field'] = 'field_release_update_status_value';
$handler->display->display_options['filters']['field_release_update_status_value']['operator'] = '<=';
$handler->display->display_options['filters']['field_release_update_status_value']['value']['value'] = '1';
$handler->display->display_options['filters']['field_release_update_status_value']['group'] = 1;
/* Filter criterion: Content: Release type (field_release_build_type) */
$handler->display->display_options['filters']['field_release_build_type_value']['id'] = 'field_release_build_type_value';
$handler->display->display_options['filters']['field_release_build_type_value']['table'] = 'field_data_field_release_build_type';
$handler->display->display_options['filters']['field_release_build_type_value']['field'] = 'field_release_build_type_value';
$handler->display->display_options['filters']['field_release_build_type_value']['value'] = array(
  'static' => 'static',
);
$handler->display->display_options['filters']['field_release_build_type_value']['group'] = 1;
/* Filter criterion: Project release: Recommended branch */
$handler->display->display_options['filters']['recommended']['id'] = 'recommended';
$handler->display->display_options['filters']['recommended']['table'] = 'project_release_supported_versions';
$handler->display->display_options['filters']['recommended']['field'] = 'recommended';
$handler->display->display_options['filters']['recommended']['relationship'] = 'supported_releases_rel';
$handler->display->display_options['filters']['recommended']['value'] = '1';
/* Filter criterion: Project release: Latest or recommended release */
$handler->display->display_options['filters']['release_type']['id'] = 'release_type';
$handler->display->display_options['filters']['release_type']['table'] = 'project_release_supported_versions';
$handler->display->display_options['filters']['release_type']['field'] = 'release_type';
$handler->display->display_options['filters']['release_type']['relationship'] = 'supported_releases_rel';
$handler->display->display_options['filters']['release_type']['value'] = 'recommended';

/* Display: Recommended releases */
$handler = $view->new_display('block', 'Recommended releases', 'recommended');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

/* Display: Releases missing security updates */
$handler = $view->new_display('attachment', 'Releases missing security updates', 'security');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Title';
$handler->display->display_options['header']['area']['content'] = '<h4>Releases missing security updates</h4>

<div class="warning">There are security updates available for code included in these releases. Review release notes before downloading.</div>
';
$handler->display->display_options['header']['area']['format'] = '3';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type']['id'] = 'project_type';
$handler->display->display_options['filters']['project_type']['table'] = 'node';
$handler->display->display_options['filters']['project_type']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type']['value'] = 'project_release';
$handler->display->display_options['filters']['project_type']['group'] = 1;
/* Filter criterion: Taxonomy term: Recommended (field_release_recommended) */
$handler->display->display_options['filters']['field_release_recommended_value']['id'] = 'field_release_recommended_value';
$handler->display->display_options['filters']['field_release_recommended_value']['table'] = 'field_data_field_release_recommended';
$handler->display->display_options['filters']['field_release_recommended_value']['field'] = 'field_release_recommended_value';
$handler->display->display_options['filters']['field_release_recommended_value']['relationship'] = 'term_node_tid';
$handler->display->display_options['filters']['field_release_recommended_value']['value'] = array(
  1 => '1',
);
$handler->display->display_options['filters']['field_release_recommended_value']['group'] = 1;
/* Filter criterion: Content: Update status (field_release_update_status) */
$handler->display->display_options['filters']['field_release_update_status_value']['id'] = 'field_release_update_status_value';
$handler->display->display_options['filters']['field_release_update_status_value']['table'] = 'field_data_field_release_update_status';
$handler->display->display_options['filters']['field_release_update_status_value']['field'] = 'field_release_update_status_value';
$handler->display->display_options['filters']['field_release_update_status_value']['operator'] = '>';
$handler->display->display_options['filters']['field_release_update_status_value']['value']['value'] = '1';
$handler->display->display_options['filters']['field_release_update_status_value']['group'] = 1;
/* Filter criterion: Content: Release type (field_release_build_type) */
$handler->display->display_options['filters']['field_release_build_type_value']['id'] = 'field_release_build_type_value';
$handler->display->display_options['filters']['field_release_build_type_value']['table'] = 'field_data_field_release_build_type';
$handler->display->display_options['filters']['field_release_build_type_value']['field'] = 'field_release_build_type_value';
$handler->display->display_options['filters']['field_release_build_type_value']['value'] = array(
  'static' => 'static',
);
$handler->display->display_options['filters']['field_release_build_type_value']['group'] = 1;
/* Filter criterion: Project release: Latest or recommended release */
$handler->display->display_options['filters']['release_type']['id'] = 'release_type';
$handler->display->display_options['filters']['release_type']['table'] = 'project_release_supported_versions';
$handler->display->display_options['filters']['release_type']['field'] = 'release_type';
$handler->display->display_options['filters']['release_type']['relationship'] = 'supported_releases_rel';
$handler->display->display_options['filters']['release_type']['value'] = 'recommended';
$handler->display->display_options['displays'] = array(
  'recommended' => 'recommended',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';

/* Display: Other releases */
$handler = $view->new_display('attachment', 'Other releases', 'other');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Title';
$handler->display->display_options['header']['area']['content'] = '<h4>Other releases</h4>';
$handler->display->display_options['header']['area']['format'] = '1';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type']['id'] = 'project_type';
$handler->display->display_options['filters']['project_type']['table'] = 'node';
$handler->display->display_options['filters']['project_type']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type']['value'] = 'project_release';
$handler->display->display_options['filters']['project_type']['group'] = 1;
/* Filter criterion: Taxonomy term: Recommended (field_release_recommended) */
$handler->display->display_options['filters']['field_release_recommended_value']['id'] = 'field_release_recommended_value';
$handler->display->display_options['filters']['field_release_recommended_value']['table'] = 'field_data_field_release_recommended';
$handler->display->display_options['filters']['field_release_recommended_value']['field'] = 'field_release_recommended_value';
$handler->display->display_options['filters']['field_release_recommended_value']['relationship'] = 'term_node_tid';
$handler->display->display_options['filters']['field_release_recommended_value']['value'] = array(
  1 => '1',
);
$handler->display->display_options['filters']['field_release_recommended_value']['group'] = 1;
/* Filter criterion: Content: Update status (field_release_update_status) */
$handler->display->display_options['filters']['field_release_update_status_value']['id'] = 'field_release_update_status_value';
$handler->display->display_options['filters']['field_release_update_status_value']['table'] = 'field_data_field_release_update_status';
$handler->display->display_options['filters']['field_release_update_status_value']['field'] = 'field_release_update_status_value';
$handler->display->display_options['filters']['field_release_update_status_value']['operator'] = '<=';
$handler->display->display_options['filters']['field_release_update_status_value']['value']['value'] = '1';
$handler->display->display_options['filters']['field_release_update_status_value']['group'] = 1;
/* Filter criterion: Content: Release type (field_release_build_type) */
$handler->display->display_options['filters']['field_release_build_type_value']['id'] = 'field_release_build_type_value';
$handler->display->display_options['filters']['field_release_build_type_value']['table'] = 'field_data_field_release_build_type';
$handler->display->display_options['filters']['field_release_build_type_value']['field'] = 'field_release_build_type_value';
$handler->display->display_options['filters']['field_release_build_type_value']['value'] = array(
  'static' => 'static',
);
$handler->display->display_options['filters']['field_release_build_type_value']['group'] = 1;
/* Filter criterion: Project release: Recommended branch */
$handler->display->display_options['filters']['recommended']['id'] = 'recommended';
$handler->display->display_options['filters']['recommended']['table'] = 'project_release_supported_versions';
$handler->display->display_options['filters']['recommended']['field'] = 'recommended';
$handler->display->display_options['filters']['recommended']['relationship'] = 'supported_releases_rel';
$handler->display->display_options['filters']['recommended']['value'] = '0';
/* Filter criterion: Project release: Latest or recommended release */
$handler->display->display_options['filters']['release_type']['id'] = 'release_type';
$handler->display->display_options['filters']['release_type']['table'] = 'project_release_supported_versions';
$handler->display->display_options['filters']['release_type']['field'] = 'release_type';
$handler->display->display_options['filters']['release_type']['relationship'] = 'supported_releases_rel';
$handler->display->display_options['filters']['release_type']['value'] = 'recommended';
$handler->display->display_options['displays'] = array(
  'recommended' => 'recommended',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';

/* Display: Development releases */
$handler = $view->new_display('attachment', 'Development releases', 'development');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Title';
$handler->display->display_options['header']['area']['content'] = '<h4>Development releases</h4>';
$handler->display->display_options['header']['area']['format'] = '1';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type']['id'] = 'project_type';
$handler->display->display_options['filters']['project_type']['table'] = 'node';
$handler->display->display_options['filters']['project_type']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type']['value'] = 'project_release';
$handler->display->display_options['filters']['project_type']['group'] = 1;
/* Filter criterion: Taxonomy term: Recommended (field_release_recommended) */
$handler->display->display_options['filters']['field_release_recommended_value']['id'] = 'field_release_recommended_value';
$handler->display->display_options['filters']['field_release_recommended_value']['table'] = 'field_data_field_release_recommended';
$handler->display->display_options['filters']['field_release_recommended_value']['field'] = 'field_release_recommended_value';
$handler->display->display_options['filters']['field_release_recommended_value']['relationship'] = 'term_node_tid';
$handler->display->display_options['filters']['field_release_recommended_value']['value'] = array(
  1 => '1',
);
$handler->display->display_options['filters']['field_release_recommended_value']['group'] = 1;
/* Filter criterion: Content: Release type (field_release_build_type) */
$handler->display->display_options['filters']['field_release_build_type_value']['id'] = 'field_release_build_type_value';
$handler->display->display_options['filters']['field_release_build_type_value']['table'] = 'field_data_field_release_build_type';
$handler->display->display_options['filters']['field_release_build_type_value']['field'] = 'field_release_build_type_value';
$handler->display->display_options['filters']['field_release_build_type_value']['value'] = array(
  'dynamic' => 'dynamic',
);
$handler->display->display_options['filters']['field_release_build_type_value']['group'] = 1;
/* Filter criterion: Project release: Show snapshot */
$handler->display->display_options['filters']['snapshot']['id'] = 'snapshot';
$handler->display->display_options['filters']['snapshot']['table'] = 'project_release_supported_versions';
$handler->display->display_options['filters']['snapshot']['field'] = 'snapshot';
$handler->display->display_options['filters']['snapshot']['relationship'] = 'supported_releases_rel';
$handler->display->display_options['filters']['snapshot']['value'] = '1';
$handler->display->display_options['filters']['snapshot']['group'] = 1;
$handler->display->display_options['displays'] = array(
  'recommended' => 'recommended',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';

<?php
/**
 * @file
 * bassets_server_image_admin_view.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function bassets_server_image_admin_view_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'content';
  $view->description = 'Replacement of admin/content/file for unprevilegded users';
  $view->tag = 'default';
  $view->base_table = 'file_managed';
  $view->human_name = 'Images';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Images';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'Use image image admin replacement';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'filename' => 'filename',
    'nothing_1' => 'nothing_1',
    'filesize' => 'filesize',
    'name' => 'name',
    'timestamp' => 'timestamp',
    'fid' => 'fid',
    'nothing' => 'nothing',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'filename' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'filesize' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'fid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Relationship: File: User who uploaded */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'file_managed';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: File: File ID */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['label'] = '';
  $handler->display->display_options['fields']['fid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['fid']['element_label_colon'] = FALSE;
  /* Field: File: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'file_managed';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['machine_name'] = 1;
  /* Field: File: Mime type */
  $handler->display->display_options['fields']['filemime']['id'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filemime']['field'] = 'filemime';
  $handler->display->display_options['fields']['filemime']['exclude'] = TRUE;
  $handler->display->display_options['fields']['filemime']['filemime_image'] = TRUE;
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = '';
  $handler->display->display_options['fields']['filename']['exclude'] = TRUE;
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['filename']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Name';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<span class="file">
  [filemime]<a href="file/[fid]">[filename]</a>
</span>';
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<ul class="links inline"><li class="edit first"><a href="file/[fid]/edit?destination=admin/content/file">Edit</a></li>
<li class="delete last"><a href="file/[fid]/delete?destination=admin/content/file">Delete</a></li>
</ul>';
  /* Sort criterion: File: Upload date */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'image' => 'image',
  );

  /* Display: Page Image Editor Overview */
  $handler = $view->new_display('page', 'Page Image Editor Overview', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_asset_lock' => 'field_asset_lock',
    'fid' => 'fid',
    'type' => 'type',
    'filemime' => 'view',
    'view' => 'view',
    'filesize' => 'filesize',
    'field_printable_measure' => 'field_printable_measure',
    'field_computed_height' => 'field_computed_height',
    'field_computed_width' => 'field_computed_width',
    'name' => 'name',
    'timestamp' => 'timestamp',
    'edit' => 'edit',
    'delete' => 'delete',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'field_asset_lock' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'fid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'filemime' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view' => array(
      'align' => '',
      'separator' => ' ',
      'empty_column' => 1,
    ),
    'filesize' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_printable_measure' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_computed_height' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_computed_width' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 1,
    ),
    'delete' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: File */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'file_managed';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_archive_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'scheme' => 'public',
        'temporary' => 1,
      ),
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_image_aspect_ratio' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_image_insert_collection' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_image_printable_measure' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_image_update_collection' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_colorspace' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_copyright' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_datetime' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_description' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_distancerange' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_meteringmode' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_orientation' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_photographer' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_madr_image_metadata_photometric' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          'image::field_file_image_alt_text' => 'image::field_file_image_alt_text',
          'image::field_file_image_title_text' => 'image::field_file_image_title_text',
          'image::field_asset_caption' => 'image::field_asset_caption',
          'image::field_asset_copyright' => 'image::field_asset_copyright',
          'image::field_asset_description' => 'image::field_asset_description',
          'image::field_asset_lock' => 'image::field_asset_lock',
          'image::field_asset_photographer' => 'image::field_asset_photographer',
          'image::field_asset_source' => 'image::field_asset_source',
          'image::field_image_iscolor' => 'image::field_image_iscolor',
          'image::field_image_motive_category' => 'image::field_image_motive_category',
          'image::field_image_motive_type' => 'image::field_image_motive_type',
          'image::field_image_tags' => 'image::field_image_tags',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: File: Lock from further usage */
  $handler->display->display_options['fields']['field_asset_lock']['id'] = 'field_asset_lock';
  $handler->display->display_options['fields']['field_asset_lock']['table'] = 'field_data_field_asset_lock';
  $handler->display->display_options['fields']['field_asset_lock']['field'] = 'field_asset_lock';
  $handler->display->display_options['fields']['field_asset_lock']['element_class'] = 'warning';
  $handler->display->display_options['fields']['field_asset_lock']['delta_offset'] = '0';
  /* Field: File: File ID */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['label'] = '';
  $handler->display->display_options['fields']['fid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['fid']['element_label_colon'] = FALSE;
  /* Field: File: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'file_managed';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['machine_name'] = 1;
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['label'] = '';
  $handler->display->display_options['fields']['filename']['exclude'] = TRUE;
  $handler->display->display_options['fields']['filename']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['filename']['link_to_file'] = FALSE;
  /* Field: File: Link */
  $handler->display->display_options['fields']['link']['id'] = 'link';
  $handler->display->display_options['fields']['link']['table'] = 'file_managed';
  $handler->display->display_options['fields']['link']['field'] = 'link';
  $handler->display->display_options['fields']['link']['label'] = 'Name';
  $handler->display->display_options['fields']['link']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['link']['alter']['text'] = '[filename]';
  $handler->display->display_options['fields']['link']['element_label_colon'] = FALSE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  /* Field: File: Height */
  $handler->display->display_options['fields']['field_computed_height']['id'] = 'field_computed_height';
  $handler->display->display_options['fields']['field_computed_height']['table'] = 'field_data_field_computed_height';
  $handler->display->display_options['fields']['field_computed_height']['field'] = 'field_computed_height';
  $handler->display->display_options['fields']['field_computed_height']['label'] = '';
  $handler->display->display_options['fields']['field_computed_height']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_computed_height']['element_label_colon'] = FALSE;
  /* Field: File: Width */
  $handler->display->display_options['fields']['field_computed_width']['id'] = 'field_computed_width';
  $handler->display->display_options['fields']['field_computed_width']['table'] = 'field_data_field_computed_width';
  $handler->display->display_options['fields']['field_computed_width']['field'] = 'field_computed_width';
  $handler->display->display_options['fields']['field_computed_width']['label'] = 'Dimensions';
  $handler->display->display_options['fields']['field_computed_width']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_computed_width']['alter']['text'] = '<span title="[field_printable_measure]">[field_computed_width] x [field_computed_height]</span>';
  $handler->display->display_options['fields']['field_computed_width']['element_label_colon'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Uploaded by';
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Last edit';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'compact';
  /* Field: File: Edit link */
  $handler->display->display_options['fields']['edit']['id'] = 'edit';
  $handler->display->display_options['fields']['edit']['table'] = 'file_managed';
  $handler->display->display_options['fields']['edit']['field'] = 'edit';
  $handler->display->display_options['fields']['edit']['label'] = '';
  $handler->display->display_options['fields']['edit']['element_label_colon'] = FALSE;
  /* Field: File: Delete link */
  $handler->display->display_options['fields']['delete']['id'] = 'delete';
  $handler->display->display_options['fields']['delete']['table'] = 'file_managed';
  $handler->display->display_options['fields']['delete']['field'] = 'delete';
  $handler->display->display_options['fields']['delete']['label'] = '';
  $handler->display->display_options['fields']['delete']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: File: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_managed';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'image' => 'image',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: File: Name */
  $handler->display->display_options['filters']['filename']['id'] = 'filename';
  $handler->display->display_options['filters']['filename']['table'] = 'file_managed';
  $handler->display->display_options['filters']['filename']['field'] = 'filename';
  $handler->display->display_options['filters']['filename']['operator'] = 'contains';
  $handler->display->display_options['filters']['filename']['group'] = 1;
  $handler->display->display_options['filters']['filename']['exposed'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator_id'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['label'] = 'File Name';
  $handler->display->display_options['filters']['filename']['expose']['operator'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['identifier'] = 'filename';
  $handler->display->display_options['filters']['filename']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['filename']['group_info']['label'] = 'Name';
  $handler->display->display_options['filters']['filename']['group_info']['identifier'] = 'filename';
  $handler->display->display_options['filters']['filename']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['filename']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  /* Filter criterion: File: Aspect ratio (field_file_image_aspect_ratio) */
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['id'] = 'field_file_image_aspect_ratio_value';
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['table'] = 'field_data_field_file_image_aspect_ratio';
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['field'] = 'field_file_image_aspect_ratio_value';
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['operator'] = 'and';
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['group'] = 1;
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['expose']['operator_id'] = 'field_file_image_aspect_ratio_value_op';
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['expose']['label'] = 'Aspect ratio is';
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['expose']['operator'] = 'field_file_image_aspect_ratio_value_op';
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['expose']['identifier'] = 'field_file_image_aspect_ratio_value';
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_file_image_aspect_ratio_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: File: Color image? (field_image_iscolor) */
  $handler->display->display_options['filters']['field_image_iscolor_value']['id'] = 'field_image_iscolor_value';
  $handler->display->display_options['filters']['field_image_iscolor_value']['table'] = 'field_data_field_image_iscolor';
  $handler->display->display_options['filters']['field_image_iscolor_value']['field'] = 'field_image_iscolor_value';
  $handler->display->display_options['filters']['field_image_iscolor_value']['operator'] = 'and';
  $handler->display->display_options['filters']['field_image_iscolor_value']['group'] = 1;
  $handler->display->display_options['filters']['field_image_iscolor_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_image_iscolor_value']['expose']['operator_id'] = 'field_image_iscolor_value_op';
  $handler->display->display_options['filters']['field_image_iscolor_value']['expose']['label'] = 'Color image';
  $handler->display->display_options['filters']['field_image_iscolor_value']['expose']['operator'] = 'field_image_iscolor_value_op';
  $handler->display->display_options['filters']['field_image_iscolor_value']['expose']['identifier'] = 'field_image_iscolor_value';
  $handler->display->display_options['filters']['field_image_iscolor_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_image_iscolor_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_image_iscolor_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: File: Photographer (field_asset_photographer) */
  $handler->display->display_options['filters']['field_asset_photographer_value']['id'] = 'field_asset_photographer_value';
  $handler->display->display_options['filters']['field_asset_photographer_value']['table'] = 'field_data_field_asset_photographer';
  $handler->display->display_options['filters']['field_asset_photographer_value']['field'] = 'field_asset_photographer_value';
  $handler->display->display_options['filters']['field_asset_photographer_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_asset_photographer_value']['group'] = 1;
  $handler->display->display_options['filters']['field_asset_photographer_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_asset_photographer_value']['expose']['operator_id'] = 'field_asset_photographer_value_op';
  $handler->display->display_options['filters']['field_asset_photographer_value']['expose']['label'] = 'Photographer';
  $handler->display->display_options['filters']['field_asset_photographer_value']['expose']['operator'] = 'field_asset_photographer_value_op';
  $handler->display->display_options['filters']['field_asset_photographer_value']['expose']['identifier'] = 'field_asset_photographer_value';
  $handler->display->display_options['filters']['field_asset_photographer_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: File: Copyright notice (field_asset_copyright) */
  $handler->display->display_options['filters']['field_asset_copyright_value']['id'] = 'field_asset_copyright_value';
  $handler->display->display_options['filters']['field_asset_copyright_value']['table'] = 'field_data_field_asset_copyright';
  $handler->display->display_options['filters']['field_asset_copyright_value']['field'] = 'field_asset_copyright_value';
  $handler->display->display_options['filters']['field_asset_copyright_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_asset_copyright_value']['group'] = 1;
  $handler->display->display_options['filters']['field_asset_copyright_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_asset_copyright_value']['expose']['operator_id'] = 'field_asset_copyright_value_op';
  $handler->display->display_options['filters']['field_asset_copyright_value']['expose']['label'] = 'Copyright notice';
  $handler->display->display_options['filters']['field_asset_copyright_value']['expose']['operator'] = 'field_asset_copyright_value_op';
  $handler->display->display_options['filters']['field_asset_copyright_value']['expose']['identifier'] = 'field_asset_copyright_value';
  $handler->display->display_options['filters']['field_asset_copyright_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_asset_copyright_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: File: Motive type (field_image_motive_type) */
  $handler->display->display_options['filters']['field_image_motive_type_tid']['id'] = 'field_image_motive_type_tid';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['table'] = 'field_data_field_image_motive_type';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['field'] = 'field_image_motive_type_tid';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_image_motive_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_image_motive_type_tid']['expose']['operator_id'] = 'field_image_motive_type_tid_op';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['expose']['label'] = 'Motive type';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['expose']['operator'] = 'field_image_motive_type_tid_op';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['expose']['identifier'] = 'field_image_motive_type_tid';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_image_motive_type_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_image_motive_type_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_image_motive_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['vocabulary'] = 'image_motive_type';
  $handler->display->display_options['filters']['field_image_motive_type_tid']['hierarchy'] = 1;
  /* Filter criterion: File: Motive category (field_image_motive_category) */
  $handler->display->display_options['filters']['field_image_motive_category_tid']['id'] = 'field_image_motive_category_tid';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['table'] = 'field_data_field_image_motive_category';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['field'] = 'field_image_motive_category_tid';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_image_motive_category_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_image_motive_category_tid']['expose']['operator_id'] = 'field_image_motive_category_tid_op';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['expose']['label'] = 'Motive category';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['expose']['operator'] = 'field_image_motive_category_tid_op';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['expose']['identifier'] = 'field_image_motive_category_tid';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_image_motive_category_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_image_motive_category_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_image_motive_category_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['vocabulary'] = 'image_motive_category';
  $handler->display->display_options['filters']['field_image_motive_category_tid']['hierarchy'] = 1;
  /* Filter criterion: File: Lock from further usage (field_asset_lock) */
  $handler->display->display_options['filters']['field_asset_lock_value']['id'] = 'field_asset_lock_value';
  $handler->display->display_options['filters']['field_asset_lock_value']['table'] = 'field_data_field_asset_lock';
  $handler->display->display_options['filters']['field_asset_lock_value']['field'] = 'field_asset_lock_value';
  $handler->display->display_options['filters']['field_asset_lock_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_asset_lock_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_asset_lock_value']['expose']['operator_id'] = 'field_asset_lock_value_op';
  $handler->display->display_options['filters']['field_asset_lock_value']['expose']['label'] = 'Locked';
  $handler->display->display_options['filters']['field_asset_lock_value']['expose']['operator'] = 'field_asset_lock_value_op';
  $handler->display->display_options['filters']['field_asset_lock_value']['expose']['identifier'] = 'field_asset_lock_value';
  $handler->display->display_options['filters']['field_asset_lock_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_asset_lock_value']['expose']['reduce'] = TRUE;
  /* Filter criterion: File: Free tagging (field_image_tags) */
  $handler->display->display_options['filters']['field_image_tags_tid']['id'] = 'field_image_tags_tid';
  $handler->display->display_options['filters']['field_image_tags_tid']['table'] = 'field_data_field_image_tags';
  $handler->display->display_options['filters']['field_image_tags_tid']['field'] = 'field_image_tags_tid';
  $handler->display->display_options['filters']['field_image_tags_tid']['value'] = '';
  $handler->display->display_options['filters']['field_image_tags_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_image_tags_tid']['expose']['operator_id'] = 'field_image_tags_tid_op';
  $handler->display->display_options['filters']['field_image_tags_tid']['expose']['label'] = 'Tags';
  $handler->display->display_options['filters']['field_image_tags_tid']['expose']['operator'] = 'field_image_tags_tid_op';
  $handler->display->display_options['filters']['field_image_tags_tid']['expose']['identifier'] = 'field_image_tags_tid';
  $handler->display->display_options['filters']['field_image_tags_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_image_tags_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_image_tags_tid']['vocabulary'] = 'image_tags';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Uploaded by';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: File: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'file_managed';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['path'] = 'images';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Images';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['content'] = $view;

  return $export;
}

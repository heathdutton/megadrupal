<?php
/**
 * @file
 * cp_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cp_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cp_item_lists';
  $view->description = 'Simple lists of either your own Course planner items, or items marked as favourites.';
  $view->tag = 'courseplanner';
  $view->base_table = 'node';
  $view->human_name = 'Course planner item lists';
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
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'type' => 'type',
    'created' => 'created',
    'ops' => 'ops',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'ops' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no items to display.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Relationship: Flags: cp_facourite */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'Favourite';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'cp_facourite';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['ops']['label'] = 'Favourite';
  /* Contextual filter: Type filter, used for building select lists */
  $handler->display->display_options['arguments']['type']['id'] = 'type';
  $handler->display->display_options['arguments']['type']['table'] = 'node';
  $handler->display->display_options['arguments']['type']['field'] = 'type';
  $handler->display->display_options['arguments']['type']['ui_name'] = 'Type filter, used for building select lists';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['type']['limit'] = '0';
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
    'cp_offering' => 'cp_offering',
    'cp_outline' => 'cp_outline',
    'cp_section' => 'cp_section',
    'cp_resource' => 'cp_resource',
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'cp_offering' => 'cp_offering',
    'cp_outline' => 'cp_outline',
    'cp_section' => 'cp_section',
    'cp_resource' => 'cp_resource',
  );
  $handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['label'] = 'Filter by type';
  $handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['type_1']['expose']['reduce'] = TRUE;

  /* Display: My items */
  $handler = $view->new_display('panel_pane', 'My items', 'cp_items_per_user');
  $handler->display->display_options['display_description'] = 'Displays the Course planner items for a selected user';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Type filter, used for building select lists */
  $handler->display->display_options['arguments']['type']['id'] = 'type';
  $handler->display->display_options['arguments']['type']['table'] = 'node';
  $handler->display->display_options['arguments']['type']['field'] = 'type';
  $handler->display->display_options['arguments']['type']['ui_name'] = 'Type filter, used for building select lists';
  $handler->display->display_options['arguments']['type']['default_action'] = 'default';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['default_argument_options']['argument'] = 'all';
  $handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['type']['limit'] = '0';
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['uid']['title'] = 'Items by %1';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  $handler->display->display_options['pane_title'] = 'My items';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['argument_input'] = array(
    'type' => array(
      'type' => 'none',
      'context' => 'entity:comment.author',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Type filter, used for building select lists',
    ),
    'uid' => array(
      'type' => 'context',
      'context' => 'entity:user.uid',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Author uid',
    ),
  );
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: My favourite items */
  $handler = $view->new_display('panel_pane', 'My favourite items', 'cp_my_favourites');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'My favourite items';
  $handler->display->display_options['display_description'] = 'Displays the favourite Course planner items for the active user.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '1';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: cp_facourite */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'Favourite';
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'cp_facourite';
  $handler->display->display_options['pane_title'] = 'My favourite items';
  $handler->display->display_options['pane_description'] = 'Displays favourite items for the logged-in user.';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Course offering jump menu */
  $handler = $view->new_display('block', 'Course offering jump menu', 'cp_offering_jumpmenu');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'jump_menu';
  $handler->display->display_options['style_options']['hide'] = TRUE;
  $handler->display->display_options['style_options']['path'] = 'nid';
  $handler->display->display_options['style_options']['choose'] = '- Course offerings -';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'node/[nid]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
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
    'cp_offering' => 'cp_offering',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['block_description'] = 'Course offering jump menu';

  /* Display: Course offerings */
  $handler = $view->new_display('panel_pane', 'Course offerings', 'cp_course_offerings');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Course offerings';
  $handler->display->display_options['display_description'] = 'A searchable list of course offerings';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'name' => 'name',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
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
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Canonical course */
  $handler->display->display_options['fields']['cp_canonical_course']['id'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['table'] = 'field_data_cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['field'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['delta_offset'] = '0';
  /* Field: Content: Outline */
  $handler->display->display_options['fields']['cp_offering_outline']['id'] = 'cp_offering_outline';
  $handler->display->display_options['fields']['cp_offering_outline']['table'] = 'field_data_cp_offering_outline';
  $handler->display->display_options['fields']['cp_offering_outline']['field'] = 'cp_offering_outline';
  $handler->display->display_options['fields']['cp_offering_outline']['label'] = 'Builds on outline';
  $handler->display->display_options['fields']['cp_offering_outline']['settings'] = array(
    'link' => 1,
  );
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
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
    'cp_offering' => 'cp_offering',
  );
  /* Filter criterion: Content: Canonical course (cp_canonical_course) */
  $handler->display->display_options['filters']['cp_canonical_course_tid']['id'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['table'] = 'field_data_cp_canonical_course';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['field'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['value'] = '';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['operator_id'] = 'cp_canonical_course_tid_op';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['label'] = 'Canonical course';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['operator'] = 'cp_canonical_course_tid_op';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['identifier'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['cp_canonical_course_tid']['vocabulary'] = 'cp_canonical';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['pane_title'] = 'Course offerings';
  $handler->display->display_options['pane_description'] = 'A searchable list of course offerings';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Course outlines */
  $handler = $view->new_display('panel_pane', 'Course outlines', 'cp_course_outlines');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Course outlines';
  $handler->display->display_options['display_description'] = 'A searchable list of course outlines';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'name' => 'name',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
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
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Canonical course */
  $handler->display->display_options['fields']['cp_canonical_course']['id'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['table'] = 'field_data_cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['field'] = 'cp_canonical_course';
  $handler->display->display_options['fields']['cp_canonical_course']['delta_offset'] = '0';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
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
    'cp_outline' => 'cp_outline',
  );
  /* Filter criterion: Content: Canonical course (cp_canonical_course) */
  $handler->display->display_options['filters']['cp_canonical_course_tid']['id'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['table'] = 'field_data_cp_canonical_course';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['field'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['value'] = '';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['operator_id'] = 'cp_canonical_course_tid_op';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['label'] = 'Canonical course';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['operator'] = 'cp_canonical_course_tid_op';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['identifier'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['cp_canonical_course_tid']['vocabulary'] = 'cp_canonical';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['pane_title'] = 'Course outlines';
  $handler->display->display_options['pane_description'] = 'A searchable list of course outlines';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';
  $translatables['cp_item_lists'] = array(
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
    t('Empty text'),
    t('There are no items to display.'),
    t('Favourite'),
    t('Title'),
    t('Type'),
    t('Post date'),
    t('All'),
    t('Filter by type'),
    t('My items'),
    t('Displays the Course planner items for a selected user'),
    t('Items by %1'),
    t('Course planner'),
    t('My favourite items'),
    t('Displays the favourite Course planner items for the active user.'),
    t('Displays favourite items for the logged-in user.'),
    t('Course offering jump menu'),
    t('node/[nid]'),
    t('Course offerings'),
    t('A searchable list of course offerings'),
    t('Search'),
    t('author'),
    t('Canonical course'),
    t('Builds on outline'),
    t('Author'),
    t('Course outlines'),
    t('A searchable list of course outlines'),
  );
  $export['cp_item_lists'] = $view;

  $view = new view();
  $view->name = 'cp_offering_lessons';
  $view->description = 'Lists lessons belonging to a particular course offering.';
  $view->tag = 'default';
  $view->base_table = 'field_collection_item';
  $view->human_name = 'Course offering lessons';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'item_id' => 'item_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'item_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No lessons to display.';
  /* Relationship: Field collection item: Entity with the Lessons (cp_offering_lessons) */
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['id'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['field'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['label'] = 'Course offering';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['required'] = TRUE;
  /* Field: Field collection item: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_field_collection_item';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = 'Week';
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'courseplanner_week',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date_1']['id'] = 'cp_lesson_date_1';
  $handler->display->display_options['fields']['cp_lesson_date_1']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['label'] = 'Day';
  $handler->display->display_options['fields']['cp_lesson_date_1']['settings'] = array(
    'format_type' => 'courseplanner_daytime',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Lesson title */
  $handler->display->display_options['fields']['cp_lesson_title']['id'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['field'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['cp_lesson_title']['empty'] = '(no lesson title)';
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date_2']['id'] = 'cp_lesson_date_2';
  $handler->display->display_options['fields']['cp_lesson_date_2']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_2']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_2']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Sections */
  $handler->display->display_options['fields']['cp_lesson_section']['id'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['field'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['cp_lesson_section']['delta_offset'] = '0';
  /* Sort criterion: Field collection item: Date -  start date (cp_lesson_date) */
  $handler->display->display_options['sorts']['cp_lesson_date_value']['id'] = 'cp_lesson_date_value';
  $handler->display->display_options['sorts']['cp_lesson_date_value']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['sorts']['cp_lesson_date_value']['field'] = 'cp_lesson_date_value';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'cp_offering' => 'cp_offering',
  );
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Field collection item: Date -  start date (cp_lesson_date) */
  $handler->display->display_options['filters']['cp_lesson_date_value']['id'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['filters']['cp_lesson_date_value']['field'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['cp_lesson_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator_id'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['label'] = 'Show lessons between…';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['description'] = 'Enter nothing to show all lessons.';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['identifier'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['cp_lesson_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_date'] = 'Monday this week';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_to_date'] = 'Sunday next week +2 weeks';

  /* Display: Lesson table */
  $handler = $view->new_display('panel_pane', 'Lesson table', 'cp_lessons_table');
  $handler->display->display_options['display_description'] = 'A table of all lessons of a course offering.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pane_title'] = 'Course offering lessons (table)';
  $handler->display->display_options['pane_description'] = 'A numbered table view of all lessons in a course offering';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
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
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Weeks */
  $handler = $view->new_display('panel_pane', 'Weeks', 'cp_lessons_week');
  $handler->display->display_options['display_description'] = 'A printer-friendly list of lessons in a course offering';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'cp_lesson_date',
      'rendered' => 1,
      'rendered_strip' => 1,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'cp_lesson_date' => 'cp_lesson_date',
    'cp_lesson_date_1' => 'cp_lesson_date_1',
    'cp_lesson_title' => 'cp_lesson_title',
    'cp_lesson_section' => 'cp_lesson_title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'cp_lesson_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_lesson_date_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_lesson_title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_lesson_section' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = 'Week';
  $handler->display->display_options['fields']['cp_lesson_date']['exclude'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'courseplanner_week',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date_1']['id'] = 'cp_lesson_date_1';
  $handler->display->display_options['fields']['cp_lesson_date_1']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_date_1']['element_class'] = 'cp-date-weekday';
  $handler->display->display_options['fields']['cp_lesson_date_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date_1']['settings'] = array(
    'format_type' => 'courseplanner_daytime',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Lesson title */
  $handler->display->display_options['fields']['cp_lesson_title']['id'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['field'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_title']['empty'] = '(no lesson title)';
  /* Field: Field collection item: Sections */
  $handler->display->display_options['fields']['cp_lesson_section']['id'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['field'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['type'] = 'entityreference_entity_view';
  $handler->display->display_options['fields']['cp_lesson_section']['settings'] = array(
    'view_mode' => 'teaser',
    'links' => 0,
  );
  $handler->display->display_options['fields']['cp_lesson_section']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Field collection item: Date -  start date (cp_lesson_date) */
  $handler->display->display_options['filters']['cp_lesson_date_value']['id'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['filters']['cp_lesson_date_value']['field'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['cp_lesson_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator_id'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['label'] = 'Show lessons between';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['description'] = 'Enter nothing to show all lessons.';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['identifier'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['cp_lesson_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_date'] = 'Monday this week';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_to_date'] = 'Sunday next week +4 weeks';
  /* Filter criterion: Field collection item: Status (cp_lesson_status) */
  $handler->display->display_options['filters']['cp_lesson_status_value']['id'] = 'cp_lesson_status_value';
  $handler->display->display_options['filters']['cp_lesson_status_value']['table'] = 'field_data_cp_lesson_status';
  $handler->display->display_options['filters']['cp_lesson_status_value']['field'] = 'cp_lesson_status_value';
  $handler->display->display_options['filters']['cp_lesson_status_value']['operator'] = 'not';
  $handler->display->display_options['filters']['cp_lesson_status_value']['value'] = array(
    -2 => '-2',
  );
  $handler->display->display_options['pane_title'] = 'Course offering lessons: weeks';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
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
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Lesson admin */
  $handler = $view->new_display('panel_pane', 'Lesson admin', 'cp_lessons_admin');
  $handler->display->display_options['display_description'] = 'A bulk operations view for administering lessons';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'cp_lesson_title' => 'cp_lesson_title',
    'cp_lesson_section' => 'cp_lesson_section',
    'cp_lesson_date_2' => 'cp_lesson_date_2',
    'cp_lesson_date' => 'cp_lesson_date_2',
    'cp_lesson_date_1' => 'cp_lesson_date_2',
    'nid' => 'nid',
    'item_id' => 'item_id',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_lesson_title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_lesson_section' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_lesson_date_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'cp_lesson_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'cp_lesson_date_1' => array(
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
    'item_id' => array(
      'sortable' => 0,
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
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Field collection item */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'field_collection_item';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'rules_component::rules_cp_clear_lesson' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Clear lessons',
    ),
    'rules_component::rules_cp_copy_lesson_to_weeks' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Copy schedule to weeks',
    ),
    'rules_component::rules_cp_create_course_outline_from_selected_lessons' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_delete_lessons' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
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
    'rules_component::rules_cp_fill_lessons_with_section' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Fill with a section',
    ),
    'rules_component::rules_cp_fill_lessons_with_outline' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Fill with an outline',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
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
    'rules_component::rules_cp_push_lessons_down' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Push lesson content down',
    ),
    'rules_component::rules_cp_lesson_repeat' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Repeat schedule until',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_cp_lesson_set_room' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Set room',
    ),
    'rules_component::rules_cp_lessons_shift' => array(
      'selected' => 1,
      'skip_confirmation' => 1,
      'override_label' => 1,
      'label' => 'Move lesson content up/down',
    ),
  );
  /* Field: Field collection item: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_field_collection_item';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Field collection item: Lesson title */
  $handler->display->display_options['fields']['cp_lesson_title']['id'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['field'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['cp_lesson_title']['empty'] = '(no lesson title)';
  $handler->display->display_options['fields']['cp_lesson_title']['hide_alter_empty'] = FALSE;
  /* Field: Field collection item: Sections */
  $handler->display->display_options['fields']['cp_lesson_section']['id'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['field'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['cp_lesson_section']['delta_offset'] = '0';
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date_2']['id'] = 'cp_lesson_date_2';
  $handler->display->display_options['fields']['cp_lesson_date_2']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_2']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_2']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = 'Week and day';
  $handler->display->display_options['fields']['cp_lesson_date']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_date']['alter']['text'] = 'Week [cp_lesson_date]';
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'courseplanner_week',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date_1']['id'] = 'cp_lesson_date_1';
  $handler->display->display_options['fields']['cp_lesson_date_1']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['label'] = 'Day';
  $handler->display->display_options['fields']['cp_lesson_date_1']['settings'] = array(
    'format_type' => 'courseplanner_daytime',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Field collection item: Field collection item ID */
  $handler->display->display_options['fields']['item_id']['id'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['table'] = 'field_collection_item';
  $handler->display->display_options['fields']['item_id']['field'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['label'] = '';
  $handler->display->display_options['fields']['item_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['item_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['item_id']['separator'] = '';
  /* Field: Edit link */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Edit link';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'field-collection/cp-offering-lessons/[item_id]/edit?destination=node/[nid]/admin';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Field collection item: Status */
  $handler->display->display_options['fields']['cp_lesson_status']['id'] = 'cp_lesson_status';
  $handler->display->display_options['fields']['cp_lesson_status']['table'] = 'field_data_cp_lesson_status';
  $handler->display->display_options['fields']['cp_lesson_status']['field'] = 'cp_lesson_status';
  $handler->display->display_options['fields']['cp_lesson_status']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Field collection item: Date -  start date (cp_lesson_date) */
  $handler->display->display_options['filters']['cp_lesson_date_value']['id'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['filters']['cp_lesson_date_value']['field'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['cp_lesson_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator_id'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['label'] = 'Show lessons between…';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['description'] = 'Enter nothing to show all lessons.';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['identifier'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['cp_lesson_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_date'] = 'Monday this week';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_to_date'] = '+1 year';
  $handler->display->display_options['pane_title'] = 'Course offering admin';
  $handler->display->display_options['pane_description'] = 'A numbered table view of all lessons in a course offering';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
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
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Calendar */
  $handler = $view->new_display('panel_pane', 'Calendar', 'cp_lessons_calendar');
  $handler->display->display_options['display_description'] = 'A calendar with all lessons of a course offering.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'fullcalendar';
  $handler->display->display_options['style_options']['firstDay'] = '1';
  $handler->display->display_options['style_options']['timeformat'] = 'H:mm{ - H:mm}';
  $handler->display->display_options['style_options']['fields']['title_field'] = 'cp_lesson_title';
  $handler->display->display_options['style_options']['fields']['date_field'] = array(
    'cp_lesson_date' => 'cp_lesson_date',
  );
  $handler->display->display_options['style_options']['fields']['title'] = TRUE;
  $handler->display->display_options['style_options']['fields']['date'] = TRUE;
  $handler->display->display_options['style_options']['fullcalendar_options']['firstHour'] = 8;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['pane_title'] = 'Course offering calendar';
  $handler->display->display_options['pane_description'] = 'A calendar view of all lessons in a course offering';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
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
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: iCal feed */
  $handler = $view->new_display('feed', 'iCal feed', 'cp_lessons_export_ical');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'date_ical';
  $handler->display->display_options['row_plugin'] = 'date_ical';
  $handler->display->display_options['row_options']['date_field'] = 'field_data_cp_lesson_date.cp_lesson_date_value';
  $handler->display->display_options['row_options']['location_field'] = 'none';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Field collection item: Entity with the Lessons (cp_offering_lessons) */
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['id'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['field'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['label'] = 'Course offering';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_date']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'cp_offering' => 'cp_offering',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['path'] = 'node/%/export/lessons.ics';
  $handler->display->display_options['displays'] = array(
    'cp_lessons_table' => 'cp_lessons_table',
    'cp_lessons_admin' => 'cp_lessons_admin',
    'cp_lessons_calendar' => 'cp_lessons_calendar',
    'default' => 0,
    'cp_lessons_week' => 0,
  );

  /* Display: RSS feed */
  $handler = $view->new_display('feed', 'RSS feed', 'cp_lessons_export_rss');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'rss_fields';
  $handler->display->display_options['row_options']['title_field'] = 'cp_lesson_title';
  $handler->display->display_options['row_options']['link_field'] = 'url';
  $handler->display->display_options['row_options']['description_field'] = 'cp_lesson_section';
  $handler->display->display_options['row_options']['creator_field'] = 'name';
  $handler->display->display_options['row_options']['date_field'] = 'cp_lesson_date';
  $handler->display->display_options['row_options']['guid_field_options'] = array(
    'guid_field' => 'url',
    'guid_field_is_permalink' => 1,
  );
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Field collection item: Entity with the Lessons (cp_offering_lessons) */
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['id'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['field'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['label'] = 'Course offering';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_date']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'courseplanner_rss',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_field_collection_item';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Field collection item: Lesson title */
  $handler->display->display_options['fields']['cp_lesson_title']['id'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['field'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['empty'] = '(no lesson title)';
  /* Field: Field collection item: Sections */
  $handler->display->display_options['fields']['cp_lesson_section']['id'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['field'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['cp_lesson_section']['delta_offset'] = '0';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'cp_offering' => 'cp_offering',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['path'] = 'node/%/export/lessons.rss';
  $handler->display->display_options['displays'] = array(
    'cp_lessons_table' => 'cp_lessons_table',
    'cp_lessons_admin' => 'cp_lessons_admin',
    'cp_lessons_calendar' => 'cp_lessons_calendar',
    'default' => 0,
    'cp_lessons_week' => 0,
  );

  /* Display: CSV export */
  $handler = $view->new_display('views_data_export', 'CSV export', 'cp_lessons_export_csv');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['attach_text'] = 'CSV export';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%1-title.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'cp_offering' => 'cp_offering',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  $handler->display->display_options['filters']['field_name']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['path'] = 'node/%/export/lessons.csv';
  $handler->display->display_options['displays'] = array(
    'cp_lessons_table' => 'cp_lessons_table',
    'cp_lessons_admin' => 'cp_lessons_admin',
    'cp_lessons_calendar' => 'cp_lessons_calendar',
    'default' => 0,
    'cp_lessons_week' => 0,
  );

  /* Display: XLS export */
  $handler = $view->new_display('views_data_export', 'XLS export', 'cp_lessons_export_xls');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
  $handler->display->display_options['style_options']['attach_text'] = 'XLS export of lessons';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%1-title.xls';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'cp_offering' => 'cp_offering',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['path'] = 'node/%/export/lessons.xls';
  $handler->display->display_options['displays'] = array(
    'cp_lessons_table' => 'cp_lessons_table',
    'cp_lessons_admin' => 'cp_lessons_admin',
    'cp_lessons_calendar' => 'cp_lessons_calendar',
    'default' => 0,
    'cp_lessons_week' => 0,
  );

  /* Display: DOC export */
  $handler = $view->new_display('views_data_export', 'DOC export', 'cp_lessons_export_doc');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_doc';
  $handler->display->display_options['style_options']['attach_text'] = 'Word Document export';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%1-title.doc';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'cp_offering' => 'cp_offering',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['path'] = 'node/%/export/lessons.doc';
  $handler->display->display_options['displays'] = array(
    'cp_lessons_table' => 'cp_lessons_table',
    'cp_lessons_admin' => 'cp_lessons_admin',
    'cp_lessons_calendar' => 'cp_lessons_calendar',
    'default' => 0,
    'cp_lessons_week' => 0,
  );

  /* Display: Schedule export */
  $handler = $view->new_display('views_data_export', 'Schedule export', 'cp_lesson_export_dates');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_txt';
  $handler->display->display_options['style_options']['attach_text'] = 'Lesson date export';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%1-title.txt';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Start date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['ui_name'] = 'Start date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_date']['exclude'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'courseplanner_export',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: End date */
  $handler->display->display_options['fields']['cp_lesson_date_1']['id'] = 'cp_lesson_date_1';
  $handler->display->display_options['fields']['cp_lesson_date_1']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['ui_name'] = 'End date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_date_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_date_1']['alter']['text'] = '[cp_lesson_date], [cp_lesson_date_1]';
  $handler->display->display_options['fields']['cp_lesson_date_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date_1']['settings'] = array(
    'format_type' => 'courseplanner_export',
    'fromto' => 'value2',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['nid']['title'] = '%1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'cp_offering' => 'cp_offering',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['path'] = 'node/%/export/schedule.txt';
  $handler->display->display_options['displays'] = array(
    'cp_lessons_admin' => 'cp_lessons_admin',
    'default' => 0,
    'cp_lessons_table' => 0,
    'cp_lessons_week' => 0,
    'cp_lessons_calendar' => 0,
  );

  /* Display: Number of lessons */
  $handler = $view->new_display('panel_pane', 'Number of lessons', 'cp_lessons_count');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Field collection item ID */
  $handler->display->display_options['fields']['item_id']['id'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['table'] = 'field_collection_item';
  $handler->display->display_options['fields']['item_id']['field'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['item_id']['label'] = 'Total number of lessons';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  $handler->display->display_options['filters']['field_name']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['pane_title'] = 'Course offering lessons: count';
  $handler->display->display_options['pane_description'] = 'Displays the number of lessons in a course offering';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
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
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Number of empty lessons */
  $handler = $view->new_display('panel_pane', 'Number of empty lessons', 'cp_lesson_count_empty');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Field collection item ID */
  $handler->display->display_options['fields']['item_id']['id'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['table'] = 'field_collection_item';
  $handler->display->display_options['fields']['item_id']['field'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['item_id']['label'] = 'Number of empty lessons';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  $handler->display->display_options['filters']['field_name']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Field collection item: Lesson title (cp_lesson_title) */
  $handler->display->display_options['filters']['cp_lesson_title_value']['id'] = 'cp_lesson_title_value';
  $handler->display->display_options['filters']['cp_lesson_title_value']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['filters']['cp_lesson_title_value']['field'] = 'cp_lesson_title_value';
  $handler->display->display_options['filters']['cp_lesson_title_value']['operator'] = 'empty';
  /* Filter criterion: Field collection item: Sections (cp_lesson_section) */
  $handler->display->display_options['filters']['cp_lesson_section_target_id']['id'] = 'cp_lesson_section_target_id';
  $handler->display->display_options['filters']['cp_lesson_section_target_id']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['filters']['cp_lesson_section_target_id']['field'] = 'cp_lesson_section_target_id';
  $handler->display->display_options['filters']['cp_lesson_section_target_id']['operator'] = 'empty';
  $handler->display->display_options['pane_title'] = 'Course offering empty lessons: count';
  $handler->display->display_options['pane_description'] = 'Displays the number of empty lessons in a course offering';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
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
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Cancelled */
  $handler = $view->new_display('attachment', 'Cancelled', 'attachment_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Cancelled lessons';
  $handler->display->display_options['display_description'] = 'An attachment with cancelled lessons.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'nothing',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'cp_lesson_date' => 'cp_lesson_date',
    'cp_lesson_date_1' => 'cp_lesson_date_1',
    'cp_lesson_title' => 'cp_lesson_title',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'cp_lesson_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_lesson_date_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_lesson_title' => array(
      'sortable' => 0,
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
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = 'Week';
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'courseplanner_week',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date_1']['id'] = 'cp_lesson_date_1';
  $handler->display->display_options['fields']['cp_lesson_date_1']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['label'] = 'Day';
  $handler->display->display_options['fields']['cp_lesson_date_1']['settings'] = array(
    'format_type' => 'courseplanner_daytime',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Lesson title */
  $handler->display->display_options['fields']['cp_lesson_title']['id'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['field'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['cp_lesson_title']['empty'] = '(no lesson title)';
  /* Field: Faked header for cancelled lessons */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Faked header for cancelled lessons';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Cancelled lessons';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Field collection item: Date -  start date (cp_lesson_date) */
  $handler->display->display_options['filters']['cp_lesson_date_value']['id'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['filters']['cp_lesson_date_value']['field'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['cp_lesson_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator_id'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['label'] = 'Show lessons between…';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['description'] = 'Enter nothing to show all lessons.';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['identifier'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['cp_lesson_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_date'] = 'Monday this week';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_to_date'] = 'Sunday next week +2 weeks';
  /* Filter criterion: Field collection item: Status (cp_lesson_status) */
  $handler->display->display_options['filters']['cp_lesson_status_value']['id'] = 'cp_lesson_status_value';
  $handler->display->display_options['filters']['cp_lesson_status_value']['table'] = 'field_data_cp_lesson_status';
  $handler->display->display_options['filters']['cp_lesson_status_value']['field'] = 'cp_lesson_status_value';
  $handler->display->display_options['filters']['cp_lesson_status_value']['value'] = array(
    -2 => '-2',
  );
  $handler->display->display_options['displays'] = array(
    'cp_lessons_week' => 'cp_lessons_week',
    'default' => 0,
    'cp_lessons_table' => 0,
    'cp_lessons_admin' => 0,
    'cp_lessons_calendar' => 0,
    'cp_lessons_count' => 0,
    'cp_lesson_count_empty' => 0,
  );
  $handler->display->display_options['inherit_exposed_filters'] = TRUE;
  $translatables['cp_offering_lessons'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Empty text'),
    t('No lessons to display.'),
    t('Course offering'),
    t('Week'),
    t('Day'),
    t('Lesson title'),
    t('(no lesson title)'),
    t('Date'),
    t('Sections'),
    t('All'),
    t('Show lessons between…'),
    t('Enter nothing to show all lessons.'),
    t('Lesson table'),
    t('A table of all lessons of a course offering.'),
    t('Course offering lessons (table)'),
    t('A numbered table view of all lessons in a course offering'),
    t('Course planner'),
    t('Weeks'),
    t('A printer-friendly list of lessons in a course offering'),
    t('Show lessons between'),
    t('Course offering lessons: weeks'),
    t('Lesson admin'),
    t('A bulk operations view for administering lessons'),
    t('- Choose an operation -'),
    t('Clear lessons'),
    t('Copy schedule to weeks'),
    t('Fill with a section'),
    t('Fill with an outline'),
    t('Push lesson content down'),
    t('Repeat schedule until'),
    t('Set room'),
    t('Move lesson content up/down'),
    t('Week and day'),
    t('Week [cp_lesson_date]'),
    t('.'),
    t('edit'),
    t('Course offering admin'),
    t('Calendar'),
    t('A calendar with all lessons of a course offering.'),
    t('Course offering calendar'),
    t('A calendar view of all lessons in a course offering'),
    t('iCal feed'),
    t('%1'),
    t('RSS feed'),
    t('author'),
    t('CSV export'),
    t('XLS export'),
    t('DOC export'),
    t('Schedule export'),
    t('[cp_lesson_date], [cp_lesson_date_1]'),
    t('Number of lessons'),
    t('Total number of lessons'),
    t(','),
    t('Course offering lessons: count'),
    t('Displays the number of lessons in a course offering'),
    t('Number of empty lessons'),
    t('Course offering empty lessons: count'),
    t('Displays the number of empty lessons in a course offering'),
    t('Cancelled'),
    t('Cancelled lessons'),
    t('An attachment with cancelled lessons.'),
  );
  $export['cp_offering_lessons'] = $view;

  $view = new view();
  $view->name = 'cp_offering_menu';
  $view->description = 'Links to the course offerings the active user may edit.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Course offering shorcuts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My course offerings';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit own cp_offering content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]/admin';
  $handler->display->display_options['fields']['title']['alter']['alt'] = 'Administer this course offering';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
    'cp_offering' => 'cp_offering',
  );
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['block_description'] = 'My course offerings';
  $translatables['cp_offering_menu'] = array(
    t('Master'),
    t('My course offerings'),
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
    t('author'),
    t('Administer this course offering'),
    t('Block'),
  );
  $export['cp_offering_menu'] = $view;

  $view = new view();
  $view->name = 'cp_outline_sections';
  $view->description = 'Displays sections and their sizes of a selected course outline.';
  $view->tag = 'courseplanner';
  $view->base_table = 'node';
  $view->human_name = 'Outline sections';
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
    'cp_section' => 'cp_section',
    'cp_size' => 'cp_size',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'cp_section' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_size' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no sections in this course outline. You can use the form below to add some.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Relationship: Content: Sections (cp_sections) */
  $handler->display->display_options['relationships']['cp_sections_value']['id'] = 'cp_sections_value';
  $handler->display->display_options['relationships']['cp_sections_value']['table'] = 'field_data_cp_sections';
  $handler->display->display_options['relationships']['cp_sections_value']['field'] = 'cp_sections_value';
  $handler->display->display_options['relationships']['cp_sections_value']['label'] = 'Sections info';
  $handler->display->display_options['relationships']['cp_sections_value']['required'] = TRUE;
  $handler->display->display_options['relationships']['cp_sections_value']['delta'] = '-1';
  /* Field: Field collection item: Section */
  $handler->display->display_options['fields']['cp_section']['id'] = 'cp_section';
  $handler->display->display_options['fields']['cp_section']['table'] = 'field_data_cp_section';
  $handler->display->display_options['fields']['cp_section']['field'] = 'cp_section';
  $handler->display->display_options['fields']['cp_section']['relationship'] = 'cp_sections_value';
  $handler->display->display_options['fields']['cp_section']['group_type'] = 'count';
  $handler->display->display_options['fields']['cp_section']['settings'] = array(
    'link' => 1,
  );
  /* Field: Field collection item: Size */
  $handler->display->display_options['fields']['cp_size']['id'] = 'cp_size';
  $handler->display->display_options['fields']['cp_size']['table'] = 'field_data_cp_size';
  $handler->display->display_options['fields']['cp_size']['field'] = 'cp_size';
  $handler->display->display_options['fields']['cp_size']['relationship'] = 'cp_sections_value';
  $handler->display->display_options['fields']['cp_size']['group_type'] = 'sum';
  $handler->display->display_options['fields']['cp_size']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
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
    'cp_outline' => 'cp_outline',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: List of sections */
  $handler = $view->new_display('panel_pane', 'List of sections', 'cp_outline_sections');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pane_title'] = 'Sections in outline';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
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
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Sums */
  $handler = $view->new_display('panel_pane', 'Sums', 'cp_outline_sections_sums');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Section */
  $handler->display->display_options['fields']['cp_section']['id'] = 'cp_section';
  $handler->display->display_options['fields']['cp_section']['table'] = 'field_data_cp_section';
  $handler->display->display_options['fields']['cp_section']['field'] = 'cp_section';
  $handler->display->display_options['fields']['cp_section']['relationship'] = 'cp_sections_value';
  $handler->display->display_options['fields']['cp_section']['group_type'] = 'count';
  $handler->display->display_options['fields']['cp_section']['label'] = 'Number of sections';
  $handler->display->display_options['fields']['cp_section']['separator'] = ', ';
  /* Field: Field collection item: Size */
  $handler->display->display_options['fields']['cp_size']['id'] = 'cp_size';
  $handler->display->display_options['fields']['cp_size']['table'] = 'field_data_cp_size';
  $handler->display->display_options['fields']['cp_size']['field'] = 'cp_size';
  $handler->display->display_options['fields']['cp_size']['relationship'] = 'cp_sections_value';
  $handler->display->display_options['fields']['cp_size']['group_type'] = 'sum';
  $handler->display->display_options['fields']['cp_size']['label'] = 'Total size';
  $handler->display->display_options['fields']['cp_size']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  $handler->display->display_options['pane_title'] = 'Sections in outline: sums';
  $handler->display->display_options['pane_description'] = 'Displays the total number of sections, and their total sizes.';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
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
  $handler->display->display_options['inherit_panels_path'] = '1';
  $translatables['cp_outline_sections'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Empty text'),
    t('There are no sections in this course outline. You can use the form below to add some.'),
    t('Sections info'),
    t('Section'),
    t('Size'),
    t('All'),
    t('List of sections'),
    t('Sections in outline'),
    t('Course planner'),
    t('Sums'),
    t('Number of sections'),
    t('Total size'),
    t('Sections in outline: sums'),
    t('Displays the total number of sections, and their total sizes.'),
  );
  $export['cp_outline_sections'] = $view;

  $view = new view();
  $view->name = 'cp_reference_dialog';
  $view->description = 'Reference dialogs used for easily referencing Course planner items.';
  $view->tag = 'courseplanner';
  $view->base_table = 'node';
  $view->human_name = 'Course planner reference dialogs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Resource';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Description */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Content: File */
  $handler->display->display_options['fields']['cp_resource_file']['id'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['table'] = 'field_data_cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['field'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['cp_resource_file']['delta_offset'] = '0';
  /* Field: Content: Link */
  $handler->display->display_options['fields']['cp_resource_link']['id'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['table'] = 'field_data_cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['field'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['click_sort_column'] = 'url';
  /* Field: Content: Reference */
  $handler->display->display_options['fields']['cp_resource_reference']['id'] = 'cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['table'] = 'field_data_cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['field'] = 'cp_resource_reference';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
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
    'cp_resource' => 'cp_resource',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  /* Filter criterion: Content: Canonical course (cp_canonical_course) */
  $handler->display->display_options['filters']['cp_canonical_course_tid']['id'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['table'] = 'field_data_cp_canonical_course';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['field'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['group'] = 1;
  $handler->display->display_options['filters']['cp_canonical_course_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['operator_id'] = 'cp_canonical_course_tid_op';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['label'] = 'Canonical course';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['operator'] = 'cp_canonical_course_tid_op';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['identifier'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['cp_canonical_course_tid']['type'] = 'select';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['vocabulary'] = 'cp_canonical';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Keywords (cp_keywords) */
  $handler->display->display_options['filters']['cp_keywords_tid']['id'] = 'cp_keywords_tid';
  $handler->display->display_options['filters']['cp_keywords_tid']['table'] = 'field_data_cp_keywords';
  $handler->display->display_options['filters']['cp_keywords_tid']['field'] = 'cp_keywords_tid';
  $handler->display->display_options['filters']['cp_keywords_tid']['value'] = '';
  $handler->display->display_options['filters']['cp_keywords_tid']['group'] = 1;
  $handler->display->display_options['filters']['cp_keywords_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['operator_id'] = 'cp_keywords_tid_op';
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['label'] = 'Keywords';
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['operator'] = 'cp_keywords_tid_op';
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['identifier'] = 'cp_keywords_tid';
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['cp_keywords_tid']['vocabulary'] = 'cp_keywords';
  /* Filter criterion: Content: Author uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'node';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Author';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  /* Filter criterion: Content: Type of resource (cp_resource_type) */
  $handler->display->display_options['filters']['cp_resource_type_value']['id'] = 'cp_resource_type_value';
  $handler->display->display_options['filters']['cp_resource_type_value']['table'] = 'field_data_cp_resource_type';
  $handler->display->display_options['filters']['cp_resource_type_value']['field'] = 'cp_resource_type_value';
  $handler->display->display_options['filters']['cp_resource_type_value']['value'] = array(
    'file' => 'file',
    'video' => 'video',
    'document' => 'document',
    'link' => 'link',
    'other' => 'other',
  );
  $handler->display->display_options['filters']['cp_resource_type_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_resource_type_value']['expose']['operator_id'] = 'cp_resource_type_value_op';
  $handler->display->display_options['filters']['cp_resource_type_value']['expose']['label'] = 'Type of resource';
  $handler->display->display_options['filters']['cp_resource_type_value']['expose']['operator'] = 'cp_resource_type_value_op';
  $handler->display->display_options['filters']['cp_resource_type_value']['expose']['identifier'] = 'cp_resource_type_value';
  $handler->display->display_options['filters']['cp_resource_type_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['cp_resource_type_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: Reference dialog: resources */
  $handler = $view->new_display('references_dialog', 'Reference dialog: resources', 'cp_references_dialog_resource');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['attach'] = array(
    0 => 'field_collection_item:cp_resource:cp_section_resources_embedded',
  );

  /* Display: Reference dialog: sections */
  $handler = $view->new_display('references_dialog', 'Reference dialog: sections', 'references_dialog_1');
  $handler->display->display_options['display_description'] = 'A view used for search-and-reference in lessons->sections and outlines->sections.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Section';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Description */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
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
    'cp_section' => 'cp_section',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  /* Filter criterion: Content: Canonical course (cp_canonical_course) */
  $handler->display->display_options['filters']['cp_canonical_course_tid']['id'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['table'] = 'field_data_cp_canonical_course';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['field'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['group'] = 1;
  $handler->display->display_options['filters']['cp_canonical_course_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['operator_id'] = 'cp_canonical_course_tid_op';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['label'] = 'Canonical course';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['operator'] = 'cp_canonical_course_tid_op';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['identifier'] = 'cp_canonical_course_tid';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['cp_canonical_course_tid']['type'] = 'select';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['vocabulary'] = 'cp_canonical';
  $handler->display->display_options['filters']['cp_canonical_course_tid']['hierarchy'] = 1;
  /* Filter criterion: Content: Keywords (cp_keywords) */
  $handler->display->display_options['filters']['cp_keywords_tid']['id'] = 'cp_keywords_tid';
  $handler->display->display_options['filters']['cp_keywords_tid']['table'] = 'field_data_cp_keywords';
  $handler->display->display_options['filters']['cp_keywords_tid']['field'] = 'cp_keywords_tid';
  $handler->display->display_options['filters']['cp_keywords_tid']['value'] = '';
  $handler->display->display_options['filters']['cp_keywords_tid']['group'] = 1;
  $handler->display->display_options['filters']['cp_keywords_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['operator_id'] = 'cp_keywords_tid_op';
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['label'] = 'Keywords';
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['operator'] = 'cp_keywords_tid_op';
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['identifier'] = 'cp_keywords_tid';
  $handler->display->display_options['filters']['cp_keywords_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['cp_keywords_tid']['vocabulary'] = 'cp_keywords';
  /* Filter criterion: Content: Author uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'node';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Author';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  $handler->display->display_options['attach'] = array(
    0 => 'field_collection_item:cp_lesson_section:cp_offering_lessons',
    1 => 'field_collection_item:cp_section:cp_sections',
    2 => 'node:cp_section:cp_lesson',
  );
  $translatables['cp_reference_dialog'] = array(
    t('Master'),
    t('more'),
    t('Search'),
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
    t('Resource'),
    t('Description'),
    t('File'),
    t('Link'),
    t('Reference'),
    t('Post date'),
    t('Title'),
    t('Canonical course'),
    t('Keywords'),
    t('Author'),
    t('Type of resource'),
    t('Reference dialog: resources'),
    t('Reference dialog: sections'),
    t('A view used for search-and-reference in lessons->sections and outlines->sections.'),
    t('Section'),
  );
  $export['cp_reference_dialog'] = $view;

  $view = new view();
  $view->name = 'cp_resources_embedded';
  $view->description = 'Resources embedded on course sections pages';
  $view->tag = 'courseplanner';
  $view->base_table = 'field_collection_item';
  $view->human_name = 'Embedded resources';
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
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'cp_embedded_resource_description' => 'cp_embedded_resource_description',
    'cp_resource_file' => 'cp_resource_reference',
    'cp_resource_link' => 'cp_resource_reference',
    'cp_resource_reference' => 'cp_resource_reference',
    'title' => 'title',
    'edit_node' => 'title',
    'item_id' => 'title',
    'nothing' => 'nothing',
    'nothing_1' => 'nothing_1',
    'edit_node_1' => 'edit_node_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'cp_embedded_resource_description' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_file' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_link' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cp_resource_reference' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'item_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node_1' => array(
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 1,
    ),
  );
  /* Relationship: Field collection item: Entity with the Section resources (cp_section_resources_embedded) */
  $handler->display->display_options['relationships']['cp_section_resources_embedded_node']['id'] = 'cp_section_resources_embedded_node';
  $handler->display->display_options['relationships']['cp_section_resources_embedded_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['cp_section_resources_embedded_node']['field'] = 'cp_section_resources_embedded_node';
  $handler->display->display_options['relationships']['cp_section_resources_embedded_node']['label'] = 'Section';
  $handler->display->display_options['relationships']['cp_section_resources_embedded_node']['required'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['cp_resource_target_id']['id'] = 'cp_resource_target_id';
  $handler->display->display_options['relationships']['cp_resource_target_id']['table'] = 'field_data_cp_resource';
  $handler->display->display_options['relationships']['cp_resource_target_id']['field'] = 'cp_resource_target_id';
  $handler->display->display_options['relationships']['cp_resource_target_id']['label'] = 'Resource';
  $handler->display->display_options['relationships']['cp_resource_target_id']['required'] = TRUE;
  /* Field: Field collection item: Contextual description */
  $handler->display->display_options['fields']['cp_embedded_resource_description']['id'] = 'cp_embedded_resource_description';
  $handler->display->display_options['fields']['cp_embedded_resource_description']['table'] = 'field_data_cp_embedded_resource_description';
  $handler->display->display_options['fields']['cp_embedded_resource_description']['field'] = 'cp_embedded_resource_description';
  $handler->display->display_options['fields']['cp_embedded_resource_description']['label'] = 'Description';
  /* Field: Content: File */
  $handler->display->display_options['fields']['cp_resource_file']['id'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['table'] = 'field_data_cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['field'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['cp_resource_file']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['cp_resource_file']['delta_offset'] = '0';
  /* Field: Content: Link */
  $handler->display->display_options['fields']['cp_resource_link']['id'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['table'] = 'field_data_cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['field'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['cp_resource_link']['label'] = '';
  $handler->display->display_options['fields']['cp_resource_link']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_resource_link']['click_sort_column'] = 'url';
  /* Field: Content: Reference */
  $handler->display->display_options['fields']['cp_resource_reference']['id'] = 'cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['table'] = 'field_data_cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['field'] = 'cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['cp_resource_reference']['label'] = 'Resource';
  /* Field: Content: Description */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['title']['label'] = 'More';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = 'View full resource';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[body]';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit resource page';
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit resource page';
  /* Field: Field collection item: Field collection item ID */
  $handler->display->display_options['fields']['item_id']['id'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['table'] = 'field_collection_item';
  $handler->display->display_options['fields']['item_id']['field'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['label'] = '';
  $handler->display->display_options['fields']['item_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['item_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['item_id']['separator'] = '';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'cp_section_resources_embedded_node';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Embedding';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'field-collection/cp-section-resources-embedded/[item_id]/edit?destination=node/[nid]';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Remove embedding';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Remove';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'field-collection/cp-section-resources-embedded/[item_id]/delete?destination=node/[nid]';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node_2']['id'] = 'edit_node_2';
  $handler->display->display_options['fields']['edit_node_2']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node_2']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node_2']['relationship'] = 'cp_section_resources_embedded_node';
  $handler->display->display_options['fields']['edit_node_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_node_2']['text'] = 'Reorder';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node_1']['id'] = 'edit_node_1';
  $handler->display->display_options['fields']['edit_node_1']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node_1']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node_1']['relationship'] = 'cp_section_resources_embedded_node';
  $handler->display->display_options['fields']['edit_node_1']['label'] = 'Embedding';
  $handler->display->display_options['fields']['edit_node_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['edit_node_1']['alter']['text'] = '[nothing]<br />
[nothing_1]<br />
[edit_node_2]';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'cp_section_resources_embedded_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'cp_section' => 'cp_section',
  );

  /* Display: Embedded resources (full view) */
  $handler = $view->new_display('entity_view', 'Embedded resources (full view)', 'entity_view_1');
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'cp_section',
  );

  /* Display: Embedded resources (teaser view) */
  $handler = $view->new_display('entity_view', 'Embedded resources (teaser view)', 'entity_view_2');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Contextual description */
  $handler->display->display_options['fields']['cp_embedded_resource_description']['id'] = 'cp_embedded_resource_description';
  $handler->display->display_options['fields']['cp_embedded_resource_description']['table'] = 'field_data_cp_embedded_resource_description';
  $handler->display->display_options['fields']['cp_embedded_resource_description']['field'] = 'cp_embedded_resource_description';
  $handler->display->display_options['fields']['cp_embedded_resource_description']['label'] = 'Description';
  /* Field: Content: File */
  $handler->display->display_options['fields']['cp_resource_file']['id'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['table'] = 'field_data_cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['field'] = 'cp_resource_file';
  $handler->display->display_options['fields']['cp_resource_file']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['cp_resource_file']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['cp_resource_file']['delta_offset'] = '0';
  /* Field: Content: Link */
  $handler->display->display_options['fields']['cp_resource_link']['id'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['table'] = 'field_data_cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['field'] = 'cp_resource_link';
  $handler->display->display_options['fields']['cp_resource_link']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['cp_resource_link']['label'] = '';
  $handler->display->display_options['fields']['cp_resource_link']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_resource_link']['click_sort_column'] = 'url';
  /* Field: Content: Reference */
  $handler->display->display_options['fields']['cp_resource_reference']['id'] = 'cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['table'] = 'field_data_cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['field'] = 'cp_resource_reference';
  $handler->display->display_options['fields']['cp_resource_reference']['relationship'] = 'cp_resource_target_id';
  $handler->display->display_options['fields']['cp_resource_reference']['label'] = 'Resource';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Show list in summary (cp_list_promote) */
  $handler->display->display_options['filters']['cp_list_promote_value']['id'] = 'cp_list_promote_value';
  $handler->display->display_options['filters']['cp_list_promote_value']['table'] = 'field_data_cp_list_promote';
  $handler->display->display_options['filters']['cp_list_promote_value']['field'] = 'cp_list_promote_value';
  $handler->display->display_options['filters']['cp_list_promote_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'cp_section',
  );
  $translatables['cp_resources_embedded'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Section'),
    t('Resource'),
    t('Description'),
    t('File'),
    t('More'),
    t('View full resource'),
    t('[body]'),
    t('Edit resource page'),
    t('.'),
    t('Embedding'),
    t('Edit'),
    t('Remove embedding'),
    t('Remove'),
    t('Edit link'),
    t('Reorder'),
    t('[nothing]<br />
[nothing_1]<br />
[edit_node_2]'),
    t('All'),
    t('Embedded resources (full view)'),
    t('Embedded resources (teaser view)'),
  );
  $export['cp_resources_embedded'] = $view;

  $view = new view();
  $view->name = 'cp_section_selector';
  $view->description = 'Used for adding sections to lessons.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Section selector';
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
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Sort criterion: Content: Author uid */
  $handler->display->display_options['sorts']['uid']['id'] = 'uid';
  $handler->display->display_options['sorts']['uid']['table'] = 'node';
  $handler->display->display_options['sorts']['uid']['field'] = 'uid';
  $handler->display->display_options['sorts']['uid']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
    'cp_section' => 'cp_section',
  );

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $translatables['cp_section_selector'] = array(
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
    t('Entity Reference'),
  );
  $export['cp_section_selector'] = $view;

  $view = new view();
  $view->name = 'cp_section_used_in_offering';
  $view->description = 'Displays a list of course offerings using the currently viewed section.';
  $view->tag = 'default';
  $view->base_table = 'field_collection_item';
  $view->human_name = 'Offerings using this section';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Course offerings using this section';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Field collection item: Entity with the Lessons (cp_offering_lessons) */
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['id'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['field'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['label'] = 'Offering';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Field collection item: Lesson title */
  $handler->display->display_options['fields']['cp_lesson_title']['id'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['field'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_title']['element_label_colon'] = FALSE;
  /* Contextual filter: Field collection item: Sections (cp_lesson_section) */
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['id'] = 'cp_lesson_section_target_id';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['field'] = 'cp_lesson_section_target_id';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['validate_options']['types'] = array(
    'cp_section' => 'cp_section',
  );
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['block_description'] = 'Section use in (offering)';
  $translatables['cp_section_used_in_offering'] = array(
    t('Master'),
    t('Course offerings using this section'),
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
    t('Offering'),
    t('All'),
    t('Block'),
    t('Section use in (offering)'),
  );
  $export['cp_section_used_in_offering'] = $view;

  $view = new view();
  $view->name = 'cp_used_in';
  $view->description = 'Lists of places/contexts where a course planner item is used.';
  $view->tag = 'courseplanner';
  $view->base_table = 'node';
  $view->human_name = 'Course item used in';
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
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '3';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Section used in (offering) */
  $handler = $view->new_display('block', 'Section used in (offering)', 'cp_section_used_in_offering');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Course offerings using this section';
  $handler->display->display_options['enabled'] = FALSE;
  $handler->display->display_options['display_description'] = 'Displays course offerings using the currently viewed section. Disabled, since multi-value arguments don\'t work with aggregation.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Lessons (cp_offering_lessons) */
  $handler->display->display_options['relationships']['cp_offering_lessons_value']['id'] = 'cp_offering_lessons_value';
  $handler->display->display_options['relationships']['cp_offering_lessons_value']['table'] = 'field_data_cp_offering_lessons';
  $handler->display->display_options['relationships']['cp_offering_lessons_value']['field'] = 'cp_offering_lessons_value';
  $handler->display->display_options['relationships']['cp_offering_lessons_value']['label'] = 'Lessons';
  $handler->display->display_options['relationships']['cp_offering_lessons_value']['required'] = TRUE;
  $handler->display->display_options['relationships']['cp_offering_lessons_value']['delta'] = '-1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Field collection item: Sections (cp_lesson_section) */
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['id'] = 'cp_lesson_section_target_id';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['field'] = 'cp_lesson_section_target_id';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['relationship'] = 'cp_offering_lessons_value';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['cp_lesson_section_target_id']['validate_options']['types'] = array(
    'cp_section' => 'cp_section',
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
  $handler->display->display_options['block_description'] = 'Section used in (offering)';

  /* Display: Section used in (outline) */
  $handler = $view->new_display('block', 'Section used in (outline)', 'cp_section_used_in_outline');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Outlines using this section';
  $handler->display->display_options['display_description'] = 'Displays outlines using the currently viewed section.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Sections (cp_sections) */
  $handler->display->display_options['relationships']['cp_sections_value']['id'] = 'cp_sections_value';
  $handler->display->display_options['relationships']['cp_sections_value']['table'] = 'field_data_cp_sections';
  $handler->display->display_options['relationships']['cp_sections_value']['field'] = 'cp_sections_value';
  $handler->display->display_options['relationships']['cp_sections_value']['label'] = 'Outline sections';
  $handler->display->display_options['relationships']['cp_sections_value']['required'] = TRUE;
  $handler->display->display_options['relationships']['cp_sections_value']['delta'] = '-1';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Field collection item: Section (cp_section) */
  $handler->display->display_options['arguments']['cp_section_target_id']['id'] = 'cp_section_target_id';
  $handler->display->display_options['arguments']['cp_section_target_id']['table'] = 'field_data_cp_section';
  $handler->display->display_options['arguments']['cp_section_target_id']['field'] = 'cp_section_target_id';
  $handler->display->display_options['arguments']['cp_section_target_id']['relationship'] = 'cp_sections_value';
  $handler->display->display_options['arguments']['cp_section_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['cp_section_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['cp_section_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['cp_section_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['cp_section_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['cp_section_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['cp_section_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['cp_section_target_id']['validate_options']['types'] = array(
    'cp_section' => 'cp_section',
  );
  $handler->display->display_options['block_description'] = 'Section used in (outline)';

  /* Display: Outline used in */
  $handler = $view->new_display('block', 'Outline used in', 'cp_outline_used_in');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Course offerings using this outline';
  $handler->display->display_options['display_description'] = 'Displays course offerings using the currently viewed outline.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Outline (cp_offering_outline) */
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['id'] = 'cp_offering_outline_target_id';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['table'] = 'field_data_cp_offering_outline';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['field'] = 'cp_offering_outline_target_id';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['cp_offering_outline_target_id']['validate_options']['types'] = array(
    'cp_outline' => 'cp_outline',
  );
  $handler->display->display_options['block_description'] = 'Outline used in';

  /* Display: Resource used in */
  $handler = $view->new_display('block', 'Resource used in', 'cp_resource_used_in');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Sections using this resource';
  $handler->display->display_options['display_description'] = 'Displays sections using the currently viewed resource.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Section resources (cp_section_resources_embedded) */
  $handler->display->display_options['relationships']['cp_section_resources_embedded_value']['id'] = 'cp_section_resources_embedded_value';
  $handler->display->display_options['relationships']['cp_section_resources_embedded_value']['table'] = 'field_data_cp_section_resources_embedded';
  $handler->display->display_options['relationships']['cp_section_resources_embedded_value']['field'] = 'cp_section_resources_embedded_value';
  $handler->display->display_options['relationships']['cp_section_resources_embedded_value']['label'] = 'Embedded resource';
  $handler->display->display_options['relationships']['cp_section_resources_embedded_value']['required'] = TRUE;
  $handler->display->display_options['relationships']['cp_section_resources_embedded_value']['delta'] = '-1';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Field collection item: Resource (cp_resource) */
  $handler->display->display_options['arguments']['cp_resource_target_id']['id'] = 'cp_resource_target_id';
  $handler->display->display_options['arguments']['cp_resource_target_id']['table'] = 'field_data_cp_resource';
  $handler->display->display_options['arguments']['cp_resource_target_id']['field'] = 'cp_resource_target_id';
  $handler->display->display_options['arguments']['cp_resource_target_id']['relationship'] = 'cp_section_resources_embedded_value';
  $handler->display->display_options['arguments']['cp_resource_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['cp_resource_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['cp_resource_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['cp_resource_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['cp_resource_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['cp_resource_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['cp_resource_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['cp_resource_target_id']['validate_options']['types'] = array(
    'cp_resource' => 'cp_resource',
  );
  $handler->display->display_options['block_description'] = 'Resource used in';
  $translatables['cp_used_in'] = array(
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
    t('Section used in (offering)'),
    t('Course offerings using this section'),
    t('Displays course offerings using the currently viewed section. Disabled, since multi-value arguments don\'t work with aggregation.'),
    t('Lessons'),
    t('All'),
    t('Section used in (outline)'),
    t('Outlines using this section'),
    t('Displays outlines using the currently viewed section.'),
    t('Outline sections'),
    t('Outline used in'),
    t('Course offerings using this outline'),
    t('Displays course offerings using the currently viewed outline.'),
    t('Resource used in'),
    t('Sections using this resource'),
    t('Displays sections using the currently viewed resource.'),
    t('Embedded resource'),
  );
  $export['cp_used_in'] = $view;

  $view = new view();
  $view->name = 'cp_user_schedule';
  $view->description = 'Lists lessons belonging to a particular course offering.';
  $view->tag = 'default';
  $view->base_table = 'field_collection_item';
  $view->human_name = 'User schedule';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'item_id' => 'item_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'item_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No lessons to display.';
  /* Relationship: Field collection item: Entity with the Lessons (cp_offering_lessons) */
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['id'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['field'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['label'] = 'Course offering';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['required'] = TRUE;
  /* Field: Field collection item: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_field_collection_item';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = 'Week';
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'courseplanner_week',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date_1']['id'] = 'cp_lesson_date_1';
  $handler->display->display_options['fields']['cp_lesson_date_1']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_1']['label'] = 'Day';
  $handler->display->display_options['fields']['cp_lesson_date_1']['settings'] = array(
    'format_type' => 'courseplanner_daytime',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['fields']['title']['label'] = 'Course';
  /* Field: Field collection item: Lesson title */
  $handler->display->display_options['fields']['cp_lesson_title']['id'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['field'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['text'] = '[cp_lesson_title] ([title])';
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_title']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['cp_lesson_title']['empty'] = '(no lesson title)';
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date_2']['id'] = 'cp_lesson_date_2';
  $handler->display->display_options['fields']['cp_lesson_date_2']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_2']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date_2']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: Sections */
  $handler->display->display_options['fields']['cp_lesson_section']['id'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['field'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['cp_lesson_section']['delta_offset'] = '0';
  /* Sort criterion: Field collection item: Date -  start date (cp_lesson_date) */
  $handler->display->display_options['sorts']['cp_lesson_date_value']['id'] = 'cp_lesson_date_value';
  $handler->display->display_options['sorts']['cp_lesson_date_value']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['sorts']['cp_lesson_date_value']['field'] = 'cp_lesson_date_value';
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['uid']['title'] = 'Schedule for %1';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Field collection item: Date -  start date (cp_lesson_date) */
  $handler->display->display_options['filters']['cp_lesson_date_value']['id'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['filters']['cp_lesson_date_value']['field'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['cp_lesson_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator_id'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['label'] = 'Show lessons between…';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['description'] = 'Enter nothing to show all lessons.';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['operator'] = 'cp_lesson_date_value_op';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['identifier'] = 'cp_lesson_date_value';
  $handler->display->display_options['filters']['cp_lesson_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['cp_lesson_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_date'] = 'Monday this week';
  $handler->display->display_options['filters']['cp_lesson_date_value']['default_to_date'] = 'Sunday next week +2 weeks';

  /* Display: Calendar */
  $handler = $view->new_display('panel_pane', 'Calendar', 'cp_lessons_calendar');
  $handler->display->display_options['display_description'] = 'A calendar with all lessons of a course offering.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'fullcalendar';
  $handler->display->display_options['style_options']['firstDay'] = '1';
  $handler->display->display_options['style_options']['timeformat'] = 'H:mm{ - H:mm}';
  $handler->display->display_options['style_options']['fields']['title_field'] = 'cp_lesson_title';
  $handler->display->display_options['style_options']['fields']['date_field'] = array(
    'cp_lesson_date' => 'cp_lesson_date',
  );
  $handler->display->display_options['style_options']['fields']['title'] = TRUE;
  $handler->display->display_options['style_options']['fields']['date'] = TRUE;
  $handler->display->display_options['style_options']['fullcalendar_options']['firstHour'] = 8;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['pane_title'] = 'User schedule';
  $handler->display->display_options['pane_description'] = 'A calendar view of all lessons owned by a user';
  $handler->display->display_options['pane_category']['name'] = 'Course planner';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['argument_input'] = array(
    'uid' => array(
      'type' => 'context',
      'context' => 'entity:user.uid',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Author uid',
    ),
  );
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: iCal feed */
  $handler = $view->new_display('feed', 'iCal feed', 'cp_lessons_export_ical');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'date_ical';
  $handler->display->display_options['row_plugin'] = 'date_ical';
  $handler->display->display_options['row_options']['date_field'] = 'field_data_cp_lesson_date.cp_lesson_date_value';
  $handler->display->display_options['row_options']['location_field'] = 'none';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Field collection item: Entity with the Lessons (cp_offering_lessons) */
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['id'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['field'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['label'] = 'Course offering';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_date']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['path'] = 'user/%/schedule.ics';
  $handler->display->display_options['displays'] = array(
    'cp_lessons_table' => 'cp_lessons_table',
    'cp_lessons_admin' => 'cp_lessons_admin',
    'cp_lessons_calendar' => 'cp_lessons_calendar',
    'default' => 0,
    'cp_lessons_week' => 0,
  );

  /* Display: RSS feed */
  $handler = $view->new_display('feed', 'RSS feed', 'cp_lessons_export_rss');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'rss_fields';
  $handler->display->display_options['row_options']['title_field'] = 'cp_lesson_title';
  $handler->display->display_options['row_options']['link_field'] = 'url';
  $handler->display->display_options['row_options']['description_field'] = 'cp_lesson_section';
  $handler->display->display_options['row_options']['creator_field'] = 'name';
  $handler->display->display_options['row_options']['date_field'] = 'cp_lesson_date';
  $handler->display->display_options['row_options']['guid_field_options'] = array(
    'guid_field' => 'url',
    'guid_field_is_permalink' => 1,
  );
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Field collection item: Entity with the Lessons (cp_offering_lessons) */
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['id'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['field'] = 'cp_offering_lessons_node';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['label'] = 'Course offering';
  $handler->display->display_options['relationships']['cp_offering_lessons_node']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field collection item: Date */
  $handler->display->display_options['fields']['cp_lesson_date']['id'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['table'] = 'field_data_cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['field'] = 'cp_lesson_date';
  $handler->display->display_options['fields']['cp_lesson_date']['label'] = '';
  $handler->display->display_options['fields']['cp_lesson_date']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['cp_lesson_date']['settings'] = array(
    'format_type' => 'courseplanner_rss',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Field collection item: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_field_collection_item';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Field collection item: Lesson title */
  $handler->display->display_options['fields']['cp_lesson_title']['id'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['table'] = 'field_data_cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['field'] = 'cp_lesson_title';
  $handler->display->display_options['fields']['cp_lesson_title']['empty'] = '(no lesson title)';
  /* Field: Field collection item: Sections */
  $handler->display->display_options['fields']['cp_lesson_section']['id'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['table'] = 'field_data_cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['field'] = 'cp_lesson_section';
  $handler->display->display_options['fields']['cp_lesson_section']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['cp_lesson_section']['delta_offset'] = '0';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Field collection item: Field name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'field_collection_item';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['value'] = array(
    'cp_offering_lessons' => 'cp_offering_lessons',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'cp_offering_lessons_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['path'] = 'user/%/schedule.rss';
  $handler->display->display_options['displays'] = array(
    'cp_lessons_table' => 'cp_lessons_table',
    'cp_lessons_admin' => 'cp_lessons_admin',
    'cp_lessons_calendar' => 'cp_lessons_calendar',
    'default' => 0,
    'cp_lessons_week' => 0,
  );
  $translatables['cp_user_schedule'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Empty text'),
    t('No lessons to display.'),
    t('Course offering'),
    t('Week'),
    t('Day'),
    t('Course'),
    t('Lesson title'),
    t('[cp_lesson_title] ([title])'),
    t('(no lesson title)'),
    t('Date'),
    t('Sections'),
    t('All'),
    t('Schedule for %1'),
    t('Show lessons between…'),
    t('Enter nothing to show all lessons.'),
    t('Calendar'),
    t('A calendar with all lessons of a course offering.'),
    t('User schedule'),
    t('A calendar view of all lessons owned by a user'),
    t('Course planner'),
    t('iCal feed'),
    t('RSS feed'),
    t('author'),
  );
  $export['cp_user_schedule'] = $view;

  $view = new view();
  $view->name = 'cp_vbo_select_lists';
  $view->description = 'Used to generate select lists for Views Bulk Operations. Not displayed in itself.';
  $view->tag = 'courseplanner';
  $view->base_table = 'node';
  $view->human_name = 'VBO select lists';
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
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['order'] = 'DESC';
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
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
    'cp_outline' => 'cp_outline',
  );

  /* Display: Fill lessons with course outline */
  $handler = $view->new_display('entityreference', 'Fill lessons with course outline', 'fill_lessons');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $translatables['cp_vbo_select_lists'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('All'),
    t('Fill lessons with course outline'),
  );
  $export['cp_vbo_select_lists'] = $view;

  return $export;
}

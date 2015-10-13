<?php
/**
 * @file
 * recruiter_job_application.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function recruiter_job_application_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'application_administration';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Application administration';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Applications';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['id'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['table'] = 'field_data_field_job_application_job_ref';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['field'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['uid']['label'] = 'recruiter';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['label'] = 'Applicant';
  $handler->display->display_options['relationships']['uid_1']['required'] = TRUE;
  /* Relationship: User: Profile */
  $handler->display->display_options['relationships']['profile']['id'] = 'profile';
  $handler->display->display_options['relationships']['profile']['table'] = 'users';
  $handler->display->display_options['relationships']['profile']['field'] = 'profile';
  $handler->display->display_options['relationships']['profile']['relationship'] = 'uid_1';
  $handler->display->display_options['relationships']['profile']['label'] = 'Applicant profile';
  $handler->display->display_options['relationships']['profile']['required'] = TRUE;
  /* Relationship: Profile: Personal information (field_resume_personal_info) */
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['id'] = 'field_resume_personal_info_value';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['table'] = 'field_data_field_resume_personal_info';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['field'] = 'field_resume_personal_info_value';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['relationship'] = 'profile';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['label'] = 'Applicant personal info';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['delta'] = '-1';
  /* Relationship: Field collection item: Entity with the Personal information (field_resume_personal_info) */
  $handler->display->display_options['relationships']['field_resume_personal_info_profile2']['id'] = 'field_resume_personal_info_profile2';
  $handler->display->display_options['relationships']['field_resume_personal_info_profile2']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['field_resume_personal_info_profile2']['field'] = 'field_resume_personal_info_profile2';
  $handler->display->display_options['relationships']['field_resume_personal_info_profile2']['relationship'] = 'field_resume_personal_info_value';
  $handler->display->display_options['relationships']['field_resume_personal_info_profile2']['required'] = TRUE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Application title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Field collection item: First name */
  $handler->display->display_options['fields']['field_resume_firstname']['id'] = 'field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['table'] = 'field_data_field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['field'] = 'field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['relationship'] = 'field_resume_personal_info_value';
  $handler->display->display_options['fields']['field_resume_firstname']['exclude'] = TRUE;
  /* Field: Field collection item: Last name */
  $handler->display->display_options['fields']['field_resume_lastname']['id'] = 'field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['table'] = 'field_data_field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['field'] = 'field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['relationship'] = 'field_resume_personal_info_value';
  $handler->display->display_options['fields']['field_resume_lastname']['label'] = 'Name';
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['text'] = '[field_resume_firstname] [field_resume_lastname]';
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['path'] = 'resume/[uid]';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['name_1']['label'] = 'Applicant user';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Recruiter user';
  /* Field: Content: Application state */
  $handler->display->display_options['fields']['field_job_application_state']['id'] = 'field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['table'] = 'field_data_field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['field'] = 'field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['label'] = 'State';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Job';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = 'view job';
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
    'job_application' => 'job_application',
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
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Application title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    244994979 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
    2108160 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid_1';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Applicant user';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    244994979 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
    2108160 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_1']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['value'] = '';
  $handler->display->display_options['filters']['uid_1']['group'] = 1;
  $handler->display->display_options['filters']['uid_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_1']['expose']['operator_id'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Recruiter user';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    244994979 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
    2108160 => 0,
  );
  /* Filter criterion: Content: Application state (field_job_application_state) */
  $handler->display->display_options['filters']['field_job_application_state_value']['id'] = 'field_job_application_state_value';
  $handler->display->display_options['filters']['field_job_application_state_value']['table'] = 'field_data_field_job_application_state';
  $handler->display->display_options['filters']['field_job_application_state_value']['field'] = 'field_job_application_state_value';
  $handler->display->display_options['filters']['field_job_application_state_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_job_application_state_value']['expose']['operator_id'] = 'field_job_application_state_value_op';
  $handler->display->display_options['filters']['field_job_application_state_value']['expose']['label'] = 'State';
  $handler->display->display_options['filters']['field_job_application_state_value']['expose']['operator'] = 'field_job_application_state_value_op';
  $handler->display->display_options['filters']['field_job_application_state_value']['expose']['identifier'] = 'field_job_application_state_value';
  $handler->display->display_options['filters']['field_job_application_state_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    244994979 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
    2108160 => 0,
    239386854 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/recruiter/job_applications/overview';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Application administration';
  $handler->display->display_options['menu']['description'] = 'Administer all applications.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Job applications';
  $handler->display->display_options['tab_options']['description'] = 'Administer all job applications.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['application_administration'] = array(
    t('Master'),
    t('Applications'),
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
    t('Displaying @start - @end of @total'),
    t('Content entity referenced from field_job_application_job_ref'),
    t('recruiter'),
    t('Applicant'),
    t('Applicant profile'),
    t('Applicant personal info'),
    t('field_resume_personal_info'),
    t('Date'),
    t('Application title'),
    t('First name'),
    t('Name'),
    t('[field_resume_firstname] [field_resume_lastname]'),
    t('Applicant user'),
    t('Recruiter user'),
    t('State'),
    t('Job'),
    t('view job'),
    t('Page'),
  );
  $export['application_administration'] = $view;

  $view = new view();
  $view->name = 'job_application_applicant';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Applicant Applications';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    115362423 => '115362423',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
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
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'You have not sent any applications yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['id'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['table'] = 'field_data_field_job_application_job_ref';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['field'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['label'] = 'Job';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Job URL */
  $handler->display->display_options['fields']['job_url']['id'] = 'job_url';
  $handler->display->display_options['fields']['job_url']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['job_url']['field'] = 'job_url';
  $handler->display->display_options['fields']['job_url']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['fields']['job_url']['label'] = '';
  $handler->display->display_options['fields']['job_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['job_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['job_url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['job_url']['link_to_entity'] = 0;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Application';
  $handler->display->display_options['fields']['title_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Field: Content: Job */
  $handler->display->display_options['fields']['field_job_application_job_ref']['id'] = 'field_job_application_job_ref';
  $handler->display->display_options['fields']['field_job_application_job_ref']['table'] = 'field_data_field_job_application_job_ref';
  $handler->display->display_options['fields']['field_job_application_job_ref']['field'] = 'field_job_application_job_ref';
  $handler->display->display_options['fields']['field_job_application_job_ref']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_job_application_job_ref']['alter']['text'] = 'View Job';
  $handler->display->display_options['fields']['field_job_application_job_ref']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_job_application_job_ref']['alter']['path'] = '[job_url]';
  $handler->display->display_options['fields']['field_job_application_job_ref']['alter']['link_class'] = 'recruiter-job-link';
  $handler->display->display_options['fields']['field_job_application_job_ref']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Application state */
  $handler->display->display_options['fields']['field_job_application_state']['id'] = 'field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['table'] = 'field_data_field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['field'] = 'field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['label'] = 'State';
  $handler->display->display_options['fields']['field_job_application_state']['type'] = 'i18n_list_default';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'job_application' => 'job_application',
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['argument_input'] = array(
    'uid' => array(
      'type' => 'context',
      'context' => 'entity:user.uid',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'User: Uid',
    ),
  );
  $translatables['job_application_applicant'] = array(
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
    t('You have not sent any applications yet.'),
    t('Job'),
    t('Date'),
    t('Application'),
    t('View Job'),
    t('State'),
    t('All'),
    t('Content pane'),
    t('View panes'),
  );
  $export['job_application_applicant'] = $view;

  $view = new view();
  $view->name = 'job_application_dashboard_block';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Job application dashboard block';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Latest job applications';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Footer: Global: Unfiltered text */
  $handler->display->display_options['footer']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['footer']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['content'] = '<a href="/admin/recruiter/job_applications">More</a>';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['id'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['table'] = 'field_data_field_job_application_job_ref';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['field'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['label'] = 'Job reference';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['uid_1']['label'] = 'Job author';
  /* Relationship: User: Profile */
  $handler->display->display_options['relationships']['profile']['id'] = 'profile';
  $handler->display->display_options['relationships']['profile']['table'] = 'users';
  $handler->display->display_options['relationships']['profile']['field'] = 'profile';
  $handler->display->display_options['relationships']['profile']['relationship'] = 'uid';
  /* Relationship: Profile: Personal information (field_resume_personal_info) */
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['id'] = 'field_resume_personal_info_value';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['table'] = 'field_data_field_resume_personal_info';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['field'] = 'field_resume_personal_info_value';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['relationship'] = 'profile';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['label'] = 'field_resume_personal_info';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['delta'] = '-1';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Author uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'node';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Field collection item: First name */
  $handler->display->display_options['fields']['field_resume_firstname']['id'] = 'field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['table'] = 'field_data_field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['field'] = 'field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['relationship'] = 'field_resume_personal_info_value';
  $handler->display->display_options['fields']['field_resume_firstname']['exclude'] = TRUE;
  /* Field: Field collection item: Last name */
  $handler->display->display_options['fields']['field_resume_lastname']['id'] = 'field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['table'] = 'field_data_field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['field'] = 'field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['relationship'] = 'field_resume_personal_info_value';
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['text'] = '[field_resume_firstname] [field_resume_lastname]';
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['path'] = 'resume/[uid]';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Applicant';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['name_1']['label'] = 'Company';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
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
    'job_application' => 'job_application',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['job_application_dashboard_block'] = array(
    t('Master'),
    t('Latest job applications'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('<a href="/admin/recruiter/job_applications">More</a>'),
    t('Job reference'),
    t('author'),
    t('Job author'),
    t('Profile'),
    t('field_resume_personal_info'),
    t('Created'),
    t('Author uid'),
    t('First name'),
    t('Last name'),
    t('[field_resume_firstname] [field_resume_lastname]'),
    t('Applicant'),
    t('Company'),
    t('Title'),
    t('Block'),
  );
  $export['job_application_dashboard_block'] = $view;

  $view = new view();
  $view->name = 'job_application_recruiter';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Recruiter Applications';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    30037204 => '30037204',
    29617163 => '29617163',
    100482362 => '100482362',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter by';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Applicant';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: User: Profile */
  $handler->display->display_options['relationships']['profile']['id'] = 'profile';
  $handler->display->display_options['relationships']['profile']['table'] = 'users';
  $handler->display->display_options['relationships']['profile']['field'] = 'profile';
  $handler->display->display_options['relationships']['profile']['relationship'] = 'uid';
  $handler->display->display_options['relationships']['profile']['required'] = TRUE;
  $handler->display->display_options['relationships']['profile']['bundle_types'] = array(
    'resume' => 'resume',
  );
  /* Relationship: Profile: Personal information (field_resume_personal_info) */
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['id'] = 'field_resume_personal_info_value';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['table'] = 'field_data_field_resume_personal_info';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['field'] = 'field_resume_personal_info_value';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['relationship'] = 'profile';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['label'] = 'Applicant personal info';
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_resume_personal_info_value']['delta'] = '0';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['id'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['table'] = 'field_data_field_job_application_job_ref';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['field'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['label'] = 'Job reference';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['uid_1']['label'] = 'Recruiter';
  $handler->display->display_options['relationships']['uid_1']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Author uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'node';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  /* Field: Field collection item: First name */
  $handler->display->display_options['fields']['field_resume_firstname']['id'] = 'field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['table'] = 'field_data_field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['field'] = 'field_resume_firstname';
  $handler->display->display_options['fields']['field_resume_firstname']['relationship'] = 'field_resume_personal_info_value';
  $handler->display->display_options['fields']['field_resume_firstname']['exclude'] = TRUE;
  /* Field: Field collection item: Last name */
  $handler->display->display_options['fields']['field_resume_lastname']['id'] = 'field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['table'] = 'field_data_field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['field'] = 'field_resume_lastname';
  $handler->display->display_options['fields']['field_resume_lastname']['relationship'] = 'field_resume_personal_info_value';
  $handler->display->display_options['fields']['field_resume_lastname']['label'] = 'Applicant';
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['text'] = '[field_resume_firstname] [field_resume_lastname]';
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_resume_lastname']['alter']['path'] = 'resume/[uid]';
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = 'Application';
  $handler->display->display_options['fields']['view_node']['text'] = 'view';
  /* Field: Content: Job URL */
  $handler->display->display_options['fields']['job_url']['id'] = 'job_url';
  $handler->display->display_options['fields']['job_url']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['job_url']['field'] = 'job_url';
  $handler->display->display_options['fields']['job_url']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['fields']['job_url']['label'] = '';
  $handler->display->display_options['fields']['job_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['job_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['job_url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['job_url']['link_to_entity'] = 0;
  /* Field: Content: Job */
  $handler->display->display_options['fields']['field_job_application_job_ref']['id'] = 'field_job_application_job_ref';
  $handler->display->display_options['fields']['field_job_application_job_ref']['table'] = 'field_data_field_job_application_job_ref';
  $handler->display->display_options['fields']['field_job_application_job_ref']['field'] = 'field_job_application_job_ref';
  $handler->display->display_options['fields']['field_job_application_job_ref']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_job_application_job_ref']['alter']['path'] = '[job_url]';
  $handler->display->display_options['fields']['field_job_application_job_ref']['alter']['link_class'] = 'recruiter-job-link';
  $handler->display->display_options['fields']['field_job_application_job_ref']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_job_application_job_ref']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: Application state */
  $handler->display->display_options['fields']['field_job_application_state']['id'] = 'field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['table'] = 'field_data_field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['field'] = 'field_job_application_state';
  $handler->display->display_options['fields']['field_job_application_state']['label'] = 'State';
  $handler->display->display_options['fields']['field_job_application_state']['type'] = 'i18n_list_default';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
    100482362 => '100482362',
  );
  $handler->display->display_options['arguments']['uid']['validate']['fail'] = 'access denied';
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['arguments']['uid_1']['table'] = 'node';
  $handler->display->display_options['arguments']['uid_1']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid_1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid_1']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['uid_1']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['uid_1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid_1']['summary_options']['items_per_page'] = '25';
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
    'job_application' => 'job_application',
  );
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['filters']['nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['nid']['expose']['operator_id'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['label'] = 'Job';
  $handler->display->display_options['filters']['nid']['expose']['operator'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['identifier'] = 'nid';
  $handler->display->display_options['filters']['nid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['id'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['table'] = 'field_data_field_job_application_job_ref';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['field'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['label'] = 'Job reference';
  $handler->display->display_options['relationships']['field_job_application_job_ref_target_id']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['relationships']['uid_1']['label'] = 'Recruiter';
  $handler->display->display_options['relationships']['uid_1']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Author uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'node';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'teaser';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'You have not received any applications yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
    100482362 => '100482362',
  );
  $handler->display->display_options['arguments']['uid']['validate']['fail'] = 'access denied';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['argument_input'] = array(
    'uid' => array(
      'type' => 'context',
      'context' => 'entity:user.uid',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Author uid',
    ),
    'nid' => array(
      'type' => 'context',
      'context' => 'entity:node.nid',
      'context_optional' => 1,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Content: Nid',
    ),
  );

  /* Display: Page Shortlisted */
  $handler = $view->new_display('page', 'Page Shortlisted', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'teaser';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'You currently don\'t have any shortlisted job applications.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
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
    'job_application' => 'job_application',
  );
  /* Filter criterion: Content: Application state (field_job_application_state) */
  $handler->display->display_options['filters']['field_job_application_state_value']['id'] = 'field_job_application_state_value';
  $handler->display->display_options['filters']['field_job_application_state_value']['table'] = 'field_data_field_job_application_state';
  $handler->display->display_options['filters']['field_job_application_state_value']['field'] = 'field_job_application_state_value';
  $handler->display->display_options['filters']['field_job_application_state_value']['value'] = array(
    'shortlisted' => 'shortlisted',
  );
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid_1';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['filters']['nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['nid']['expose']['operator_id'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['label'] = 'Job';
  $handler->display->display_options['filters']['nid']['expose']['operator'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['identifier'] = 'nid';
  $handler->display->display_options['filters']['nid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
  );
  $handler->display->display_options['path'] = 'recruiter/applications/shortlisted';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Shortlisted';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Page Rejected */
  $handler = $view->new_display('page', 'Page Rejected', 'page_2');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'teaser';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'You currently don\'t have any rejected job applications.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
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
    'job_application' => 'job_application',
  );
  /* Filter criterion: Content: Application state (field_job_application_state) */
  $handler->display->display_options['filters']['field_job_application_state_value']['id'] = 'field_job_application_state_value';
  $handler->display->display_options['filters']['field_job_application_state_value']['table'] = 'field_data_field_job_application_state';
  $handler->display->display_options['filters']['field_job_application_state_value']['field'] = 'field_job_application_state_value';
  $handler->display->display_options['filters']['field_job_application_state_value']['value'] = array(
    'rejected' => 'rejected',
  );
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid_1';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['relationship'] = 'field_job_application_job_ref_target_id';
  $handler->display->display_options['filters']['nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['nid']['expose']['operator_id'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['label'] = 'Job';
  $handler->display->display_options['filters']['nid']['expose']['operator'] = 'nid_op';
  $handler->display->display_options['filters']['nid']['expose']['identifier'] = 'nid';
  $handler->display->display_options['filters']['nid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
  );
  $handler->display->display_options['path'] = 'recruiter/applications/rejected';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Rejected';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Rejected';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Page All */
  $handler = $view->new_display('page', 'Page All', 'page_3');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'teaser';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'You currently don\'t have any job applications.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'recruiter/applications/all';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'All';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Job Applications';
  $handler->display->display_options['tab_options']['weight'] = '-47';
  $handler->display->display_options['tab_options']['name'] = 'menu-recruiter-menu';
  $translatables['job_application_recruiter'] = array(
    t('Master'),
    t('more'),
    t('Filter by'),
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
    t('Applicant'),
    t('Profile'),
    t('Applicant personal info'),
    t('Job reference'),
    t('Recruiter'),
    t('Date'),
    t('Author uid'),
    t('First name'),
    t('[field_resume_firstname] [field_resume_lastname]'),
    t('Application'),
    t('view'),
    t('Job'),
    t('State'),
    t('All'),
    t('Block'),
    t('Title'),
    t('Content pane'),
    t('You have not received any applications yet.'),
    t('View panes'),
    t('Page Shortlisted'),
    t('You currently don\'t have any shortlisted job applications.'),
    t('Page Rejected'),
    t('You currently don\'t have any rejected job applications.'),
    t('Page All'),
    t('You currently don\'t have any job applications.'),
  );
  $export['job_application_recruiter'] = $view;

  return $export;
}

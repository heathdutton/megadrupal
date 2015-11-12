<?php
/**
 * @file
 * nm_members.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function nm_members_views_default_views() {
  //roles to exclude
  // see thrice below 'Filter criterion: User: Roles'
  $admin = variable_get('user_admin_role');
  $roles = variable_get('members_view_exclude_roles', array($admin => $admin));



  $export = array();

  $view = new view();
  $view->name = 'nm_member_profile';
  $view->description = 'Blocks for the Member Profile';
  $view->tag = 'nodemaker, profiles';
  $view->base_table = 'profile';
  $view->human_name = 'Member Profile';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Profile: Profile Picture */
  $handler->display->display_options['fields']['field_nm_profile_picture']['id'] = 'field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['table'] = 'field_data_field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['field'] = 'field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['label'] = '';
  $handler->display->display_options['fields']['field_nm_profile_picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_profile_picture']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_nm_profile_picture']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  /* Field: Profile: My Website */
  $handler->display->display_options['fields']['field_nm_my_website']['id'] = 'field_nm_my_website';
  $handler->display->display_options['fields']['field_nm_my_website']['table'] = 'field_data_field_nm_my_website';
  $handler->display->display_options['fields']['field_nm_my_website']['field'] = 'field_nm_my_website';
  $handler->display->display_options['fields']['field_nm_my_website']['label'] = '';
  $handler->display->display_options['fields']['field_nm_my_website']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_my_website']['click_sort_column'] = 'url';
  /* Contextual filter: Profile: User uid */
  $handler->display->display_options['arguments']['user']['id'] = 'user';
  $handler->display->display_options['arguments']['user']['table'] = 'profile';
  $handler->display->display_options['arguments']['user']['field'] = 'user';
  $handler->display->display_options['arguments']['user']['default_action'] = 'default';
  $handler->display->display_options['arguments']['user']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['user']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['user']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['user']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['user']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['user']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['user']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['user']['validate_options']['type'] = 'either';
  /* Filter criterion: Profile: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'profile';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'nm_member_profile' => 'nm_member_profile',
  );

  /* Display: Block: Cover Photo & Bio */
  $handler = $view->new_display('block', 'Block: Cover Photo & Bio', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Profile: Cover Photo */
  $handler->display->display_options['fields']['field_nm_cover_photo']['id'] = 'field_nm_cover_photo';
  $handler->display->display_options['fields']['field_nm_cover_photo']['table'] = 'field_data_field_nm_cover_photo';
  $handler->display->display_options['fields']['field_nm_cover_photo']['field'] = 'field_nm_cover_photo';
  $handler->display->display_options['fields']['field_nm_cover_photo']['label'] = '';
  $handler->display->display_options['fields']['field_nm_cover_photo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_cover_photo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_nm_cover_photo']['settings'] = array(
    'image_style' => 'large',
    'image_link' => '',
  );
  /* Field: Profile: Bio */
  $handler->display->display_options['fields']['field_nm_bio']['id'] = 'field_nm_bio';
  $handler->display->display_options['fields']['field_nm_bio']['table'] = 'field_data_field_nm_bio';
  $handler->display->display_options['fields']['field_nm_bio']['field'] = 'field_nm_bio';
  $handler->display->display_options['fields']['field_nm_bio']['label'] = '';
  $handler->display->display_options['fields']['field_nm_bio']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_bio']['hide_empty'] = TRUE;

  /* Display: Block: Name, Avatar & Website */
  $handler = $view->new_display('block', 'Block: Name, Avatar & Website', 'block_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Profile: Last Name */
  $handler->display->display_options['fields']['field_nm_last_name']['id'] = 'field_nm_last_name';
  $handler->display->display_options['fields']['field_nm_last_name']['table'] = 'field_data_field_nm_last_name';
  $handler->display->display_options['fields']['field_nm_last_name']['field'] = 'field_nm_last_name';
  $handler->display->display_options['fields']['field_nm_last_name']['label'] = '';
  $handler->display->display_options['fields']['field_nm_last_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_nm_last_name']['element_label_colon'] = FALSE;
  /* Field: Profile: First Name */
  $handler->display->display_options['fields']['field_nm_first_name']['id'] = 'field_nm_first_name';
  $handler->display->display_options['fields']['field_nm_first_name']['table'] = 'field_data_field_nm_first_name';
  $handler->display->display_options['fields']['field_nm_first_name']['field'] = 'field_nm_first_name';
  $handler->display->display_options['fields']['field_nm_first_name']['label'] = '';
  $handler->display->display_options['fields']['field_nm_first_name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_nm_first_name']['alter']['text'] = '[field_nm_first_name] [field_nm_last_name]';
  $handler->display->display_options['fields']['field_nm_first_name']['element_type'] = 'h2';
  $handler->display->display_options['fields']['field_nm_first_name']['element_label_colon'] = FALSE;
  /* Field: Profile: Profile Picture */
  $handler->display->display_options['fields']['field_nm_profile_picture']['id'] = 'field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['table'] = 'field_data_field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['field'] = 'field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['label'] = '';
  $handler->display->display_options['fields']['field_nm_profile_picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_profile_picture']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_nm_profile_picture']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  /* Field: Profile: My Website */
  $handler->display->display_options['fields']['field_nm_my_website']['id'] = 'field_nm_my_website';
  $handler->display->display_options['fields']['field_nm_my_website']['table'] = 'field_data_field_nm_my_website';
  $handler->display->display_options['fields']['field_nm_my_website']['field'] = 'field_nm_my_website';
  $handler->display->display_options['fields']['field_nm_my_website']['label'] = '';
  $handler->display->display_options['fields']['field_nm_my_website']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_my_website']['click_sort_column'] = 'url';

  /* Display: Block: Social Networks */
  $handler = $view->new_display('block', 'Block: Social Networks', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Social Networks';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Profile: Facebook */
  $handler->display->display_options['fields']['field_nm_facebook']['id'] = 'field_nm_facebook';
  $handler->display->display_options['fields']['field_nm_facebook']['table'] = 'field_data_field_nm_facebook';
  $handler->display->display_options['fields']['field_nm_facebook']['field'] = 'field_nm_facebook';
  $handler->display->display_options['fields']['field_nm_facebook']['label'] = '';
  $handler->display->display_options['fields']['field_nm_facebook']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_facebook']['click_sort_column'] = 'url';
  /* Field: Profile: Twitter */
  $handler->display->display_options['fields']['field_nm_twitter']['id'] = 'field_nm_twitter';
  $handler->display->display_options['fields']['field_nm_twitter']['table'] = 'field_data_field_nm_twitter';
  $handler->display->display_options['fields']['field_nm_twitter']['field'] = 'field_nm_twitter';
  $handler->display->display_options['fields']['field_nm_twitter']['label'] = '';
  $handler->display->display_options['fields']['field_nm_twitter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_twitter']['click_sort_column'] = 'url';
  /* Field: Profile: Google+ */
  $handler->display->display_options['fields']['field_nm_googleplus']['id'] = 'field_nm_googleplus';
  $handler->display->display_options['fields']['field_nm_googleplus']['table'] = 'field_data_field_nm_googleplus';
  $handler->display->display_options['fields']['field_nm_googleplus']['field'] = 'field_nm_googleplus';
  $handler->display->display_options['fields']['field_nm_googleplus']['label'] = '';
  $handler->display->display_options['fields']['field_nm_googleplus']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_googleplus']['click_sort_column'] = 'url';

  /* Display: Block: Professional Networks */
  $handler = $view->new_display('block', 'Block: Professional Networks', 'block_4');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Professional Networks';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Profile: LinkedIn */
  $handler->display->display_options['fields']['field_nm_linkedin']['id'] = 'field_nm_linkedin';
  $handler->display->display_options['fields']['field_nm_linkedin']['table'] = 'field_data_field_nm_linkedin';
  $handler->display->display_options['fields']['field_nm_linkedin']['field'] = 'field_nm_linkedin';
  $handler->display->display_options['fields']['field_nm_linkedin']['label'] = '';
  $handler->display->display_options['fields']['field_nm_linkedin']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_linkedin']['click_sort_column'] = 'url';

  /* Display: Block: Image Networks */
  $handler = $view->new_display('block', 'Block: Image Networks', 'block_5');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Image Networks';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Profile: Flickr */
  $handler->display->display_options['fields']['field_nm_flickr']['id'] = 'field_nm_flickr';
  $handler->display->display_options['fields']['field_nm_flickr']['table'] = 'field_data_field_nm_flickr';
  $handler->display->display_options['fields']['field_nm_flickr']['field'] = 'field_nm_flickr';
  $handler->display->display_options['fields']['field_nm_flickr']['label'] = '';
  $handler->display->display_options['fields']['field_nm_flickr']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_flickr']['click_sort_column'] = 'url';
  /* Field: Profile: Instagram */
  $handler->display->display_options['fields']['field_nm_instagram']['id'] = 'field_nm_instagram';
  $handler->display->display_options['fields']['field_nm_instagram']['table'] = 'field_data_field_nm_instagram';
  $handler->display->display_options['fields']['field_nm_instagram']['field'] = 'field_nm_instagram';
  $handler->display->display_options['fields']['field_nm_instagram']['label'] = '';
  $handler->display->display_options['fields']['field_nm_instagram']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_instagram']['click_sort_column'] = 'url';
  /* Field: Profile: Pinterest */
  $handler->display->display_options['fields']['field_nm_pinterest']['id'] = 'field_nm_pinterest';
  $handler->display->display_options['fields']['field_nm_pinterest']['table'] = 'field_data_field_nm_pinterest';
  $handler->display->display_options['fields']['field_nm_pinterest']['field'] = 'field_nm_pinterest';
  $handler->display->display_options['fields']['field_nm_pinterest']['label'] = '';
  $handler->display->display_options['fields']['field_nm_pinterest']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_pinterest']['click_sort_column'] = 'url';

  /* Display: Block: Video Networks */
  $handler = $view->new_display('block', 'Block: Video Networks', 'block_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Video Networks';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Profile: Vimeo */
  $handler->display->display_options['fields']['field_nm_vimeo']['id'] = 'field_nm_vimeo';
  $handler->display->display_options['fields']['field_nm_vimeo']['table'] = 'field_data_field_nm_vimeo';
  $handler->display->display_options['fields']['field_nm_vimeo']['field'] = 'field_nm_vimeo';
  $handler->display->display_options['fields']['field_nm_vimeo']['label'] = '';
  $handler->display->display_options['fields']['field_nm_vimeo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_vimeo']['click_sort_column'] = 'url';
  /* Field: Profile: YouTube */
  $handler->display->display_options['fields']['field_nm_youtube']['id'] = 'field_nm_youtube';
  $handler->display->display_options['fields']['field_nm_youtube']['table'] = 'field_data_field_nm_youtube';
  $handler->display->display_options['fields']['field_nm_youtube']['field'] = 'field_nm_youtube';
  $handler->display->display_options['fields']['field_nm_youtube']['label'] = '';
  $handler->display->display_options['fields']['field_nm_youtube']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_youtube']['click_sort_column'] = 'url';
  $export['nm_member_profile'] = $view;

  $view = new view();
  $view->name = 'nm_members';
  $view->description = 'All members';
  $view->tag = 'nodemaker, members';
  $view->base_table = 'users';
  $view->human_name = 'Members';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Members';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '40';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'name' => 'name',
    'field_nm_first_name' => 'field_nm_first_name',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* Relationship: User: Profile */
  $handler->display->display_options['relationships']['profile']['id'] = 'profile';
  $handler->display->display_options['relationships']['profile']['table'] = 'users';
  $handler->display->display_options['relationships']['profile']['field'] = 'profile';
  $handler->display->display_options['relationships']['profile']['bundle_types'] = array(
    'nm_member_profile' => 'nm_member_profile',
  );
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Profile: Profile Picture */
  $handler->display->display_options['fields']['field_nm_profile_picture']['id'] = 'field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['table'] = 'field_data_field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['field'] = 'field_nm_profile_picture';
  $handler->display->display_options['fields']['field_nm_profile_picture']['relationship'] = 'profile';
  $handler->display->display_options['fields']['field_nm_profile_picture']['label'] = '';
  $handler->display->display_options['fields']['field_nm_profile_picture']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_nm_profile_picture']['alter']['path'] = 'user/[uid]';
  $handler->display->display_options['fields']['field_nm_profile_picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_profile_picture']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_nm_profile_picture']['settings'] = array(
    'image_style' => 'nodemaker_thumbnail',
    'image_link' => '',
  );
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Profile: Last Name */
  $handler->display->display_options['fields']['field_nm_last_name']['id'] = 'field_nm_last_name';
  $handler->display->display_options['fields']['field_nm_last_name']['table'] = 'field_data_field_nm_last_name';
  $handler->display->display_options['fields']['field_nm_last_name']['field'] = 'field_nm_last_name';
  $handler->display->display_options['fields']['field_nm_last_name']['relationship'] = 'profile';
  $handler->display->display_options['fields']['field_nm_last_name']['label'] = '';
  $handler->display->display_options['fields']['field_nm_last_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_nm_last_name']['element_label_colon'] = FALSE;
  /* Field: Profile: First Name */
  $handler->display->display_options['fields']['field_nm_first_name']['id'] = 'field_nm_first_name';
  $handler->display->display_options['fields']['field_nm_first_name']['table'] = 'field_data_field_nm_first_name';
  $handler->display->display_options['fields']['field_nm_first_name']['field'] = 'field_nm_first_name';
  $handler->display->display_options['fields']['field_nm_first_name']['relationship'] = 'profile';
  $handler->display->display_options['fields']['field_nm_first_name']['label'] = '';
  $handler->display->display_options['fields']['field_nm_first_name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_nm_first_name']['alter']['text'] = '<span class="real-name">[field_nm_first_name]  [field_nm_last_name]</span> ';
  $handler->display->display_options['fields']['field_nm_first_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_nm_first_name']['alter']['path'] = 'user/[uid]';
  $handler->display->display_options['fields']['field_nm_first_name']['element_label_colon'] = FALSE;
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['created']['alter']['text'] = 'member since [created] ';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'F Y';
  /* Sort criterion: Profile: Profile Picture (field_nm_profile_picture:fid) */
  $handler->display->display_options['sorts']['field_nm_profile_picture_fid']['id'] = 'field_nm_profile_picture_fid';
  $handler->display->display_options['sorts']['field_nm_profile_picture_fid']['table'] = 'field_data_field_nm_profile_picture';
  $handler->display->display_options['sorts']['field_nm_profile_picture_fid']['field'] = 'field_nm_profile_picture_fid';
  $handler->display->display_options['sorts']['field_nm_profile_picture_fid']['relationship'] = 'profile';
  $handler->display->display_options['sorts']['field_nm_profile_picture_fid']['order'] = 'DESC';
  /* Sort criterion: Profile: First Name (field_nm_first_name) */
  $handler->display->display_options['sorts']['field_nm_first_name_value']['id'] = 'field_nm_first_name_value';
  $handler->display->display_options['sorts']['field_nm_first_name_value']['table'] = 'field_data_field_nm_first_name';
  $handler->display->display_options['sorts']['field_nm_first_name_value']['field'] = 'field_nm_first_name_value';
  $handler->display->display_options['sorts']['field_nm_first_name_value']['relationship'] = 'profile';
  $handler->display->display_options['sorts']['field_nm_first_name_value']['order'] = 'DESC';
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['operator'] = 'not';
  $handler->display->display_options['filters']['rid']['value'] = $roles;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['display_description'] = 'All Members';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['operator'] = 'not';
  $handler->display->display_options['filters']['rid']['value'] = $roles;
  /* Filter criterion: Profile: Profile ID */
  $handler->display->display_options['filters']['pid']['id'] = 'pid';
  $handler->display->display_options['filters']['pid']['table'] = 'profile';
  $handler->display->display_options['filters']['pid']['field'] = 'pid';
  $handler->display->display_options['filters']['pid']['relationship'] = 'profile';
  $handler->display->display_options['filters']['pid']['operator'] = 'not empty';
  $handler->display->display_options['path'] = 'members';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Members';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Attachment */
  $handler = $view->new_display('attachment', 'Attachment', 'attachment_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['operator'] = 'not';
  $handler->display->display_options['filters']['rid']['value'] = $roles;
  /* Filter criterion: Profile: Profile ID */
  $handler->display->display_options['filters']['pid']['id'] = 'pid';
  $handler->display->display_options['filters']['pid']['table'] = 'profile';
  $handler->display->display_options['filters']['pid']['field'] = 'pid';
  $handler->display->display_options['filters']['pid']['relationship'] = 'profile';
  $handler->display->display_options['filters']['pid']['operator'] = 'empty';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $handler->display->display_options['attachment_position'] = 'after';
  $export['nm_members'] = $view;

  return $export;
}

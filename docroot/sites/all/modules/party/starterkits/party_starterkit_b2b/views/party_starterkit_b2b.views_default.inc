<?php
/**
 * @file Provide default views for the Party Business to Business Starterkit
 */

/**
 * Implement hook_views_default_views().
 */
function party_starterkit_b2b_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'contacts';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'party';
  $view->human_name = 'Contacts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Contacts';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There are currently no contacts in this organizations.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Relationship: Party: Attached Profile */
  $handler->display->display_options['relationships']['eid_profile2']['id'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2']['table'] = 'party_attached_entity';
  $handler->display->display_options['relationships']['eid_profile2']['field'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2']['label'] = 'Attached Contact Profile';
  $handler->display->display_options['relationships']['eid_profile2']['required'] = 0;
  $handler->display->display_options['relationships']['eid_profile2']['data_set'] = 'profile2_party_contact';
  $handler->display->display_options['relationships']['eid_profile2']['main'] = 0;
  /* Relationship: Party: Relation: Employee of (party <-> party) */
  $handler->display->display_options['relationships']['relation_employee_party']['id'] = 'relation_employee_party';
  $handler->display->display_options['relationships']['relation_employee_party']['table'] = 'party';
  $handler->display->display_options['relationships']['relation_employee_party']['field'] = 'relation_employee_party';
  $handler->display->display_options['relationships']['relation_employee_party']['required'] = 1;
  $handler->display->display_options['relationships']['relation_employee_party']['r_index'] = '0';
  /* Relationship: Party: Relation: Employee of (party -> relation) */
  $handler->display->display_options['relationships']['relation_base_left_employee']['id'] = 'relation_base_left_employee';
  $handler->display->display_options['relationships']['relation_base_left_employee']['table'] = 'party';
  $handler->display->display_options['relationships']['relation_base_left_employee']['field'] = 'relation_base_left_employee';
  $handler->display->display_options['relationships']['relation_base_left_employee']['label'] = 'Employee of (Relationship)';
  $handler->display->display_options['relationships']['relation_base_left_employee']['required'] = 0;
  $handler->display->display_options['relationships']['relation_base_left_employee']['r_index'] = '0';
  /* Relationship: Party: Attached Profile */
  $handler->display->display_options['relationships']['eid_profile2_1']['id'] = 'eid_profile2_1';
  $handler->display->display_options['relationships']['eid_profile2_1']['table'] = 'party_attached_entity';
  $handler->display->display_options['relationships']['eid_profile2_1']['field'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2_1']['relationship'] = 'relation_employee_party';
  $handler->display->display_options['relationships']['eid_profile2_1']['label'] = 'Organisation Attached Profile';
  $handler->display->display_options['relationships']['eid_profile2_1']['required'] = 0;
  $handler->display->display_options['relationships']['eid_profile2_1']['data_set'] = 'profile2_party_org';
  $handler->display->display_options['relationships']['eid_profile2_1']['main'] = 0;
  /* Field: Party: Party ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'party';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  $handler->display->display_options['fields']['pid']['label'] = '';
  $handler->display->display_options['fields']['pid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['pid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['pid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['pid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['pid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['pid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['pid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['pid']['separator'] = '';
  $handler->display->display_options['fields']['pid']['format_plural'] = 0;
  /* Field: Party: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'party';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = 'Contact Name';
  $handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['label']['alter']['path'] = 'party/[pid]';
  $handler->display->display_options['fields']['label']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['label']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['label']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label']['empty_zero'] = 0;
  $handler->display->display_options['fields']['label']['hide_alter_empty'] = 1;
  /* Field: Party: Party ID */
  $handler->display->display_options['fields']['pid_1']['id'] = 'pid_1';
  $handler->display->display_options['fields']['pid_1']['table'] = 'party';
  $handler->display->display_options['fields']['pid_1']['field'] = 'pid';
  $handler->display->display_options['fields']['pid_1']['relationship'] = 'relation_employee_party';
  $handler->display->display_options['fields']['pid_1']['label'] = '';
  $handler->display->display_options['fields']['pid_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['pid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['pid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['pid_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['pid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['pid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['pid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['pid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['pid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['pid_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['pid_1']['separator'] = '';
  $handler->display->display_options['fields']['pid_1']['format_plural'] = 0;
  /* Field: Party: Label */
  $handler->display->display_options['fields']['label_1']['id'] = 'label_1';
  $handler->display->display_options['fields']['label_1']['table'] = 'party';
  $handler->display->display_options['fields']['label_1']['field'] = 'label';
  $handler->display->display_options['fields']['label_1']['relationship'] = 'relation_employee_party';
  $handler->display->display_options['fields']['label_1']['label'] = 'Organization';
  $handler->display->display_options['fields']['label_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['label_1']['alter']['path'] = 'party/[pid_1]';
  $handler->display->display_options['fields']['label_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['label_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['label_1']['hide_alter_empty'] = 1;
  /* Field: Profile: Email */
  $handler->display->display_options['fields']['party_contact_email']['id'] = 'party_contact_email';
  $handler->display->display_options['fields']['party_contact_email']['table'] = 'field_data_party_contact_email';
  $handler->display->display_options['fields']['party_contact_email']['field'] = 'party_contact_email';
  $handler->display->display_options['fields']['party_contact_email']['relationship'] = 'eid_profile2';
  /* Field: Profile: Phone Number */
  $handler->display->display_options['fields']['party_contact_phone']['id'] = 'party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['table'] = 'field_data_party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['field'] = 'party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['relationship'] = 'eid_profile2';
  $handler->display->display_options['fields']['party_contact_phone']['label'] = 'Phone';
  $handler->display->display_options['fields']['party_contact_phone']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['external'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['html'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['hide_empty'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['empty_zero'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['group_rows'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['delta_offset'] = '0';
  $handler->display->display_options['fields']['party_contact_phone']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['field_api_classes'] = 0;
  /* Field: Bulk operations: Party */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'party';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_delete_party' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'use_queue' => 0,
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
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_subscribe_to_kambiocrm_newsletter' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
  );
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
  /* Sort criterion: Profile: Date created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'profile';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['relationship'] = 'eid_profile2';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Post Date';
  /* Filter criterion: Party: Hats (party_hat) */
  $handler->display->display_options['filters']['party_hat_hat_name']['id'] = 'party_hat_hat_name';
  $handler->display->display_options['filters']['party_hat_hat_name']['table'] = 'field_data_party_hat';
  $handler->display->display_options['filters']['party_hat_hat_name']['field'] = 'party_hat_hat_name';
  $handler->display->display_options['filters']['party_hat_hat_name']['value'] = 'contact';
  $handler->display->display_options['filters']['party_hat_hat_name']['group'] = 1;
  /* Filter criterion: Profile: Name (party_contact_name:given) */
  $handler->display->display_options['filters']['party_contact_name_given']['id'] = 'party_contact_name_given';
  $handler->display->display_options['filters']['party_contact_name_given']['table'] = 'field_data_party_contact_name';
  $handler->display->display_options['filters']['party_contact_name_given']['field'] = 'party_contact_name_given';
  $handler->display->display_options['filters']['party_contact_name_given']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['party_contact_name_given']['operator'] = 'contains';
  $handler->display->display_options['filters']['party_contact_name_given']['group'] = 1;
  $handler->display->display_options['filters']['party_contact_name_given']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_contact_name_given']['expose']['operator_id'] = 'party_contact_name_given_op';
  $handler->display->display_options['filters']['party_contact_name_given']['expose']['label'] = 'First';
  $handler->display->display_options['filters']['party_contact_name_given']['expose']['operator'] = 'party_contact_name_given_op';
  $handler->display->display_options['filters']['party_contact_name_given']['expose']['identifier'] = 'party_contact_name_given';
  $handler->display->display_options['filters']['party_contact_name_given']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_contact_name_given']['expose']['multiple'] = FALSE;
  /* Filter criterion: Profile: Name (party_contact_name:family) */
  $handler->display->display_options['filters']['party_contact_name_family']['id'] = 'party_contact_name_family';
  $handler->display->display_options['filters']['party_contact_name_family']['table'] = 'field_data_party_contact_name';
  $handler->display->display_options['filters']['party_contact_name_family']['field'] = 'party_contact_name_family';
  $handler->display->display_options['filters']['party_contact_name_family']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['party_contact_name_family']['operator'] = 'contains';
  $handler->display->display_options['filters']['party_contact_name_family']['group'] = 1;
  $handler->display->display_options['filters']['party_contact_name_family']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_contact_name_family']['expose']['operator_id'] = 'party_contact_name_family_op';
  $handler->display->display_options['filters']['party_contact_name_family']['expose']['label'] = 'Last';
  $handler->display->display_options['filters']['party_contact_name_family']['expose']['operator'] = 'party_contact_name_family_op';
  $handler->display->display_options['filters']['party_contact_name_family']['expose']['identifier'] = 'party_contact_name_family';
  $handler->display->display_options['filters']['party_contact_name_family']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_contact_name_family']['expose']['multiple'] = FALSE;
  /* Filter criterion: Profile: Name (party_contact_name:title) */
  $handler->display->display_options['filters']['party_contact_name_title']['id'] = 'party_contact_name_title';
  $handler->display->display_options['filters']['party_contact_name_title']['table'] = 'field_data_party_contact_name';
  $handler->display->display_options['filters']['party_contact_name_title']['field'] = 'party_contact_name_title';
  $handler->display->display_options['filters']['party_contact_name_title']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['party_contact_name_title']['operator'] = 'contains';
  $handler->display->display_options['filters']['party_contact_name_title']['group'] = 1;
  $handler->display->display_options['filters']['party_contact_name_title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_contact_name_title']['expose']['operator_id'] = 'party_contact_name_title_op';
  $handler->display->display_options['filters']['party_contact_name_title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['party_contact_name_title']['expose']['operator'] = 'party_contact_name_title_op';
  $handler->display->display_options['filters']['party_contact_name_title']['expose']['identifier'] = 'party_contact_name_title';
  $handler->display->display_options['filters']['party_contact_name_title']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_contact_name_title']['expose']['multiple'] = FALSE;
  /* Filter criterion: Profile: Email (party_contact_email) */
  $handler->display->display_options['filters']['party_contact_email_email']['id'] = 'party_contact_email_email';
  $handler->display->display_options['filters']['party_contact_email_email']['table'] = 'field_data_party_contact_email';
  $handler->display->display_options['filters']['party_contact_email_email']['field'] = 'party_contact_email_email';
  $handler->display->display_options['filters']['party_contact_email_email']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['party_contact_email_email']['group'] = 1;
  $handler->display->display_options['filters']['party_contact_email_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_contact_email_email']['expose']['operator_id'] = 'party_contact_email_email_op';
  $handler->display->display_options['filters']['party_contact_email_email']['expose']['label'] = 'Email';
  $handler->display->display_options['filters']['party_contact_email_email']['expose']['operator'] = 'party_contact_email_email_op';
  $handler->display->display_options['filters']['party_contact_email_email']['expose']['identifier'] = 'party_contact_email_email';
  $handler->display->display_options['filters']['party_contact_email_email']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_contact_email_email']['expose']['multiple'] = FALSE;
  /* Filter criterion: Profile: Name (party_org_name) */
  $handler->display->display_options['filters']['party_org_name_value']['id'] = 'party_org_name_value';
  $handler->display->display_options['filters']['party_org_name_value']['table'] = 'field_data_party_org_name';
  $handler->display->display_options['filters']['party_org_name_value']['field'] = 'party_org_name_value';
  $handler->display->display_options['filters']['party_org_name_value']['relationship'] = 'eid_profile2_1';
  $handler->display->display_options['filters']['party_org_name_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['party_org_name_value']['group'] = 1;
  $handler->display->display_options['filters']['party_org_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_org_name_value']['expose']['operator_id'] = 'party_org_name_value_op';
  $handler->display->display_options['filters']['party_org_name_value']['expose']['label'] = 'Organization Name';
  $handler->display->display_options['filters']['party_org_name_value']['expose']['operator'] = 'party_org_name_value_op';
  $handler->display->display_options['filters']['party_org_name_value']['expose']['identifier'] = 'party_org_name_value';
  $handler->display->display_options['filters']['party_org_name_value']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_org_name_value']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There are currently no contacts.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  $handler->display->display_options['path'] = 'contacts';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Contacts';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There are currently no contacts in this organization.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Party: Party ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'party';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  $handler->display->display_options['fields']['pid']['label'] = '';
  $handler->display->display_options['fields']['pid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['pid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['pid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['pid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['pid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['pid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['pid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['pid']['separator'] = '';
  $handler->display->display_options['fields']['pid']['format_plural'] = 0;
  /* Field: Party: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'party';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = 'Contact Name';
  $handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['label']['alter']['path'] = 'party/[pid]';
  $handler->display->display_options['fields']['label']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['label']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['label']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label']['empty_zero'] = 0;
  $handler->display->display_options['fields']['label']['hide_alter_empty'] = 1;
  /* Field: Profile: Email */
  $handler->display->display_options['fields']['party_contact_email']['id'] = 'party_contact_email';
  $handler->display->display_options['fields']['party_contact_email']['table'] = 'field_data_party_contact_email';
  $handler->display->display_options['fields']['party_contact_email']['field'] = 'party_contact_email';
  $handler->display->display_options['fields']['party_contact_email']['relationship'] = 'eid_profile2';
  /* Field: Profile: Phone Number */
  $handler->display->display_options['fields']['party_contact_phone']['id'] = 'party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['table'] = 'field_data_party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['field'] = 'party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['relationship'] = 'eid_profile2';
  $handler->display->display_options['fields']['party_contact_phone']['label'] = 'Phone';
  $handler->display->display_options['fields']['party_contact_phone']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['external'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['html'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['hide_empty'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['empty_zero'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['group_rows'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['delta_offset'] = '0';
  $handler->display->display_options['fields']['party_contact_phone']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['field_api_classes'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Party: Party ID */
  $handler->display->display_options['arguments']['pid']['id'] = 'pid';
  $handler->display->display_options['arguments']['pid']['table'] = 'party';
  $handler->display->display_options['arguments']['pid']['field'] = 'pid';
  $handler->display->display_options['arguments']['pid']['relationship'] = 'relation_employee_party';
  $handler->display->display_options['arguments']['pid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['pid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['pid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['pid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['pid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['pid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['pid']['not'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Party: Hats (party_hat) */
  $handler->display->display_options['filters']['party_hat_hat_name']['id'] = 'party_hat_hat_name';
  $handler->display->display_options['filters']['party_hat_hat_name']['table'] = 'field_data_party_hat';
  $handler->display->display_options['filters']['party_hat_hat_name']['field'] = 'party_hat_hat_name';
  $handler->display->display_options['filters']['party_hat_hat_name']['value'] = 'contact';
  $handler->display->display_options['filters']['party_hat_hat_name']['group'] = 1;
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
    'pid' => array(
      'type' => 'user',
      'context' => 'entity:party_activity.party-activity-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Party: Party ID',
    ),
  );

  $views['contacts'] = $view;
  
  // Organizations view
  $view = new view;
  $view->name = 'organizations';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'party';
  $view->human_name = 'Organizations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Organization';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Party: Attached Profile */
  $handler->display->display_options['relationships']['eid_profile2']['id'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2']['table'] = 'party_attached_entity';
  $handler->display->display_options['relationships']['eid_profile2']['field'] = 'eid_profile2';
  $handler->display->display_options['relationships']['eid_profile2']['label'] = 'Attached Organisation Profile';
  $handler->display->display_options['relationships']['eid_profile2']['required'] = 0;
  $handler->display->display_options['relationships']['eid_profile2']['data_set'] = 'profile2_party_org';
  $handler->display->display_options['relationships']['eid_profile2']['main'] = 0;
  /* Field: Party: Party ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'party';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  $handler->display->display_options['fields']['pid']['label'] = '';
  $handler->display->display_options['fields']['pid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['pid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['pid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['pid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['pid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['pid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['pid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['pid']['separator'] = '';
  $handler->display->display_options['fields']['pid']['format_plural'] = 0;
  /* Field: Party: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'party';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = 'Name';
  $handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['label']['alter']['path'] = 'party/[pid]';
  $handler->display->display_options['fields']['label']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['label']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['label']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label']['empty_zero'] = 0;
  $handler->display->display_options['fields']['label']['hide_alter_empty'] = 1;
  /* Field: Profile: Phone Number */
  $handler->display->display_options['fields']['party_contact_phone']['id'] = 'party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['table'] = 'field_data_party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['field'] = 'party_contact_phone';
  $handler->display->display_options['fields']['party_contact_phone']['relationship'] = 'eid_profile2';
  $handler->display->display_options['fields']['party_contact_phone']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['external'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['alter']['html'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['hide_empty'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['empty_zero'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['group_rows'] = 1;
  $handler->display->display_options['fields']['party_contact_phone']['delta_offset'] = '0';
  $handler->display->display_options['fields']['party_contact_phone']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['party_contact_phone']['field_api_classes'] = 0;
  /* Field: Profile: Address */
  $handler->display->display_options['fields']['party_org_address']['id'] = 'party_org_address';
  $handler->display->display_options['fields']['party_org_address']['table'] = 'field_data_party_org_address';
  $handler->display->display_options['fields']['party_org_address']['field'] = 'party_org_address';
  $handler->display->display_options['fields']['party_org_address']['relationship'] = 'eid_profile2';
  $handler->display->display_options['fields']['party_org_address']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['external'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['party_org_address']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['party_org_address']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['party_org_address']['alter']['html'] = 0;
  $handler->display->display_options['fields']['party_org_address']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['party_org_address']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['party_org_address']['hide_empty'] = 0;
  $handler->display->display_options['fields']['party_org_address']['empty_zero'] = 0;
  $handler->display->display_options['fields']['party_org_address']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['party_org_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['party_org_address']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  $handler->display->display_options['fields']['party_org_address']['group_rows'] = 1;
  $handler->display->display_options['fields']['party_org_address']['delta_offset'] = '0';
  $handler->display->display_options['fields']['party_org_address']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['party_org_address']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['party_org_address']['field_api_classes'] = 0;
  /* Field: Profile: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'profile';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['relationship'] = 'eid_profile2';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Bulk operations: Party */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'party';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_delete_party' => array(
      'selected' => 1,
      'use_queue' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'use_queue' => 0,
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
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_subscribe_to_kambiocrm_newsletter' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
  /* Sort criterion: Profile: Date created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'profile';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['relationship'] = 'eid_profile2';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Date created';
  /* Sort criterion: Profile: Name (party_org_name) */
  $handler->display->display_options['sorts']['party_org_name_value']['id'] = 'party_org_name_value';
  $handler->display->display_options['sorts']['party_org_name_value']['table'] = 'field_data_party_org_name';
  $handler->display->display_options['sorts']['party_org_name_value']['field'] = 'party_org_name_value';
  $handler->display->display_options['sorts']['party_org_name_value']['relationship'] = 'eid_profile2';
  $handler->display->display_options['sorts']['party_org_name_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['party_org_name_value']['expose']['label'] = 'Name';
  /* Filter criterion: Party: Hats (party_hat) */
  $handler->display->display_options['filters']['party_hat_hat_name']['id'] = 'party_hat_hat_name';
  $handler->display->display_options['filters']['party_hat_hat_name']['table'] = 'field_data_party_hat';
  $handler->display->display_options['filters']['party_hat_hat_name']['field'] = 'party_hat_hat_name';
  $handler->display->display_options['filters']['party_hat_hat_name']['value'] = 'org';
  $handler->display->display_options['filters']['party_hat_hat_name']['group'] = 1;
  /* Filter criterion: Profile: Name (party_org_name) */
  $handler->display->display_options['filters']['party_org_name_value']['id'] = 'party_org_name_value';
  $handler->display->display_options['filters']['party_org_name_value']['table'] = 'field_data_party_org_name';
  $handler->display->display_options['filters']['party_org_name_value']['field'] = 'party_org_name_value';
  $handler->display->display_options['filters']['party_org_name_value']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['party_org_name_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['party_org_name_value']['group'] = 1;
  $handler->display->display_options['filters']['party_org_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_org_name_value']['expose']['operator_id'] = 'party_org_name_value_op';
  $handler->display->display_options['filters']['party_org_name_value']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['party_org_name_value']['expose']['operator'] = 'party_org_name_value_op';
  $handler->display->display_options['filters']['party_org_name_value']['expose']['identifier'] = 'party_org_name_value';
  $handler->display->display_options['filters']['party_org_name_value']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_org_name_value']['expose']['multiple'] = FALSE;
  /* Filter criterion: Profile: Address (party_org_address:administrative_area) */
  $handler->display->display_options['filters']['party_org_address_administrative_area']['id'] = 'party_org_address_administrative_area';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['table'] = 'field_data_party_org_address';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['field'] = 'party_org_address_administrative_area';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['operator'] = 'contains';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_org_address_administrative_area']['expose']['operator_id'] = 'party_org_address_administrative_area_op';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['expose']['label'] = 'State';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['expose']['operator'] = 'party_org_address_administrative_area_op';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['expose']['identifier'] = 'party_org_address_administrative_area';
  $handler->display->display_options['filters']['party_org_address_administrative_area']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_org_address_administrative_area']['expose']['multiple'] = FALSE;
  /* Filter criterion: Profile: Address (party_org_address:locality) */
  $handler->display->display_options['filters']['party_org_address_locality']['id'] = 'party_org_address_locality';
  $handler->display->display_options['filters']['party_org_address_locality']['table'] = 'field_data_party_org_address';
  $handler->display->display_options['filters']['party_org_address_locality']['field'] = 'party_org_address_locality';
  $handler->display->display_options['filters']['party_org_address_locality']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['party_org_address_locality']['operator'] = 'contains';
  $handler->display->display_options['filters']['party_org_address_locality']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_org_address_locality']['expose']['operator_id'] = 'party_org_address_locality_op';
  $handler->display->display_options['filters']['party_org_address_locality']['expose']['label'] = 'City';
  $handler->display->display_options['filters']['party_org_address_locality']['expose']['operator'] = 'party_org_address_locality_op';
  $handler->display->display_options['filters']['party_org_address_locality']['expose']['identifier'] = 'party_org_address_locality';
  $handler->display->display_options['filters']['party_org_address_locality']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_org_address_locality']['expose']['multiple'] = FALSE;
  /* Filter criterion: Profile: Address (party_org_address:postal_code) */
  $handler->display->display_options['filters']['party_org_address_postal_code']['id'] = 'party_org_address_postal_code';
  $handler->display->display_options['filters']['party_org_address_postal_code']['table'] = 'field_data_party_org_address';
  $handler->display->display_options['filters']['party_org_address_postal_code']['field'] = 'party_org_address_postal_code';
  $handler->display->display_options['filters']['party_org_address_postal_code']['relationship'] = 'eid_profile2';
  $handler->display->display_options['filters']['party_org_address_postal_code']['operator'] = 'contains';
  $handler->display->display_options['filters']['party_org_address_postal_code']['exposed'] = TRUE;
  $handler->display->display_options['filters']['party_org_address_postal_code']['expose']['operator_id'] = 'party_org_address_postal_code_op';
  $handler->display->display_options['filters']['party_org_address_postal_code']['expose']['label'] = 'Postal Code';
  $handler->display->display_options['filters']['party_org_address_postal_code']['expose']['operator'] = 'party_org_address_postal_code_op';
  $handler->display->display_options['filters']['party_org_address_postal_code']['expose']['identifier'] = 'party_org_address_postal_code';
  $handler->display->display_options['filters']['party_org_address_postal_code']['expose']['required'] = 0;
  $handler->display->display_options['filters']['party_org_address_postal_code']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'organizations';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Organizations';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  $views['organizations'] = $view;

  return $views;
}
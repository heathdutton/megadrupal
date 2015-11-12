<?php

/**
 * @file
 * Contains hooks for view plugins.
 */


/**
 * Implements hook_views_default_views().
 */
function webform_confirmation_views_default_views() {
  $view = new view();
  $view->name = 'list_of_submissions';
  $view->description = 'This view has the list of all the submissions of the webform.';
  $view->tag = 'default';
  $view->base_table = 'webform_confirmation';
  $view->human_name = 'List of Submissions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'List of Submissions';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'The input that you are searching does not exist.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Webform Confirmation: Senders Mail ID */
  $handler->display->display_options['fields']['sender_email']['id'] = 'sender_email';
  $handler->display->display_options['fields']['sender_email']['table'] = 'webform_confirmation';
  $handler->display->display_options['fields']['sender_email']['field'] = 'sender_email';
  $handler->display->display_options['fields']['sender_email']['label'] = 'Mail ID';
  /* Field: Webform Confirmation: ID of the webform */
  $handler->display->display_options['fields']['webform_id']['id'] = 'webform_id';
  $handler->display->display_options['fields']['webform_id']['table'] = 'webform_confirmation';
  $handler->display->display_options['fields']['webform_id']['field'] = 'webform_id';
  $handler->display->display_options['fields']['webform_id']['label'] = '';
  $handler->display->display_options['fields']['webform_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['webform_id']['element_label_colon'] = FALSE;
  /* Field: Webform Confirmation: Submission Id */
  $handler->display->display_options['fields']['sid']['id'] = 'sid';
  $handler->display->display_options['fields']['sid']['table'] = 'webform_confirmation';
  $handler->display->display_options['fields']['sid']['field'] = 'sid';
  $handler->display->display_options['fields']['sid']['label'] = '';
  $handler->display->display_options['fields']['sid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['sid']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'LIST OF SUBMISSIONS';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href = "/node/[webform_id]/submission/[sid]">Submission</a>';
  /* Field: Webform Confirmation: Name of the webform */
  $handler->display->display_options['fields']['webform_name']['id'] = 'webform_name';
  $handler->display->display_options['fields']['webform_name']['table'] = 'webform_confirmation';
  $handler->display->display_options['fields']['webform_name']['field'] = 'webform_name';
  $handler->display->display_options['fields']['webform_name']['label'] = 'Webform Name';
  /* Field: Webform Confirmation: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'webform_confirmation';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Filter criterion: Webform Confirmation: Senders Mail ID */
  $handler->display->display_options['filters']['sender_email']['id'] = 'sender_email';
  $handler->display->display_options['filters']['sender_email']['table'] = 'webform_confirmation';
  $handler->display->display_options['filters']['sender_email']['field'] = 'sender_email';
  $handler->display->display_options['filters']['sender_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['sender_email']['expose']['operator_id'] = 'sender_email_op';
  $handler->display->display_options['filters']['sender_email']['expose']['label'] = 'Mail ID';
  $handler->display->display_options['filters']['sender_email']['expose']['operator'] = 'sender_email_op';
  $handler->display->display_options['filters']['sender_email']['expose']['identifier'] = 'sender_email';
  $handler->display->display_options['filters']['sender_email']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Webform Confirmation: Name of the webform */
  $handler->display->display_options['filters']['webform_name']['id'] = 'webform_name';
  $handler->display->display_options['filters']['webform_name']['table'] = 'webform_confirmation';
  $handler->display->display_options['filters']['webform_name']['field'] = 'webform_name';
  $handler->display->display_options['filters']['webform_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['webform_name']['expose']['operator_id'] = 'webform_name_op';
  $handler->display->display_options['filters']['webform_name']['expose']['label'] = 'Webform Name';
  $handler->display->display_options['filters']['webform_name']['expose']['operator'] = 'webform_name_op';
  $handler->display->display_options['filters']['webform_name']['expose']['identifier'] = 'webform_name';
  $handler->display->display_options['filters']['webform_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Webform Confirmation: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'webform_confirmation';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: List of Submissions Block */
  $handler = $view->new_display('block', 'List of Submissions Block', 'block');
  $handler->display->display_options['display_description'] = 'This view has the list of all the approved and unapproved Submissions.';

  // Add view to list of views to provide.
  $views[$view->name] = $view;
  // Repeat all of the above for each view the module should provide.
  // At the end, return array of default views.
  return $views;
}

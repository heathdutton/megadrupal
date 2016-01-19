<?php

/**
 * @file
 * Views definition for Goals Extras.
 */

/**
 * Implements hook_views_default_views().
 */
function goals_extras_views_default_views() {
  // Block of completed goals.
  $view = new view();
  $view->name = 'goals_completed_by_user';
  $view->description = 'A simple block of goal images for goals the current user has completed.';
  $view->tag = 'Goals';
  $view->base_table = 'goals';
  $view->human_name = 'Goals Completed by User';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Your completed goals';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Goal: Goal - completed by users */
  $handler->display->display_options['relationships']['goals_completed_goals']['id'] = 'goals_completed_goals';
  $handler->display->display_options['relationships']['goals_completed_goals']['table'] = 'goals';
  $handler->display->display_options['relationships']['goals_completed_goals']['field'] = 'goals_completed_goals';
  /* Field: Goal: Goal Image */
  $handler->display->display_options['fields']['goal_img']['id'] = 'goal_img';
  $handler->display->display_options['fields']['goal_img']['table'] = 'field_data_goal_img';
  $handler->display->display_options['fields']['goal_img']['field'] = 'goal_img';
  $handler->display->display_options['fields']['goal_img']['label'] = '';
  $handler->display->display_options['fields']['goal_img']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['goal_img']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['goal_img']['settings'] = array(
    'image_style' => 'goal_image',
    'image_link' => '',
  );
  /* Contextual filter: Goal - completed by users: User */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'goals_completed_goals';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'goals_completed_goals';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

  $views[$view->name] = $view;

  return $views;
}

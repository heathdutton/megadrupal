<?php

/**
 * @file
 * This file contains all the functions necessary for the admin
 * pages of the reviews system.
 */

/**
 * Form builder for the main settings page.
 */
function reviews_settings($form, &$form_state) {
  $form = array();

  $form['main'] = array(
    '#type' => 'fieldset',
    '#title' => 'Enable/Disable',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['main']['reviews_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('reviews_enabled', ''),
    '#description' => t('Turn the review system on or off. When the reviews system is turned off all existing reviews data is retained but the ability to leave or view reviews is disabled.'),
  );

  $form['main']['reviews_sort_order'] = array(
    '#type' => 'select',
    '#title' => t('Sort order for displaying reviews'),
    '#options' => array(0 => 'Oldest first.', 1 => 'Newest first.'),
    '#default_value' => variable_get('reviews_sort_order', 0),
  );

  $form['main']['reviews_allow_multiple'] = array(
    '#type' => 'select',
    '#title' => t('Allow multiple reviews per content item from a single user'),
    '#options' => array(0 => 'No', 1 => 'Yes'),
    '#default_value' => variable_get('reviews_allow_multiple', 0),
  );

  $form['main']['reviews_link_type'] = array(
    '#type' => 'select',
    '#title' => t('Add Review Link Type'),
    '#description' => t('Show Add Review link as a node tab or a link at the bottom of the content.'),
    '#options' => array('tab' => t('Tab'), 'link' => t('Link')),
    '#default_value' => variable_get('reviews_link_type', 'tab'),
  );

  $form['main']['reviews_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Reviews per Page'),
    '#description' => t('Number of reviews to show per page, enter 0 to show all reviews without pagination.'),
    '#size' => 5,
    '#required' => FALSE,
    '#default_value' => variable_get('reviews_per_page', 0),
  );

  $form['main']['reviews_notify_admin'] = array(
    '#type' => 'select',
    '#title' => t('Notify Admin'),
    '#description' => t('Notify the site administrator when a review is left.'),
    '#options' => array(0 => 'No', 1 => 'Yes'),
    '#default_value' => variable_get('reviews_notify_admin', 0),
  );

  $form['rating'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rating Field'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['rating']['reviews_use_rating'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Rating field'),
    '#description' => t('Allow users to leave Fivestar ratings with their reviews.'),
    '#default_value' => variable_get('reviews_use_rating', 0) && module_exists('fivestar'),
  );

  if (! module_exists('fivestar')){
    $form['rating']['reviews_use_rating']['#disabled'] = TRUE;
    $form['rating']['reviews_use_rating']['#description'] .= "<br>".t('Disabled because the Fivestar module is not enabled.');
  }

  if(module_exists('fivestar')){

    $state = array(
      'visible' => array(
        ':input[name="reviews_use_rating"]' => array('checked' => TRUE),
      ),
    );

    $star_range = range(0,100);
    unset($star_range[0]);

    $form['rating']['reviews_rating_star_count'] = array(
      '#type' => 'select',
      '#title' => t('Number of Stars'),
      '#description' => t('Maximum number of stars in a rating.'),
      '#options' => $star_range,
      '#default_value' => variable_get('reviews_rating_star_count', 5),
      '#states' => $state,
    );

    $widgets = module_invoke_all('fivestar_widgets');

    $form['rating']['reviews_rating_star_widget'] = array(
      '#type' => 'select',
      '#title' => t('Star Widget'),
      '#description' => t('Star widget to use for displaying a rating.'),
      '#options' => array('default' => 'Default') + $widgets,
      '#default_value' => variable_get('reviews_rating_star_widget', 'Default'),
      '#states' => $state,
    );
  }

  $form['ctypes'] = array(
    '#type' => 'fieldset',
    '#title' => 'Reviewable Content Types',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['ctypes']['reviews_enabled_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available Content Types'),
    '#options' => reviews_get_node_types(),
    '#default_value' => variable_get('reviews_enabled_content_types', array()),
  );

  $form['moderation'] = array(
    '#type' => 'fieldset',
    '#title' => 'Moderation Settings',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['moderation']['reviews_moderate_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Do Not Moderate'),
    '#description' => t('<em>WARNING:</em> Reviews posted against any of the checked content types above will be published <em>immediately</em>.'),
    '#options' => reviews_get_node_types(),
    '#default_value' => variable_get('reviews_moderate_content_types', array()),
  );

  $form['#submit'][] = 'reviews_set_block_content_types';
  $form['#submit'][] = 'reviews_cache_clear';

  return system_settings_form($form);
}

/**
 * Additional submit handler to rebuild menu after saving reviews settings.
 */
function reviews_cache_clear($form, &$form_state) {
  variable_set('reviews_link_type', $form_state['values']['reviews_link_type']);
  menu_rebuild();
  cache_clear_all();
}

/**
 * Additional submit handler for setting forms.
 */
function reviews_set_block_content_types($form, &$form_state) {
  $types = $form_state['values']['reviews_enabled_content_types'];

  $result = db_delete('block_node_type')
    ->condition('module', 'reviews')
    ->condition('delta', 'reviews')
    ->execute();

  foreach ($types as $key => $type) {
    if ($type != '0') {
      db_insert('block_node_type')
        ->fields(array(
          'module' => 'reviews',
          'delta' => 'reviews',
          'type' => $type,
        ))
        ->execute();
    }
  }
}

/**
 * Form builder for reviews moderation.
 */
function reviews_list($form, &$form_state) {
  $form = array();

  // Check for confirmation forms.
  if (isset($form_state['confirm_delete'])) {
    return array_merge($form, reviews_list_confirm_delete($form, $form_state));
  }

  $result = db_select('reviews', 'r')->extend('PagerDefault')->limit(15)
    ->fields('r')
    ->orderBy('created', 'ASC')
    ->execute();

  $header = array(
    'rid' => array('data' => t('Review ID')),
    'node_title' => array('data' => t('Reviewed Content Title')),
    'username' => array('data' => t('Reviewer')),
    'review' => array('data' => t('Review')),
    'status' => array('data' => t('Status')),
    'actions' => array('data' => t('Operations')),
  );

  $data = array();
  while ($review = $result->fetchAssoc()) {
    $review_content = unserialize($review['review']);

    $links['items'] = array(
      l(t('view'), 'node/' . $review['nid'] . '/reviews', array('fragment' => 'review_' . $review['rid'])),
    );

    $data[$review['rid']] = array(
      'rid' => $review['rid'],
      'node_title' => check_plain(reviews_get_node_title($review['nid'])),
      'username' => check_plain(reviews_get_username($review['uid'])),
      'review' => check_markup($review_content['value'], $review_content['format']),
      'status' => reviews_get_review_status($review['status']),
      'actions' => theme('item_list', $links),
    );
    if(variable_get('reviews_use_rating')) {
      $data[$review['rid']]['rating'] = $review['rating'];
    }
  }

  $form['reviews'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $data,
    '#empty' => t('There are currently no reviews.'),
  );

  $form['pager'] = array(
    '#markup' => theme('pager', array('tags' => array())),
  );

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => user_access('administer reviews'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Confirm handler for review_list form.
 */
function reviews_list_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == t('Delete')) {
    // Delete the selected reviews.
    if ($form_state['values']['delete'] === TRUE) {
      return reviews_list_confirm_delete_submit($form, $form_state);
    }
    // Rebuild the form to confirm review deletion.
    $form_state['rebuild'] = TRUE;
    $form_state['confirm_delete'] = TRUE;
    return;
  }
}

/**
 * Confirm handler for the reviews_list form.
 */
function reviews_list_confirm_delete($form, &$form_state) {
  $form['reviews'] = array('#type' => 'value', '#value' => $form_state['values']['reviews']);
  $form['delete'] = array('#type' => 'value', '#value' => TRUE);
  return confirm_form($form,
    t('Are you sure you want to delete the selected reviews'),
    'admin/structure/reviews/list',
    t('Warning: This action cannot be undone and all deleted reviews will be permanently lost.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * This function deletes the selected reviews
 */
function reviews_list_confirm_delete_submit($form, $form_state) {
  $reviews = $form_state['values']['reviews'];
  foreach ($reviews as $key => $review) {
    $nid = reviews_get_nid_from_rid($review);
    db_delete('reviews')
      ->condition('rid', $review, '=')
      ->execute();
    if ($nid) {
      reviews_update_review_count_for_node($nid);
    }
  }
  drupal_set_message(t('The selected reviews have been deleted.'), 'status');
}

/**
 * Form builder for reviews moderation.
 */
function reviews_moderation($form, &$form_state) {
  $form = array();

  $result = db_select('reviews', 'r')->extend('PagerDefault')->limit(15)
    ->fields('r')
    ->condition('status', 0, '=')
    ->orderBy('created', 'ASC')
    ->execute();

  $header = array(
    'rid' => array('data' => t('Review ID')),
    'node_title' => array('data' => t('Reviewed Content Title')),
    'username' => array('data' => t('Reviewer')),
    'review' => array('data' => t('Review')),
    'rating' => array('data' => t('Rating')),
    'actions' => array('data' => t('Operations')),
  );

  if (!variable_get('reviews_use_rating')) {
    unset($header['rating']);
  }

  $data = array();
  while ($review = $result->fetchAssoc()) {
    $review_content = unserialize($review['review']);
    $data[$review['rid']] = array(
      'rid' => $review['rid'],
      'node_title' => check_plain(reviews_get_node_title($review['nid'])),
      'username' => check_plain(reviews_get_username($review['uid'])),
      'review' => check_markup($review_content['value'], $review_content['format']),
      'rating' => $review['rating'],
      'actions' => l(t('view'), 'node/' . $review['nid'] . '/reviews', array('fragment' => 'review_' . $review['rid'])) . ' / ' . l(t('approve'), 'admin/content/reviews/approve/' . $review['rid']),
    );
    if (!variable_get('reviews_use_rating')) {
      unset($header['rating']);
    }
  }

  $form['reviews'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $data,
    '#empty' => t('There are currently no reviews awaiting moderation.'),
  );

  $form['pager'] = array(
    '#markup' => theme('pager', array('tags' => array())),
  );

  return $form;
}

/**
 * Function for menu callback to approve a review.
 */
function reviews_approve_review($rid) {
  // Update the review status to approved.
  db_update('reviews')
    ->fields(array('status' => 1))
    ->condition('rid', $rid, '=')
    ->execute();

  // Update the reviews count table.
  $nid = reviews_get_nid_from_rid($rid);
  if ($nid) {
    reviews_update_review_count_for_node($nid);
  }

  drupal_set_message(t('Review approved'), 'status');
  drupal_goto('admin/content/reviews/moderate');
}

<?php

/**
 * @file
 * Contains the administrative functions of the Shout Box module.
 *
 * This file is included by the Shout box admin settings form.
 */

/**
 * Definne _drupal_wall_global().
 */
function _drupal_wall_global() {
  
  $output = "";
  // Create system variable for older post dispaly
  variable_set ('drupal_wall_global_post_offset', 0);
  variable_set ('drupal_wall_content_type', 0);
  // Get drupal wall post global form
  $_drupal_wall_content_post_form = drupal_get_form ('_drupal_wall_content_post_form');
  $drupal_wall_post_global_form = drupal_render ($_drupal_wall_content_post_form);
  $output .= $drupal_wall_post_global_form;
  // Get drupal global wall post content
  $drupal_wall_global_post = _drupal_wall_get_global_wall_post ();
  $output .= '<div class = "drupal-wall-global-post"> Wall Post </div>';
  $output .= '<fieldset id="drupal-wall-global-post" class="form-wrapper">';
  if ($drupal_wall_global_post) {
    $output .= $drupal_wall_global_post;
    // Get older post button
    $_drupal_wall_global_post_display =  drupal_get_form ('_drupal_wall_global_post_display');
    $drupal_wall_global_post = drupal_render ($_drupal_wall_global_post_display);
    $output .= $drupal_wall_global_post;
  }
  else {
    $output .= '<div class="drupal_wall_global_outer"><span class="status_msg">No wall status avaliable !</span></div>';
  }
  $output .= '</fieldset>';
 
  return $output;
}

/**
 * Define _drupal_wall_get_global_wall_post().
 *
 * @return array Merge array of wall post with its related comment and FALSE if it isn't.
 */
function _drupal_wall_get_global_wall_post() {
  $query = db_select ('node', 'n');
  $query->leftjoin ('field_data_body', 'b', 'n.nid=b.entity_id');
  $query = $query->fields ('n', array (
      'nid',
      'uid',
      'created',
      'comment',
      'title',
      'type' 
  ))->fields ('b', array (
      'body_value' 
  ))->condition ('n.status', 1);
  
  if (variable_get ('drupal_wall_content_type') == 0) {
    $query = $query->condition ('n.type', 'drupal_wall');
  }
  
  $query = $query->orderby ('n.created', 'desc')->range ((0 + variable_get ('drupal_wall_global_post_offset')), variable_get ('drupal_wall_global_post_limit'));
  if (variable_get ('drupal_wall_post_type_photo') == 1) {
    $query->leftjoin ('field_data_field_drupal_wall_photos', 'p', 'n.nid=p.entity_id');
    $query = $query->fields ('p', array (
        'field_drupal_wall_photos_fid' 
    ));
    $query->leftjoin ('field_data_field_drupal_wall_image_style', 'ps', 'n.nid=ps.entity_id');
    $query = $query->fields ('ps', array (
        'field_drupal_wall_image_style_value' 
    ));
  }
  
  if (variable_get ('drupal_wall_post_type_video') == 1) {
    $query->leftjoin ('field_data_field_drupal_wall_videos', 'v', 'n.nid = v.entity_id');
    $query = $query->fields ('v', array (
        'field_drupal_wall_videos_value' 
    ));
  }
  $query = $query->execute ()->fetchAll ();
  
  if (variable_get ('drupal_wall_show_comments') == 1) {
    $query2 = db_select ('comment', 'c');
    $query2->leftjoin ('field_data_comment_body', 'cb', 'c.cid = cb.entity_id');
    $query2 = $query2->fields ('c', array (
        'nid',
        'cid',
        'name',
        'uid',
        'subject',
        'created' 
    ))->fields ('cb', array (
        'comment_body_value' 
    ))->condition ('c.status', 1)->execute ()->fetchAll ();
    
    // Merging comments to it node in comment array.
    foreach ($query as $key => $value) {
      foreach ($query2 as $key2 => $value2) {
        if ($value->nid == $value2->nid) {
          $value->comments[] = $value2;
        }
      }
    }
  }
  
  $size = variable_get ('drupal_wall_global_post_limit') + variable_get ('drupal_wall_global_post_offset');
  variable_set ('drupal_wall_global_post_offset', $size);
  if (array_key_exists ('0', $query)) {
    $html = theme ('drupal_wall_posts', array (
        'wall_post' => $query,
        'user_info' => '' 
    ));
    return $html;
  }
  else {
    return NULL;
  }
}

/**
 * Define _drupal_wall_global_post_display().
 */
function _drupal_wall_global_post_display($form, &$form_state) {
  
  $form = array ();
  if (variable_get ('drupal_wall_older_post_button') == 1) {
    $form['drupal_wall_older_post'] = array (
        '#value' => t ('Show older posts'),
        '#type' => 'submit',
        '#prefix' => '<div style="margin:20px; clear:both;">',
        '#suffix' => '</div>',
        '#ajax' => array (
            'callback' => '_drupal_wall_global_post_display_ajax',
            'wrapper' => 'drupal_wall_append_older_wall_post_global',
            'method' => 'append' 
        ) 
    );
  }
  return $form;
}

/**
 * Define _drupal_wall_global_post_display_ajax().
 */
function _drupal_wall_global_post_display_ajax() {
  
  $html = NULL;
  if (variable_get ('drupal_wall_global_post_offset') != - 1) {
    $html = _drupal_wall_get_global_wall_post ();
  }
  if ($html != NULL) {
    return $html;
  }
  elseif (variable_get ('drupal_wall_global_post_offset') != - 1) {
    variable_set ('drupal_wall_global_post_offset', - 1);
    return '<p style="color:red; margin:25px;">No more older wall post exists !</p>';
  }
}

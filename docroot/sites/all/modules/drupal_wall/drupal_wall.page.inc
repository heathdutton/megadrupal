<?php

/**
 * @file
 * Contains the logical functions of the Drupal wall module.
 */

/**
 * Drupal wall form for post input textarea and post button.
 */
function _drupal_wall_content_post_form($form, &$form_state) {
  
  if(!user_access('create post on drupal wall')) return;
  global $base_url;
  
  $form['drupal_wall_status'] = array (
      '#type' => variable_get ('drupal_wall_textbox_type'),
      '#format' => 'filtered_html',
      '#required' => FALSE,
      '#attributes' => array (
          'placeholder' => t (variable_get ('drupal_wall_what_is_on_your_mind_string_post_box','What\'s on your mind ?')) 
      ),
      '#prefix' => '<div class="drupal_wall_post_status">' 
  );
  
  $media_share['none'] = '-none-';
  if (variable_get ('drupal_wall_post_type_photo') == 1) {
    $media_share['photo'] = 'Add Photo';
  }
  if (variable_get ('drupal_wall_post_type_video') == 1) {
    $media_share['video'] = 'Share Video';
  }
  
  if (variable_get ('drupal_wall_post_type_photo') == 1 || variable_get ('drupal_wall_post_type_video') == 1) {
    $form['drupal_wall_photo_video_enable'] = array (
        '#type' => 'select',
        '#title' => t ('Share'),
        '#options' => $media_share,
        '#states' => array (
            'visible' => array (
                ':input[name="drupal_wall_group_redirect"]' => array (
                    'value' => '-1' 
                ) 
            ) 
        ) 
    );
  }
  
  // Hiding image option if type photo is disable.
  if (variable_get ('drupal_wall_post_type_photo') == 1) {
    // For image style DDL
    $form['drupal_wall_image_style'] = array (
          '#type' => 'select',
          '#title' => t ('Photo Style'),
          '#options' => drupal_wall_image_styles_ddl(),
          '#states' => array (
              'visible' => array (
                  ':input[name="drupal_wall_photo_video_enable"]' => array (
                      'value' => 'photo' 
                  ) 
              ) 
          ),
          '#description' => t ('Select image style you want apply effects on photo, Default image style will be thumbnail.') 
    );
    $form['drupal_wall_photo_status'] = array (
        '#type' => 'managed_file',
        '#upload_location' => 'public://drupal_wall//',
        '#upload_validators' => array (
            'file_validate_extensions' => array (
                'gif png jpg jpeg' 
            ) 
        ),
        '#states' => array (
            'visible' => array (
                ':input[name="drupal_wall_photo_video_enable"]' => array (
                    'value' => 'photo' 
                ) 
            ) 
        ) 
    );
  }
  
  // Hiding video option if type video is disable.
  if (variable_get ('drupal_wall_post_type_video') == 1) {
    $form['drupal_wall_video_status'] = array (
        '#type' => 'textfield',
        '#description' => t ('Please enter the youtube video URL. Example:') . ' http://www.youtube.com/watch?v=vw-G-adwRNU',
        '#states' => array (
            'visible' => array (
                ':input[name="drupal_wall_photo_video_enable"]' => array (
                    'value' => 'video' 
                ) 
            ) 
        ) 
    );
  }
  $form['drupal_wall_status_post'] = array (
      '#value' => t ('Post'),
      '#type' => 'submit',
      '#suffix' => '</div>',
      '#attributes' => array (
          'class' => array (
              'post_btn' 
          ) 
      ) 
  );
  return $form;
}

/**
 * Validate form for Drupal wall post box.
 */
function _drupal_wall_content_post_form_validate($form, &$form_state) {
  
  if (empty ($form_state['values']['drupal_wall_status'])) {
    drupal_set_message (t ('Sorry ! Nothing has shared to post.'), 'warning');
  }
  if (isset ($form_state['values']['drupal_wall_photo_video_enable']) && $form_state['values']['drupal_wall_photo_video_enable'] == 'video') {
    if (isset ($form_state['values']['drupal_wall_video_status']) && $form_state['values']['drupal_wall_video_status'] != '') {
      $video_url = $form_state['values']['drupal_wall_video_status'];
      $pos_v = strrpos ($video_url, 'v=', - 1);
      if ((! $pos_v)) {
        form_set_error ('drupal_wall_video_status', t ('Sorry ! Not able to recognize the Youtube video, please enter valid video URL.'));
      }
      if (stristr ($video_url, 'www.youtube.com/') === FALSE) {
        form_set_error ('drupal_wall_video_status', t ('Sorry ! Only Youtube video are allowed to shared, Please enter valid video URL'));
      }
    }
  }
}

/**
 * Submit form to save the post status content (save node).
 */
function _drupal_wall_content_post_form_submit($form, &$form_state) {
  if (variable_get ('drupal_wall_textbox_type') == 'text_format') {
    $status_msg = $form_state['values']['drupal_wall_status']['value'];
  }
  else {
    $status_msg = $form_state['values']['drupal_wall_status'];
  }
  $video_exist = FALSE;
  $photo_exist = FALSE;
  $msg = NULL;
  
  if (isset ($form_state['values']['drupal_wall_photo_video_enable'])) {
    if ($form_state['values']['drupal_wall_photo_video_enable'] == 'video' && isset ($form_state['values']['drupal_wall_video_status']) && $form_state['values']['drupal_wall_video_status'] != '') {
      $video_status = $form_state['values']['drupal_wall_video_status'];
      $video_exist = TRUE;
      $msg = 'Shared a new video';
    }
    elseif ($form_state['values']['drupal_wall_photo_video_enable'] == 'photo' && isset ($form_state['values']['drupal_wall_photo_status']) && $form_state['values']['drupal_wall_photo_status'] != '') {
      $photo_status = $form_state['values']['drupal_wall_photo_status'];
      $photo_exist = TRUE;
      $msg = 'Added a new photo';
    }
  }
  
  if ($status_msg != '' || $photo_exist || $video_exist) {
    
    if ($form_state['values']['form_id'] == '_drupal_wall_content_post_form') {
      global $user;
      $new_node = new stdClass ();
      
      $new_node->type = 'drupal_wall';
      node_object_prepare ($new_node);
      $new_node->uid = ! empty ($user->uid) ? $user->uid : 0;
      $new_node->name = ! empty ($user->name) ? $user->name : variable_get ('anonymous', t ('Anonymous'));
      $new_node->language = 'und';
      $new_node->body[$new_node->language][0]['value'] = $status_msg;
      //$new_node->body[$new_node->language][0]['format'] = 'full_html';
      
      // Taking first 50 char of status_msg as title.
      
      $status_msg_title = truncate_utf8($status_msg, 50, $wordsafe = TRUE, $add_ellipsis = TRUE, $min_wordsafe_length = 1);
      
      // Saving image to photo field in node.
      if ($photo_exist) {
        $image_id = $photo_status;
        if (isset ($image_id) && $image_id != NULL) {
          // Load the file via file.fid.
          $file = file_load ($image_id);
          if (is_object ($file)) {
            $file->status = FILE_STATUS_PERMANENT;
            file_save ($file);
            file_usage_add ($file, 'drupal_wall', 'image', $image_id);
            
            /* $image_path = file_create_url ($file->uri);
            $image = (object) array (
                'uid' => $user->uid,
                'uri' => $file->uri,
                'filemime' => file_get_mimetype ($image_path),
                'status' => 1 
            ); */
            //if ($image = file_move ($image, 'public://drupal_wall//')) { 
            if ( strpos($file->uri, 'drupal_wall') && file_exists($file->uri)) {
               $new_node->field_drupal_wall_photos[$new_node->language][0] = (array) $file;
            }
            else {
              form_set_error ('drupal_wall_photo_status', t ('Failed to write the uploaded file in "sites/default/" file folder. Please provide write permission to this directory'));
            }
          }
        }
        // Add image style
        if (isset($form_state['values']['drupal_wall_image_style'])) {
          if ($form_state['values']['drupal_wall_image_style'] != '_none') {
            $new_node->field_drupal_wall_image_style[$new_node->language][0]['value'] = $form_state['values']['drupal_wall_image_style'];
          }
        /* else {
            $new_node->field_drupal_wall_image_style[$new_node->language][0]['value'] = '_none';
          } */
        }
      }
      
      // Save video URL to video field of node.
      if ($video_exist) {
        $video_url = $video_status;
        $pos_v = strrpos ($video_url, 'v=', - 1);
        if ($pos_v) {
          $video_url = 'http://www.youtube.com/embed/' . drupal_substr ($video_url, $pos_v + 2);
          $new_node->field_drupal_wall_videos[$new_node->language][0]['value'] = $video_url;
        }
      }
      
      $new_node->language = 'und';
      $new_node->title = $status_msg_title;
      
      $new_node->comment = 2;
      $new_node->status = 1;
      // Get content type default setting.
      $node_options = variable_get ('node_options_' . $new_node->type);
      $promote = array_key_exists ('1', $node_options);
      $new_node->promote = ! empty ($promote) ? $promote : 0;
      $new_node->revision = 0;
      node_submit ($new_node);
      node_save ($new_node);
      drupal_set_message (t ('Your post has been saved !'));
    }
  }
}

/**
 * Drupal wall Comment form for user comment posts.
 */
function _drupal_wall_comment_post_form($form_state, $args) {
  
  global $base_url;
  // Comment display form.
  $form['drupal_wall_comment'] = array (
      '#type' => 'textarea',
      '#resizable' => FALSE,
      '#attributes' => array (
          'placeholder' => ' ' . t ('write a comment...') 
      ),
      '#required' => TRUE,
      '#prefix' => '<div style="margin-bottom:-8px;">',
      '#suffix' => '</div>' 
  );
  $form['drupal_wall_comment_nid'] = array (
      '#type' => 'hidden',
      '#value' => $args['build_info']['args']['0'] 
  );
  
  $form['drupal_wall_comment_uid'] = array (
      '#type' => 'hidden',
      '#value' => $args['build_info']['args']['1'] 
  );
  
  $form['drupal_wall_submit'] = array (
      '#type' => 'button',
      '#value' => t ('Post'),
      '#ajax' => array (
          'callback' => '_drupal_wall_comment_ajax_form_post',
          'wrapper' => 'div_append_next_user_comment_' . $args['build_info']['args']['0'],
          'method' => 'append',
          'effect' => 'none' 
      ),
      '#prefix' => '<div style="margin-bottom:-10px;">',
      '#suffix' => '</div>' 
  );
  
  $form['#action'] = url ('user/' . variable_get ('drupal_wall_user_id'));
  $form['#submit'] = array (
      '_drupal_wall_comment_ajax_form_post' 
  );
  return $form;
}

/**
 * Ajax call to save users comments for its respective node.
 */
function _drupal_wall_comment_ajax_form_post($form, $form_state) {
  
  global $base_url;
  global $user;
  $comment_value = trim (strip_tags ($form_state['values']['drupal_wall_comment']));
  
  if ($comment_value != '') {
    
    // Taking first 50 char of status_msg as title.
    $comment_msg = truncate_utf8($comment_value, 49, $wordsafe = TRUE, $add_ellipsis = TRUE, $min_wordsafe_length = 1);
    
    $comment = new stdClass ();
    
    $comment->nid = $form_state['input']['drupal_wall_comment_nid'];
    $comment->cid = 0;
    $comment->pid = 0;
    $comment->uid = $user->uid;
    $comment->mail = $user->mail;
    
    $comment->created = time ();
    $comment->is_anonymous = 0;
    $comment->homepage = '';
    $comment->status = COMMENT_PUBLISHED;
    $comment->language = LANGUAGE_NONE;
    
    $comment->subject = $comment_msg;
    $comment->comment_body[$comment->language][0]['value'] = $comment_value;
    $comment->comment_body[$comment->language][0]['format'] = 'filtered_html';
    
    // Saving a comment.
    $new_comment = comment_submit ($comment);
    $cid = comment_save ($comment);
    
    if (is_numeric ($new_comment->cid)) {
      // Returning comment result in its respective comment block.
      $html = '<div class="comment" id="drupal_wall_comment_cid_' . $new_comment->cid . '"><div class="comment_left"><a href="' . $base_url . '/user/' . $user->uid . '"><img src="' . _drupal_wall_user_profile_picture ($user->uid) . '" width="32px"></a></div><div class="comment_right"><div class="comment_delete">' . drupal_render (drupal_get_form ('_drupal_wall_delete_comment_form', $new_comment->cid, $user->uid)) . '</div><strong><a href="' . $base_url . '/user/' . $user->uid . '">' . ucwords ($user->name) . '</a>&nbsp;</strong>' . $comment_value . '<div class="caption">' . date ('F j, Y', time ()) . ' at ' . date ('h:ia', time ()) . '</div></div></div>';
      
      $form_state['values']['drupal_wall_comment'] = '';
      return $html;
    }
    else {
      return '<span style="color:red">No able to save comment</span>';
    }
  }
}

/**
 * Drupal wall Delete form for user to delete post.
 */
function _drupal_wall_delete_edit_node_form($form_state, $args) {
  $form = array ();
  global $base_url;
  global $user;
  $drupal_wall_path = drupal_get_path ('module', 'drupal_wall');
  
  if (variable_get ('drupal_wall_edit_post_button') == 1) {
    $img = '<img src="' . $base_url . '/' . $drupal_wall_path . '/images/edit-icon.png' . '" >';
    $redirect = $base_url . '/node/' . $args['build_info']['args']['0'] . '/edit?destination=user/' . $user->uid;
    $form['drupal_wall_edit_post'] = array (
        '#markup' => l ($img, $redirect, array (
            'attributes' => array (
                'class' => 'anchor-class' 
            ),
            'html' => TRUE 
        )) 
    );
  }
  if (variable_get ('drupal_wall_delete_post_button') == 1) {
    $form['drupal_wall_delete_post'] = array (
        '#value' => t ('Delete Button'),
        '#type' => 'image_button',
        '#src' => $base_url . '/' . $drupal_wall_path . '/images/delete-icon.png',
        '#attributes' => array (
            'class' => array (
                'delete_btn' 
            ) 
        ),
        '#ajax' => array (
            'callback' => '_drupal_wall_delete_node_post',
            'wrapper' => 'drupal_wall_post_nid_' . $args['build_info']['args']['0'],
            'method' => 'replace',
            'event' => 'click' 
        ) 
    );
  }
  
  if (variable_get ('drupal_wall_delete_post_button') == 1 || variable_get ('drupal_wall_edit_post_button') == 1) {
    $form['drupal_wall_post_nid'] = array (
        '#type' => 'hidden',
        '#value' => $args['build_info']['args']['0'] 
    );
    
    $form['drupal_wall_post_uid'] = array (
        '#type' => 'hidden',
        '#value' => $args['build_info']['args']['1'] 
    );
    $form['#action'] = url ('user/' . variable_get ('drupal_wall_user_id'));
  }
  return $form;
}

/**
 * Delete form for user to delete its post.
 */
function _drupal_wall_delete_node_post($form, $form_state) {
  
  $nid = $form_state['input']['drupal_wall_post_nid'];
  global $user;
  $node = node_load($nid);
  $type = is_string($node) ? $node : $node->type;
  // Deleting all content for node 'nid'.
  if (user_access('delete any ' . $type . ' content', $user) || (user_access('delete own ' . $type . ' content', $user) && ($user->uid == $node->uid))) {
    node_delete ($nid);
  }
  // Replace with null (hiding).
  return '';
}

/**
 * Drupal wall delete form for user to delete post.
 *
 * @param array $form
 *          Drupal submit form array.
 *          
 * @param string $args
 *          Arguments send to function.
 *          
 * @return array Drupal submited form content array.
 */
function _drupal_wall_delete_comment_form($form, $args) {
  global $base_url;
  global $user;
  $drupal_wall_path = drupal_get_path ('module', 'drupal_wall');
  $form = array ();
  
  // Delete buuton visible only to user who post that comment.
  if ($args['build_info']['args']['1'] == $user->uid && variable_get ('drupal_wall_delete_post_button') == 1) {
    $form['drupal_wall_delete_comment_' . $args['build_info']['args']['0']] = array (
        '#value' => t ('Delete Comment'),
        '#type' => 'image_button',
        '#src' => $base_url . '/' . $drupal_wall_path . '/images/delete-icon.png',
        '#action' => url ('user/' . variable_get ('drupal_wall_user_id')),
        '#ajax' => array (
            'callback' => '_drupal_wall_delete_comment_form_submit',
            'wrapper' => 'drupal_wall_comment_cid_' . $args['build_info']['args']['0'],
            'method' => 'replace',
            'event' => 'click' 
        ),
        '#attributes' => array (
            'class' => array (
                'delete_btn' 
            ) 
        ) 
    );
    
    $form['drupal_wall_comment_cid'] = array (
        '#type' => 'hidden',
        '#value' => $args['build_info']['args']['0'] 
    );
    
    $form['drupal_wall_comment_uid'] = array (
        '#type' => 'hidden',
        '#value' => $args['build_info']['args']['1'] 
    );
    $form['#action'] = url ('user/' . variable_get ('drupal_wall_user_id'));
  }
  return $form;
}

/**
 * Delete form for user to delete its Comment.
 *
 * @param array $form
 *          Drupal submit form array.
 *          
 * @param string $form_state
 *          Drupal form state array.
 *          
 * @return array Delete comment node and return nothing.
 */
function _drupal_wall_delete_comment_form_submit($form, $form_state) {
  $cid = $form_state['input']['drupal_wall_comment_cid'];
  
  // Deleting all content for node Comment via cid.
  comment_delete ($cid);
  // Replace with null (hiding).
  return '';
}

/**
 * Define drupal_wall_image_styles().
 *
 * @return An array of styles.
 * @see image_style_load().
 */
function drupal_wall_image_styles() {
  // Select the module-defined styles.
  foreach (module_implements ('image_default_styles') as $module) {
    $module_styles = module_invoke ($module, 'image_default_styles');
    foreach ($module_styles as $style_name => $style) {
      if (empty($style['label'])) {
        $width = ! empty ($style['effects'][0]['data']['width']) ? $style['effects'][0]['data']['width'] : '';
        $height = ! empty ($style['effects'][0]['data']['height']) ? $style['effects'][0]['data']['height'] : '';
        if (! empty ($width) && ! empty ($height)) {
          $style['label'] = ucwords (str_replace ('_', ' ', $style_name)) . '(' . $width . 'X' . $height . ')';
        }
        else {
          $style['label'] = ucwords (str_replace ('_', ' ', $style_name));
        }
      }
      $drupal_wall_image_styles[$style_name] = $style['label'];
    }
  }
  // Select all the user-defined styles.
  $user_styles = db_select ('image_styles', NULL, array (
      'fetch' => PDO::FETCH_ASSOC 
  ))->fields ('image_styles')->orderBy ('name')->execute ()->fetchAllAssoc ('name', PDO::FETCH_ASSOC);
  foreach ($user_styles as $style_name => $style) {
    $style['effects'] = image_style_effects ($style);
    $key = $style['isid'];
    $width = ! empty ($style['effects'][$key]['data']['width']) ? $style['effects'][$key]['data']['width'] : '';
    $height = ! empty ($style['effects'][$key]['data']['height']) ? $style['effects'][$key]['data']['height'] : '';
    
    if (! empty ($width) && ! empty ($height)) {
      $drupal_wall_image_styles[$style_name] = ucwords (str_replace ('_', ' ', $style_name)) . '(' . $width . 'X' . $height . ')';
    }
    else {
      $drupal_wall_image_styles[$style_name] = ucwords (str_replace ('_', ' ', $style_name));
    }
  }
  return $drupal_wall_image_styles;
}

/**
 * Define drupal_wall_image_styles_ddl().
 *
 * @return An array of styles.
 * @see image_style_load().
 */
function drupal_wall_image_styles_ddl() {
   // Get allow image style from admin
   $image_styles_list = array ();
   $image_styles_list = variable_get ('allow_image_style_options');
   $allow_image_options = array ();
   $drupal_wall_image_style_options = array (
          '_none' => '-None-' 
   );
   if (! empty ($image_styles_list)) {
     foreach ($image_styles_list as $allow_image_styles_key => $allow_image_styles_val) {
       if ($allow_image_styles_val != '0') {
         $allow_image_options[$allow_image_styles_val] = $allow_image_styles_val;
       }
     }
     // Option list for front end user image style
     foreach (drupal_wall_image_styles () as $image_styles_key => $image_styles_val) {
       if (array_key_exists ($image_styles_key, $allow_image_options)) {
         $drupal_wall_image_style_options[$image_styles_key] = $image_styles_val;
       }
     }
   }
  return $drupal_wall_image_style_options;
}
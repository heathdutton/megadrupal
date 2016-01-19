<?php

/**
 * @file
 * Contains the logical functions of the Drupal wall module.
 */

function _commune_attachment_form_files() {
  $attaches = array();
  $commune_path = drupal_get_path('module','commune');
  /* remove embedly support to other file. 
  $enable = variable_get('commune_embed_use_embedly');
  if($enable) {
    $attaches['js'] = array(
      'http://cdn.embed.ly/jquery.embedly-3.1.1.min.js' => array(
                 'type'=>'external'),
      'http://cdn.embed.ly/jquery.preview-0.3.2.min.js' => array(
                 'type'=>'external'),
    );
    $attaches['css'] = array(
       'http://cdn.embed.ly/jquery.preview-0.3.2.css' => array(
                 'type'=>'external'),
    );
  }
  */

  $attaches['js'][] = $commune_path . '/js/jquery.mentionsInput.js';
  $attaches['js'][] = $commune_path . '/js/commune-post.js';
  $attaches['css'][] = $commune_path . '/css/jquery.mentionsInput.css';
  return $attaches;
}

/**
 * Drupal wall form for post input textarea and post button.
 */
function _commune_content_post_form($form, &$form_state, $subject_id) {

  /* only return the form if the user has access to this.  */
  if(!user_access('create commune_post content')) {
     return null;
  }

  global $base_url;

  $form['commune_subject_id'] = array(
      '#type' => 'hidden',
      '#value' => $subject_id,
  );

  $form['#attributes']['class'] = 'commune-post-form';

  $commune_path = drupal_get_path ('module', 'commune');
  $form['#attached'] = _commune_attachment_form_files();
  $form['commune_status'] = array (
      '#type' => variable_get ('commune_textbox_type', 'textarea'),
      '#format' => 'filtered_html',
      '#required' => FALSE,
      '#attributes' => array (
          'placeholder' => t ("What's on your mind?"),
          'class' => array('commune-post-textbox'),
      ),
      '#prefix' => '<div class="drupal_wall_post_status"><div class="commune-post-textarea">', 
      '#suffix' => '</div><div class="commune-post-actions">',
  );

  $form['commune_file'] = array(
      '#markup' => '<a class="commune-file" href="#">Upload File</a>',
      '#prefix' => '<div class="commune-action-button">',
      '#suffix' => '</div>',
  );
  
  $form['commune_link'] = array(
      '#markup' => '<a class="commune-link" href="#">Attach Link</a>',
      '#prefix' => '<div class="commune-action-button">',
      '#suffix' => '</div>',
  );

  $form['commune_status_post'] = array (
      '#value' => t ('Post'),
      '#type' => 'submit',
      '#suffix' => '<div style="clear:both;"></div></div>',
      '#attributes' => array (
          'class' => array ('post_btn commune-action-post-button'),
      ) 
  );
  
  $form['commune_file_upload_area'] = array(
    '#markup' => '<div class="commune-file-area">',
  );

  $form['commune_file_upload'] = array(
    '#type' => 'dragndrop_upload',
    '#title' => t('Drag & Drop files here'),
    '#file_upload_max_size' => '10M',
    '#upload_location' => 'public://commune',
    '#upload_validators' => array (
    	'file_validate_extensions' => array (
    	    'gif png jpg jpeg pdf wmv mp4 mp3 pdf' 
    	) 
     ),
  );
  $form['commune_file_upload_area_close'] = array(
     '#markup' => '</div>',
  );

  /* embedly support for links. */
  $form['commune_link_shared'] = array (
    '#type' => 'textfield',
    '#prefix' => '<div class="commune-link-area">',
    '#attributes' => array('class' => array('wall-embed-input')),
    '#description' => t ('Please enter the youtube video URL. Example:') . ' http://www.youtube.com/watch?v=vw-G-adwRNU',
  );
  
 $form['commune_preview_area'] = array(
     '#markup' => "<div class='wall-embed-preview selector-wrapper'></div>",
     '#suffix' => "</div>",
 );

 $form['commune_post_markup_close'] = array(
     '#markup' => '</div>'
 );

  $form['commune_post_mentions'] = array(
      '#type' => 'hidden'
  );

  $form['commune_post_markup'] = array(
      '#type' => 'hidden'
  );

  return $form;
}

/**
 * Validate form for Drupal wall post box.
 */
function _commune_content_post_form_validate($form, &$form_state) {
  if (empty ($form_state['values']['commune_status'])) {
    drupal_set_message (t ('Sorry ! Nothing has shared to post.'), 'warning');
  } 
}

/**
 * Submit form to save the post status content (save node).
 */
function _commune_content_post_form_submit($form, &$form_state) {
  if (variable_get ('commune_textbox_type') == 'text_format') {
    $status_msg = $form_state['values']['commune_status']['value'];
  }
  else {
    $status_msg = $form_state['values']['commune_status'];
  }
  $msg = NULL;

  if (isset($form_state['values']['commune_file_upload'])) {
     $file_id = $form_state['values']['commune_file_upload'];
     $msg = t('Shared a new file');
  }

  if (isset($form_state['values']['commune_link_shared'])) {
     $url = $form_state['values']['commune_link_shared'];
     $msg = t('Shared a new link');
  }

  if ($status_msg != '' || $file_id) {
    if ($form_state['values']['form_id'] == '_commune_content_post_form') {
      global $user;
      $new_node = new stdClass ();
      
      $new_node->type = 'commune_post';
      node_object_prepare ($new_node);
      $new_node->uid = ! empty ($user->uid) ? $user->uid : 0;
      $new_node->name = ! empty ($user->name) ? $user->name : variable_get ('anonymous', t ('Anonymous'));
      $new_node->language = 'und';
      $new_node->body[$new_node->language][0]['value'] = $status_msg;
      $status_msg_title = truncate_utf8($status_msg, 50, $wordsafe = TRUE, $add_ellipsis = TRUE, $min_wordsafe_length = 1);
      
      if ($file_id) {
         $scheme = 'public://commune//';
         $file = file_load($file_id);
         if (is_object ($file)) {
            $file->status = FILE_STATUS_PERMANENT;
            file_save($file);
            file_move($file, $scheme);
            $file->description = '';
            $file->display = 1;
            $new_node->field_commune_upload[$new_node->language][0] = (array) $file;
         }
      } else if ($url) {
         $new_node->field_commune_url[$new_node->language][0]['url'] = $url;
      }
      
      $new_node->title = $status_msg_title;
      $context = $form_state['values']['commune_subject_id'];
      $new_node->context_id = $context;
      $new_node->comment = 2;
      $new_node->status = 1;
      // Get content type default setting.
      $node_options = variable_get ('node_options_' . $new_node->type);
      $promote = array_key_exists ('1', $node_options);
      $new_node->promote = ! empty ($promote) ? $promote : 0;
      $new_node->revision = 0;
       
      $mentions = $form_state['values']['commune_post_mentions'];
      $new_node->commune_mentions = drupal_json_decode($mentions);

      $markup = $form_state['values']['commune_post_markup'];
      $new_node->commune_post_markup = $markup;

      node_submit ($new_node);
      node_save ($new_node);
      drupal_set_message (t ('Your post has been saved !'));
      if($file_id)
        file_usage_add ($file, 'commune', 'commune', $new_node->nid);
    }
  }
}

/**
 * Drupal wall Comment form for user comment posts.
 */
function _commune_comment_post_form($form_state, $args, $nid, $uid) {
  
  global $base_url;
  // Comment display form.
  $form['commune_comment'] = array (
      '#type' => 'textarea',
      '#resizable' => FALSE,
      '#attributes' => array (
          'placeholder' => ' ' . t ('write a comment...'),
          'class' => array('commune-comment-input'),
      ),
      '#required' => TRUE,
      '#prefix' => '<div style="margin-bottom:-8px;">',
      '#suffix' => '</div>' 
  );
  $form['commune_comment_nid'] = array (
      '#type' => 'hidden',
      '#value' => $args['build_info']['args']['0'] 
  );
  
  $form['commune_comment_uid'] = array (
      '#type' => 'hidden',
      '#value' => $args['build_info']['args']['1'] 
  );
  
  $form['commune_submit'] = array (
      '#type' => 'button',
      '#value' => t ('Comment'),
      '#attributes' => array('class' => array('commune-comment-post-btn')),
      '#ajax' => array (
          'callback' => '_commune_comment_ajax_form_post',
          'wrapper' => 'div_append_next_user_comment_' . $args['build_info']['args']['0'],
          'method' => 'append',
          'effect' => 'none' 
      ),
      '#prefix' => '<div style="margin-bottom:-10px;">',
      '#suffix' => '</div>' 
  );
  
  $form['#action'] = url ('user/'); 
  $form['#submit'] = array (
      '_commune_comment_ajax_form_post' 
  );
  return $form;
}

/**
 * Ajax call to save users comments for its respective node.
 */
function _commune_comment_ajax_form_post($form, $form_state) {
  
  global $base_url;
  global $user;
  $comment_value = trim (strip_tags ($form_state['values']['commune_comment']));
  
  if ($comment_value != '') {
    
    // Taking first 50 char of status_msg as title.
    $comment_msg = truncate_utf8($comment_value, 49, $wordsafe = TRUE, $add_ellipsis = TRUE, $min_wordsafe_length = 1);
    
    $comment = new stdClass ();
    
    $comment->nid = $form_state['input']['commune_comment_nid'];
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
      $html = '<div class="comment" id="commune_comment_cid_' . $new_comment->cid . '"><div class="comment_left"><a href="' . $base_url . '/user/' . $user->uid . '"><img src="' . _commune_user_profile_picture ($user->uid) . '" width="32px"></a></div><div class="comment_right"><div class="comment_delete">' . drupal_render (drupal_get_form ('_commune_delete_comment_form', $new_comment->cid, $user->uid)) . '</div><strong><a href="' . $base_url . '/user/' . $user->uid . '">' . ucwords ($user->name) . '</a>&nbsp;</strong>' . $comment_value . '<div class="caption">' . date ('F j, Y', time ()) . ' at ' . date ('h:ia', time ()) . '</div></div></div>';
      
      $form_state['values']['commune_comment'] = '';
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
function _commune_delete_edit_node_form($form_state, $args) {
  $form = array ();
  global $base_url;
  global $user;
  $commune_path = drupal_get_path ('module', 'commune');
  
  if (variable_get ('commune_edit_post_button') == 1) {
    $img = '<img src="' . $base_url . '/' . $commune_path . '/images/edit-icon.png' . '" >';
    $redirect = $base_url . '/node/' . $args['build_info']['args']['0'] . '/edit?destination=' . $_GET['q'];
    $form['commune_edit_post'] = array (
        '#markup' => l ($img, $redirect, array (
            'attributes' => array (
                'class' => 'anchor-class' 
            ),
            'html' => TRUE 
        )) 
    );
  }
  if (variable_get ('commune_delete_post_button') == 1) {
    $form['commune_delete_post'] = array (
        '#value' => t ('Delete Button'),
        '#type' => 'image_button',
        '#src' => $base_url . '/' . $commune_path . '/images/delete-icon.png',
        '#attributes' => array (
            'class' => array (
                'delete_btn' 
            ) 
        ),
        '#ajax' => array (
            'callback' => '_commune_delete_node_post',
            'wrapper' => 'commune_post_nid_' . $args['build_info']['args']['0'],
            'method' => 'replace',
            'event' => 'click' 
        ) 
    );
  }
  
  if (variable_get ('commune_delete_post_button') == 1 || variable_get ('commune_edit_post_button') == 1) {
    $form['commune_post_nid'] = array (
        '#type' => 'hidden',
        '#value' => $args['build_info']['args']['0'] 
    );
    
    $form['commune_post_uid'] = array (
        '#type' => 'hidden',
        '#value' => $args['build_info']['args']['1'] 
    );
    $form['#action'] = url ('user/'); //.variable_get ('commune_user_id'));
  }
  return $form;
}

/**
 * Delete form for user to delete its post.
 */
function _commune_delete_node_post($form, $form_state) {
  
  $nid = $form_state['input']['commune_post_nid'];
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
function _commune_delete_comment_form($form, $args) {
  global $base_url;
  global $user;
  $commune_path = drupal_get_path ('module', 'commune');
  $form = array ();
  
  // Delete button visible only to user who post that comment.
  if ($args['build_info']['args']['1'] == $user->uid && variable_get ('commune_delete_post_button') == 1) {
    $form['#attributes']['class'] = 'drupal-wall-delete-btn-form';
    
    $form['commune_comment_cid'] = array (
        '#type' => 'hidden',
        '#value' => $args['build_info']['args']['0'] 
    );
    
    $form['commune_comment_uid'] = array (
        '#type' => 'hidden',
        '#value' => $args['build_info']['args']['1'] 
    );

/*
    $form['commune_confirm'] = array(
        '#markup' => l('x', '#', array('attributes' => array('class' => 'delete_btn'))));
*/

    $form['actions']['submit'] = array(
        '#type' => 'image_button',
        '#src' => $base_url . '/' . $commune_path . '/images/delete-icon.png',
        '#ajax' => array (
            'callback' => '_commune_delete_comment_form_submit',
            'wrapper' => 'commune_comment_cid_' . $args['build_info']['args']['0'],
            'method' => 'replace',
            'event' => 'click' 
        ),
        '#attributes' => array (
            'class' => array (
                'delete_btn' 
            ) 
        ) 
    );
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
function _commune_delete_comment_form_submit($form, $form_state) {
  $cid = $form_state['input']['commune_comment_cid'];
  
  // Deleting all content for node Comment via cid.
  comment_delete ($cid);
  // Replace with null (hiding).
  return '';
}

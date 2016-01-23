<?php

/**
 * @file
 * Contains the administrative functions of the Shout Box module.
 *
 * This file is included by the Shout box admin settings form.
 */

/**
 * Admin configure form control on page.
 */
function _drupal_wall_configure_form() {
  $form['drupal_wall'] = array (
      '#type' => 'fieldset',
      '#title' => t ('Drupal Wall Settings') 
  );
  $form['drupal_wall']['drupal_wall_wall_post_limit'] = array (
      '#type' => 'textfield',
      '#title' => t ('Number of posts to show on the user wall'),
      '#default_value' => (int) variable_get ('drupal_wall_wall_post_limit'),
      '#description' => t ('Enter range of wall post limit (in numeric). Example "10"'),
      '#size' => 7,
      '#required' => TRUE 
  );
  // Drupal Global wall post display content
  $form['drupal_wall']['drupal_wall_global_post_limit'] = array (
      '#type' => 'textfield',
      '#title' => t ('Number of posts to show on the global wall'),
      '#default_value' => (int) variable_get ('drupal_wall_global_post_limit'),
      '#description' => t ('Enter range of global wall post limit (in numeric). Example "10"'),
      '#size' => 7,
      '#required' => TRUE 
  );
  $form['drupal_wall']['drupal_wall_textbox_type'] = array (
      '#type' => 'radios',
      '#title' => t ('Textbox'),
      '#default_value' => variable_get ('drupal_wall_textbox_type'),
      '#description' => t ('Choose the type of form widget used for entering a wall post.'),
      '#options' => array (
          'textfield' => t ('Text Field'),
          'textarea' => t ('Text Area'),
          'text_format' => t ('Text Format') 
      ),
      '#required' => TRUE 
  );
  // Wall post type.
  $form['drupal_wall']['wall_post'] = array (
      '#markup' => '<strong>' . t ('Media type of wall post') . '</strong>' 
  );
  $form['drupal_wall']['drupal_wall_post_type_photo'] = array (
      '#type' => 'checkbox',
      '#title' => t ('Allow Photos'),
      '#default_value' => variable_get ('drupal_wall_post_type_photo') 
  );
  $form['drupal_wall']['drupal_wall_post_type_video'] = array (
      '#type' => 'checkbox',
      '#title' => t ('Allow share video'),
      '#default_value' => variable_get ('drupal_wall_post_type_video') 
  );
  // Select image Style
  $image_style = drupal_wall_image_styles ();
  $form['drupal_wall']['wall_post_image_style'] = array (
      '#type' => 'fieldset',
      '#title' => t ('Select image style for status posts'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE 
  );
  $allow_image_style_options = variable_get ('allow_image_style_options');
  if (! empty ($allow_image_style_options)) {
    $default_value = $allow_image_style_options;
  }
  else {
    $default_value = array ();
  }
  $form['drupal_wall']['wall_post_image_style']['allow_image_style_options'] = array (
      '#type' => 'checkboxes',
      '#title' => t ('Allow image styles for front end users.'),
      '#options' => $image_style,
      '#default_value' => $default_value 
  );
  $form['drupal_wall']['button_msg'] = array (
      '#markup' => '<strong>' . t ('Enable or Disable operations') . '</strong>' 
  );
  $form['drupal_wall']['drupal_wall_older_post_button'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Show older Post" Button'),
      '#default_value' => variable_get ('drupal_wall_older_post_button') 
  );
  $form['drupal_wall']['drupal_wall_delete_post_button'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Delete Post" Button'),
      '#default_value' => variable_get ('drupal_wall_delete_post_button') 
  );
  $form['drupal_wall']['drupal_wall_edit_post_button'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Edit Post" Button'),
      '#default_value' => variable_get ('drupal_wall_edit_post_button') 
  );
  $form['drupal_wall']['drupal_wall_show_comments'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Display users comments"'),
      '#default_value' => variable_get ('drupal_wall_show_comments') 
  );
  $form['drupal_wall']['drupal_wall_comment_post_textbox'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable user to "Write a comment".'),
      '#default_value' => variable_get ('drupal_wall_comment_post_textbox') 
  );
  $form['drupal_wall']['drupal_wall_likes_post'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Like or unlike Node & Comment" operation'),
      '#default_value' => variable_get ('drupal_wall_likes_post') 
  );
  $form['drupal_wall']['drupal_wall_content_type'] = array (
      '#type' => 'radios',
      '#title' => t ('Show content type on user wall post'),
      '#options' => array (
          'Only Drupal wall content',
          'All content of user' 
      ),
      '#default_value' => variable_get ('drupal_wall_content_type', 0) 
  );
  $form['drupal_wall']['drupal_wall_likes_node'] = array (
      '#type' => 'textfield',
      '#title' => t ('Flag Like machine name'),
      '#default_value' => variable_get ('drupal_wall_likes_node'),
      '#description' => t ('Please enter the machine name of flag like.&nbsp;') . l (t ('Create a new flag'), 'admin/structure/flags/add') . t (', select Flag type = "Nodes" and Bundles/content type = "Drupal Wall".'),
      '#states' => array (
          'visible' => array (
              ':input[name="drupal_wall_likes_post"]' => array (
                  'checked' => TRUE 
              ) 
          ) 
      ) 
  );
  $form['drupal_wall']['drupal_wall_likes_comment'] = array (
      '#type' => 'textfield',
      '#title' => t ("Flag's Comment Like machine name"),
      '#default_value' => variable_get ('drupal_wall_likes_comment'),
      '#description' => t ('Please enter the machine name of flag comment like.&nbsp;') . l (t ('Create a new flag'), 'admin/structure/flags/add') . t (', select Flag type = "Comments" and Bundles/content type = "Drupal Wall comment".'),
      '#states' => array (
          'visible' => array (
              ':input[name="drupal_wall_likes_post"]' => array (
                  'checked' => TRUE 
              ) 
          ) 
      ) 
  );
//drupal_wall_what_is_your_mind_string
  $form['drupal_wall']['what_is_on_your_mind'] = array (
      '#type' => 'fieldset',
      '#title' => t ('What\'s on your mind'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE
  );
  
  
  $form['drupal_wall']['what_is_on_your_mind']['drupal_wall_what_is_on_your_mind_string_post_box'] = array (
      '#type' => 'textfield',
      '#title' => t ("What's on your mind for wall post box"),
      '#default_value' => variable_get ('drupal_wall_what_is_on_your_mind_string_post_box','What\'s on your mind ?'),
      '#description' => t ('What\'s on your mind" string post box.'),
  );
  
  $form['drupal_wall']['what_is_on_your_mind']['drupal_wall_what_is_on_mind_string_user_page_title'] = array (
      '#type' => 'textfield',
      '#title' => t ("What's on your mind for wall"),
      '#default_value' => variable_get ('drupal_wall_what_is_on_mind_string_user_page_title','What\'s on your mind ?'),
      '#description' => t ('"What\'s on your mind" string on user profile page.'),
  );  
  
  return system_settings_form ($form);
}

/**
 * Validate admin configure form control on page.
 */
function _drupal_wall_configure_form_validate($form, $form_state) {
  if (isset ($form_state['values']['drupal_wall_wall_post_limit'])) {
    if (! (is_numeric ($form_state['values']['drupal_wall_wall_post_limit']))) {
      form_set_error ('drupal_wall_wall_post_limit', t ('Wall post limit should be numeric. Eg: 10'));
    }
    elseif ($form_state['values']['drupal_wall_wall_post_limit'] < 1 || $form_state['values']['drupal_wall_wall_post_limit'] > 999) {
      form_set_error ('drupal_wall_wall_post_limit', t ('Wall post limit should be between 1 to 999.'));
    }
  }
  // Udate iamge list style DDL
  _drupal_wall_table_update ($form_state);
}

/**
 * Define _drupal_wall_table_update().
 */
function _drupal_wall_table_update(&$form_state) {
  $option_list = drupal_wall_image_styles ();
  $query = db_select ('field_config', 'fc');
  $query = $query->fields ('fc', array (
      'field_name',
      'data',
      'deleted' 
  ))->condition ('fc.field_name', 'field_drupal_wall_image_style')->condition ('fc.deleted', 0);
  $query = $query->execute ()->fetchAll ();
  $data = unserialize ($query['0']->data);
  if (! empty ($form_state['values']['allow_image_style_options'])) {
    $selected_new_image_style = array (
        '_none' => '-None-' 
    );
    foreach ($form_state['values']['allow_image_style_options'] as $allow_image_style_options_key => $allow_image_style_options_val) {
      if ($allow_image_style_options_val != '0') {
        $selected_new_image_style[$allow_image_style_options_key] = $option_list[$allow_image_style_options_key];
      }
    }
    $data['settings']['allowed_values'] = $selected_new_image_style;
    $serialize_data = serialize ($data);
    $num_updated = db_update ('field_config')->fields (array (
        'data' => $serialize_data 
    ))->condition ('field_name', 'field_drupal_wall_image_style', '=')->execute ();
  }
}
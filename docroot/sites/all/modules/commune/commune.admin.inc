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
function _commune_configure_form() {
  $form['commune'] = array (
      '#type' => 'fieldset',
      '#title' => t ('Commune Settings') 
  );
  $form['commune']['commune_wall_post_limit'] = array (
      '#type' => 'textfield',
      '#title' => t ('Number of posts to show on the user wall'),
      '#default_value' => (int) variable_get ('commune_wall_post_limit'),
      '#description' => t ('Enter range of wall post limit (in numeric). Example "10"'),
      '#size' => 7,
      '#required' => TRUE 
  );


  $form['commune']['commune_global_post_limit'] = array (
      '#type' => 'textfield',
      '#title' => t ('Number of posts to show on the global wall'),
      '#default_value' => (int) variable_get ('commune_global_post_limit'),
      '#description' => t ('Enter range of global wall post limit (in numeric). Example "10"'),
      '#size' => 7,
      '#required' => TRUE 
  );
  $form['commune']['commune_textbox_type'] = array (
      '#type' => 'radios',
      '#title' => t ('Textbox'),
      '#default_value' => variable_get ('commune_textbox_type'),
      '#description' => t ('Choose the type of form widget used for entering a wall post.'),
      '#options' => array (
          'textfield' => t ('Text Field'),
          'textarea' => t ('Text Area'),
          /* 'text_format' => t ('Text Format')  */
      ),
      '#required' => TRUE 
  );

  // Wall post type.
  $form['commune']['wall_post'] = array (
      '#markup' => '<strong>' . t ('Media type of wall post') . '</strong>' 
  );
  $form['commune']['commune_post_type_file'] = array (
      '#type' => 'checkbox',
      '#title' => t ('Allow Files'),
      '#default_value' => variable_get ('commune_post_type_file') 
  );
  $form['commune']['commune_post_type_link'] = array (
      '#type' => 'checkbox',
      '#title' => t ('Allow share link'),
      '#default_value' => variable_get ('commune_post_type_link') 
  );
  $form['commune']['button_msg'] = array (
      '#markup' => '<strong>' . t ('Enable or Disable operations') . '</strong>' 
  );
  $form['commune']['commune_show_post_types'] = array (
      '#type' => 'select',
      '#title' => t ('Post Privacy Setting'),
      '#options' => array(
          'public' => t('Public'),
          'private' => t('Private'),
       ),
      '#default_value' => variable_get('commune_show_post_types','private'),
      '#description' => t ('Public will display all posts made by user on anywall and all posts made to users wall. Private will display only posts made to the users wall'),
  );
  $form['commune']['commune_older_post_button'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Show older Post" Button'),
      '#default_value' => variable_get ('commune_older_post_button') 
  );
  $form['commune']['commune_delete_post_button'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Delete Post" Button'),
      '#default_value' => variable_get ('commune_delete_post_button') 
  );
  $form['commune']['commune_edit_post_button'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Edit Post" Button'),
      '#default_value' => variable_get ('commune_edit_post_button') 
  );
  $form['commune']['commune_show_comments'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Display users comments"'),
      '#default_value' => variable_get ('commune_show_comments') 
  );
  $form['commune']['commune_comment_post_textbox'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable user to "Write a comment".'),
      '#default_value' => variable_get ('commune_comment_post_textbox') 
  );
  $form['commune']['commune_likes_post'] = array (
      '#type' => 'checkbox',
      '#title' => t ('"Enable "Like or unlike Node & Comment" operation'),
      '#default_value' => variable_get ('commune_likes_post') 
  );
  $form['commune']['commune_content_type'] = array (
      '#type' => 'radios',
      '#title' => t ('Show content type on user wall post'),
      '#options' => array (
          'Only Commune Posts',
          'All content of user' 
      ),
      '#default_value' => variable_get ('commune_content_type', 0) 
  );

  return system_settings_form ($form);
}

/**
 * Validate admin configure form control on page.
 */
function _commune_configure_form_validate($form, $form_state) {
  if (isset ($form_state['values']['commune_wall_post_limit'])) {
    if (! (is_numeric ($form_state['values']['commune_wall_post_limit']))) {
      form_set_error ('commune_wall_post_limit', t ('Wall post limit should be numeric. Eg: 10'));
    }
    elseif ($form_state['values']['commune_wall_post_limit'] < 1 || $form_state['values']['commune_wall_post_limit'] > 999) {
      form_set_error ('commune_wall_post_limit', t ('Wall post limit should be between 1 to 999.'));
    }
  }
}


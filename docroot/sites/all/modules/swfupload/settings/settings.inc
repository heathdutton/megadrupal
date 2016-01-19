<?php

/**
 * @file
 * The swfupload settings form.
 */

/**
 *
 */
function swfupload_settings_form() {

  // Add js and css
  $path = drupal_get_path('module', 'swfupload');
  drupal_add_css($path .'/settings/settings.css');
  drupal_add_js($path .'/settings/settings.js');

  // Load all roles
  $roles = user_roles();
  $node_types = node_get_types();

  // The first loop we make goes trough all the node types
  foreach ($node_types as $node_type) {

    // Get the default values
    $settings = variable_get('swfupload_setting_' . $node_type->type, array());

    // Now we are in the loop of every node type, we can loop every role :-)
    foreach ($roles as $key => $role) {
      $roles[$key] = $role;

      // Set a default value if array is empty
      if (empty($settings[$role])) {
        $settings[$role]->filepath = '%file_directory_path';
        $settings[$role]->max_img_resolution = '800x600';
        $settings[$role]->list = 1;
        $settings[$role]->file_extensions = 'jpg jpeg gif png txt';
        $settings[$role]->max_file_size = 1;
        $settings[$role]->node_max_file_size = 0;
        $settings[$role]->max_files = 1;
        $settings[$role]->upload_usersize = 10;

        if ($role == t('authenticated user')) {
          $settings[$role]->upload_usersize = round((file_upload_max_size() / (1024 * 1024)));
        }
      }

      // Create the prefix for each role
      $form_prefix = $node_type->type .'_'. str_replace(' ', '_', $role);

      // Path settings
      $form[$form_prefix .'_filepath'] = array(
        '#type' => 'textfield',
        '#title' => t('Path settings'),
        '#default_value' => $settings[$role]->filepath,
        '#maxlength' => 255,
      );
      // Resolution
      $form[$form_prefix .'_max_img_resolution'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum resolution for uploaded images'),
        '#default_value' => $settings[$role]->max_img_resolution,
        '#size' => 15,
        '#maxlength' => 10,
        '#field_suffix' => '<kbd>'. t('WIDTHxHEIGHT') .'</kbd>'
      );
      // Display attachments
      $form[$form_prefix .'_list'] = array(
        '#type' => 'checkbox',
        '#title' => t('List files by default'),
        '#default_value' => $settings[$role]->list,
      );
      // Total files per node
      $form[$form_prefix .'_max_files'] = array(
        '#type' => 'select',
        '#title' => t('Maximum files per node'),
        '#default_value' => $settings[$role]->max_files,
        '#options' => array(0 => 0, 1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 10 => 10, 20 => 20, 50 => 50),
      );
      // Allowed extentions
      $form[$form_prefix .'_file_extensions'] = array(
        '#type' => 'textfield',
        '#title' => t('Permitted file extensions'),
        '#default_value' => $settings[$role]->file_extensions,
        '#maxlength' => 255,
      );
      $form[$form_prefix .'_max_file_size'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum file size per upload'),
        '#default_value' => $settings[$role]->max_file_size,
        '#size' => 5,
        '#maxlength' => 5,
        '#field_suffix' => t('MB'),
      );
      $form[$form_prefix .'_node_max_file_size'] = array(
        '#type' => 'textfield',
        '#title' => t('Total file size per node'),
        '#default_value' => $settings[$role]->node_max_file_size,
        '#size' => 5,
        '#maxlength' => 5,
        '#field_suffix' => t('MB'),
      );

      $form[$form_prefix .'_upload_usersize'] = array(
        '#type' => 'textfield',
        '#title' => t('Total file size per user'),
        '#default_value' => $settings[$role]->upload_usersize,
        '#size' => 5,
        '#maxlength' => 5,
        '#field_suffix' => t('MB'),
      );
    }
  }
  $form['node_types'] = array('#type' => 'value', '#value' => $node_types);
  $form['roles'] = array('#type' => 'value', '#value' => $roles);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Theme function for the file settings form
 */
function theme_swfupload_settings_form($form) {
  // Create an array for all fields we want to render.
  $fields = array('filepath', 'max_img_resolution', 'list', 'max_files', 'file_extensions', 'max_file_size', 'node_max_file_size', 'upload_usersize', );

  // Loop trough all node types
  foreach ($form['node_types']['#value'] as $node_type) {
    // Reset the output array
    $output = array('<div class="swf-nodetype-wrapper">');

    // Loop trough all roles
    foreach ($form['roles']['#value'] as $role) {
      // Reset the rows array
      $rows = array();

      // Create the prefix for each role
      $form_prefix = $node_type->type .'_'. str_replace(' ', '_', $role);

      // Loop trough the fields we want to render
      foreach ($fields as $field) {
        $title = $form[$form_prefix ."_". $field]['#title'];
        unset($form[$form_prefix ."_". $field]['#title']);
        $rows[] = array(
          array(
            'data' => $title,
            'class' => 'left'
          ),
          drupal_render($form[$form_prefix ."_". $field])
        );
      }
      $output[] = theme('table', array(array('data' => $role, 'colspan' => 2, 'class' => 'left2')), $rows);
    }
    $output[] = '</div><div class="swf-harmonica-arrow"></div>';
    $lis[] = "<label><span>$node_type->name</span></label>". join("\n", $output);
  }
  return theme('item_list', $lis) . drupal_render($form) .'<div style="clear:both"></div>';
}

/**
 * validation of the file settings form
 */
function swfupload_settings_form_validate($form, &$form_state) {

  // Loop trough all node types
  foreach ($form['node_types']['#value'] as $node_type) {

    // Loop trough all the roles
    foreach ($form['roles']['#value'] as $role) {

      // Create the prefix for each role
      $form_prefix = $node_type->type .'_'. str_replace(' ', '_', $role);

      // Max resolution
      if ($form_state['values'][$form_prefix .'_max_img_resolution'] != 0) {
        $max_resolution = explode('x', $form_state['values'][$form_prefix .'_max_img_resolution']);
        if (count($max_resolution) != 2) {
          form_set_error($form_prefix .'_max_img_resolution', t('Maximum resolution for uploaded images is not valid'));
        }
        else {
          if (!is_numeric($max_resolution[0]) or !is_numeric($max_resolution[1])) {
            form_set_error($form_prefix .'_max_img_resolution', t('Maximum resolution for uploaded images is not valid'));
          }
        }
      }

      // Permited extentions
      $extensions = explode(' ', $form_state['values'][$form_prefix .'_file_extensions']);
      foreach ($extensions as $extention) {
        if (!preg_match("/^([A-Z0-9]){2,10}$/i", $extention)) {
          form_set_error($form_prefix .'_file_extensions', t('One of more extentions are not valid'));
          break;
        }
      }

      // Maximum file size
      if (!is_numeric($form_state['values'][$form_prefix .'_max_file_size'])) {
        form_set_error($form_prefix .'_max_file_size', t('Maximum file size per upload is not valid'));
      }

      // Total node size
      if (!is_numeric($form_state['values'][$form_prefix .'_node_max_file_size'])) {
        form_set_error($form_prefix .'_node_max_file_size', t('Total file size per node is not valid'));
      }

      // Total user size
      if (!is_numeric($form_state['values'][$form_prefix .'_upload_usersize'])) {
        form_set_error($form_prefix .'_upload_usersize', t('Total file size per user is not valid'));
      }
    }
  }
}

/**
 * Submittion of the file settings form
 */
function swfupload_settings_form_submit($form, &$form_state) {

  // Loop trough all node types
  foreach ($form['node_types']['#value'] as $node_type) {

    // Loop trough all the roles
    foreach ($form['roles']['#value'] as $role) {

      // Create the prefix for each role
      $form_prefix = $node_type->type .'_'. str_replace(' ', '_', $role);

      $settings[$role]->filepath = $form_state['values'][$form_prefix .'_filepath'];
      $settings[$role]->max_img_resolution = $form_state['values'][$form_prefix .'_max_img_resolution'];
      $settings[$role]->list = $form_state['values'][$form_prefix .'_list'];
      $settings[$role]->file_extensions = drupal_strtolower($form_state['values'][$form_prefix .'_file_extensions']);
      $settings[$role]->max_file_size = $form_state['values'][$form_prefix .'_max_file_size'];
      $settings[$role]->upload_usersize = $form_state['values'][$form_prefix .'_upload_usersize'];
      $settings[$role]->node_max_file_size = $form_state['values'][$form_prefix .'_node_max_file_size'];
      $settings[$role]->max_files = $form_state['values'][$form_prefix .'_max_files'];
    }
    variable_set('swfupload_setting_'. $node_type->type, $settings);
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

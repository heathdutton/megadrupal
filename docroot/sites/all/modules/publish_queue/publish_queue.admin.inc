<?php 

/**
 * @file
 * Administrative pages functions for the publish_queue module.
 */

/**
 * Render the admin page form
 */
function publish_queue_admin() {
  $form['publish_queue_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Publish queue interval'),
    '#default_value' => variable_get('publish_queue_interval', 86400),
    '#size' => 20,
    '#maxlength' => 20,
    '#description' => t('The interval between publishing nodes in queue. This value must be entered in seconds. IE: 86400s = 1 day'),
  );
  return system_settings_form($form);
}
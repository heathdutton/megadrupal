<?php

/**
 * @file
 * Administrative page callbacks for the flickr_bricks module.
 */

/**
 * Implements hook_admin_settings().
 */
function flickr_bricks_admin_settings_form($form_state) {

  $form['flickr_bricks_feed_url'] = array(
    '#title' => t('Flickr Feed URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('flickr_bricks_feed_url', 'http://api.flickr.com/services/feeds/photos_public.gne'),
    '#description' => t('Warning: Do not change this Flickr public feed url unless Flickr makes a change.'),
  );

  $form['flickr_bricks_ids'] = array(
    '#title' => t('Flickr ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('flickr_bricks_ids', ''),
    '#description' => t('The Flickr ID is unique and you will get it when you register an account with <a href="http://www.flickr.com" target="_blank">www.flickr.com</a>. Your Flickr ID will look something like this 22828405@N04, if you do not find your ID, you can get it easily from <a href="http://idgettr.com/" target="_blank">http://idgettr.com</a> by giving your username. <br/><br/>Note: Multiple IDs should be separated by commas.'),
  );

  $form['flickr_bricks_tags'] = array(
    '#title' => t('Flickr Tags'),
    '#type' => 'textfield',
    '#default_value' => variable_get('flickr_bricks_tags', ''),
    '#description' => t('Flickr tags will display all the images that has been tagged with the given tags. For example, say you added chipmunks,baseball,winter. Flickr bricks will display image with the either one of chipmunks, baseball and winter tags. <br/><br/>Note: Multiple Tags should be separated by commas.'),
  );

  $form['flickr_bricks_img_count'] = array(
    '#title' => t('Number of images'),
    '#type' => 'select',
    '#options' => range(0, 20),
    '#default_value' => variable_get('flickr_bricks_img_count', '15'),
  );

  return system_settings_form($form);
}

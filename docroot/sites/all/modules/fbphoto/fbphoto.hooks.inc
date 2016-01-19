<?php

/**
 * Implements hook_theme().
 */
function fbphoto_theme() {
  return array(
      'fbphoto_photo' => array(
          'variables' => array('photo' => NULL)
      ),
      'fbphoto_album' => array(
          'variables' => array('album' => NULL)
      ),
  );
}

/**
 * Implements hook_filter_info().
 */
function fbphoto_filter_info() {
  $filters['fbphoto'] = array(
      'title' => t('Facebook photo'),
      'description' => t('Converts [fb:https://www.facebook.com/media/set/?set=a.299178216769602.79888.201606459860112&type=3] to inline images.'),
      'process callback' => 'fbphoto_filter_callback',
      'tips callback' => '_filter_autop_tips',
      'cache' => FALSE, # @TODO: Remove me
  );
  return $filters;
}

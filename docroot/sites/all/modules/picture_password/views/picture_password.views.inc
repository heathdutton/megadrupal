<?php
/**
 * @file
 * picture_password.views.inc
 *
 * Views data for Picture Password module.
 */

/**
 * Implements hook_views_data().
 */
function picture_password_views_data() {

  $data['picture_password']['table']['group']  = t('Kaje Picture Passwords');

  $data['picture_password']['table']['base'] = array(
    'field' => 'drupal_uid',
    'title' => t('Picture Password IDs'),
    'help' => t('Stores Kaje Picture Password account IDs corresponding to Drupal user IDs.'),
  );

  $data['picture_password']['table']['join'] = array(
    // Make all of our fields instantly available in a Users view.
    // picture_password.drupal_uid connects to users.uid
    'users' => array(
      'left_field' => 'uid',
      'field' => 'drupal_uid',
      'type' => 'INNER',
    ),
  );

  $data['picture_password']['drupal_uid'] = array(
    'title' => t('Uid'),
    'help' => t('The Drupal user ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['picture_password']['kaje_uid'] = array(
    'title' => t('Kaje uid'),
    'help' => t('Kaje Picture Password account ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['picture_password']['flags'] = array(
    'title' => t('Flags'),
    'help' => t('Flags pertaining to Kaje'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_data_alter().
 *
function picture_password_views_data_alter(&$data) {
}
 */

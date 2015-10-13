<?php

/**
 * @file
 * Providing extra integration with views.
 */


/**
 * Implements hook_views_data_alter()
 */
function wechat_views_data_alter(&$data) { 
  $data ['wechat_request_message']['from_user_name']['relationship'] =  array(
      'base' => 'wechat_user', // The name of the table to join with.
      'base field' => 'openid', // The name of the field on the joined table.
      // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
      'handler' => 'views_handler_relationship',
      'label' => t('wechat user'),
      'title' => t('wechat user'),
  );
  $data ['wechat_response_message']['to_user_name']['relationship'] =  array(
      'base' => 'wechat_user', // The name of the table to join with.
      'base field' => 'openid', // The name of the field on the joined table.
      // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
      'handler' => 'views_handler_relationship',
      'label' => t('wechat user'),
      'title' => t('wechat user'),
  );  
}


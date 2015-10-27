<?php

/**
 * Implementation of hook_views_data_alter().
 */
function views_share_views_data_alter(&$data) {
  // Add our share area to Global.
  $data['views']['share'] = array(
    'title' => t('Share'),
    'help' => t('Enable sharing for this view.'),
    'area' => array(
      'handler' => 'views_share_handler_area_share',
    ),
  );
}

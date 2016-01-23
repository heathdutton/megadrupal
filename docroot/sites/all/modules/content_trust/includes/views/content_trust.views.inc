<?php

/**
 * Implementation of hook_views_data_alter().
 */
function content_trust_views_data_alter(&$data) {
  $data['node']['content_trust_type'] = array(
    'title' => t('Types moderated by Content Trust'),
    'real field' => 'type',
    'help' => t('Automatically filter view for types defined in Content Trust settings.'),
    'filter' => array(
      'handler' => 'content_trust_handler_filter_content_trust',
    ),
  );
}

<?php

/**
 * @file
 * Provide views handlers.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data().
 */
function views_ctools_dropbutton_views_data() {

  $data['views']['views_ctools_dropbutton'] = array(
    'title' => t('Ctools dropbutton'),
    'help' => t('Display fields in a ctools dropbutton.'),
    'field' => array(
      'handler' => 'ViewsHandlerFieldViewsCtoolsDropbutton',
    ),
  );

  return $data;
}

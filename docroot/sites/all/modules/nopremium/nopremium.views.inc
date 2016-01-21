<?php

/**
 * @file
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 */



/**
 * Implementation of hook_views_data()
 */
function nopremium_views_data() {
  $data['node']['premium'] = array(
    'title' => t('Premium content'),
    'help' => t('Whether or not the node is premium content.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'premium' => array('', t('Premium content')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Premium content'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Whether or not the node is premium content. To list premium nodes first, set this to descending.'),
    ),
  );
  return $data;
}

<?php
/**
 * @file
 * Views integration for the classified module
 *
 * @copyright (c) 2010 Ouest Systemes Informatiques (OSInet)
 *
 * @author Frederic G. MARAND <fgm@osinet.fr>
 *
 * @license General Public License version 2 or later
 *
 * New code implementing a feature set derived from the ed_classified module:
 *
 * ---- Information about ed_classified ----
 *
 * Michael Curry, Exodus Development, Inc.
 * exodusdev@gmail.com
 * for more information, please visit:
 *   http://exodusdev.com/drupal/modules/ed_classified.module
 *
 * Copyright (c) 2006, 2007 Exodus Development, Inc.  All Rights Reserved.
 *
 * Licensed under the terms of the GNU Public License (GPL) version 2.  Please
 * see LICENSE.txt for license terms.  Possession and use of this code signifies
 * acceptance of license terms.
 *
 * ---- /ed-classified ----
 */

/**
 * Implements hook_views_data().
 */
function classified_views_data() {
  $data = array();

  $data['classified_node'] = array(
    'table' => array(
      'group' => t('Classified Ads'),
      'join' => array(
        'node' => array(
          'field' => 'vid',
          'left_field' => 'vid',
        ),
      ),
    ),
    'expires' => array(
      'title' => t('Expiration'),
      'help' => t('Date the ad expires'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),

    'purge' => array(
      'title' => t('Purge'),
      'help' => t('Date the ad will be deleted from system'),
      'field' => array(
        'handler' => 'classified_handler_field_date_purge',
        'click sortable' => TRUE,
        'real field' => 'expires',
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function classified_views_handlers() {
  $ret = array(
    'info' => array(
      'path' => drupal_get_path('module', 'classified') . '/views',
    ),
    'handlers' => array(
      'classified_handler_field_date_purge' => array(
        'parent' => 'views_handler_field_date',
      ),
    ),
  );

  return $ret;
}


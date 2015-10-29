<?php
/**
 * @file
 * Default Views for the classified module
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
 * Implements hook_views_default_views().
 */
function classified_views_default_views() {

  $views = array();

  $path = drupal_get_path('module', 'classified') . '/views';
  foreach (file_scan_directory($path, '/.*\.php$/') as $file) {
    require_once DRUPAL_ROOT . '/' . $file->filename;
    $views[$view->name] = $view;
  }
  return $views;
}

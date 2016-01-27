<?php

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_menu() {

  $items = array();

  $items['admin/config/brilliant_gallery'] = array(
    'title' => 'Brilliant gallery',
    'description' => 'Brilliant gallery module settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('brilliant_gallery_admin'),
    'access arguments' => array('administer brilliant_gallery'),
    'type' => MENU_NORMAL_ITEM,
  );
  // Same as 'admin/config/brilliant_gallery' -- just for the convenience/clarity in a dropdown menu.
  $items['admin/config/brilliant_gallery/settings'] = array(
    'title' => 'Brilliant gallery',
    'description' => 'Brilliant gallery module settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('brilliant_gallery_admin'),
    'access arguments' => array('administer brilliant_gallery'),
    'type' => MENU_NORMAL_ITEM,
  );

  # A shortcut to the permissions settings for this module.
  $items['admin/config/brilliant_gallery/permissions'] = array(
    #'path' => 'admin/user/access#module-brilliant_gallery',
    'title' => 'Configure permissions',
    'description' => 'Configure access permissions for the Brilliant gallery module',
    'page callback' => 'brilliant_gallery_perms',
    #'page arguments' => 'brilliant_gallery_perms',
    'access arguments' => array('administer brilliant_gallery'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['admin/config/brilliant_gallery/manage'] = array(
    'title' => 'Manage galleries',
    //'description' => 'Manage galleries displayed using the Brilliant gallery module - e.g. visibility of individual images, etc.',
    'page callback' => 'render_brilliant_gallery_manage',
    //'page callback' => 'drupal_get_form',
    //'page arguments' => '',
    #'page arguments' => 'brilliant_gallery_perms',
    'access arguments' => array('administer brilliant_gallery'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['brilliant_gallery'] = array(
    'title' => 'Brilliant gallery',
    'page callback' => 'brilliant_gallery_all',
    'access arguments' => array('access brilliant_gallery'),
    'type' => MENU_CALLBACK,
  );

  $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
  $items[FILE_DIRECTORY_PATH . '/' . $pcache] = array(
    'title' => 'render image',
    'page callback' => 'brilliant_gallery_deliver_image',
    'access arguments' => array('access brilliant_gallery'),
    'type' => MENU_CALLBACK,
  );

  # 2 menu callback functions to implement the ajax load and save routines
  $items['bgchecklist/loadall'] = array(
    'page callback' => 'brilliant_gallery_checklist_loadall',
    'type' => MENU_CALLBACK,
    'access arguments' => array('access brilliant_gallery'),
  );
  $items['bgchecklist/save'] = array(
    'page callback' => 'brilliant_gallery_checklist_save',
    'type' => MENU_CALLBACK,
    'access arguments' => array('administer brilliant_gallery'),
  );

  return $items;
}

<?php

/**
 * @file
 *
 */

/**
 *
 */
function alfresco_browser_page() {
  // Disable admin_menu, admin module output and similar modules
  module_invoke_all('suppress');

  // @todo Controlar cuando el nodo no existe o se ha producido un error.

  alfresco_include('soap');
  $home_path = variable_get('alfresco_browser_home', ALFRESCO_BROWSER_HOME);
  $home_node = alfresco_soap_node_load($home_path);
  $home_ref  = $home_node ? $home_node->id : '';
  $home_name = $home_node ? $home_node->cm_name : t('- Error -');

  if (variable_get('alfresco_browser_remember_folder', 1)) {
    if (isset($_SESSION['alfresco_browser_last_folder'])) {
      $last_folder = $_SESSION['alfresco_browser_last_folder'];
      if ($last_node = alfresco_soap_node_load('workspace://SpacesStore/'. $last_folder)) {
        $folder_path = _alfresco_browser_node_path($last_node);
        $folder_name = $last_node->cm_name;
        $folder_ref  = $last_node->id;
      }
    }
  }

  // Alfresco Browser Settings
  $settings = array('alfresco' => array(
    'homeRef'            => $home_ref,
    'homeText'           => $home_name,
    'iconsPath'          => url(drupal_get_path('module', 'alfresco') .'/images/filetypes'),
    'serviceTreeUrl'     => url('alfresco/browser/service/spaces'),
    'serviceGridUrl'     => url('alfresco/browser/service/items'),
    'serviceDownloadUrl' => url('alfresco/browser/service/download'),
    'serviceUploadUrl'   => url('alfresco/browser/service/upload'),
    'serviceDeleteUrl'   => url('alfresco/browser/service/delete'),
    'serviceCreateUrl'   => url('alfresco/browser/service/create'),
    'queryLimit'         => intval(variable_get('alfresco_browser_query_limit', 20)),
    'accessAdd'          => user_access('upload alfresco repository'),
    'accessDelete'       => user_access('upload alfresco repository'),
    'folderPath'         => !empty($folder_path) ? $folder_path : '',
    'folderRef'          => !empty($folder_path) ? $folder_ref  : $home_ref,
    'folderName'         => !empty($folder_path) ? $folder_name : $home_name,
    'maxUploadSize'      => format_size(file_upload_max_size()),
    'searchMode'         => intval(variable_get('alfresco_browser_search', 1)),
    'token'              => drupal_get_token('alfresco_browser'),
  ));
  drupal_add_js($settings, 'setting');

  echo theme('alfresco_browser_page');
}

/**
 *
 */
function alfresco_browser_repository_page() {
  drupal_add_css(drupal_get_path('module', 'alfresco_browser') .'/alfresco_browser.css');
  drupal_set_title('');
  return '<iframe id="embedded-browser" frameborder="0"  src="'. url('alfresco/browser') .'"></iframe>';
}

function alfresco_browser_service_spaces() {

  $node_id = isset($_GET['node']) ? trim($_GET['node']) : '';
  $cache_clear = isset($_GET['cache']) ? trim($_GET['cache']) : '';

  if (!alfresco_valid_uuid($node_id)) {
    drupal_json_output(array());
    exit();
  }

  if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
    $cache_key = 'tree='. $node_id;
    if ($cache = _alfresco_browser_cache_get($cache_key, $cache_clear)) {
      drupal_json_output($cache);
      exit();
    }
  }

  alfresco_include('soap');

  $reference = 'workspace://SpacesStore/'. $node_id;
  $space = alfresco_soap_node_load($reference);
  $nodes = array();

  if (!$space) {
    if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
      _alfresco_browser_cache_set($cache_key, $nodes);
    }

    // node not found
    drupal_json_output($nodes);
    exit();
  }

  foreach ($space->getChildren() as $child) {
    /* @var $child_node AlfNode */
    $child_node = $child->child;

    if (alfresco_browser_is_folder_type($child_node->getType(), TRUE)) {
      $count = _aflresco_browser_count_folder($child_node);
      $nodes[] = array(
        'id'   => $child_node->id,
        'text' => trim($child_node->cm_name) . ' ('. $count['items'] .')',
        'qtip' => trim($child_node->cm_title),
        'leaf' => empty($count['folders']),
      );
    }
  }

  usort($nodes, 'alfresco_browser_node_sort');

  if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
    _alfresco_browser_cache_set($cache_key, $nodes);
  }

  drupal_json_output($nodes);
}

function _aflresco_browser_count_folder($node) {
  $folders = 0;
  $items = 0;

  foreach ($node->getChildren() as $child) {
    $child_node = $child->child;
    if (alfresco_browser_is_folder_type($child_node->getType())) {
      $folders ++;
    }
    else {
      $items ++;
    }
  }

  return array('folders' => $folders, 'items' => $items);
}

function alfresco_browser_service_items() {

  $node  = isset($_GET['node'])  ? trim($_GET['node']) : '';
  $query = isset($_GET['query']) ? trim($_GET['query']) : '';
  $start = isset($_GET['start']) ? intval($_GET['start']) : 0;
  $limit = isset($_GET['limit']) ? intval($_GET['limit']) : intval(variable_get('alfresco_browser_query_limit', 20));
  $cache_clear = isset($_GET['cache']) ? trim($_GET['cache']) : '';

  if (empty($query) && !alfresco_valid_uuid($node)) {
    drupal_json_output(array('total' => 0, 'rows' => array()));
    exit();
  }

  if (empty($query) && variable_get('alfresco_browser_remember_folder', 1)) {
    $_SESSION['alfresco_browser_last_folder'] = $node;
  }

  if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
    $cache_key = empty($query) ? 'node='. $node : 'query='. $query;
    if ($cache = _alfresco_browser_cache_get($cache_key, $cache_clear)) {
      $rows = $limit > 0 ? array_slice($cache, $start, $limit) : $cache;
      drupal_json_output(array('total' => count($cache), 'rows' => $rows));
      exit();
    }
  }

  alfresco_include('soap');
  $session = alfresco_soap_get_session();

  $nodes = array();
  $results = array();

  $namespace = AlfNamespaceMap::getInstance();

  if (!empty($query)) {
    $store = new AlfStore($session, 'SpacesStore');

    $search_mode = variable_get('alfresco_browser_search', 1);
    switch ($search_mode) {
      case 1:
        $results = $session->query($store, '+ISNODE:T +TYPE:"cm:content" +@cm\:name:"*'. $query .'*"');
        break;
      case 2:
        $results = $session->query($store, '+ISNODE:T +TYPE:"cm:content" +ALL:"*'. $query .'*"');
        break;
    }
  }
  else {
    $reference = 'workspace://SpacesStore/'. $node;
    $folder = alfresco_soap_node_load($reference);

    if (!$folder) {
      if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
        _alfresco_browser_cache_set($cache_key, $nodes);
      }
      drupal_json_output(array('total' => 0, 'rows' => $nodes));
      exit();
    }

    foreach ($folder->children as $child_assoc) {
      $child_node = $child_assoc->child;
      if (!alfresco_browser_is_folder_type($child_node->type)) {
        $results[] = $child_node;
      }
    }
  }

  if (count($results) == 0) {
    if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
      _alfresco_browser_cache_set($cache_key, $nodes);
    }
    drupal_json_output(array('total' => 0, 'rows' => $nodes));
    exit();
  }

  $module_path = drupal_get_path('module', 'alfresco_browser');

  foreach ($results as $child_node) {
    $node_ref = $child_node->__toString();
    $nid = alfresco_exists_node($node_ref);

    if ($child_node->cm_content instanceof AlfContentData) {
      $content_data = $child_node->cm_content;
    }
    else {
      $content_data = NULL;
    }

    $nodes[] = array(
      'id'          => $child_node->id,
      'nid'         => $nid ? url('node/'. $nid, array('absolute' => TRUE)) : '',
      'name'        => trim($child_node->cm_name),
      'size'        => isset($content_data) ? format_size($content_data->size) : '',
      'mimetype'    => isset($content_data) ? $content_data->mimetype : '',
      'icon'        => alfresco_icon_path($child_node->cm_name, FALSE),
      'author'      => !is_null($child_node->cm_author) ? $child_node->cm_author : '',
      'creator'     => !is_null($child_node->cm_creator) ? $child_node->cm_creator : '',
      'modified'    => format_date(strtotime($child_node->cm_modified), 'custom', 'Y-m-d H:i:s'),
      'created'     => format_date(strtotime($child_node->cm_created), 'custom', 'Y-m-d H:i:s'),
      'description' => trim($child_node->cm_description),
      'title'       => trim($child_node->cm_title),
      'type'        => $namespace->getShortName($child_node->type),
    );
  }

  if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
    _alfresco_browser_cache_set($cache_key, $nodes);
  }

  $rows = $limit > 0 ? array_slice($nodes, $start, $limit) : $nodes;
  drupal_json_output(array('total' => count($nodes), 'rows' => $rows));
}

function alfresco_browser_service_download() {

  $node = isset($_GET['node']) ? trim($_GET['node']) : '';
  $mode = isset($_GET['mode']) ? trim($_GET['mode']) : '';

  if (empty($node)) {
    drupal_not_found();
    exit();
  }

  $mode = ($mode == 'inline' || $mode == 'attachment') ? $mode : NULL;
  $reference = 'workspace://SpacesStore/'. $node;

  alfresco_file_transfer($reference, $mode);
}

/**
 *
 */
function alfresco_browser_service_upload() {
  if (!user_access('upload alfresco repository')) {
    exit();
  }

  $space = isset($_POST['space']) ? trim($_POST['space']) : '';
  $name = isset($_POST['name']) ? trim($_POST['name']) : '';
  $title = isset($_POST['title']) ? trim($_POST['title']) : '';
  $description = isset($_POST['description']) ? trim($_POST['description']) : '';
  $author = isset($_POST['author']) ? trim($_POST['author']) : '';

  /*
  $validators = array(
    'file_validate_size' => array(file_upload_max_size())
  );
  */
  alfresco_include('soap');

  if ($file = file_save_upload('file')) {
    $filepath = $file->uri;
    $mimetype = $file->filemime;
    // @see Fix for https://issues.alfresco.com/jira/browse/ETWOONE-400
    $encoding = 'UTF-8';
    $reference = 'workspace://SpacesStore/'. $space;

    $properties = array(
      'cm_title' => $title,
      'cm_description' => $description,
      'cm_author' => $author,
    );

    $replace = variable_get('alfresco_browser_file_exists', ALFRESCO_FILE_EXISTS_RENAME);
    $result = alfresco_soap_node_add($reference, $filepath, $name, $mimetype, $encoding, 'cm_content', $properties, $replace);

    if (!isset($result->error)) {
      $success = array('success' => TRUE);
    }
    else {
      $success = array('success' => FALSE, 'error' => $result->error);
    }

    if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
      global $user;
      $cid = "user=$user->uid";
      cache_clear_all($cid, 'cache_alfresco_browser', TRUE);
    }

    // drupal_json no funciona con subida de ficheros
    drupal_add_http_header('Content-Type', 'text/html;charset=UTF-8');
    echo drupal_json_encode($success);
    exit();
  }
}

/**
 *
 */
function alfresco_browser_service_create_space() {

  if (!user_access('upload alfresco repository')) {
    exit();
  }

  $space = isset($_POST['space']) ? trim($_POST['space']) : '';
  $name = isset($_POST['name']) ? trim($_POST['name']) : '';
  $title = isset($_POST['title']) ? trim($_POST['title']) : '';
  $description = isset($_POST['description']) ? trim($_POST['description']) : '';

  alfresco_include('soap');

  $reference = 'workspace://SpacesStore/'. $space;
  $properties = array(
    'cm_title' => $title,
    'cm_description' => $description,
  );

  $result = alfresco_soap_node_add_folder($reference, $name, 'cm_folder', $properties);
  if (!isset($result->error)) {
    $success = array('success' => TRUE);
  }
  else {
    $success = array('success' => FALSE, 'error' => $result->error);
  }

  if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
    global $user;
    $cid = "user=$user->uid;tree=$space";
    cache_clear_all($cid, 'cache_alfresco_browser', TRUE);
  }

  drupal_json_output($success);
}

/**
 *
 */
function alfresco_browser_service_delete() {

  if (!user_access('upload alfresco repository') || !drupal_valid_token($_GET['token'], 'alfresco_browser')) {
    exit();
  }

  $node = isset($_GET['node']) ? trim($_GET['node']) : '';
  $reference = 'workspace://SpacesStore/'. $node;

  alfresco_include('soap');

  $result = alfresco_soap_node_delete($reference);
  if (isset($result->error)) {
    drupal_json_output(array('success' => FALSE, 'error' => $result->error));
    exit();
  }

  if ($nid = alfresco_exists_node($reference)) {
    node_delete($nid);
  }

  if (variable_get('alfresco_browser_cache_enabled', TRUE)) {
    global $user;
    $cid = "user=$user->uid";
    cache_clear_all($cid, 'cache_alfresco_browser', TRUE);
  }

  drupal_json_output(array('success' => TRUE));
}

/**
 * Function used by usort to sort alfresco nodes by text.
 */
function alfresco_browser_node_sort($a, $b) {
  return strnatcmp($a['text'], $b['text']);
}

/**
 *
 */
function _alfresco_browser_cache_get($key, $clear = '') {
  global $user;

  switch ($clear) {
    case 'node':
      $cid = "user=$user->uid;$key";
      cache_clear_all($cid, 'cache_alfresco_browser');
      break;

    case 'all':
      $cid = "user=$user->uid";
      cache_clear_all($cid, 'cache_alfresco_browser', TRUE);
      break;

    default:
      $cid = "user=$user->uid;$key";
      $cache = cache_get($cid, 'cache_alfresco_browser');
      if ($cache && !empty($cache->data)) {
        return $cache->data;
      }
  }

  return FALSE;
}

/**
 *
 */
function _alfresco_browser_cache_set($key, $data) {
  global $user;

  $cid = "user=$user->uid;$key";
  cache_set($cid, $data, 'cache_alfresco_browser', CACHE_TEMPORARY);
}

/**
 * Devuelve el path del nodo de Alfresco en el repositorio.
 */
function _alfresco_browser_node_path($node) {
  static $paths = array();

  if (!isset($paths[$node->id])) {
    $home_path = variable_get('alfresco_browser_home', ALFRESCO_BROWSER_HOME);
    $home_node = alfresco_soap_node_load($home_path);

    $path = '/'. $node->id;
    $current_folder = $node;
    while ($parent_folder = $current_folder->getPrimaryParent()) {
      $id = $parent_folder->id;
      $parent_type = $parent_folder->getType();
      if ($parent_type == '{http://www.alfresco.org/model/system/1.0}store_root') {
        break;
      }
      $folder_id = $parent_folder->id;
      $path = '/'. $folder_id . $path;
      $current_folder = $parent_folder;
      if ($id == $home_node->id) {
        break;
      }
    }
    $paths[$node->id] = $path;
  }

  return $paths[$node->id];
}

/**
 * @todo: Enhanced suppport other folder types
 */
function alfresco_browser_is_folder_type($type, $ignore_systemfolder = FALSE) {
  static $cache = array();

  //$ctype = $session->getContentType(str_replace('_', ':', $type));
  if (!isset($cache[$type])) {
    $namespace = AlfNamespaceMap::getInstance();
    $short_type = $namespace->getShortName($type);
    if ($ignore_systemfolder && $short_type == 'cm_systemfolder') {
      return FALSE;
    }

    $folder_types = variable_get('alfresco_folder_types', array('cm_folder'));
    $cache[$type] = in_array($short_type, $folder_types);
  }

  return $cache[$type];
}

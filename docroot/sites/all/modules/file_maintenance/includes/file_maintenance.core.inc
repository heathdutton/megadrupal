<?php

/**
 *
 * @file
 * Core functionality.
 */

// $Id$

/**
 * Takes an array of file items and creates a renderable array.
 *
 * @param $files
 * @param $path
 */
function _file_maintenance_build_file_list(&$files, $uri=NULL) {

  /*
   * Table
   */
  $table = array(
    '#theme' => 'table',
    '#rows' => array(),
    '#prefix' => '<div id="file-list-table-container"><div id="file-list-table-header"></div><div id="file-list-table-scrollview">',
    '#suffix' => '</div></div>',
    '#sticky' => FALSE,
    '#attributes' => array(
      'class' => array('file-list-table', 'static-header'),
    ),
  );

  /*
   * File header
   */
  $header = array();

  if (_file_maintenance_file_list_column_active('fid')) {
    $header[] = array(
        'data' =>  t('FID'),
    );
  }
  if (_file_maintenance_file_list_column_active('thumbnail')) {
    $header[] = array(
        'data' =>  t(''),
    );
  }

  // Always visible
  $header[] = array(
    'data' =>  t('Filename'),
      'class' =>  'filename-column',
  );

  if (_file_maintenance_file_list_column_active('dimensions')) {
    $header[] = array(
        'data' =>  t('Dimensions'),
    );
  }
  if (_file_maintenance_file_list_column_active('size')) {
    $header[] = array(
        'data' =>  t('Size'),
    );
  }
  if (_file_maintenance_file_list_column_active('filesystem')) {
    $header[] = array(
        'data' =>  t('FS'),
        'title' =>  t('Filesystem'),
    );
  }
  if (_file_maintenance_file_list_column_active('database')) {
    $header[] = array(
        'data' =>  t('DB'),
        'title' =>  t('Database'),
    );
  }
  if (_file_maintenance_file_list_column_active('status')) {
    $header[] = array(
        'data' =>  t('Status'),
    );
  }
  if (_file_maintenance_file_list_column_active('usage')) {
    $header[] = array(
        'data' =>  t('Usage'),
      // TODO
        'class' =>  'usage-column',
    );
  }

  if ($uri !== NULL) {

    $total_filesize = 0;

    $img_path = _file_maintenance_get_image_path();
    $yes_image = render(theme('image', array('path' => $img_path . '/yes.png')));
    $no_image = render(theme('image', array('path' => $img_path . '/no.png')));

    $hidden_file_count = 0;

    foreach ($files as $file) {
      if (_file_maintenance_hide_item($file)) {
        $hidden_file_count++;
        continue;
      }

      $total_filesize += $file['#file']->filesize;

      if (_file_maintenance_file_list_column_active('thumbnail') || _file_maintenance_file_list_column_active('dimensions')) {
        $img = '';
        $image_info = array('width' => '', 'height' => '');

        if (!file_validate_is_image($file['#file'])) {
          $img = theme('image_style', array(
            'style_name' => 'file_maintenance_thumbnail',
            'path' => $file['#uri'],
            'attributes' => array(),
          ));
          $image_info = image_get_info(drupal_realpath($file['#uri']));
        }
        else {
          $ext = pathinfo($file['#filename'], PATHINFO_EXTENSION);
          $img = '<div class="extension-icon"><b style="vertical-align:middle;">' . $ext . '</b></div>';
        }
      }

      /*
       * File row.
       */

      $id = !empty($file['#file']->fid) ? 'file-fid-' . $file['#file']->fid : '';

      // $file['#filename'] is generated from uri and might differ from $file['#file']->filename.
      $filename = $file['#filename'];

      if (isset($file['#file']->filename) && $file['#file']->filename !== $file['#filename']) {
        $filename .= '<div class="original-filename">(' . $file['#file']->filename . ')</div>';
      }

      $classes = _file_maintenance_get_filename_classes($file, TRUE);

      $row = array(
        'data' => array(),
        'id' => $id,
        'class' => $classes,
      );

      /*
       * Data cells
       */

      // FID
      if (_file_maintenance_file_list_column_active('fid')) {
        $row['data'][] = array(
          'data' =>  $file['#file']->fid,
        );
      }

      // Thumbnail
      if (_file_maintenance_file_list_column_active('thumbnail')) {
        $row['data'][] = array(
          'data' => '<div class="file-icon">' . $img . '</div>',
        );
      }

      // Filename is always visible
      $row['data'][] = array(
        'data' => $filename,
        'class' => array('file-name'),
      );

      // Dimensions
      if (_file_maintenance_file_list_column_active('dimensions')) {
        $row['data'][] = array(
          'data' => $image_info['width'] || $image_info['height'] ? $image_info['width'] . ' x ' . $image_info['height'] : '',
          'class' => 'nowrap file-dimensions'
        );
      }

      // Size
      if (_file_maintenance_file_list_column_active('size')) {
        $row['data'][] = array(
          'data' => format_size($file['#file']->filesize),
          'class' => 'nowrap file-size'
        );
      }

      // Filesystem
      if (_file_maintenance_file_list_column_active('filesystem')) {
        $row['data'][] = array(
          'data' => $file['#in_filesystem'] ? $yes_image : $no_image,
          'class' => 'info-icon',
        );
      }

      // Database
      if (_file_maintenance_file_list_column_active('database')) {
        $row['data'][] = array(
          'data' => $file['#in_database'] ? $yes_image : $no_image,
          'class' => 'info-icon',
        );
      }

      // Status
      if (_file_maintenance_file_list_column_active('status')) {
        $row['data'][] = array(
          'data' =>  $file['#file']->status,
        	'class' =>  'db-status',
        );
      }

      // Usage
      if (_file_maintenance_file_list_column_active('usage')) {

        $usage_infos = _file_maintenance_build_usage_info($file);

        $row['data'][] = array(
            'data' =>  '<div class="usage-info"><ul>' . implode('', $usage_infos) . '</ul></div>',
        );
      }

      $table['#rows'][] = $row;
    }

    if (empty($table['#rows'])) {
      $table['#rows'][] = array(
        array(
          'data' => '<em>' . t('There are no files in this directory.') . '</em>',
          'colspan' => count($header)
        )
      );
    }

    $table['#header'] = $header;

    $count = count($files) - $hidden_file_count;

    // TODO: wrapper is not really necessary.
    // TODO: fix count files (don't count hidden files).
    $stats = array(
      '#markup' =>
        '<div class="file-list-header-wrapper"><div class="file-list-header">'
        . sprintf('<div style="float:right">Files: <b>%d</b>, Total size: <b>%s</b></div>', $count, format_size($total_filesize))
        . '<div class="current-uri">' . $uri . '</div>'
        . '</div></div>'
    );

  }
  else {
    /*
     * When $uri is NULL.
     */
    $table['#header'] = $header;

    $stats = array(
      '#markup' => '<div class="file-list-header-wrapper"><div class="file-list-header">&nbsp;</div></div>'
    );
  }

  $list = array(
    'stats' => $stats,
    'table' => $table
  );

  return array(
    'list' => $list,
    '#theme_wrappers' => array('file_list_wrapper'),
    //'#attributes' => array('class' => array('file-list'))
  );
}

function _file_maintenance_file_list_column_active($name) {
  return variable_get('file_maintenance_file_list_column_' . $name, FILE_MAINTENANCE_FILE_LIST_COLUMN_);
}

/**
 * Build stage: $prefix = TRUE.
 * Theme stage: $prefix = FALSE.
 *
 * @param $item
 * @param $prefix
 */
function _file_maintenance_get_filename_classes(&$item, $prefix=FALSE) {
  $prefix = $prefix ? '#' : '';
  $classes = array();
  if ($item[$prefix . 'in_database']) {
    if ($item[$prefix . 'in_filesystem']) {
      $classes[] = 'database-filesystem';
    }
    else {
      $classes[] = 'database-only';
    }
  }
  elseif ($item[$prefix . 'in_filesystem']) {
      $classes[] = 'filesystem-only';
  }
  if ($item[$prefix . 'is_temporary']) {
    $classes[] = 'temporary';
  }

  return $classes;
}

function _file_maintenance_get_image_path() {
  return drupal_get_path('module', 'file_maintenance') . '/images';
}

/**
 * Build usage info
 *
 * @param $module_usage
 */
function _file_maintenance_build_usage_info(&$file_item) {
  static $lang_list = NULL;
  static $module_list = NULL;

  if (!isset($lang_list)) {
    $lang_list = language_list();
  }

  $usage_infos = array();
    // TODO handle other entities
  foreach ($file_item['#usage'] as $module => &$module_usage) {
    foreach ($module_usage as $object_type => &$objects) {
      foreach ($objects as $id => $count) {
        switch ($object_type) {

          /*
           * Node.
           */
          case 'node':
            $node = node_load($id);
            $node_language = $node->language == LANGUAGE_NONE ? $GLOBALS['language'] : $lang_list[$node->language];

            $node_type_name = t(node_type_get_name($node));

            $link_text = $node->title;

            // TODO checkplain(title)?
            $link_title = $node_type_name . "($id/" . $node->language . ")[$count]";
            $link = array(
              '#theme' => 'link',
              '#path' => $object_type . '/' . $id,
              '#text' => $link_text,
              '#options' => array(
                'attributes' => array(
                  'title' => $link_title
                ),
                'language' => $node_language,
                'html' => FALSE
              ),
            );
            $usage_info = render($link);
            //$usage_infos[] = l($link_text, $object_type.'/'.$id, array('attributes' => array('title' => $link_title), 'language' => $node_language));

            break;

          /*
           * File.
           *
           * Another file is referenced. Highlight the file on mouseover.
           * TODO select file on click?
           */
          case 'file':

            if (!isset($module_list)) {
              $module_list = system_list('module_enabled');
            }

            // Find module name. Use the machine name if it can not be determined.
            $module_name = isset($module_list[$module]) ? $module_list[$module]->info['name'] . ' (' . $module . ')' : $module;

            // Mark missing files.
            // TODO: check at earlier stage.
            $class = db_query("SELECT fid FROM {file_managed} WHERE fid=:fid", array(':fid' => $id))->fetchField() ? '' : 'missing';

            //$module_name = $module_list[];
            $usage_info =
                "<b class=\"$class\" title=\"" . $module_name . "\""
                . " onmouseover=\"jQuery(this).addClass('highlight'); jQuery('#file-fid-" . $id . "').addClass('highlight')\""
                . " onmouseout=\"jQuery('#file-fid-" . $id . "').removeClass('highlight'); jQuery(this).removeClass('highlight')\""
                . ">$object_type($id)[$count]</b>";
                break;

          /*
           * Default.
           */
          default:
            $entity = entity_load_single($object_type, $id);
            $entity_info = entity_get_info($object_type);
            $usage_info = l($object_type . "($id)[$count]", $object_type . '/' . $id, array('attributes' => array('title' => $entity->name)));
        }
        $usage_infos[] = '<li>' . $usage_info . '</li>';
      }
    }
  }
  return $usage_infos;
}

// TODO: cache between directory_tree and file_list
function _file_maintenance_get_file_list($uri) {
  $files = array();
  if (file_uri_scheme($uri) !== FALSE) {
    _file_maintenance_read_files_database($files, 'list', $uri);
    _file_maintenance_read_files_filesystem($files, 'list', $uri);
    return _file_maintenance_build_file_list($files, $uri);
  }
  return _file_maintenance_build_file_list($files, $uri);;
}

// TODO set expanded ($dir_item['#expanded']);
function _file_maintenance_get_directory_tree($expanded_list=array()) {
  $directories = array();
  _file_maintenance_read_files_database($directories, 'tree');

  foreach (array('public', 'private', 'temporary') as $scheme) {
    _file_maintenance_read_files_filesystem($directories, 'tree', $scheme . '://');
  }

  _file_maintenance_mark_errors($directories);
  _file_maintenance_calc_files_size_deep($directories, $expanded_list);
  return $directories;
}

/**
 * Reads files from the filesystem. If $type is list, file items for the
 * given path are added. If $type is tree, directory items from the given
 * path and all subdirectories are added. Path must be a valid uri scheme.
 *
 * @param $files
 * @param $type
 * @param $uri
 * @param $recurse
 */
function _file_maintenance_read_files_filesystem(&$files, $type='list', $uri, $recurse=FALSE) {

  $recurse = $type == 'tree';

  // TODO check errors
  $filesystem_files = @file_scan_directory($uri, '/^[^\.].*/', array('recurse' => $recurse));

  foreach ($filesystem_files as $file) {

    // Skip directories.
    if (!is_dir(drupal_realpath($file->uri))) {

      $file->filesize = filesize(drupal_realpath($file->uri));

      $scheme = file_uri_scheme($file->uri) . '://';

      // Create file item.
      $file_item = _file_maintenance_create_file_item($type, $file, $file->filename);
      $file_item['#in_filesystem'] = TRUE;

      if ($type == 'tree') {
        if (!isset($files[$scheme])) {
          $parent = NULL;
          $files[$scheme] = _file_maintenance_create_dir_item($parent, $scheme, $scheme, 0, 'filesystem');
        }
        $files[$scheme]['#in_filesystem'] = TRUE;

        $dir = _file_maintenance_add_tree_file($files[$scheme], $file->uri, $file_item, 'filesystem', 1);
      }
      else {

        _file_maintenance_add_list_file($files, $file_item);
      }
    }

  }

}

/**
 * Reads the files from the database that reside in the directory location given with $uri.
 * If $uri is NULL, the entire files will be read.
 *
 * @param $files
 * @param $type Either 'list' or 'tree'.
 * @param $uri A directory location.
 */
function _file_maintenance_read_files_database(&$files, $type='list', $uri = NULL) {

  $query = new EntityFieldQuery();

  // TODO report invalid records
  $query = $query->entityCondition('entity_type', 'file')
                 ->propertyCondition('uri', '', '!=');

  if (isset($uri)) {
    $regex = '^' . preg_quote($uri) . (preg_match('/\/$/', $uri) ? '' : '/') . '[^/]+$';
    $query = $query->propertyCondition('uri', $regex, 'RLIKE');
  }

  $query = $query->propertyOrderBy('uri', 'ASC');

  $entities = $query->execute();

  if (isset($entities['file'])) {
    $files_managed = file_load_multiple(array_keys($entities['file']));

    foreach ($files_managed as $file) {

      $usage = file_maintenance_file_usage_list($file, TRUE);
      $scheme = file_uri_scheme($file->uri) . '://';
      $basename = basename($file->uri);

      // Create file item.
      $file_item = _file_maintenance_create_file_item($type, $file, $basename, $usage);
      $file_item['#in_database'] = TRUE;

      if ($type == 'tree') {
        if (!isset($files[$scheme])) {
          $parent = NULL;
          $files[$scheme] = _file_maintenance_create_dir_item($parent, $scheme, $scheme, 0, 'database');
        }
        $files[$scheme]['#in_database'] = TRUE;

        $dir = _file_maintenance_add_tree_file($files[$scheme], $file->uri, $file_item, 'database', 1);
      }
      else {

        _file_maintenance_add_list_file($files, $file_item);
      }
    }
  }

  return $files;
}

/**
 *
 * TODO optimize
 *
 * @param $dir_item
 * @param $property
 * @param $value
 */
function _file_maintenance_set_trail_property(&$dir_item, $property, $value) {
  if (!isset($dir_item[$property][$value])) {
    $dir_item[$property][$value] = TRUE;
    if (isset($dir_item['#parent'])) {
      _file_maintenance_set_trail_property($dir_item['#parent'], $property, $value);
    }
  }
}

function _file_maintenance_hide_item(&$item) {
  if ( (variable_get('file_maintenance_files_displayed', FILE_MAINTENANCE_FILES_DISPLAYED) == 'database_missing' && $item['#in_database'] && !$item['#dont_hide'])
    || (variable_get('file_maintenance_files_displayed', FILE_MAINTENANCE_FILES_DISPLAYED) == 'filesystem_missing' && $item['#in_filesystem'] && !$item['#dont_hide'])
    || (variable_get('file_maintenance_files_displayed', FILE_MAINTENANCE_FILES_DISPLAYED) == 'filesystem' && !$item['#in_filesystem'])
    || (variable_get('file_maintenance_files_displayed', FILE_MAINTENANCE_FILES_DISPLAYED) == 'database' && !$item['#in_database'])
    || (!variable_get('file_maintenance_show_temp_dirs', FILE_MAINTENANCE_SHOW_TEMP_DIRS) && $item['#is_temporary'])
  ) {
    hide($item);
    return TRUE;
  }
  return FALSE;
}

function _file_maintenance_mark_errors(&$dirtree) {

  foreach ($dirtree as $dirname => &$dir_item) {
    /*
     * If a file has a conflict, mark it in the trail.
     */
    $database_missing = $filesystem_missing = FALSE;
    $hidden_file_count = 0;

    foreach ($dir_item['#files'] as &$file_item) {
      if (!$database_missing && !$file_item['#in_database'] && !$file_item['#is_temporary']) {
        _file_maintenance_set_trail_property($dir_item, '#error', 'database_missing');
        $file_item['#error']['database_missing'] = TRUE;
        $database_missing = TRUE;
      }
      if (!$filesystem_missing && !$file_item['#in_filesystem']) {
        _file_maintenance_set_trail_property($dir_item, '#error', 'filesystem_missing');
        $file_item['#error']['filesystem_missing'] = TRUE;
        $filesystem_missing = TRUE;
      }
      if (_file_maintenance_hide_item($file_item)) {
        $hidden_file_count++;
      }
    }

    if (count($dir_item['#files']) > $hidden_file_count) {
       _file_maintenance_set_trail_property($dir_item, '#dont_hide', TRUE);
    }

    $dir_item['#file_count'] = count($dir_item['#files']) - $hidden_file_count;

    if (!empty($dir_item['#subdirs'])) {
      /*
       * Recursion.
       */
      list($file_count, $children_size) = _file_maintenance_mark_errors($dir_item['#subdirs']);
    }
  }
}

// TODO after traversing calculate by children
// TODO rename directory_tree_preprocess
function _file_maintenance_calc_files_size_deep(&$dirtree, &$expanded_list) {
  $total_dir_size = 0;
  $total_file_count = 0;
  foreach ($dirtree as $dirname => &$dir_item) {

    if (_file_maintenance_hide_item($dir_item)) {
      continue;
    }
/*
    $hidden_file_count = 0;
    foreach ($dir_item['#files'] as $file_item) {
      if (_file_maintenance_hide_item($file_item)) {
        $hidden_file_count++;
        continue;
      }
    }

    if (count($dir_item['#files']) > $hidden_file_count) {
       _file_maintenance_set_trail_property($dir_item, '#error', 'dont_hide');
    }
*/
    if (!empty($dir_item['#subdirs'])) {
      /*
       * Recursion.
       */
      list($file_count, $children_size) = _file_maintenance_calc_files_size_deep($dir_item['#subdirs'], $expanded_list);
    }
    else {
      $file_count = 0;
      $children_size = 0;
    }

    $dir_item['#files_size_deep'] = $dir_item['#files_size'] + $children_size;
    $total_dir_size += $dir_item['#files_size_deep'];

    $dir_item['#file_count_deep'] = $dir_item['#file_count'] + $file_count;
    $total_file_count += $dir_item['#file_count_deep'];

    $dir_item['#dir_id'] = _file_maintenance_get_unique_css_id($dir_item['#uri']);

    $dir_item['#expanded'] = in_array($dir_item['#dir_id'], $expanded_list);

  }
  return array($total_file_count, $total_dir_size);
}

// TODO cache
// TODO add wildcards
function _file_maintenance_is_temp_dir($uri) {
  $temp_dirs = variable_get('file_maintenance_temp_dirs', FILE_MAINTENANCE_TEMP_DIRS);
  foreach (explode("\n", $temp_dirs) as $temp_dir) {
    $temp_dir = preg_replace('/\/$/', '', trim($temp_dir));
    if (drupal_strlen($temp_dir) && preg_match('/^' . preg_quote($temp_dir, '/') . '(?:\/|$)/', $uri)) {
      return TRUE;
    }
  }
  return FALSE;
}

function _file_maintenance_add_list_file(&$files, $file) {

  if (isset($files[$file['#uri']])) {
    /*
     * Merge file
     */
    // File from database has precedence.
    if ($file['#in_database']) {
      unset($files[$file['#uri']]['#file']);
    }
    if (!$files[$file['#uri']]['#in_database']) {
      unset($files[$file['#uri']]['#in_database']);
    }
    if (!$files[$file['#uri']]['#in_filesystem']) {
      unset($files[$file['#uri']]['#in_filesystem']);
    }
    $files[$file['#uri']] += $file;
  }
  else {
    $files[$file['#uri']] = $file;
  }

}

/**
 * Adds a file item to the directory item tree.
 * Traverses down the tree and builds directory items where necessary according
 * to the path of the file.
 *
 * @param $dir_item
 * @param $uri
 * @param $file
 * @param $type filesystem or database
 * @param $level Current recursion level. Used only in recursive calls.
 * @param $cur_uri The current directory uri within the recursion. Used only in recursive calls.
 */

function _file_maintenance_add_tree_file(&$dir_item, $uri, &$file, $type, $level=0, $cur_uri=NULL) {

  if (!isset($cur_uri)) {
    // In the initial call $path will be an uri.
    $scheme = file_uri_scheme($uri);
    $path = file_uri_target(drupal_dirname($uri));

    // (The second slash will be appended further down.)
    $cur_uri = $scheme . ':/';
  }
  else {
    // Subsequent (recursive) calls.
    $path = $uri;
  }

  if (drupal_strlen($path)) {

    if (strpos($path, '/') === FALSE) {
      $next = $path;
      $pathrest = '';
    }
    else {
      list($next, $pathrest) = explode('/', $path, 2);
    }

    $cur_uri .= '/' . $next;

    // Create subdirectory item if it does not exists.
    if (!isset($dir_item['#subdirs'][$next])) {
      $dir_item['#subdirs'][$next] = _file_maintenance_create_dir_item($dir_item, $next, $cur_uri, $level, $type);
    }

    // Mark item occurance (database/filesystem).
    $dir_item['#subdirs'][$next]['#in_' . $type] = TRUE;

    /*
     * Recursion.
     */
    _file_maintenance_add_tree_file($dir_item['#subdirs'][$next], $pathrest, $file, $type, $level+1, $cur_uri);

  }
  else {
    /*
     * End of path traversal reached.
     */
    if (!isset($dir_item['#files'][$file['#uri']])) {
      // On first occurance of file, add it's size to the directorie's files_size.
      $dir_item['#files_size'] += $file['#file']->filesize;

      $file['#level'] = $level;
    }

    _file_maintenance_add_list_file($dir_item['#files'], $file);

    return $dir_item;
  }
}

function _file_maintenance_create_dir_item(&$parent, $dirname, $uri, $level, $type) {
  $is_temporary = _file_maintenance_is_temp_dir($uri);
  return array(
    '#theme' => 'file_maintenance_tree_directory',
    '#subdirs' => array(),
    '#files' => array(),
    '#dirname' => $dirname,
    '#uri' => $uri,
    '#level' => $level,
    '#file_count' => 0,
    '#files_size' => 0,
    '#icon' => $is_temporary ? 'folder_temporary' : 'folder',
    '#parent' => &$parent,
    '#expanded' => FALSE,
    '#in_database' => FALSE,
    '#in_filesystem' => FALSE,
    '#is_temporary' => $is_temporary,
    '#error' => array(),
    '#dont_hide' => FALSE,
  );
}

/**
 *
 * @param $type
 * @param $file
 * @param $usage
 * @param $basename
 */
function _file_maintenance_create_file_item($type, $file, $basename, &$usage=NULL) {
  return array(
    '#theme' => '' . $type . '_file',
    '#file' => $file,
    '#usage' => $usage,
    '#uri' => $file->uri,
    '#filename' => $basename,
    '#in_database' => FALSE,
    '#in_filesystem' => FALSE,
    '#is_temporary' => _file_maintenance_is_temp_dir(drupal_dirname($file->uri)),
    '#error' => array(),
    '#dont_hide' => FALSE,
  );
}

/**
 * Generates a unique id that can be used as css identifier.
 *
 * @param $string
 */
function _file_maintenance_get_unique_css_id($string) {

  // Valid characters in a CSS identifier are:
  // - the hyphen (0x2D)
  // - a-z (0x30 - 0x39)
  // - A-Z (0x41 - 0x5A)
  // - the underscore (0x5F)
  // - 0-9 (0x61 - 0x7A)
  // - ISO 10646 characters 0xA1 and higher
  // We escape any character not in the above list plus the hyphen and the underscore.
  return preg_replace_callback(
    // (hyphen (\x{2D}) and underscore (\x{5F}) are removed so they get escaped as well.)
    '/[^\x{30}-\x{39}\x{41}-\x{5A}\x{61}-\x{7A}\x{A1}-\x{FF}]/',
    create_function(
      '$matches',
      'return sprintf("-%02x", ord($matches[0]));'
    ),
    $string
  );

}




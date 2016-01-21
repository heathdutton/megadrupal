<?php
// $Id$

/**
 * @file
 * OM Subthemer Utilities
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 * This is a library of common functions:
 *   - om_subthemer_preview()
 *
 */
 
 
/**
 * @constants
 * 
 * Default is neccessary on creating new subtheme for all the variables to load
 * Admin can set this up on admin settings
 * by default, this module creates files/om_subthemes
 *
 */ 
define('OM_SUBTHEMER_FILE_DIRECTORY', file_directory_path() . '/om_subthemes');
define('OM_SUBTHEMER_PATH', drupal_get_path('module', 'om_subthemer'));

// Initialize global variables 
$_om_subthemer_layout_structure  = array();
$_om_subthemer_layout_content    = array(); 
$_om_subthemer_count             = NULL; 
$_om_subthemer_regions           = array();
$_om_subthemer_content_variables = array();
$_om_subthemer_region_classes    = NULL;
$_om_subthemer_blocks            = array();
$_om_subthemer_variable          = array();



/**
 * Implementation of hook_init().
 *
 */
function om_subthemer_init() {
  global $_om_subthemer_variable;
  
  $_om_subthemer_variable = variable_get('om_subthemer', array());
}


/**
 * OM Type settings.
 *
 * This is a shortcut for getting node
 * current settings values
 *
 */
function om_type_settings_get($type = NULL, $setting = NULL) {
  global $_om_subthemer_variable;
  $om_subthemer = $_om_subthemer_variable; 
   
  $out = '';
  if (!empty($setting) && isset($om_subthemer[$type][$setting])) {
    $out .= $om_subthemer[$type][$setting];
  }
  elseif ($setting == 'base_theme') {
    $out .=  'om';
  }
  elseif ($setting == 'default_skin') {
    $out .=  'starter';
  }
    
  return $out;
}


/**
 * OM Subthemer preview
 *  
 * This uses an iframe to see the preview of the actual file created,
 * which means all files must be created, updated before it can appear
 * on this page, sometimes due to browser caching you may have to
 * refresh to see the updated preview
 *
 */
function om_subthemer_preview(&$node) {

  // add some default formatting
  drupal_add_css(OM_SUBTHEMER_PATH . '/css/om_subthemer.css');

  // 1) set om_subthemes folder
  //    creates files/om_subthemes/      
  om_directory(OM_SUBTHEMER_FILE_DIRECTORY);
  
  // 2) set user folder 
  //    creates files/om_subthemes/u10001d10001
  om_subthemer_user_folder($node);

  // 3) create default files for new nodes
  //    zip and index.html file must exist for this preview to work
  //    so they will be created using default values if they don't exist
  if (!is_file($node->om_user_files . '.zip') && !is_file($node->om_user_files . '/index.html')) {
    include_once OM_SUBTHEMER_PATH . '/inc/om_subthemer.write.inc';
  
    // 3.1) copy current subtheme files to user subtheme folder
    //      - gets default skin and all its content
    om_current_skin_copy($node); 
    
    // 3.2) get layout for variables
    //      - generates the default layout if no layout exists
    //        using default.layout file from the default skin
    om_process_layout($node);
    
    // 3.3) write om subtheme
    //      - make a zip copy of the new subtheme, already download
    //        when you click download link
    om_file_write($node);   
  }
  
  // base_path() has to be included here to correct the reading of files
  $om_zip = $node->om_user_files . '.zip';  
  $om_index = $node->om_user_files . '/index.html';
  
  // download link, preview link, iframe preview
  $out = '';
  $out .= '<div class="om-actions">';
  $out .= l(t('Download'), $om_zip, array('attributes' => array('class' => array('om-btn om-download'))));
  $out .= l(t('Fullscreen'), $om_index, array('attributes' => array('class' => array('om-btn om-fullscreen'))));  
  $out .= '</div>';
  $out .= '<iframe src="' . base_path() . $om_index . '" width="100%" height="500"></iframe>';

  return $out;
}


/**
 * Generates user unique folder
 * - combination of user id and node id
 * - creates this folder if it doesn't exist 
 */
function om_subthemer_user_folder(&$node) {
  
  // combination of user id and node id
  $om_ucode = 'u' . ($node->uid + 1000000);
  $om_dcode = 'd' . ($node->nid + 1000000);
  
  // create user subtheme folder to files/om_subtheme/
  $node->om_user_folder  = $om_ucode . $om_dcode;
  
  // the whole path now becomes user files
  $node->om_user_files = OM_SUBTHEMER_FILE_DIRECTORY . '/' . $node->om_user_folder;
  om_directory($node->om_user_files);      
}


/**
 * OM Subthemer directory creator
 * - make sure the directory exists
 *
 */
function om_directory($dir = NULL) {
  //print $dir . '<br />';
  if ($dir != NULL) (is_dir($dir)) ? '': mkdir($dir);
}


/**************************************************************************************
 * Skin
 **************************************************************************************/

/**
 * copy reset.css and base.css from none base theme
 * if none selected, otherwise delete these files 
 * if existed on user created theme
 *
 */
function om_basetheme_none_copy($node) {
    
  $base_theme_used = om_type_settings_get($node->type, 'base_theme');
  
  if ($base_theme_used == 'none') {  
    $path = OM_SUBTHEMER_PATH .'/base_themes/none/css';

    $files = array();   
    // copy files
    om_files_get($path, $files);
    //dsm($files);
    foreach ($files as $key => $theme_file) {
      om_file_copy($node, $theme_file, 'css');  
    } 
  }
  else {
    $dir = $node->om_user_files . '/css';

    $files = scandir($dir);

    foreach ($files as $file) {
      if (($file == 'reset.css') || ($file == 'base.css')) {
        $file = $dir . '/' . $file;
        unlink($file);
      }
    }    
  }
}


/**
 * copy all current skin files to current user subtheme folder
 *
 */
function om_current_skin_copy($node) {

  // get skin name
  om_skin_load($node);
  
  $om_subtheme_path = OM_SUBTHEMER_PATH .'/skins/' . $node->om_current_skin;
  
  if (!is_dir($om_subtheme_path)) {
    $om_subtheme_path = base_path() . 'sites/all/libraries/om_skins/' . $node->om_current_skin;
  }
  //else {
    //drupal_set_message(t('Skin doesn\'t exist.'), 'error');
  //}
  
  
  // creates folders
  $folders = array('css', 'css/images', 'js');
  foreach ($folders as $key => $folder) {
    om_directory($node->om_user_files . '/' . $folder);
  }
    
  $files = array();   
  // copy files
  om_files_get($om_subtheme_path, $files);
  //dsm($files);
  foreach ($files as $key => $theme_file) {
    om_file_copy($node, $theme_file, $node->om_current_skin);  
  } 
}


/**
 * Loads current or default skin 
 *
 */ 
function om_skin_load(&$node) {

  $default_skin = om_type_settings_get($node->type, 'default_skin');

  // get this node its skin
  $sql = "SELECT nid, skin FROM {om_subthemer} WHERE nid = :nid"; 
  
  $result = db_query($sql, array(':nid' => $node->nid));  
       
  $out = $result->fetchObject();              
  
  if (!is_object($out)) { 
    // add default on new nodes
    db_insert('om_subthemer')
      ->fields(array('nid', 'skin'))
      ->values(array(
        'nid'  => $node->nid,
        'skin' => $default_skin,                    
      ))
      ->execute();
                      
    $node->om_current_skin = $default_skin;    
  }
  else {
    $node->om_current_skin = $out->skin;
  }
}


/**
 * Scan and load all available base themes 
 *
 */
function om_available_base_themes() {
  
  // scan available themes
  $base_themes = om_folders_get(OM_SUBTHEMER_PATH . '/base_themes');
  
  // additional base themes
  $om_base_themes_library = 'sites/all/libraries/om_base_themes';
  
  if (is_dir($om_base_themes_library)) $base_themes = array_merge($base_themes, om_folders_get($om_base_themes_library));

  return $base_themes;
}


/**
 * Scan and load all available skins
 *
 */
function om_available_skins() {
  
  // scan available skins  
  $skins = om_folders_get(OM_SUBTHEMER_PATH . '/skins');  

  // additional skins
  $om_skins_library = 'sites/all/libraries/om_skins';
  
  if (is_dir($om_skins_library)) $skins = array_merge($skins, om_folders_get($om_skins_library));
  
  return $skins;
}


/**************************************************************************************
 * File Functions
 **************************************************************************************/

/**
 * This was removed from D7
 * adding it again, it should be in public space
 *
 */
function file_directory_path() {
  variable_get('file_public_path', conf_path() . '/files');
  return conf_path() . '/files';
}

 
/**
 * Delete user OM Subtheme
 * - file write
 *
 */
function om_files_delete($dir) {
  $files = scandir($dir);
  if ($files[0] == '.') array_shift($files); // removes '.'
  if ($files[0] == '..') array_shift($files); // removes '..'

  foreach ($files as $file) {
    $file = $dir . '/' . $file;
    if (is_dir($file)) {
       om_files_delete($file);
       if (is_dir($file)) rmdir($file);
    } 
    else {
       unlink($file);
    }
  }
  rmdir($dir);
}


/**
 * OM Subthemer file creator
 * 
 * @param $content
 *   Text content
 * @param $dir
 *   Directory where to start scanning for files
 * @param $file_name
 *   Name of the file to be created
 *
 */
function om_file_create($content = NULL, $dir = NULL, $file_name = NULL) {
  // should not process without content
  if ($content) {
    om_directory($dir);
    $file = $dir . '/' . $file_name;
    file_put_contents($file, $content);
  }
}


/**
 * OM Subthemer files get
 * 
 * @param $dir
 *   Directory where to start scanning for files
 * @param $files
 *   Files to be returned
 *   
 */
function om_files_get($dir = NULL, &$files = array()) {
  $paths = scandir($dir);
  $excluded_files = array('.', '..', '.cvs', '.svn', '.git');
  foreach ($paths as $key => $val) {
    (!in_array($val, $excluded_files)) ? (is_dir($dir . '/' . $val)) ? om_files_get($dir . '/' . $val, $files): $files[] = $dir . '/' . $val: ''; 
  }
}


/**
 * OM Subthemer folders get
 * 
 * @param $dir
 *   Directory where to start scanning for files
 * @param $files
 *   Folders to be returned
 *   
 */
function om_folders_get($dir = NULL) {
  $paths = scandir($dir);
  $folders = array();
  $excluded_files = array('.', '..', '.cvs', '.svn', '.git');
  foreach ($paths as $key => $val) {
    if (!in_array($val, $excluded_files)) (is_dir($dir . '/' . $val)) ? $folders[$val] = ucwords(preg_replace('/_/', ' ', $val)): ''; 
  }
  return $folders;
}


/**
 * OM Subthemer file copy
 * 
 * @param $node
 * @param $file
 *   - file with path collected from om_subthemer/skin/%skin | subthemer/base_themes/templates 
 * @param $last_folder
 *   - file root folder
 *
 */
function om_file_copy($node, $file = NULL, $last_folder = NULL) {

  // this is the way to delete the absolute path, /sites/...
  $source_path = explode('/', $file);
  while ($source_path[0] != $last_folder) {
    // deletes path origins
    array_shift($source_path);
  }
  
  // delete last path which is the user code, e.g. u1000001d1000001
  // which should not happen if copying to /css
  if ($last_folder != 'css') array_shift($source_path);
  
  $file_path = implode('/', $source_path);
  
  // add the new path where this file will be copied to
  $new_file = $node->om_user_files . '/' . $file_path;
  //dsm($new_file);
  if (!copy($file, $new_file)) drupal_set_message(t("Failed to copy $new_file."));;

}


/**************************************************************************************
 * Layout Functions
 **************************************************************************************/

/**
 * Adds additional variables to node
 * - om_layout_html_content
 * - om_layout_php_content
 *
 * This are required to generate layouts for various forms and files
 *
 */
function om_process_layout(&$node) {  
  //global $_om_subthemer_layout_content;
  global $_om_subthemer_content_variables;
  global $_om_subthemer_regions;
  global $_om_subthemer_region_classes;
  global $_om_subthemer_blocks;
  global $_om_subthemer_variable;
    
  // need this to generate the layout
  include_once OM_SUBTHEMER_PATH . '/inc/om_subthemer.layout.inc';
  
  // html and php values
  $_om_subthemer_content_variables = om_get_variables($node->type, $node);

  // layout array
  $om_divs = om_layout_load($node->nid);

  // returns default if empty
  om_default_layout($node, $om_divs);
  
  // copy base theme template files
  om_base_theme_templates($node); 
    
  // get content type base theme
  $om_subthemer    = $_om_subthemer_variable;  
  $type_settings   = isset($om_subthemer[$node->type]) ? $om_subthemer[$node->type]: array(); 
  $base_theme_used = om_type_settings_get($node->type, 'base_theme');
  
  // copy some css from base_themes/none/css,
  // if you're making a standalone theme
  om_basetheme_none_copy($node);
  
  // get php output
  $node->om_layout_php_content = om_subthemer_layout($om_divs[0], $type_settings, $base_theme_used);

  // 1) update node regions based on region created by the layout
  om_regions_update($_om_subthemer_regions, $node->nid);
  
  // 2) update dummy blocks assigned to each subtheme node
  $_om_subthemer_blocks = om_blocks_get($node->nid);
  
  // 3) get html output
  $node->om_layout_html_content = om_subthemer_layout($om_divs[0], $type_settings, 'html');  
  
  // 4) add region classes, this is for OM Base Theme
  $node->om_region_classes = $_om_subthemer_region_classes;      
}

/**
 * Loads current layout from the db
 * and updates global variables
 *
 */ 
function om_layout_load($nid = NULL) {
  global $_om_subthemer_layout_content; // needed by layout
  global $_om_subthemer_count;
   
  $sql = "SELECT cid, pid, tagid, type, iw, weight, nid 
          FROM {om_subthemer_layout} 
          WHERE nid = :nid 
          ORDER BY weight ASC";
  
  $result = db_query($sql, array(':nid' => $nid));

  $layout = array();
  while ($record = $result->fetchObject()) {
    $layout[$record->pid][$record->cid]['tagid']  = $record->tagid;
    $layout[$record->pid][$record->cid]['type']   = $record->type;     
    $layout[$record->pid][$record->cid]['iw']     = $record->iw;                  
    $layout[$record->pid][$record->cid]['weight'] = $record->weight;                  
    $layout[$record->pid][$record->cid]['cid']    = $record->cid;                  
    $layout[$record->pid][$record->cid]['pid']    = $record->pid;                  
  }
  
  if (!empty($layout)) {
    // Number of divs
    $_om_subthemer_count = count($layout);  

    //correcting the order of parent and child divs
    $om_correct_order = _om_order($layout[0]);
    $om_correct_order = array_reverse(explode(' ', $om_correct_order));
  
    // change global variable
    $_om_subthemer_layout_content = $layout;
    //dsm($om_layout_content);   
  } 
  return $layout;
}

 
/**
 * Default layout
 *  
 * Generates default layout if layout has been deleted or the node is newly created
 *
 */
function om_default_layout($node, &$om_divs) {
 
  // add default values if the node is new
  if (empty($om_divs)) {
    $om_divs = om_base_theme_file($node->type, 'layout');
    
    // process uploading new layout to database
    om_subthemer_layout_upload($om_divs, $node->nid);
    
    // has to be called again for the global variable $om_layout_content
    $om_divs = om_layout_load($node->nid);    
    
    drupal_set_message(t('Default layout has been generated, you may need to refresh your browser to see this layout.'));
  } 
}


/**
 * OM Subthemer base theme files
 * 
 * looks for default.layout file in the current subtheme skin folder
 *
 * @param $type - node type
 * @param $file - file/variable name
 *
 */
function om_base_theme_file($type = NULL, $file = NULL) {
  
  // get default base theme for this node
  $default_base_theme = om_type_settings_get($type, 'base_theme');
  
  $file_name = OM_SUBTHEMER_PATH .'/base_themes/' . $default_base_theme . '/' . $default_base_theme . '.' . $file;

  if (!file_exists($file_name)) {
    $file_name = 'sites/all/libraries/om_base_themes/' . $default_base_theme . '/' . $default_base_theme . '.' . $file;
  }
  //else {
    //drupal_set_message(t('File doesn\'t exist.'), 'error');
  //}
  
  $file_content = file_get_contents($file_name);
  
  // converts file to array
  ob_start();
  eval($file_content);
  ob_end_clean();
  
  return $$file;
}


/**
 * copy all base theme template files to current user subtheme folder
 *
 */
function om_base_theme_templates($node) {

  // get default base theme for this node
  $default_base_theme = variable_get('om_subthemer_base_theme_'. $node->type, 'om');
  
  $templates_path = OM_SUBTHEMER_PATH .'/base_themes/' . $default_base_theme . '/templates';

  if (!is_dir($templates_path)) {
    $templates_path = 'sites/all/libraries/om_base_themes/' . $default_base_theme . '/templates';
  }
  //else {
    //drupal_set_message(t('Base Theme doesn\'t exist.'), 'error');
  //}
      
  $files = array();   
  // copy files
  om_files_get($templates_path, $files);
  //dsm($files);
  foreach ($files as $key => $template) {
    om_file_copy($node, $template, 'templates');  
  } 
}


/**
 * Uploads default layout to db
 * 
 * Process uploading of new layout to db,
 * this is called only for default layouts
 *
 */
function om_subthemer_layout_upload($default = array(), $nid = NULL) {
  //$default_variables = array();
  //$om_variables = array();
  
  if (!empty($default)) {

    $weight = 0;
    foreach ($default as $cid => $prop) {
      $tagid = ucwords($prop['tagid']);
      $type = om_reserved_variables($tagid, $prop['type']);

      db_insert('om_subthemer_layout')
        ->fields(array('cid', 'pid', 'tagid', 'type', 'iw', 'weight', 'nid'))
        ->values(array(
          'cid'    => $prop['cid'],
          'pid'    => $prop['pid'],
          'tagid'  => $tagid,
          'type'   => $type,
          'iw'     => $prop['iw'],
          'weight' => $weight++,
          'nid'    => $nid,                    
         ))
         ->execute();
    }
  }
}


/**
 * Uploads default settings to db
 * 
 * Process uploading of new settings to db,
 * this is called only for default settings
 *
 */
function om_subthemer_settings_upload($settings = array(), $type = NULL) {
  
  if (!empty($settings)) {
    foreach ($settings as $sid => $prop) {
      $variable   = ucwords(trim($prop['variable']));
      $value_html = str_replace('\"', '"', $prop['value_html']);
      $value_php  = str_replace('\"', '"', $prop['value_php']);
      $source     = ucwords(trim($prop['source']));
      
      db_insert('om_subthemer_vars')
        ->fields(array('variable', 'value_html', 'value_php', 'source', 'type'))
        ->values(array(
          'variable'   => $variable,
          'value_html' => $value_html,
          'value_php'  => $value_php,
          'source'     => $source,
          'type'       => $type,
         ))
         ->execute();
    }
  }
}


/**
 * Uploads blocks to db
 * 
 * Process uploading of new blocks to db,
 *
 */
function om_subthemer_blocks_upload($blocks = array(), $nid = NULL) {
  
  if (!empty($blocks)) {
    foreach ($blocks as $bid => $prop) {
      $title   = ucwords(trim($prop['title']));
      $content = str_replace('\"', '"', $prop['content']);
      $module  = ucwords(trim($prop['module']));
      $region  = strtolower(trim(preg_replace('/ /', '_', $prop['region'])));
      
      db_insert('om_subthemer_blocks')
        ->fields(array('title', 'content', 'module', 'region', 'weight', 'nid'))
        ->values(array(
          'title'   => $title,
          'content' => $content,
          'module'  => $module,
          'region'  => $region,
          'weight'  => $prop['weight'],
          'nid'     => $nid,          
         ))
         ->execute();      
    }
  }
}


/**
 * Regions assigned to each subtheme
 * 
 *
 * @return $regions - array('header_block' => 'Header Block', ...);
 */
function om_regions_get($nid = NULL) {  
  $sql = "SELECT regions 
          FROM {om_subthemer} 
          WHERE nid = :nid"; 

  $result = db_query($sql, array(':nid' => $nid));

  $regions_var = $result->fetchObject();
  
  $regions = array();
  if (is_object($regions_var) && isset($regions_var->regions)) {  
    ob_start();
    eval($regions_var->regions);
    ob_end_clean();
    return $regions;
  }
}


/**
 * Blocks assigned to each subtheme
 * 
 *
 * @return $blocks - array('header_block' => '<div ...);
 */
function om_blocks_get($nid = NULL) {  
  $sql = "SELECT bid, title, content, module, region, weight, nid 
          FROM {om_subthemer_blocks} 
          WHERE nid = :nid 
          ORDER BY weight ASC";

  $result = db_query($sql, array(':nid' => $nid));
    
  $blocks = array();
  while ($record = $result->fetchObject()) {
    // this will add all blocks for each region
    $blocks[$record->region][] = $record->content;
  }
  return $blocks;
}
  

/**
 * Drupal's default page.tpl.php and om variables
 * 
 * All these variables mush be set to variable type
 * instead of column, row
 *
 * @return $type - column, region, variable
 */
function om_reserved_variables($tagid = NULL, $type = NULL) {
  // drupal variables
  $vars = array();
     
  // Variables:  
  $vars[] = 'CSS';          // An array of CSS files for the current page.
  $vars[] = 'Language';     // $language: (object) The language the site is being displayed in.
                            // $language->language contains its textual representation.
                            // $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
  $vars[] = 'RDF Namespaces'; // All the RDF namespace prefixes used in the HTML document.    
  $vars[] = 'GRDDL Profile'; // $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.                          
  $vars[] = 'Head Title';   // A modified version of the page title, for use in the TITLE tag.
  $vars[] = 'Head Title Array'; // $head_title_array: (array) An associative array containing the string parts
                            // that were used to generate the $head_title variable, already prepared to be
                            // output as TITLE tag. The key/value pairs may contain one or more of the
                            // following, depending on conditions:
                            // - title: The title of the current page, if any.
                            // - name: The name of the site.
                            // - slogan: The slogan of the site, if any, and if there is no title.  
  $vars[] = 'Head';         // Markup for the HEAD element (including meta tags, keyword tags, and so on).
  $vars[] = 'Styles';       // Style tags necessary to import all CSS files for the page.
  $vars[] = 'Scripts';      // Script tags necessary to load the JavaScript files and settings for the page.
  $vars[] = 'Page Top';     // Initial markup from any modules that have altered the
                            // page. This variable should always be output first, before all other dynamic content.
  $vars[] = 'Page';         // The rendered page content.
  $vars[] = 'Page Bottom';  // Final closing markup from any modules that have altered the
                            // page. This variable should always be output last, after all other dynamic content.
  $vars[] = 'Classes';      // String of classes that can be used to style contextually through CSS.

  // General utility variables: 
  $vars[] = 'Base Path';    // The base URL path of the Drupal installation. At the very least, this will always default to /.
  $vars[] = 'Directory';    // The directory the theme is located in, e.g. themes/garland or themes/garland/minelli.
  $vars[] = 'Is Front';     // TRUE if the current page is the front page.
  $vars[] = 'Logged In';    // TRUE if the user is registered and signed in.
  $vars[] = 'Is Admin';     // TRUE if the user has permission to access administration pages. 

  // Site identity:
  $vars[] = 'Front Page';   // The URL of the front page. Use this instead of $base_path, when linking to the front page. This includes the language domain or prefix.
  $vars[] = 'Logo';         // The path to the logo image, as defined in theme configuration.
  $vars[] = 'Site Name';    // The name of the site, empty when display has been disabled in theme settings.
  $vars[] = 'Site Slogan';  // The slogan of the site, empty when display has been disabled in theme settings.
 
  // Navigation:
  $vars[] = 'Main Menu';    // (array): An array containing primary navigation links for the site, if they have been configured.
  $vars[] = 'Secondary Menu'; // (array): An array containing secondary navigation links for the site, if they have been configured.
  $vars[] = 'Breadcrumb';   // The breadcrumb trail for the current page.
   
  // Page content (in order of occurrence in the default page.tpl.php):
  $vars[] = 'Title Prefix'; // (array): An array containing additional output populated by modules, intended to be displayed in front of the main title tag that appears in the template.
  $vars[] = 'Title';        // The page title, for use in the actual HTML content.
  $vars[] = 'Title Suffix'; // (array): An array containing additional output populated by modules, intended to be displayed after the main title tag that appears in the template.
  $vars[] = 'Messages';     // HTML for status and error messages. Should be displayed prominently.
  $vars[] = 'Tabs';         // Tabs linking to any sub-pages beneath the current page (e.g., the view and edit tabs when displaying a node).
  $vars[] = 'Action Links'; // (array): Actions local to the page, such as 'Add menu' on the menu administration interface.
  $vars[] = 'Feed Icons';   // A string of all feed icons for the current page. 
  $vars[] = 'Node';         // The node object, if there is an automatically-loaded node associated
                            // with the page, and the node ID is the second argument in the page's path
                            // (e.g. node/12345 and node/12345/revisions, but not comment/reply/12345).
 
  // Regions:
  $vars[] = 'Help';         // Dynamic help text, mostly for admin pages.
  //$vars[] = 'Header Block'; // Items for the header region.
  //$vars[] = 'Menu Bar';     // Items for the header region.
  //$vars[] = 'Highlighted';  // Items for the highlighted content region. 
  //$vars[] = 'Sidebar First'; // Items for the first sidebar.
  //$vars[] = 'Content';      // The main content of the current page.
  //$vars[] = 'Sidebar Second'; // Items for the second sidebar. 
  //$vars[] = 'Footer';       // Items for the footer region.
 
  // corrects type
  if (in_array($tagid, $vars)) $type = 'variable';
  return $type;
}

/**
 * Update regions array
 *
 */
function om_regions_update($regions = array(), $nid = NULL) {
  if (!empty($regions)) {
    $update = '';
    foreach ($regions as $key => $region) {
      $region_key = strtolower(preg_replace('/ /', '_', $region));
      $update .= '$regions[\'' . $region_key . '\'] = \'' . $region . '\';';
    }
    db_update('om_subthemer')
      ->fields(array('regions' => $update))
      ->condition('nid', $nid, '=')
      ->execute();    
  }
}


/**
 * Arrange layout based on child parent relationships
 *
 * @see om_subthemer.layout_edit.inc
 *
 */
function _om_order($order = array()) {
  //static $count = 0;
  global $_om_subthemer_layout_content;
  $out = '';
  if (is_array($order)) {
    foreach ($order as $cid => $tagid) {
      //$count++;
      //print $count;
      if (isset($_om_subthemer_layout_content[$cid])) {
        $out .= _om_order($_om_subthemer_layout_content[$cid]);
      }
      $out .= ' ' . $cid;
    }
  }
  return $out;
}


/**
 * Insert new blank rows
 *
 */
function _om_new_rows(&$query = NULL, $times = 1, $record = NULL, $form_type = NULL) {
  $form_process = '_om_' . $form_type;
  for ($i = 0; $i < $times; $i++) {
    $query[] = $form_process($record);  
  }
  return $query;
}


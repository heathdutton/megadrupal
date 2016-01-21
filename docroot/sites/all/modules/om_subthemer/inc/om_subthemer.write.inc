<?php
// $Id$

/**
 * @file
 * OM Subthemer Write
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 * All file and directory creations are handled here 
 *
 */


/**
 * Writes:
 * - index.html
 * - page.tpl.php
 * - subtheme.info
 * - subtheme.zip
 *
 */
function om_file_write($node) {  

  // get content type base theme
  $om_subthemer    = variable_get('om_subthemer', array());  
  $type_settings   = isset($om_subthemer[$node->type]) ? $om_subthemer[$node->type]: array(); 
  $base_theme_used = isset($type_settings['base_theme']) ? $type_settings['base_theme']: 'om';
  
  // create directory and index html file  
  $index_html = om_index_html_file($node);
  om_file_create($index_html, $node->om_user_files , 'index.html');

  // create page.tpl.php file  
  $page_tpl = om_page_template($node);
  om_file_create($page_tpl, $node->om_user_files , 'page.tpl.php');

  // create html.tpl.php file  
  $html_tpl = om_html_template($node);
  om_file_create($html_tpl, $node->om_user_files , 'html.tpl.php');

  if ($base_theme_used == 'om') {
    // create regions.php  
    $regions_php = om_regions_file();
    om_file_create($regions_php, $node->om_user_files , 'regions.php');  
  }
      
  // previous info file must be deleted if the node title has changed
  om_info_delete($node);

  // create info file  
  $info = om_info_file($node);
  $info_name = strtolower(preg_replace('/ /', '_', $node->title));
  om_file_create($info, $node->om_user_files, $info_name . '.info');
  
  // increase script timeout value
  ini_set("max_execution_time", 300);
  
  // ZipArchive is native to php5.2 and above, otherwise it must be installed
  
  // create object
  $zip = new ZipArchive();

  // open archive
  if ($zip->open($node->om_user_files . '.zip', ZIPARCHIVE::OVERWRITE) !== TRUE) {
    die("Could not open archive");
  }

  // the directory to be processed
  $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($node->om_user_files . '/'));
  
  // add each file found to the archive
  foreach ($iterator as $key => $value) {

    // this is the way to delete the absolute path, /sites/...
    $path = explode('/', $key);
    while ($path[0] != $node->om_user_folder) {
      array_shift($path);
    }
    // delete last path which is the user code u10001d10001
    array_shift($path);
    
    // transform array to path and add the node title as the folder
    $path_file = $info_name . '/' . implode('/', $path);
    
    // get file names
    $file = array_reverse(explode('/', $key));
    
    // include only subtheme files
    $excluded_files = array('.', '..', 'index.html');
    if (!in_array($file[0], $excluded_files)) {     
      $zip->addFile(realpath($key),  $path_file) or die("ERROR: Could not add file: $key");
    }
    
  }
  // close and save archive
  $zip->close();  
}


/**
 * index.html
 *  
 * Add doctype, head, body tags to processed divs as html
 *
 */
function om_index_html_file($node) {
  global $_om_subthemer_regions;
  
  $om_subthemer = variable_get('om_subthemer', array());  
  $type_settings = isset($om_subthemer[$node->type]) ? $om_subthemer[$node->type]: array(); 
  $base_theme = isset($type_settings['base_theme']) ? $type_settings['base_theme']: 'om';
  
  // for preview purposes, $classes on body tag needs to be processed
  $left = 'first';
  $right = 'second';

  // layout and body classes.
  $layout = 'none';  
  $classes = 'front not-logged-in'; 
     
  if (in_array('Sidebar First', $_om_subthemer_regions) || in_array('Left', $_om_subthemer_regions)) $layout = $left;
  
  if (in_array('Sidebar Second', $_om_subthemer_regions) || in_array('Right', $_om_subthemer_regions)) {
    $layout = ($layout == $left) ? 'both' : $right;
  }

  // Add information about the number of sidebars.
  if ($layout == 'both') {
    $classes .= ' two-sidebars';
  }
  elseif ($layout == 'none') {
    $classes .= ' no-sidebars';
  }
  else {
    $classes .= ' one-sidebar sidebar-'. $layout;
  }

  if ($type_settings['grid'] <> 0) $classes .= ' grids-' . $type_settings['grid'];

  if ($type_settings['html5']) {
    $output = '<!DOCTYPE html>';
    $output .= "\n" . '<html class="js" lang="en" dir="ltr">';  
  }
  else {        
    $output = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">';
    $output .= "\n" . '<html class="js" dir="ltr" version="XHTML+RDFa 1.0" xml:lang="en" xmlns="http://www.w3.org/1999/xhtml">';
  }
  $output .= "\n" . '<head profile="http://www.w3.org/1999/xhtml/vocab">';
  $output .= "\n\t" . '<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />';
  $output .= "\n\t" . '<meta content="initial-scale=1.0, width=device-width, minimum-scale=0.5, maximum-scale=1.0, user-scalable=yes" name="viewport">';
  $output .= "\n\t" . '<meta content="width" name="MobileOptimized">';
  $output .= "\n\t" . '<meta content="true" name="HandheldFriendly">';
  $output .= "\n\t" . '<meta content="yes" name="apple-mobile-web-app-capable">';
  $output .= "\n\t" . '<meta content="on" http-equiv="cleartype">';
  $output .= "\n\t" . '<meta content="IE=edge, chrome=1" http-equiv="X-UA-Compatible">'; 
  $output .= "\n\t" . '<link type="image/x-icon" href="favicon.ico" rel="shortcut icon">';  
  $output .= "\n\t" . '<title>' . $node->title . ' - (' . $node->name . ')</title>';
  
  // get all stylesheets
  $css = drupal_add_css();
  //dsm($css);  
  // remove active base theme css files
  // In D7, the array of css is no longer grouped by types
  // so I changed the filter to substring
  $stylesheets = array();
  foreach ($css as $css_key => $css_file) {
    $theme_style = strstr($css_key, 'themes');
    if (empty($theme_style)) $stylesheets[$css_key] = $css_file;
  }
  //dsm($stylesheets);
  
  // print all stylesheets
  $output .= "\n\t" . drupal_get_css($stylesheets);
  
  if ($base_theme == 'none') {
    $output .= "\n\t" . '<link type="text/css" rel="stylesheet" media="all" href="css/reset.css" />';  
    $output .= "\n\t" . '<link type="text/css" rel="stylesheet" media="all" href="css/om_grids.css" />';  
    $output .= "\n\t" . '<link type="text/css" rel="stylesheet" media="all" href="css/base.css" />';    
  }
  elseif ($base_theme != 'none') {

    // get stylesheets from active base theme  
    $stylesheets = om_base_theme_file($node->type, 'stylesheets'); 
    foreach ($stylesheets as $style_key => $style) {
      $output .= "\n\t" . '<link type="text/css" rel="stylesheet" media="all" href="' . base_path() . drupal_get_path('theme', $base_theme) . '/' . $style . '" />';  
    }  
  }
    
  $output .= "\n\t" . '<link type="text/css" rel="stylesheet" media="all" href="css/style.css" />';
  
  // get all scripts
  $js = drupal_add_js();
  //dsm($js);

  // remove active base theme script files  
  // In D7, the array of js is no longer grouped by types
  // so I changed the filter to substring  
  $scripts = array();
  foreach ($js as $js_key => $js_file) {
    $theme_script = strstr($js_key, 'themes') . strstr($js_key, 'admin_menu');
    if (empty($theme_script)) $scripts[$js_key] = $js_file;
  }
  //dsm($scripts);  
  
  // prints the scripts
  $output .= "\n\t" . drupal_get_js('header', $scripts);

  if ($base_theme != 'none') {
  
    // get scripts from active base theme
    $scripts = om_base_theme_file($node->type, 'scripts');
    foreach ($scripts as $script_key => $script) {
      $output .= "\n\t" . '<script type="text/javascript" src="' . base_path() . drupal_get_path('theme', $base_theme) . '/' . $script . '" ></script>';  
    }  
  }  
  
  $output .= "\n\t" . '<script type="text/javascript" src="js/script.js" ></script>';
  if ($type_settings['html5']) $output .= "\n\t" . '<!--[if lt IE 9]><script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script><![endif]-->';  
  $output .= "\n" . '</head>';
  $output .= "\n" . '<body class="' . $classes . '">';
  $output .= "\n" . $node->om_layout_html_content;
  date_default_timezone_set('UTC');
  $output .= "\n" . '<div id="legal"><a href="http://www.drupal.org/project/om_subthemer">OM SubThemer</a> ' . date('d-m-Y h:i:s A') . ' | V7.x-1.x | by <a href="http://www.danielhonrade.com">Daniel Honrade</a></div>';
  $output .= "\n\n" . '</body>';
  $output .= "\n" . '</html>';
  return $output;
}


/**
 * page.tpl.php
 *  
 * Adds doctype, head, body tags to processed divs as php
 *
 */
function om_page_template($node) {
  $output = "\n" . '<!-- This layout has been auto-generated by OM Subthemer -->';  
  $output .= "\n" . $node->om_layout_php_content;
  $output .= "\n";
  
  return $output;
}


/**
 * html.tpl.php
 *  
 * Adds doctype, head, body tags to processed divs as php
 *
 */
function om_html_template($node) {

  // get content type base theme
  $om_subthemer  = variable_get('om_subthemer', array());  
  $type_settings = isset($om_subthemer[$node->type]) ? $om_subthemer[$node->type]: array(); 
  $base_theme    = isset($type_settings['base_theme']) ? $type_settings['base_theme']: 'om';
  
  //$subtheme = strtolower(preg_replace('/ /', '_', $node->title));
  
  $subtheme = ($base_theme == 'om') ?  'om': 'phptemplate';

  if ($type_settings['html5']) {
    $output = '<!DOCTYPE html>';
    $output .= "\n" . '<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>';  
  }
  else {      
    $output = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">';
    $output .= "\n" . '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>';
  }
  $output .= "\n" . '<head profile="<?php print $grddl_profile; ?>">';
  $output .= "\n" . '<?php print $head; ?>';
  $output .= "\n\t" . '<title><?php print $head_title; ?></title>';
  $output .= "\n\t" . '<?php print $styles; ?>';
  $output .= "\n\t" . '<?php print ' . $subtheme . '_get_ie_styles(); ?>';
  $output .= "\n\t" . '<?php print $scripts; ?>';
  if ($type_settings['html5']) $output .= "\n\t" . '<!--[if lt IE 9]><script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script><![endif]-->';
  $output .= "\n" . '</head>';
  $output .= "\n" . '<body class="<?php print $classes; ?>" <?php print $attributes;?>>';
  $output .= "\n\t" . '<div id="skip-link">';
  $output .= "\n\t\t" . '<a href="#main-content" class="element-invisible element-focusable"><?php print t(\'Skip to main content\'); ?></a>';
  $output .= "\n\t" . '</div>';
  $output .= "\n\t" . '<?php print $page_top; ?>';
  $output .= "\n\t" . '<?php print $page; ?>';
  $output .= "\n\t" . '<?php print $page_bottom; ?>';
  $output .= "\n" . '</body>';
  $output .= "\n" . '</html>';  
  
  return $output;
}


/**
 * subtheme.info - delete
 *
 * Delete info file prior to creation, the user changed the node title
 *
 */
function om_info_delete($node) {
  // checking if the file exist
  $dir = $node->om_user_files;
  $info_file = scandir($dir);
  $excluded_files = array('.', '..');
  
  foreach ($info_file as $file) {
    $file_name_array = explode('.', $file);
    // delete any/all stray .info file
    if (isset($file_name_array[1])) if (!in_array($file, $excluded_files) && ($file_name_array[1] == 'info')) unlink($dir . '/' . $file);
  }
}


/**
 * subtheme.info
 * 
 * Adds/Updates regions and OM Subtheme name
 *
 */
function om_info_file($node) {
  global $_om_subthemer_regions;
  
  // get content type base theme
  $om_subthemer  = variable_get('om_subthemer', array());  
  $type_settings = isset($om_subthemer[$node->type]) ? $om_subthemer[$node->type]: array(); 
  $base_theme    = isset($type_settings['base_theme']) ? $type_settings['base_theme']: 'om';
  
  $output = '';
  $output .= '; name, description, screenshot, version - these are necessary for theme list';
  $output .= "\n";
  $output .= "\n" . 'name        = ' . $node->title . ' - Created by ' . ucwords($node->name);
  $output .= "\n" . 'description = "This a subtheme of OM Base Theme. For more info, read the <a href="http://drupal.org/node/1056254">OM Theme Documentation</a>"';
  $output .= "\n" . 'screenshot  = "screenshot.png"';
  $output .= "\n";
  
  if ($base_theme != 'none') $output .= "\n" . 'base theme = ' . $base_theme;
  
  $output .= "\n";
  $output .= "\n" . '; compatibilities';
  $output .= "\n" . 'core        = "7.x"';
  $output .= "\n" . 'engine      = phptemplate';
  $output .= "\n";
  $output .= "\n" . '; viewport';
  $output .= "\n" . '; you can turn off these by adding ';' at the begining of each line';
  $output .= "\n" . 'head[meta][viewport][initial-scale] = 1.0';
  $output .= "\n" . 'head[meta][viewport][width] = device-width';
  $output .= "\n" . 'head[meta][viewport][minimum-scale] = 0.5';
  $output .= "\n" . 'head[meta][viewport][maximum-scale] = 1.0';
  $output .= "\n" . 'head[meta][viewport][user-scalable] = yes';
  $output .= "\n";
  $output .= "\n" . '; support for older devices/browsers,'; 
  $output .= "\n" . '; you can turn off these by adding ';' at the begining of each line';
  $output .= "\n" . 'head[meta][MobileOptimized] = width';
  $output .= "\n" . 'head[meta][HandheldFriendly] = true';
  $output .= "\n" . 'head[meta][apple-mobile-web-app-capable] = yes';
  $output .= "\n" . 'head[meta][cleartype] = on';
  $output .= "\n" . 'head[meta][X-UA-Compatible][IE] = edge';  
  $output .= "\n" . 'head[meta][X-UA-Compatible][chrome] = 1';   
  $output .= "\n";
  $output .= "\n" . '; 960 grids, 12, 16, 24 columns';
  $output .= "\n" . 'settings[grid_guide] = on'; 
  $output .= ($type_settings['grid'] <> 0) ? "\n" . 'settings[grid] = ' . $type_settings['grid']: "\n" . ';settings[grid] = 16';
  $output .= "\n";
  $output .= "\n" . '; this will set the site offline with a timer,';
  $output .= "\n" . '; great for underconstruction sites, maintenance';
  $output .= "\n" . '; you can set the time of launch or when the site will';
  $output .= "\n" . '; be live again.';
  $output .= "\n" . 'settings[offline][switch] = off';
  $output .= "\n" . 'settings[offline][message] = "We\'re currently improving this site."';
  $output .= "\n" . 'settings[countdown][year] = 2012';
  $output .= "\n" . 'settings[countdown][month] = 1';
  $output .= "\n" . 'settings[countdown][days] = 1';
  $output .= "\n" . 'settings[countdown][hours] = 0';
  $output .= "\n" . 'settings[countdown][minutes] = 0';
  $output .= "\n" . 'settings[countdown][seconds] = 0';
  $output .= "\n";
  $output .= "\n" . '; default stylesheets';
  $output .= "\n";
  if ($base_theme == 'none') $output .= "\n" . 'stylesheets[all][] = "css/reset.css"';
  if ($base_theme == 'none') $output .= "\n" . 'stylesheets[all][] = "css/om_grids.css"';
  if ($base_theme == 'none') $output .= "\n" . 'stylesheets[all][] = "css/base.css"';
  $output .= "\n" . 'stylesheets[all][] = "css/style.css"';
  $output .= "\n";
  $output .= "\n" . '; jquery scripts';
  $output .= "\n" . 'scripts[]       = "js/script.js"';
  $output .= "\n";
  $output .= "\n" . '; default regions, on the left inside the brackets'; 
  $output .= "\n" . '; are the variable names used in page.tpl.php,';
  $output .= "\n" . '; on right side are the names necessary for blocks list';
  $output .= "\n";
  foreach ($_om_subthemer_regions as $key => $region) {
    $output .= "\n" . 'regions[' . $key . '] = ' . $region;
  }
  return $output;
}


/**
 * @file
 * regions.php
 * 
 * Region properties
 *
 */
function om_regions_file() {
  global $_om_subthemer_regions_php;
  $regions = $_om_subthemer_regions_php;

  $description = file_get_contents(OM_SUBTHEMER_PATH . '/base_themes/om/om.regions');

  $output = $description;
  $output .= "\n";
  $output .= "\n" . '// Values are auto-generated by OM Subthemer';
  $output .= "\n";    
  $output .= "\n" . 'function om_regions_get_info() {';
  $output .= "\n";  
  $output .= "\n\t" . '$regions = array();';
  foreach ($regions as $key => $region) {
    $output .= "\n\t" . '$regions[\'' . $key . '\'] = array(';
    $output .= "\n\t\t" . '\'tag\'    => \'' . $region['tag'] . '\',';
    $output .= "\n\t\t" . '\'id\'     => \'' . preg_replace('/_/', '-', trim($key)) . '\',';
    $output .= "\n\t\t" . '\'class\'  => \'' . $region['class'] . '\',';
    $output .= "\n\t\t" . '\'inner\'  => ' . $region['inner'] . ',';
    $output .= "\n\t\t" . '\'top\'    => ' . $region['top'] . ',';
    $output .= "\n\t\t" . '\'bottom\' => ' . $region['bottom'] . ',';
    $output .= "\n\t\t" . '\'grid\'   => ' . $region['grid'] . ',';
    $output .= "\n\t" . ');'; 
  }
  $output .= "\n\t" . 'return $regions;';
  $output .= "\n" . '}';
  $output .= "\n";
  $output .= "\n";
  return $output;
}

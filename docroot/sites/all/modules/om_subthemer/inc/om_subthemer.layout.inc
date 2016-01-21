<?php
// $Id$

/**
 * @file
 * OM Subthemer Layout Engine
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 * 
 * This is a recursive function which automatically creates:
 *   - Wrapper div - if other elements are nested under it
 *   - Region div  - if no other elements are nested under it
 *   - Variable    - if the tag name is a reserved variable name,
 *                   the list of reserved variable names is in /modules/system/page.tpl.php.
 *                   You can also register other variable names via om subthemer settings
 * 
 * index.html for previewing and page.tpl.php for the subtheming are generated
 * using this same function
 *
 */


/**
 * Layout Generator
 *
 * @params
 * $divs 
 *   - array elements in the layout, same as $om_layout_content
 *     but starts with array(0 => ...
 * $type_settings
 *   - a node type used by OM Subthemer
 *     and with attached settings
 * $output_type 
 *   - html/php
 *
 */ 
function om_subthemer_layout($divs = array(), $type_settings = array(), $output_type = NULL) {
  global $_om_subthemer_layout_content; // layout array
  global $_om_subthemer_regions;        // regions
  global $_om_subthemer_regions_php;    // regions.php
  global $_om_subthemer_region_classes; // used by OM subthme, these are additional classes for regions
                                        // basically grouped together in its page.tpl.php
  global $_om_subthemer_count;          // count number of arrays
  
  static $output   = '';     // init output

  $out_content     = '';     // recursive content for final output
  
  static $count    = 0;      // count recursion, if it equals the number of layout arrays it will trigger the output

  $div_count       = 0;      // init every level count of div
  $div_count_total = 0;      // init every level total of div

  static $level    = 1;      // count level for indents on divs on html file
  $tab = '';                 // init tab for indents on divs on html file
  
  // for source formating
  // calculates number of tabs
  for ($i = 1; $i < $level; $i++) {
    $tab .= "\t";
  }
  
  // initialize elements object to populate divs with classes, 
  // container tags, and contents
  $elements = new stdClass;
    
  //read array
  foreach ($divs as $cid => $prop) {
  
    // count only columns and rows
    if ($prop['type'] != 'variable') $div_count++;
    $div_count_total = count($divs); //counts inner divs
    
    // addtional classess
    $elements->classes = om_layout_classes($div_count_total, $div_count, $prop, $type_settings);
    
    // div id, variable names    
    $elements->tag         = 'div'; //$prop['tag']; 
    $elements->tagname     = $prop['tagid'];
    $elements->tagid       = strtolower(preg_replace('/ /', '-', $prop['tagid']));
    $elements->tagvar      = strtolower(preg_replace('/ /', '_', $prop['tagid']));
    $elements->type        = $prop['type'];
    $elements->iw          = $prop['iw'];
    $elements->top         = 0; //$prop['top'];
    $elements->bottom      = 0; //$prop['bottom'];   
    $elements->grid        = 0; //$prop['grid'];   
    $elements->output_type = $output_type;

    // wrapper level class
    $class_wrapper_level = ($type_settings['class_wrapper_level']) ? ' wrapper-level-' . $level: '';
    
    // wrapper divs                
    $id_wrapper = ($type_settings['id_wrapper']) ? ' id="' . $elements->tagid . '"': '';
    $wrapper_div = "\n$tab" . '<div' . $id_wrapper . ' class="wrapper wrapper-' . $elements->tagid . $class_wrapper_level . $elements->classes . '">';
    $wrapper_div_end = '</div><!-- /' . (($type_settings['id_wrapper']) ? '#': '.wrapper-') . $elements->tagid . ' -->';  
        
    // inner wrapper switch
    if ($elements->iw) { 
      $id_wrapper_inner = ($type_settings['id_wrapper_inner']) ? ' id="' . $elements->tagid . '-inner"': '';
      $wrapper_div .= '<div' . $id_wrapper_inner . ' class="wrapper-inner">';  
      $wrapper_div_end  = '</div></div><!-- /' . (($type_settings['id_wrapper_inner']) ? '#' . $elements->tagid: '.wrapper') . '-inner, /' . (($type_settings['id_wrapper']) ? '#': '.wrapper-') . $elements->tagid . ' -->'; 
    } 
          
    // process regions, reference function
    om_layout_regions($elements, $tab);
          
    if (isset($_om_subthemer_layout_content[$cid])) { //start recursion if div id is a parent
    
      // add tabs for the next level
      $level++; 
      
      // put wrapper div if parent
      $output .= $wrapper_div;
      
      //start recursion
      om_subthemer_layout($_om_subthemer_layout_content[$cid], $type_settings, $output_type); 
      
      // put end wrapper div
      $output .= "\n$tab" . $wrapper_div_end;
      
      // undo the added tabs in the inner levels
      $level--; 
    } 
    else {
      
      // For creating regions on .info file. 
      // Since this function is being used by both html and php output,
      // this should not run on html processing.
      // Variables must be filtered out from creating regions.
       if (($elements->type != 'variable') && ($elements->output_type != 'html' )) $_om_subthemer_regions[$elements->tagvar] = $elements->tagname;
      
      // additional classes for OM regions, used in OM page.tpl.php
      if (($elements->type != 'variable') && ($elements->output_type == 'om')) $_om_subthemer_region_classes .= "\n" . '$' . $elements->tagvar . '_classes = \'' . $elements->classes . '\';';     

      // variables for regions.php
      if (($elements->type != 'variable') && ($elements->output_type == 'om')) {
        $_om_subthemer_regions_php[$elements->tagvar]['tag']    = $elements->tag; 
        $_om_subthemer_regions_php[$elements->tagvar]['class']  = $elements->classes; 
        $_om_subthemer_regions_php[$elements->tagvar]['inner']  = $elements->iw; 
        $_om_subthemer_regions_php[$elements->tagvar]['top']    = $elements->top; 
        $_om_subthemer_regions_php[$elements->tagvar]['bottom'] = $elements->bottom;                         
        $_om_subthemer_regions_php[$elements->tagvar]['grid']   = $elements->grid;                         
      }      
      
      // get region contents
      $output .= om_get_value($elements, $tab); 
    }

    //adds float clear below the columns
    if (($elements->type == 'column') && ($div_count == $div_count_total)) $output .= "\n$tab<div class=\"om-clearfix\"></div>";
  }
  $count++;

  //switch to start the output
  if ($count == $_om_subthemer_count) {
    $count = 0; // reset
    $out_content = $output;
    
    // use reset because this function is being used more than once,
    // this prevents from adding previous content to the output
    $output = ''; 
    
    return $out_content;
  }
}


/**
 * Get default html content
 *
 * @param $elements, includes:
 *
 *   Property          Sample Values
 *   - tagname         - Content Elements
 *   - tagid           - content-elements
 *   - tagvar          - content_elements
 *   - type            - variable  
 *   - output_type     - html 
 *   - region          - <div ... 
 *   - region_end      - .../div>   
 *   - var_wrapper     - <div ... 
 *   - var_wrapper_end - .../div>   
 *   - iw              - 0  
 *
 */ 
function om_get_value($elements, $tab = NULL) {
  global $_om_subthemer_content_variables;
  global $_om_subthemer_blocks;
  
  // needed for preview
  $node = $_om_subthemer_content_variables['node'];
  
  // needed for $content variable in db
  if (!empty($_om_subthemer_blocks[$elements->tagvar])) {
    // check for the assigned blocks for each region and add its content
    $region_content = '';
    foreach ($_om_subthemer_blocks[$elements->tagvar] as $bkey => $bval) {
      $region_content .= $bval;
    }
  }
  else {
    // default html region content, just the title
    $region_content = "\n$tab\t<h3 class=\"region-title\">" . $elements->tagid . '</h3>';  
  }
  
  // html preview or php for page.tpl.php
  ($elements->output_type == 'html') ? $output_type = 'value_html': $output_type = 'value_php';   
  
  $value = '';
  // the settings contains php value including the variable $content
  // for each existing tagname, e.g.
  // Title, $content = '<h1>...'
  // the variable exists in the database   
  if (isset($_om_subthemer_content_variables[$elements->tagname])) {
    $value = $_om_subthemer_content_variables[$elements->tagname][$output_type];
  }
  else { // the variable doesn't exist in the database
    // get default variable value
    // This default Variable value should exist in the settings
    // This default Region value should exist in the settings
    ($elements->type == 'variable') ? $tagname = 'Variable': $tagname = 'Region';     
    $value = $_om_subthemer_content_variables[$tagname][$output_type];
  }
  $content = '';
  // $content variable source
  ob_start();
  eval($value);
  ob_end_clean(); 
     
  // it is a variable
  return "\n$tab" . $content;   
}


/**
 * Generates divs region divs
 *
 * TODO: refactor this function
 *
 */ 
function om_layout_regions(&$elements, $tab = NULL) {

  if ($elements->output_type == 'html') { // html preview tags
    // variable div prefix and suffix
    $var_wrapper       = "\n$tab" . '<div id="' . $elements->tagid . '">';
    $var_wrapper_end   = '</div><!-- /#' . $elements->tagid . ' -->';
    // region div prefix and suffix
    $wrapper           = "\n$tab<div id=\"" . $elements->tagid . '" class="region region-' . $elements->tagid . $elements->classes . '">';
    $wrapper_end       = '</div><!-- /#' . $elements->tagid . ' -->';
    // for inner wrapper
    $wrapper_inner     = '<div id="' . $elements->tagid . '-inner" class="region-inner">';
    $wrapper_inner_end = '</div></div><!-- /#' . $elements->tagid . '-inner, /#' . $elements->tagid . ' -->';
  
    // regions divs      
    if ($elements->type == 'variable') {
      // variable divs
      $elements->wrapper = $var_wrapper;
      $elements->wrapper_end = $var_wrapper_end;
    }
    else {
      // region divs
      $elements->wrapper = $wrapper;
      $elements->wrapper_end = $wrapper_end;
    }  
    // for inner wrapper
    if ($elements->iw && ($elements->type != 'variable')) { 
      $elements->wrapper .= $wrapper_inner;
      $elements->wrapper_end  = $wrapper_inner_end;
    }   
  
  }
  elseif ($elements->output_type == 'om') { // om doesn't need these wrapper settings,
      $elements->wrapper = '';   // they are already in the template.php
      $elements->wrapper_end = '';
  }
  else { // php default tags and conditionals
    // variable div prefix and suffix  
    $var_wrapper       = "\n$tab" . '<div id="' . $elements->tagid . '">';
    $var_wrapper_end   = '</div><!-- /#' . $elements->tagid . ' -->';
    // region div prefix and suffix
    $wrapper           = '<?php if (!empty($page[\'' . $elements->tagvar . '\'])) : ?>' . "<div id=\"" . $elements->tagid . '" class="region region-' . $elements->tagid . $elements->classes . '">';
    $wrapper_end       = '</div><!-- /#' . $elements->tagid . ' --><?php endif; ?>';
    // for inner wrapper
    $wrapper_inner     = '<div id="' . $elements->tagid . '-inner" class="region-inner">';
    $wrapper_inner_end = '</div></div><!-- /#' . $elements->tagid . '-inner, /#' . $elements->tagid . ' --><?php endif; ?>';

    // regions divs      
    if ($elements->type == 'variable') {
      // variable divs
      $elements->wrapper = $var_wrapper;
      $elements->wrapper_end = $var_wrapper_end;
    }
    else {
      // region divs
      $elements->wrapper = $wrapper;
      $elements->wrapper_end = $wrapper_end;
    }  
    // for inner wrapper
    if ($elements->iw && ($elements->type != 'variable')) { 
      $elements->wrapper .= $wrapper_inner;
      $elements->wrapper_end  = $wrapper_inner_end;
    }   
  }
}

    
/**
 * Generates divs classes
 *
 */ 
function om_layout_classes($div_count_total = NULL, $div_count = NULL, $prop = array(), $type_settings = array()) {
  $classes = '';
    
  // row/column class
  $type = $prop['type']; 
  
  if ($type_settings['class_row'] && ($type == 'row')) { 
    $classes .= ' row';
    $classes .= ' rows-' . $div_count_total;
    $classes .= ' row-' . $div_count_total . '-' . $div_count;
    $classes .= ' row-' . $div_count;
  }  
  
  if ($type_settings['class_column'] && ($type == 'column')) { 
    $classes .= ' column';
    $classes .= ' columns-' . $div_count_total;
    $classes .= ' column-' . $div_count_total . '-' . $div_count;
    $classes .= ' column-' . $div_count;
  } 
    
  // first/last
  if ($type_settings['class_first_last']) {
    if ($div_count == 1) $classes .= ' ' . $type . '-first';
    if ($div_count == $div_count_total) $classes .= ' ' . $type . '-last';
  } 
    
  // wrapper-outer
  if ($prop['pid'] == 0) $classes .= ' wrapper-outer';
  
  return $classes;
}


/**
 * Get Variables Content
 *
 * @param $content
 *   Variable name
 * @param $node
 *
 */
function om_get_variables($type = NULL, $node) {
  
  // vid, variable, value, source, type
  $vars = om_set_variables($type);
  
  // this usually happens on the initial install,
  // and it will get the default file and upload it to db
  if (empty($vars)) {
    $settings = om_base_theme_file($type, 'settings');
    
    om_subthemer_settings_upload($settings, $type);
    
    //vid, variable, value, source, type
    $vars = om_set_variables($type);
  }
  
  $vars['node'] = $node;
  
  return $vars;
}


/**
 * Set Content
 *
 * @param $type
 *   Theme type, 
 *
 */
function om_set_variables($type = NULL) {
  
  //vid, variable, value, source, type
  $sql = "SELECT vid, variable, value_html, value_php, source, type 
          FROM {om_subthemer_vars} 
          WHERE type = :type 
          ORDER BY variable ASC";

  $result = db_query($sql, array(':type' => $type));
  
  $vars = array();
  while ($record = $result->fetchObject()) {
    $vars[$record->variable]['variable']   = $record->variable;
    $vars[$record->variable]['value_html'] = $record->value_html;
    $vars[$record->variable]['value_php']  = $record->value_php;
    $vars[$record->variable]['source']     = $record->source;
    $vars[$record->variable]['type']       = $record->type;
  }
  return $vars;
}



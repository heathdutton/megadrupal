<?php
// $Id$

/**
 * @file
 * OM Subthemer Skin Select
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * OM Skin form
 *
 */
function om_skin_select($form, &$form_state) {

  // I just need the node title, so better use this function to get the node title
  $title = drupal_get_title();
    
  drupal_set_title(t('Skin for %title', array('%title' => check_plain($title))), PASS_THROUGH); 
     
  $form['skin'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
  );
 
  // scan files for skins
  $skins_path = OM_SUBTHEMER_PATH . '/skins';
  
  $skins = om_folders_get($skins_path);  
  
  // additional skins
  $om_skins_library = 'sites/all/libraries/om_skins';
  
  if (is_dir($om_skins_library)) $skins = array_merge($skins, om_folders_get($om_skins_library));

  // get skin setting for this node                      
  $sql = "SELECT nid, skin
          FROM {om_subthemer}
          WHERE nid = :nid";

  $result = db_query($sql, array(':nid' => arg(1)));
  
  $record = $result->fetchObject();
  $record->skins = $skins; // for options
  $layout[0] = _om_skin_select($record); 
 
  $form['skin'] += $layout;  

  $form['submit'] = array( '#type' => 'submit', '#value' => t('Save Settings'), );
  //dsm($form); 
  return $form;
}


/**
 * Skin Options
 *
 */
function _om_skin_select($record) {
  
  foreach ($record->skins as $key => $subtheme) {
  
    $path = OM_SUBTHEMER_PATH . '/skins/' . $key;
    // included screenshots for the options
    (is_dir($path)) ? $path = OM_SUBTHEMER_PATH . '/skins/': $path = 'sites/all/libraries/om_skins/';

    $options[$key] = '<h5>' . $subtheme . '</h5><div class="screenshot"><img alt="' . $subtheme . '" src="' . base_path() . $path . $key . '/screenshot.png' . '" /></div>';
  }

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => isset($record->nid) ? $record->nid: '',
  );  
  
  $form['skin'] = array(
    '#type' => 'radios',
    '#default_value' => isset($record->skin) ? $record->skin: '',
    '#options' => $options, 
  );

  return $form;
}

/**
 * Implementation of theme_hook()
 * 
 * Skin Table layout
 *
 */
function theme_om_skin_select($vars) {
  $form = $vars['form'];
  drupal_add_css(OM_SUBTHEMER_PATH . '/css/om_subthemer.css');
  
  $rows = array();
  $output = '';
  //dsm($form['skin']);
  foreach (element_children($form['skin'][0]['skin']) as $key) {
    $row = array();
    // Strips labels
    $form['skin'][$key]['skin']['#title'] = '';
    $row[] = drupal_render($form['skin'][0]['skin'][$key]);
    $rows[] = array('class' => array('om-subtheme-list'), 'data' => $row);
  }

  $header = array('Choose skin for your layout.');

  // Header
  $form['skin']['#children'] = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form);
  
  return $output;
}


/**
 * Skin Submit
 * 
 */
function om_skin_select_submit($form, $form_state) {
  $form_values = $form_state['values'];
  //dsm($form_values);
  // Update an existing skin select  
  db_update('om_subthemer') // Table name no longer needs {}
    ->fields(array(
      'skin'   => $form_values['skin'],
    ))
    ->condition('nid', $form_values['nid'], '=')
    ->execute();
            
  // nicely format this skin name
  $skin = ucwords(preg_replace('/_/', ' ', $form_values['skin']));
  drupal_set_message(t($skin . ' has been selected.'));

  // need this to update files on zip
  include_once OM_SUBTHEMER_PATH . '/inc/om_subthemer.write.inc';    
  
  $node = node_load(arg(1));

  // set om_subthemes folder
  om_directory(OM_SUBTHEMER_FILE_DIRECTORY);
  
  // set user folder 
  om_subthemer_user_folder($node);

  // 1) copy current subtheme files to user subtheme folder
  om_current_skin_copy($node);
  
  // 2) get layout for variables
  om_process_layout($node);
      
  // 3) write om subtheme
  om_file_write($node); 

}



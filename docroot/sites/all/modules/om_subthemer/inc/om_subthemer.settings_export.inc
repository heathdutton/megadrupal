<?php
// $Id$

/**
 * @file
 * OM Subthemer Settings Export
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * OM Subthemer export settings form
 *
 */
function om_settings_export($form, &$form_state, $type, $name) {

  drupal_set_title(t('Export %type settings', array('%type' => check_plain($name))), PASS_THROUGH);
  
  $sql = "SELECT vid, variable, value_html, value_php, source 
          FROM {om_subthemer_vars} 
          WHERE type = :type
          ORDER BY variable ASC";
                      
  $result = db_query($sql, array(':type' => $type));
  
  $vars = array();
  while ($record = $result->fetchObject()) {
    $vars[] = array($record->variable, $record->value_html, $record->value_php, $record->source);
  }                      
                      
  //dsm($vars);
  $out = '';
  // formats this layout to array variable type
  foreach ($vars as $key => $prop) {
    if (is_numeric($key)) {
      $out .= '
        $settings[' . $key . '] = array(
          \'variable\'   => \'' . $prop[0] . '\', 
          \'value_html\' => \'' . addslashes(stripslashes($prop[1])) . '\', 
          \'value_php\'  => \'' . addslashes(stripslashes($prop[2])) . '\', 
          \'source\'     => \'' . $prop[3] . '\',
          );' . "\n";         
    } 
  }
  
  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['export']['settings'] = array(
    '#type' => 'textarea',
    '#title' => t(''),
    '#rows' => 30,
    '#default_value' => $out,
  );  

  return $form;
}

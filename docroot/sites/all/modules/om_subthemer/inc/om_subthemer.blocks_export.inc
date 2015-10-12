<?php
// $Id$

/**
 * @file
 * OM Subthemer Blocks Export
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * OM Subthemer export blocks form
 *
 */
function om_blocks_export($form, &$form_state) {

  $sql = "SELECT title, content, module, region, weight 
          FROM {om_subthemer_blocks} 
          WHERE nid = :nid 
          ORDER BY weight ASC";

  $result = db_query($sql, array(':nid' => arg(1)));
  
  $om_blocks_content = array();
  while ($record = $result->fetchObject()) {
    $om_blocks_content[] = array($record->title, $record->content, $record->module, 'disabled', $record->weight);
  }
  
  $blocks = '';
  
  // formats this layout to array variable type
  foreach ($om_blocks_content as $key => $prop) {
    if (is_numeric($key)) {
      $blocks .= '
        $blocks[' . $key . '] = array(
          \'title\'   => \'' . addslashes(stripslashes($prop[0])) . '\', 
          \'content\' => \'' . addslashes(stripslashes($prop[1])) . '\', 
          \'module\'  => \'' . $prop[2] . '\', 
          \'region\'  => \'' . $prop[3] . '\',
          \'weight\'  => ' . $prop[4] . ',
        );' . "\n";         
    } 
  }
  
  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export Blocks'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  
  $form['export']['blocks'] = array(
    '#type' => 'textarea',
    '#title' => t(''),
    '#rows' => 30,
    '#default_value' => $blocks,
  );  

  return $form;
}

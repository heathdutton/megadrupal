<?php
// $Id$

/**
 * @file
 * OM Subthemer Layout Export
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * OM Subthemer export layout form
 *
 */
function om_layout_export($form, &$form_state) {

  $sql = "SELECT cid, pid, tagid, type, iw, weight, nid 
          FROM {om_subthemer_layout} 
          WHERE nid = :nid 
          ORDER BY weight ASC";

  $result = db_query($sql, array(':nid' => arg(1)));
  
  $om_layout_content = array();
  while ($record = $result->fetchObject()) {
    $om_layout_content[] = array($record->tagid, $record->type, $record->iw, $record->weight, $record->cid, $record->pid);
  }
  
  $layout = '';
  
  // formats this layout to array variable type
  foreach ($om_layout_content as $key => $prop) {
    if (is_numeric($key)) {
      $layout .= '
        $layout[' . $key . '] = array(
          \'tagid\'  => \'' . addslashes(stripslashes($prop[0])) . '\', 
          \'type\'   => \'' . $prop[1] . '\', 
          \'iw\'     => ' . $prop[2] . ', 
          \'weight\' => ' . $prop[3] . ',
          \'cid\'    => ' . $prop[4] . ',
          \'pid\'    => ' . $prop[5] . '
          );' . "\n";         
    }
  }
  
  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export Layout'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['export']['layout'] = array(
    '#type' => 'textarea',
    '#title' => t(''),
    '#rows' => 30,
    '#default_value' => $layout,
  );  

  return $form;
}

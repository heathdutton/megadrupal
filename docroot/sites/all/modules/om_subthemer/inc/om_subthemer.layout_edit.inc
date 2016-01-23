<?php
// $Id$

/**
 * @file
 * OM Subthemer Layout Edit
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 * 
 * Layout edit for each OM Subthemer layout
 *
 */

/**
 * OM Layout Form
 *
 */
function om_layout_edit($form, &$form_state) {
  
  global $_om_subthemer_layout_structure; // child to parent order
  global $_om_subthemer_layout_content; // parent to child order
      
  $node = node_load(arg(1));
      
  drupal_set_title(t('Layout for %title', array('%title' => check_plain($node->title))), PASS_THROUGH); 
    
  $form['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $sql = "SELECT lid, cid, pid, tagid, type, iw, weight, nid 
          FROM {om_subthemer_layout} 
          WHERE nid = :nid 
          ORDER BY weight DESC";

  $result = db_query($sql, array(':nid' => $node->nid));
  //dsm($result);  
  $layout = array();
  $om_layout_structure = array();
  $om_layout_content = array();

  while ($record = $result->fetchObject()) {
    //dsm($record);
    $layout[] = _om_layout_edit($record);
    $_om_subthemer_layout_structure[$record->cid][$record->pid] = $record->tagid; 
    $_om_subthemer_layout_content[$record->pid][$record->cid] = $record->tagid; 
  }

  /** 
   * correcting the order of parent and child divs
   * - this has to be reversed since it's getting child to parent
   */
  $om_correct_order = array_reverse(explode(' ', _om_order($_om_subthemer_layout_content[0])));
  
  //substituting the correct order of parent and child divs
  $layout_order = array();
  foreach ($om_correct_order as $ckey => $val) {
    foreach ($layout as $lkey => $lval) {    
      if ($layout[$lkey]['cid']['#value'] == $val) $layout_order[$ckey] = $layout[$lkey];
    }
  }

  // add another blank question fields
  $record = new stdClass();
  $record->cid = 0;
  $record->pid = 0;  
  $record->weight = 0;  
  $record->nid = $node->nid;
  _om_new_rows($layout_order, 5, $record, 'layout_edit'); 
     
  $form['layout'] += $layout_order; 
  
  //dsm($form['layout']);
  $form['submit'] = array( '#type' => 'submit', '#value' => t('Save Layout'), );

  $form['export'] = array( '#type' => 'submit', '#value' => t('Export'), );

  $form['import'] = array( '#type' => 'submit', '#value' => t('Import'), );
      
  return $form;
}

/**
 * Layout Form
 *
 */
function _om_layout_edit($record) {
 
  $form['lid'] = array(
    '#type' => 'hidden',
    '#value' => isset($record->lid) ? $record->lid: '',
    '#attributes' => array('class' => array('om-lid')),    
  );  
  $form['cid'] = array(
    '#type' => 'textfield',
    '#value' => isset($record->cid) ? $record->cid: '',
    '#attributes' => array('class' => array('om-cid')),    
  );
  $form['nid'] = array(
    '#type' => 'textfield',
    '#value' => isset($record->nid) ? $record->nid: '',
  );  
  $form['pid'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($record->pid) ? $record->pid: '',
    '#attributes' => array('class' => array('om-pid')),
  );
  $form['tagid'] = array(
    '#type' => 'textfield',
    '#title' => t('Tag ID'),
    '#default_value' => isset($record->tagid) ? $record->tagid: '',
    '#size' => 30,
  );
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#default_value' => isset($record->type) ? $record->type: '',
    '#options' => array('row' => 'Row', 'column' => 'Column', 'variable' => 'Variable'),
  );
  $form['iw'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inner'),
    '#default_value' => isset($record->iw) ? $record->iw: '',
    '#disabled' => isset($record->type) ? ($record->type == 'variable') ? 'disabled': '': '',
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => isset($record->weight) ? $record->weight: '',
    '#delta' => 50,
    '#attributes' => array('class' => array('om-weight')),
  );
  $form['delete'] = array(
    '#type' => ($form['cid']['#value']) ? 'checkbox' : 'hidden',
    '#title' => t('Delete'),
    '#return_value' => 1,
    '#default_value' => 0,
    '#description' => t(''),
  );    
  return $form;
}

/**
 * Implementation of theme_hook()
 * - View in table format
 *
 */
function theme_om_layout_edit($vars) {
  $form = $vars['form'];
  drupal_add_js(OM_SUBTHEMER_PATH . '/js/om_subthemer.js', 'file');

  global $_om_subthemer_layout_structure;
  
  //dsm($om_layout_structure);

  $output = '';
  //dsm($form['layout']);
  $rows = array();
  foreach (element_children($form['layout']) as $key) {
    $row = array();
    // Strips labels
    $indent = '';
    $form['layout'][$key]['tagid']['#title'] = '';
    $form['layout'][$key]['type']['#title'] = '';
    $form['layout'][$key]['iw']['#title'] = '';
    $form['layout'][$key]['cid']['#title'] = '';        
    $form['layout'][$key]['pid']['#title'] = '';  
    $form['layout'][$key]['weight']['#title'] = '';      
    $form['layout'][$key]['delete']['#title'] = '';    
    
    // 0 as parent has no indent
    ($form['layout'][$key]['pid']['#value'] != 0) ? $indent = _om_indent($_om_subthemer_layout_structure[$form['layout'][$key]['pid']['#value']]): $indent = '';
    // level
    ($form['layout'][$key]['pid']['#value'] != 0) ? $level = _om_level($_om_subthemer_layout_structure[$form['layout'][$key]['pid']['#value']]): $level = 0;
      
    $row[] = array('class' => array('col-tagid'), 'data' => $indent . drupal_render($form['layout'][$key]['tagid']));
    // add level class to row/column
    ($form['layout'][$key]['type']['#value'] != 'variable') ? $level_val = ' not-variable level-' . $level: $level_val = '';
    $row[] = array('class' => array('col-type') . $level_val, 'data' => drupal_render($form['layout'][$key]['type']));
    $row[] = array('class' => array('col-iw'), 'data' => drupal_render($form['layout'][$key]['iw']));
    $row[] = array('class' => array('col-hidden'), 'data' => drupal_render($form['layout'][$key]['lid']) . drupal_render($form['layout'][$key]['cid']) . drupal_render($form['layout'][$key]['pid']) . drupal_render($form['layout'][$key]['weight']));
    $row[] = array('class' => array('col-delete'), 'data' => drupal_render($form['layout'][$key]['delete']));    
    ($form['layout'][$key]['type']['#value'] == 'variable') ? $tabledrag_leaf = 'tabledrag-leaf': $tabledrag_leaf = '';
    $rows[] = array(
      'data' => $row,
      'id' => 'om-row-'. $key,
      'class' => array('draggable row-'. $key . ' parent-' .  (!empty($form['layout'][$key]['pid']['#value']) ? $form['layout'][$key]['pid']['#value']: 'none') . ' ' . $tabledrag_leaf),
    ); 
  }
  $header = array('Tag ID', 'Type', 'Inner Wrapper', 'Weight', 'Delete'); //, 'Update'

  // Header
  $form['layout']['#children'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' =>  array('id' => 'om-subthemer-layout')));

  //drupal_add_tabledrag($table_id, $action, $relationship, $group, $subgroup = NULL, $source = NULL, $hidden = TRUE, $limit = 0);
  drupal_add_tabledrag('om-subthemer-layout', 'match', 'parent', 'om-pid', 'om-pid', 'om-cid', TRUE, 0);
  drupal_add_tabledrag('om-subthemer-layout', 'order', 'group', 'om-weight');
  
  $output .= drupal_render_children($form);
  
  return $output;
}

/**
 * Layout Submit
 * table: cid, pid, tagid, type, iw, weight, nid
 */
function om_layout_edit_submit($form, $form_state) {
 
  if ($form_state['values']['op'] == t('Save Layout')) { 
   
    $form_values = $form_state['values'];
    $tagid = '';
    $cid = 1;
    foreach ($form_values['layout'] as $layout) {
    
      // checks for reserved variables and correct type
      $tagid = ucwords($layout['tagid']);
      
      // assigns variable type to reserved variable names
      $type = om_reserved_variables($tagid, $layout['type']);
      
      // ensures that variable types will not have inner wrappers
      $iw = ($type == 'variable') ? $iw = 0: $iw = $layout['iw'];
      
      // always make the new child the highest id
      if ($cid < $layout['cid']) $cid = $layout['cid'] + 1;
      
      if ($layout['delete']) {
      
        // delete children
        _om_delete_child($layout['cid'], $layout['nid']);
        
        // Delete an existing layout
        db_delete('om_subthemer_layout')
          ->condition('cid', $layout['cid'], '=')
          ->condition('nid', $layout['nid'], '=')          
          ->execute(); 
                
        drupal_set_message(t('The div (' . $layout['tagid'] . ') has been deleted.'));      
      }
      elseif ($layout['cid']) { 
        $cid++;
        // Update an existing layout  
        $tagid = _om_duplicate_check($tagid, $layout['nid'], $layout['cid']);

        db_update('om_subthemer_layout') // Table name no longer needs {}
          ->fields(array(
            'cid'    => $layout['cid'],
            'pid'    => $layout['pid'],
            'tagid'  => $tagid,
            'type'   => $type,
            'iw'     => $iw,
            'weight' => $layout['weight'],            
          ))
          ->condition('lid', $layout['lid'], '=')
          ->execute();
                
        drupal_set_message(t('The div (' . $tagid . ') has been updated.'));
      }
      elseif (!$layout['cid'] && !empty($layout['tagid'])) {
        $cid++;
        // create a new divs
        $tagid = _om_duplicate_check($tagid, $layout['nid'], 0);

        db_insert('om_subthemer_layout')
          ->fields(array('cid', 'pid', 'tagid', 'type', 'iw', 'weight', 'nid'))
          ->values(array(
            'cid'    => $cid,
            'pid'    => $layout['pid'],  
            'tagid'  => $tagid,                    
            'type'   => $type,
            'iw'     => $iw, 
            'weight' => $layout['weight'],                                                                          
            'nid'    => $layout['nid'],                                                                                                                                                                           
          ))
          ->execute();
                
        drupal_set_message(t('The layout (' . $tagid . ') has been added.'));
      }
      else {
        // If no condition is set, nothing should happen
      }   
    }
    // files need to be written again
  
    // need this to update files on zip
    @include_once OM_SUBTHEMER_PATH . '/inc/om_subthemer.write.inc';    
    
    // need to update the files
    $node = node_load(arg(1));

    // set user folder 
    om_subthemer_user_folder($node);

    // 1) get layout for variables
    om_process_layout($node);
    
    // 2) write om subtheme
    om_file_write($node); 
  }
  elseif ($form_state['values']['op'] == t('Export')) {
    drupal_goto('node/' . arg(1) . '/om-layout/export');
  }
  elseif ($form_state['values']['op'] == t('Import')) {
    drupal_goto('node/' . arg(1) . '/om-layout/import');
  }      
}

/**
 * Delete children of divs
 *
 */
function _om_delete_child($cid = NULL, $nid = NULL) {
  global $_om_subthemer_layout_content;
  $div_delete = array();
  $div_delete = explode(' ', _om_order($_om_subthemer_layout_content[$cid]));
  if (!empty($div_delete)) {
    foreach ($div_delete as $key => $val) {
      if (!empty($val)) {
        db_delete('om_subthemer_layout')
          ->condition('cid', $val, '=')
          ->condition('nid', $nid, '=')          
          ->execute(); 
      }
    } 
  } 
}

/**
 * Duplicate tags checker
 * - adds delta if with duplicates
 * - ex. if container exists, it will become container-0 and so on
 */
function _om_duplicate_check($tagid = NULL, $nid = 0, $cid = 0) {
  $tagid = trim($tagid);
  $sql = "SELECT tagid, cid 
          FROM {om_subthemer_layout} 
          WHERE nid = :nid";

  $result = db_query($sql, array(':nid' => $nid));
  //dsm($result);  
  $tags = array();
  while ($record = $result->fetchObject()) {
    $tags[$record->cid] = $record->tagid;
  }
  
  // this becomes not an array when new layout is generated
  if (is_array($tags)) { 
    //for existing div tags
    if ($cid) unset($tags[$cid]); 

    //for new div tags
    if (in_array($tagid, $tags)) {
      $counter = 0;
      $tagid_new = $tagid . '-' . $counter; 
      while (in_array($tagid_new, $tags)) {
        $tagid_new = $tagid . '-' . $counter++;        
      }
      return $tagid_new;    
    }
    else {
      return $tagid;
    }
  }
}


/**
 * Indentation based on child parent relationships
 * - adding more indents to nested divs 
 */
function _om_indent($struct) {
  global $_om_subthemer_layout_structure;
  
  $out = '';
  foreach ($struct as $pid => $tagid) {
    if (isset($_om_subthemer_layout_structure[$pid])) {
      $out .= _om_indent($_om_subthemer_layout_structure[$pid]);
    }
    $out .= '<div class="indentation">&nbsp;</div>';
  }
  return $out;
}


/**
 * Level based on child parent relationships
 * 
 */
function _om_level($struct) {
  global $_om_subthemer_layout_structure;
  
  $out = '';
  foreach ($struct as $pid => $tagid) {
    if (isset($_om_subthemer_layout_structure[$pid])) {
      $out += _om_level($_om_subthemer_layout_structure[$pid]);
    }
    $out += 1;
  }
  return $out;
}





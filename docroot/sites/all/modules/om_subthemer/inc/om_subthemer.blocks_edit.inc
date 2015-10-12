<?php
// $Id$

/**
 * @file
 * OM Subthemer Blocks
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 * 
 * OM Subthemer blocks editing
 *   - these are dummy blocks which are used for subtheme preview,
 *     you can copy any html blocks directly from the browser source output
 *     paste it here via import function
 *
 */

/**
 * OM Blocks Form
 *
 */
function om_blocks_edit($form, &$form_state) {
  global $_om_subthemer_regions;
    
  $node = node_load(arg(1));

  drupal_set_title(t('Dummy Blocks for %title', array('%title' => check_plain($node->title))), PASS_THROUGH);

  $_om_subthemer_regions = om_regions_get($node->nid);
  //dsm($_om_subthemer_regions);
  // added additional region for no region
  $_om_subthemer_regions['disabled'] = 'Disabled'; 
    
  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $sql = "SELECT bid, title, content, module, region, weight, nid 
          FROM {om_subthemer_blocks} 
          WHERE nid = :nid 
          ORDER BY weight ASC";
  
  $result = db_query($sql, array(':nid' => $node->nid));
  
  $blocks = array();
  while ($record = $result->fetchObject()) {
    // if the regions changes in the layout edit all previous regions
    // will default to disabled
    if (!isset($_om_subthemer_regions[$record->region])) $record->region = 'disabled';
    $blocks[] = _om_blocks_edit($record);
  }
  //dsm($blocks);
  
  // add another blank question fields
  $record = new stdClass();
  $record->bid = 0;
  $record->weight = 0;  
  $record->nid = $node->nid;
  _om_new_rows($blocks, 5, $record, 'blocks_edit'); 
     
  $form['blocks'] += $blocks; 
  
  //dsm($form['blocks']);
  $form['submit'] = array( '#type' => 'submit', '#value' => t('Save Blocks'), );

  $form['export'] = array( '#type' => 'submit', '#value' => t('Export'), );

  $form['import'] = array( '#type' => 'submit', '#value' => t('Import'), );
    
  return $form;
}

/**
 * blocks Form
 *
 */
function _om_blocks_edit($record) {
 
  $form['bid'] = array(
    '#type' => 'hidden',
    '#value' => isset($record->bid) ? $record->bid: '',
    '#attributes' => array('class' => array('om-bid')),    
  );  
  $form['nid'] = array(
    '#type' => 'textfield',
    '#value' => isset($record->nid) ? $record->nid: '',
  );  
  $form['region'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($record->region) ? $record->region: 'disabled',
    '#attributes' => array('class' => array('om-region')),
    '#size' => 15,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($record->title) ? $record->title: '',
    '#size' => 15,
  );
  $form['content'] = array(
    '#type' => 'textarea',
    '#title' => t('Content'),
    '#default_value' => isset($record->content) ? $record->content: '',
    '#cols' => 30,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => isset($record->weight) ? $record->weight: '',
    '#delta' => 50,
    '#attributes' => array('class' => array('om-weight')),
  );
  $form['module'] = array(
    '#type' => 'textfield',
    '#title' => t('Module'),
    '#default_value' => isset($record->module) ? $record->module: '',
    '#size' => 15,
  );  
  $form['delete'] = array(
    '#type' => ($form['bid']['#value']) ? 'checkbox' : 'hidden',
    '#title' => t('Delete'),
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
function theme_om_blocks_edit($vars) {
  global $_om_subthemer_regions;

  $form = $vars['form'];
  
  drupal_add_css(OM_SUBTHEMER_PATH . '/css/om_subthemer.css');

  //drupal_add_tabledrag($table_id, $action, $relationship, $group, $subgroup = NULL, $source = NULL, $hidden = TRUE, $limit = 0);
  drupal_add_tabledrag('om-subthemer-blocks', 'match', 'parent', 'om-region', 'om-region', 'om-section', TRUE, 0);
  drupal_add_tabledrag('om-subthemer-blocks', 'order', 'group', 'om-weight');

  $header = array('Regions &amp; Block Titles', 'Content', 'Module', 'Region', 'Delete'); //, 'Update'
  
  $output = '';
  //dsm($form['blocks']);
  $rows = array();
  //$regions = $form['regions']['#value'];    
  
  //dsm($regions);
  foreach ($_om_subthemer_regions as $region_key => $region_name) {

    $region = array(
      '#type' => 'hidden',
      '#value' => $region_key,
      '#attributes' => array('class' => array('om-section')),
    );
    $rows[] = array(
      'data' => array('data' => $region_name, NULL, NULL, drupal_render($region), NULL),
      'id' => 'om-row-'. $region_key,
      'class' => array('draggable', 'om-row-region', 'tabledrag-root'),
    ); 

    foreach (element_children($form['blocks']) as $key) {

      if ($form['blocks'][$key]['region']['#value'] == $region_key) {
        
        $row = array();
        // Strips labels
        $form['blocks'][$key]['title']['#title'] = '';
        $form['blocks'][$key]['content']['#title'] = '';
        $form['blocks'][$key]['module']['#title'] = '';
        $form['blocks'][$key]['region']['#title'] = '';        
        $form['blocks'][$key]['weight']['#title'] = '';      
        $form['blocks'][$key]['delete']['#title'] = '';    

        // 0 as parent has no indent
        ($form['blocks'][$key]['region']['#value'] != NULL) ? $indent = '<div class="indentation">&nbsp;</div>': $indent = '';
  
        $row[] = array('class' => array('col-title'), 'data' => $indent . drupal_render($form['blocks'][$key]['title']));
        $row[] = array('class' => array('col-content'), 'data' => drupal_render($form['blocks'][$key]['content']));
        $row[] = array('class' => array('col-module'), 'data' => drupal_render($form['blocks'][$key]['module']));
        $row[] = array('class' => array('col-hidden'), 'data' => drupal_render($form['blocks'][$key]['region']) . drupal_render($form['blocks'][$key]['bid']) . drupal_render($form['blocks'][$key]['nid']) . drupal_render($form['blocks'][$key]['weight']));
        $row[] = array('class' => array('col-delete'), 'data' => drupal_render($form['blocks'][$key]['delete']));    

        $rows[] = array(
          'data' => $row,
          'id' => 'om-row-'. $key,
          'class' => array('draggable', 'row-'. $key, 'tabledrag-leaf'),
        ); 
      }
    }
  }
  //dsm($rows);

  // Header
  $form['blocks']['#children'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'om-subthemer-blocks')));
  
  $output .= drupal_render_children($form);
  
  return $output;
}

/**
 * blocks Submit
 * table: bid, title, content, module, region, weight, nid
 */
function om_blocks_edit_submit($form, $form_state) {

  if ($form_state['values']['op'] == t('Save Blocks')) { 
    
    $form_values = $form_state['values'];

    foreach ($form_values['blocks'] as $block) {

      if ($block['delete']) {
        // Delete an existing layout
        db_delete('om_subthemer_blocks')
          ->condition('bid', $block['bid'], '=')
          ->execute(); 
                
        drupal_set_message(t('The block (' . $block['title'] . ') has been deleted.'));      
      }
      elseif ($block['bid']) { 

        db_update('om_subthemer_blocks') // Table name no longer needs {}
          ->fields(array(
            'title'   => $block['title'],
            'content' => $block['content'],
            'module'  => $block['module'],
            'region'  => $block['region'],
            'weight'  => $block['weight'],
          ))
          ->condition('bid', $block['bid'], '=')
          ->execute();
                
        drupal_set_message(t('The block (' . $block['title'] . ') has been updated.'));
      }
      elseif (!$block['bid'] && !empty($block['title'])) {
        // new content for blocks
        db_insert('om_subthemer_blocks')
          ->fields(array('title', 'content', 'module', 'region', 'weight', 'nid'))
          ->values(array(
            'title'   => $block['title'],
            'content' => $block['content'],
            'module'  => $block['module'],
            'region'  => $block['region'],
            'weight'  => $block['weight'],
            'nid'     => $block['nid'],                                                                                                                                                                           
          ))
          ->execute();
                
        drupal_set_message(t('The block (' . $block['title'] . ') has been added.'));
      }
      else {
        // If no condition is set, nothing should happen
      }   
    }
    // files need to be written again
  
    // need this to update files on zip
    include_once OM_SUBTHEMER_PATH . '/inc/om_subthemer.write.inc';    
    
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
    drupal_goto('node/' . arg(1) . '/om-blocks/export');
  }
  elseif ($form_state['values']['op'] == t('Import')) {
    drupal_goto('node/' . arg(1) . '/om-blocks/import');
  }          
}







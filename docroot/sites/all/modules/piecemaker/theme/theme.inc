<?php
/**
 * @file
 *   Contains various theme functions for Piecemaker API
 */

/**
 * Themes the actuall Piecemaker element
 * 
 * @param $vars
 *   An array that contains the handler, key, profile array, and alternate content callback function 
 */
function theme_piecemaker($vars) {
  $settings = &drupal_static('theme_piecemaker_settings');
  $ids = &drupal_static(__FUNCTION__, array());
  $handler = $vars['handler'];
  $key = $vars['key'];
  $profile = $vars['profile'];
  $alternate_callback = $vars['alternate_callback'];
  if (is_object($profile)) {
    //Cast the profile to an array to make it easier to use
    $profile = (array) $profile;
  }
  $id = 'piecemaker-' . $handler . '-' . $key;
  $i = 0;
  while (in_array($id, $ids)) {
    //Ensure we have a unique html ID to use.
    $id .= '-' . $i;
    $i++;
  }
  //Store this id
  $ids[] = $id;
  $pm_path = libraries_get_path('piecemaker');
  $path = drupal_get_path('module', 'piecemaker');
  //Add The JS we need
  drupal_add_js($pm_path . '/swfobject/swfobject.js');
  drupal_add_js($path . '/js/Piecemaker.js');
  
  
  $settings['Piecemaker_URI'] = base_path() . $pm_path;
  $settings['Piecemaker'][$id] = array(
    'id' => $id,
    'flashvars' => array(
       'xmlSource' => base_path() . "piecemaker/{$handler}/{$key}/settings.xml",
       'cssSource' => base_path() . $profile['flash_settings']['flashvars']['cssSource'],
     ),
     'width' => $profile['flash_settings']['width'],
     'height' => $profile['flash_settings']['height'],
  );
  $settings['Piecemaker'][$id]['params'] = $profile['flash_settings']['params'];
  $alternate = '<p>You do not have flash enabled</p>';
  if (function_exists($alternate_callback)) {
    $alternate = call_user_func_array($alternate_callback, $vars);
  }
  $out = "<div id=\"{$id}\" class=\"piecemaker piecemaker-{$handler} piecemaker-{$handler}-{$key}\">
  {$alternate}
   </div>";
   
   return $out;
}

/**
 * Themes the tranistion add/delete protion of the proile form
 */
function theme_piecemaker_transition_form($vars) {
  $form = $vars['form'];
  $tvars['header'] = array(
    t('Effect'),
    t('Weight'),
    t('Pieces'),
    t('Time'),
    t('Delay'),
    t('Depth Offset'),
    t('Cube Distance'),
    t('Add/Delete')
  );
  foreach(element_children($form) as $key) {
    $row = array();
    if ($key === 'add') {
      //We need to create a item list that will be appended to the bottom of the form
      //since the descriptions are getting too big for the table and break the layout
      $head = $tvars['header'];
      $items[] =  array_shift($head) . ': ' . $form[$key]['Transition']['#description'];
      unset($form[$key]['Transition']['#description']);
      array_shift($head); //To get rid of the weight column
      $items[] =  array_shift($head) . ': ' . $form[$key]['Pieces']['#description'];
      unset($form[$key]['Pieces']['#description']);
      $items[] =  array_shift($head) . ': ' . $form[$key]['Time']['#description'];
      unset($form[$key]['Time']['#description']);
      $items[] =  array_shift($head) . ': ' . $form[$key]['Delay']['#description'];
      unset($form[$key]['Delay']['#description']);
      $items[] =  array_shift($head) . ': ' . $form[$key]['DepthOffset']['#description'];
      unset($form[$key]['DepthOffset']['#description']);
      $items[] =  array_shift($head) . ': ' . $form[$key]['CubeDistance']['#description'];
      unset($form[$key]['CubeDistance']['#description']);
    } 
    $row[] = drupal_render($form[$key]['Transition']);
    $row[] = drupal_render($form[$key]['weight']);
	  $row[] = drupal_render($form[$key]['Pieces']);
	  $row[] = drupal_render($form[$key]['Time']);
	  $row[] = drupal_render($form[$key]['Delay']);
	  $row[] = drupal_render($form[$key]['DepthOffset']);
	  $row[] = drupal_render($form[$key]['CubeDistance']);
    $row[] = drupal_render($form[$key]['action']);
    $tvars['rows'][] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }
  $tvars['attributes'] = array(
    'id' => 'piecemaker-transitions',
    'class' => array('no-sticky'),
  );
  //Sticky headers don't work with draggable tables.
  $tvars['sticky'] = FALSE;
  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => 'Field Descriptions',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $fieldset['list']['#markup'] = theme('item_list', array('items' => $items));
  drupal_add_tabledrag('piecemaker-transitions', 'order', 'sibling', 'trans-weight');
  return theme('table', $tvars) . drupal_render($fieldset);
}

/**
 * Themes the flash params protion of the proile form
 */
function theme_piecemaker_profile_params($vars) {
  $form = $vars['form'];
  $add = drupal_render($form['add']);
  foreach(element_children($form) as $key) {
    $row = array();
    $row[] = drupal_render($form[$key]['key']);
    $row[] = drupal_render($form[$key]['value']);
    $tvars['rows'][] = $row;
  }
  $tvars['header'] = array(
    t('Key'),
    t('Value'),
  );
  $tvars['attributes'] = array(
    'id' => 'piecemaker-params',
  );
  return theme('table', $tvars) . $add;
}
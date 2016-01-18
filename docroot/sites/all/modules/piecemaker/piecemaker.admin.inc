<?php

function piecemaker_profiles_page() {
  $profiles = db_query('SELECT * FROM {piecemaker_profiles}');
  
  if (!$profiles->rowCount()) {
    return t('There are no Piecemaker profiles yet. !link', array('!link' => l('Please add one.', 'admin/config/media/piecemaker/profiles/add')));
  }
  while ($profile = $profiles->fetch()) {
    $row = array();
    $row[] = $profile->title;
    $row[] = l('Edit', "admin/config/media/piecemaker/profiles/{$profile->pid}/edit");
    $row[] = l('Delete', "admin/config/media/piecemaker/profiles/{$profile->pid}/delete");
    $vars['rows'][] = $row;    
  }
  $vars['header']= array(t('Profile Title'), t('Edit Profile'), t('Delete Profile'));
  return theme('table', $vars);
}

function piecemaker_settings_page($form, &$form_state) {
  if (!empty($form_state['submitted']) && !empty($form_state['rebuild'])) {
    unset($form_state['values']['piecemaker_default_transitions']['add'], $form_state['input']['piecemaker_default_transitions']['add']);
  }
  $form_state['#transition_element'] = 'piecemaker_default_transitions';
  $vals = _piecemaker_default_settings();  
  $form['piecemaker_default_transitions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Transitions'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#theme' => 'piecemaker_transition_form',
    '#prefix' => '<div id="transitions-wrap">',
    '#suffix' => '</div>',
  );
  $flash = _piecemaker_default_flash_settings();
  $form['flash_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flash Settings'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['flash_settings']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Flash Width'),
    '#description' => t('Width of Flash object. Should be larger then "Image Width" below.'),
    '#default_value' => $flash['width'],
    '#field_suffix' => 'pixels',
  );
  $form['flash_settings']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Flash Height'),
    '#description' => t('Height of Flash object. Should be larger then "Image Height" below.'),
    '#default_value' => $flash['height'],
    '#field_suffix' => 'pixels',
  );
  $form['flash_settings']['flashvars']['cssSource'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom CSS'),
    '#default_value' => $flash['flashvars']['cssSource'],
    '#description' => t('A custom CSS file to use for this Piecemaker instance. If one is not provided the default piecemaker.css will be used. Please provide the path relative this Drupal install.'),
  );
  $form['flash_settings']['params'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flash Paramaters'),
    '#theme' => 'piecemaker_profile_params',
    '#prefix' => '<div id="piecemaker-flash-params">',
    '#suffix' => '</div>',
  );
  $params = !empty($form_state['flash_settings']['params']) ? $form_state['flash_settings']['params'] : $flash['params'];
  foreach($params as $key => $value) {
    $form['flash_settings']['params'][$key]['key'] = array(
      '#type' => 'textfield',
      '#default_value' => $key,
    );
    $form['flash_settings']['params'][$key]['value'] = array(
      '#type' => 'textfield',
      '#default_value' => $value,
    );
  }
  $extra = !empty($form_state['params_extra']) ? $form_state['params_extra'] : 1;
  for ($i = 0; $i < $extra; $i++) {
    $form['flash_settings']['params'][$i]['key'] = array(
      '#type' => 'textfield',
    );
    $form['flash_settings']['params'][$i]['value'] = array(
      '#type' => 'textfield',
    );
  }
  $form['flash_settings']['params']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add Paramaters'),
    '#submit' => array('piecemaker_add_params_rows'),
    '#ajax' => array(
      'wrapper' => 'piecemaker-flash-params',
      'callback' => 'piecemaker_add_flash_params',
    ),
  );
  $transitions = variable_get('piecemaker_default_transitions', array());
  if (isset($form_state['unsaved_transitions']) && is_array($form_state['unsaved_transitions'])) {
    $transitions = $form_state['unsaved_transitions'];
    drupal_set_message('Transitions have not been saved yet. You must click "Save Settings" to save your changes.', 'warning');
  }
  $trans_options = _piecemaker_transitions();
  $weights = range(0, 50);
  $i = 1;
  foreach($transitions as $key => $transition) {
    $form['piecemaker_default_transitions'][$key]['weight'] = array(
      '#type' => 'select',
      '#options' => $weights,
      '#attributes' => array('class' => array('trans-weight')),
      '#default_value' => array($key),
    );
    $form['piecemaker_default_transitions'][$key]['Transition'] = array(
      '#type' => 'hidden',
      '#value' => $transition['Transition'],
      '#suffix' => $transition['Transition'],
    );
    $form['piecemaker_default_transitions'][$key]['Pieces'] = array(
      '#type' => 'hidden',
      '#value' => $transition['Pieces'],
      '#suffix' => $transition['Pieces'],
    );
    $form['piecemaker_default_transitions'][$key]['Time'] = array(
      '#type' => 'hidden',
      '#value' => $transition['Time'],
      '#suffix' => $transition['Time'],
    );
    $form['piecemaker_default_transitions'][$key]['Delay'] = array(
      '#type' => 'hidden',
      '#value' => $transition['Delay'],
      '#suffix' => $transition['Delay'],
    );
    $form['piecemaker_default_transitions'][$key]['DepthOffset'] = array(
      '#type' => 'hidden',
      '#value' => $transition['DepthOffset'],
      '#suffix' => $transition['DepthOffset'],
    );
    $form['piecemaker_default_transitions'][$key]['CubeDistance'] = array(
      '#type' => 'hidden',
      '#value' => $transition['CubeDistance'],
      '#suffix' => $transition['CubeDistance'],
    );
    $form['piecemaker_default_transitions'][$key]['action'] = array(
      '#type' => 'submit',
      '#value' => 'Delete Transition ' . $i,
      '#submit' => array('piecemaker_delete_transition'),
      '#ajax' => array(
        'callback' => 'piecemaker_add_transition_ajax',
        'wrapper' => 'transitions-wrap',
        'effect' => 'fade',
        'progress' => array(
          'type' => 'throbber',
          'message' => t('Deleting Transition'),
        ),
      ),
    );
    $i++;      
  }
  $form['piecemaker_default_transitions']['add']['weight'] = array(
    '#type' => 'select',
    '#options' => $weights,
    '#attributes' => array('class' => array('trans-weight')),
    '#default_value' => array(50),
  );
  $form['piecemaker_default_transitions']['add']['Transition'] = array(
    '#type' => 'select',
    '#options' => $trans_options,
    '#description' => t('Transition type of the Tweener class. For more info on these types see the official Tweener Documentation and go to "Transition Types“". The best results are achieved by those transition types, which begin with easeInOut.'),
  );
  $pieces = range(0,10);
  unset($pieces[0]);
  $form['piecemaker_default_transitions']['add']['Pieces'] = array(
    '#type' => 'select',
    '#options' => $pieces,
    '#description' => t('Number of pieces to which the image is sliced'),
  );
  $form['piecemaker_default_transitions']['add']['Time'] = array(
    '#type' => 'textfield',
    '#description' => t('Time for one cube to turn'),
    '#default_value' => '1.2',
  );
  $form['piecemaker_default_transitions']['add']['Delay'] = array(
    '#type' => 'textfield',
    '#description' => t('Delay between the start of one cube to the start of the next cube'),
    '#default_value' => '.1',
  );
  $form['piecemaker_default_transitions']['add']['DepthOffset'] = array(
    '#type' => 'textfield',
    '#description' => t('The offset during transition on the z-axis. Value between 100 and 1000 are recommended. But go for experiments. :)'),
    '#default_value' => '300',
  );
  $form['piecemaker_default_transitions']['add']['CubeDistance'] = array(
    '#type' => 'textfield',
    '#description' => t('The distance between the cubes during transition. Values between 5 and 50 are recommended. But go for experiments.:)'),
    '#default_value' => '30',
  );
  $form['piecemaker_default_transitions']['add']['action'] = array(
    '#type' => 'submit',
    '#value' => 'Add Transition',
    '#submit' => array('piecemaker_add_transition'),
    '#ajax' => array(
	    'callback' => 'piecemaker_add_transition_ajax',
	    'wrapper' => 'transitions-wrap',
	    'effect' => 'fade',
	    'progress' => array(
	      'type' => 'throbber',
        'message' => t('Adding Transition'),
      ),
	  ),
  );
  $form['piecemaker_default_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Settings'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['piecemaker_default_settings']['ImageWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Width'),
    '#description' => t('Width of every Image. ex: "900"'),
    '#default_value' => $vals['ImageWidth'],
  );
  $form['piecemaker_default_settings']['ImageHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Height'),
    '#description' => t('Height of every Image. ex: "360"'),
    '#default_value' => $vals['ImageHeight'],
  );
  $form['piecemaker_default_settings']['LoaderColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Loader Color'),
    '#description' => t('Color of the cubes before the first image appears, also the color of the back sides of the cube, which become visible at some transition types. ex: "0x333333"'),
    '#default_value' => $vals['LoaderColor'],
  );
  $form['piecemaker_default_settings']['InnerSideColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Inner Side Color'),
    '#description' => t('Color of the inner sides of the cube when sliced. ex: "0x222222"'),
    '#default_value' => $vals['InnerSideColor'],
  );
  $form['piecemaker_default_settings']['SideShadowAlpha'] = array(
    '#type' => 'textfield',
    '#title' => t('Side Shadow Alpha'),
    '#description' => t('Sides get darker when moved away from the front. This is the degree of darkness - 0 == no change, 1 == 100% black. ex: "0.8"'),
    '#default_value' => $vals['SideShadowAlpha'],
  );
  $form['piecemaker_default_settings']['DropShadowAlpha'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Alpha'),
    '#description' => t('Alpha of the drop shadow - 0 == no shadow, 1 == opaque. ex: "0.7"'),
    '#default_value' => $vals['DropShadowAlpha'],
  );
  $form['piecemaker_default_settings']['DropShadowDistance'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Distance'),
    '#description' => t('Distance of the shadow from the bottom of the image. ex: "25"'),
    '#default_value' => $vals['DropShadowDistance'],
  );
  $form['piecemaker_default_settings']['DropShadowScale'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Scale'),
    '#description' => t('As the shadow is blurred, it appears wider that the actual image, when not resized. Thus it‘s a good idea to make it slightly smaller. - 1 would be no resizing at all.. ex: "0.95"'),
    '#default_value' => $vals['DropShadowScale'],
  );
  $form['piecemaker_default_settings']['DropShadowBlurX'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Blur-X'),
    '#description' => t('Blur of the drop shadow on the x-axis. ex: "40"'),
    '#default_value' => $vals['DropShadowBlurX'],
  );
  $form['piecemaker_default_settings']['DropShadowBlurY'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Blur-Y'),
    '#description' => t('Blur of the drop shadow on the y-axis. ex: "4"'),
    '#default_value' => $vals['DropShadowBlurY'],
  );
  $form['piecemaker_default_settings']['MenuDistanceX'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Distance-X'),
    '#description' => t('Distance between two menu items (from center to center). ex: "20"'),
    '#default_value' => $vals['MenuDistanceX'],
  );
  $form['piecemaker_default_settings']['MenuDistanceY'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Distance-Y'),
    '#description' => t('Distance of the menu from the bottom of the image. ex: "50"'),
    '#default_value' => $vals['MenuDistanceY'],
  );
  $form['piecemaker_default_settings']['MenuColor1'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Color 1'),
    '#description' => t('Color of an inactive menu item. ex: "0x999999"'),
    '#default_value' => $vals['MenuColor1'],
  );
  $form['piecemaker_default_settings']['MenuColor2'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Color 2'),
    '#description' => t('Color of an active menu item. ex: "0x333333"'),
    '#default_value' => $vals['MenuColor2'],
  );
  $form['piecemaker_default_settings']['MenuColor3'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Color 3'),
    '#description' => t('Color of the inner circle of an active menu item. Should equal the background color of the whole thing.. ex: "0xFFFFFF"'),
    '#default_value' => $vals['MenuColor3'],
  );
  $form['piecemaker_default_settings']['ControlSize'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Size'),
    '#description' => t('Size of the controls, which appear on rollover (play, stop, info, link) . ex: "100"'),
    '#default_value' => $vals['ControlSize'],
  );
  $form['piecemaker_default_settings']['ControlDistance'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Distance'),
    '#description' => t('Distance between the controls (from the borders). ex: "20"'),
    '#default_value' => $vals['ControlDistance'],
  );
  $form['piecemaker_default_settings']['ControlColor1'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Color 1'),
    '#description' => t('Background color of the controls. ex: "0x222222"'),
    '#default_value' => $vals['ControlColor1'],
  );
  $form['piecemaker_default_settings']['ControlColor2'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Color 2'),
    '#description' => t('Font color of the controls. ex: "0xFFFFFF"'),
    '#default_value' => $vals['ControlColor2'],
  );
  $form['piecemaker_default_settings']['ControlAlpha'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Alpha'),
    '#description' => t('Alpha of a control, when mouse is not over. ex: "0.8"'),
    '#default_value' => $vals['ControlAlpha'],
  );
  $form['piecemaker_default_settings']['ControlAlphaOver'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Alpha Over'),
    '#description' => t('Alpha of a control, when mouse is over. ex: "0.95"'),
    '#default_value' => $vals['ControlAlphaOver'],
  );
  $form['piecemaker_default_settings']['ControlsX'] = array(
    '#type' => 'textfield',
    '#title' => t('Controls X'),
    '#description' => t('X-position of the point, which aligns the controls (measured from [0,0] of the image). ex: "450"'),
    '#default_value' => $vals['ControlsX'],
  );
  $form['piecemaker_default_settings']['ControlsY'] = array(
    '#type' => 'textfield',
    '#title' => t('Controls Y'),
    '#description' => t('Y-position of the point, which aligns the controls (measured from [0,0] of the image). ex: "280"'),
    '#default_value' => $vals['ControlsY'],
  );
  $form['piecemaker_default_settings']['ControlsAlign'] = array(
    '#type' => 'textfield',
    '#title' => t('Controls Align'),
    '#description' => t('Type of alignment from the point [controlsX, controlsY] - can be "center", "left" or "right". ex: "center"'),
    '#default_value' => $vals['ControlsAlign'],
  );
  $form['piecemaker_default_settings']['TooltipHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Height'),
    '#description' => t('Height of the tooltip surface in the menu. ex: "31"'),
    '#default_value' => $vals['TooltipHeight'],
  );
  $form['piecemaker_default_settings']['TooltipColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Color'),
    '#description' => t('Color of the tooltip surface in the menu. ex: "0x222222"'),
    '#default_value' => $vals['TooltipColor'],
  );
  $form['piecemaker_default_settings']['TooltipTextY'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text-Y'),
    '#description' => t('Y-distance of the tooltip text field from the top of the tooltip. ex: "5"'),
    '#default_value' => $vals['TooltipTextY'],
  );
  $form['piecemaker_default_settings']['TooltipTextStyle'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text Style'),
    '#description' => t('The style of the tooltip text, specified in the CSS file. ex: "P-Italic"'),
    '#default_value' => $vals['TooltipTextStyle'],
  );
  $form['piecemaker_default_settings']['TooltipTextColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text Color'),
    '#description' => t('Color of the tooltip text. ex: "0xFFFFFF"'),
    '#default_value' => $vals['TooltipTextColor'],
  );
  $form['piecemaker_default_settings']['TooltipMarginLeft'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Margin Left'),
    '#description' => t('Margin of the text to the left end of the tooltip. ex: "5"'),
    '#default_value' => $vals['TooltipMarginLeft'],
  );
  $form['piecemaker_default_settings']['TooltipMarginRight'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Margin Right'),
    '#description' => t('Margin of the text to the right end of the tooltip. ex: "7"'),
    '#default_value' => $vals['TooltipMarginRight'],
  );
  $form['piecemaker_default_settings']['TooltipTextSharpness'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text Sharpness'),
    '#description' => t('Sharpness of the tooltip text (-400 to 400) - see Adobe Docs. ex: "50"'),
    '#default_value' => $vals['TooltipTextSharpness'],
  );
  $form['piecemaker_default_settings']['TooltipTextThickness'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text Thickness'),
    '#description' => t('Thickness of the tooltip text (-400 to 400) - see Adobe Docs. ex: "-100"'),
    '#default_value' => $vals['TooltipTextThickness'],
  );
  $form['piecemaker_default_settings']['InfoWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Width'),
    '#description' => t('The width of the info text field. ex: "400"'),
    '#default_value' => $vals['InfoWidth'],
  );
  $form['piecemaker_default_settings']['InfoBackground'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Background'),
    '#description' => t('The background color of the info text field. ex: "0xFFFFFF"'),
    '#default_value' => $vals['InfoBackground'],
  );
  $form['piecemaker_default_settings']['InfoBackgroundAlpha'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Background Alpha'),
    '#description' => t('The alpha of the background of the info text, the image shines through, when smaller than 1. ex: "0.95"'),
    '#default_value' => $vals['InfoBackgroundAlpha'],
  );
  $form['piecemaker_default_settings']['InfoMargin'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Margin'),
    '#description' => t('The margin of the text field in the info section to all sides. ex: "15"'),
    '#default_value' => $vals['InfoMargin'],
  );
  $form['piecemaker_default_settings']['InfoSharpness'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Sharpness'),
    '#description' => t('Sharpness of the info text (see above). ex: "0"'),
    '#default_value' => $vals['InfoSharpness'],
  );
  $form['piecemaker_default_settings']['InfoThickness'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Thickness'),
    '#description' => t('Thickness of the info text (see above). ex: "0"'),
    '#default_value' => $vals['InfoThickness'],
  );
  $form['piecemaker_default_settings']['Autoplay'] = array(
    '#type' => 'textfield',
    '#title' => t('Autoplay'),
    '#description' => t('Number of seconds from one transition to another, if not stopped. Set to 0 to disable autoplay. ex: "10"'),
    '#default_value' => $vals['Autoplay'],
  );
  $form['piecemaker_default_settings']['FieldOfView'] = array(
    '#type' => 'textfield',
    '#title' => t('Field Of View'),
    '#description' => t('See the official Adobe Docs. ex: "45"'),
    '#default_value' => $vals['FieldOfView'],
  );
  
  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
    '#weight' => -5,
  );
  return $form;
}

function piecemaker_settings_page_submit($form, &$form_state) {
  $default_transitions = array();
  variable_set('piecemaker_default_settings', $form_state['values']['piecemaker_default_settings']);
  $transitions = $form_state['values']['piecemaker_default_transitions'];
  //Get rid of the Add row since they didn't click it here.
  unset($transitions['add']);
  uasort($transitions, 'drupal_sort_weight');
  foreach ($transitions as $trans) {
    unset($trans['weight']);
	unset($trans['action']);
    $default_transitions[] = $trans; 
  }
  variable_set('piecemaker_default_transitions', $default_transitions);
  $flash = $form_state['values']['flash_settings'];
  $params = $flash['params'];
  unset($params['add'], $flash['params']);
  foreach($params as $param) {
    if (!empty($param['value'])) {
      $flash['params'][$param['key']] = $param['value'];
    }    
  }
  variable_set('piecemaker_default_flash_settings', $flash);
}

function piecemaker_add_transition($form, &$form_state) {
  $key = $form_state['#transition_element'];
  $transitions = $form_state['values'][$key];
  uasort($transitions, 'drupal_sort_weight');
  foreach ($transitions as $trans) {
    unset($trans['weight']);
	  unset($trans['action']);
    $default_transitions[] = $trans; 
  }
  $form_state['unsaved_transitions'] = $default_transitions;
  $form_state['rebuild'] = TRUE;
  drupal_set_message('Transition Added');
}

function piecemaker_delete_transition($form, &$form_state) {
  $elem = $form_state['#transition_element'];
  $default_transitions = array();
  $clicked = $form_state['clicked_button'];
  $key = $clicked['#parents'][1];
  $transitions = $form_state['values'][$elem];
  unset($transitions[$key]);
  //Kill the add row since it's not needed;
  unset($transitions['add']);
  uasort($transitions, 'drupal_sort_weight');
  foreach ($transitions as $trans) {
    unset($trans['weight']);
    $default_transitions[] = $trans; 
  }
  $form_state['unsaved_transitions'] = $default_transitions;
  $form_state['rebuild'] = TRUE;
  drupal_set_message('Deleted Transition');
}

/**
 * Generates a Profile settings form
 * 
 * The various settings available are defined by the Piecemaker Library.
 * The following settings have to be in place within the XML file to control the Piecemaker component. You can find them all in the Settings node.
 *  - ImageWidth = "900" - Width of every Image
 *  - ImageHeight = "360" - Height of every Image
 *  - LoaderColor = "0x333333" - Color of the cubes before the first image appears, also the color of the back sides of the cube, which become visible at some transition types
 *  - InnerSideColor = "0x222222" - Color of the inner sides of the cube when sliced
 *  - SideShadowAlpha = "0.8" - Sides get darker when moved away from the front. This is the degree of darkness - 0 == no change, 1 == 100% black
 *  - DropShadowAlpha = "0.7" - Alpha of the drop shadow - 0 == no shadow, 1 == opaque
 *  - DropShadowDistance = "25" - Distance of the shadow from the bottom of the image
 *  - DropShadowScale = "0.95" - As the shadow is blurred, it appears wider that the actual image, when not resized. Thus it‘s a good idea to make it slightly smaller. - 1 would be no resizing at all.
 *  - DropShadowBlurX = "40" - Blur of the drop shadow on the x-axis
 *  - DropShadowBlurY = "4" - Blur of the drop shadow on the y-axis
 *  - MenuDistanceX = "20" - Distance between two menu items (from center to center)
 *  - MenuDistanceY = "50" - Distance of the menu from the bottom of the image
 *  - MenuColor1 = "0x999999" - Color of an inactive menu item
 *  - MenuColor2 = "0x333333" - Color of an active menu item
 *  - MenuColor3 = "0xFFFFFF" - Color of the inner circle of an active menu item. Should equal the background color of the whole thing.
 *  - ControlSize = "100" - Size of the controls, which appear on rollover (play, stop, info, link) 
 *  - ControlDistance = "20" - Distance between the controls (from the borders)
 *  - ControlColor1 = "0x222222" - Background color of the controls
 *  - ControlColor2 = "0xFFFFFF" - Font color of the controls
 *  - ControlAlpha = "0.8" - Alpha of a control, when mouse is not over
 *  - ControlAlphaOver = "0.95" - Alpha of a control, when mouse is over
 *  - ControlsX = "450" - X-position of the point, which aligns the controls (measured from [0,0] of the image)
 *  - ControlsY = "280" - Y-position of the point, which aligns the controls (measured from [0,0] of the image)
 *  - ControlsAlign = "center" - Type of alignment from the point [controlsX, controlsY] - can be "center", "left" or "right"
 *  - TooltipHeight = "31" - Height of the tooltip surface in the menu
 *  - TooltipColor = "0x222222" - Color of the tooltip surface in the menu
 *  - TooltipTextY = "5" - Y-distance of the tooltip text field from the top of the tooltip
 *  - TooltipTextStyle = "P-Italic" - The style of the tooltip text, specified in the CSS file
 *  - TooltipTextColor = "0xFFFFFF" - Color of the tooltip text
 *  - TooltipMarginLeft = "5" - Margin of the text to the left end of the tooltip
 *  - TooltipMarginRight = "7" - Margin of the text to the right end of the tooltip
 *  - TooltipTextSharpness = "50" - Sharpness of the tooltip text (-400 to 400) - see Adobe Docs
 *  - TooltipTextThickness = "-100" - Thickness of the tooltip text (-400 to 400) - see Adobe Docs
 *  - InfoWidth = "400" - The width of the info text field
 *  - InfoBackground = "0xFFFFFF" - The background color of the info text field
 *  - InfoBackgroundAlpha = "0.95" - The alpha of the background of the info text, the image shines through, when smaller than 1
 *  - InfoMargin = "15" - The margin of the text field in the info section to all sides
 *  - InfoSharpness = "0" - Sharpness of the info text (see above)
 *  - InfoThickness = "0" - Thickness of the info text (see above)
 *  - Autoplay = "10" - Number of seconds from one transition to another, if not stopped. Set to 0 to disable autoplay
 *  - FieldOfView = "45" - see the official Adobe Docs
 * 
 * @see piecemaker_profile_form_submit()
 * @ingroup forms
 * @param $form['settings']_state
 *   An associative array containing the current state of the form; not used.
 * @param $profile
 *   A Piecemaker profile object
 * @return
 *   Form definition.
 */
function piecemaker_profile_form($form, &$form_state, $profile = NULL) {
  if (!empty($form_state['submitted']) && !empty($form_state['rebuild'])) {
    unset($form_state['values']['transitions']['add'], $form_state['input']['transitions']['add']);
    unset($form_state['values']['flash_settings']['params'][0], $form_state['input']['flash_settings']['params'][0]);
  }
  $form_state['cache'] = TRUE;
  $form_state['#transition_element'] = 'transitions';
  $vals = !empty($profile->settings) ? $profile->settings : _piecemaker_default_settings();
  $flash = !empty($profile->flash_settings) ? $profile->flash_settings : _piecemaker_default_flash_settings();
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => !empty($profile->pid) ? $profile->pid : NULL,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile Name'),
    '#description' => t('A descriptive name for this Profile. It will be used when Profiles are listed out.'),
    '#default_value' => !empty($profile->title) ? $profile->title : NULL,
    '#required' => TRUE,
  );
  $form['flash_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flash Settings'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['flash_settings']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Flash Width'),
    '#description' => t('Width of Flash object. Should be larger then "Image Width" below.'),
    '#default_value' => $flash['width'],
    '#field_suffix' => 'pixels',
  );
  $form['flash_settings']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Flash Height'),
    '#description' => t('Height of Flash object. Should be larger then "Image Height" below.'),
    '#default_value' => $flash['height'],
    '#field_suffix' => 'pixels',
  );
  $form['flash_settings']['flashvars']['cssSource'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom CSS'),
    '#default_value' => $flash['flashvars']['cssSource'],
    '#description' => t('A custom CSS file to use for this Piecemaker instance. If one is not provided the default piecemaker.css will be used. Please provide the path relative this Drupal install.'),
  );
  $form['flash_settings']['params'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flash Paramaters'),
    '#theme' => 'piecemaker_profile_params',
    '#prefix' => '<div id="piecemaker-flash-params">',
    '#suffix' => '</div>',
  );
  $params = !empty($form_state['flash_settings']['params']) ? $form_state['flash_settings']['params'] : $flash['params'];
  foreach($params as $key => $value) {
    $form['flash_settings']['params'][$key]['key'] = array(
      '#type' => 'textfield',
      '#default_value' => $key,
    );
    $form['flash_settings']['params'][$key]['value'] = array(
      '#type' => 'textfield',
      '#default_value' => $value,
    );
  }
  $extra = !empty($form_state['params_extra']) ? $form_state['params_extra'] : 1;
  for ($i = 0; $i < $extra; $i++) {
    $form['flash_settings']['params'][$i]['key'] = array(
      '#type' => 'textfield',
    );
    $form['flash_settings']['params'][$i]['value'] = array(
      '#type' => 'textfield',
    );
  }
  $form['flash_settings']['params']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add Paramaters'),
    '#submit' => array('piecemaker_add_params_rows'),
    '#ajax' => array(
      'wrapper' => 'piecemaker-flash-params',
      'callback' => 'piecemaker_add_flash_params',
    ),
  );
  $transitions = !empty($profile->transitions) ? $profile->transitions : variable_get('piecemaker_default_transitions', array());
  if (isset($form_state['unsaved_transitions']) && is_array($form_state['unsaved_transitions'])) {
    $transitions = $form_state['unsaved_transitions'];
    drupal_set_message('Transitions have not been saved yet. You must click "Save Settings" to save your changes.', 'warning');
  }
  $form['transitions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transitions'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#theme' => 'piecemaker_transition_form',
    '#prefix' => '<div id="transitions-wrap">',
    '#suffix' => '</div>',
  );
  $trans_options = _piecemaker_transitions();
  $weights = range(0, 50);
  $i = 1;
  foreach($transitions as $key => $transition) {
    $form['transitions'][$key]['weight'] = array(
      '#type' => 'select',
      '#options' => $weights,
      '#attributes' => array('class' => array('trans-weight')),
      '#default_value' => array($key),
    );
    $form['transitions'][$key]['Transition'] = array(
      '#type' => 'hidden',
      '#value' => $transition['Transition'],
      '#suffix' => $transition['Transition'],
    );
    $form['transitions'][$key]['Pieces'] = array(
      '#type' => 'hidden',
      '#value' => $transition['Pieces'],
      '#suffix' => $transition['Pieces'],
    );
    $form['transitions'][$key]['Time'] = array(
      '#type' => 'hidden',
      '#value' => $transition['Time'],
      '#suffix' => $transition['Time'],
    );
    $form['transitions'][$key]['Delay'] = array(
      '#type' => 'hidden',
      '#value' => $transition['Delay'],
      '#suffix' => $transition['Delay'],
    );
    $form['transitions'][$key]['DepthOffset'] = array(
      '#type' => 'hidden',
      '#value' => $transition['DepthOffset'],
      '#suffix' => $transition['DepthOffset'],
    );
    $form['transitions'][$key]['CubeDistance'] = array(
      '#type' => 'hidden',
      '#value' => $transition['CubeDistance'],
      '#suffix' => $transition['CubeDistance'],
    );
    $form['transitions'][$key]['action'] = array(
      '#type' => 'submit',
      '#value' => 'Delete Transition ' . $i,
      '#submit' => array('piecemaker_delete_transition'),
      '#ajax' => array(
        'callback' => 'piecemaker_add_transition_ajax',
        'wrapper' => 'transitions-wrap',
        'effect' => 'fade',
        'progress' => array(
          'type' => 'throbber',
          'message' => t('Deleting Transition'),
        ),
      ),
    );
    $i++;      
  }
  $form['transitions']['add']['weight'] = array(
    '#type' => 'select',
    '#options' => $weights,
    '#attributes' => array('class' => array('trans-weight')),
    '#default_value' => array(50),
  );
  $form['transitions']['add']['Transition'] = array(
    '#type' => 'select',
    '#options' => $trans_options,
    '#description' => t('Transition type of the Tweener class. For more info on these types see the official Tweener Documentation and go to "Transition Types“". The best results are achieved by those transition types, which begin with easeInOut.'),
  );
  $pieces = range(0,10);
  unset($pieces[0]);
  $form['transitions']['add']['Pieces'] = array(
    '#type' => 'select',
    '#options' => $pieces,
    '#description' => t('Number of pieces to which the image is sliced'),
  );
  $form['transitions']['add']['Time'] = array(
    '#type' => 'textfield',
    '#description' => t('Time for one cube to turn'),
    '#default_value' => '1.2',
  );
  $form['transitions']['add']['Delay'] = array(
    '#type' => 'textfield',
    '#description' => t('Delay between the start of one cube to the start of the next cube'),
    '#default_value' => '.1',
  );
  $form['transitions']['add']['DepthOffset'] = array(
    '#type' => 'textfield',
    '#description' => t('The offset during transition on the z-axis. Value between 100 and 1000 are recommended. But go for experiments. :)'),
    '#default_value' => '300',
  );
  $form['transitions']['add']['CubeDistance'] = array(
    '#type' => 'textfield',
    '#description' => t('The distance between the cubes during transition. Values between 5 and 50 are recommended. But go for experiments.:)'),
    '#default_value' => '30',
  );
  $form['transitions']['add']['action'] = array(
    '#type' => 'submit',
    '#value' => 'Add Transition',
    '#submit' => array('piecemaker_add_transition'),
    '#ajax' => array(
      'callback' => 'piecemaker_add_transition_ajax',
      'wrapper' => 'transitions-wrap',
      'effect' => 'fade',
      'progress' => array(
        'type' => 'throbber',
        'message' => t('Adding Transition'),
      ),
    ),
  );
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['settings']['ImageWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Width'),
    '#description' => t('Width of every Image. ex: "900"'),
    '#default_value' => $vals['ImageWidth'],
    '#field_suffix' => 'pixels',
  );
  $form['settings']['ImageHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Height'),
    '#description' => t('Height of every Image. ex: "360"'),
    '#default_value' => $vals['ImageHeight'],
    '#field_suffix' => 'pixels',
  );
  $form['settings']['LoaderColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Loader Color'),
    '#description' => t('Color of the cubes before the first image appears, also the color of the back sides of the cube, which become visible at some transition types. ex: "0x333333"'),
    '#default_value' => $vals['LoaderColor'],
  );
  $form['settings']['InnerSideColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Inner Side Color'),
    '#description' => t('Color of the inner sides of the cube when sliced. ex: "0x222222"'),
    '#default_value' => $vals['InnerSideColor'],
  );
  $form['settings']['SideShadowAlpha'] = array(
    '#type' => 'textfield',
    '#title' => t('Side Shadow Alpha'),
    '#description' => t('Sides get darker when moved away from the front. This is the degree of darkness - 0 == no change, 1 == 100% black. ex: "0.8"'),
    '#default_value' => $vals['SideShadowAlpha'],
  );
  $form['settings']['DropShadowAlpha'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Alpha'),
    '#description' => t('Alpha of the drop shadow - 0 == no shadow, 1 == opaque. ex: "0.7"'),
    '#default_value' => $vals['DropShadowAlpha'],
  );
  $form['settings']['DropShadowDistance'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Distance'),
    '#description' => t('Distance of the shadow from the bottom of the image. ex: "25"'),
    '#default_value' => $vals['DropShadowDistance'],
  );
  $form['settings']['DropShadowScale'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Scale'),
    '#description' => t('As the shadow is blurred, it appears wider that the actual image, when not resized. Thus it‘s a good idea to make it slightly smaller. - 1 would be no resizing at all.. ex: "0.95"'),
    '#default_value' => $vals['DropShadowScale'],
  );
  $form['settings']['DropShadowBlurX'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Blur-X'),
    '#description' => t('Blur of the drop shadow on the x-axis. ex: "40"'),
    '#default_value' => $vals['DropShadowBlurX'],
  );
  $form['settings']['DropShadowBlurY'] = array(
    '#type' => 'textfield',
    '#title' => t('Drop Shadow Blur-Y'),
    '#description' => t('Blur of the drop shadow on the y-axis. ex: "4"'),
    '#default_value' => $vals['DropShadowBlurY'],
  );
  $form['settings']['MenuDistanceX'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Distance-X'),
    '#description' => t('Distance between two menu items (from center to center). ex: "20"'),
    '#default_value' => $vals['MenuDistanceX'],
  );
  $form['settings']['MenuDistanceY'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Distance-Y'),
    '#description' => t('Distance of the menu from the bottom of the image. ex: "50"'),
    '#default_value' => $vals['MenuDistanceY'],
  );
  $form['settings']['MenuColor1'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Color 1'),
    '#description' => t('Color of an inactive menu item. ex: "0x999999"'),
    '#default_value' => $vals['MenuColor1'],
  );
  $form['settings']['MenuColor2'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Color 2'),
    '#description' => t('Color of an active menu item. ex: "0x333333"'),
    '#default_value' => $vals['MenuColor2'],
  );
  $form['settings']['MenuColor3'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Color 3'),
    '#description' => t('Color of the inner circle of an active menu item. Should equal the background color of the whole thing.. ex: "0xFFFFFF"'),
    '#default_value' => $vals['MenuColor3'],
  );
  $form['settings']['ControlSize'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Size'),
    '#description' => t('Size of the controls, which appear on rollover (play, stop, info, link) . ex: "100"'),
    '#default_value' => $vals['ControlSize'],
  );
  $form['settings']['ControlDistance'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Distance'),
    '#description' => t('Distance between the controls (from the borders). ex: "20"'),
    '#default_value' => $vals['ControlDistance'],
  );
  $form['settings']['ControlColor1'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Color 1'),
    '#description' => t('Background color of the controls. ex: "0x222222"'),
    '#default_value' => $vals['ControlColor1'],
  );
  $form['settings']['ControlColor2'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Color 2'),
    '#description' => t('Font color of the controls. ex: "0xFFFFFF"'),
    '#default_value' => $vals['ControlColor2'],
  );
  $form['settings']['ControlAlpha'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Alpha'),
    '#description' => t('Alpha of a control, when mouse is not over. ex: "0.8"'),
    '#default_value' => $vals['ControlAlpha'],
  );
  $form['settings']['ControlAlphaOver'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Alpha Over'),
    '#description' => t('Alpha of a control, when mouse is over. ex: "0.95"'),
    '#default_value' => $vals['ControlAlphaOver'],
  );
  $form['settings']['ControlsX'] = array(
    '#type' => 'textfield',
    '#title' => t('Controls X'),
    '#description' => t('X-position of the point, which aligns the controls (measured from [0,0] of the image). ex: "450"'),
    '#default_value' => $vals['ControlsX'],
  );
  $form['settings']['ControlsY'] = array(
    '#type' => 'textfield',
    '#title' => t('Controls Y'),
    '#description' => t('Y-position of the point, which aligns the controls (measured from [0,0] of the image). ex: "280"'),
    '#default_value' => $vals['ControlsY'],
  );
  $form['settings']['ControlsAlign'] = array(
    '#type' => 'textfield',
    '#title' => t('Controls Align'),
    '#description' => t('Type of alignment from the point [controlsX, controlsY] - can be "center", "left" or "right". ex: "center"'),
    '#default_value' => $vals['ControlsAlign'],
  );
  $form['settings']['TooltipHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Height'),
    '#description' => t('Height of the tooltip surface in the menu. ex: "31"'),
    '#default_value' => $vals['TooltipHeight'],
  );
  $form['settings']['TooltipColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Color'),
    '#description' => t('Color of the tooltip surface in the menu. ex: "0x222222"'),
    '#default_value' => $vals['TooltipColor'],
  );
  $form['settings']['TooltipTextY'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text-Y'),
    '#description' => t('Y-distance of the tooltip text field from the top of the tooltip. ex: "5"'),
    '#default_value' => $vals['TooltipTextY'],
  );
  $form['settings']['TooltipTextStyle'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text Style'),
    '#description' => t('The style of the tooltip text, specified in the CSS file. ex: "P-Italic"'),
    '#default_value' => $vals['TooltipTextStyle'],
  );
  $form['settings']['TooltipTextColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text Color'),
    '#description' => t('Color of the tooltip text. ex: "0xFFFFFF"'),
    '#default_value' => $vals['TooltipTextColor'],
  );
  $form['settings']['TooltipMarginLeft'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Margin Left'),
    '#description' => t('Margin of the text to the left end of the tooltip. ex: "5"'),
    '#default_value' => $vals['TooltipMarginLeft'],
  );
  $form['settings']['TooltipMarginRight'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Margin Right'),
    '#description' => t('Margin of the text to the right end of the tooltip. ex: "7"'),
    '#default_value' => $vals['TooltipMarginRight'],
  );
  $form['settings']['TooltipTextSharpness'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text Sharpness'),
    '#description' => t('Sharpness of the tooltip text (-400 to 400) - see Adobe Docs. ex: "50"'),
    '#default_value' => $vals['TooltipTextSharpness'],
  );
  $form['settings']['TooltipTextThickness'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip Text Thickness'),
    '#description' => t('Thickness of the tooltip text (-400 to 400) - see Adobe Docs. ex: "-100"'),
    '#default_value' => $vals['TooltipTextThickness'],
  );
  $form['settings']['InfoWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Width'),
    '#description' => t('The width of the info text field. ex: "400"'),
    '#default_value' => $vals['InfoWidth'],
  );
  $form['settings']['InfoBackground'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Background'),
    '#description' => t('The background color of the info text field. ex: "0xFFFFFF"'),
    '#default_value' => $vals['InfoBackground'],
  );
  $form['settings']['InfoBackgroundAlpha'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Background Alpha'),
    '#description' => t('The alpha of the background of the info text, the image shines through, when smaller than 1. ex: "0.95"'),
    '#default_value' => $vals['InfoBackgroundAlpha'],
  );
  $form['settings']['InfoMargin'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Margin'),
    '#description' => t('The margin of the text field in the info section to all sides. ex: "15"'),
    '#default_value' => $vals['InfoMargin'],
  );
  $form['settings']['InfoSharpness'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Sharpness'),
    '#description' => t('Sharpness of the info text (see above). ex: "0"'),
    '#default_value' => $vals['InfoSharpness'],
  );
  $form['settings']['InfoThickness'] = array(
    '#type' => 'textfield',
    '#title' => t('Info Thickness'),
    '#description' => t('Thickness of the info text (see above). ex: "0"'),
    '#default_value' => $vals['InfoThickness'],
  );
  $form['settings']['Autoplay'] = array(
    '#type' => 'textfield',
    '#title' => t('Autoplay'),
    '#description' => t('Number of seconds from one transition to another, if not stopped. Set to 0 to disable autoplay. ex: "10"'),
    '#default_value' => $vals['Autoplay'],
  );
  $form['settings']['FieldOfView'] = array(
    '#type' => 'textfield',
    '#title' => t('Field Of View'),
    '#description' => t('See the official Adobe Docs. ex: "45"'),
    '#default_value' => $vals['FieldOfView'],
  );  
  
  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Profile'),
    '#weight' => -5,
  );
  if (!empty($profile->pid)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => -4,
      '#submit' => array('piecemaker_profile_form_delete_submit'),
    );
  }
  $form['actions']['cancel'] = array(
    '#markup' => l('Cancel', 'admin/config/media/piecemaker'),
  );
  return $form;
}

/**
 * Validates piecemaker_profile_form form submissions.
 */
function piecemaker_profile_form_validate($form, &$form_state) {
  
}

/**
 * Processes piecemaker_profile_form form submissions.
 */
function piecemaker_profile_form_submit($form, &$form_state) {
  $up = array();
  $values['title'] = $form_state['values']['title'];
  $values['settings'] = $form_state['values']['settings'];
  $transitions = $form_state['values']['transitions'];
  //Get rid of the Add row since they didn't click it here.
  unset($transitions['add']);
  uasort($transitions, 'drupal_sort_weight');
  foreach ($transitions as $trans) {
    unset($trans['weight']);
    $values['transitions'][] = $trans; 
  }
  if (!empty($form_state['values']['pid'])) {
    $up = array('pid');
    $values['pid'] = $form_state['values']['pid'];
  }
  $flash = $form_state['values']['flash_settings'];
  $params = $flash['params'];
  unset($params['add'], $flash['params']);
  foreach($params as $param) {
    if (!empty($param['value'])) {
      $flash['params'][$param['key']] = $param['value'];
    }    
  }
  $values['flash_settings'] = $flash;
  $success = drupal_write_record('piecemaker_profiles', $values, $up);
  
  if (!$success) {
    form_set_error('pid', 'There was a problem saving your profile data. Please try again.');
  }
  else {
    drupal_set_message(t('Profile settings saved'));
    $form_state['redirect'] = 'admin/config/media/piecemaker';
  }
}

/**
 * Processes delete button piecemaker_profile_form form submissions.
 */
function piecemaker_profile_form_delete_submit($form, &$form_state) {
  $pid = $form_state['values']['pid'];
  $form_state['redirect'] = "admin/config/media/piecemaker/profiles/{$pid}/delete";
}

/**
 * Menu callback -- ask for confirmation of piecemaker_profile deletion
 */
function piecemaker_profile_delete_confirm($form, &$form_state, $profile) {
  $form['#profile'] = $profile;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['pid'] = array('#type' => 'value', '#value' => $profile->pid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $profile->title)),
    'admin/config/media/piecemaker',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute piecemaker_profile deletion
 */
function piecemaker_profile_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $profile = piecemaker_profile_load($form_state['values']['pid']);
    $num_deleted = db_delete('piecemaker_profiles')
      ->condition('pid', $form_state['values']['pid'])
      ->execute();
    watchdog('Piecemaker', 'Piecemaker Profile: deleted %title.', array('%title' => $profile->title));
    drupal_set_message(t('Profile %title has been deleted.', array('%title' => $profile->title)));
  }

  $form_state['redirect'] = 'admin/config/media/piecemaker';
}

/**
 * Form submit callback for Adding Flash params
 */
function piecemaker_add_params_rows($form, &$form_state) {
  // If this is a Ajax POST, add 1, otherwise add 5 more choices to the form.
  if ($form_state['values']['flash_settings']['params']['add']) {
    $n = $_GET['q'] == 'system/ajax' ? 1 : 3;
    $form_state['params_extra'] = $n;
  }
  $flash = $form_state['values']['flash_settings'];
  $params = $flash['params'];
  unset($params['add'], $flash['params']);
  foreach($params as $param) {
    if (!empty($param['value'])) {
      $flash['params'][$param['key']] = $param['value'];
    }    
  }
  $form_state['flash_settings']['params'] = $flash['params'];
  $form_state['rebuild'] = TRUE;
  drupal_set_message('Params added. You must save the profile for your changes to be made final.');
}
/**
 * Provides the different Transition types
 * 
 * @return
 *   An Array of the different transition type
 */
function _piecemaker_transitions() {
  $trans['easeNone'] = 'easeNone';    
  $trans['easeInQuad'] = 'easeInQuad';
  $trans['easeOutQuad'] = 'easeOutQuad';
  $trans['easeInOutQuad'] = 'easeInOutQuad';
  $trans['easeOutInQuad'] = 'easeOutInQuad';
  $trans['easeInCubic'] = 'easeInCubic';
  $trans['easeOutCubic'] = 'easeOutCubic';
  $trans['easeInOutCubic'] = 'easeInOutCubic';
  $trans['easeOutInCubic'] = 'easeOutInCubic';
  $trans['easeInQuart'] = 'easeInQuart';
  $trans['easeOutQuart'] = 'easeOutQuart';
  $trans['easeInOutQuart'] = 'easeInOutQuart';
  $trans['easeOutInQuart'] = 'easeOutInQuart';
  $trans['easeInQuint'] = 'easeInQuint';
  $trans['easeOutQuint'] = 'easeOutQuint';
  $trans['easeInOutQuint'] = 'easeInOutQuint';
  $trans['easeOutInQuint'] = 'easeOutInQuint';
  $trans['easeInSine'] = 'easeInSine';
  $trans['easeOutSine'] = 'easeOutSine';
  $trans['easeInOutSine'] = 'easeInOutSine';
  $trans['easeOutInSine'] = 'easeOutInSine';
  $trans['easeInCirc'] = 'easeInCirc';
  $trans['easeOutCirc'] = 'easeOutCirc';
  $trans['easeInOutCirc'] = 'easeInOutCirc';
  $trans['easeOutInCirc'] = 'easeOutInCirc';
  $trans['easeInExpo'] = 'easeInExpo';
  $trans['easeOutExpo'] = 'easeOutExpo';
  $trans['easeInOutExpo'] = 'easeInOutExpo';
  $trans['easeOutInExpo'] = 'easeOutInExpo';
  $trans['easeInElastic'] = 'easeInElastic';
  $trans['easeOutElastic'] = 'easeOutElastic';
  $trans['easeInOutElastic'] = 'easeInOutElastic';
  $trans['easeOutInElastic'] = 'easeOutInElastic';
  $trans['easeInBack'] = 'easeInBack';
  $trans['easeOutBack'] = 'easeOutBack';
  $trans['easeInOutBack'] = 'easeInOutBack';
  $trans['easeOutInBack'] = 'easeOutInBack';
  $trans['easeInBounce'] = 'easeInBounce';
  $trans['easeOutBounce'] = 'easeOutBounce';
  $trans['easeInOutBounce'] = 'easeInOutBounce';
  $trans['easeOutInBounce'] = 'easeOutInBounce';
  return $trans; 
}
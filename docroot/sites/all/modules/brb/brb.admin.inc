<?php

/**
 * @file
 * Administrative page callbacks for the BrB module.
 */


/**
* The admin settings form
*/
function brb_admin_settings($form, &$form_state) {
  $form = array();
  
  // IE conditional
  $form['brb_ie_conditional'] = array(
    '#description' => t("The versions of IE you want to block. For more information read ") . l(t('About Conditional Comments'), 'http://msdn.microsoft.com/en-us/library/ms537512%28v=vs.85%29.aspx') . '.',
    '#type' => 'textfield',
    '#title' => t('Internet Explorer conditional'),
    '#default_value' => variable_get('brb_ie_conditional', BRB_IE_CONDITIONAL_DEFAULT),
    '#size' => 60,
    '#maxlength' => 128,
  );
  
  // Overlay option
  $form['brb_overlay'] = array(
    '#description' => t('Apply an overlay layer to the viewport making the content page no accessible.'),
    '#title' => t('Apply overlay layer'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('brb_overlay', TRUE),
  );
  
  // Dialog windows title
  $form['brb_title'] = array(
    '#description' => t("The title to display on the dialog window."),
    '#type' => 'textfield',
    '#title' => t('Dialog window title'),
    '#default_value' => variable_get('brb_title', BRB_TITLE),
    '#size' => 60,
    '#maxlength' => 128,
  );
  
  // Explanation Message
  $form['brb_body'] = array(
    '#description' => t('A brief message letting the user know why his/her browser is not supported.'),
    '#title' => t('Explanation Message'),
    '#type' => 'textarea',
    '#default_value' => variable_get('brb_body', BRB_BODY_DEFAULT),
  );
  
  // Drag and drop table
  $browsers = variable_get('brb_browsers', array());
  uasort($browsers, 'brb_cmp');
  
  foreach ($browsers as $i => $browser) {
    //create a partial table row containing the data from the table
    $data = array(
      $browser->name,
    );

    //add our static "row" data into a form value
    $form['rows'][$i]['data']=array(
                                 '#type' => 'value',
                                 '#value' => $data
                               );

    //add URL
    $form['rows'][$i]['url-' . $i]=array(
      '#type' => 'textfield',
      '#size' => 60,
      '#default_value' => $browser->url,
    );

    //add exclude flag
    $form['rows'][$i]['exclude-' . $i]=array(
      '#type' => 'checkbox',
      '#default_value' => $browser->exclude,
    );

    //now create the weight form element.  
    //NOTE how we add the id into the element key
    $form['rows'][$i]['weight-' . $i]=array(
      '#type' => 'weight',
      '#default_value' => $browser->weight,
    );
  }
  
  // Let the system module add submit buttons and manage validation
  //$form = system_settings_form($form);

  //Don't forget the submit buttons
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration') );
  $form['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults') );

  return $form;
}

function theme_brb_admin_settings($variables) {
  $form = $variables['form'];
  $output = '';
  
  // element_children() gets only numeric keys
  foreach (element_children($form['rows']) as $id) {
      $this_row = array();
      
      $this_row[] = $form['rows'][$id]['data']['#value'][0];      
      $this_row[] = drupal_render($form['rows'][$id]['url-' . $id]);
      $this_row[] = drupal_render($form['rows'][$id]['exclude-' . $id]);
      //Add the weight field to the row
      $form['rows'][$id]['weight-' . $id]['#attributes']['class'][] = 'weight';
      $this_row[] = drupal_render($form['rows'][$id]['weight-' . $id]);
      
      //Add the row to the array of rows
      $table_rows[] = array('data' => $this_row, 'class' => array('draggable'));
  }
  
  
  $output = theme(
    'table', 
    array(
      'header' => array(t("Supported Browsers"), t("URL"), t("Exclude"), t("Order")), //Make sure the header count matches the column count
      'rows' => $table_rows,
      'attributes' => array('id' => 'browsers-table'),
    )
  );
  
  $output .= drupal_render_children($form);

  // Call add_tabledrag to add and setup the JS for us
  // The key thing here is the first param - the table ID
  // and the 4th param, the class of the form item which holds the weight
  drupal_add_tabledrag('browsers-table', 'order', 'sibling', 'weight');      
  return $output;
}

/**
 * Validation function
 */
function brb_admin_settings_validate($form, &$form_state) {
  if (!empty($form_state['values']['brb_firefox']) && !valid_url($form_state['values']['brb_firefox'], TRUE)) {
    form_set_error('brb_firefox', t('You must enter a valid URL.'));
  }
  
  if (!empty($form_state['values']['brb_safari']) && !valid_url($form_state['values']['brb_safari'], TRUE)) {
    form_set_error('brb_safari', t('You must enter a valid URL.'));
  }
  
  if (!empty($form_state['values']['brb_chrome']) && !valid_url($form_state['values']['brb_chrome'], TRUE)) {
    form_set_error('brb_chrome', t('You must enter a valid URL.'));
  }
  
  if (!empty($form_state['values']['brb_opera']) && !valid_url($form_state['values']['brb_opera'], TRUE)) {
    form_set_error('brb_opera', t('You must enter a valid URL.'));
  }
  
  if (!empty($form_state['values']['brb_ie']) && !valid_url($form_state['values']['brb_ie'], TRUE)) {
    form_set_error('brb_ie', t('You must enter a valid URL.'));
  }
}

/**
 * Submit function
 */
function brb_admin_settings_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Reset to defaults')) {
    brb_restore();
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    // Save IE conditional
    variable_set('brb_ie_conditional', $form_state['values']["brb_ie_conditional"]);
    
    // Save overlay
    variable_set('brb_overlay', $form_state['values']["brb_overlay"]);
    
    // Save browser list
    $browsers = variable_get('brb_browsers', array());
    foreach ($form_state['values'] as $key => $data) {
      //update url elements
      if (substr($key, 0, 3)=='url') {
        //cunningly we have the DB id of the row in the element name
        $id = str_replace('url-', '', $key);
        $browsers[$id]->url = $data;
      }
      //update exclude elements
      if (substr($key, 0, 7)=='exclude') {
        //cunningly we have the DB id of the row in the element name
        $id = str_replace('exclude-', '', $key);
        $browsers[$id]->exclude = $data;
      }
      //update weight elements
      if (substr($key, 0, 6)=='weight') {
        //cunningly we have the DB id of the row in the element name
        $id = str_replace('weight-', '', $key);
        $browsers[$id]->weight = $data;
      }
    }
    variable_set('brb_browsers', $browsers);
    
    // Save title
    variable_set('brb_title', $form_state['values']["brb_title"]);
    
    // Save body
    variable_set('brb_body', $form_state['values']["brb_body"]);
    
    drupal_set_message(t('The configuration options have been saved.'));
  }
  
  //optionally set the redirect value in form_submit ...
}

/**
 * Restore the original values.
 */
function brb_restore() {
  // Insert IE conditional
  variable_set('brb_ie_conditional', BRB_IE_CONDITIONAL_DEFAULT);
  
  // Insert overlay option
  variable_set('brb_overlay', TRUE);
  
  // Insert browsers list
  variable_set(
    'brb_browsers',
    array(
      1 => (object)array(
        'name' => 'Firefox',
        'url' => 'http://www.mozilla.com/firefox/',
        'exclude' => FALSE,
        'weight' => 1,
      ),
      2 => (object)array(
        'name' => 'Safari',
        'url' => 'http://www.apple.com/safari/download/',
        'exclude' => FALSE,
        'weight' => 2,
      ),
      3 => (object)array(
        'name' => 'Chrome',
        'url' => 'http://www.google.com/chrome/',
        'exclude' => FALSE,
        'weight' => 3,
      ),
      4 => (object)array(
        'name' => 'Opera',
        'url' => 'http://www.opera.com/download/',
        'exclude' => FALSE,
        'weight' => 4,
      ),
      5 => (object)array(
        'name' => 'IE',
        'url' => 'http://www.microsoft.com/windows/internet-explorer/default.aspx',
        'exclude' => FALSE,
        'weight' => 5,
      ),
    )
  );
  
  // Insert title
  variable_set('brb_title', BRB_TITLE);
  
  // Insert explanation message in table variables
  variable_set('brb_body', BRB_BODY_DEFAULT);
}


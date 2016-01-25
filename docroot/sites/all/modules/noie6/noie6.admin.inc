<?php

/**
 * @file
 * Administrative page callbacks for the noie6 module.
 */


/**
* The admin settings form
*/
function noie6_admin_settings($form, &$form_state) {
  $form = array();
  
  // Overlay option
  $form['noie6_overlay'] = array(
    '#description' => t('Apply an overlay layer to the viewport making the content page no accessible.'),
    '#title' => t('Apply overlay layer'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('noie6_overlay', TRUE),
  );
  
  // Explanation Message
  $form['noie6_body'] = array(
    '#description' => t('Let the users know why IE6 is not supported.'),
    '#title' => t('Explanation Message'),
    '#type' => 'textarea',
    '#default_value' => variable_get('noie6_body', 'Sorry but your browser is not supported.'),
  );
  
  // Drag and drop table
  $browsers = variable_get('noie6_browsers', array());
  uasort($browsers, 'noie6_cmp');
  
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

function theme_noie6_admin_settings($variables) {
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
      'header' => array(t("Browser"), t("URL"), t("Exclude"), t("Order")), //Make sure the header count matches the column count
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
function noie6_admin_settings_validate($form, &$form_state) {
  if (!empty($form_state['values']['noie6_firefox']) && !valid_url($form_state['values']['noie6_firefox'], TRUE)) {
    form_set_error('noie6_firefox', t('You must enter a valid URL.'));
  }
  
  if (!empty($form_state['values']['noie6_safari']) && !valid_url($form_state['values']['noie6_safari'], TRUE)) {
    form_set_error('noie6_safari', t('You must enter a valid URL.'));
  }
  
  if (!empty($form_state['values']['noie6_chrome']) && !valid_url($form_state['values']['noie6_chrome'], TRUE)) {
    form_set_error('noie6_chrome', t('You must enter a valid URL.'));
  }
  
  if (!empty($form_state['values']['noie6_opera']) && !valid_url($form_state['values']['noie6_opera'], TRUE)) {
    form_set_error('noie6_opera', t('You must enter a valid URL.'));
  }
  
  if (!empty($form_state['values']['noie6_ie']) && !valid_url($form_state['values']['noie6_ie'], TRUE)) {
    form_set_error('noie6_ie', t('You must enter a valid URL.'));
  }
}

/**
 * Submit function
 */
function noie6_admin_settings_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Reset to defaults')) {
    noie6_restore();
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    // Save overlay
    variable_set('noie6_overlay', $form_state['values']["noie6_overlay"]);
    
    // Save browser list
    $browsers = variable_get('noie6_browsers', array());
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
    variable_set('noie6_browsers', $browsers);
    
    // Save body
    variable_set('noie6_body', $form_state['values']["noie6_body"]);
    
    drupal_set_message(t('The configuration options have been saved.'));
  }
  
  //optionally set the redirect value in form_submit ...
}

/**
 * Restore the original values.
 */
function noie6_restore() {
  // Insert overlay option
  variable_set('noie6_overlay', TRUE);
  
  // Insert browsers list
  variable_set(
    'noie6_browsers',
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
        'name' => 'Update IE',
        'url' => 'http://www.microsoft.com/windows/internet-explorer/default.aspx',
        'exclude' => FALSE,
        'weight' => 5,
      ),
    )
  );
  
  // Insert explanation message in table variables
  variable_set(
    'noie6_body',
    '<h2>' . t("Internet Explorer 6 is a very old browser and it is not supported in this site.") . '</h2>' .
    '<p>' . t('The following list contains some of the reasons why this browser it is not supported:') . '</p>' .
    '<ul>' .
    '  <li>' . t('It does not support the <a href="@url">W3C standards</a>.', array('@url' => 'http://www.w3.org/standards/agents/browsers')) . '</li>' .
    '  <li>' . t('It is not compatible with <a href="@url">CSS3</a>.', array('@url' => 'http://www.css3.info/selectors-test/')) . '</li>' .
    '  <li>' . t('It has a very low score in <a href="@url">Acid 3 test</a>.', array('@url' => 'http://acid3.acidtests.org/')) . '</li>' .
    '  <li>' . t('It is insecure and slow.') . '</li>' .
    '  <li>' .
        t('There are several movements against this browser: <a href="@url_1">IE6: Do NOT want!</a>, <a href="@url_2">IE Death March</a>, <a href="@url_3">Dear IE6</a>, <a href="@url_4">Stop IE6</a>.',
          array(
            '@url_1' => 'http://ie6.forteller.net/index.php?title=Main_Page',
            '@url_2' => 'http://iedeathmarch.org/',
            '@url_3' => 'http://dearie6.com/',
            '@url_4' => 'http://stopie6.com/',
          )
        ) .
    '  </li>' .
    '  <li>' . t('Google has officially <a href="@url">stopped</a> giving it support.', array('@url' => 'http://googleenterprise.blogspot.com/2010/01/modern-browsers-for-modern-applications.html')) . '</li>' .
    '  <li>' . t('And even a <a href="@url">funeral</a> has been celebrated.', array('@url' => 'http://ie6funeral.com/')) . '</li>' .
    '</ul>' .
    '<h4>' . t('We strongly recommend that you download one of the following browsers:') . '</h4>'
  );
}


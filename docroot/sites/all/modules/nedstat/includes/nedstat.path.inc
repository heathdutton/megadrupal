<?php


/**
 * Returns an overview of all path
 * @todo: implement search functionality
 * 
 * @return string
 */
function nedstat_path_overview_page() {
  $data = nedstat_path_load_all(TRUE);
  return theme('nedstat_overview_paths', array('data' => $data));
}


/**
 * Returns a form to add/edit a path
 * 
 * @param array $form
 * @param array $form_state
 * @param array $path
 */
function nedstat_path_add_form($form, &$form_state, $path = NULL) {
  //Set option count.
  $total = 0;
  
  if (isset($form_state['path_count'])) {
    $total = $form_state['path_count'];
  }
  else {
    $total = 5;
  }
  
  $form['paths']['#tree'] = TRUE;

  //create all path elements
  for ($index = 0; $index < $total; $index++) {
    $form['paths'][$index]['url'] = array(
      '#type' => 'textfield',
      '#title' => t('Url'),
      '#default_value' => '',
      '#required' => FALSE,
    );
      
    $form['paths'][$index]['countername'] = array(
      '#type' => 'textfield',
      '#title' => t('Countername'),
      '#default_value' => '',
      '#required' => FALSE,
    );
  }
  
  $form['#validate'] = array('nedstat_path_form_validate');
  
  //add the form actions
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
}

/**
 * Validation handler for path form
 * Since we need this also for the edit form,
 * we changed the name
 * 
 * @param array $form
 * @param array $form_state
 */
function nedstat_path_form_validate($form, &$form_state) {
  //@todo: implement validation handler
  //valid path?
  //valid countername?
}


/**
 * Submit handler for path form
 * 
 * @param array $form
 * @param array $form_state
 */
function nedstat_path_add_form_submit($form, $form_state) {
  $values = $form_state['values'];
  
  foreach ((array)$values['paths'] as $path) {
    $path = (object)$path;

    if (!empty($path->url)) {
      _nedstat_path_insert($path);
      drupal_set_message(t('Inserted path !path successfully.', array('!path' => $path->url)));
    }
  }
  
  drupal_goto('admin/config/webanalytics/nedstat/path/list');
}

/**
 * Returns a form to edit a countername based on url
 * 
 * @param $form
 * @param $form_state
 * @param $path
 */
function nedstat_path_form($form, &$form_state, $path = NULL) {
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#default_value' => !empty($path) ? $path->url : '',
    '#required' => FALSE,
  );
      
  $form['countername'] = array(
    '#type' => 'textfield',
    '#title' => t('Countername'),
    '#default_value' => !empty($path) ? $path->countername : '',
    '#required' => FALSE,
  );
  
  $form['path'] = array(
    '#type' => 'value',
    '#value' => $path,
  );
  
  //add the form actions
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );  
  
  return $form;
}

/**
 * Submithandler of the edit path form
 * 
 * @param $form
 * @param $form_state
 */
function nedstat_path_form_submit($form, &$form_state) {
  $path = new StdClass();
  
  $path->id = $form_state['values']['path']->id;
  $path->url = $form_state['values']['url'];
  $path->countername = $form_state['values']['countername'];
  
  _nedstat_path_update($path);
  drupal_set_message(t('Updated path !path successfully.', array('!path' => $path->url)));
  
  drupal_goto('admin/config/webanalytics/nedstat/path/list');
}

/**
 * Returns a confirmation form to delete a path
 * 
 * @param $form
 * @param $form_state
 * @param $path
 */
function nedstat_path_delete_form($form, &$form_state, $path = NULL) {
  $form['path'] = array(
    '#type' => 'value',
    '#value' => $path,
  );
  
  $question = t('Are you sure you want to delete path "<em>!path</em>"', array('!path' => $path->url));
  $path = 'admin/config/webanalytics/nedstat/path/list';
  
  return confirm_form($form, $question, $path);
}

/**
 * Submithandler to delete a path
 * 
 * @param $form
 * @param $form_state
 * @param $path
 */
function nedstat_path_delete_form_submit($form, &$form_state, $path = NULL) {
  $path = $form_state['values']['path'];
  _nedstat_path_delete($path);
    
  drupal_set_message(t('Deleted path "!path".', array('!path' => $path->url)));
  drupal_goto('admin/config/webanalytics/nedstat/path/list');
}

/**
 * Load all nedstat path counters
 * 
 * @return array
 */
function nedstat_path_load_all($links = FALSE) {
  $result = db_query('SELECT * FROM {nedstat_counter_path}');
  $items = array();
  
  foreach ($result as $path) {
    $path = (object)$path;
    $path->storage = STORED_IN_DATABASE;
    
    //build links if requested
    if ($links && user_access('administer nedstat paths')) {
      $path->links = array(
        'edit' => l(t('Edit'), 'admin/config/webanalytics/nedstat/path/'. $path->id . '/edit'),
        'delete' => l(t('Delete'), 'admin/config/webanalytics/nedstat/path/'. $path->id . '/delete'),
      );
    }
    
    $items[] = $path;
  }
  
  //invoke hook_nedstat_paths to get
  //all paths handled by other datasources
  $data = module_invoke_all('nedstat_paths');
  foreach ((array)$data as $item) {
    $path = (object)$item;
    $path->storage = STORED_IN_CODE;
    
    //add path to resultset
    $items[] = $path;
  }

  return $items;
}

/**
 * Load path based on id
 * 
 * @param $url
 * @return string
 */
function nedstat_path_load($id) {
  $path = NULL;
  $path = db_query('SELECT * FROM {nedstat_counter_path} WHERE id = :id LIMIT 1', array(':id' => $id))->fetchAssoc();
  
  if (!empty($path)) {
    $path = (object)$path;
  }
  
  return $path;
}

/**
 * Load path based on url
 * 
 * @param $url
 * @return string
 */
function nedstat_path_load_by_url($url) {
  $path = NULL;
  $path = db_query('SELECT * FROM {nedstat_counter_path} WHERE url = :url LIMIT 1', array(':url' => $url))->fetchAssoc();
  
  if (!empty($path)) {
    $path = (object)$path;
  }
  
  return $path;
}

/**
 * Insert a path to the database
 *
 * Making this function private to avoid conflicts with hook_path_insert
 * 
 * @param array $path
 */
function _nedstat_path_insert($path) {
  db_insert('nedstat_counter_path')
    ->fields(array(
      'url' => $path->url,
      'countername' => $path->countername
    ))
    ->execute();
}

/**
 * Update a countername
 *
 * Making this function private to avoid conflicts with hook_path_insert
 * 
 * @param array $path
 */
function _nedstat_path_update($path) {
  db_update('nedstat_counter_path')
    ->fields(array(
      'url' => $path->url,
      'countername' => $path->countername
    ))
    ->condition('id', $path->id)
    ->execute();
}

/**
 * Delete a countername
 *
 * Making this function private to avoid conflicts with hook_path_insert
 * 
 * @param array $path
 */
function _nedstat_path_delete($path) {
  db_delete('nedstat_counter_path')
    ->condition('id', $path->id)
    ->execute();
}
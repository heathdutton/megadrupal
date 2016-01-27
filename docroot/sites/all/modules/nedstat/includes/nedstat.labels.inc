<?php


/**
 * Returns a overview of all labels
 *
 * @return string
 */
function nedstat_labels_overview_page() {
  $data = nedstat_label_load_all(TRUE); 
  return theme('nedstat_overview_labels', array('data' => $data));
}

/**
 * Returns a form to add/edit nedstat label
 *
 * @param $form
 * @param $form_state
 * @return void
 */
function nedstat_label_form($form, &$form_state, $label = NULL) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Label name'),
    '#description' => t('The name of the label'),
    '#default_value' => !empty($label) ? $label->name : '',
    '#required' => TRUE,
  );
  
  $form['machinename'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#description' => t('The name used in the tagging, this should be unique.'),
    '#default_value' => !empty($label) ? $label->machinename : '',
    '#required' => TRUE,
    '#disabled' => is_null($label) ? FALSE : TRUE,
  );
  
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Label description'),
    '#description' => t('The description of the label, this is for administer purpose only'),
    '#default_value' => !empty($label) ? $label->description : '',
    '#rows' => 3,
    '#required' => FALSE,
  );
  
  
  //adding options
  $form['options'] = array(
    '#tree' => TRUE,
    '#theme' => 'nedstat_label_options',
  );
  
  
  //Set option count.
  $total = 0;
  
  if (isset($form_state['label_options'])) {
    $total = $form_state['label_options'];
  }
  else {
    $total = !empty($label->options) ? count($label->options) + 2 : 5;
  }

  //create all options for this label
  for ($index = 0; $index < $total; $index++) {
    $form['options'][$index]['id'] = array(
      '#type' => 'value',
      '#value' => !empty($label->options[$index]->id) ? $label->options[$index]->id : 0,
      '#required' => FALSE,
    );
      
    $form['options'][$index]['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Option #@count', array('@count' => ($index+1))),
      '#default_value' => !empty($label->options[$index]) ? $label->options[$index]->name : '',
      '#required' => FALSE,
    );
      
    $form['options'][$index]['is_default'] = array(
      '#type' => 'checkbox',
      '#title' => t('is default'),
      '#default_value' => !empty($label->options[$index]) ? $label->options[$index]->is_default : '',
      '#required' => FALSE,
    );
      
    $form['options'][$index]['weight'] = array(
      '#type' => 'value',
      '#value' => $index,
    );
  }
  
  //add the label so we can check if it is
  //a new or existing label
  $form['label'] = array(
    '#type' => 'value',
    '#value' => $label,
  );
  
  $form['actions']['more_options'] = array(
    '#type' => 'submit',
    '#value' => t('Add options'),
    '#description' => t("If the amount of options above isn't enough, click here to add more options."),
    '#submit' => array('nedstat_label_options_more_options_submit'),
    '#ajax' => array(
      'callback' => 'nedstat_label_options_ajax',
      'wrapper' => 'nedstat-label-options-wrapper',
      'method' => 'replace',
      'effect' => 'none',
    ),
  );
  
  //add the form actions
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => is_null($label) ? t('Save') : t('Update'),
  );
  
  if (!is_null($label)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );  
  }
  
  return $form;
}

/**
 * Display a new option
 * 
 * @param $form
 * @param $form_state
 */
function nedstat_label_options_ajax($form, &$form_state) {
  return $form['options'];
}


/**
 * Submithandler to add more options
 * 
 * @param $form
 * @param $form_state
 */
function nedstat_label_options_more_options_submit($form, &$form_state) {
  $form_state['label_options'] = count($form_state['values']['options']) + 1;
  $form_state['rebuild'] = TRUE;
}


/**
 * Validation handler to validate the label form
 * 
 * @param $form
 * @param $form_state
 */
function nedstat_label_form_validate($form, &$form_state) {
  //@todo: validate label and options
}

/**
 * Submit handler to handle the submitted values of the label form
 * 
 * @param $form
 * @param $form_state
 */
function nedstat_label_form_submit($form, &$form_state) {
  //build label based on form submitted values
  $label = nedstat_build_label($form_state['values']);
  
  //check if the label is set.  Based on this
  //we can insert or update the label
  if (!empty($label) && $label->id == 0) {
    nedstat_label_insert($label);
    drupal_set_message(t('Inserted label !name successfully.', array('!name' => $label->name)));
  }
  else{
    nedstat_label_update($label);
    drupal_set_message(t('Updated label !name successfully.', array('!name' => $label->name)));
  }
}

/**
 * Returns a confirmation form to delete the label
 * 
 * @param $form_state
 * @param $label
 */
function nedstat_label_delete_form(&$form, $form_state, $label = NULL) {
  $form['label'] = array(
    '#type' => 'value',
    '#value' => $label,
  );
  
  $question = t('Are you sure you want to delete label "<em>!name</em>"', array('!name' => $label->name));
  $path = 'admin/config/webanalytics/nedstat/label/list';
  
  return confirm_form($form, $question, $path);
}

/**
 * Submit handler to delete a label
 * 
 * @param $form
 * @param $form_state
 */
function nedstat_label_delete_form_submit($form, &$form_state) {
  $label = $form_state['values']['label'];
  nedstat_label_delete($label);
  
  
  drupal_set_message(t('Deleted label "!name".', array('!name' => $label->name)));
  drupal_goto('admin/config/webanalytics/nedstat/label/list');
}

/**
 * Builds a label object based on the values given
 * 
 * @param $values
 */
function nedstat_build_label($values) {
  $label = new StdClass();
  
  //label
  $label->id = !empty($values['label']->id) ? $values['label']->id : 0;
  $label->machinename = !empty($values['machinename']) ? $values['machinename'] : '';
  $label->name = !empty($values['name']) ? $values['name'] : '';
  $label->description = !empty($values['description']) ? $values['description'] : '';
  $label->weight = !empty($values['label']->weight) ? $values['label']->weight : 0;
  $label->options = array();
  
  //options
  foreach ((array)$values['options'] as $option) {
    $label->options[] = (object)$option;
  }
  
  return $label;

}

/**
 * Load all labels stored in the database
 */
function nedstat_label_load_all($links = FALSE) {
  $items = array();
  
  //load all labels from database
  $result = db_query_range('SELECT * FROM {nedstat_labels} l ORDER BY l.weight', 0, 25);
  foreach ($result as $label) {
    //load options
    $label->storage = STORED_IN_DATABASE;
    $label->options = nedstat_label_option_load_all($label->id);

    //build links if requested
    if ($links && user_access('administer nedstat labels')) {
      $label->links = array(
        'edit' => l(t('Edit'), 'admin/config/webanalytics/nedstat/label/'. $label->id . '/edit'),
        'delete' => l(t('Delete'), 'admin/config/webanalytics/nedstat/label/'. $label->id . '/delete'),
      );
    }
    
    //add label to resultset
    $items[] = $label;
  }
  
  
  
  //invoke hook_nedstat_labels to get
  //all labels handled by other datasources
  $data = module_invoke_all('nedstat_labels');
  foreach ((array)$data as $item) {
    $label = nedstat_build_label($item);
    $label->storage = STORED_IN_CODE;
    
    //add label to resultset
    $items[] = $label;
  }
  
  return $items;
}

/**
 * Filter the label data to an array
 * of key values (label/value)
 *
 * @param $data
 */
function nedstat_filter_label_data($data) {
  $items = array();
  
  foreach ((array)$data as $label) {
    
    foreach ((array)$label->options as $option) {
      if ($option->is_default) {
        $items[] = array(
          'label' => $label->machinename,
          'value' => strtolower($option->name),
        );  
      }  
    }  
  }
  
  return $items;
}

/**
 * Load a label based on id
 * 
 * @param $label_id
 */
function nedstat_label_load($label_id) {
  $label = NULL;
  $label = db_query('SELECT * FROM {nedstat_labels} l WHERE id = :id LIMIT 1', array(':id' => $label_id))->fetchAssoc();
  
  if (!empty($label)) {
    $label = (object)$label;
    $label->options = nedstat_label_option_load_all($label->id);
  }
    
  return $label;
}

/**
 * Insert a label into the database
 * 
 * @param $label
 */
function nedstat_label_insert($label) {
  $label->id = db_insert('nedstat_labels')
  ->fields(array(
    'name' => $label->name,
    'machinename' => $label->machinename,
    'description' => $label->description,
    'weight' => $label->weight,
  ))
  ->execute();
    
  //insert options
  foreach ((array)$label->options as $key => $option) {
    if (!empty($option->name)) {
      $label->options[$key]->id = nedstat_label_option_insert($option, $label->id);
    }
  }
  
  return $label->id;
}

function nedstat_label_update($label) {
  db_update('nedstat_labels')
  ->fields(array(
    'name' => $label->name,
    'machinename' => $label->machinename,
    'description' => $label->description,
    'weight' => $label->weight,
  ))
  ->condition('id', $label->id)
  ->execute();
    
  //update options
  foreach ((array)$label->options as $key => $option) {
    if (!empty($option->name)) {
      if ($option->id == 0) {
        $label->options[$key]->id = nedstat_label_option_insert($option, $label->id);
      }
      else{
        nedstat_label_option_update($option, $label->id);
      }
    }
    else{
      if (!empty($option->label_id)) {
        nedstat_label_option_delete($option);  
      }
    }
  }
  
  return $label->id;
}

/**
 * Removes a label and all corresponding options
 * from database
 * 
 * @param $label
 */
function nedstat_label_delete($label) {
  db_delete('nedstat_labels')
    ->condition('id', $label->id)
    ->execute();
  
  //remove the label options
  db_delete('nedstat_label_options')
    ->condition('label_id', $label->id)
    ->execute();
}

/**
 * Load all options of a given label
 * 
 * @param $label_id
 */
function nedstat_label_option_load_all($label_id) {
  $items = array();
  
  $query = 'SELECT * FROM {nedstat_label_options} o WHERE o.label_id = :label_id ORDER BY o.weight';
  $result = db_query($query, array(':label_id' => $label_id));
  
  foreach ($result as $item) {
    $items[] = $item;
  }
  
  return $items;
}

/**
 * Insert a label option into the database
 * 
 * @param $option
 * @param $label_id
 */
function nedstat_label_option_insert($option, $label_id) {
  $option->id = db_insert('nedstat_label_options')
  ->fields(array(
    'label_id' => $label_id,
    'name' => $option->name,
    'is_default' => $option->is_default,
    'weight' => $option->weight,
  ))
  ->execute();
  
  return $option->id; 
}

/**
 * Update a label option
 * 
 * @param $option
 * @param $label_id
 */
function nedstat_label_option_update($option, $label_id) {
  db_update('nedstat_label_options')
    ->fields(array(
      'label_id' => $label_id,
      'name' => $option->name,
      'is_default' => $option->is_default,
      'weight' => $option->weight,
    ))
    ->condition('id', $option->id)
    ->execute();
}

/**
 * Removes a label option from database
 * 
 * @param $option
 */
function nedstat_label_option_delete($option) {
  db_delete('nedstat_label_options')
    ->condition('label_id', $option->label_id)
    ->execute();
}
<?php
/**
 * @file
 * Administrative page callbacks for the term_queue module.
 */

/**
 * Render the listing of all available term queues.
 */
function term_queue_admin_list() {
  $queues = term_queue_list();  
  
  $rows = array();
  foreach ($queues as $obj) {
    $row = array();
    $row[] = check_plain($obj->title);
    $row[] = check_plain($obj->description);
    $row[] = l(t('Edit Queue'), 'admin/structure/term_queue/'. $obj->qid . '/edit'); 
    $row[] = l(t('Edit Terms'), 'admin/structure/term_queue/'. $obj->qid); 
    $row[] = l(t('Delete'), 'admin/structure/term_queue/'. $obj->qid . '/delete');         
    
    $row = array('data' => $row);
    $rows[] = $row;
  }
  
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No queues have been created'),
        'colspan' => '3'
      )
    );
  }
  
  $header = array(
    array(
      'data' => t('Title')
    ),
    array(
      'data' =>t ('Description')
    ),
    array(
      'data' => t('Operations'),
      'colspan' => '3'
    )
  );
  
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'sticky' => FALSE,
    'empty' => 'There are no term queues to display yet'
  ));

  return $output;
}


/**
 * Submit handler for term queue.
 *
 * @see term_queue_form()
 */
function term_queue_form_submit($form, &$form_state) { 
  term_queue_save($form_state['values']);
  $form_state['redirect'] = 'admin/structure/term_queue/list';
  return;
}

/**
 * Form to for Term Queue Settings
 *
 * @ingroup forms
 */
function term_queue_settings_form($form, &$form_state) {
  $form = array();

  $form['global_settings'] = array('#type' => 'fieldset', '#title' => t('Global Settings'));

  $form['global_settings']['term_queue_use_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Autocomplete Textfield When Adding Terms?'),
    '#default_value' => variable_get('term_queue_use_autocomplete', FALSE),
    '#description' => t('Enable this when you have a large number of terms or vocabularies'),
  );

  return system_settings_form($form); 
}

/**
 * Form to edit an existing term queue
 *
 * @ingroup forms
 */
function term_queue_edit_form($edit = array()) {
  return drupal_get_form('term_queue_form', (array)$edit);
}

/**
 * Form builder for the queue delete form.
 *
 * @ingroup forms
 * @see term_queue_delete_form_submit()
 */
function term_queue_delete_form($form, $form_state, $queue) {
 
  $form['qid'] = array('#type' => 'value', '#value' => $queue->qid);
  $form['machine_name'] = array('#type' => 'value', '#value' => $queue->machine_name);

  return confirm_form($form,
    t('Are you sure you want to delete the queue %field?', array('%field' => $queue->title)), 
    'admin/structure/term_queue/list',
    NULL,
    t('Delete'), 
    t('Cancel'));
}

/**
 * Submit handler to delete a queue after confirmation, pull the trigger.
 *
 * @see term_queue_delete_form()
 */
function term_queue_delete_form_submit($form, &$form_state) {
  term_queue_delete($form_state['values']);
  $form_state['redirect'] = 'admin/structure/term_queue/list';
  return;
}

/**
 * Form function for adding terms to a queue.
 *
 * @ingroup forms
 * @see term_queue_add_term_form_submit()
 */
function term_queue_add_term_form($form, &$form_state, $queue) {

  drupal_set_title(t('@queue', array('@queue' => $queue->title)));
  
  $vocab_terms = array();
  //Get all the vocabularies system wide
  $vocabs = taxonomy_get_vocabularies();
  
  $qid = NULL;
  if (isset($queue->qid)) {
    //Get all the terms that are already in this queue so we can exclude them
    $qid = $queue->qid;
    $existing_terms = term_queue_get_terms($qid); 
  }
  
  $existing_tids = array();
  if (!empty($existing_terms)) {
    foreach($existing_terms as $term) {
      $existing_tids[] = $term->tid;
    } 
  }
  
  $options = array();
  
  // Building an array of options need not be done if we're using the auto-
  // complete widget.
  // TODO Later on it's possible that we will use this as a string of tids to
  // exclude when using the autocomplete field.
  if (!is_autocomplete()) {
    //See if we are restricted by vocabulary:
    if ($queue->vid_machine_name == 'any') {
      //We're not, so get all terms for all vocabs
      foreach ($vocabs as $vid => $vocab) {
          $vocab_terms[$vid] = taxonomy_get_tree($vid);
      }
      
      foreach ($vocab_terms as $vid => $terms) {
        $vocab_name = $vocabs[$vid]->name;
        $option[$vocab_name] = array();
        foreach ($terms as $term) {
          if (!in_array($term->tid, $existing_tids)) {
            $choice = new stdClass();
            $choice->option = array($term->tid => str_repeat('-', $term->depth) . $term->name);
            $options[$vocab_name][] = $choice;
          }
        }
      }  
    }
    else {
      //We're restricted to one particular vocabulary which we're going to refer to by machine_name
      $vocab = taxonomy_vocabulary_machine_name_load($queue->vid_machine_name);
      $terms = taxonomy_get_tree($vocab->vid);
      
      foreach ($terms as $term) {
        if (!in_array($term->tid, $existing_tids)) {
          $choice = new stdClass();
          $choice->option = array($term->tid => str_repeat('-', $term->depth) . $term->name);
          $options[] = $choice; 
        }
      }
    }
  }
  $form['qid'] = array(
    '#type' => 'value',
    '#value' => $qid,
  );
  
  $form['machine_name'] = array(
    '#type' => 'value',
    '#value' => $queue->machine_name,
  );
  // use a different form field for autocomplete
  if(is_autocomplete()) {
    // If the queue is not restricted to a vocab use a special char rather than
    // 'any' which could potentially be a machine name of a vocab.
    if ($queue->vid_machine_name == 'any') {
      $queue->vid_machine_name = '~';
    }
    $form['tid'] = array(
      '#type' => 'textfield',
      '#title' => t('Term'),
      '#autocomplete_path' => "term_queue/autocomplete/".$queue->vid_machine_name,
      '#description' => t('Begin typing the term name and choices will appear. Terms are displayed as "termname (category)"'),
    );
  }
  else {
    $form['tid'] = array(
      '#type' => 'select',
      '#title' => t('Term'),
      '#options' => $options,    
      '#description' => t('Select a term to be added to the queue.'),
      '#weight' => -15,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Term'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), "admin/structure/term_queue/{$queue->machine_name}"),
  );

  return $form;
}

/**
 * Implements a submit handler to add a term to a queue.
 *
 * @see term_queue_add_term_form()
 */
function term_queue_add_term_form_submit($form, &$form_state) {
  $machine_name = $form_state['values']['machine_name'];
  
  $qid = $form_state['values']['qid'];
  
  if (empty($qid)) {
    // The term_queue is probably only in code and needs to be saved to the
    // database in order to get a term queue
    if (module_exists('ctools')) {
      module_load_include('inc', 'ctools', 'includes/export');
      $queues = ctools_export_load_object('term_queue');
      ctools_export_crud_save('term_queue', $queues[$machine_name]);
      // Can't think of a better way to do this at the moment. We'll reload the
      // term queue from the database to get the qid
      $result = db_select('term_queue', 'tq')
        ->fields('tq', array('qid'))
        ->condition('machine_name', $machine_name)
        ->execute()
        ->fetchAssoc();
      $qid = $result['qid'];
    }
  }
  
  // handle the tid value if using autocomplete
  if(is_autocomplete()) {
    // value comes in as 'Charterhouse Group (Company) [tid:5193]', parse the tid.
    $value = preg_match("/tid:\d+/", $form_state['values']['tid'], $matches);
    $tid_array = explode(":", $matches[0]);
    $tid = $tid_array[1];
  }
  else {
    $tid = $form_state['values']['tid'];
  }

  $term = term_queue_add_term($qid, $tid);
  $form_state['redirect'] = "admin/structure/term_queue/$machine_name/add";
  
  if ($term) {
    $term_queue = term_queue_load($machine_name);
    drupal_set_message(t('%term has been saved to %queue.', array('%term' => $term->name, '%queue' => $term_queue->title)));
  }
  return;
}



/**
 * Form builder for the queue terms overview.
 *
 * Display all the terms in a queue, with option to delete
 * each one. The form is made drag and drop by the theme function.
 *
 * @ingroup forms
 * @see term_queue_edit_terms_form_submit()
 * @see theme_term_queue_edit_terms_form()
 */
function term_queue_terms_form ($form, &$form_state, $queue) {
  drupal_set_title(t('@queue', array('@queue' => $queue->title)));

  $form = array(
    '#queue' => (array)$queue,
    '#tree' => TRUE,
  );
  
  if (isset($queue->qid)) {
    $qterms = term_queue_get_terms($queue->qid);
    
    foreach ($qterms as $qterm){
      $form[$qterm->tid]['#qterm'] = (array)$qterm;
      $form[$qterm->tid]['name'] = array(
          '#value' => $qterm->name,
      );
      $form[$qterm->tid]['weight'] = array(
          '#type' => 'weight',
          '#delta' => '20',
          '#default_value' => $qterm->weight,
      );
      $form[$qterm->tid]['delete'] = array(
          '#value' => l(t('Delete'), "admin/structure/term_queue/{$queue->machine_name}/{$qterm->tid}/delete"),
      );
    }
  
    if (count($qterms) > 0) {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save')
      );
    }
  
  }
  
  $form['#empty_text'] = t('No terms have been added.');

  return $form;
}




/**
 * Submit handler to save the reordering or queue terms.
 *
 * @see taxonomy_overview_terms()
 */
//function term_queue_terms_weight_form_submit($form, &$form_state) {
function term_queue_terms_form_submit($form, &$form_state) {

  $queue = $form['#queue'];
  foreach ($form_state['values'] as  $tid => $qterm) {
    if (is_numeric($tid) && $form[$tid]['#qterm']['weight'] != $qterm['weight']) {
      term_queue_update_term($queue['qid'], $tid, $qterm['weight']);
    }
  }
  
  drupal_set_message(t('Term queue saved'));
  
  return;
}


/**
 * Theme the terms overview as a sortable list of terms.
 * @ingroup themeable
 * @see term_queue_term_form()
 */
function theme_term_queue_terms_form($form) {
    
  //For some reason the '$form' array seems to contain a single element that contains the whole form array.
  $form = $form['form'];
 
  $rows = array();
  $element_children = element_children($form);
  foreach ($element_children as $key) {
    if (isset($form[$key]['#qterm'])) {
      $element = $form[$key];
      
      $row = array();
      $row[] = $element['name']['#value'];
      $element['weight']['#attributes']['class'] = array('term-queue-weight');
      $row[] = drupal_render($element['weight']);
      $row[] = $element['delete']['#value'];
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
    
    else {
	//Stick all the element's keys that we're not rendering here (submit button, hidden fields etc) in an array so that we can render
	//the rest of them later.
	$remaining_elements[] = $key;
    }
  }
  
  if (empty($rows)) {
    $rows[] = array(array('data' => $form['#empty_text'], 'colspan' => '3'));
  }
  
  $header = array(t('Name'), t('Weight'), t('Operations'));
  
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'sticky' => FALSE,
    'empty' => $form['#empty_text'],
    'attributes' => array('id' => 'term-queue')
  ));
  
  drupal_add_tabledrag('term-queue', 'order', 'sibling', 'term-queue-weight');

  $output .= drupal_render_children($form, $remaining_elements);

  return $output;
}


/**
 * Form builder for the term delete form.
 *
 * @ingroup forms
 * @see term_queue_delete_term_form_submit()
 */
function term_queue_delete_term_form($form, &$form_state, $queue, $tid) {
  $form['qid'] = array('#type' => 'value', '#value' => $queue->qid);
  $form['machine_name'] = array('#type' => 'value', '#value' => $queue->machine_name);
  $form['tid'] = array('#type' => 'value', '#value' => $tid);
  $term = taxonomy_term_load($tid);

  return confirm_form($form,
    t('Are you sure you want to remove %term from the queue %queue?', array('%term' => $term->name, '%queue' => $queue->title)), 
    "admin/structure/term_queue/$queue->machine_name",
    NULL,
    t('Delete'), 
    t('Cancel'));
}

/**
 * Submit handler to delete a term after confirmation, pull the trigger.
 *
 * @see term_queue_delete_term_form()
 */
function term_queue_delete_term_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  
  term_queue_delete_term($values['qid'], $values['tid']);
  $form_state['redirect'] = 'admin/structure/term_queue/' . $values['machine_name'];
  return;
}


/**
 * Callback for the term name autocomplete.
 *
 * @param $vid_machine_name
 *   Is passed in the URL set by #autocomplete_path but strangely does not have
 *   to be specified as a variable in hook_menu (I don't think).
 * @param $string
 *   Passed automatically from the autocomplete widget.
 */
function term_queue_autocomplete_term($vid_machine_name, $string = '') {
  // TODO: This should not return matches that are already in the node queue. It
  // would be most efficient to pass the array of items to exclude that we
  // build in term_queue_add_term_form
  $matches = array();
  if($string) {
    $query = db_select('taxonomy_term_data', 'td');
    $query->fields('td', array('tid', 'name'));
    $query->fields('tv', array('name'));
    $query->join('taxonomy_vocabulary', 'tv', 'tv.vid = td.vid');
    $query->condition('td.name', $string.'%', 'LIKE');
    
    if ($vid_machine_name != '~') {
      $query->condition('tv.machine_name', $vid_machine_name); 
    }
    $query->range(0, 20);
    $result = $query->execute();

    foreach ($result as $term) {
      $key = "$term->name ($term->tv_name) [tid:$term->tid]";
      $matches[$key] = "$term->name (<b>$term->tv_name</b>)";
    }
  }
  drupal_json_output($matches); 
}


/**
 * helper to check if autocomplete is enabled.
 */
function is_autocomplete() {
  return variable_get('term_queue_use_autocomplete', FALSE);
}

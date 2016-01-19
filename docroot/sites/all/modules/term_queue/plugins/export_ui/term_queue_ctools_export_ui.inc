<?php

//We're going to be calling functions from the main .profile file
require_once(drupal_get_path('module','term_queue').'/term_queue.module');

/**
* Define this Export UI plugin.
*/
$plugin = array(
  'schema' => 'term_queue',  // As defined in hook_schema().
  'access' => 'administer term queue',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'term_queue',
    'menu title' => 'Term Queue',
    'menu description' => 'Configure lists of taxonomy terms to be used on your site.',
  ),

  // Define user interface texts.
  'title singular' => t('queue'),
  'title plural' => t('queues'),
  'title singular proper' => t('Term Queue'),
  'title plural proper' => t('Term Queues'),

  //Ed Crompton. Handler directive.
  'handler' => array(
    'class' => 'term_queue_export_ui',
    'parent' => 'ctools_export_ui',
  ),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'term_queue_ctools_export_ui_form',
    //'settings' => 'term_queue_form',
    
    // 'submit' and 'validate' are also valid callbacks.
  ),
);


/**
* Define the preset add/edit form.
*/
function term_queue_ctools_export_ui_form(&$form, &$form_state) {
  
  $edit = $form_state['item'];

  if (isset($edit->qid)) {
    $prefix_to_form['qid'] = array(
     '#type' => 'value',
     '#value' => $edit->qid
    );
  }

  $prefix_to_form['title'] = array('#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($edit->title) ? $edit->title : '',
    '#maxlength' => 255,
    '#description' => t('The name for this term queue, e.g., <em>"Top 10"</em>.'),
    '#required' => TRUE,
  );
  
  //Preserve the correct order of the form because the title field that we're adding needs to go before the machine_name that references it as a source.
  $form = array_merge($prefix_to_form, $form);

  //There's already a machine name field on the form. Change it so that it's hidden away nicely.
  $form['info']['machine_name']['#type'] = 'machine_name';
  $form['info']['machine_name']['#machine_name'] = array(
        'exists' => 'term_queue_name_exists',
	'source' => array('title'),
	'label' => t('Machine name'),
	'replace_pattern' => '[^a-z0-9_]+', //Seems that this has to be the same as defined in ctools/plugins/export_ui/ctools_export_ui.class.php in function ctools_export_ui_edit_name_validate()
	'replace' => '_',
        );
  
  $form['description'] = array('#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => !empty($edit->description) ? $edit->description : '',
    '#description' => t('Description of the term queue; can be used by modules.'),
  );
  
  $form['vid_machine_name'] = array('#type' => 'select',
    '#title' => t('Limit to vocabulary'),
    '#options' => term_queue_vocabulary_options(),
    '#default_value' => !empty($edit->vid_machine_name) ? $edit->vid_machine_name : 0,
    '#description' => t('Select a particular vocabulary to limit this term queue to.'),
  );
}
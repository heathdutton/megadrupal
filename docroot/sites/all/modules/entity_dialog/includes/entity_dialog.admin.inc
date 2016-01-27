<?php
/**
 * @file
 * Settings form.



/**
 * Module settings page.
 */
function entity_dialog_admin_settings($form, &$form_state) {
  $path = drupal_get_path('module', 'entity_dialog');
  drupal_add_css($path . '/entity_dialog_admin.css');
  
  $entities = entity_get_info();

 // $entities['node']['bundles']['article']['label'] = 'Article';
 // $entities['node']['bundles']['article']['crud'] => must exist for entity dialog to work
  $form['notcompat'] = array(
    '#type' => 'fieldset',
    '#title' => 'Not Compatible',
    '#description' => t('Entities which are not compatible with the Entity Dialog module:'),
    '#weight' => 10, // put it after the others.
  );
  $form['compat'] = array(
    '#markup' => '<h2>' . t('Entity Dialog Configuration') . '</h2> <p>' . 
    t('For each entity type, choose the operations that you would like to use Entity Dialog.') . '</p><p>' . t('For each operation that you enable, Entity Dialog will find links on your site which match the given path and load the linked page in a dialog via AJAX.') . '</p>',
  );

  $active_settings = variable_get('entity_dialog_active', array());
  $entity_classes = variable_get('entity_dialog_class', array());

  // Triggering element.
  $trig = isset($form_state['triggering_element']) ? $form_state['triggering_element'] : FALSE;
  $triggered_by = isset($trig['#name'])? $trig['#name']: FALSE;

  foreach ($entities as $entity) {
    if (isset($entity['bundles'])) {
      foreach ($entity['bundles'] as $bundle_name => $bundle) {
        if (isset($bundle['crud'])) {
          $reload_page_key = $bundle_name . '_reload_page'; 
          
          // Set up a fieldset for each bundle.
          $form[$bundle_name] = array(
            '#type' => 'fieldset',
            '#title' => $bundle['label'],
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            
            // Wrap the CRUD in a fieldset.
            'crud' => array(
              '#type' => 'fieldset',
              '#title' => 'CRUD',
            ),            

            // Add a checkbox to allow the reloading of the page after save.
            // This is a bit of a stop gap for people that don't want to be bothered 
            // implementing the 'entity_dialog_close' hook to update the page via ajax. 
            $reload_page_key => array(
              '#type' => 'checkbox',
              '#title' => 'Reload on save',
              '#default_value' => in_array($reload_page_key, $active_settings),
              '#description' => 'You can optionally have the page reload after saving an entity form. In order to update your page via ajax after save you can leave this option unchecked and implement \'hook_entity_dialog_close\' in a custom module.',
            ),
          );
          
          // Set up a checkbox for each CRUD op.        
          foreach ($bundle['crud'] as $action_key => $action) {  
            $ekey = $bundle_name . "_" . entity_dialog_convert_op_strings($action_key);
            $wrapper_class = drupal_clean_css_identifier($ekey);
            
            // If the checkbox has been ticked and the form is being rebuilt after an ajax callback
            // or the form is loading from the DB.
            $is_ticked = ( $ekey == $triggered_by) ? $trig['#value'] : in_array($ekey, $active_settings); 
           
            $form[$bundle_name]['crud'][$action_key][$ekey] = array(
              '#type' => 'checkbox',
              '#title' => $action_key,
              '#description' => 'Path: ' . $action['path'],
              '#default_value' => $is_ticked,
              '#ed_bundle' => $bundle_name,
              '#ed_action' => $action_key,
              '#ajax' => array(
                'wrapper' => $wrapper_class,
                'method' => 'replace',
                'event' => 'change',
                'callback' => 'entity_dialog_admin_form_ajax_callback',
              ),
              '#prefix' => '<div id="' . $wrapper_class . '">',  // Container for ajax callback.
              '#suffix' => (!$is_ticked) ? '</div>' : '',  // We only want to close it here if the checkbox isn't checked,
            );                                            // otherwise we close it after the class field to keep the structure correct. 
            
            if ($is_ticked) {
              // We have a class value set from a previous submit, show show the field.
              $default_value = isset($entity_classes[$ekey . '_class']) ? $entity_classes[$ekey . '_class'] : '';
              $form[$bundle_name]['crud'][$action_key][$ekey . '_class'] = array(
                '#type' => 'select',
                '#title' => 'Style',
                '#options' => array(
                  'ed-default' => 'default',
                  'ed-small' => 'Small',
                ),
                '#default_value' => $default_value,
                '#suffix' => '</div>',
              ); 
            }
          }           
        }
        else {
          // Entities not compatable with Entity Dialog
          $form['notcompat']["$bundle[label]"] = array(
            '#markup' => '<li>' . $bundle['label'] . '</li>',
          );
       }
     }
   }  
 }
 
 // Add a sumbit button to save the page.
 $form['submit'] = array(
   '#type' => 'submit',
   '#value' => t('Submit'),
   '#weight' => 100,
 );

  return $form;
}

/**
 * Ajax callback function for Entity Dialog admin config form.
**/
function entity_dialog_admin_form_ajax_callback($form, &$form_state) {
  if (isset($form_state['triggering_element'])) {
    // The element of the form that we want to return is determined by the bundle and 
    // action that we passed through on the triggering element the first time we built the form.
    $trigger = $form_state['triggering_element'];
    $bundle = $trigger['#ed_bundle'];
    $action = $trigger['#ed_action'];
    
    return $form[$bundle]['crud'][$action];
  }
}

/**
 * Implementation of hook_form_submit().
 * Modified system_settings_save().
 **/
function entity_dialog_admin_settings_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  
  $classes = array();
  $settings = array();
  foreach ($form_state['values'] as $key => $value) {
    if ( $value == 1 ) {
      $settings[] = $key;
    }
    elseif (!empty($value)) {
       $classes[$key] = $value; 
    }
  }
  variable_set('entity_dialog_active', $settings);
  variable_set('entity_dialog_class', $classes);

  drupal_set_message(t('The configuration options have been saved.'));
}
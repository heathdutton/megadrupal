<?php

/*
 * Form builder for the module site configuration page
 *
 * @param $form
 *   The system settings form to display
 *
 * @see system_settings_form()
 */
function civi_relate_admin($form, &$form_state) {
  $form = array();
  /* STATIC: $mode == 0
   * DYNAMIC: $mode == 1
   */
  $mode = variable_get('civi_relate_mode', 0);
  /*Note: #type select generates a drop down
   */
  $form['civi_relate_profile'] = array(
    '#type' => 'select',
    '#options' => civi_relate_profiles_get(),
    '#title' => t('Select CiviCRM Profile'),
    '#description' => t('This Module establishes a relationship between the acting user and contact created by that user through a CiviCRM Profile. Choose the CiviCRM Profile you wish to use.'),
    '#default_value' => variable_get('civi_relate_profile', 'NONE'),
  );
  $form['civi_relate_ab_perms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Permissioned Relationship'),
    '#description' => t('Acting user may edit the contact created via this profile'),
    '#default_value' => variable_get('civi_relate_ab_perms', 1),
  );
  $form['civi_relate_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Select Mode'),
    '#description' => t('CiviRelate may be set to allow dynamic selection of the relationship when the Profile is submitted. The default or static mode creates a single relationship configured below. To configure Dynamic mode, switch the radio button above and complete the appropriate section below.'),
    '#default_value' => variable_get('civi_relate_mode', 0),
    '#options' => array(t('Static'), t('Dynamic')),
  );
  $form['civi_relate_relationship'] = array(
    '#type' => 'select',
    '#options' => civi_relate_relationships_get('select'),
    '#title' => t('Static'),
    '#description' => t('Select a single CiviCRM Relationship to establish'),
    '#default_value' => variable_get('civi_relate_relationship', 'NONE'),
    '#states' => array (
      'enabled' => array(
      ':input[name="civi_relate_mode"]' => array('value' => '0'),
      ),
      'visible' => array(
      ':input[name="civi_relate_mode"]' => array('value' => '0'),
      ),
      'disabled' => array(
      ':input[name="civi_relate_mode"]' => array('value' => '1'),
      ),
      'invisible' => array(
      ':input[name="civi_relate_mode"]' => array('value' => '1'),
      ),
    ),
  );
  /* checkboxes input are stored in an array.  If checked input[key]='key' else input[key]=0
   */
  $form['civi_relate_dynamic_relationship'] = array(
    '#type' => 'checkboxes',
    '#default_value' => variable_get('civi_relate_dynamic_relationship',array()),
    '#options' => civi_relate_relationships_get(),
    '#title' => t('Dynamic'),
    '#description' => t('Select the CiviCRM Relationships that users may create on profile submission'),
    '#element_validate' => array('civi_relate_dynamic_relationship_validate'),
    '#states' => array (
      'enabled' => array(
      ':input[name="civi_relate_mode"]' => array('value' => '1'),
      ),
      'visible' => array(
      ':input[name="civi_relate_mode"]' => array('value' => '1'),
      ),
      'disabled' => array(
      ':input[name="civi_relate_mode"]' => array('value' => '0'),
      ),
      'invisible' => array(
      ':input[name="civi_relate_mode"]' => array('value' => '0'),
      ),
    ),
  );
  // Submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

function civi_relate_admin_submit ($form, &$form_state) {
  $array_to_build_select = array();
  $all_relationships = civi_relate_relationships_get();
  foreach ($form_state['values']['civi_relate_dynamic_relationship'] as $id => $relationship) {
    /* When the checkbox is unchecked $relationship will be 0 here - skip unchecked elements.
     */
    if ($relationship) {
      $array_to_build_select[$id] = $all_relationships[$id];
    }
  }
  variable_set('civi_relate_dynamic_relationship_select',$array_to_build_select);
  variable_set('civi_relate_dynamic_relationship',$form_state['values']['civi_relate_dynamic_relationship']);
  variable_set('civi_relate_profile',$form_state['values']['civi_relate_profile']);
  variable_set('civi_relate_ab_perms',$form_state['values']['civi_relate_ab_perms']);
  variable_set('civi_relate_mode',$form_state['values']['civi_relate_mode']);
  variable_set('civi_relate_relationship',$form_state['values']['civi_relate_relationship']);
  drupal_set_message('CiviRelate settings saved');
}

function civi_relate_dynamic_relationship_validate (&$element, &$form_state) {
  /* If we are in DYNAMIC mode, there should be at least two boxes checked.
   */
  $boxes_checked = 0;
  if ($form_state['values']['civi_relate_mode']) {
    foreach ($form_state['values']['civi_relate_dynamic_relationship'] as $id => $relationship) {
      /* When the checkbox is unchecked $relationship will be 0 here - skip unchecked elements.
       */
      if ($relationship) {
        ++$boxes_checked;
      }
    }
    if ($boxes_checked < 2) {
      form_set_error($element['#name'], t('At least two relationships must be selected when in Dynamic mode'));
    }
  }
}
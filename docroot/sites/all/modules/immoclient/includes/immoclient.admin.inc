<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function immoclient_admin_form($form, &$form_state){
  $form['obj_ref_man'] = array(
    '#title' => t('Show form object number references'),
    '#type' => 'select',
    '#options' => array('TRUE' => t('Yes'), 'FALSE' => t('No')),
    '#default_value' => variable_get('obj_ref_man',''),
    '#required' => FALSE,
    '#description' => t('To create the object references automatically, '
            . 'select "no". They have to be unique, so choose it!'),
    '#attached' => array(
      'css' => array(
            'seven' => drupal_get_path('module', 'immoclient') . '/css/immoclient_seven.css',
          ),
      ),
  );
  $form['prefix_obj_ref_ext'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix for the external object reference number'),
    '#default_value' => variable_get('prefix_obj_ref_ext', ''),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Your individual prefix, seen in external portals and at your exposé from your clients.'),
    '#required' => FALSE,
  );
  $form['prefix_obj_ref_int'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix for the internal object reference number'),
    '#default_value' => variable_get('prefix_obj_ref_int', ''),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Your individual prefix, can and should be the same as '
            . 'the external one, but hasn\'t to be.'),
    '#required' => FALSE,
  );
  return system_settings_form($form);
}
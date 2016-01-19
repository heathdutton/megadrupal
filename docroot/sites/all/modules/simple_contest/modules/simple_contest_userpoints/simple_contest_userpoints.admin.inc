<?php

function _simple_contest_userpoints_form_simple_contest_content_type_settings_form_alter( &$form, &$form_state ) {
  
  if ( !$form['is_sc']['#default_value'] ) {
    return;
  }
  
  $type = $form['type']['#value'];

  $settings = $form['#contest_settings'];

  $form['sc']['sc_userpoints'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('!Points for contest winners', array_merge(userpoints_translation(), array('!type' => $type))),
    '#group'       => 'additional_settings',
  );
  
  $form['sc']['sc_userpoints']['userpoints'] = array(
    '#type'   => 'markup',
    '#prefix' => '<div id="simple_contest_userpoints_points_wrapper">',
    '#suffix' => '</div>',
    '#tree'   => TRUE,
  );
  
  $max_winners = isset($form_state['values']['max_winners'])
    ?  $form_state['values']['max_winners']
    :  $settings['max_winners'];
    
  $points = isset($form_state['values']['userpoints'])
    ?  $form_state['values']['userpoints']
    :  $settings['userpoints'];
    
  $max_winners = intval($max_winners);
  $limit = $max_winners;
  if ($limit == 0 || $limit>10) {
    $limit=10;
  }
  
  for ( $i=1; $i <= $limit; $i++ ) {
    $form['sc']['sc_userpoints']['userpoints']['rank_' . $i] = array(
      '#type'          => 'textfield',
      '#title'         => t('!Points for Rank #!rank', array_merge(userpoints_translation(), array('!rank' => $i))),
      '#default_value' => isset($points['rank_' . $i])?$points['rank_' . $i]:0,
      '#size'          => 5,
      '#maxlength'     => 5,
    );
  }
  
  if ($max_winners<= 0 || $max_winners > 10 ) {
    
    $form['sc']['sc_userpoints']['userpoints']['rank_0'] = array(
      '#type'          => 'textfield',
      '#title'         => t('!Points for other winners', userpoints_translation()),
      '#default_value' => isset($points['rank_0'])?$points['rank_0']:0,
      '#size'          => 5,
      '#maxlength'     => 5,
    );
  }
  
  $form['sc']['sc_userpoints']['userpoints_tid'] = array(
    '#type'          => 'select',
    '#title'         => t('Default !points category', userpoints_translation()),
    '#description'   => t('Choose the category of !points.', userpoints_translation()),
    '#options'       => userpoints_get_categories(),
    '#default_value' => isset($form_state['values']['userpoints_tid'])
      ?  $form_state['values']['userpoints_tid']
      :  $settings['userpoints_tid'],
  );
  
  $form['#validate'][] = 'simple_contest_userpoints_form_content_type_settings_form_validate';
  
}

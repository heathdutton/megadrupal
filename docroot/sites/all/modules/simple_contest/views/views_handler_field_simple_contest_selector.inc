<?php

/**
* @file
* Views field handler. Contains all relevant simple contest selectors.
* Implements the Views Form API.
*/

class views_handler_field_simple_contest_selector extends views_handler_field {
  function query() {
    // do nothing -- to override the parent query.
  }

  // $this->options['']
  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Returns the form which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }
    
    $form[$this->options['id']] = array(
      '#tree' => TRUE,
    );
    
    foreach ($this->view->result as $row_id => $row) {
      $entity_id = $this->get_value($row);
      
      $form[$this->options['id']][$row_id] = array(
        '#type' => 'checkbox',
        '#return_value' => $entity_id,
      );
    }
    
    $form['#validate'][] = 'simple_contest_selector_views_form_validate';
    $form['#submit'][] = 'simple_contest_selector_views_form_submit';
    $form['#pre_render'][] = 'simple_contest_selector_views_form_pre_render';
    
  }
  
}

function simple_contest_selector_views_form_pre_render(&$form) {  
  
  if (arg(0) != 'node' || !is_numeric(arg(1))) {
    $form['actions']['submit']['#access'] = FALSE;
  }
  
  $form['actions']['submit']['#value'] = t('Confirm and finalize results');
  return $form;
}

function simple_contest_selector_views_form_validate(&$form, &$form_state) {
  
  $sc = _simple_contest_selector_form_get_field($form_state['build_info']['args'][0]);
  
  if (!$sc) {
    return;
  }
  
  $selected = array();
  
  foreach($form_state['values']['simple_contest_selector'] as $id => $sel ) {
    if ($sel) {
      $selected[] = $id;
    }
  }
  
  if (empty($selected)) {
    form_set_error('simple_contest_selector', t('Please select atleast one winner.'));
  }
  
}

/*
 * simple_contest_selector_form_submit
 */
function simple_contest_selector_views_form_submit(&$form, &$form_state) {
  
  $selected = array();

  foreach( $form_state['values']['simple_contest_selector'] as $id => $sel ) {
    if ( $sel === 'on' ) {
      $selected[] = $id;
    }
  }

  if ( empty($selected) ) {
    drupal_set_message(t('Please select atleast one winner.'));
    return;
  }
  
  $view = $form_state['build_info']['args'][0];
  $contest = node_load($view->args[0]);
  
  $settings = simple_contest_get_settings($contest->type);
  
  $results = array();
  $field_alias = '';


  foreach($view->display_handler->handlers['field'] as $fid => $field) {
    if( $field->field_alias == $settings['view_field'] ) {
      $field_alias = $field->field_alias;
      break;
    }
  }
  
  if (empty($field_alias)) {
    watchdog('simple_contest', 'Cannot find field %field in View %view.' , array(
      '%field' => $settings['view_field'],
      '%view' => $settings['view'],
      ),  WATCHDOG_WARNING);
      drupal_set_message(t('Cannot find field %field in View %view.' , array(
        '%field' => $settings['view_field'],
        '%view' => $settings['view'],
      )));
    return;
  }
  
  foreach ( $view->result as $num => $result ) {
    if ( in_array($num, $selected) ) {
      $results[$result->$field_alias] = $result->$field_alias;
    }
  }
  
  if ( !empty($results) ) {
    
    // save the contest results
    simple_contest_save_result($contest, $results);
    
    drupal_set_message(t('Contest results has been finalized.'));
  } else {
    drupal_set_message(t('No winners has been selected.'));
  }
  
}

function _simple_contest_selector_form_get_field($view) {
  foreach ($view->field as $field_name => $field) {
    if ($field instanceof views_handler_field_simple_contest_selector) {
      $field->view = $view;
      return $field;
    }
  }
  return FALSE;
}


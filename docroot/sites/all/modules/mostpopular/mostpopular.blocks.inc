<?php
/*
 * Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
 * Copyright © 2009-2012 New Signature
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * You can contact New Signature by electronic mail at labs@newsignature.com -or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 */

/**
 * @file
 * Provides an admin GUI for configuring most popular blocks.
 */ 
function mostpopular_blocks_admin_form($form, &$form_state) {
  if (!empty($form_state['confirm_delete'])) {
    $bid = $form_state['confirm_delete'];
    
    return confirm_form($form,
      t('Are you sure you want to delete block %id?', array('%id' => $bid)),
      'admin/config/mostpopular/blocks',
      t('All services and intervals attached to this block will also be deleted.  Are you sure you want to do this?'),
      t('Yes, delete this block'),
      t('No, nevermind')
    );
  }
  
  $form['blocks'] = array(
    '#tree' => TRUE,
    '#theme' => 'mostpopular_admin_blocks_table',
    '#weight' => 0,
  );
  
  if (!isset($form_state['blocks'])) {
    $form_state['blocks'] = mostpopular_blocks();
  }
  
  foreach ($form_state['blocks'] as $bid => $block) {
    $form['blocks'][$bid] = array(
      'block' => array(
        '#type' => 'value',
        '#value' => $block,
      ),
      'id' => array(
        '#type' => 'item',
        '#title' => t('ID'),
        '#markup' => !empty($block->bid) ? check_plain($block->bid) : t('New Block'),
      ),
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Machine Name'),
        '#default_value' => check_plain($block->name),
        '#required' => TRUE,
        '#size' => 5,
      ),
      'title' => array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#default_value' => check_plain($block->title),
        '#required' => TRUE,
      ),
    );
    if (empty($block->remote_bid)) {
      $form['blocks'][$bid]['remote'] = array(
        '#type' => 'markup',
        '#markup' => t('Local'), 
      );
      $form['blocks'][$bid]['count'] = array(
        '#type' => 'textfield',
        '#title' => t('Max Results'),
        '#default_value' => check_plain($block->count),
        '#size' => 3,
        '#required' => TRUE,
      );
    }
    else {
      $form['blocks'][$bid]['remote'] = array(
        '#type' => 'markup',
        '#markup' => t('Remote'),
      );
      
      if (!empty($block->data['remote_database']) && !empty($block->remote_bid)) {
        db_set_active($block->data['remote_database']);
        
        $count = db_select('mostpopular_block', 'b')
          ->fields('b', array('count'))
          ->condition('bid', $block->remote_bid)
          ->execute()
          ->fetchColumn();
        
        db_set_active('default');
      }
      $form['blocks'][$bid]['count'] = array(
        '#type' => 'markup',
        '#markup' => isset($count) ? check_plain($count) : t('Unknown'),
      );
    }
    
    if (!empty($block->bid)) {
      $form['blocks'][$bid]['configure'] = array(
        '#type' => 'link',
        '#title' => t('Configure'),
        '#href' => "admin/structure/block/manage/mostpopular/$block->bid/manage",
        '#options' => array(
          'query' => drupal_get_destination(),
        ),
      );
      $form['blocks'][$bid]['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#submit' => array( 'mostpopular_blocks_admin_form_delete_block' ),
        '#limit_validation_errors' => array(),
        '#bid' => $block->bid,
        '#prefix' => '&nbsp;',
      );
    }
  }
  
  $form['add_local_block'] = array(
    '#type' => 'submit',
    '#value' => t('Create a new block'),
    '#submit' => array( 'mostpopular_blocks_admin_form_add_local_block' ),
    '#weight' => 20,
    '#limit_validation_errors' => array(),
  );
  $form['add_remote_block'] = array(
    '#type' => 'submit',
    '#value' => t('Create a remote block'),
    '#submit' => array( 'mostpopular_blocks_admin_form_add_remote_block' ),
    '#weight' => 20,
    '#limit_validation_errors' => array(),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array( 'mostpopular_blocks_admin_form_submit' ),
    '#weight' => 10,
  );
  
  return $form;
}

function mostpopular_blocks_admin_form_add_local_block($form, &$form_state) {
  $form_state['blocks'][] = (object)array(
    'name' => 'block_' . (count($form_state['blocks']) + 1),
    'title' => t('Most Popular'),
    'count' => 5,
    'data' => array(),
  );
  $form_state['rebuild'] = TRUE;
}

function mostpopular_blocks_admin_form_add_remote_block($form, &$form_state) {
  $form_state['blocks'][] = (object)array(
    'name' => 'remote_' . (count($form_state['blocks']) + 1),
    'title' => t('Most Popular'),
    'remote_bid' => 1,
    'count' => 5,
    'data' => array(),
  );
  $form_state['rebuild'] = TRUE;
}

function mostpopular_blocks_admin_form_delete_block($form, &$form_state) {
  $form_state['confirm_delete'] = $form_state['triggering_element']['#bid'];
  $form_state['rebuild'] = TRUE;
}


function mostpopular_blocks_validate_count($element, &$form_state) {
  $value = $element['#value'];
  if (!is_numeric($value)) {
    form_error($element, t('This must be a valid integer.'));
  }
  $value = (int)$value;
  if ($value <= 0) {
    form_error($element, t('This must be a positive integer.'));
  }
}

function mostpopular_blocks_admin_form_submit(&$form, &$form_state) {
  if (!empty($form_state['confirm_delete'])) {
    $bid = $form_state['confirm_delete'];
    
    mostpopular_block_delete($bid);
    
    $form_state['redirect'] = 'admin/config/mostpopular/blocks';
    return;
  }
  
  foreach ($form_state['values']['blocks'] as $element) {
    $block = $element['block'];
    
    foreach ($element as $key => $val) {
      $block->$key = $val;
    }
    
    // Save the block definition.
    _mostpopular_save('block', $block);
  }
  
  drupal_set_message(t('Your Most Popular blocks were saved.'));
}

function theme_mostpopular_admin_blocks_table($vars) {
  $elements = $vars['element'];
  
  $header = array(
    t('ID'),
    t('Machine Name'),
    t('Title'),
    t('Type'),
    t('Max Results'),
    t('Actions'),
  );
  
  $rows = array();
  foreach (element_children($elements) as $bid) {
    $block = $elements[$bid];
    
    unset($block['id']['#title']);
    unset($block['name']['#title']);
    unset($block['title']['#title']);
    unset($block['count']['#title']);
    
    $row = array(
      'data' => array(
        drupal_render($block['id']),
        drupal_render($block['name']),
        drupal_render($block['title']),
        drupal_render($block['remote']),
        drupal_render($block['count']),
        drupal_render($block),
      ),
    );
    $rows[$bid] = $row;
  }
  
  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
  ));
}

/**
 * Implements form_FORM_ID_alter().
 * 
 * Adds extra properties to the block configuration form.
 * 
 * @param array $form
 * @param array $form_state
 */
function mostpopular_form_block_admin_configure_alter(&$form, &$form_state) {
  if ($form['module']['#value'] == 'mostpopular') {
    $delta = $form['delta']['#value'];
    
    $block = mostpopular_blocks($delta);
    if ($block) {
      $form_state['mostpopular_block'] = $block;
      
      // Assign weights to each of the existing components
      $form['title']['#weight'] = 0;
      $form['regions']['#weight'] = 10;
      $form['visibility_title']['#weight'] = 30;
      $form['visibility']['#weight'] = 31;
      $form['actions']['#weight'] = 40;
      
      $form['mostpopular'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#title' => t('Most Popular settings'),
        '#weight' => 20,
      
        'data' => array(
          '#tree' => TRUE,
          '#weight' => 10,
        
          'show_count' => array(
            '#type' => 'checkbox',
            '#title' => t('Show item counts'),
            '#description' => t('Should the number of times each item appears be displayed in the Most Popular block?'),
            '#default_value' => isset($block->data['show_count']) ? $block->data['show_count'] : FALSE,
          ),
        ),
      );
      if (empty($block->remote_bid)) {
        $form['mostpopular']['count'] = array(
          '#type' => 'textfield',
          '#title' => t('Max Results'),
          '#description' => t('The number of results to display in this block'),
          '#default_value' => $block->count,
          '#size' => 3,
          '#required' => TRUE,
          '#element_validate' => array('mostpopular_blocks_validate_count'),
          '#weight' => 1,
        );
      }
      else {
        $block->data += array(
          'remote_database' => 'default', 
        );
        
        global $databases;
        $options = array();
        foreach ($databases as $db => $data) {
          $options[$db] = $db;
        }
        
        $form['mostpopular']['data']['remote_database'] = array(
          '#type' => 'radios',
          '#title' => t('Database'),
          '#description' => t('Choose a database that will be used to lookup the most popular records.'),
          '#options' => $options,
          '#required' => TRUE,
          '#default_value' => check_plain($block->data['remote_database']),
        );
        $form['mostpopular']['remote_bid'] = array(
          '#type' => 'textfield',
          '#title' => t('Remote Block ID'),
          '#description' => t('The ID of the block in the Remote database.  That block\'s services, intervals and cached items will be displayed on this site.'),
          '#required' => TRUE,
          '#size' => 5,
          '#default_value' => check_plain($block->remote_bid),
          '#weight' => 11,
        );
      }
    }
  }
}
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
 * Provides an admin GUI for configuring intervals.
 */
/* ----------------------------------------------------------------------------
 * Intervals Form
 * --------------------------------------------------------------------------*/
function mostpopular_intervals_admin_form($form, &$form_state) {
  $form += array(
    '#tree' => TRUE,
    'blocks' => array(
  		'#theme' => 'mostpopular_admin_intervals_table',
    ),
  );
  
  $blocks = mostpopular_blocks_local();
  $form_state['blocks'] = $blocks;
  
  $block_options = array();
  foreach ($blocks as $bid => $block) {
    $block_options[$bid] = $block->title;
  }
  
  foreach ($blocks as $bid => $block) {
    $form['blocks'][$bid] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Block: @title', array(
        '@title' => $block->title,
      )),
      
      'bid' => array(
        '#type' => 'hidden',
        '#value' => $bid,
      ),
      'intervals' => array(
        '#tree' => TRUE,
      ),
    );
    
    if (!isset($form_state['intervals'][$bid])) {
      $form_state['intervals'][$bid] = mostpopular_interval_load_by_block($bid);
    }
    
    // If there are no intervals for this block, create some default ones.
    if (empty($form_state['intervals'][$bid])) {
      $form_state['intervals'][$bid] = mostpopular_interval_defaults($bid);
    }
    
    foreach ($form_state['intervals'][$bid] as $iid => $interval) {
      $form['blocks'][$bid]['intervals'][$iid] = array(
        'id' => array(
          '#markup' => !empty($interval->iid) ? $interval->iid : t('New'),
        ),
        'title' => array(
          '#type' => 'textfield',
          '#size' => 32,
          '#default_value' => check_plain($interval->title),
        ),
        'string' => array(
          '#type' => 'textfield',
          '#size' => 32,
          '#default_value' => check_plain($interval->string),
        ),
        'weight' => array(
          '#type' => 'textfield',
          '#size' => 3,
          '#default_value' => isset($interval->weight) ? $interval->weight : count($form_state['intervals'][$bid]),
        ),
        'bid' => array(
          '#type' => 'select',
          '#default_value' => $bid,
          '#options' => $block_options,
        ),
      );
    }
    $form['blocks'][$bid]['add_button'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#submit' => array( 'mostpopular_intervals_admin_form_add'),
      '#bid' => $bid,
      '#name' => "add_button[$bid]",
    );
  }
    
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array( 'mostpopular_intervals_admin_form_submit' ),
  );

  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to Default Configuration'),
    '#attributes' => array( 'onclick' => 'javascript:return confirm("' . t(
"This will reset all the intervals and all the cached most popular data.  Are you sure you want to do this?") .
    '");'),
    '#weight' => 101,
    '#submit' => array('mostpopular_intervals_admin_form_reset'),
  );
  return $form;
}

function mostpopular_intervals_admin_form_add($form, &$form_state) {
  if (!empty($form_state['triggering_element']['#bid'])) {
    $bid = $form_state['triggering_element']['#bid'];
    
    $form_state['intervals'][$bid][] = (object)array(
      'bid' => $bid,
      'title' => '',
      'string' => '',
    );
  }
  $form_state['rebuild'] = TRUE;
}

function mostpopular_intervals_admin_form_validate($form, &$form_state) {
  $intervals = $form_state['intervals'];
  
  // Validate the intervals
  foreach ($form_state['values']['blocks'] as $bid => $block) {
    if (isset($block['intervals'])) {
      $good = FALSE;
      foreach ($block['intervals'] as $iid => $values) {
        
        // Lookup the interval definition.
        if (isset($intervals[$bid][$iid])) {
          $interval = $intervals[$bid][$iid];
        }
        
        $title = $values['title'];
        $string = $values['string'];
    
        if (empty($title)) {
          if (empty($string)) {
            // Ignore blank rows
            unset($form_state['intervals'][$bid][$iid]);
          }
          else {
            form_set_error("blocks][$bid][intervals][$iid][title", t('You must specify the title to show users for this interval.'));
          }
        }
        else {
          if (empty($string) || strtotime($string) === FALSE) {
            form_set_error("blocks][$bid][intervals][$iid][string", t("You must specify an interval that can be understood by <a href='@strtotime' target='php'>strtotime()</a>.", array(
              '@strtotime' => 'http://php.net/manual/en/function.strtotime.php'
            )));
          }
          elseif (strtotime($string) > time()) {
            form_set_error("blocks][$bid][intervals][$iid][string", t('You must specify a negative interval relative to the current time, i.e. "-1 week"'));
          }
          else {
            // Update the interval object
            if ($string != $interval->string) {
              $interval->dirty = TRUE;
            }
            $interval->string = $string;
            
            if ($title != $interval->title) {
              $interval->dirty = TRUE;
            }
            $interval->title = $title;
            
            $interval->bid = $values['bid'];
            $interval->weight = $values['weight'];
            
            $form_state['intervals'][$bid][$iid] = $interval;
            
            $good = TRUE;
          }
        }
      }
      if (!$good) {
        form_set_error(NULL, t('You must define at least one interval for each block.'));
      }
    }
  }
}

function mostpopular_intervals_admin_form_submit($form, $form_state) {
  $exist = array();
  
  foreach ($form_state['intervals'] as $bid => $block) {
    foreach ($block as $iid => $interval) {
      
      // Save the interval
      $status = FALSE;
      if (empty($interval->iid)) {
        $status = 'added';
      }
      if (!empty($interval->dirty)) {
        $status = 'updated';
      }
      mostpopular_interval_save($interval);
      
      // Rehash the interval by its new ID.
      $exist[$bid][$interval->iid] = $interval;
      
      if ($status) {
        drupal_set_message(t('Interval %title was @status.', array(
          '%title' => mostpopular_interval_title($interval),
          '@status' => $status,
        )));
      }
    }
  }
  
  // Identify the intervals to remove
  $intervals = mostpopular_intervals();
  foreach ($intervals as $interval) {
    if (!isset($exist[$interval->bid][$interval->iid])) {

      // Remove the interval and any cached items
      mostpopular_interval_delete($interval);

      drupal_set_message(t('Interval %title was removed.', array(
        '%title' => mostpopular_interval_title($interval),
      )));
    }
  }
  $form_state['intervals'] = $exist;
  
  drupal_set_message(t('The interval configuration has been saved.'));
}

/**
 * Defines a theme function for rendering the intervals form.
 * 
 * @param array $variables 
 *   - form: The form to render.
 */
function theme_mostpopular_admin_intervals_table($variables) {
  $elements = $variables['element'];
  
  $header = array(
    '',
    t('ID'),
    array('data' => t('Title'), 'colspan' => 2),
    t('Interval'),
    t('Block'), 
    t('Weight'),
  );

  $rows = array();
  $output = '';

  foreach (element_children($elements) as $bid) {
    $block = $elements[$bid];
    
    $block['bid']['#attributes']['class'][] = 'mostpopular-block-bid';
    
    $title = $block['#title'];
    $rows[] = array(
      'data' => array(
        array(
          'data' => '<strong>' . $title . '</strong>' . drupal_render($block['bid']),
          'colspan' => 7,
        ),
      ),
    );
    foreach (element_children($block['intervals']) as $sid) {
      $item = $block['intervals'][$sid];

      // Add class to group weight fields for drag and drop
      $item['bid']['#attributes']['class'][] = "mostpopular-bid";
      $item['bid']['#attributes']['class'][] = "mostpopular-bid-$bid";
      $item['weight']['#attributes']['class'][] = "mostpopular-weight";
      $item['weight']['#attributes']['class'][] = "mostpopular-weight-$bid";
  
      $row = array();
      $row[] = array( 'data' => '', 'width' => 1 );
      $row[] = drupal_render($item['id']);
      $row[] = array( 'data' => t('Past'), 'width' => '8' );
      $row[] = drupal_render($item['title']);
      $row[] = drupal_render($item['string']);
      $row[] = drupal_render($item['bid']);
      $row[] = drupal_render($item['weight']);
      
      $rows[] = array(
        'data' => $row,
        'class' => array( 'draggable' ),
      );
    }
    $rows[] = array(
      'data' => array(
        array( 'data' => '', 'width' => 1 ),
        array( 'data' => drupal_render($block['add_button']), 'colspan' => 6 ),
      ),
    );
    
    // Add tabledrag behavior to this region
    drupal_add_tabledrag('mostpopular-admin-intervals', 'match', 'parent', 'mostpopular-bid', "mostpopular-bid-$bid", 'mostpopular-block-bid', FALSE);
    drupal_add_tabledrag('mostpopular-admin-intervals', 'order', 'sibling', 'mostpopular-weight', "mostpopular-weight-$bid");
  }
  $output .= theme('table', array( 
    'header' => $header, 
    'rows' => $rows, 
    'attributes' => array(
      'id' => 'mostpopular-admin-intervals'
    ),
  ));
  
  drupal_add_css(drupal_get_path('module', 'mostpopular') . '/css/mostpopular-admin.css');
  
  return $output;
}

<?php

/**
 * @file
 * Contains function for the settings form.
 */

/**
 * Returns built admin form.
 */
function sequential_nodes_admin_settings($form, &$form_state) {

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => TRUE,
  );

  $form['settings']['sequential_nodes_prev_link'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Previous link display'),
    '#default_value' => variable_get('sequential_nodes_prev_link', '[node:title]'),
    '#description' => t('Text to display as previous node link'),
  );

  $form['settings']['sequential_nodes_use_mid_link'] = array(
    '#type' => 'select',
    '#title' => t('Middle link display style'),
    '#options' => array(
      '0' => t('Hidden'),
      '<front>' => t('Link to the front page'),
      'jumplink' => t('Link to the top of the current page'),
    ),
    '#default_value' => variable_get('sequential_nodes_use_mid_link', '0'),
    '#description' => t('Show or hide middle link'),
  );

  $form['settings']['sequential_nodes_mid_speed'] = array(
    '#type' => 'select',
    '#title' => t('Speed to scroll to the top'),
    '#options' => array(
      'slow' => t('Slow'),
      'fast' => t('Fast'),
    ),
    '#default_value' => variable_get('sequential_nodes_mid_speed', 'slow'),
    '#description' => t('Page scroll speed when link to top is selected'),
    '#states' => array(
      'visible' => array(
        'select[name="sequential_nodes_use_mid_link"]' => array('value' => 'jumplink'),
      ),
    ),
  );

  $form['settings']['sequential_nodes_mid_link'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Middle text display'),
    '#default_value' => variable_get('sequential_nodes_mid_link', '[node:title]'),
    '#description' => t('Text to display as middle link'),
    '#states' => array(
      'invisible' => array(
        'select[name="sequential_nodes_use_mid_link"]' => array('value' => '0'),
      ),
    ),
  );

  $form['settings']['sequential_nodes_next_link'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Next link display'),
    '#default_value' => variable_get('sequential_nodes_next_link', '[node:title]'),
    '#description' => t('Text to display as next node link'),
  );

  $form['settings']['markup_text'] = array(
    '#type' => 'item',
    '#markup' => t('These fields support tokens'),
  );

  $form['settings']['tokens'] = array(
    '#theme' => 'token_tree',
    '#dialog' => TRUE,
    '#token_types' => array('node'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
  );

  $form['adv_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
  );

  $form['adv_settings']['sequential_nodes_preferable'] = array(
    '#type' => 'select',
    '#title' => t('Preferable nodes are:'),
    '#options' => array(
      0 => t('None'),
      1 => t('Sticky'),
      2 => t('Promoted to front page'),
    ),
    '#default_value' => variable_get('sequential_nodes_preferable', 1),
    '#description' => t('Nodes with this field set are first in the sequence'),
  );

  return system_settings_form($form);

}

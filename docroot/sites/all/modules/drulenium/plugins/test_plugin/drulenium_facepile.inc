<?php

/**
 * @file facepile.inc
 *
 * Main (ctools) plugin file for "facepile" plugin type
 */

$plugin = array(
    'title' => t('Verify Text'),
    'description' => t('Drulenium Verify Text plugin'),
    'html tag name' => 'facepile'
)
;

function drulenium_facepile_defaults() {
  return array(
      'text' => '',
      'size' => 'small',
      'pages' => '',
      'numrows' => 1,
      'width' => 200,
      'colorscheme' => 'light' ,
      'locator' => '',
  );
}

function drulenium_facepile_fb_settings($options) {

  $form = array();
  $form['locator'] = array(
      '#type' => 'textfield',
      '#title' => t('Element Locator'),
      '#description' => t('The Locator of the element you want to Verify.')
  );
  $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text'),
      '#description' => t('The Text you want to Verify.')
  );
  /*
   $form['pages'] = array(
       '#type' => 'textarea',
       '#title' => t('Pages'),
       '#description' => t('The pages you want to Test')
   );
  */
  /*
   $form['visibility'] = array(
       '#type' => 'vertical_tabs',
       '#attached' => array(
           'js' => array(drupal_get_path('module', 'block') . '/block.js'),
       ),
   );

  // Per-path visibility.
  $form['visibility']['path'] = array(
      '#type' => 'fieldset',
      '#title' => t('Pages'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'visibility',
      '#weight' => 0,
  );
  $form['visibility']['path']['pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Test on specific pages'),
      '#description' => "Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog.",
      '#default_value' => isset($options['visibility']['path']['pages']) ? $options['visibility']['path']['pages'] : 'blog/*',
  );

  // Per-node-type testing.
  $form['visibility']['user'] = array(
      '#type' => 'fieldset',
      '#title' => t('Content Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'visibility',
      '#weight' => 20,
  );
  $node_types = array();
  $result = db_query(
      'SELECT type, name FROM {node_type}');
  foreach ($result as $row) {
  $node_types[$row->type] = $row->name;
  }
  $form['visibility']['user']['custom'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Test for specific content types'),
      '#options' => $node_types,
      '#description' => t('Test on pages that display content of the given type(s).'),
      '#default_value' => isset($options['visibility']['user']['custom']) ? $options['visibility']['user']['custom'] : '',
  );

  // Per-term-type testing.
  $form['visibility']['term'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'visibility',
      '#weight' => 20,
  );
  $node_types = array();
  $result = db_query(
      'SELECT machine_name, name FROM {taxonomy_vocabulary}');
  foreach ($result as $row) {
  $node_types[$row->machine_name] = $row->name;
  }
  $form['visibility']['term']['custom'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Test for specific Taxonomy Vocabularies'),
      '#options' => $node_types,
      '#description' => t('Test on taxonomy term pages of the given Vocabulary type(s)'),
      '#default_value' => isset($options['visibility']['term']['custom']) ? $options['visibility']['term']['custom'] : '',
  );
  */

  $defaults = drulenium_facepile_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}

function drulenium_facepile_drupal_settings($options) {
  return array();
}

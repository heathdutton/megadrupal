<?php

/**
 * @file
 * Creates a test page for DART ad tag.
 */


/**
 * Menu callback; Displays all DART tags on a single page for testing purposes.
 */
function dart_test_page() {
  $tags = dart_tag_load();

  $content = array();
  $content['test_page_form'] = drupal_get_form('dart_test_page_form');
  $content['ads'] = array();

  foreach ($tags as $tag) {
    $tag->mode = 'test';

    $content['ads'][$tag->machinename] = array(
      '#type'   => 'markup',
      '#prefix' => '<div class="ad_wrap">',
      '#suffix' => '</div>',
      'title'   => array(
        '#type'   => 'markup',
        '#prefix' => '<h2>',
        '#suffix' => '</h2>',
        '#markup' => $tag->name,
      ),
      'tag'     => array(
        '#type'   => 'markup',
        '#theme'  => 'dart_tag',
        '#tag'    => $tag,
      ),
      'krumo'   => array(
        '#type'   => 'markup',
        '#markup' => module_exists('devel') ? kpr($tag, TRUE) : '',
      ),
      '#sorted' => TRUE,
    );
  }

  return $content;
}

/**
 * Form function for the override settings at the top of the test page.
 */
function dart_test_page_form($form, &$form_state, $edit = array()) {
  $form['#method'] = 'get';
  $form['site'] = array(
    '#type' => 'textfield',
    '#title' => t('Site'),
    '#default_value' => isset($_GET['site']) ? check_plain($_GET['site']) : '',
  );
  $form['zone'] = array(
    '#type' => 'textfield',
    '#title' => t('Zone'),
    '#default_value' => isset($_GET['zone']) ? check_plain($_GET['zone']) : '',
  );
  $form['adtest'] = array(
    '#type' => 'hidden',
    '#default_value' => 'true',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Override'),
  );

  return $form;
}

/**
 * Alters tags to use test prefix.site.zone if the adtest query string.
 */
function dart_tag_adtest(&$tag) {
  // If there is an "adtest" query string, show test ad tags instead.
  if (isset($_GET['adtest']) && $_GET['adtest'] == 'true') {
    $tag->prefix = isset($_GET['prefix']) ? check_plain($_GET['prefix']) : variable_get('dart_test_prefix', '');
    $tag->site = isset($_GET['site']) ? check_plain($_GET['site']) : variable_get('dart_test_site', '');
    $tag->zone = isset($_GET['zone']) ? check_plain($_GET['zone']) : variable_get('dart_test_zone', '');
  }
}

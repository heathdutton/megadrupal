<?php
// $Id$

/**
 * @file
 * Admin page callback for the seotools module.
 */

/*
 * TODO: finish wizard
 */
function seo_admin_setup($step = 0) {
  if (!$step) {
    $step = 0;
  }
  $output = render(drupal_get_form('seo_setup_' . $step .'_form'));
  return $output;
}

/** 
 * Checks if dependencies are installed
 * @param $form
 * @param $form_state
 */
function seo_setup_0_form($form, &$form_state) {
  // run checks here if needed
  drupal_goto('admin/config/search/seo/setup/1');
  
  return $form;
}

function seo_setup_1_form($form, $form_state) {

  drupal_set_title(t('SEO setup: meta tags'));
  $form = array();
  $instructions = t('This form enables you to setup the default meta tags for your site.');

  $form['instructions'] = array(
    '#markup' => $instructions,
  );
 
  $form['setup_mode'] = array(
    '#type' => 'value',
    '#value' => 1,
  );
  
  $types = node_type_get_types();
  $patterns = array(
    '' => '',
    'article' => 'article/[node:title]',
    'page' => '',
  );
  
  // Pathauto setup
  include_once(drupal_get_path('module', 'metatag') . '/metatag.admin.inc');
  $config = metatag_config_load('global');
  $f = metatag_config_edit_form(array(), $form_state, $config);

  $f['actions']['save']['#value'] = t('Save global meta tags');
  unset($f['actions']['cancel']);
  //unset($f['#theme']);
  //unset($f['#submit']);
  $form = array_merge($form, $f);
  $form['#submit'][] = 'seo_setup_1_form_submit';


  return $form;
}

function seo_setup_1_form_validate($form, &$form_state) {

}

function seo_setup_1_form_submit($form, &$form_state) {
  //drupal_set_message(t('API account information has been saved.'));
  drupal_goto('admin/config/search/seo/setup/2');
}

function seo_setup_2_form($form, $form_state) {
  
  drupal_set_title(t('SEO setup: meta tags'));
  $form = array();
  $instructions = t('This form enables you to setup url path patterns for your site.');
  $instructions .= ' ' . t('It is recommended that you set up relevant paths for each of your content types.');

  $form['instructions'] = array(
    '#markup' => $instructions,
  );
 
  $form['setup_mode'] = array(
    '#type' => 'value',
    '#value' => 2,
  );
  
  $types = node_type_get_types();
  $patterns = array(
    '' => '',
    'article' => 'article/[node:title]',
    'page' => '',
  );
  
  // Pathauto setup
  include_once(drupal_get_path('module', 'pathauto') . '/pathauto.admin.inc');
  $f = pathauto_patterns_form(array(), $form_state);
  $description = '';
  foreach ($f['node'] AS $name => $field) {
    if (strpos($name, 'pathauto_node_') === 0) {
      $type = str_replace('pathauto_node_', '', $name);
      $type = substr($type, 0, -8);
      if (!isset($f['node'][$name]['#default_value']) || !$f['node'][$name]['#default_value']) {
        $f['node'][$name]['#default_value'] = isset($patterns[$type]) ? $patterns[$type] : "$type/[node:title]";
      }
    }
  }
  $f['taxonomy_term']['#collapsed'] = TRUE;
  $f['user']['#collapsed'] = TRUE;

  $f['actions']['save']['#value'] = t('Save url path patterns');
  //unset($f['actions']);
  //unset($f['#theme']);
  //unset($f['#submit']);
  $form = array_merge($form, $f);
  $form['#submit'][] = 'seo_setup_2_form_submit';


  return $form;
}

function seo_setup_2_form_submit($form, &$form_state) {
  //drupal_set_message(t('API account information has been saved.'));
  drupal_goto('admin/config/search/seo/setup/3');
}

/**
 * Select social media profiles
 * @param $form
 * @param $form_state
 */
function seo_setup_3_form($form, &$form_state) {
  $msg = t('Congratulations, you have successfully configured essential SEO features for your site.');
  /*
  $msg .= '<h3>' . t('Next steps') . '</h3>';
  $items[] = t('If you have list of targeted keywords for your site, !import_link.',
    array(
      '!import_link' => l(t('import your keywords'), 'admin/structure/kwresearch/keyword_list/import'),
    )
  );
  $items[] = t('Discover targeted keywords for your site through !kwreserach_link.',
    array(
      '!kwreserach_link' => l(t('keyword research'), 'admin/structure/kwresearch/keyword_report'),
    )
  );
  $items[] = t('Analyze and optimize your content for the search engines using !contentanalysis_link.',
    array(
      '!contentanalysis_link' => l(t('content analysis'), 'admin/content/insight'),
    )
  );
  $msg .= theme('item_list', array('items' => $items));
  */
  
	$form['done'] = array(
	  '#markup' => $msg,
	);

  return $form;
}
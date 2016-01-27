<?php

/**
 * Implements template_html_head_alter();
 *
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function easy_clean_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * Implements template_proprocess_search_block_form().
 *
 * Changes the search form to use the HTML5 "search" input attribute
 */
function easy_clean_preprocess_search_block_form(&$variables) {
  $variables['search_form'] = str_replace('type="text"', 'type="search"', $variables['search_form']);
}

/**
 * Implements template_preprocess_html();
 */
function easy_clean_preprocess_html(&$variables) { }

/**
 * Implements template_preprocess_page();
 */
function easy_clean_preprocess_page(&$variables) {
  
  if (isset($variables['main_menu'])) {
    $variables['primary_nav'] = theme('links__system_main_menu', array(
      'links' => $variables['main_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'main-menu'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $variables['primary_nav'] = FALSE;
  }
	
  if (isset($variables['secondary_menu'])) {
    $variables['secondary_nav'] = theme('links__system_secondary_menu', array(
      'links' => $variables['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'secondary-menu'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $variables['secondary_nav'] = FALSE;
  }
}

/**
 * Implements template_preprocess_node();
 */
function easy_clean_preprocess_node(&$variables) {
  $variables['submitted'] = t('Submitted by !username on ', array('!username' => $variables['name']));
  $variables['submitted_date'] = t('!datetime', array('!datetime' => $variables['date']));
  $variables['submitted_pubdate'] = format_date($variables['created'], 'custom', 'Y-m-d\TH:i:s');
}

/**
 * Implements template_breadcrumb();
 */
function easy_clean_breadcrumb($variables) {
  
	$breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
		
		$breadcrumb[] = drupal_get_title();
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= '<div class="breadcrumb">' . implode(' » ', $breadcrumb) . '</div>';
    return $output;
  }
}

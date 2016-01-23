<?php

/**
 * @file
 * Contains theme override functions and preprocess functions for the Scratch theme.
 * Copied from Boron
 */

/**
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function scratch_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * Changes the search form to use the HTML5 "search" input attribute
 */
function scratch_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}

/**
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 */
function scratch_preprocess_html(&$vars) {
  if (module_exists('rdf')) {
    $vars['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $vars['rdf']->version = 'version="HTML+RDFa 1.1"';
    $vars['rdf']->namespaces = $vars['rdf_namespaces'];
    $vars['rdf']->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $vars['doctype'] = '<!DOCTYPE html>' . "\n";
    $vars['rdf']->version = '';
    $vars['rdf']->namespaces = '';
    $vars['rdf']->profile = '';
  }
}

/**
 * Implementation of hook_css_alter().
 * Removing certain core or module stylesheets.
 */
function resflows_css_alter(&$css) {
  foreach ($css as $file => $value) {
    if (strpos($file, 'system.menus.css') !== FALSE) {
      unset($css[$file]);
    }
  }
}

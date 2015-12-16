<?php
/**
 * Implements template_proprocess_html().
 */
function versatile_html5_preprocess_html(&$vars) {
  if (module_exists('rdf')) {
    $vars['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $vars['rdf_version'] = ' version="HTML+RDFa 1.1"';
    $vars['rdf_profile'] = ' profile="' . $vars['grddl_profile'] . '"';
  }
  else {
    $vars['doctype'] = '<!DOCTYPE html>' . "\n";
    $vars['rdf_version'] = '';
    $vars['rdf_profile'] = '';
  }
}

/**
 * Preprocess function for site template layout.
 */
function versatile_html5_preprocess_versatile_html5_site_template(&$vars) {
  versatile_preprocess_versatile_site_template($vars);
}

<?php
/**
 * @file
 * Implements hook_preprocess_html().
 */

global $theme_key, $language;
$theme_name = $theme_key;

// DOCTYPE.
$vars['doctype'] = '<!DOCTYPE html>' . "\n";
$vars['rdf_profile'] = '';

// Use a proper attributes array for the html attributes.
$vars['html_attributes_array']['lang'][] = $language->language;
$vars['html_attributes_array']['dir'][] = $language->dir;

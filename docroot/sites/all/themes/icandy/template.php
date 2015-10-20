<?php
/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by user themes.
 */
function icandy_preprocess_search_theme_form(&$vars, $hook) {

  // Modify elements of the search form
  $vars['form']['search_theme_form']['#title'] = NULL;
   // Add a custom class to the search box
  $vars['form']['search_theme_form']['#attributes'] = array('class' => t('s'));
  // Rebuild the rendered version (search form only, rest remains unchanged)
  unset($vars['form']['search_theme_form']['#printed']);
  $vars['search']['search_theme_form'] = drupal_render($vars['form']['search_theme_form']);

  // Rebuild the rendered version (submit button, rest remains unchanged)
  unset($vars['form']['submit']['#printed']);
  $vars['search']['submit'] = NULL;

  // Collect all form elements to make it easier to print the whole form.
  $vars['search_form'] = implode($vars['search']);
}


/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
    $out='<ul>';
    if (!empty($breadcrumb)) {
        //print_r ($breadcrumb);
        foreach ($breadcrumb as $key => $val) {
            $out .='<li>'. $val .'</li>';
        }
      }
      else {
        global $base_path;
        $out .='<li><a href="'. $base_path .'">Home</a></li>';
    }
    $out .='</ul>';
    return $out;
}

function icandy_preprocess_node(&$variables) {
  $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
}


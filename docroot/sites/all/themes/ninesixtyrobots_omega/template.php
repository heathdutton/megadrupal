<?php

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega) provides
 * all the basic functionality. However, in case you wish to customize the output that Drupal
 * generates through Alpha & Omega this file is a good place to do so.
 * 
 * Alpha comes with a neat solution for keeping this file as clean as possible while the code
 * for your subtheme grows. Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */

/**
* Implmentation of hook_theme().
*/
function ninesixtyrobots_omega_theme() {
 return array(
   // Add our own function to override the default node form for story.
   'article_node_form' => array(
     'render element' => 'form',
   ),
 );
}

/**
* Implements hook_form_FORM_ID_alter().
*/
function ninesixtyrobots_omega_form_article_node_form_alter(&$form, &$form_state) {
 // Move the status element outside of the options fieldset so that it doesn't
 // get taken over by the vertical tabs #pre_render operation. If the status
 // element is not moved here you'll end up with duplicates when trying to
 // render the status element on it's own below.
 $form['status'] = $form['options']['status'];
 unset($form['options']['status']);
}

/**
* Custom function to pull the Published check box out and make it obvious.
*/
function ninesixtyrobots_omega_article_node_form($variables) {
 $form = $variables['form'];
 $published = drupal_render($form['status']);
 $buttons = drupal_render($form['actions']);
 // Make sure we also render the rest of the form, not just our custom stuff.
 $everything_else = drupal_render_children($form);
 return $everything_else . $published . $buttons;
}

/**
* Override the breadcrumb to allow for a theme delimiter setting.
*/
function ninesixtyrobots_omega_breadcrumb($variables) {
 $breadcrumb = $variables['breadcrumb'];
 $delimiter = theme_get_setting('breadcrumb_delimiter');

 if (!empty($breadcrumb)) {
   $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

   $output .= '<div class="breadcrumb">' . implode($delimiter, $breadcrumb) . '</div>';
   return $output;
 }
}

/**
* Preprocess function for theme_username().
*
* Override the username display to automatically swap out username for a 
* field called real_name, if it exists.
*/
function ninesixtyrobots_omega_preprocess_username(&$variables) {
 // Ensure that the full user object is loaded.
 $account = user_load($variables['account']->uid);

 // See if it has a real_name field and add that as the name instead.
 if (isset($account->field_real_name[LANGUAGE_NONE][0]['safe_value'])) {
   $variables['name'] = $account->field_real_name[LANGUAGE_NONE][0]['safe_value'];
 }
}

/**
* Implements hook_form_FORM_ID_alter().
*
* Override the search box to add our pretty graphic instead of the button.
*/
function ninesixtyrobots_omega_form_search_block_form_alter(&$form, &$form_state) {
 $form['actions']['submit']['#type'] = 'image_button';
 $form['actions']['submit']['#src'] = drupal_get_path('theme', 'ninesixtyrobots_omega') . '/images/search.png';
 $form['actions']['submit']['#attributes']['class'][] = 'btn';
}

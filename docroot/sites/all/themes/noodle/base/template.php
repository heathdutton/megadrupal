<?php


/**
* Auto-rebuild the theme registry during theme development.
*/
if (theme_get_setting('noodle_rebuild_registry')) {
  // Rebuild .info data.
  system_rebuild_theme_data();
  // Rebuild theme registry.
  drupal_theme_rebuild();
}


/**
* Preprocess html with more options then default
*/
function noodle_preprocess_html(&$variables) {
  // Add a class that gives us a domain/server name. A very handy solution in altering domain based style.
  $variables['classes_array'][] = 'domain-' . str_replace('.', '-', $_SERVER['SERVER_NAME']);
  foreach($variables['user']->roles as $role) {
    $variables['classes_array'][] =  'role-'.str_replace(' ', '-', $role);
  }
     // dsm ($variables);

}

/**
* Add unique class (mlid) to all menu items.
*/
function noodle_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';
  $element['#attributes']['class'][] = 'menu-' . $element['#original_link']['mlid'];
  
  ## print_r ($element);

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
* Add additional features to default drupal page.
*/
function noodle_preprocess_page(&$variables) {
   //template suddestions (http://drupal.org/node/1035656#comment-3997484). ie: page--type--blog.tpl.php or page--node--2.tpl.php
	if (isset($variables['node'])) {  
    $variables['theme_hook_suggestions'][] = 'page__type__'. $variables['node']->type;
    $variables['theme_hook_suggestions'][] = 'page__node__' . $variables['node']->nid;
  }

  // Adding theme's main javascript'.
  drupal_add_js(drupal_get_path('theme', 'noodle') . '/noodle.js', array('weight' => 10, 'scope' => 'header'));
   
  // let's add meerkat.js if the bottom message region is populated 
  if (block_get_blocks_by_region('message_bottom')) {
   drupal_add_js(drupal_get_path('theme', 'noodle') . '/jquery.meerkat.min.js', array('weight' => 1, 'scope' => 'footer'));

    drupal_add_js('jQuery(document).ready(function () {
      jQuery("#message-bottom").meerkat({ width: "100%", position: "bottom", close: ".close-meerkat", dontShowAgain: ".forget-meerkat", animationIn: "slide", animationSpeed: 500 });
    });', array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
  }

  // let's add accordion jq if accordion region is populated 
  if (block_get_blocks_by_region('sidebar_accordion')) {
    drupal_add_js(drupal_get_path('theme', 'noodle') . '/jquery.switchtarget.min.js', array('weight' => 1, 'scope' => 'footer'));
    drupal_add_js('jQuery(document).ready(function(){jQuery(".accordionlink").switchTarget({effect : "sliding", startHidden : "true", speed : "fast"});});', 'inline');
  }

   #print_r ($variables);
}

/**
* Override default node template.
*/
function noodle_preprocess_node(&$variables) {

  // Add a class for the view mode.
  if (!$variables['teaser']) {
    $variables['classes_array'][] = 'view-mode-' . $variables['view_mode'];
  }

  // Add a class to show node is authored by current user.
  if ($variables['uid'] && $variables['uid'] == $GLOBALS['user']->uid) {
    $variables['classes_array'][] = 'node-by-viewer';
  }

  $variables['title_attributes_array']['class'][] = 'node-title';
}


/**
* Override default block template.
*/
function noodle_preprocess_block(&$variables) {

}

/**
* Override default maintenance page.
*/
function noodle_preprocess_maintenance_page(&$variables) {
  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
}

/**
* Hide filter tips in comments.
*/

function noodle_form_comment_form_alter(&$form, &$form_state, &$form_id) {
  //dpm($form);
  if (theme_get_setting('noodle_comments_format_tips')) {
    $form['comment_body']['#after_build'][] = 'noodle_customize_comment_form';  
  }
}

function noodle_customize_comment_form(&$form) {
  $form[LANGUAGE_NONE][0]['format']['#access'] = FALSE;
  return $form;  
}

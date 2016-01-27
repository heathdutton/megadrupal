<?php

/**
 * @file
 * Page callbacks for recipe index page.
 */

function recipe_landing_page() {
  return theme('recipe_landing_page');
}


function theme_recipe_landing_page() {
  drupal_set_title(t("Recipes Menu"));

  // Add the recipe.css file for this page.
  drupal_add_css(drupal_get_path('module', 'recipe') . '/recipe.css');

  // Get a list of current sub-menus and render them as a list.
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  if (user_access('create recipe content')) {
    $content[] = array(
      'title' => t('Add a new recipe'),
      'description' => '',
      'href' => 'node/add/recipe',
      'localized_options' => array('attributes' => array()),
    );
  }

  $output = '<div class="recipe_index_list">';
  $output .= theme('node_add_list', array('content' => $content));
  $output .= '</div>';

  // Render the recent recipes list.
  if (variable_get('recipe_recent_box_enable', 1) == 1) {
    $node_list = recipe_get_latest(variable_get('recipe_recent_display', 5));
    $build_node_list = node_title_list($node_list);
    $output .= '<div class="recipe_index_recent">';
    $output .= '<h2 class="title">' . variable_get('recipe_recent_box_title', t('Latest recipes')) . '</h2><div>' . drupal_render($build_node_list) . '</div>';
    $output .= '</div>';
  }

  return $output;
}

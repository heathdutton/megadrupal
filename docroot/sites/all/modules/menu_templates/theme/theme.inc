<?php
/**
 * @file
 * Here we will use Drupal hooks to interfear in the menu render, so our templates
 * can replace the Drupal standard way to render templates.
 */

/**
 * Implements hook_preprocess_menu_tree().
 */
function menu_templates_preprocess_menu_tree(&$variables) {
  // Do not change anything
  return $variables;
}

/**
 * Implements hook_menu_tree().
 */
function menu_templates_menu_tree($variables) {
  // Get the first non argument menu item
  foreach ($variables['tree'] as $name => $item) {
    if ($name[0] !== '#') {
      // Get the level and menu name of the link
      $level = _menu_templates_guess_link_level($item['#original_link']);
      $menu_name = $item['#original_link']['menu_name'];
      break;
    }
  }

  // Try to load the template of this menu
  $template = menu_templates_get_template_details(array('mid' => $menu_name));

  if (isset($template["l${level}_wrapper"])) {
    // Replace the token and return
    $output = str_replace('!tree', $variables['tree']['#children'], $template["l${level}_wrapper"]);
    return $output;
  }

  // Use the fallback
  $fallback = _menu_templates_get_fallback('menu_tree');
  template_preprocess_menu_tree($variables);
  return $fallback($variables);
}

/**
 * Implements hook_menu_link().
 */
function menu_templates_menu_link(array $variables) {
  $element = $variables['element'];
  $menu_name = $element['#original_link']['menu_name'];
  $link_level = _menu_templates_guess_link_level($element['#original_link']);

  // Try to load the template of this menu
  $template = menu_templates_get_template_details(array('mid' => $menu_name));

  if (isset($template["l${link_level}_template"])) {
    // We found a template, so render using it
    $link = l($element['#title'], $element['#href'], $element['#localized_options']);
    $attributes = drupal_attributes($element['#attributes']);
    $sub_menu = $element['#below'] ? drupal_render($element['#below']) : '';

    $tokens = array("!link", "!attributes", "!submenu");
    $values = array($link, $attributes, $sub_menu);

    $output = str_replace($tokens, $values, $template["l${link_level}_template"]);

    return $output;
  }

  // Use the fallback mode
  $fallback = _menu_templates_get_fallback('menu_link');
  return $fallback($variables);
}

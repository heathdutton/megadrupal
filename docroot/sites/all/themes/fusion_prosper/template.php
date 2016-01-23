<?php

/**
 * Template overrides.
 */

 /**
 * Override or insert variables into the page template for HTML output.
 */
function fusion_prosper_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

 /**
 * Override or insert variables into the page template.
 */
function fusion_prosper_process_page(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
 }

/**
 * Override or insert variables into the node template.
 */
function fusion_prosper_preprocess_node(&$vars) {

  // Render Ubercart fields into separate variables for node--product.tpl.php
  if (module_exists('uc_product') && uc_product_is_product($vars)) {
    $vars['fusion_uc_image'] = drupal_render($vars['content']['uc_product_image']);
    $vars['fusion_uc_body'] = drupal_render($vars['content']['body']);
    $vars['fusion_uc_display_price'] = drupal_render($vars['content']['display_price']);
    $vars['fusion_uc_add_to_cart'] = drupal_render($vars['content']['add_to_cart']);
    $vars['fusion_uc_sell_price'] = drupal_render($vars['content']['sell_price']);
    $vars['fusion_uc_cost'] = drupal_render($vars['content']['cost']);
    $vars['fusion_uc_sku'] = drupal_render($vars['content']['sku']);
    $vars['fusion_uc_weight'] = (!empty($vars['content']['weight'])) ? drupal_render($vars['content']['weight']) : ''; // Hide weight if empty
    if ($vars['fusion_uc_weight'] == '') {
      unset($vars['content']['weight']);
    }
    $dimensions = !empty($vars['content']['height']) && !empty($vars['content']['width']) && !empty($vars['content']['length']); // Hide dimensions if empty
    $vars['fusion_uc_dimensions'] = ($dimensions) ? drupal_render($vars['content']['dimensions']) : '';
    if ($vars['fusion_uc_dimensions'] == '') {
      unset($vars['content']['dimensions']);
    }
    $list_price = !empty($vars['content']['list_price']) && $vars['content']['list_price'] > 0; // Hide list price if empty or zero
    $vars['fusion_uc_list_price'] = ($list_price) ? drupal_render($vars['content']['list_price']) : '';
    if ($vars['fusion_uc_list_price'] == '') {
      unset($vars['content']['list_price']);
    }
    $vars['fusion_uc_additional'] = drupal_render($vars['content']); // Render remaining fields
  }
}

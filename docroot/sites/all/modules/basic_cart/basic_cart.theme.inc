<?php
/**
 * @file
 * Basic cart main theme function wrapper. 
 */

module_load_include('inc', 'basic_cart', 'basic_cart.cart');

/**
 * Theme function implementation.
 */
function basic_cart_price($vars) {
  $price = $vars['price'];
  $html = '<span class="basic-cart-price">' . $price . '</span>';
  return $html;
}

/**
 * Theme function implementation.
 */
function basic_cart_add_to_cart_button($vars) {
  $path = $vars['path'];
  $css_class = $vars['css_class'];
  $options = array(
    'attributes' => array(
      'class' => array($css_class)
    )
  );
  return '<div class="basic-cart-add-to-cart">' . l(t('Add to cart'), $path, $options) . '</div>';
}

/**
 * Theme implementation for rendering the total price.
 * 
 * @todo find a way to pass $price through the variables array.
 */
function theme_basic_cart_cart_total_price($variables) {
  $price = basic_cart_get_total_price();
  $total = basic_cart_price_format($price->total);
  
  // Building the HTML.
  $html  = '<div class="basic-cart-cart-total-price-contents row">';
  $html .= '  <div class="basic-cart-total-price cell">' . t('Total price') . ': <strong>' . $total . '</strong></div>';
  $html .= '</div>';
  
  $vat_is_enabled = (int) variable_get('basic_cart_vat_state');
  if (!empty ($vat_is_enabled) && $vat_is_enabled) {
    $vat_value = basic_cart_price_format($price->vat);
    $html .= '<div class="basic-cart-cart-total-vat-contents row">';
    $html .= '  <div class="basic-cart-total-vat cell">' . t('Total VAT') . ': <strong>' . $vat_value . '</strong></div>';
    $html .= '</div>';
  }
  
  return $html;
}

/**
 * Theme implementation for rendering the cart element.
 */
function theme_basic_cart_render_cart_element($variables) {
  // Element name and nid.
  $name = $variables['form']['#name'];
  $nid = (int) str_replace(array('cartcontents[', ']'), '', $name);
  if (empty($nid)) {
    return '';
  }
  // Delete image.
  $vars = array(
    'path' => base_path() . drupal_get_path('module', 'basic_cart') . '/images/delete2.png',
    'alt' => t('Remove from cart'),
    'title' => t('Remove from cart'),
    'attributes' => array('class' => 'basic-cart-delete-image-image'),
  );
  $delete_link = l(theme('image', $vars), 'cart/remove/' . $nid, array('html' => TRUE));
  // Getting the node for it's title and description.
  $node = basic_cart_get_cart($nid);
  // Node description.
  $desc = '';
  if (isset($node->basic_cart_node_description)) {
    $desc = drupal_strlen($node->basic_cart_node_description) > 50 ?
                truncate_utf8($node->basic_cart_node_description, 50) : $node->basic_cart_node_description;
  }
  // Price and currency.
  $unit_price = isset($node->basic_cart_unit_price) ? $node->basic_cart_unit_price : 0;
  $unit_price = basic_cart_price_format($unit_price);
  
  // Prefix.
  $prefix  = '<div class="basic-cart-cart-contents row">';
  $prefix .= '  <div class="basic-cart-delete-image cell">' . $delete_link . '</div>';
  $prefix .= '  <div class="basic-cart-cart-node-title cell">' . l($node->title, 'node/' . $nid) . '<br />';
  $prefix .= '    <span class="basic-cart-cart-node-summary">' . $desc . '</span>';
  $prefix .= '  </div>';
  $prefix .= '  <div class="cell basic-cart-cart-unit-price"><strong>' . $unit_price . '</strong></div>';
  $prefix .= '  <div class="cell basic-cart-cart-x">x</div>';
  $prefix .= '  <div class="basic-cart-cart-quantity cell">';
  $prefix .= '    <div class="cell">';
  // Suffix.
  $suffix  = '    </div>';
  $suffix .= '  </div>';
  $suffix .= '</div>';
  
  // Rendering the element as textfield.
  $quantity = theme('textfield', $variables['form']);
  // Full view return.
  return $prefix . $quantity . $suffix;
}

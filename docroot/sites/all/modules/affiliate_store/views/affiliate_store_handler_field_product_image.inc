<?php

/**
 * @file
 * Views field handler for affiliate store product image.
 */

/**
 * Field handler for affiliate store product image.
 *
 * Convert a product image URL into HTML image element.
 *
 * @ingroup views_field_handlers
 */
class affiliate_store_handler_field_product_image extends views_handler_field {
  /**
   * Render a product image URL into HTML image element.
   */
  function render($values) {
    // Use product title as image ALT attribute.
    $url = $this->get_value($values);
    if (!$url) {
      return '';
    }

    $title = !empty($this->view->field['title']) ?
      $values->{$this->view->field['title']->field_alias} : '';
    $thumb_url = !empty($this->view->field['thumb_url']) ?
      $values->{$this->view->field['thumb_url']->field_alias} : '';
    $image_url = !empty($this->view->field['image_url']) ?
      $values->{$this->view->field['image_url']->field_alias} : '';
    $options = array('path' => $url, 'alt' => $title, 'title' => $title);
    $image = theme('image', $options);

    // Link to full image if available. The full image field must also be
    // selected in the view.
    if (($url === $thumb_url) && $image_url) {
      $options = array(
        'element' => $image,
        'url' => $image_url,
        'title' => $title,
      );
      return theme('affiliate_store_image', $options);
    }
    return $image;
  }
}

<?php
/**
 * @file
 * Contains the basic shopify product field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a product.
 * Definition terms:
 * - link_to_product default: Should this field have the checkbox "link to product" enabled by default.
 *
 */
class views_handler_field_shopify_product extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    // Don't add the additional fields to groupby
    if (!empty($this->options['link_to_product'])) {
      $this->additional_fields['id'] = array(
        'table' => 'shopify_products',
        'field' => 'id'
      );
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_product'] = array(
      'default' => isset($this->definition['link_to_product default']) ? $this->definition['link_to_product default'] : FALSE,
      'bool' => TRUE
    );
    return $options;
  }

  /**
   * Provide link to product option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_product'] = array(
      '#title' => t('Link this field to the product'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_product']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_product']) && !empty($this->additional_fields['id'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = "product/" . $this->get_value($values, 'id');
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}

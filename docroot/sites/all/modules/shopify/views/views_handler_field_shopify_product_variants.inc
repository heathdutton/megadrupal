<?php
/**
 * @file
 * Defines the variants handler class.
 */

/**
 * Class views_handler_field_shopify_product_variants
 *
 * Provides a custom views field for displaying child variants.
 */
class views_handler_field_shopify_product_variants extends views_handler_field_entity {

  public function option_definition() {
    $options = parent::option_definition();
    $options['view_mode'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $info = entity_get_info('shopify_product');
    $form['view_mode'] = array(
      '#title' => t('Variants view mode'),
      '#type' => 'select',
      '#options' => array_combine(array_keys($info['view modes']), array_keys($info['view modes'])),
      '#default_value' => isset($this->options['view_mode']) ? $this->options['view_mode'] : 'full',
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Renders variants for display.
   */
  function render($data) {
    if ($entity = $this->get_value($data)) {
      if (isset($entity->variants) && $entity->variants) {
        return entity_view('shopify_product', $entity->variants, $this->options['view_mode']);
      }
    }
  }

}

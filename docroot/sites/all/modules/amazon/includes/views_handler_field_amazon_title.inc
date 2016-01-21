<?php

class views_handler_field_amazon_title extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['link_format'] = array('default' => 'amazon');

    return $options;
  }

  /**
   * Override init function to provide generic option to link to node.
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    $this->additional_fields['detailpageurl'] = 'detailpageurl';
    $this->additional_fields['asin'] = 'asin';
  }

  /**
   * Provide link options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_format'] = array(
      '#title' => t('Link behavior'),
      '#type' => 'radios',
      '#options' => array(
        'plain' => t('No link'),
        'amazon' => t("A link to the product's Amazon page"),
      ),
      '#default_value' => $this->options['link_format'],
    );
    if (module_exists('amazon_store')) {
      $form['link_format']['#options']['amazon_store'] = t("A link to the product's Amazon Store page (Amazon Store Module)");
    }

  }

  function render($values) {
		$value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value, 'xss'), $values);
  }

  function render_link($title, $values) {
    switch ($this->options['link_format']) {
      case 'plain':
        break;
      case 'amazon':
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = $this->sanitize_value($this->get_value($values, 'detailpageurl'), 'link');
        $this->options['alter']['html'] = TRUE;
        break;
      case 'amazon_store':
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['html'] = TRUE;
        $asin = $this->get_value($values, 'asin');
        $this->options['alter']['path'] = AMAZON_STORE_PATH . '/item/' . $asin;
        break;
    }

    return $title;
  }
}

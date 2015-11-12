<?php

/**
 * Field handler to present an 'approve' link for a given claim.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_nodeownership_approve_link extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  function render($values) {
    if ($id = $this->get_value($values)) {
      return $this->render_link($id, $values);
    }
  }

  function render_link($id, $values) {
    if (user_access('administer nodeownership')) {
      $claim = entity_load_single('claim', $id);

      // Return N/A for anonymous and already approved claims.
      if ($claim->uid == 0 || $claim->status != 0) {
        return 'N/A';
      }

      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/config/nodeownership/nodeclaim/approve/' . $claim->id;
      $this->options['alter']['link_class'] = 'nodeownership-decline';
      $this->options['alter']['rel'] = $id;

      $text = !empty($this->options['text']) ? $this->options['text'] : t('Approve');
      return $this->tokenize_value($text);
    }
  }

  function add_self_tokens(&$tokens, $item) {
    $tokens['[nodeownership-approve-path]'] = '';
    parent::add_self_tokens($tokens, $item);
  }
}


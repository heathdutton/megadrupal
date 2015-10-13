<?php
/**
 * @file
 * Definition of TaxonomyDeleteLinkViewsHandlerDeleteTermLink.
 *
 * @ingroup views_field_handlers
 */

/**
 * Provide taxonomy term delete link.
 */
class TaxonomyDeleteLinkViewsHandlerDeleteTermLink extends views_handler_field {
  /**
   * Construct a new field handler.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['tid'] = 'tid';
    $this->additional_fields['vid'] = 'vid';
    $this->additional_fields['vocabulary_machine_name'] = array(
      'table' => 'taxonomy_vocabulary',
      'field' => 'machine_name',
    );
  }

  /**
   * Default options form.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  /**
   * Creates the form item for the options added.
   */
  public function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Loads additional fields.
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Renders the field handler.
   */
  public function render($values) {
    // Check there is an actual value, as on a relationship there may not be.
    if ($tid = $this->get_value($values, 'tid')) {
      $term = new stdClass();
      $term->vid = $values->{$this->aliases['vid']};
      $term->vocabulary_machine_name = $values->{$this->aliases['vocabulary_machine_name']};
      if (taxonomy_delete_link_term_delete_access($term)) {
        $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
        $tid = $this->get_value($values, 'tid');
        return l($text, 'taxonomy/term/' . $tid . '/delete', array('query' => drupal_get_destination()));
      }
    }
  }

}

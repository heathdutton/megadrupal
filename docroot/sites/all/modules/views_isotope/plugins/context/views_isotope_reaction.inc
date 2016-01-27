<?php
/**
 * @file
 * Contains the isotope_reaction subclass.
 */

/**
 * Expose isotope configurations as context reactions.
 */
class isotope_reaction extends context_reaction {
  /**
   * Editor form.
   */
  public function editor_form($context) {
    $form = $this->options_form($context);
    return $form;
  }

  /**
   * Submit handler for editor form.
   */
  public function editor_form_submit($context, $values) {
    return $values;
  }

  /**
   * Configuration form for the Isotope context reaction.
   */
  public function options_form($context) {
    $values = $this->fetch_from_context($context);
    $form = array(
      '#tree' => TRUE,
      '#title' => t('Isotope'),
      'pre' => array(
        '#type' => 'item',
        '#title' => t('Isotope Configuration'),
        '#description' => t('You can manage these configurations at !link.', array('!link' => l(t('Configure Isotope'), 'admin/config/user-interface/isotope'))
        ),
      ),
      // Choose the config here.
      'config' => array(
        '#type' => 'select',
        '#options' => views_isotope_available_configs(),
        '#title' => t('Choose Isotope Configuration'),
        '#default_value' => isset($values['config']) ? $values['config'] : 'isotope_default_config',
      ),
    );
    return $form;
  }

  /**
   * Display a message to the user.
   */
  public function execute() {
    foreach ($this->get_contexts() as $k => $v) {
      if (!empty($v->reactions[$this->plugin]['config'])) {
        return $v->reactions[$this->plugin]['config'];
      }
    }
  }
}

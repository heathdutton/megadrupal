<?php

/**
 * Class context_toolbar_context_reaction_hide_toolbar
 */
class context_hide_toolbar_context_reaction_hide_toolbar extends context_reaction {

  /**
   * Create a simple options form for the context.
   */
  function options_form($context) {
    $settings = $this->fetch_from_context($context);
    return array(
      'enable' => array(
        '#type' => 'checkbox',
        '#default_value' => isset($settings['enable']) ? $settings['enable'] : '0',
        '#title' => t('Hide the admin toolbar.'),
      )
    );
  }

  /**
   * Check the settings and if we should hide the toolbar.
   */
  function execute() {
    $hide = FALSE;
    foreach ($this->get_contexts() as $context) {
      if ($context->reactions['context_hide_toolbar_hide']['enable'] == '1') {
        $hide = TRUE;
      }
    }
    return $hide;
  }
}

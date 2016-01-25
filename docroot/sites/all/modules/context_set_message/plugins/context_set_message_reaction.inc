<?php
/**
 * @file: Contains the context_set_message_reaction subclass.
 */

/**
 * Expose set_messages as context reactions.
 */
class context_set_message_reaction extends context_reaction {
  /**
   * Editor form.
   */
  function editor_form($context) {
    $form = $this->options_form($context);
    return $form;
  }

  /**
   * Submit handler for editor form.
   */
  function editor_form_submit($context, $values) {
    return $values;
  }

  /**
   * Allow admins to provide a section title, section subtitle and section class.
   */
  function options_form($context) {
    $values = $this->fetch_from_context($context);
    $form = array(
      '#tree' => TRUE,
      '#title' => t('Drupal Set Message'),
      'severity' => array(
        '#title' => t('Severity'),
        '#description' => t('Choose a severity for the message.'),
        '#type' => 'select',
        '#options' => array(
          'status' => 'Status',
          'warning' => 'Warning',
          'error' => 'Error',
        ),
        '#default_value' => isset($values['severity']) ? $values['severity'] : '',
      ),
      'message' => array(
        '#title' => t('Message'),
        '#type' => 'textarea',
        '#default_value' => isset($values['message']) ? $values['message'] : '',
        '#description' => t('The message to display to the user'),
       ),
      'message_extra' => array(
        '#type' => 'fieldset',
        '#title' => t('Tokens'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#description' => t('Depending on where this message is displayed, some Node and Term tokens may not always be available.'),
        'hide_empty' => array(
          '#type' => 'checkbox',
          '#title' => t('Hide empty tokens'),
          '#description' => t('Hide empty tokens before displaying the message to the user'),
          '#default_value' => isset($values['hide_empty']) ? $values['hide_empty'] : TRUE
        ),
        'message_tokens' => array(
          '#theme' => 'token_tree',
          '#token_types' => array('user', 'site', 'current-date', 'node', 'term'),
          '#global_types' => FALSE,
          '#click_insert' => TRUE
        ),
      )
    );
    return $form;
  }

  /**
   * Display a message to the user.
   */
  function execute() {
    foreach ($this->get_contexts() as $k => $v) {
      if (!empty($v->reactions[$this->plugin]['message'])) {
        $severity = isset($v->reactions[$this->plugin]['severity']) ? $v->reactions[$this->plugin]['severity'] : 'status';
        $message = isset($v->reactions[$this->plugin]['message']) ? $v->reactions[$this->plugin]['message'] : '';

        if(module_exists('token')) {
          // Prepare variables for token_replace().
          global $user;
          global $language;
  
          $account = user_load($user->uid);
          $node = ( arg(0) == 'node' && is_numeric(arg(1)) ) ? node_load(arg(1)) : NULL;
          $term = ( arg(0) == 'taxonomy' && is_numeric(arg(2)) ) ? taxonomy_term_load(arg(2)) : NULL;
  
          $token_data = array(
            'user' => $account,
            'node' => $node,
            'term' => $term
          );
  
          $token_options = array(
            'language' => $language,
            'clear' => isset($v->reactions[$this->plugin]['message_extra']['hide_empty']) ? $v->reactions[$this->plugin]['message_extra']['hide_empty'] : TRUE,
          );
          $message = token_replace($message, $token_data, $token_options);
        }

        // Set the message.
        drupal_set_message($message, $severity, TRUE);
      }
    }
  }
}

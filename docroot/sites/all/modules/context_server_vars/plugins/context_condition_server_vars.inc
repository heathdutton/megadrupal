<?php
/**
 * @file
 * Context condition plugin for Contextual server variables.
 */

class context_condition_server_vars extends context_condition {
  /**
   * Condition form.
   */
  function condition_form($context) {
    $form = array();
    $values = $context->conditions['server_vars']['values'];

    $form['variables'] = array(
      '#type' => 'container',
      '#prefix' => '<div id="context-server-vars-ajax-wrapper">',
      '#suffix' => '</div>',
      '#theme' => 'context_server_vars_form',
    );

    $form['variables']['remove'] = array();
    $variables = isset($values['variables']) ? $values['variables'] : array(array());
    foreach ($variables as $delta => $var) {
      if (is_numeric($delta)) {
        $form['variables'][$delta] = array(
          '#delta' => $delta,
          '#element_validate' => array('context_server_vars_options_form_element_validate'),
        );

        $form['variables'][$delta]['name'] = array(
          '#type' => 'textfield',
          '#title' => t('Variable'),
          '#default_value' => isset($var['name']) ? $var['name'] : '',
          '#size' => 32,
        );

        $form['variables'][$delta]['op'] = array(
          '#type' => 'select',
          '#title' => t('Operator'),
          '#options' => array(
            '==' => t('Equals'),
            '>' => t('Greater than'),
            '>=' => t('Greater than or equal to'),
            '<' => t('Less than'),
            '<=' => t('Less than or equal to'),
            'strpos' => t('Contains'),
            'preg_match' => t('Regular expression'),
          ),
          '#default_value' => isset($var['op']) ? $var['op'] : '',
        );

        $form['variables'][$delta]['negate'] = array(
          '#type' => 'checkbox',
          '#title' => t('Negate'),
          '#default_value' => isset($var['negate']) ? $var['negate'] : FALSE,
        );

        $form['variables'][$delta]['value'] = array(
          '#type' => 'textfield',
          '#title' => t('Value'),
          '#default_value' => isset($var['value']) ? $var['value'] : '',
        );

        $form['variables']['remove'][$delta] = array(
          '#type' => 'button',
          '#value' => t('Remove'),
          '#name' => $delta,
          '#ajax' => array(
            'callback' => 'context_server_vars_options_form_ajax',
            'wrapper' => 'context-server-vars-ajax-wrapper',
          ),
          '#limit_validation_errors' => TRUE,
        );
      }
    }

    $form['add'] = array(
      '#type' => 'button',
      '#value' => t('Add another'),
      '#ajax' => array(
        'callback' => 'context_server_vars_options_form_ajax',
        'wrapper' => 'context-server-vars-ajax-wrapper',
      ),
    );

    $form['and_or'] = array(
      '#type' => 'checkbox',
      '#title' => t('Match all'),
      '#default_value' => isset($values['and_or']) ? $values['and_or'] : '',
    );

    return $form;
  }

  /**
   * Condition form submit handler.
   */
  function condition_form_submit($values) {
    // AJAX handler for Add and Remove buttons.
    if (strstr(request_uri(), 'system/ajax')) {
      $form_state = array('submitted' => FALSE);
      $form_build_id = $_POST['form_build_id'];
      $form = form_get_cache($form_build_id, $form_state);

      $form_state['input'] = $_POST;
      $form_state['values'] = array();
      $form = form_builder($form['#form_id'], $form, $form_state);

      switch ($form_state['triggering_element']['#value']) {
        case t('Add another'):
          $values['variables'][] = array(
            'name' => '',
            'op' => '==',
            'value' => '',
            'negate' => FALSE,
          );
          break;

        case t('Remove'):
          unset($values['variables'][$form_state['triggering_element']['#name']]);
          break;
      }
    }

    // Cleanup.
    unset($values['add'], $values['variables']['remove']);

    return $values;
  }

  /**
   * Execute.
   */
  function execute() {
    foreach ($this->get_contexts() as $context) {
      if (isset($context->conditions['server_vars'])) {
        $values = $context->conditions['server_vars']['values'];
        $return = array();
        foreach ($values['variables'] as $delta => $var) {
          $var['name'] = drupal_strtoupper($var['name']);
          $return[$delta] = FALSE;
          if (isset($_SERVER[$var['name']])) {
            switch ($var['op']) {
              case '==':
                $return[$delta] = $_SERVER[$var['name']] == $var['value'];
                break;

              case '>':
                $return[$delta] = $_SERVER[$var['name']] > $var['value'];
                break;

              case '>=':
                $return[$delta] = $_SERVER[$var['name']] >= $var['value'];
                break;

              case '<':
                $return[$delta] = $_SERVER[$var['name']] < $var['value'];
                break;

              case '<=':
                $return[$delta] = $_SERVER[$var['name']] <= $var['value'];
                break;

              case 'strpos':
                $return[$delta] = strpos($_SERVER[$var['name']], $var['value']) !== FALSE;
                break;

              case 'preg_match':
                $return[$delta] = preg_match($var['value'], $_SERVER[$var['name']]);
                break;
            }
          }
          $return[$delta] = $var['negate'] ? !$return[$delta] : $return[$delta];
        }
        if (($values['and_or'] && !in_array(FALSE, $return)) || in_array(TRUE, $return)) {
          $this->condition_met($context);
        }
      }
    }
  }
}

/**
 * Form element validate callback for Context condition options form.
 */
function context_server_vars_options_form_element_validate($element, &$form_state, $form) {
  $values = $form_state['values']['conditions']['plugins']['server_vars']['values']['variables'][$element['#delta']];
  if ('preg_match' == $values['op']) {
    if (@preg_match($values['value'], NULL) === FALSE) {
      form_error($element['value'], t('Value is not a valid regular expression.'));
    }
  }
}

/**
 * AJAX callback for Context condition options form.
 */
function context_server_vars_options_form_ajax($form, $form_state) {
  return $form['conditions']['plugins']['server_vars']['values']['variables'];
}


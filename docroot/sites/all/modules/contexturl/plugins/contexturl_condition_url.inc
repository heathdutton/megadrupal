<?php

/**
 * @file
 * Adds url argument condition to Context
 */

/**
 * URL as a Context condition.
 */
class contexturl_condition_url extends context_condition {
  /**
   * condition_values function
   */
  function condition_values() {
    return array(1 => t('True'));
  }

  /**
   * condition_form function
   */
  function condition_form($context) {
    $form['#type'] = 'value';
    $form['#value'] = TRUE;
    return $form;
  }

  /**
   * condition_form_submit function
   */
  function condition_form_submit($values) {
    return array($values);
  }

  /**
   * options_form function
   */
  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    return array(
      'evaluationtype' => array(
        '#type' => 'select',
        '#title' => t('Select the function for URL pattern evaluation'),
        '#description' => t('Select the function for URL pattern evaluation.'),
        '#options' => array(
            'fnmatch' => t('fnmatch'),
            'preg_match' => t('preg_match'),
        ),
        '#default_value' => isset($defaults['evaluationtype']) ? $defaults['evaluationtype'] : 1,
      ),
      'pageurl' => array(
        '#type' => 'textarea',
        '#title' => t('Page URL'),
        '#description' => t('Enter the URL formulas. Put each formula on a separate line. You can use the * character (asterisk) and any other pattern of the PHP fnmatch function and the ~ character (tilde) to exlcude one or more URL.'),
        '#default_value' => isset($defaults['pageurl']) ? $defaults['pageurl'] : '',
      ),
    );
  }

  /**
   * execute function
   */
  function execute() {
    foreach (context_enabled_contexts() as $context) {
      $options = $this->fetch_from_context($context, 'options');
      if (!empty($options['pageurl'])) {

        $current_url = $_SERVER['HTTP_HOST'] . request_uri();
        $evaluationtype = explode("\n", isset($options['evaluationtype']) ? $options['evaluationtype'] : 1);
        $patterns = explode("\n", $options['pageurl']);

        $return = FALSE;
        foreach ($patterns as $pattern) {
          $pattern = trim($pattern);
          if ($pattern) {
            $negate = FALSE;
            if (strpos($pattern, '~') === 0) {
              $negate = TRUE;
              $pattern = substr($pattern, 1);
            }

            if ($evaluationtype == 'preg_match') {
              if (($negate && !preg_match($pattern, $current_url)) || (!$negate && preg_match($pattern, $current_url))) {
                $return = TRUE;
                break;
              }
            }
            else {
              if (($negate && !fnmatch($pattern, $current_url)) || (!$negate && fnmatch($pattern, $current_url))) {
                $return = TRUE;
                break;
              }
            }
          }
        }

        if ($return == TRUE) {
          $this->condition_met($context, $return);
        }
      }
    }
  }
}

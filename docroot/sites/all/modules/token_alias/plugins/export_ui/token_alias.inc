<?php

/**
 * @file
 * Export UI plugin definition.
 */
$plugin = array(
  'schema' => 'token_alias',
  'access' => 'administer token_alias',
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'token-alias',
    'menu title' => 'Token alias',
    'menu description' => 'Administer token aliases.',
  ),
  'title singular' => t('token alias'),
  'title singular proper' => t('Token alias'),
  'title plural' => t('token aliases'),
  'title plural proper' => t('Token aliases'),
  'form' => array(
    'settings' => 'token_alias_ctools_export_ui_form',
    'submit' => 'token_alias_ctools_export_ui_form_submit',
  ),
);

function token_alias_ctools_export_ui_form(&$form, &$form_state) {
  $token_info = token_get_info();
  $token_type_options = array();
  foreach ($token_info['types'] as $token_type => $type_info) {
    $token_type_options[$token_type] = $type_info['name'];
  }
  $form['info']['token_type'] = array(
    '#type' => 'select',
    '#description' => t('Select the token type for this alias.'),
    '#title' => t('Token type'),
    '#options' => $token_type_options,
    '#required' => TRUE,
    '#default_value' => $form_state['item']->token_type,
    '#ajax' => array(
      'callback' => 'token_alias_token_type_form_ajax',
      'wrapper' => 'token-alias-token-tree-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['info']['alias_type'] = array(
    '#type' => 'select',
    '#description' => t('Select the type for this alias.'),
    '#title' => t('Alias type'),
    '#options' => array(
      TOKEN_ALIAS_TEXT => t('Text'),
      TOKEN_ALIAS_CALC => t('Calculation'),
    ),
    '#default_value' => $form_state['item']->alias_type,
    '#required' => TRUE,
  );

  $functions = array(// built-in functions
    'sin', 'sinh', 'arcsin', 'asin', 'arcsinh', 'asinh',
    'cos', 'cosh', 'arccos', 'acos', 'arccosh', 'acosh',
    'tan', 'tanh', 'arctan', 'atan', 'arctanh', 'atanh',
    'pow', 'exp',
    'sqrt', 'abs', 'ln', 'log',
    'ceil', 'floor', 'round');
  drupal_alter('ctools_math_expression_functions', $functions);
  $form['info']['text'] = array(
    '#type' => 'textarea',
    '#description' => t('This field supports tokens. Enter the text or formula to replace this token with. Available functions for using calculation expressions are: @functions. Empty token values will be replaced with 0 in calculations.', array('@functions' => implode(', ', $functions))),
    '#title' => t('Text'),
    '#default_value' => $form_state['item']->text,
    '#required' => TRUE,
  );

  $token_types = 'all';
  if (isset($form_state['values']['token_type'])) {
    $token_types = array($form_state['values']['token_type']);
  }
  elseif (isset($form_state['item']->token_type)) {
    $token_types = array($form_state['item']->token_type);
  }
  $form['info']['token_tree'] = array(
    '#theme' => 'token_tree',
    '#dialog' => TRUE,
    '#token_types' => $token_types,
    '#prefix' => '<div id="token-alias-token-tree-wrapper">',
    '#suffix' => '</div>',
  );

  $form['info']['round'] = array(
    '#type' => 'select',
    '#default_value' => $form_state['item']->round,
    '#title' => t('Round to decimals'),
    '#options' => array(-1 => t('Do not round')) + range(0, 10),
    '#description' => t('Select the number of decimals to round to.'),
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="alias_type"]' => array('value' => TOKEN_ALIAS_CALC),
      ),
    ),
  );

  $form['info']['decimal_separator'] = array(
    '#type' => 'select',
    '#default_value' => $form_state['item']->decimal_separator,
    '#title' => t('Decimal separator'),
    '#options' => array(
      '.' => t('Decimal point'),
      ',' => t('Decimal comma'),
    ),
    '#description' => t('Select the decimal separator.'),
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="alias_type"]' => array('value' => TOKEN_ALIAS_CALC),
      ),
    ),
  );

  $form['info']['thousand_separator'] = array(
    '#type' => 'select',
    '#default_value' => $form_state['item']->thousand_separator == ' ' ? 'space' : $form_state['item']->thousand_separator,
    '#title' => t('Thousand separator'),
    '#options' => array(
      '.' => t('Point'),
      ',' => t('Comma'),
      'space' => t('Space'),
    ),
    '#description' => t('Select the thousand separator.'),
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="alias_type"]' => array('value' => TOKEN_ALIAS_CALC),
      ),
    ),
  );
}

function token_alias_token_type_form_ajax($form, &$form_state) {
  return $form['info']['token_tree'];
}

function token_alias_ctools_export_ui_form_submit(&$form, &$form_state) {
  if ($form_state['values']['thousand_separator'] == 'space') {
    $form_state['values']['thousand_separator'] = ' ';
  }
  token_clear_cache();
}

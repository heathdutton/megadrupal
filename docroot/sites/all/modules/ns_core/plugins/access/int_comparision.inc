<?php

/**
 * @file
 * Plugin to provide access control/visibility based on specified context string matching user-specified string
 */

$plugin = array(
  'title' => t("Integer: comparison"),
  'description' => t('Control access by string match.'),
  'callback' => 'ns_core_int_comparison_ctools_access_check',
  'settings form' => 'ns_core_int_comparison_ctools_access_settings',
  'summary' => 'ns_core_int_comparison_ctools_access_summary',
  'defaults' => array('operator' => '=', 'value1' => 0, 'value2' => 0, 'substitute' => 0),
  'all contexts' => TRUE,
);

/**
 * Settings form
 */
function ns_core_int_comparison_ctools_access_settings($form, &$form_state, $conf) {
  $form['settings']['operator'] = array(
    '#type' => 'radios',
    '#title' => t('Operator'),
    '#options' => array(
      '=' => t('Equal'),
      '!=' => t('Not equal'),
      '<' => t('Less than'),
      '>' => t('Bigger than'),
    ),
    '#default_value' => $conf['operator'],
    '#description' => t('If using a regular expression, you should enclose the pattern in slashes like so: <em>/foo/</em>. If you need to compare against slashes you can use another character to enclose the pattern, such as @. See <a href="http://www.php.net/manual/en/reference.pcre.pattern.syntax.php">PHP regex documentation</a> for more.'),
  );
  
  $form['settings']['value1'] = array(
    '#type' => 'textfield',
    '#title' => t('Field integer to compare'),
    '#default_value' => $conf['value1'],
  );

  $form['settings']['value2'] = array(
    '#type' => 'textfield',
    '#title' => t('Value to compare field with'),
    '#default_value' => $conf['value2'],
  );

  if (!empty($form_state['display']->context)) {
    // Set extended description if both CCK and Token modules are enabled, notifying of unlisted keywords
    if (module_exists('content') && module_exists('token')) {
      $description = t('If checked, context keywords will be substituted in this content. Note that CCK fields may be used as keywords using patterns like <em>%node:field_name-formatted</em>.');
    }
    elseif (!module_exists('token')) {
      $description = t('If checked, context keywords will be substituted in this content. More keywords will be available if you install the Token module, see http://drupal.org/project/token.');
    }
    else {
      $description = t('If checked, context keywords will be substituted in this content.');
    }
    $form['settings']['substitute'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use context keywords'),
      '#description' => $description,
      '#default_value' => !empty($conf['substitute']),
    );
    $form['contexts'] = array(
      '#title' => t('Substitutions'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $rows = array();
    foreach ($form_state['display']->context as $context) {
      foreach (ctools_context_get_converters('%' . check_plain($context->keyword) . ':', $context) as $keyword => $title) {
        $rows[] = array(
          check_plain($keyword),
          t('@identifier: @title', array('@title' => $title, '@identifier' => $context->identifier)),
        );
      }
    }
    $header = array(t('Keyword'), t('Value'));
    $form['contexts']['context'] = array('#markup' => theme('table', array('header' => $header, 'rows' => $rows)));
  }
  return $form;
}

/**
 * Check for access
 */
function ns_core_int_comparison_ctools_access_check($conf, $contexts) {
  $value1 = $conf['value1'];
  $value2 = $conf['value2'];
  $args = func_get_args();
  if ($conf['substitute'] && !empty($contexts)) {
    $value1 = ctools_context_keyword_substitute($value1, array(), $contexts);
    $value2 = ctools_context_keyword_substitute($value2, array(), $contexts);
  }
  switch ($conf['operator']) {
    case '=':
      return $value1 === $value2;
    case '!=':
      return $value1 !== $value2;
    case '<':
      return $value1 < $value2;
    case '>':
      return $value1 > $value2;
  }
}

/**
 * Provide a summary description based upon the specified context
 */
function ns_core_int_comparison_ctools_access_summary($conf, $context) {
  $values = array('@value1' => $conf['value1'], '@value2' => $conf['value2']);
  switch ($conf['operator']) {
    case '=':
      return t('@value1 is @value2', $values);
    case '!=':
      return t('@value1 is not @value2', $values);
    case '<':
      return t('@value1 is less than @value2', $values);
    case '>':
      return t('@value1 is greater than @value2', $values);
  }
}
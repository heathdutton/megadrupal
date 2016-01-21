<?php
/**
 * @file
 * Provides the BeautyTips Form Errors administrative interface.
 */

/**
 * Build settings form.
 */
function bt_form_errors_settings() {
  $form = array();

  $form['info'] = array(
    '#markup' => t('<strong>Note:</strong> This module needs the option "Add beautytips js to every page" to be enabled on the <a href="!btconfpage">BeautyTips configuration page</a>.', array('!btconfpage' => url('admin/config/user-interface/beautytips'))),
  );

  $settings = bt_form_errors_get_settings();

  $styles = beautytips_get_styles(TRUE);
  if (count($styles)) {
    unset($styles['default']);
    foreach ($styles as $name => $style) {
      $bt_style_options[$name]['cssSelect'] = '#edit-beautytips-default-style-' . str_replace('_', '-', $name);
      $bt_style_options[$name]['text'] = 'Aenean risus purus, pharetra in, blandit quis, gravida a, turpis.  Aenean risus purus, pharetra in, blandit quis, gravida a, turpis.  Aenean risus purus, pharetra in, blandit quis, gravida a, turpis.';
      $bt_style_options[$name]['width'] = isset($style['width']) ? $style['width'] : '300px';
      $bt_style_options[$name]['style'] = $name;
      $style_options[$name] = $name;
    }
  }
  $form['bt_style'] = array(
    '#type' => 'radios',
    '#title' => t('Choose error tips style'),
    '#options' => $style_options,
    '#default_value' => $settings['bt_style'],
  );

  $options = array(
    BT_FORM_ERRORS_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BT_FORM_ERRORS_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %node-wildcard for every node edit form. %front is the front page.", array('%node-wildcard' => 'node/*/edit', '%front' => '<front>'));

  $access = user_access('use PHP for settings');
  if (module_exists('php') && $access) {
    $options += array(BT_FORM_ERRORS_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
    $title = t('Pages or PHP code');
    $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  else {
    $title = t('Pages');
  }
  $form['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show BeautyTips Form Errors on specific pages'),
    '#options' => $options,
    '#default_value' => isset($settings['visibility']) ? $settings['visibility'] : BT_FORM_ERRORS_VISIBILITY_NOTLISTED,
  );
  $form['pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . $title . '</span>',
    '#default_value' => isset($settings['pages']) ? $settings['pages'] : '',
    '#description' => $description,
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Tooltips settings form submit.
 */
function bt_form_errors_settings_submit($form, &$form_state) {
  $settings = array(
    'bt_style' => $form_state['values']['bt_style'],
    'visibility' => $form_state['values']['visibility'],
    'pages' => $form_state['values']['pages'],
  );

  variable_set('bt_form_errors', $settings);

  drupal_set_message(t('The configuration options have been saved.'));
}

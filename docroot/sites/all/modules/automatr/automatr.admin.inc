<?php

/**
 * @file
 * Admin page callback file for the Volacci Automatr (TM) module.
 */

/**
 * Page callback: Form constructor for the administrative settings form.
 *
 * @see automatr_admin_settings_form_validate()
 * @see automatr_menu()
 *
 * @ingroup forms
 */
function automatr_admin_settings_form($form, &$form_state) {
  $form = array();

  // Site link.
  $description = theme('image', array('path' => drupal_get_path('module', 'automatr') . '/logo.png'));
  $description .= t('<p>Manage your account on the <a href="@website">Volacci Automatr&trade;</a> website. Get <a href="@support_site">Automatr&trade; support</a>.</p>', array(
    '@website' => AUTOMATR_WEBSITE,
    '@support_site' => AUTOMATR_SUPPORT_WEBSITE,
  ));
  $form['description'] = array(
    '#markup' => $description,
    '#type' => 'item',
  );

  // Automatr (TM) implementation code.
  $form['site_info'] = array(
    '#collapsed' => automatr_is_valid_implementation_code(variable_get('automatr_implementation_code', '')),
    '#collapsible' => TRUE,
    '#description' => t('Please paste the entire <em>Automatr&trade; Implementation Code</em> which can be found under <em>Preferences</em> &gt; <em>Info</em> in your <a href="@url">Volacci Automatr&trade;</a> account.', array(
      '@url' => AUTOMATR_WEBSITE_ACCOUNT_PAGE,
    )),
    '#title' => t('Automatr&trade; implementation code'),
    '#type' => 'fieldset',
  );
  $form['site_info']['automatr_implementation_code'] = array(
    '#default_value' => variable_get('automatr_implementation_code', ''),
    '#required' => TRUE,
    '#size' => 10,
    '#title' => t('Automatr&trade; implementation code'),
    '#type' => 'textarea',
  );

  // Implementation scope.
  $form['implementation_title'] = array(
    '#type' => 'item',
    '#title' => t('Implementation scope'),
  );
  $form['implementation'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'automatr') . '/automatr.admin.js'),
    ),
  );

  // Per-page implementation.
  $form['implementation']['pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
  );
  $options = array(
    AUTOMATR_IMPLEMENTATION_PAGES_NOT_LISTED => t('All pages except those listed'),
    AUTOMATR_IMPLEMENTATION_PAGES_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
    '%blog' => 'blog',
    '%blog-wildcard' => 'blog/*',
    '%front' => '<front>',
  ));
  $title = t('Pages');
  if (module_exists('php') && user_access('use PHP for settings')) {
    $options += array(AUTOMATR_IMPLEMENTATION_PAGES_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
    $title = t('Pages or PHP code');
    $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array(
      '%php' => '<?php ?>',
    ));
  }
  $form['implementation']['pages']['automatr_implementation_pages_type'] = array(
    '#type' => 'radios',
    '#title' => t('Add Automatr&trade; implementation code to specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('automatr_implementation_pages_type', AUTOMATR_IMPLEMENTATION_PAGES_NOT_LISTED),
  );
  $form['implementation']['pages']['automatr_implementation_pages_pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . $title . '</span>',
    '#default_value' => variable_get('automatr_implementation_pages_pages', AUTOMATR_IMPLEMENTATION_PAGES_DEFAULT_PAGES),
    '#description' => $description,
  );

  // Per-role implementation.
  $form['implementation']['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );
  $form['implementation']['roles']['automatr_implementation_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Add Automatr&trade; implementation code for specific roles'),
    '#default_value' => variable_get('automatr_implementation_roles', array()),
    '#options' => array_map('check_plain', user_roles()),
    '#description' => t('Add Automatr&trade; implementation code only for the selected role(s). If you select no roles, Automatr&trade; implementation code will be added for all users.'),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for automatr_admin_settings_form().
 */
function automatr_admin_settings_form_validate($form, &$form_state) {
  $form_state['values']['automatr_implementation_code'] = trim($form_state['values']['automatr_implementation_code']);

  if (!automatr_is_valid_implementation_code($form_state['values']['automatr_implementation_code'])) {
    form_set_error('automatr_implementation_code', t('The Automatr&trade; implementation code is invalid.'));
  }
}

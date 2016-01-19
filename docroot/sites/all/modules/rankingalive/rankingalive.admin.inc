<?php

/**
 * @file
 * Administrative page callbacks for rankingalive module.
 */

/**
 * Admin settings form.
 *
 * @see rankingalive_admin_settings_form_validate()
 */
function rankingalive_admin_settings_form($form_state) {

  // Account settings.
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['account']['rankingalive_id'] = array(
    '#title' => t('Ranking Alive ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('rankingalive_id', ''),
    '#size' => 60,
    '#maxlength' => 40,
    '#required' => TRUE,
    '#description' => t('This ID is unique to each site you want to track. You have to <a href="@register">create an account and register your site</a> before using it, or if you already have registered your site, go to <a href="@list">your list of managed websites</a> and click on the "Show tag" button to access to your ID. You should have something similar to this at line 2 : <pre>var subs_id= \'XXXXXXXXXX\';</pre> Don\'t worry about all this code, you just need to copy the key between quotes and paste it in this field above', array('@register' => 'https://signup.rankingalive.com/', '@list' => 'http://report.rankingalive.com/subscribers')),
  );

  // Pages settings.
  $php_access = user_access('use PHP for rankingalive tracking visibility');
  $visibility = variable_get('rankingalive_visibility_pages', 0);
  $pages      = variable_get('rankingalive_pages', RANKINGALIVE_PAGES);

  // Hide Pages settings fieldsest if set to PHP mode and if the user hasn't the
  // permission to add php code.
  if ($visibility == 2 && !$php_access) {
    $form['page_settings'] = array();
    $form['page_settings']['rankingalive_visibility_pages'] = array(
      '#type' => 'value',
      '#value' => 2,
    );
    $form['page_settings']['rankingalive_pages'] = array(
      '#type' => 'value',
      '#value' => $pages,
    );
  }
  else {
    $options = array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    );

    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
      array(
        '%blog' => 'blog',
        '%blog-wildcard' => 'blog/*',
        '%front' => '<front>',
      )
    );

    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }

    $form['page_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Pages tracking settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['page_settings']['rankingalive_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Add tracking to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_settings']['rankingalive_pages'] = array(
      '#type' => 'textarea',
      '#title' => $title,
      '#title_display' => 'invisible',
      '#default_value' => $pages,
      '#description' => $description,
      '#rows' => 10,
    );
  }

  // Advanced feature configurations.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['rankingalive_js_scope'] = array(
    '#type' => 'select',
    '#title' => t('JavaScript scope'),
    '#description' => t('Ranking Alive recommends adding the JavaScript to the footer.'),
    '#options' => array(
      'footer' => t('Footer'),
      'header' => t('Header'),
    ),
    '#default_value' => variable_get('rankingalive_js_scope', 'footer'),
  );

  return system_settings_form($form);
}

/**
 * Implements _form_validate().
 */
function rankingalive_admin_settings_form_validate($form, &$form_state) {
  // Trim some text values.
  $form_state['values']['rankingalive_id'] = trim($form_state['values']['rankingalive_id']);
  $form_state['values']['rankingalive_pages'] = trim($form_state['values']['rankingalive_pages']);
}

<?php

/**
 * @file
 * SplashR admin functions.
 */

/**
 * SplashR admin settings form.
 */
function splashr_admin_form() {
  drupal_add_js(drupal_get_path('module', 'splashr') . '/js/splashr.admin.js');
  $form = array();

  $form['settings_title'] = array(
    '#type' => 'item',
    '#title' => t('Settings'),
  );
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // General settings.
  $form['settings']['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
  );
  $form['settings']['general']['splashr_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable splash'),
    '#default_value' => variable_get('splashr_enabled'),
    '#description' => t("Enable splash feature on the site."),
  );
  $splash_type = variable_get('splashr_type', 'flash');
  $form['settings']['general']['splashr_type'] = array(
    '#type' => 'select',
    '#title' => t('Splash content type'),
    '#default_value' => $splash_type,
    '#options' => array(
      'flash' => t('Flash'),
      'image' => t('Image background'),
      'html' => t('Html'),
    ),
    '#description' => t("Type of splash content to use."),
  );

  // Splash position.
  $form['settings']['position'] = array(
    '#type' => 'fieldset',
    '#title' => t('Position'),
  );
  $form['settings']['position']['splashr_position'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#options' => drupal_map_assoc(array('absolute', 'fixed')),
    '#default_value' => variable_get('splashr_position', 'absolute'),
    '#description' => t('CSS positioning to use for displaying splash div.'),
  );
  $form['settings']['position']['splashr_offset_top'] = array(
    '#type' => 'textfield',
    '#title' => t('Top offset'),
    '#default_value' => variable_get('splashr_offset_top'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#attributes' => array('style' => 'width:50px'),
    '#field_suffix' => 'px',
    '#description' => t('CSS top offset to use for displaying splash div.'),
  );

  // Cookie settings.
  $form['settings']['cookie'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie'),
  );
  $form['settings']['cookie']['splashr_cookie_disable'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't use cookie"),
    '#default_value' => variable_get('splashr_cookie_disable'),
  );
  $form['settings']['cookie']['splashr_cookie_validity'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie validity'),
    '#default_value' => variable_get('splashr_cookie_validity'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#attributes' => array('style' => 'width:50px'),
    '#field_suffix' => t('days'),
    '#states' => array(
      'invisible' => array(
        ':input[name="splashr_cookie_disable"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Overlay settings.
  $form['settings']['overlay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overlay'),
  );
  $form['settings']['overlay']['splashr_overlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show overlay'),
    '#default_value' => variable_get('splashr_overlay'),
  );
  $form['settings']['overlay']['splashr_overlay_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Overlay opacity'),
    '#default_value' => variable_get('splashr_overlay_opacity'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#attributes' => array('style' => 'width:50px'),
    '#field_suffix' => '%',
    '#states' => array(
      'visible' => array(
        ':input[name="splashr_overlay"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Dismiss settings.
  $form['settings']['dismiss'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dismiss'),
  );
  $form['settings']['dismiss']['splashr_dismiss_click'] = array(
    '#type' => 'checkbox',
    '#title' => t('Dismiss on click'),
    '#default_value' => variable_get('splashr_dismiss_click'),
    '#description' => t('Hide splash on mouse click anywhere within its div.')
  );
  $form['settings']['dismiss']['splashr_dismiss_timeout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Dismiss on timeout'),
    '#default_value' => variable_get('splashr_dismiss_timeout'),
    '#description' => t('Hide splash after specific time has passed.'),
  );
  $form['settings']['dismiss']['splashr_dismiss_timeout_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#default_value' => variable_get('splashr_dismiss_timeout_time'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#attributes' => array('style' => 'width:50px'),
    '#field_suffix' => t('seconds'),
    '#states' => array(
      'visible' => array(
        ':input[name="splashr_dismiss_timeout"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings']['dismiss']['splashr_dismiss_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add dismiss link'),
    '#default_value' => variable_get('splashr_dismiss_link'),
    '#description' => t('Adds extra dismiss link with configurable text to the splash content.'),
  );
  $form['settings']['dismiss']['splashr_dismiss_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Dismiss link text'),
    '#default_value' => variable_get('splashr_dismiss_link_text'),
    '#description' => t('Text to be used for dimiss link.'),
    '#states' => array(
      'visible' => array(
        ':input[name="splashr_dismiss_link"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Content.
  $form['splash_type_title'] = array(
    '#type' => 'item',
    '#title' => t('Content'),
  );
  $form['splash_type'] = array(
    '#type' => 'vertical_tabs',
  );

  // Flash content.
  $form['splash_type']['flash_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flash'),
  );
  $form['splash_type']['flash_content']['splashr_flash_path'] = array(
    '#type' => 'textfield',
    '#title' => t('SWF file path'),
    '#default_value' => variable_get('splashr_flash_path'),
    '#size' => 60,
    '#description' => t('Use a path relative to document root and don\'t add a leading slash.'),
  );
  $form['splash_type']['flash_content']['splashr_flash_width'] = array(
    '#type' => 'textfield',
    '#title' => t('SWF width'),
    '#default_value' => variable_get('splashr_flash_width'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#attributes' => array('style' => 'width:50px'),
    '#field_suffix' => 'px',
    '#description' => t('Display width of Flash animation object in pixels.'),
  );
  $form['splash_type']['flash_content']['splashr_flash_height'] = array(
    '#type' => 'textfield',
    '#title' => t('SWF height'),
    '#default_value' => variable_get('splashr_flash_height'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#attributes' => array('style' => 'width:50px'),
    '#field_suffix' => 'px',
    '#description' => t('Display height of Flash animation object in pixels.'),
  );

  // Image background content.
  $form['splash_type']['image_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image background'),
  );
  $form['splash_type']['image_content']['splashr_image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path'),
    '#default_value' => variable_get('splashr_image_path'),
    '#size' => 60,
    '#description' => t('Use a path relative to document root and don\'t add a leading slash.'),
  );

  // HTML content..
  $form['splash_type']['html_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML'),
  );
  $form['splash_type']['html_content']['splashr_html'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML'),
    '#default_value' => variable_get('splashr_html'),
  );

  $php_access = user_access('use PHP for tracking visibility');
  $visibility = variable_get('splashr_visibility_pages', 0);
  $pages = variable_get('splashr_pages', '');

  // Visibility.
  $form['visibility_title'] = array(
    '#type' => 'item',
    '#title' => t('Visibility'),
  );
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
  );

  // Visiblity per page.
  $form['visibility']['visibility_pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
  );
  if ($visibility == 2 && !$php_access) {
    $form['visibility']['visibility_pages'] = array();
    $form['visibility']['visibility_pages']['splashr_visibility_pages'] = array('#type' => 'value', '#value' => 2);
    $form['visibility']['visibility_pages']['splashr_pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }

    $form['visibility']['visibility_pages']['splashr_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Show splash on specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['visibility']['visibility_pages']['splashr_pages'] = array(
      '#type' => 'textarea',
      '#title' => $title,
      '#title_display' => 'invisible',
      '#default_value' => $pages,
      '#description' => $description,
      '#rows' => 10,
    );
  }

  // Visibility per role.
  $form['visibility']['visibility_roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );
  $form['visibility']['visibility_roles']['splashr_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Show splash for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('splashr_visibility_roles', 0),
  );
  $role_options = array_map('check_plain', user_roles());
  $form['visibility']['visibility_roles']['splashr_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('splashr_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will see the splash. If a user has any of the roles checked, that user will see the splash (or not, depending on the setting above).'),
  );

  $form['#validate'][] = 'splashr_admin_form_validate';
  return system_settings_form($form);
}

function splashr_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!$values['splashr_dismiss_click'] && !$values['splashr_dismiss_timeout'] && !$values['splashr_dismiss_link']) {
    form_set_error('splashr_dismiss_timeout', t('You need to enable at least one way to dismiss the splash.'));
  }
}

<?php

/**
 * @file
 * Administration pages for Uservoice module.
 */

/**
 * Administration form.
 */
function uservoice_admin_form($form, &$form_state) {

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  // Uservoice subdomain.
  $form['account']['uservoice_subdomain'] = array(
    '#type' => 'textfield',
    '#title' => t("UserVoice Subdomain"),
    '#default_value' => variable_get('uservoice_subdomain', ''),
    '#description' => t("Your unique UserVoice subdomain. If your UserVoice web portal is available at http://mysubdomain.uservoice.com, your subodmain should be entered as 'mysubdomain' without 'http://' or any other additions."),
    '#required' => TRUE,
  );

  // Javascript API key.
  $form['account']['uservoice_js_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t("UserVoice Javascript API Key"),
    '#default_value' => variable_get('uservoice_js_api_key', ''),
    '#description' => t("Your unique UserVoice Javascript API key, found in your UserVoice admin console, under the 'Advanced Setup' portion of your <a href=\"https://app.uservoice.com/admin/settings/widgets/\">Widget Settings page</a>."),
    '#required' => TRUE,
  );

  // Uservoice SSO Key.
  $form['account']['uservoice_sso_key'] = array(
    '#type' => 'textfield',
    '#title' => t("UserVoice SSO Key"),
    '#default_value' => variable_get('uservoice_sso_key', ''),
    '#description' => t("Your unique UserVoice SSO key, found in your UserVoice admin console, under the 'User Authentication' portion of your <a href=\"https://app.uservoice.com/admin/settings/portal/\">Web Portal settings</a>."),
    '#required' => TRUE,
  );

  // ---------------------------------------------------------------------------
  // Widget settings.
  $form['widget_settings_title'] = array(
    '#type' => 'item',
    '#title' => t('Widget settings'),
  );
  $form['widget_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // Widget style tab.
  $form['widget_settings']['widget_style'] = array(
    '#type' => 'fieldset',
    '#title' => t('Widget style'),
  );

  $options = array(
    'new' => t('New'),
    'classic' => t('Classic'),
  );
  $form['widget_settings']['widget_style']['uservoice_widget_type'] = array(
    '#type' => 'radios',
    '#title' => t('Widget type'),
    '#default_value' => variable_get('uservoice_widget_type', USERVOICE_DEFAULT_WIDGET_TYPE),
    '#options' => $options,
    '#description' => t("Choose which type of UserVoice widget you want to use on this site. Visit <a href=\"https://app.uservoice.com/admin/settings/widgets/\">your account's Widgets settings page</a> for more information."),
  );

  // UserVoice contact form.
  $disabled = FALSE;
  if (!module_exists('contact')) {
    $disabled = TRUE;
  }
  $form['widget_settings']['widget_style']['uservoice_contact_form'] = array(
    '#type' => 'checkbox',
    '#title' => t("Replace Drupal's default site-wide contact form with embedded UserVoice classic widget?"),
    '#default_value' => variable_get('uservoice_contact_form', USERVOICE_DEFAULT_CONTACT_FORM),
    '#description' => t("For more information, visit <a href=\"https://app.uservoice.com/admin/settings/widgets/classic-widget-inline\">your account's Widgets settings</a>. Requires Drupal's core 'contact' module to be enabled."),
    '#disabled' => $disabled,
  );

  // ---------------------------------------------------------------------------
  // New widget settings.
  $form['widget_settings']['widget_style']['new_widget'] = array(
    '#type' => 'fieldset',
    '#title' => t('"New" widget style'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      // Show/hide widget style settings based on widget type chosen.
      // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_process_states/7
      'visible' => array(
        ':input[name="uservoice_widget_type"]' => array('value' => 'new'),
      ),
    ),
  );
  // Starting experience.
  $options = array(
    'contact' => t('Contact'),
    'satisfaction' => t('Satisfaction Rating'),
    'smartvote' => t('SmartVote'),
  );
  $form['widget_settings']['widget_style']['new_widget']['uservoice_new_widget_starting_experience'] = array(
    '#type' => 'radios',
    '#title' => t('Starting Experience'),
    '#default_value' => variable_get('uservoice_new_widget_starting_experience', USERVOICE_NEW_WIDGET_DEFAULT_STARTING_EXPERIENCE),
    '#options' => $options,
    '#description' => t('Choose what users will see when they first click on your widget icon.'),
  );

  // Trigger position.
  $options = array(
    'top-left' => t('Top-left'),
    'top-right' => t('Top-right'),
    'bottom-left' => t('Bottom-left'),
    'bottom-right' => t('Bottom-right'),
  );
  $form['widget_settings']['widget_style']['new_widget']['uservoice_new_widget_trigger_position'] = array(
    '#type' => 'radios',
    '#title' => t('Trigger Position'),
    '#default_value' => variable_get('uservoice_new_widget_trigger_position', USERVOICE_NEW_WIDGET_DEFAULT_TRIGGER_POSITION),
    '#options' => $options,
    '#description' => t('Choose where the widget icon will appear.'),
  );

  // Accent Color.
  $form['widget_settings']['widget_style']['new_widget']['uservoice_new_widget_accent_color'] = array(
    '#type' => 'textfield',
    '#title' => t("Accent Color"),
    '#default_value' => variable_get('uservoice_new_widget_accent_color', USERVOICE_NEW_WIDGET_DEFAULT_ACCENT_COLOR),
    '#description' => t("Accent color for buttons and link text within the widget. Should be a valid css color value."),
  );

  // Trigger Color.
  $form['widget_settings']['widget_style']['new_widget']['uservoice_new_widget_trigger_color'] = array(
    '#type' => 'textfield',
    '#title' => t("Trigger Color"),
    '#default_value' => variable_get('uservoice_new_widget_trigger_color', USERVOICE_NEW_WIDGET_DEFAULT_TRIGGER_COLOR),
    '#description' => t("Foreground color for the widget trigger icon. Should be a valid css color value."),
  );

  // Trigger Background Color.
  $form['widget_settings']['widget_style']['new_widget']['uservoice_new_widget_trigger_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t("Trigger Background Color"),
    '#default_value' => variable_get('uservoice_new_widget_trigger_background_color', USERVOICE_NEW_WIDGET_DEFAULT_TRIGGER_BACKGROUND_COLOR),
    '#description' => t("Background color for the widget trigger icon. Should be a valid css color value."),
  );

  // Autoprompt.
  $form['widget_settings']['widget_style']['new_widget']['uservoice_new_widget_autoprompt'] = array(
    '#type' => 'checkbox',
    '#title' => t("Autoprompt users occasionally for Satisfaction and SmartVote?"),
    '#default_value' => variable_get('uservoice_new_widget_autoprompt', USERVOICE_NEW_WIDGET_DEFAULT_AUTOPROMPT),
    '#description' => t("Autoprompts allow you to actively engage users by automatically popping up the Satisfaction and SmartVote widgets when certain conditions are met. Select which Autoprompts you want enabled via <a href=\"https://app.uservoice.com/admin/settings/widgets/\">your account's Widgets settings</a>."),
  );

  // Custom trigger.
  $form['widget_settings']['widget_style']['new_widget']['uservoice_new_widget_custom_triggers'] = array(
    '#type' => 'checkbox',
    '#title' => t("Scan DOM for custom triggers?"),
    '#default_value' => variable_get('uservoice_new_widget_custom_triggers', USERVOICE_NEW_WIDGET_DEFAULT_CUSTOM_TRIGGERS),
    '#description' => t("Selecting this option will make the UserVoice widget trigger for a click event on any DOM element with the followign classes: '.uservoice-contact-trigger', '.uservoice-satisfaction-trigger', '.uservoice-smartvote-trigger'; select the appropriate class depending on which widget mode you want to trigger. (Only select this option if you need it, as it will add some time to DOM processing.)"),
  );

  // ---------------------------------------------------------------------------
  // Classic widget settings.
  $form['widget_settings']['widget_style']['classic_widget'] = array(
    '#type' => 'fieldset',
    '#title' => t('"Classic" widget style'),
    '#description' => t('Some of these settings, such as color values, are also used for the embedded contact form, if that option is checked.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      // Show/hide widget style settings based on widget type chosen.
      // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_process_states/7
      'invisible' => array(
        ':input[name="uservoice_widget_type"]' => array('value' => 'new'),
        ':input[name="uservoice_contact_form"]' => array('checked' => FALSE),
      ),
    ),
  );
  // Widget mode.
  $options = array(
    'full' => t('Give feedback & Contact support'),
    'feedback' => t('Give feedback only'),
    'support' => t('Contact support only'),
  );
  $form['widget_settings']['widget_style']['classic_widget']['uservoice_classic_widget_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Widget Mode'),
    '#default_value' => variable_get('uservoice_classic_widget_mode', USERVOICE_CLASSIC_WIDGET_DEFAULT_MODE),
    '#options' => $options,
  );

  // Default tab selection.
  $options = array(
    'support' => t('Contact support'),
    'feedback' => t('Give feedback'),
  );
  $form['widget_settings']['widget_style']['classic_widget']['uservoice_classic_widget_default_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Default tab selection'),
    '#default_value' => variable_get('uservoice_classic_widget_default_mode', USERVOICE_CLASSIC_WIDGET_DEFAULT_TAB),
    '#options' => $options,
  );

  // Primary color.
  $form['widget_settings']['widget_style']['classic_widget']['uservoice_classic_widget_primary_color'] = array(
    '#type' => 'textfield',
    '#title' => t("Primary Color"),
    '#default_value' => variable_get('uservoice_classic_widget_primary_color', USERVOICE_CLASSIC_WIDGET_DEFAULT_PRIMARY_COLOR),
    '#description' => t("CSS color value. Affects the color of the widget."),
  );

  // Link color.
  $form['widget_settings']['widget_style']['classic_widget']['uservoice_classic_widget_link_color'] = array(
    '#type' => 'textfield',
    '#title' => t("Link color"),
    '#default_value' => variable_get('uservoice_classic_widget_link_color', USERVOICE_CLASSIC_WIDGET_DEFAULT_LINK_COLOR),
    '#description' => t("CSS color value. Affects the color of links in the widget."),
  );

  // Tab Label.
  $form['widget_settings']['widget_style']['classic_widget']['uservoice_classic_widget_tab_label'] = array(
    '#type' => 'textfield',
    '#title' => t("Tab label"),
    '#default_value' => variable_get('uservoice_classic_widget_tab_label', USERVOICE_CLASSIC_WIDGET_DEFAULT_TAB_LABEL),
    '#description' => t("Text to display on the widget tab."),
  );

  // Tab position.
  $options = array(
    'top-left' => t('Top-left'),
    'top-right' => t('Top-right'),
    'middle-left' => t('Middle-left'),
    'middle-right' => t('Middle-right'),
    'bottom-left' => t('Bottom-left'),
    'bottom-right' => t('Bottom-right'),
  );
  $form['widget_settings']['widget_style']['classic_widget']['uservoice_classic_widget_tab_position'] = array(
    '#type' => 'radios',
    '#title' => t('Tab position'),
    '#default_value' => variable_get('uservoice_classic_widget_tab_position', USERVOICE_CLASSIC_WIDGET_DEFAULT_TAB_POSITION),
    '#options' => $options,
    '#description' => t('Where the widget tab should appear on the page.'),
  );

  // Tab color.
  $form['widget_settings']['widget_style']['classic_widget']['uservoice_classic_widget_tab_color'] = array(
    '#type' => 'textfield',
    '#title' => t("Tab color"),
    '#default_value' => variable_get('uservoice_classic_widget_tab_color', USERVOICE_CLASSIC_WIDGET_DEFAULT_TAB_COLOR),
    '#description' => t("CSS color value. Affects the color of the tab."),
  );

  // Invert tab.
  $form['widget_settings']['widget_style']['classic_widget']['uservoice_classic_widget_tab_inverted'] = array(
    '#type' => 'checkbox',
    '#title' => t("Invert tab colors?"),
    '#default_value' => variable_get('uservoice_classic_widget_tab_inverted', USERVOICE_CLASSIC_WIDGET_DEFAULT_TAB_INVERTED),
    '#description' => t("Inverts the tab colors."),
  );

  // ---------------------------------------------------------------------------
  // Page specific visibility settings.
  $php_access = user_access('use PHP for settings');
  $visibility = variable_get('uservoice_widget_visibility_pages', 0);
  $pages = variable_get('uservoice_widget_pages', USERVOICE_WIDGET_PAGES);

  $form['widget_settings']['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if ($visibility == 2 && !$php_access) {
    $form['widget_settings']['page_vis_settings'] = array();
    $form['widget_settings']['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['widget_settings']['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
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
                       '%front' => '<front>')
                     );

    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }
    $form['widget_settings']['page_vis_settings']['uservoice_widget_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Add the UserVoice widget to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['widget_settings']['page_vis_settings']['uservoice_widget_pages'] = array(
      '#type' => 'textarea',
      '#title' => $title,
      '#title_display' => 'invisible',
      '#default_value' => $pages,
      '#description' => $description,
      '#rows' => 10,
    );
  }

  // ---------------------------------------------------------------------------
  // Role visilibity settings.
  $form['widget_settings']['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );

  $form['widget_settings']['role_vis_settings']['uservoice_widget_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add the widget for specific roles'),
    '#options' => array(
      t('Add for the selected roles only'),
      t('Add for every role except the selected ones'),
    ),
    '#default_value' => variable_get('uservoice_widget_visibility_roles', 0),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['widget_settings']['role_vis_settings']['uservoice_widget_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('uservoice_widget_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, the UserVoice widget will be added for all users. If a user has any of the roles checked, the widget will be added for that user (or excluded, depending on the setting above).'),
  );

  // ---------------------------------------------------------------------------
  // Caching
  $form['widget_settings']['caching'] = array(
    '#type' => 'fieldset',
    '#title' => t('Caching'),
  );

  $form['widget_settings']['caching']['uservoice_script_cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t("How long should the javascript for the UserVoice widgets be cached? (Time in seconds)"),
    '#default_value' => variable_get('uservoice_script_cache_time', USERVOICE_DEFAULT_SCRIPT_CACHE_TIME),
    '#description' => t("The UserVoice javascript is constructed and customized on a per-user basis during page build; as such, you can gain performance benefits by caching the generated script for each user. The cache for a given user is flushed during hook_user_update(), so in most cases it is safe to set a high value here. Set to 0 to disable script caching."),
    '#required' => TRUE,
  );

  // ---------------------------------------------------------------------------
  // Gadgets
  global $base_secure_url;
  $sso_key = variable_get('uservoice_sso_key', '');
  $user_profile_gadget_url = '';
  $order_history_gadget_url = '';
  if (!empty($sso_key)) {
    $user_profile_gadget_url = $base_secure_url . '/' . USERVOICE_USER_PROFILE_GADGET_PATH;
    $order_history_gadget_url = $base_secure_url . '/' . USERVOICE_ORDER_HISTORY_GADGET_PATH;
  }

  $form['gadgets'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gadgets'),
    '#description' => t("Custom gadgets can display relevant information about an individual user from your website when you view a ticket within the UserVoice admin console. To use these gadgets, copy the url shown, then visit your <a href=\"https://app.uservoice.com/admin/settings/gadgets/\">your account's gadgets integration page</a> and add a custom gadget using the copied url.<br/><br/>IMPORTANT: To use these gadgets, these urls must be accessible over SSL."),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'uservoice') . '/uservoice.admin.js',
        array(
          'data' => array(
            'userVoice' => array(
              'userProfileGadgetURL' => $user_profile_gadget_url,
              'orderHistoryGadgetURL' => $order_history_gadget_url,
            ),
          ),
          'type' => 'setting',
        ),
      ),
    ),
  );

  $form['gadgets']['uservoice_user_profile_gadget'] = array(
    '#type' => 'item',
    '#title' => t("User profile gadget"),
    '#markup' => '<span id="user-profile-gadget-url">' . $user_profile_gadget_url . '</span>',
    '#description' => t("This gadget displays user profile information for the given user."),
  );

  $form['gadgets']['uservoice_order_history_gadget'] = array(
    '#type' => 'item',
    '#title' => t("Order history gadget"),
    '#markup' => '<span id="order-history-gadget-url">' . $order_history_gadget_url . '</span>',
    '#description' => t("This gadget displays order history information for the given user. Currently integrates with Membership Suite (moneyscripts.net). You can write your own integration using hook_uservoice_order_history_alter(). See uservoice.api.php."),
  );

  // ---------------------------------------------------------------------------
  // Web Hooks.
  global $base_url;

  $form['webhooks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Web hooks'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['webhooks']['uservoice_webhooks'] = array(
    '#type' => 'checkbox',
    '#title' => t("Save UserVoice ticket information in your Drupal database?"),
    '#default_value' => variable_get('uservoice_save_webhooks', USERVOICE_DEFAULT_WEBHOOKS),
    '#description' => t("For instance, whenever someone creates a new UserVoice support request, the ticket information will be saved in your database in the uservoice_webhook table. Has no visible effect, but can be useful if you want to keep a record of which users created which tickets, for data analysis, etc."),
  );

  $form['webhooks']['uservoice_webhooks_info'] = array(
    '#type' => 'item',
    '#title' => t("Web hook URL"),
    '#markup' => '<span id="webhook-url">' . $base_url . '/' . USERVOICE_WEBHOOK_PATH . '</span>',
    '#description' => t("To enable this feature, you need to visit your <a href=\"https://app.uservoice.com/admin/settings/service-hooks/\">your account's service hooks page</a> and add a custom web hook using the above url. Content type should be 'JSON', and you should tick whichever notifications you want to be recorded."),
    '#states' => array(
      // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_process_states/7
      'visible' => array(
        ':input[name="uservoice_webhooks"]' => array('checked' => TRUE),
      ),
    ),
  );

  // ---------------------------------------------------------------------------
  // Validation and submit handlers.
  $form['#validate'][] = 'uservoice_admin_form_validate';
  $form['#submit'][] = 'uservoice_admin_form_submit';

  return system_settings_form($form);
}

/**
 * Form validation handler for UserVoice administration form.
 * @TODO: proper form validation.
 */
function uservoice_admin_form_validate($form, &$form_state) {
}


/**
 * Submit handler for UserVoice administration form.
 */
function uservoice_admin_form_submit($form, &$form_state) {
  // Flush UserVoice cache table, as relevant script/cache settings may have
  // changed.
  cache_clear_all('*', 'cache_uservoice_script', TRUE);

  // Rebuild menu tree, since contact form may have changed. Needs to be called
  // at shutdown because our work in hook_menu_alter depends on values set here.
  // @todo: only call this if 'replace contact form' setting has changed.
  // @see uservoice_menu_alter()
  drupal_register_shutdown_function('menu_rebuild');
}

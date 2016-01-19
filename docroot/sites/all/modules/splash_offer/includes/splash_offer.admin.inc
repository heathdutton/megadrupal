<?php
/**
 * @file
 * Administration page callbacks for the splash_offer module.
 *
 * @ingroup splash_offer
 * @{
 */

/**
 * Form builder for the splash_offer form.
 *
 * CRUD form for splash_offer entities
 *
 * @see splash_offer_form_validate()
 * @see splash_offer_form_submit()
 * @ingroup forms
 */
function splash_offer_form($form, &$form_state, $entity, $op, $bundle) {
  $form['#splash_offer'] = $entity;
  $entity_info = entity_get_info('splash_offer');

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'splash_offer') . '/splash_offer.js'
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name that displays in the admin interface.'),
    '#default_value' => isset($entity->name) ? $entity->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -20,
  );

  // Add the field related form elements.
  field_attach_form('splash_offer', $entity, $form, $form_state);

  $form['data'] = array(
    '#tree' => TRUE,
    '#weight' => 40,
  );

  $form['data']['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Buttons'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['data']['links']['mode'] = array(
    '#name' => '#mode',
    '#type' => 'radios',
    '#title' => t('Button Mode'),
    '#default_value' => $entity->links['#mode'],
    '#options' => array(
      SPLASH_OFFER_BUTTON_MODE_NONE  => t('No buttons'),
      SPLASH_OFFER_BUTTON_MODE_UI    => t('jQuery UI Buttons'),
      SPLASH_OFFER_BUTTON_MODE_HTML  => t('HTML Buttons (.tpl)')
    ),
  );

  $form['data']['links']['yes']['text'] = array(
    '#type' => 'textfield',
    '#description' => t('Text to appear on the accept button.'),
    '#title' => t('Accept Button'),
    '#default_value' => $entity->links['yes']['text'],
    '#size' => 25,
    '#states' => array(
      'invisible' => array(
        ':input[name="data[links][mode]"]' => array('value' => SPLASH_OFFER_BUTTON_MODE_NONE),
      ),
    ),
  );
  $form['data']['links']['yes']['path'] = array(
    '#type' => 'textfield',
    '#description' => t('Enter the URL to where the user will be sent if they click the accept button.', array('!label' => $entity_info['label'])),
    '#title' => t('Accept URL'),
    '#default_value' => $entity->links['yes']['path'],
    '#size' => 100,
    '#states' => array(
      'invisible' => array(
        ':input[name="data[links][mode]"]' => array('value' => SPLASH_OFFER_BUTTON_MODE_NONE),
      ),
    ),
  );
  $form['data']['links']['no']['text'] = array(
    '#type' => 'textfield',
    '#description' => t('Text to appear on the decline button. When the user clicks this button, the !label will disappear.', array('!label' => $entity_info['label'])),
    '#title' => t('Decline Button'),
    '#default_value' => $entity->links['no']['text'],
    '#size' => 25,
    '#states' => array(
      'invisible' => array(
        ':input[name="data[links][mode]"]' => array('value' => SPLASH_OFFER_BUTTON_MODE_NONE),
      ),
    ),
  );

  $form['data']['storage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Repeat Viewing'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['data']['storage']['cookies']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cookies?'),
    '#description' => t("Should this !label use cookies to know when it's been previously viewed by a user?", array('!label' => $entity_info['label'])),
    '#default_value' => $entity->storage['cookies']['#enabled'],
  );

  $form['data']['storage']['cookies']['fs_cookies'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced (cookies)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="data[storage][cookies][enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['data']['storage']['cookies']['fs_cookies']['lifetime'] = array(
    '#type' => 'textfield',
    '#description' => t('How many days before the cookie expires? Set to 0 (zero) to expire when the current browser session ends.'),
    '#title' => t('Cookie Lifespan in Days?'),
    '#default_value' => $entity->storage['cookies']['#lifetime'],
    '#required' => FALSE,
    '#size' => 10,
  );
  $form['data']['storage']['cookies']['fs_cookies']['default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set cookie by default?'),
    '#description' => t("When checked, the user will not have to check the <em>Don't show again</em> option; it will already be checked for them.  Uncheck here for the opposite to be true."),
    '#default_value' => $entity->storage['cookies']['#default'],
  );

  // Per-path visibility.
  $form['data']['audience'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages/Audience'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
  );

  $form['data']['audience']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Which roles should see this !label?', array('!label' => $entity_info['label'])),
    '#default_value' => array_keys($entity->access['roles']),
    '#options' => user_roles(),
  );

  $access = user_access('use PHP for settings');
  $options = array(
    BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  if (module_exists('php') && $access) {
    $options += array(BLOCK_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
    $title = t('Pages or PHP code');
    $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  else {
    $title = t('Pages');
  }
  $form['data']['audience']['pages']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show this !label on specific pages', array('!label' => $entity_info['label'])),
    '#options' => $options,
    '#default_value' => $entity->access['pages']['#visibility'],
  );
  $form['data']['audience']['pages'][0] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . $title . '</span>',
    '#default_value' => $entity->access['pages'][0],
    '#description' => $description,
  );

  if (module_exists('locale')) {
    $form['data']['audience']['locale'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Which locales should see this !label?', array('!label' => $entity_info['label'])),
      '#description' => t('Leave empty to show to all locales'),
      '#default_value' => $entity->access['locale'],
      '#options' => locale_language_list(),
    );
  }

  // Note this code is repeated in _splash_offer_page_visibility
  $auto_pages = splash_offer_get_auto_pages($entity, TRUE);
  $form['data']['audience']['pages'][1] = array(
    '#type' => 'textarea',
    '#title' => 'Also, never show on these pages',
    '#default_value' => $auto_pages,
    '#description' => t('These have been automatically defined for you and cannot be changed. See <code>hook_splash_offer_no_show_paths()</code>.'),
    '#disabled' => TRUE,
  );

  if (module_exists('mobile_detect')
      && ($detect = mobile_detect_get_object())) {
    $form['data']['devices'] = array(
      '#type' => 'fieldset',
      '#title' => t('Trigger Devices'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['data']['devices']['always_trigger'] = array(
      '#type' => 'checkbox',
      '#title' => t('Always trigger, regardless of device'),
      '#default_value' => !$entity->access['devices']['#enabled'],
    );
    $form['data']['devices']['fs_phones'] = array(
      '#type' => 'fieldset',
      '#title' => t('Phone Devices'),
      '#collapsible' => TRUE,
      '#collapsed' => !$entity->access['devices']['#enabled'],
    );
    $form['data']['devices']['fs_phones']['phones'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Which phones trigger this !label?', array('!label' => $entity_info['label'])),
      '#default_value' => (array) $entity->access['devices']['phones'],
      '#options' => drupal_map_assoc(array_keys($detect->getPhoneDevices())),
    );
    $form['data']['devices']['fs_tablets'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tablet Devices'),
      '#collapsible' => TRUE,
      '#collapsed' => !$entity->access['devices']['#enabled'],
    );
    $form['data']['devices']['fs_tablets']['tablets'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Which tablets trigger this !label?', array('!label' => $entity_info['label'])),
      '#default_value' => (array) $entity->access['devices']['tablets'],
      '#options' => drupal_map_assoc(array_keys($detect->getTabletDevices())),
    );
  };

  $form['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 50,
  );

  $form['other']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active'),
    '#description' => t('To disable a !label and hide it from users, uncheck this box.', array('!label' => $entity_info['label'])),
    '#default_value' => $entity->status,
  );

  $form['other']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#description' => t('In the case of multiple !labels, lower numbered !labels will be presented first.', array('!labels' => $entity_info['plural label'])),
    '#default_value' => $entity->weight,
    '#delta' => 20, //the range is from -1 * delta through delta
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  if (!empty($entity->is_new)) {
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create !entity', array('!entity' => $entity_info['label'])),
    );
  }
  else {
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save !entity', array('!entity' => $entity_info['label'])),
    );
  }
  return $form;
}

/**
 * Form validation handler for splash_offer_form().
 */
function splash_offer_form_validate($form, &$form_state) {
  // Validate the cookie lifetime.
  if ($form_state['values']['data']['storage']['cookies']['enabled']) {
    $lifetime = $form_state['values']['data']['storage']['cookies']['fs_cookies']['lifetime'];
    if (!is_numeric($lifetime) || $lifetime < 0) {
      form_set_error('data][storage', t('Cookie lifespan must be zero or a positive number when using cookies.'));
    }
  }

  // Validate the accept link.
  if (!empty($form_state['values']['data']['links']['yes']['text'])) {
    if (!empty($form_state['values']['data']['links']['yes']['path'])) {
      // Check that the link exists
      $source = valid_url($form_state['values']['data']['links']['yes']['path'], TRUE);
      if (!$source) {
        $normal_path = drupal_get_normal_path($form_state['values']['data']['links']['yes']['path']);
        $source = drupal_valid_path($normal_path);
      }
      if (!$source) {
        form_set_error('data][links][yes][path', t('The path / url does not exist.'));
      }
    }
    else {
      form_set_error('data][links][yes][path', t('You have entered text for the accept button, please enter a valid accept path / url.'));
    }
  }

  // Notify field widgets to validate their data.
  $splash_offer = $form['#splash_offer'];
  field_attach_form_validate('splash_offer', $splash_offer, $form, $form_state);
}

/**
 * Form submission handler for splash_offer_form().
 *
 * @see splash_offer_form()
 * @see splash_offer_form_validate()
 */
function splash_offer_form_submit($form, &$form_state) {
  $entity = entity_ui_form_submit_build_entity($form, $form_state);
  $entity_info = entity_get_info('splash_offer');

  // Save and go back.
  $entity->save();
  $form_state['redirect'] = $entity_info['admin ui']['path'];
}

/** @} */ //end of group splash_offer

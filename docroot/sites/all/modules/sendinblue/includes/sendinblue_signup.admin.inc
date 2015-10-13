<?php
/**
 * @file
 * Sendinblue_signup module admin settings.
 */

/**
 * Return a form for adding/editing a sendinblue signup form.
 */
function sendinblue_signup_form($form, &$form_state, SendinBlueSignup $signup) {
  // Store the existing list for updating on submit.
  $form_state['signup'] = $signup;
  // Load css and js files.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'sendinblue') . '/css/admin-setting.css',
  );
  // Form Field for organization.
  $form['#attributes'] = array('class' => array('container-fluid'), 'id' => 'wrap');
  $form['wrap_left'] = array(
    '#prefix' => '<div id="wrap-left" class="col-md-9">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  $form['wrap_left']['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['wrap_left']['form']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The title for this signup form.'),
    '#size' => 35,
    '#maxlength' => 32,
    '#default_value' => $signup->title,
    '#required' => TRUE,
    '#attributes' => array('style' => 'width:200px;'),
  );
  // Machine-readable list name.
  $status = isset($signup->status) && $signup->mcsId && (($signup->status & ENTITY_IN_CODE) || ($signup->status & ENTITY_FIXED));
  $form['wrap_left']['form']['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $signup->name,
    '#maxlength' => 32,
    '#disabled' => $status,
    '#machine_name' => array(
      'exists' => 'sendinblue_signup_load_multiple_by_name',
      'source' => array('title'),
    ),
    '#description' => t('A unique machine-readable name for this list. It must only contain lowercase letters, numbers, and underscores.'),
    '#attributes' => array('style' => 'width:200px;'),
  );
  $form['wrap_left']['form']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($signup->settings['description']) ? $signup->settings['description'] : '',
    '#rows' => 2,
    '#maxlength' => 500,
    '#description' => t('This description will be shown on the signup form below the title. (500 characters or less)'),
  );
  $mode_defaults = array(
    SENDINBLUE_SIGNUP_BLOCK => array(SENDINBLUE_SIGNUP_BLOCK),
    SENDINBLUE_SIGNUP_PAGE => array(SENDINBLUE_SIGNUP_PAGE),
    SENDINBLUE_SIGNUP_BOTH => array(SENDINBLUE_SIGNUP_BLOCK, SENDINBLUE_SIGNUP_PAGE),
  );
  $form['wrap_left']['form']['mode'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display Mode'),
    '#required' => TRUE,
    '#options' => array(
      SENDINBLUE_SIGNUP_BLOCK => t('Block'),
      SENDINBLUE_SIGNUP_PAGE => t('Page'),
    ),
    '#default_value' => !empty($signup->mode) ? $mode_defaults[$signup->mode] : array(),
  );
  $form['wrap_left']['form']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Page URL'),
    '#description' => t('Path to the signup page. ie "newsletter/signup".'),
    '#default_value' => isset($signup->settings['path']) ? $signup->settings['path'] : NULL,
    '#states' => array(
      // Hide unless needed.
      'visible' => array(
        ':input[name="wrap_left[form][mode][' . SENDINBLUE_SIGNUP_PAGE . ']"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="wrap_left[form][mode][' . SENDINBLUE_SIGNUP_PAGE . ']"]' => array('checked' => TRUE),
      ),
    ),
    '#attributes' => array('style' => 'width:400px;'),
  );

  // Fields for organization.
  $form['wrap_left']['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $attributes = SendinblueManager::getAttributeLists();
  $form['wrap_left']['fields']['mergefields']['EMAIL']['check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Email'),
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  $form['wrap_left']['fields']['mergefields']['EMAIL']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#tree' => TRUE,
    '#default_value' => isset($signup->settings['fields']['mergefields']['EMAIL']['label']) ? $signup->settings['fields']['mergefields']['EMAIL']['label'] : t('Email'),
    '#required' => TRUE,
    '#attributes' => array('style' => 'width:200px;'),
    '#prefix' => '<div class="sendinblue_sub_field">',
    '#suffix' => '</div>',
  );
  $form['wrap_left']['fields']['mergefields']['EMAIL']['required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required?'),
    '#tree' => TRUE,
    '#default_value' => TRUE,
    '#disabled' => TRUE,
    '#prefix' => '<div class="sendinblue_sub_field">',
    '#suffix' => '</div>',
  );

  foreach ($attributes as $attribute) {
    $form['wrap_left']['fields']['mergefields'][$attribute['name']]['check'] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($attribute['name']),
      '#default_value' => isset($signup->settings['fields']['mergefields'][$attribute['name']]['check']) ? $signup->settings['fields']['mergefields'][$attribute['name']]['check'] : FALSE,
    );
    $form['wrap_left']['fields']['mergefields'][$attribute['name']]['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#tree' => TRUE,
      '#default_value' => isset($signup->settings['fields']['mergefields'][$attribute['name']]['label']) ? $signup->settings['fields']['mergefields'][$attribute['name']]['label'] : $attribute['name'],
      '#required' => TRUE,
      '#states' => array(
        // Hide unless needed.
        'visible' => array(
          ':input[name="wrap_left[fields][mergefields][' . $attribute['name'] . '][check]"]' => array('checked' => TRUE),
        ),
        'required' => array(
          ':input[name="wrap_left[form][mode][' . $attribute['name'] . '][check]"]' => array('checked' => TRUE),
        ),
      ),
      '#attributes' => array('style' => 'width:200px;'),
      '#prefix' => '<div class="sendinblue_sub_field">',
      '#suffix' => '</div>',
    );
    $form['wrap_left']['fields']['mergefields'][$attribute['name']]['required'] = array(
      '#type' => 'checkbox',
      '#title' => t('Required?'),
      '#tree' => TRUE,
      '#default_value' => isset($signup->settings['fields']['mergefields'][$attribute['name']]['required']) ? $signup->settings['fields']['mergefields'][$attribute['name']]['required'] : FALSE,
      '#states' => array(
        // Hide unless needed.
        'visible' => array(
          ':input[name="wrap_left[fields][mergefields][' . $attribute['name'] . '][check]"]' => array('checked' => TRUE),
        ),
        'required' => array(
          ':input[name="wrap_left[form][mode][' . $attribute['name'] . '][check]"]' => array('checked' => TRUE),
        ),
      ),
      '#prefix' => '<div class="sendinblue_sub_field">',
      '#suffix' => '</div>',
    );
  }

  $form['wrap_left']['fields']['submit_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Submit Button Label'),
    '#required' => 'TRUE',
    '#default_value' => isset($signup->settings['fields']['submit_button']) ? $signup->settings['fields']['submit_button'] : t('Submit'),
    '#attributes' => array('style' => 'width:200px;'),
    '#tree' => TRUE,
  );

  // Fields Field for organization.
  $form['wrap_left']['subscription'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscription'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['wrap_left']['subscription']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $sendinblue_lists = SendinblueManager::getLists();
  $options = array();
  foreach ($sendinblue_lists as $mc_list) {
    $options[$mc_list['id']] = $mc_list['name'];
  }
  $form['wrap_left']['subscription']['settings']['list'] = array(
    '#type' => 'select',
    '#title' => t('List where subscribers are saved'),
    '#options' => $options,
    '#default_value' => isset($signup->settings['subscription']['settings']['list']) ? $signup->settings['subscription']['settings']['list'] : '',
    '#description' => t('Select the list where you want to add your new subscribers'),
    '#attributes' => array('style' => 'width:200px;'),
  );
  $form['wrap_left']['subscription']['settings']['redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL redirection'),
    '#required' => FALSE,
    '#default_value' => isset($signup->settings['subscription']['settings']['redirect_url']) ? $signup->settings['subscription']['settings']['redirect_url'] : '',
    '#description' => t('Redirect to this URL after subscription'),
    '#attributes' => array('style' => 'width:400px;'),
  );
  $form['wrap_left']['subscription']['settings']['email_confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Email confirmation'),
    '#required' => FALSE,
    '#default_value' => isset($signup->settings['subscription']['settings']['email_confirmation']) ? $signup->settings['subscription']['settings']['email_confirmation'] : '',
    '#description' => t('You can choose to send a confirmation email. You will be able to set up the template & sender that will be sent to your new subscribers'),
  );
  $sendinblue_templates = SendinblueManager::getTemplateList();
  $options = array();
  foreach ($sendinblue_templates as $mc_template) {
    $options[$mc_template['id']] = $mc_template['name'];
  }
  $form['wrap_left']['subscription']['settings']['template'] = array(
    '#type' => 'select',
    '#title' => t('Select Template'),
    '#options' => $options,
    '#default_value' => isset($signup->settings['subscription']['settings']['template']) ? $signup->settings['subscription']['settings']['template'] : '-1',
    '#description' => t('Select the template that will be sent to your new subscribers. You can create new template at !SendinBlue.',
      array('!SendinBlue' => l(t('SendinBlue'), 'https://my.sendinblue.com/camp/listing/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link#temp_active_m'))),
    '#states' => array(
      // Hide unless needed.
      'visible' => array(
        ':input[name="wrap_left[subscription][settings][email_confirmation]"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="wrap_left[subscription][settings][email_confirmation]"]' => array('checked' => TRUE),
      ),
    ),
    '#attributes' => array('style' => 'width:200px;'),
  );
  /*
  $sendinblue_senders = SendinblueManager::getSenderList();
  $options = array();
  foreach ($sendinblue_senders as $mc_sender) {
    $options[$mc_sender['id']] = $mc_sender['name'];
  }
  $form['wrap_left']['subscription']['settings']['sender'] = array(
    '#type' => 'select',
    '#title' => t('Select Sender'),
    '#options' => $options,
    '#default_value' => isset($signup->settings['subscription']['settings']['sender']) ? $signup->settings['subscription']['settings']['sender'] : '-1',
    '#description' => t('Select the sender that will be sent to your new subscribers. You can create new sender at !SendinBlue.',
      array('!SendinBlue' => l(t('SendinBlue'), 'https://my.sendinblue.com/advanced/advanceparamres'))),
    '#states' => array(
      // Hide unless needed.
      'visible' => array(
        ':input[name="wrap_left[subscription][settings][email_confirmation]"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="wrap_left[subscription][settings][email_confirmation]"]' => array('checked' => TRUE),
      ),
    ),
    '#attributes' => array('style' => 'width:200px;'),
  );
  */
  $form['wrap_left']['subscription']['messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Messages'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['wrap_left']['subscription']['messages']['success'] = array(
    '#type' => 'textfield',
    '#title' => t('Success message'),
    '#required' => FALSE,
    '#default_value' => isset($signup->settings['subscription']['messages']['success']) ? $signup->settings['subscription']['messages']['success'] : t('Thank you, you have successfully registered!'),
    '#description' => t('Set up the success message that will appear when one of your visitors successfully signs up'),
    '#attributes' => array('style' => 'width:400px;'),
  );
  $form['wrap_left']['subscription']['messages']['general'] = array(
    '#type' => 'textfield',
    '#title' => t('General error message'),
    '#required' => FALSE,
    '#default_value' => isset($signup->settings['subscription']['messages']['general']) ? $signup->settings['subscription']['messages']['general'] : t('Something wrong occured'),
    '#description' => t('Set up the message that will appear when an error occurs during the subscription process'),
    '#attributes' => array('style' => 'width:400px;'),
  );
  $form['wrap_left']['subscription']['messages']['existing'] = array(
    '#type' => 'textfield',
    '#title' => t('Existing subscribers'),
    '#required' => FALSE,
    '#default_value' => isset($signup->settings['subscription']['messages']['existing']) ? $signup->settings['subscription']['messages']['existing'] : 'You have already registered',
    '#description' => t('Set up the message that will appear when a subscriber is already in your database'),
    '#attributes' => array('style' => 'width:400px;'),
  );
  $form['wrap_left']['subscription']['messages']['invalid'] = array(
    '#type' => 'textfield',
    '#title' => t('Existing subscribers'),
    '#required' => FALSE,
    '#default_value' => isset($signup->settings['subscription']['messages']['invalid']) ? $signup->settings['subscription']['messages']['invalid'] : 'Your email address is invalid',
    '#description' => t('Set up the message that will appear when the email address used to sign up is not valid'),
    '#attributes' => array('style' => 'width:400px;'),
  );
  $markup = SendinblueManager::generateSidebar();
  $form['wrap_right'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="wrap-right-side" class="col-md-3">',
    '#markup' => $markup,
    '#suffix' => '</div><div class="clearfix"></div>',
    '#tree' => TRUE,
  );
  // Action Buttons.
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => isset($signup),
    '#submit' => array('sendinblue_signup_delete_submit'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/system/sendinblue/signup',
  );
  return $form;
}

/**
 * Submit handler for sendinblue_signup_form().
 */
function sendinblue_signup_form_submit($form, &$form_state) {
  if (isset($form_state['signup'])) {
    $signup = $form_state['signup'];
    $prior_settings = $signup->settings;
    if (!$form_state['values']['wrap_left']['form']['mode'][SENDINBLUE_SIGNUP_PAGE]) {
      $form_state['values']['wrap_left']['form']['path']  = '';
    }
  }
  else {
    $signup = sendinblue_signup_create();
    $prior_settings = array();
  }
  $signup->title = $form_state['values']['wrap_left']['form']['title'];
  $signup->name = $form_state['values']['wrap_left']['form']['name'];
  $signup->mode = array_sum($form_state['values']['wrap_left']['form']['mode']);
  $signup->settings['description'] = $form_state['values']['wrap_left']['form']['description'];
  $signup->settings['path'] = $form_state['values']['wrap_left']['form']['path'];
  $signup->settings['fields'] = $form_state['values']['wrap_left']['fields'];
  $signup->settings['subscription'] = $form_state['values']['wrap_left']['subscription'];
  $signup->settings['fields']['mergefields'] = $_POST["wrap_left"]["fields"]["mergefields"];
  if ($signup->save()) {
    if (isset($form_state['values']['wrap_left']['form']['path'])) {
      if (!isset($prior_settings['path']) || $prior_settings['path'] != $signup->settings['path'] || !($signup->mode & SENDINBLUE_SIGNUP_PAGE)) {
        // We have a new (or removed) path. Rebuild menus.
        menu_rebuild();
      }
    }
    drupal_set_message(t('Signup form @name has been saved.',
    array('@name' => $signup->name)));
    $form_state['redirect'] = 'admin/config/system/sendinblue/signup';
  }
  else {
    drupal_set_message(t('There has been an error saving your signup form.'), 'error');
  }
}

/**
 * Signup deletion form.
 */
function sendinblue_signup_signup_delete_form($form, &$form_state, $signup) {
  $form_state['signup'] = $signup;
  return confirm_form(
    $form,
    t('Are you sure you want to delete the signup form %name?', array('%name' => $signup->label())),
    'admin/config/system/sendinblue/signup/manage/' . $signup->identifier(),
    t('This action cannot be undone.'),
    t('Delete Signup form')
  );
}

/**
 * Submit function for the delete button on the signup overview and edit forms.
 */
function sendinblue_signup_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/system/sendinblue/signup/' . $form_state['signup']->identifier() . '/delete';
}

/**
 * Submit handler for sendinblue_signup_signup_delete_form().
 */
function sendinblue_signup_signup_delete_form_submit($form, &$form_state) {
  $signup = $form_state['signup'];
  sendinblue_signup_delete_multiple(array($signup->identifier()));
  drupal_set_message(t('%name has been deleted.', array('%name' => $signup->label())));
  $form_state['redirect'] = 'admin/config/system/sendinblue/signup';
}

/**
 * Create a new SendinBlue Signup object.
 */
function sendinblue_signup_create(array $values = array()) {
  return entity_get_controller('sendinblue_signup')->create($values);
}

/**
 * Load signup entities.
 */
function sendinblue_signup_load_multiple_by_name($name = NULL) {
  $signups = entity_load_multiple_by_name('sendinblue_signup', isset($name) ? array($name) : FALSE);
  return isset($name) ? reset($signups) : $signups;
}

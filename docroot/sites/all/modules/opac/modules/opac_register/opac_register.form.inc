<?php
/**
 * @file
 * All forms for opac register module.
 */


/**
 * Form constructor for opac registration form.
 *
 * Allows a user to type his email address.
 *
 * @see opac_register_registration_form_validate()
 * @ingroup forms
 */
function opac_register_registration_form($form, &$form_state) {
  $want_libray_registration = FALSE;

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'opac_register') . '/opac_register.js',
  );

  if (variable_get('opac_register_before_registration_text')) {
    $form['before_registration_text'] = array(
      '#markup' => '<p>' . t(variable_get('opac_register_before_registration_text')) . '</p>',
    );

  }

  if (user_is_anonymous()) {

    $form['person_type'] = array(
      '#type' => 'radios',
      '#default_value' => 'natural',
      '#options' => array(
        'natural' => t('I am a natural person'),
        'collectivity' => t('I am a collectivity'),
      ),
      '#required' => TRUE,
    );

    $form['drupal_register'] = array(
      '#type' => 'fieldset',
      '#title' => t('Personal informations'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#attributes' => array(
        'id' => 'opac-register-drupal-register',
      ),
    );
    $form['drupal_register']['title'] = array(
      '#type' => 'select',
      '#title' => t('Salutation'),
      '#default_value' => 'miss',
      '#options' => array(
        'mrs' => t('Mrs'),
        'mr' => t('Mr.'),
      ),
      '#states' => array(
        'visible' => array(
          ':input[name="person_type"]' => array('value' => 'natural'),
        ),
        'required' => array(
          ':input[name="person_type"]' => array('value' => 'natural'),
        ),
      ),
    );
    $form['drupal_register']['firstname'] = array(
      '#type' => 'textfield',
      '#title' => t('Firstname'),
      '#size' => 30,
      '#states' => array(
        'visible' => array(
          ':input[name="person_type"]' => array('value' => 'natural'),
        ),
        'required' => array(
          ':input[name="person_type"]' => array('value' => 'natural'),
        ),
      ),
    );
    $form['drupal_register']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#size' => 30,
      '#states' => array(
        'visible' => array(
          ':input[name="person_type"]' => array('value' => 'collectivity'),
        ),
        'required' => array(
          ':input[name="person_type"]' => array('value' => 'collectivity'),
        ),
      ),
    );
    $form['drupal_register']['othernames'] = array(
      '#type' => 'textfield',
      '#title' => t('Other name'),
      '#size' => 30,
      '#states' => array(
        'visible' => array(
          ':input[name="person_type"]' => array('value' => 'collectivity'),
        ),
      ),
    );
    $form['drupal_register']['surname'] = array(
      '#type' => 'textfield',
      '#title' => t('Surname'),
      '#size' => 30,
      '#states' => array(
        'visible' => array(
          ':input[name="person_type"]' => array('value' => 'natural'),
        ),
        'required' => array(
          ':input[name="person_type"]' => array('value' => 'natural'),
        ),
      ),
    );
    $form['drupal_register']['login'] = array(
      '#type' => 'textfield',
      '#title' => t('Login'),
      '#size' => 60,
      '#required' => TRUE,
      '#description' => t('You can keep the default login or choose another one.'),
    );
    $form['drupal_register']['password'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#size' => 60,
      '#required' => TRUE,
    );
    $form['drupal_register']['password_confirm'] = array(
      '#title' => t('Password confirmation'),
      '#description' => t('Please enter the same password again.'),
      '#type' => 'password',
      '#size' => 60,
      '#required' => TRUE,
    );
    $form['drupal_register']['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#size' => 60,
      '#required' => variable_get('opac_register_field_email_mandatory', TRUE),
      '#rules' => array(
        array('rule' => 'email', 'error' => 'Email is not valid.'),
      ),
    );
    $form['drupal_register']['dateofbirth'] = array(
      '#type' => 'date',
      '#title' => t('Date of birth'),
      '#states' => array(
        'visible' => array(
          ':input[name="person_type"]' => array('value' => 'natural'),
        ),
        'required' => array(
          ':input[name="person_type"]' => array('value' => 'natural'),
        ),
      ),
    );

    if (variable_get('opac_register_terms', TRUE)) {
      $form['drupal_register']['conditions'] = array(
        '#type' => 'checkbox',
        '#title' => t('I agree with the ') . l(t('terms of service.'), 'opac-register/drupal-terms'),
        '#required' => TRUE,
      );
    }

    $form['want_libray_registration'] = array(
      '#type' => 'checkbox',
      '#title' => t('I want to pre-register to the library.'),
      '#default_value' => variable_get('opac_register_library_registration_expanded', TRUE),
    );
    $form['library_register'] = array(
      '#type' => 'fieldset',
      '#title' => t('Personal informations for library registration'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#states' => array(
        'visible' => array(
          ':input[name="want_libray_registration"]' => array('checked' => TRUE),
        ),
      ),
    );

  }
  else {
    $form['want_libray_registration'] = array(
      '#type' => 'hidden',
      '#value' => '1',
    );
    $want_libray_registration = TRUE;
    $form['library_register'] = array(
      '#type' => 'fieldset',
      '#title' => t('Personal informations for library registration'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['library_register']['password'] = array(
      '#type' => 'password',
      '#title' => t('password'),
      '#size' => 60,
      '#required' => TRUE,
      '#description' => "You have to enter this site's password.",
    );
  }

  $servers = opac_register_ils_servers();
  if (count($servers) > 1) {
    $form['library_register']['server'] = array(
      '#type' => 'select',
      '#title' => t('Library'),
      '#options' => $servers,
    );
  }
  else {
    $keys = array_keys($servers);
    $form['library_register']['server'] = array(
      '#type' => 'hidden',
      '#value' => $keys[0],
    );
  }

  $form['library_register']['branch'] = array(
    '#type' => 'select',
    '#title' => t('Branch'),
    '#options' => opac_register_user_branches(),
  );

  $zip_options = array(
    '#type' => 'textfield',
    '#title' => t('Zip code'),
    '#size' => 10,
  );
  if (variable_get('opac_register_field_zip-code_mandatory', TRUE)) {
    if ($want_libray_registration) {
      $zip_options['#required'] = TRUE;
    }
    else {
      $zip_options['#states'] = array(
        'required' => array(
          '#edit-want-libray-registration' => array('checked' => TRUE),
        ),
      );
    }
  }
  $form['library_register']['zip_code'] = $zip_options;

  $address_options = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#size' => 50,
  );
  if (variable_get('opac_register_field_address_mandatory', TRUE)) {
    if ($want_libray_registration) {
      $address_options['#required'] = TRUE;
    }
    else {
      $address_options['#states'] = array(
        'required' => array(
          '#edit-want-libray-registration' => array('checked' => TRUE),
        ),
      );
    }
  }
  $form['library_register']['address'] = $address_options;

  if (variable_get('opac_register_library_city_use_taxonomy', FALSE)) {
    $vid = variable_get('opac_register_library_city_taxonomy');
    $terms = taxonomy_get_tree($vid);
    $city_options = array();

    // Adding an empty value if the field is not required.
    if (!variable_get('opac_register_field_city_mandatory', TRUE)) {
      $city_options[''] = '';
    }

    foreach ($terms as $term) {
      $city_options[$term->name] = $term->name;
    }

    $form['library_register']['city'] = array(
      '#type' => 'select',
      '#title' => t('City'),
      '#options' => $city_options,
    );
  }
  else {
    $city_options = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#size' => 30,
    );
    if (variable_get('opac_register_field_city_mandatory', TRUE)) {
      if ($want_libray_registration) {
        $city_options['#required'] = TRUE;
      }
      else {
        $city_options['#states'] = array(
          'required' => array(
            '#edit-want-libray-registration' => array('checked' => TRUE),
          ),
        );
      }
    }

    $form['library_register']['city'] = $city_options;

  }
  $phone_options = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#size' => 30,
  );
  if (variable_get('opac_register_field_phone_mandatory', TRUE)) {
    if ($want_libray_registration) {
      $phone_options['#required'] = TRUE;
    }
    else {
      $phone_options['#states'] = array(
        'required' => array(
          '#edit-want-libray-registration' => array('checked' => TRUE),
        ),
      );
    }
  }
  $form['library_register']['phone'] = $phone_options;

  if (variable_get('opac_register_terms', TRUE)) {
    $form['library_register']['terms_use'] = array(
      '#type' => 'checkbox',
      '#title' => t('I accept the ') . l(t('library terms of use.'), 'opac-register/library-terms'),
    );
  }

  $form['info_accurate'] = array(
    '#type' => 'submit',
    '#value' => t('Register'),
  );
  return $form;
}

/**
 * Validation handler for opac_register_registration_form().
 */
function opac_register_registration_form_validate($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  module_load_include('inc', 'opac', 'includes/opac.harvest');

  $connector = opac_get_instance(opac_server_load($values['server']));

  if (user_is_anonymous()) {

    $password = $form_state['values']['password'];
    $password_confirm = $form_state['values']['password_confirm'];

    if (strlen($password) < 6) {
      form_set_error('password', t('Your password must be at least 6 characters.'));
    }

    if ($password != $password_confirm) {
      form_set_error('password_confirm', t("Passwords don't match"));
    }

    if ($values['person_type'] == 'natural') {
      if (!$values['title']) {
        form_set_error('title', t('Salutation field is required'));
      }
      if (!$values['firstname']) {
        form_set_error('firstname', t('Firstname field is required'));
      }
      if (!$values['surname']) {
        form_set_error('surname', t('Surname field is required'));
      }
    }
    else {
      if (!$values['name']) {
        form_set_error('name', t('Name field is required'));
      }
    }

    if ($values['want_libray_registration']) {
      if (variable_get('opac_register_field_zip-code_mandatory', TRUE) && !$values['zip_code']) {
        form_set_error('zip-code', t('Zip code field is required'));
      }

      if (variable_get('opac_register_field_address_mandatory', TRUE) && !$values['address']) {
        form_set_error('address', t('Address field is required'));
      }
      if (variable_get('opac_register_field_city_mandatory', TRUE) && !$values['city']) {
        form_set_error('address', t('City field is required'));
      }
      if (variable_get('opac_register_field_phone_mandatory', TRUE) && !$values['phone']) {
        form_set_error('address', t('Phone field is required'));
      }

    }

    if (form_get_errors()) {
      return;
    }
  }
  $email = isset($values['email']) ? $values['email'] : $user->mail;
  $login = isset($values['login']) ? $values['login'] : $user->name;

  // Check if login exists in ILS.
  $response = $connector->loginExists($login);

  if ($response) {
    form_set_error('opac_register_registration_form', t('This login already exists in the ILS. Please choose another one or contact the library.'));
  }

  // Check if user exists in Drupal.
  if (user_is_anonymous()) {
    $exists_drupal = FALSE;
    if (opac_register_user_exists(array('name' => $login))) {
      $exists_drupal = TRUE;
    }
    if (opac_register_user_exists(array('mail' => $email))) {
      $exists_drupal = TRUE;
    }

    if ($exists_drupal) {
      form_set_error('opac_register_registration_form', t('This login or email already exists in Drupal. Please choose another one.'));
    }
  }
}

/**
 * Submit handler for opac_register_registration_form().
 */
function opac_register_registration_form_submit($form, &$form_state) {
  global $user;
  module_load_include('inc', 'opac', 'includes/helper.taxonomy');
  $values = $form_state['values'];
  $opac_server_identifier = opac_server_load($values['server']);
  $connector = opac_get_instance($opac_server_identifier);

  $info = array();
  if (user_is_anonymous()) {
    $info['email'] = $values['email'];
    $info['login'] = $values['login'];
    if ($values['person_type'] == 'natural') {
      $info['firstname'] = $values['firstname'];
      $info['surname'] = $values['surname'];
    }
    else {
      $info['firstname'] = $info['surname'] = $values['name'];
    }
    $info['title'] = $values['title'];
    $dob_year = $values['dateofbirth']['year'];
    $dob_month = $values['dateofbirth']['month'];
    $dob_day = $values['dateofbirth']['day'];
    $info['dateofbirth'] = $dob_year . '-' . sprintf("%02d", $dob_month) . '-' . sprintf("%02d", $dob_day);
  }
  else {
    $user_data = user_load($user->uid);
    $info['email'] = $user->mail;
    $info['login'] = $user->name;
    foreach (array('firstname', 'surname', 'title', 'dateofbirth') as $key) {
      $fieldname = "field_account_$key";
      if (isset($user_data->{$fieldname}['und'][0]['value'])) {
        $info[$key] = $user_data->{$fieldname}['und'][0]['value'];
      }
      else {
        $info[$key] = '';
        $message = t('Field @field is not set. This can cause problems for ILS registration.', array('@field' => $fieldname));
        drupal_set_message($message, 'error');
      }
    }
  }

  if (user_is_anonymous()) {
    $drupal_user = array(
      'name' => $info['login'],
      'mail' => $info['email'],
      'pass' => $values['password'],
      'status' => 1,
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      ),
      'field_account_firstname' => array(
        'und' => array(
          0 => array(
            'value' => $info['firstname'],
          ),
        ),
      ),
      'field_account_surname' => array(
        'und' => array(
          0 => array(
            'value' => $info['surname'],
          ),
        ),
      ),
      'field_account_title' => array(
        'und' => array(
          0 => array(
            'value' => $info['title'],
          ),
        ),
      ),
      'field_account_dateofbirth' => array(
        'und' => array(
          0 => array(
            'value' => $info['dateofbirth'],
          ),
        ),
      ),
    );

    // Ugly (but needed) fix for the CAS module.
    if (module_exists('cas')) {
      $drupal_user['cas_name'] = $info['login'];
    }

    // Let's save the user.
    $account = user_save('', $drupal_user);

    if ($account) {
      if (variable_get('opac_register_after_registration_text')) {
        drupal_set_message(t(variable_get('opac_register_after_registration_text')));
      }
      drupal_set_message(t("Your drupal account has been successfully created. You can now login with @login as username.", array('@login' => $info['login'])));
      $params['account'] = $account;
      if ($account->mail) {
        $send = drupal_mail(
          'opac_register',
          'confirm_drupal_register',
          $account->mail,
          language_default(),
          $params
        );
      }
    }
    else {
      form_set_error(t("Your drupal account could not be created."));
    }
  }

  if ($values['want_libray_registration']) {
    $ils_user = array(
      'userid' => $info['login'],
      'password' => $values['password'],
      'firstname' => $info['firstname'],
      'surname' => $info['surname'],
      'branchcode' => $values['branch'],
    );

    if (isset($sex)) {
      $ils_user['sex'] = $sex;
    }
    $ils_user['title'] = $info['title'];
    $ils_user['dateofbirth'] = $info['dateofbirth'];
    $ils_user['email'] = $info['email'];
    if ($values['zip_code']) {
      $ils_user['zipcode'] = $values['zip_code'];
    }
    if ($values['address']) {
      $ils_user['address'] = $values['address'];
    }
    if ($values['city']) {
      $ils_user['city'] = $values['city'];
    }
    if ($values['phone']) {
      $ils_user['phone'] = $values['phone'];
    }
    if (isset($values['othernames'])) {
      $ils_user['othernames'] = $values['othernames'];
    }

    // We create the user in the ILS.
    $result = $connector->createUser($ils_user);

    /* But we also change (or set) the opac server identifier in the
    drupal account. */
    $drupal_user_edit = array(
      variable_get('opac_users_server_field', '') => array(
        'und' => array(
          0 => array(
            'value' => $values['server'],
          ),
        ),
      ),
    );
    /* If the account was defined (drupal and ils registration at the same
    time), we use it, else, the ils registration took place after the drupal
    registration, so we use the logged-in user informations. */
    if (!isset($account)) {
      $account = $user;
    }
    user_save($account, $drupal_user_edit);

    if (!$result) {
      form_set_error('opac_register_registration_form', t('Registration process failed.'));
    }
    else {
      drupal_set_message(t('Your ILS registration has been successfully processed.'));
      if ($account->mail) {
        $params['account'] = $account;

        // Getting branch name.
        $vid = _opac_taxonomy_get_vid('library');
        $tid = _opac_taxonomy_get_term($vid, 'field_code', $values['branch']);
        $term = taxonomy_term_load($tid);
        $params['ilsname'] = $term->name;

        $send = drupal_mail(
          'opac_register',
          'confirm_ils_register',
          $account->mail,
          language_default(),
          $params
        );
      }
    }
  }
}

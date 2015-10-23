<?php

/**
 * @file
 * Admin settings include for dotmailer module
 */

define('DOTMAILER_TOKEN_MODULE_URL', 'https://www.drupal.org/project/token');
define('DOTMAILER_TEST_ADDRESS_BOOK_NAME', 'Test');

/**
 * Form callback for the admin settings for dotmailer
 */
function dotmailer_admin_settings($form, &$form_state) {
  // Build the fieldset with the proper number of accounts. We'll use
  // $form_state['num_accts'] to determine the number of textfields to build.
  if (empty($form_state['num_accts'])) {
    $form_state['num_accts'] = 1;
  }
  $form['dotmailer_account_info'] = array(
    '#type' => 'fieldset',
    '#title' => 'Dotmailer Account Information',
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#weight' => -1,
    '#prefix' => '<div id="accts-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  for ($i = 1; $i <= $form_state['num_accts']; $i++) {
    // Only show the book selection forms if account information has been provided
    $username = variable_get('dotmailer_username'.$i);
    $password = variable_get('dotmailer_password'.$i);
  }

  if ($username && $password) {
    $form = _dotmailer_admin_settings_with_account($form, $form_state);
  }
  else {
    $form = _dotmailer_admin_settings_no_account($form, $form_state);
  }
  if($form_state['num_accts'] < 5 && !$username) {
    $form['dotmailer_account_info']['add_more'] = array(
      '#type' => 'submit',
      '#value' => t('Add an account'),
      '#submit' => array('dotmailer_add_more'),
      // See the examples in ajax_example.module for more details on the
      // properties of #ajax.
      '#ajax' => array(
        'callback' => 'dotmailer_add_more_callback',
        'wrapper' => 'accts-fieldset-wrapper',
      ),
    );
  }
  return system_settings_form($form);
}
/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function dotmailer_add_more_callback($form, $form_state) {
  return $form['dotmailer_account_info'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function dotmailer_add_more($form, &$form_state) {
  $form_state['num_accts']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Helper for providing the form elements when an account is active
 */
function _dotmailer_admin_settings_with_account(&$form, &$form_state) {
  $usernames = array();
  for ($i = 1; $i <= 5; $i++) {
    $usernames[] = variable_get('dotmailer_username'.$i);
  }
  $usernames = array_filter($usernames, 'strlen');
  $usernames_str = implode(", " , $usernames);
  $form['dotmailer_account_info']['dotmailer_setup_account'] = array(
    '#markup' => '<p>' . t('Current configured accounts: @username',
      array('@username' => $usernames_str)) . '</p>',
  );

  $form['dotmailer_account_info']['reset'] = array(
    '#markup' => '<p>' . l(t('Reset all accounts'),
      'admin/config/dotmailer/reset') . '</p>',
  );
  $accounts = array();
  for ($i = 1; $i <= 5; $i++) {
    if (variable_get('dotmailer_username'.$i)) {
      $accounts[$i]['username'] = 'dotmailer_username'.$i;
      $accounts[$i]['password'] = 'dotmailer_password'.$i;
    }
  }
  $address_books = array();
  foreach ($accounts as $account) {
    $account_address_books = dotmailer_listaddressbooks($account['username'],$account['password']);
    if (!empty($account_address_books)) {
      foreach($account_address_books as $account_address_book) {
        $account_address_book->username = $account['username'];
        $account_address_book->password = $account['password'];
      }
      $address_books[$account['username']] = $account_address_books;
    }
  }

  if ($address_books) {
    $available_lists = array();

    $form['dotmailer_books'] = array(
      '#type' => 'vertical_tabs',
      '#collapsible' => FALSE,
      '#title' => 'dotmailer Subscription Lists',
      '#tree' => TRUE,
    );

    $active_lists = variable_get('dotmailer_active_lists');

    if (!$active_lists) {
      $form['dotmailer_books']['none_available']['#markup'] = t('Currently no lists are active.');
    }
    $count = 1;
    foreach ($address_books as $address_book_account) {
      foreach ($address_book_account as $address_book) {

        // Handle Test Address book as it is not possible to write to that Address book using the dotMailer API
        if (DOTMAILER_TEST_ADDRESS_BOOK_NAME === $address_book->Name) {
          $test_address_book_id = $address_book->ID;
        }

        $available_lists[$count][$address_book->ID] = "$address_book->Name
        [$address_book->ID]";

        if ($active_lists[$count-1] && in_array($address_book->ID,
          $active_lists[$count-1])) {
          // Get the stored books
          $form['dotmailer_books']['dotmailer_book_' . $address_book->ID] =
            _dotmailer_admin_settings_address_book_settings($address_book,
              $address_book->username, $address_book->password);
        }
      }
      // Active list configuration
      $form['dotmailer_active_books'.$count] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => 0,
        '#title' => t('Active Address books for @apikey',
          array('@apikey' => variable_get($address_book->username))),
      );
      if ($active_lists) {
        $default_value = $active_lists[$count-1];
      }
      else {
        $default_value = array();
      }
      $form['dotmailer_active_books'.$count]['dotmailer_active_lists'.$count] =
        array(
        '#type' => 'checkboxes',
        '#options' => $available_lists[$count],
        '#default_value' => $default_value,
        '#multiple' => 'multiple',
        '#description' => t('Select the dotMailer address books to use within your Drupal site.'),
      );

      // Handle Test Address book as it is not possible to write to that Address book using the dotMailer API
      $form['dotmailer_active_books'.$count]['dotmailer_active_lists'.$count][$test_address_book_id]['#return_value'] = 0;
      $form['dotmailer_active_books'.$count]['dotmailer_active_lists'.$count][$test_address_book_id]['#description'] = t('This address book cannot be used with the dotMailer API, !api_docs (see ERROR_ADDRESSBOOK_NOTWRITABLE code).', array('!api_docs' => l(t('see documentation'), 'http://www.dotmailer.com/resources/dev-tools-guides/api-error-codes/')));
      $form['dotmailer_active_books'.$count]['dotmailer_active_lists'.$count][$test_address_book_id]['#disabled'] = TRUE;

      $count++;
    }
  }
  else {
    drupal_set_message(t('You do not have any valid dotmailer address books.'), 'error');
    // Books is a string if there is an error.
    if (!empty($address_books)) {
      drupal_set_message(check_plain($address_books), 'error');
    }
  }


  // Messages configuration
  $form['dotmailer_messages'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => 'Anonymous Lists Messaging',
  );
  $form['dotmailer_messages']['dotmailer_subscription_success_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Subscription Success Message'),
    '#default_value' => variable_get('dotmailer_subscription_success_message', t('Thank you, you have been successfully subscribed.')),
  );
  $form['dotmailer_messages']['dotmailer_subscription_failure_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Subscription Failure Message'),
    '#default_value' => variable_get('dotmailer_subscription_failure_message', t('We were unable to subscribe you at this time. Please try again later.')),
  );
  $form['dotmailer_messages']['dotmailer_unsubscription_success_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Unsubscription Success Message'),
    '#default_value' => variable_get('dotmailer_unsubscription_success_message', t('Thank you, you have been successfully unsubscribed.')),
  );
  $form['dotmailer_messages']['dotmailer_unsubscription_failure_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Unsubscription Failure Message'),
    '#default_value' => variable_get('dotmailer_unsubscription_failure_message', t('We were unable to unsubscribe you at this time. Please try again later.')),
  );

  // Extra settings
  $form['dotmailer_user_register'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show subscription options on the user registration form.'),
    '#description' => t('This will only apply for books granted to the authenticated role.'),
    '#default_value' => variable_get('dotmailer_user_register', FALSE),
  );
  $form['dotmailer_user_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Subscription Options on User Edit Screen'),
    '#description' => t('If set, a tab will be presented for managing newsletter subscriptions when editing an account.'),
    '#default_value' => variable_get('dotmailer_user_edit', FALSE),
  );

  return $form;
}

/**
 * Provides the form elements for the settings for each address book.
 */
function _dotmailer_admin_settings_address_book_settings($address_book, $username, $password) {
  static $weight = 0;

  $saved_book = dotmailer_get_saved_address_book($address_book);


  $element = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => $address_book->Name,
    '#tree' => TRUE,
  );

  $element['book_id'] = array(
    '#type' => 'value',
    '#value' => $address_book->ID,
  );

  $element['username'] = array(
    '#type' => 'value',
    '#value' => $username,
  );

  $element['password'] = array(
    '#type' => 'value',
    '#value' => $password,
  );

  $element['name'] = array(
    '#type' => 'value',
    '#value' => $address_book->Name,
  );

  // Few fields for use with sign up block
  // Title
  $element['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Display title for address book'),
    '#default_value' => $saved_book->title,
    '#maxlength' => 127,
  );

  // List weight
  $element['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $saved_book->weight ? $saved_book->weight : $weight++,
    '#delta' => 20,
    '#description' => t('Weight of the book in the list, if same as another it will be sorted alphabetically'),
  );

  // URL for external viewing information
  $element['external_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to view information about this list'),
    '#default_value' => $saved_book->external_url,
    '#description' => t('Used as a URL field so remember to include http://'),
    '#maxlength' => 127,
  );

  // Add a row of checkboxes for role enabling.
  $element['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#description' => t('Choose which roles may subscribe to this book. All users will see the books they\'re eligble for at the !subscribe and in the subscription block. Lists assigned to the Authenticated role may also apear in the registration form if that option is selected below. Authenticated user may also manage their book settings from their profile.', array('!subscribe' => l(t('newsletter subscription page'), 'dotmailer/subscribe'))),
    '#tree' => TRUE,
  );

  // Loop through the roles and allow the admin to assign a book to a role
  foreach (user_roles() as $rid => $name) {
    $element['roles'][$rid] = array(
      '#type' => 'checkbox',
      '#title' => $name,
      '#default_value' => isset($saved_book->roles[$rid]) ? $saved_book->roles[$rid] : FALSE,
    );
  }

  $element['description'] = array(
    '#type' => 'textarea',
    '#title' => t('List Description'),
    '#default_value' => $saved_book->description,
    '#description' => t('This description will be shown to the user on the book signup and user account settings pages'),
  );

  $element['booktype'] = array(
    '#type' => 'select',
    '#title' => t('Subscription Method'),
    '#options' => array(DOTMAILER_BOOKTYPE_OPTIN => "Opt-in", DOTMAILER_BOOKTYPE_OPTOUT => 'Opt-out', DOTMAILER_BOOKTYPE_REQUIRED => 'Required'),
    '#default_value' => ($saved_book->booktype) ? $saved_book->booktype : DOTMAILER_BOOKTYPE_OPTIN,
    '#description' => t('<strong>Opt-in:</strong> Users must sign up to recieve messages.<br/><strong>Opt-out: </strong> Users are automatically signed up but may unsubscribe.<br/><strong>Required: </strong> Users will remain on the book as long as they have an account and cannot unsubscribe.'),
  );

  $element['doublein'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require subscribers to Double Opt-in'),
    '#default_value' => $saved_book->doublein,
    '#description' => t('New subscribers will be sent a link with an email they must follow to confirm their subscription. <em>This may need to be set up in dotMailer</em>.'),
  );


  $datafields = dotmailer_listcontactdatalabels($username, $password, $address_book->ID);
  if ($datafields) {
    // Merge var fieldset
    $element['datafields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Contact Data Fields'),
      '#description' => t(
        'Select Drupal user variables to send to dotmailer as contact data fields. Available Drupal variables are any Profile or !token variables for the given user. For more information on contact data fields, see the !doc.',         array(
          '!doc' => l(t('dotMailer Documentation'), 'https://support.dotmailer.com/entries/51187898-Data-Mapping-Mapping-customer-information-to-dotMailer-contact-data-fields'),
          '!token' => l(t('Token'), DOTMAILER_TOKEN_MODULE_URL),
        )
      ),
      '#tree' => TRUE,
    );

    foreach ($datafields->ContactDataLabel as $field) {
      $element['datafields'][$field->Name] = array(
        '#type' => 'textfield',
        '#title' => $field->Name,
        '#default_value' => !empty($saved_book->datafields[$field->Name]) ? $saved_book->datafields[$field->Name] : '',
      );
    }

    if (module_exists('token')) {
      $element['datafields']['token_help'] = array(
        '#title' => t('Replacement patterns'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $element['datafields']['token_help']['help'] = array(
        '#theme' => 'token_tree',
        '#token_types' => array('user'),
      );
    }else{
      drupal_set_message(t('Install the !token module to send user values to dotMailer contact data fields', array('!token' => l(t('Token'), DOTMAILER_TOKEN_MODULE_URL))), 'warning');
    }
  }

  return $element;
}

/**
 * Provides a settings form for the case where there is no account set up yet.
 */
function _dotmailer_admin_settings_no_account(&$form, &$form_state) {
  for ($i = 1; $i <= $form_state['num_accts']; $i++) {
    $form['dotmailer_account_info'][$i]['#prefix'] = '<h3>Account ' . $i .
      '</h3>';
    // Show the username and password form so we can authenticate the user
    $form['dotmailer_account_info'][$i]['dotmailer_username' . $i] = array(
      '#type' => 'textfield',
      '#title' => t('Dotmailer API username'),
      '#required' => TRUE,
      '#default_value' => variable_get('dotmailer_username' . $i, ''),
      '#description' => t('The username used to connect to the dotmailer API.'),
    );
    $form['dotmailer_account_info'][$i]['dotmailer_password' . $i] = array(
      '#type' => 'textfield',
      '#title' => t('Dotmailer API password'),
      '#required' => TRUE,
      '#default_value' => variable_get('dotmailer_password' . $i, ''),
      '#description' => t('The password used to connect to the dotmailer API.'),
    );
  }
  return $form;
}

/**
 * Submit the admin settings and serialize the saved books into objects
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function dotmailer_admin_settings_validate($form, &$form_state) {
  // No books selected or first time here
  if (empty($form_state['values']['dotmailer_active_lists1'])) {
    return;
  }
  // Set active lists if values submitted
  if (!empty($form_state['values']['dotmailer_active_lists1'])) {
    $active_lists = array();
    $i = 1;
    while($i <= 5) {
      if (isset($form_state['values']['dotmailer_active_lists'.$i])) {
        $active_lists[] = $form_state['values']['dotmailer_active_lists'.$i];
      }
      $i++;
    }
    $active_lists_complete = array();
    foreach ($active_lists as $active_list) {
      $active_lists_complete[] = $active_list;
    }
    variable_set('dotmailer_active_lists', $active_lists_complete);
  }

  variable_set('dotmailer_user_register', $form_state['values']['dotmailer_user_register']);
  variable_set('dotmailer_user_edit', $form_state['values']['dotmailer_user_edit']);

  // Loop through each book and create an array to serialize with,
  // saves storing lots of individual values per book
  $books = array();
  $sorted_books = array();
  if ($form_state['values']['dotmailer_books']['dotmailer_books__active_tab']) {
    unset($form_state['values']['dotmailer_books']['dotmailer_books__active_tab']);
  }
  foreach ($form_state['values']['dotmailer_books'] as $form_book) {
    $book = dotmailer_get_saved_address_book($form_book['book_id']);
    $book->Id = $form_book['book_id'];
    $book->Username = $form_book['username'];
    $book->Password = $form_book['password'];
    $book->Name = $form_book['name'];
    $book->title = $form_book['title'];
    $book->weight = $form_book['weight'];
    $book->external_url = $form_book['external_url'];
    $book->roles = array_filter((array)$form_book['roles']);
    $book->description = $form_book['description'];
    $book->booktype = $form_book['booktype'];
    $book->doublein = $form_book['doublein'];
    $book->datafields = $form_book['datafields'];
    $books[$form_book['weight'] . ' ' . $book->Name . ' ' . $book->Id] = $book;
  }
  ksort($books);
  foreach ($books AS $this_book) {
    $sorted_books[$this_book->Id] = $this_book;
  }

  // Remove books from the form_state
  unset($form_state['values']['dotmailer_books']);
  variable_set('dotmailer_books', $sorted_books);
}


<?php
/**
 * @file
 *  wildfire_lists.api.inc
 *
 *  Forms and page callbacks for the list module
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

// Pretty much all of the forms use the lists API in some fashion, so always
// ensure the API is loaded:
module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

/**
 * Form to add a new list.
 */
function wildfire_lists_add_form($form, &$form_state) {
  $form = array();

  $form = _wildfire_lists_form();
  $form['submit']['#value'] = t('Add');

  return $form;
}

/**
 * Submit handler for wildfire_lists_add_form().
 */
function wildfire_lists_add_form_submit($form, &$form_state) {
  wildfire_lists_add_list($form_state['values']['listname']);
  $msg = t(
    'The list %list has been created. You can now !importlink into this list.',
    array(
      '%list' => $form_state['values']['listname'],
      '!importlink' => l(
        t('import'),
        'admin/wildfire/lists/import'
      )
    )
  );

  drupal_set_message(filter_xss($msg));

  $form_state['redirect'] = 'admin/wildfire/lists';

}

/**
 * FAPI form to allow a user to be added to a mailing list by typing an email.
 */
function wildfire_lists_adduser_form($form, &$form_state) {

  $form['error_messages'] = array(
    '#markup' => '<div id="adduser-error" class="error messages"></div>',
  );
  $form['status_messages'] = array(
    '#markup' => '<div id="adduser-status" class="status messages"></div>',
  );
  $form['warning_messages'] = array(
    '#markup' => '<div id="adduser-warning" class="warning messages"></div>',
  );

  $form['lid'] = array(
    '#type' => 'hidden',
    '#value' => arg(3),
    '#attributes' => array(
      'id' => 'edit-lid',
    ),
  );
  $form['adduser_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add user'),
    '#description' => t('In order to add users to the list below, you can do so directly from this page by entering an email address and clicking the "add" button.'),
  );
  $form['adduser_fieldset']['adduser_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#description' => t('Type an email address in order to add it to this list.'),
  );
  $form['adduser_fieldset']['adduser_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#ajax' => array(
      'callback' => 'wildfire_lists_adduser_callback'
    ),
  );
  $form['newuser_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('New user details'),
    '#description' => t('The email address is not already registered. Please complete the fields below to complete the registration process for this email address.'),
    '#attributes' => array('id' => 'edit-newuser-fieldset'),
    '#parents' => array('newuser_fieldset'),
  );

  if (module_exists(('field'))) {

    field_attach_form(
      'user',
      NULL,
      $form['newuser_fieldset'],
      $form_state,
      NULL
    );

    // Add a 'save' button to save a new user's details.
    $form['newuser_fieldset']['newuser_save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 10,
      '#ajax' => array(
        'callback' => 'wildfire_lists_newuser_callback',
        'wrapper' => 'wildfire-userlist',
      ),
    );
    // Add a 'cancel' button in case the details are not to be saved.
    $form['newuser_fieldset']['newuser_cancel'] = array(
      '#type' => 'button',
      '#value' => t('Cancel'),
      '#weight' => 10,
    );

  }

  return $form;
}

/**
 * Submit handler for wildfire_lists_adduser_form
 *
 * TODO: Add support for a non-JS codepath. Craig knows how to refactor this
 * appropriately to do this, but it's low on the priority list.
 */
function wildfire_lists_adduser_form_submit($form, &$form_state) {

  if (TRUE) {
    /**
     * Form submit called via AJAX POST.
     *
     * We handle all the form submission in wildfire_lists_adduser_callback()
     * as we want to pass back AJAX commands, however on 'submit' buttons
     * this still gets called in order to process the form if it needs it.
     *
     * To support a non-JS codepath, form processing should be moved here, with
     * wildfire_lists_adduser_callback() as essentially an overlay to make the
     * form work nicely.
     */
  }
  else {
    /**
     * Form submit occurred via a standard POST. Do nothing, as presently
     * this is not supported.
     */
    drupal_set_message(
      t('This form requires Javascript to be enabled in your browser'),
      'error'
    );
  }
}

/**
 * Adds a user to the list via an AJAX callback. Uses Drupal FAPI
 *
 * @param $form array
 *    The form array
 * @param $form_state array
 *    The form state
 *
 * @return array
 *    An array of AJAX commands that get converted to JSON and processed
 *    client-side
 */
function wildfire_lists_adduser_callback($form, $form_state) {

  $commands = array();

  $commands[] = ajax_command_html('#adduser-error', '');
  $commands[] = ajax_command_html('#adduser-warning', '');
  $commands[] = ajax_command_html('#adduser-status', '');

  $commands[] = ajax_command_invoke('#adduser-error', 'fadeOut');
  $commands[] = ajax_command_invoke('#adduser-warning', 'fadeOut');
  $commands[] = ajax_command_invoke('#adduser-status', 'fadeOut');

  if (!wildfire_valid_email_address($form_state['values']['adduser_email'])) {

    $commands[] = ajax_command_html(
      '#adduser-error',
      t('Please enter a valid email address.')
    );

    $commands[] = ajax_command_invoke('#adduser-error', 'fadeIn');

    return array('#type' => 'ajax', '#commands' => $commands);

  }
  else {

    $query = db_select('users', 'u')
      ->fields('u', array(
        'uid',
      ));
    $query->condition('mail', $form_state['values']['adduser_email'], '=');
    $result = $query->execute();

    $uid = $result->fetchField();

    if (!$uid) {

      /*
       * If there are extra fields to fill in, show the newuser fieldset so that
       * the end user can fill them in
       * If there aren't any additional fields, skip over and just perform the
       * new user save.
       */

      $has_fields = FALSE;

      if (module_exists('field')) {
        $fields = field_info_instances('user');
        if (!empty($fields['user'])) {
          $has_fields = TRUE;
        }
      }

      if ($has_fields) {
        $commands[] = ajax_command_invoke('#edit-newuser-fieldset', 'slideDown');
        $commands[] = ajax_command_invoke('#edit-adduser-email', 'attr', array('readonly', 'readonly'));
        return array('#type' => 'ajax', '#commands' => $commands);
      }
      else {
        // Call the new user callback, skipping the field entry stage.
        return wildfire_lists_newuser_callback($form, $form_state);

      }

    }
    else {

      /*
       * If the user is valid and already subscribed, say so and skip
       * adding the user to the list.
       */

      module_load_include('module', 'wildfire_lists', 'wildfire_lists');
      if (wildfire_user_is_subscribed($uid, $form_state['values']['lid'])) {

        $commands[] = ajax_command_html(
          '#adduser-warning',
          t(
            'The user %email is already in the list.',
            array(
              '%email' => $form_state['values']['adduser_email'],
            )
          )
        );

        $commands[] = ajax_command_invoke('#adduser-warning', 'fadeIn');
        return array('#type' => 'ajax', '#commands' => $commands);

      }

      $add_user_commands = wildfire_lists_adduser_add_listrow(
        $form_state['values']['adduser_email'],
        $uid,
        $form_state['values']['lid']
      );
      if (!empty($add_user_commands) && is_array($add_user_commands)) {
        $commands = array_merge($commands, $add_user_commands);
      }

      $commands[] = ajax_command_invoke('#adduser-status', 'fadeIn');
      $commands[] = ajax_command_invoke('#edit-adduser-email', 'val', array(''));
      $commands[] = ajax_command_invoke('#edit-adduser-email', 'removeAttr', array('readonly'));

      return array('#type' => 'ajax', '#commands' => $commands);

    }
  }

}

/**
 * Adds a new user to the list via an AJAX callback. Uses Drupal FAPI
 *
 * @param $form array
 *    The form array
 * @param $form_state array
 *    The form state
 *
 * @return array
 *    An array of AJAX commands that get converted to JSON and processed
 *    client-side
 */
function wildfire_lists_newuser_callback($form, $form_state) {

  $commands = array();

  $edit = array();

  $edit['mail'] = $form_state['values']['adduser_email'];
  $edit['name'] = wildfire_generate_username($form_state['values']['adduser_email']);
  $edit['pass'] = wildfire_random_password();
  $edit['status'] = 1;

  /**
   * TODO: Abstract users so that list entries only need a user account for
   * custom fields. See Codebase ticket #145
   */
  $account = user_save(NULL, $edit);

  if ($account !== FALSE) {

    $add_user_commands = wildfire_lists_adduser_add_listrow(
      $form_state['values']['adduser_email'],
      $account->uid,
      $form_state['values']['lid']
    );
    if (!empty($add_user_commands) && is_array($add_user_commands)) {
      $commands = array_merge($commands, $add_user_commands);
    }

    $commands[] = ajax_command_invoke('#edit-newuser-fieldset', 'slideUp');
    $commands[] = ajax_command_invoke('#edit-newuser-fieldset input[name^=field]', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-adduser-email', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-adduser-email', 'removeAttr', array('readonly'));

  }

  return array('#type' => 'ajax', '#commands' => $commands);

}

/**
 * Adds a row to the users list via AJAX commands.
 * Helper function used by AJAX callbacks.
 *
 * Used by :
 *    wildfire_lists_adduser_callback()
 *    wildfire_lists_newuser_callback()
 *
 * @param $email
 *    The email address to use as a label for the profile link
 * @param $uid
 *    The Users UID for the row to be added
 * @param $lid
 *    The List LID for the row to be added
 *
 * @return array
 *    An array of AJAX commands that get converted to JSON and processed
 *    client-side
 */
function wildfire_lists_adduser_add_listrow($email, $uid, $lid) {

  $commands = array();

  // Add the user to the relevant list.
  wildfire_lists_user_add($uid, $lid);

  $commands[] = ajax_command_html(
    '#adduser-status',
    t(
      'The user %email was added successfully.',
      array(
        '%email' => $email,
      )
    )
  );

  $commands[] = ajax_command_invoke('#adduser-status', 'fadeIn');

  $user_table_row = wildfire_lists_construct_user_list_row(
    $email,
    $uid,
    $lid,
    TRUE
  );

  // Theme the row, ready to insert the resulting markup into the page.
  $table_row =  theme(
    'wildfire_list_row',
    array(
      'row' => $user_table_row,
      'uid' => $uid,
    )
  );

  $commands[] = ajax_command_prepend('#wildfire-userlist>tbody', $table_row);

  $commands[] = ajax_command_remove('#list-empty-row');

  return $commands;

}

/**
 * FAPI form to confirm the deletion of a list.
 */
function wildfire_lists_delete_form($form, &$form_state, $lid) {
  // Sanity check. Redirect to lists list if a stupid list ID was
  // provided.
  $list = wildfire_list_get_name($lid);

  if (!$list) {
    drupal_goto('admin/wildfire/lists');
  }

  $form = array();

  $form['lid'] = array(
    '#type' => 'hidden',
    '#value' => arg(3),
  );
  $form['message'] = array(
    '#markup' => t('Are you sure you want to delete the list %list? This will not delete the users in this list, but will remove them from the list. There is no way to undo this.', array('%list' => $list)),
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

/**
 * Submit handler for wildfire_lists_delete_form().
 */
function wildfire_lists_delete_form_submit($form, &$form_state) {
  $list = wildfire_list_get_name($form_state['values']['lid']);

  if (wildfire_lists_delete($form_state['values']['lid'])) {
    drupal_set_message(t('List %name has been deleted.', array('%name' => $list)));
  }
  else {
    drupal_set_message(t('The list with ID %id was not found and could not be deleted.', array('%id' => $form_state['values']['lid'])), 'warning');
  }
}

/**
 * Menu callback to edit a given mailing list.
 *
 * @param $lid
 *    The lid of the list that should be edited.
 *
 * @return array
 *  A drupal FAPI form for the edit form, or an empty array if $lid is invalid
 */
function wildfire_lists_edit_form($form, &$form_state, $lid = 0) {

  if ($lid) {
    // Make sure the list exists before we try to edit it.
    $listname = wildfire_list_get_name($lid);

    if ($listname) {
      // Get the base form and add to it.
      $form = _wildfire_lists_form();

      $form['listname']['#default_value'] = check_plain($listname);
      $form['lid'] = array(
        '#type' => 'hidden',
        '#value' => $lid,
      );

      return $form;
    }
    else {
      // List was not found in the database, so give the error and redirect to
      // the main lists page.
      drupal_set_message(
        t('Could not edit list with ID %lid: list was not found.', array('%lid' => $lid)),
        'error'
        );
      drupal_goto('admin/wildfire/lists');
    }
  }
  else {
    // List ID was not provided, so redirect to the main lists page.
    drupal_goto('admin/wildfire/lists');
  }

  return array();

}

/**
 * Submit handler for wildfire_lists_edit_form().
 */
function wildfire_lists_edit_form_submit($form, &$form_state) {
  $record['lid'] = $form_state['values']['lid'];
  $record['name'] = check_plain($form_state['values']['listname']);

  drupal_write_record('wildfire_lists', $record, array('lid'));
  drupal_set_message(t('The changes to list %list have been saved.', array('%list' => $record['name'])));
}

/**
 * Page callback for the page used to export lists.
 */
function wildfire_lists_export_form($form, &$form_state, $lid) {
  // Check parameter.
  if (!wildfire_list_valid($lid)) {
    wildfire_error(
      'No list with ID "@lid" exists.',
      array('@lid' => $lid),
      drupal_goto('admin/wildfire/lists')
    );
  }

  $totals = wildfire_list_get_totals($lid);
  $options = wildfire_options_list_statuses();

  // Append the number of users in each status to the name of the status on the
  // checkbox label. For example: Subscribed (1337).
  foreach ($options as $key => &$option) {
    $option .= ' (' . $totals[$key] . ')';
  }

  $intro = t(
    'You are about to export from the @listname list, which contains a total of @total.',
    array(
      '@listname' => $totals['name'],
      '@total' => format_plural($totals['total'], '1 user', '@count users'),
    )
  );

  $form = array();

  $form['lid'] = array(
    '#type' => 'hidden',
    '#value' => $lid,
  );
  $form['intro'] = array(
    '#markup' => '<p>' . $intro . '</p>',
  );
  $form['statuses'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Status'),
    '#options' => $options,
    '#default_value' => array(),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  return $form;
}

/**
 * Validation handler for wildfire_lists_export_form().
 */
function wildfire_lists_export_form_validate($form, &$form_state) {
  $at_least_one_checked = FALSE;

  // Make sure the user has checked at least one type of status to export.
  foreach ($form_state['values']['statuses'] as $key => $status) {
    if ($status == $key) {
      $at_least_one_checked = TRUE;
      break;
    }
  }

  if (!$at_least_one_checked) {
    form_set_error('statuses', t('You must check at least one status to be exported.'));
  }
}

/**
 * Submit handler for wildfire_lists_export_form().
 */
function wildfire_lists_export_form_submit($form, &$form_state) {
  // The statuses on the form are in key => value format where the key is the
  // status such as WILDFIRE_STATUS_SUBSCRIBED, and the value is 0 or 1
  // (unchecked or checked). Take all the checked ones and put their statuses in
  // our own array.
  $statuses = array();

  foreach ($form_state['values']['statuses'] as $key => $status) {
    if ($status == $key) {
      $statuses[] = $key;
    }
  }

  wildfire_lists_export($form_state['values']['lid'], $statuses);
}

/**
 * Returns the add or edit form for a list. Used internally by the add and edit
 * list menu callbacks.
 *
 * @return array
 *    A Drupal form represented as an array.
 */
function _wildfire_lists_form() {
  $form = array();

  $form['listname'] = array(
    '#type' => 'textfield',
    '#title' => t('List name'),
    '#required' => TRUE,
    '#maxlength' => 128,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Multi-step form handler for importing lists from a CSV file.
 */
function wildfire_lists_import_form($form, &$form_state = NULL) {
  // Add CSS. Don't preprocess. It'll be used rarely.
  drupal_add_css(drupal_get_path('module', 'wildfire_lists') . '/css/wildfire.upload.css', array('preprocess' => FALSE));

  // Add the base path as a Drupal.* setting in JavaScript so we can use it to
  // construct the path to which the AJAX form should submit.
  drupal_add_js(
    array(
    'base_path' => base_path(),
  ), array('type' => 'setting'
  , 'scope' => JS_DEFAULT));

  $form = array();

  // If we're just entering the form for the first time, make sure we set the
  // correct step.
  $step = isset($form_state['storage']['step']) ? (int) $form_state['storage']['step'] : 0;
  $form_state['storage']['step'] = $step + 1;
  $submit_name = t('Submit');

  switch ($form_state['storage']['step']) {
    case 1:
      // Get the maximum size of a file upload in bytes. Multiply because the
      // ini setting is measured in megabytes.
      $maxbytes = wildfire_get_max_upload_size(TRUE);

      // Prepare some help text. Uploading files is going to be complicated for
      // someone with no computer skillz, so make it VERY clear.
      $helptext = t(
        'Choose a CSV file to import from your local computer. Please note:'
        . ' The maximum size is !maxsize.'
        . ' Large files might take some time to upload, depending on the speed of your connection. Please be patient.'
        . ' Microsoft Excel files must be exported to CSV format before they can be accepted.',
        array(
          '!maxsize' => $maxbytes
        )
      );

      $form['#attributes'] = array('enctype' => "multipart/form-data");
      $submit_name = t('Upload');

      $form['csvfile'] = array(
        '#type' => 'file',
        '#title' => t('CSV file'),
        '#description' => check_plain($helptext),
      );

      break;

    case 2:
      $tfcols = $form_state['storage']['tfcols'];
      $headers = $form_state['storage']['headers'];

      // Add a custom theme function for this step of the form so that we can
      // render the XHTML table properly.
      $form['#theme'] = 'wildfire_lists_import_form_step2';

      $form['linecount'] = array(
        '#markup' => '<p>' . t(
          '@file has been uploaded. It contains @lines.',
      array(
          '@file' => array_pop(explode('/', $form_state['storage']['csvfile'])),
          '@lines' => format_plural($form_state['storage']['linecount'], '1 line', '@count lines'),
        )
      ) . '</p>',
      );
      $form['importhelp'] = array(
        '#markup' => '<p>' . t('In the table below, each row represents a field from the CSV file that has been uploaded. For each field, choose the most appropriate place to import it by using the drop-down selector in the rightmost column.') . '</p>',
      );

      $options = array(
        'skip' => t('Do not import'),
      );

      foreach ($tfcols as $key => $tfcol) {
        $options[$key] = $tfcol['title'];
      }

      // Build the form elements that will appear in the rows of the table.
      foreach ($headers as $key => $colname) {
        $form['field'][$key] = array('#markup' => $colname);

        if (isset($firstrow)) {
          $form['sample'][$key] = array('#markup' => $firstrow[$key]);
        }

        $form['importdest'][$key] = array(
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => 'skip',
          '#attributes' => array(
            'id' => 'edit-' . $key,
          )
        );

        // If the value of the sample or the column name looks like an email
        // address, automatically alter the default value of the select form element
        // to choose Email.
        if (wildfire_valid_email_address($colname) || (isset($firstrow) && wildfire_valid_email_address($firstrow[$key]))) {
          $form['importdest'][$key]['#default_value'] = 'mail';
        }
      }

      $form['importlist'] = array(
        '#type' => 'select',
        '#title' => t('Destination list'),
        '#description' => t('Choose the list into which the data should be imported.'),
        '#options' => wildfire_lists_import_get_lists(),
      );

      $form['newlistname'] = array(
        '#type' => 'textfield',
        '#title' => t('New list name'),
        '#maxlength' => 128,
      );

      $form['file'] = array(
        '#type' => 'hidden',
        '#value' => $form_state['storage']['csvfile'],
        '#attributes' => array(
          'id' => 'edit-file',
        )
      );

      $submit_name = t('Import');

      break;

    case 3:
      // Use a custom theme function for the final stage of the form.
      $form['#theme'] = 'wildfire_lists_import_form_step3';

      // Get the results of the batch from the variable we set during batch
      // processing.
      $results = variable_get('wildfire_batch_results', t('No batch results could be found.'));

      // If the results are an array, it indicates a batch success. Otherwise,
      // $results will contain the error message.
      if (!is_array($results)) {
        drupal_set_message(check_plain($results), 'error');
      }
      else {
        $form['go_to_list'] = array(
          '#markup' => l(
          t('View the list »'),
            'admin/wildfire/lists/' . $form_state['storage']['lid'] . '/view'
          )
        );
      }
      break;
  }

  if ($form_state['storage']['step'] < 3) {
    $form['importsubmit'] = array(
      '#type' => 'submit',
      '#value' => $submit_name,
    );
  }

  return $form;
}

/**
 * Submit handler for wildfire_lists_import_form().
 */
function wildfire_lists_import_form_submit($form, &$form_state) {

  switch ($form_state['storage']['step']) {
    case 1:
      $error = FALSE;

      // Set up some limits for the uploaded file. CSV files only, and use PHP's
      // maximum file size as the max.
      $maxbytes = wildfire_get_max_upload_size();
      $validators = array(
        'file_validate_extensions' => array('csv'),
        'file_validate_size' => array($maxbytes),
      );

      // Save the file from the form.
      /**
       * 2011-09-14, craig: file_directory_path() no longer available,
       * read out global var that defines the equivalent instead
       * See: http://drupal.org/update/modules/6/7#file_directory_path
       */
      $file = file_save_upload(
        'csvfile',
        $validators,
        //'public://' . variable_get('file_public_path', conf_path() . '/files')
        'temporary://'
      );

      // Check that nothing has gone wrong with the upload.
      if (!$file) {
        // Oops, something went wrong. Go back a step.
        drupal_set_message(
          t(
            'The file could not be uploaded. Please check that it is a valid CSV file, and is under %size in size.',
            array(
          '%size' => format_size($maxbytes),
        )
          ),
          'error'
        );
        $form_state['storage']['step']--;
      }
      else {
        // File uploaded successfully. Open it up and try to guess whether the
        // first row contains column names.
        $handle = fopen($file->uri, "r");

        // Get the first line of the CSV file.
        $headers = fgetcsv($handle);
        $firstrow = NULL;

        // Check to see if the first row contains an email address. If not, it
        // could be the header containing the column names, in which case the
        // second row should contain an email address.
        if (!wildfire_row_contains_email_address($headers)) {
          $firstrow = fgetcsv($handle);
          if (wildfire_row_contains_email_address($firstrow)) {
            // The second row contains an email address, so the first row
            // contains the column names.
          }
          else {
            // Neither the first nor second row contained an email address. We
            // can't work with this file.
            drupal_set_message(
              t('The uploaded file does not contain any email addresses in the first or second rows. Please check that this is a valid CSV file and try again.'),
              'error'
            );
            $form_state['storage']['step']--;
          }
        }
        else {
          // The first row DOES contain an email address, so this CSV file has
          // no headers. Present the user with a choice of column values instead
          // of names.
        }

        if ($handle) {
          fclose($handle);
        }

        if (!$error) {
          // Now, $headers contains EITHER the list of column names OR the first
          // row's column values. If it contains column names, $firstrow
          // contains their values.

          // Set up a list of wildfire's column names, against which the
          // CSV columns can be mapped. Email is ALWAYS present.
          $tfcols = array(
            'mail' => array(
              'fid' => 0,
              'title' => t('Email address'),
              'explanation' => t(''),
              'type' => t(''),
            ),
          );

          // If the 'fields' module exists, lookup what other fields we can use
          if (module_exists('field')) {
            $fields = field_info_instances('user');

            $c = 0;
            foreach ($fields['user'] as $field) {
              // We can only reliably import into text fields...
              if ($field['widget']['type'] == 'text_textfield') {
                $tfcols[$field['field_name']] = array(
                  'fid' => $c++,
                  'title' => $field['label'],
                  'explanation' => $field['description'],
                  'type' => t(''),
                );
              }
            }
          }

          $form_state['storage']['tfcols'] = $tfcols;
          $form_state['storage']['headers'] = $headers;
          $form_state['storage']['csvfile'] = $file->uri;
          $form_state['storage']['linecount'] = wildfire_get_lines_in_file($file->uri);

          // Don't forget to trigger Drupal 7's multistep form API...
          $form_state['rebuild'] = TRUE;

        }
      }
      break;

    case 2:
      // First, check that at least one column is mapped to 'mail' so that new
      // users have an email address.
      $c = 0;
      $mailpresent = FALSE;

      while (isset($form_state['values'][$c]) && $c < 1000) {
        if ($form_state['values'][$c] == 'mail') {
          $mailpresent = TRUE;
          break;
        }
        $c++;
      }

      if (!$mailpresent) {
        // None of the columns were mapped to mail, so give the user a warning.
        drupal_set_message(t('One column must be mapped to email address, otherwise import is not possible.'), 'warning');
        break;
      }

      // If the user has chosen to add a new list, add one, if max has not been
      // reached.
      $lid = 0;

      if ($form_state['values']['importlist'] == 0) {

        // Check that there was a list name filled in and that it doesn't
        // contain odd characters.
        $listname = '';

        if (empty($form_state['values']['newlistname'])) {
          $listname = 'Untitled list';
        }
        else {
          $listname = check_plain($form_state['values']['newlistname']);
        }

        $lid = wildfire_lists_add_list($listname);

      }
      else {
        // The user is inserting to an existing list, so take its list ID.
        $lid = $form_state['values']['importlist'];
      }

      // Store the $lid into state, as we use it later...
      $form_state['storage']['lid'] = $lid;

      // Set up a batch to go through each row of the file and attempt to
      // process it.
      $batch = array(
        'operations' => array(
          array(
            'wildfire_list_batch_import_process',
            array(
              $form_state['storage']['csvfile'],
              $form_state['storage']['lid'],
              $form_state['storage']['linecount'],
              $form_state['values'],
            ),
          ),
        ),
        'finished' => 'wildfire_list_batch_import_finished',
        'title' => t('Import batch'),
        'init_message' => t('Import is starting.'),
        'progress_message' => t('Processing. Please wait…'),
        'error_message' => t('Import has encountered an error.'),
        'file' => drupal_get_path('module', 'wildfire_lists') . '/wildfire_lists.api.inc',
      );
      batch_set($batch);

      // Don't forget to trigger Drupal 7's multistep form API...
      $form_state['rebuild'] = TRUE;

      break;
  }
}

/**
 * Form for setting up the options for a list clean
 *
 * @param array $form
 *  The Drupal FAPI form
 * @param array $form_state
 *  The form state
 * @param int $lid
 *  The list ID to clean
 *
 * @return array
 *  The Drupal FAPI form
 */
function wildfire_lists_clean_form($form, &$form_state, $lid) {

  if (!wildfire_list_valid($lid)) {
    drupal_set_message(
      t(
        '!lid is not a valid list ID',
        array(
          '!lid' => $lid
        )
      )
    );

    drupal_goto('admin/wildfire/lists');
  }

  $form = array();

  $form['lid'] = array(
    '#type' => 'hidden',
    '#value' => $lid,
  );

  $form['clean_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('List clean options'),
    '#collapsible' => FALSE,
  );

  $form['clean_fieldset']['remove-duplicates'] = array(
    '#type' => 'checkbox',
    '#title' => 'Remove duplicate addresses',
    '#default_value' => TRUE,
  );

  $form['clean_fieldset']['remove-invalid'] = array(
    '#type' => 'checkbox',
    '#title' => 'Remove invalid addresses',
    '#default_value' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'edit-actions',
      'class' => array('form-actions'),
    )
  );

  $form['actions']['clean_list'] = array(
    '#type' => 'submit',
    '#value' => t('Clean list'),
    '#attributes' => array(
      'class' => array('button-yes'),
    )
  );

  return $form;

}

/**
 * Submit handler for the list clean form
 *
 * @param array $form
 *  The Drupal FAPI form
 * @param array $form_state
 *  The form state
 *
 * @return array
 *  The Drupal FAPI form
 */
function wildfire_lists_clean_form_submit($form, &$form_state) {

  // Set the default options.
  $options = array(
    'remove-duplicates' => FALSE,
    'remove-invalid' => FALSE,
  );

  foreach($options as $key => $default_value) {
    if ($form_state['values'][$key] == 1) {
      $options[$key] = TRUE;
    }
  }

  if (wildfire_list_clean($form['lid']['#value'], $options)) {
    // All done; redirect the user to the lists admin
    $form_state['redirect'] = 'admin/wildfire/lists';
  }

}

/**
 * Menu callback to show a list of all the email lists.
 */
function wildfire_lists_list($arg1 = NULL, $arg2 = NULL) {
  $headers = array(
    array(
      'data' => t('Name'),
      'field' => 'name',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Size'),
      'field' => 'count',
    ),
    array(
      'data' => t('Unsubscribed'),
    ),
    array(
      'data' => t('Active'),
    ),
    array(
      'data' => t('Operations'),
    ),
  );

  $query = db_select('wildfire_lists', 'cl')->extend('PagerDefault');
  $query->limit(50);
  $query->fields('cl', array(
      'lid',
      'name',
    ));
  $query->orderBy('name', 'ASC');
  $query->orderBy('count', 'ASC');
  $query->addExpression('(SELECT COUNT(1) FROM {wildfire_lists_users} WHERE lid = cl.lid)', 'count');
  $query->condition('status', WILDFIRE_LIST_STATUS_LIVE, '=');
  $result = $query->execute();

  while ($dbrow = $result->fetchAssoc()) {
    $row = array();
    $totals = wildfire_list_get_totals($dbrow['lid']);

    $row[] = $dbrow['name'];
    $row[] = $dbrow['count'];
    $row[] = $totals[WILDFIRE_STATUS_UNSUBSCRIBED];
    $row[] = $totals[WILDFIRE_STATUS_SUBSCRIBED];

    $options = array();

    $options[] = l(
      t('view'),
      'admin/wildfire/lists/' . $dbrow['lid'] . '/view',
      array('attributes' => array('title' => t('View the contents of this list, add, or remove users')))
    );
    $options[] = l(
      t('edit'),
      'admin/wildfire/lists/' . $dbrow['lid'] . '/edit',
      array('attributes' => array('title' => t('Edit this list')))
    );
    $options[] = l(
      t('delete'),
      'admin/wildfire/lists/' . $dbrow['lid'] . '/delete',
      array('attributes' => array('title' => t('Delete this list')))
    );
    $options[] = l(
      t('export'),
      'admin/wildfire/lists/' . $dbrow['lid'] . '/export',
      array('attributes' => array('title' => t('Export this list')))
    );

    if (user_access('lock unlock wildfire lists')) {

      // Add extra links to lock / unlock lists
      if (wildfire_list_is_locked($dbrow['lid'])) {
        $options[] = l(
          t('unlock'),
          'admin/wildfire/lists/' . $dbrow['lid'] . '/lock',
          array('attributes' => array('title' => t('Unlock the list. USE WITH CAUTION!')))
        );
      }
      else {
        $options[] = l(
          t('lock'),
          'admin/wildfire/lists/' . $dbrow['lid'] . '/lock',
          array('attributes' => array('title' => t('Lock the list. USE WITH CAUTION!')))
        );
      }

    }

    if (user_access('clean wildfire lists')) {

      // Add extra links to manually clean lists
      $options[] = l(
        t('clean'),
        'admin/wildfire/lists/' . $dbrow['lid'] . '/clean',
        array('attributes' => array('title' => t('Clean the list by removing duplicate or invalid email addresses')))
      );

    }

    $row[] = implode(' | ', $options);

    $rows[] = $row;
  }

  // Handle the situation when there are no lists.
  if (!isset($rows) || !count($rows)) {
    $rows[] = array(
      array(
        'colspan' => count($headers),
        'data' => t(
          'No lists. !add',
          array(
            '!add' => l(
              t('Add one?'),
              'admin/wildfire/lists/add',
              array('attributes' => array('title' => t('Add a new list')))
            ),
          )
        ),
      ),
    );
  }

  $output = theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'wildfire-lists')));

  $output .= theme('pager', array('tags' => NULL));

  return $output;
}

/**
 * Form handler for managing which lists a given user is in.
 *
 * @param $uid
 *    The uid of the user whose list mappings are being edited.
 */
function wildfire_lists_user_lists_form($form, &$form_state, $uid = 0) {
  $form = array();
  $options = wildfire_lists_get();
  $listcount = wildfire_lists_count();

  if ($listcount == 0) {
    drupal_set_message(
      t(
        'There are no mailing lists. !add',
        array(
          '!add' => l(
            t('Add one?'),
            'admin/wildfire/lists/add',
            array('attributes' => array('title' => t('Add a new list')))
          ),
        )
      ),
      'warning'
      );
  }
  else {
    // Work out which lists the user is already part of.
    $default_values = array();

    $query = db_select('wildfire_lists_users', 'clu');
    $query->fields('clu', array(
      'lid'
    ));
    $query->condition('uid', $uid, '=');
    $result = $query->execute();

    while ($dbrow = $result->fetchAssoc()) {
      $default_values[] = $dbrow['lid'];
    }

    $form['lists'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Lists'),
      '#description' => t('Check the boxes for each list the user should be in.'),
      '#options' => $options,
      '#default_value' => $default_values,
    );
    $form['uid'] = array(
      '#type' => 'hidden',
      '#value' => $uid,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  return $form;
}

/**
 * Submit handler for wildfire_lists_user_lists_form().
 */
function wildfire_lists_user_lists_form_submit($form, &$form_state) {
  // Filter out just the checkboxes that were turned on (ie not 0, or FALSE).
  $options = array_filter($form_state['values']['lists']);
  $uid = $form_state['values']['uid'];

  // Delete all list mappings for this user.
  db_delete('wildfire_lists_users')
    ->condition('uid', $uid)
    ->execute();

  // Add new mappings for each list whose box was checked.
  foreach ($options as $lid) {
    $record = array(
      'lid' => $lid,
      'uid' => $uid,
    );
    drupal_write_record('wildfire_lists_users', $record);
  }

  drupal_set_message(t('List mappings updated'));
}

/**
 * AJAX callback. Removes the specified user from the specified list.
 *
 * This works slightly differently to the other AJAX calls, as it's triggered
 * from a link not a form submission.
 *
 * @param $uid int
 *    The user ID of the user who should be removed.
 * @param $lid int
 *    The list ID of the list from which the user should be removed.
 *
 * @return string
 *    JSON string containing AJAX actions that are parsed client-side
 */
function wildfire_lists_user_remove_ajax($uid = 0, $lid = 0) {

  $commands = array();

  $commands[] = ajax_command_html('#adduser-error', '');
  $commands[] = ajax_command_html('#adduser-warning', '');
  $commands[] = ajax_command_html('#adduser-status', '');

  $commands[] = ajax_command_invoke('#adduser-error', 'fadeOut');
  $commands[] = ajax_command_invoke('#adduser-warning', 'fadeOut');
  $commands[] = ajax_command_invoke('#adduser-status', 'fadeOut');

  // Check that the list ID is valid.
  if (!wildfire_list_valid($lid)) {

    $commands[] = ajax_command_html(
      '#adduser-error',
      t('Could not remove user ID %uid from list ID %lid: list not found.', array('%uid' => $uid, '%lid' => $lid))
    );

    $commands[] = ajax_command_invoke('#adduser-error', 'fadeIn');

  }
  // Check that the uid is valid. Only bother checking if there wasn't a problem
  // with the lid.
  else {
    if (!wildfire_valid_user($uid)) {

      $commands[] = ajax_command_html(
        '#adduser-error',
        t('Could not remove user ID %uid from list with ID %lid: user not found.', array('%uid' => $uid, '%lid' => $lid))
      );

      $commands[] = ajax_command_invoke('#adduser-error', 'fadeIn');

    }
  }

  // Get the list name. Used in response messages.
  $listname = wildfire_list_get_name($lid);

  // Check that the user exists.
  $account = user_load($uid);
  if (isset($account)) {
    if (wildfire_lists_user_remove($uid, $lid)) {

      $commands[] = ajax_command_html(
        '#adduser-status',
        t('User %email was removed from list %list.', array('%email' => $account->mail, '%list' => check_plain($listname)))
      );

      $commands[] = ajax_command_invoke('#adduser-status', 'fadeIn');

      $commands[] = ajax_command_remove('#list-row-' . $uid);

      // The UI should represent the correct state, so we can use this API
      // function to determine if the list is empty, rather than reading the DOM
      if (wildfire_list_get_count($lid) == 0) {

        // Theme the row, ready to insert the resulting markup into the page.
        $table_row =  theme(
          'wildfire_list_row',
          array(
            'row' => array()
          )
        );

        $commands[] = ajax_command_append('#wildfire-userlist>tbody', $table_row);

      }

    }
    else {

      $commands[] = ajax_command_html(
        '#adduser-error',
        t('User %email was not in list %list. No action taken.', array('%email' => $account->mail, '%list' => check_plain($listname)))
      );

      $commands[] = ajax_command_invoke('#adduser-error', 'fadeIn');

    }
  }

  print ajax_render($commands);
  exit;

}

/**
 * Menu callback to list the users in a mailing list.
 *
 * @param $lid
 *    The list ID of the list to be displayed.
 *
 * @return string
 *  HTML representing the page.
 */
function wildfire_lists_view($lid = 0) {

  // Check that this is a valid list ID.
  if (!$lid) {
    drupal_goto('admin/wildfire/lists');
  }

  $listname = wildfire_list_get_name($lid);

  if (!$listname) {
    drupal_set_message(
      t('Could not view list with ID %lid: list not found.', array('%lid' => $lid)),
      'error'
    );
    drupal_goto('admin/wildfire/lists');
  }

  drupal_add_css(drupal_get_path('module', 'wildfire_lists') . '/css/wildfire.lists.css');
  drupal_add_js(drupal_get_path('module', 'wildfire_lists') . '/js/wildfire.lists.js');

  drupal_set_title(t('List @list', array('@list' => $listname)), PASS_THROUGH);

  $headers = array(
    t('Email'),
    t('Status'),
    t('Operations'),
  );

  $query = db_select('wildfire_lists_users', 'clu')->extend('PagerDefault');
  $query->limit(50);
  $query->fields('clu', array(
      'uid',
      'locked',
      'mail'
    ));
  $query->fields('u', array(
      'mail',
    ));
  $query->leftJoin('users', 'u', 'clu.uid = u.uid');
  $query->leftJoin('wildfire_optouts', 'co', 'clu.uid = co.uid');
  $query->condition('clu.lid', $lid, '=');
  $query->addExpression('CASE WHEN co.uid IS NULL THEN 1 ELSE 0 END', 'subscribed');
  $result = $query->execute();

  $rows = array();

  while ($dbrow = $result->fetchAssoc()) {
    $row = array();

    /**
     * 2011-08-22, craig: If the 'mail' field doesn't contain anything,
     * the user joined at uid doesn't actually exist. Therefore, there's
     * nothing to link to and thus we should display it as such.
     *
     * The entries in wildfire_lists_users should be deleted on user
     * deletion in any case now, so this is just a safeguard against faulty
     * records that may already exist.
     */

    // If the user is "pending removal", show this rather than
    // Subscribed / Unsubscribed
    if ($dbrow['locked'] == WILDFIRE_LIST_USER_PENDING_REMOVAL) {
      $subscribed = 2;
    }
    else {
      $subscribed = $dbrow['subscribed'] ? 1 : 0;
    }

    if (!empty($dbrow['u_mail'])) {

      $row = wildfire_lists_construct_user_list_row(
        $dbrow['u_mail'],
        $dbrow['uid'],
        $lid,
        $subscribed
      );

    }
    elseif (!empty($dbrow['mail']))

      $row = wildfire_lists_construct_user_list_row(
        $dbrow['mail']
      );

    else {

      $row = wildfire_lists_construct_user_list_row(
        t('Unknown / deleted user')
      );

    }

    $rows[] = array(
      'data' => $row,
      'id' => 'list-row-' . $dbrow['uid']
    );
  }

  if (!count($rows)) {
    $rows[] = array(
      array(
        'colspan' => count($headers),
        'data' => t('List is empty'),
        'id' => 'list-empty-row',
      ),
    );
  }

  $output = drupal_get_form('wildfire_lists_adduser_form');
  $output = drupal_render($output);

  $output .= theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'wildfire-userlist')));

  $output .= theme('pager', array('tags' => NULL));

  return $output;
}

/**
 * Grabs the contents of a test list via an AJAX callback. Uses Drupal FAPI
 *
 * @param $form array
 *    The form array
 * @param $form_state array
 *    The form state
 *
 * @return array
 *    An array of AJAX commands that get converted to JSON and processed
 *    client-side
 */
function wildfire_lists_test_list_callback($form, $form_state) {

  $commands = array();
  $recipient_addresses = array();

  $list = wildfire_list_get($form_state['values']['recently_used_tests'], TRUE);

  foreach ($list as $item) {
    $recipient_addresses[] = $item['mail'];
  }

  $recipient_addresses = implode(', ', $recipient_addresses);

  $commands[] = ajax_command_invoke('#edit-recipient-addresses', 'val', array($recipient_addresses));

  return array('#type' => 'ajax', '#commands' => $commands);

}

/**
 * Constructs an array representing a row in the user list table
 *
 * @param $profile_link_text string
 *    The text to use for the the profile link
 * @param $uid int
 *    The Users UID for the row to be added
 * @param $lid int
 *    The List LID for the row to be added
 * @param $subscribed int
 *    Set to 0 to display unsubscribed, 1 to display subscribed, 2 to
 *    display pending removal.
 *
 * @return array
 *    Array containing the row cells
 */
function wildfire_lists_construct_user_list_row($profile_view_text, $uid = 0, $lid = 0, $subscribed = 1) {

  $row = array();

  if (!empty($uid) && !empty($lid)) {

    $profile_link = l(
      $profile_view_text,
      'user/' . $uid . '/edit',
      array(
        'attributes' => array(
          'title' => t('View this user\'s profile')
        )
      )
    );

    $edit_link = l(
      t('edit'),
      'user/' . $uid . '/edit',
      array(
        'attributes' => array(
          'title' => t('Edit this user\'s profile')
        )
      )
    );

    $row[] = $profile_link . ' (' . $edit_link . ')';

  }
  else {
    $row[] = $profile_view_text;
  }

  switch ($subscribed) {
    case 0:
      $row[] = t('Unsubscribed');
    break;
    case 1:
      $row[] = t('Subscribed');
    break;
    case 2:
      $row[] = t('Pending removal');
    break;
    default:
      $row[] = t(
        'Unknown status !status',
        array(
          '!status' => $subscribed
        )
      );
    break;
  }

  $options = array();

  if ($uid > 0 && $lid > 0) {

    $options[] = l(
      t('remove'),
      'admin/wildfire/lists/user/remove/' . $uid . '/' . $lid,
      array(
        'attributes' => array(
          'class' => array(
            'operations-remove-' . $uid,
            'use-ajax',
          ),
          'id' => 'operations-remove-' . $uid,
          'title' => t('Remove this user from the list. Note that this will not'
            . ' delete the user and will not remove the user from any other lists.')
        ),
      )
    );

  }

  $row[] = implode(' | ', $options);

  return $row;

}

/**
 * Unsubscribe callback for when a user clicks an email link to unsubscribe.
 *
 * @param string $token
 *    The unsubscribe token.
 */
function wildfire_unsubscribe_page($token = '') {
  $outcome = 'fail';
  $mail = '';

  // Parameter checking. Make sure the token has the right number of characters.
  $token = check_plain($token);

  if (drupal_strlen($token) == WILDFIRE_UNSUBSCRIBE_LENGTH) {
    // Find the token in the database.
    $query = db_select('wildfire_job_mails', 'cjm')
      ->fields('cjm', array(
        'uid',
      ));
    $query->fields('u', array(
        'mail',
      ));
    $query->leftJoin('users', 'u', 'cjm.uid = u.uid');
    $query->condition('token', $token, '=');
    $result = $query->execute();

    $row = $result->fetchAssoc();

    if (is_array($row)) {
      $mail = $row['mail'];

      // Check that the user has not already opted out of everything. Note that
      // this only checks for global optouts.
      $query = db_select('wildfire_optouts', 'co')
        ->fields('co', array(
          'uid'
        ));
      $query->condition('uid', $row['uid'], '=');
      $query->condition('lid', 0, '=');
      $result = $query->execute();

      $optout = $result->fetchField();

      if ($optout === FALSE) {
        module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

        // User has not already opted out. Opt them out of everything.
        wildfire_list_unsubscribe($row['uid'], 0);
        $outcome = 'success';
      }
      else {
        // User has already opted out.
        $outcome = 'noaction';
      }
    }
    else {
      $outcome = 'invalidtoken';
    }
  }

  return theme('wildfire_list_unsubscribe', array('outcome' => $outcome, 'mail' => $mail));
}

/**
 * FAPI form fields to be added to either a user edit or registration form.
 *
 * @return array
 *    A FAPI form array.
 */
function wildfire_user_form() {
  // Get optout label and description for user profile edit page as this is
  // editable it needs to come from the variables table in the database
  $optout_text = wildfire_lists_user_get_optout_text();

  $form = array();

  $form['wildfire_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['wildfire_fieldset']['global_optout'] = array(
    '#type' => 'checkbox',
    '#title' => check_plain($optout_text['label']),
    '#description' => check_plain($optout_text['description']),
  );

  return $form;
}

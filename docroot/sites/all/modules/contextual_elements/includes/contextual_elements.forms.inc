<?php
/**
 * @file
 * Form, validations and submission functions.
 */
/**
 * The instance add/edit form.
 * FORM
 *
 * @param $contextual_element
 *   If set then we are editing that element.  If NULL we are creating a new element.
 */
function contextual_elements_form_element($form, &$form_state, $contextual_element = NULL) {
  // Set an appropriate page title.
  $title = (isset($contextual_element->cid) ? contextual_elements_title('edit', $contextual_element) : t('Add an Element'));
  drupal_set_title($title);

  $token = module_exists('token');
  $token_link = (!$token ? '<br />' . t('Install the !link for better token integration and a list of available substitutions.', array('!link' => l(t('Token module'), 'http://drupal.org/project/token', array('attributes' => array('target' => '_blank'))))) : '');

  // If the $contextual_element object was not passed in (new element) and the
  // session is set with an object, we likely encountered an error and should
  // repopulate the fields for the user.
  if (!isset($contextual_element) && isset($_SESSION['contextual_elements_add']) && is_object($_SESSION['contextual_elements_add'])) {
    $contextual_element = $_SESSION['contextual_elements_add'];
    $contextual_element->roles = unserialize($contextual_element->roles);
    unset($_SESSION['contextual_elements_add']);
  }

  // If we are editing a element, set a redirect back to the view
  if (isset($contextual_element->cid) && is_numeric($contextual_element->cid)) {
    $form['#redirect'] = "admin/config/search/contextual-elements/{$contextual_element->cid}/view";
  }

  $form['cid'] = array(
    '#type' => 'value',
    '#value' => (isset($contextual_element->cid) ? $contextual_element->cid : NULL),
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Provide an administrative label for this set of elements.'),
    '#maxlength' => 255,
    '#default_value' =>  (isset($contextual_element->name) ? $contextual_element->name : ''),
    '#required' => TRUE,
    '#weight' => -10,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('Provide a description for this set of elements.'),
    '#maxlength' => 255,
    '#default_value' =>  (isset($contextual_element->description) ? $contextual_element->description : ''),
    '#required' => FALSE,
    '#weight' => -9,
  );

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => '<strong>' . t('Disabled:')  . '</strong> ' . t('Elements are not rendered, regardless of filtering.') . '</br />' .
                      '<strong>' . t('Test Mode:') . '</strong> ' . t('HTML placeholders are rendered instead of the actual elements.') . '<br />' .
                      '<strong>' . t('Active:')    . '</strong> ' . t('Actual elements are rendered on the pages allowed by the filters.'),
    '#options' => _contextual_elements_opts('status'),
    '#default_value' => (isset($contextual_element->status) ? $contextual_element->status : 0),
    '#weight' => -8,
  );
  $form['debug'] = array(
    '#type' => 'select',
    '#title' => t('Debug'),
    '#description' => t('Display messages to users with debug permissions showing each filter result for this element.'),
    '#options' => _contextual_elements_opts('debug'),
    '#default_value' => (isset($contextual_element->debug) ? $contextual_element->debug : 0),
    '#weight' => -6,
  );

  // Variables
  $form['vars'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Javascript Variables'),
    '#description' => t('Define custom variables to be inserted into the page before your elements.') . '<br />' . t('You may use standard token replacement patterns.') . $token_link,
    '#collapsible' => TRUE,
    '#collapsed' => (empty($contextual_element->var_data) && empty($contextual_element->var_name)),
    '#weight' => -5,
  );
  $form['vars']['var_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Variable Name'),
    '#description' => t('The variable name may only contain letters, numbers and underscores and may not begin with a number.'),
    '#maxlength' => 64,
    '#default_value' => (isset($contextual_element->var_name) ? $contextual_element->var_name : ''),
  );
  $form['vars']['var_data'] = array(
    '#type' => 'textarea',
    '#title' => t('Variables'),
    '#description' => t('Enter key|value pairs to be included in the variables.<br />Separate pairs with a vertical bar. (key|value)<br />Enter one pair per line.'),
    '#rows' => 5,
    '#default_value' => (isset($contextual_element->var_data) ? $contextual_element->var_data : ''),
  );
  if ($token) {
    $form['vars']['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }
  
  // GA Vars
  // Google Analytics supports up to 5 custom variables.
  $form['ga_vars'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Analytics Variables'),
    '#description' => t('You can add Google Analytics <a href="!custom_var_documentation">Custom Variables</a> here. Google Analytics will only accept custom variables if the <em>name</em> and <em>value</em> combined are less than 64 bytes after URL encoding. Keep the names as short as possible and expect long values to get trimmed. You may use tokens in custom variable values. Global and user tokens are always available; on node pages, node tokens are also available.', array('!custom_var_documentation' => 'http://code.google.com/intl/en/apis/analytics/docs/tracking/gaTrackingCustomVariables.html')),
    '#theme' => 'contextual_elements_admin_gavar_table',
    '#collapsible' => TRUE,
    '#collapsed' => (empty($contextual_element->ga_vars_name)),
    '#weight' => -4,
    '#tree' => TRUE,
  );
  $form['ga_vars']['ga_vars_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Variable Name'),
    '#description' => t('Assign your GA variables to a specific name for your scripts below to reference.  May only contain letters, numbers and underscores and may not begin with a number.'),
    '#maxlength' => 64,
    '#default_value' => (isset($contextual_element->ga_vars_name) ? $contextual_element->ga_vars_name : ''),
  );
  for ($i = 1; $i < 6; $i++) {
    $form['ga_vars']['slots'][$i]['slot'] = array(
      '#default_value' => $i,
      '#description' => t('Slot number'),
      '#disabled' => TRUE,
      '#size' => 1,
      '#title' => t('Custom variable slot #@slot', array('@slot' => $i)),
      '#title_display' => 'invisible',
      '#type' => 'textfield',
    );
    $form['ga_vars']['slots'][$i]['name'] = array(
      '#default_value' => !empty($contextual_element->ga_vars[$i]['name']) ? $contextual_element->ga_vars[$i]['name'] : '',
      '#description' => t('The custom variable name.'),
      '#size' => 20,
      '#title' => t('Custom variable name #@slot', array('@slot' => $i)),
      '#title_display' => 'invisible',
      '#type' => 'textfield',
    );
    $form['ga_vars']['slots'][$i]['value'] = array(
      '#default_value' => !empty($contextual_element->ga_vars[$i]['value']) ? $contextual_element->ga_vars[$i]['value'] : '',
      '#description' => t('The custom variable value. You may use tokens in this field.'),
      '#title' => t('Custom variable value #@slot', array('@slot' => $i)),
      '#title_display' => 'invisible',
      '#type' => 'textfield',
      '#element_validate' => array('contextual_elements_token_element_validate'),
      '#token_types' => array('node'),
    );
    if ($token) {
      $form['ga_vars']['slots'][$i]['value']['#element_validate'][] = 'token_element_validate';
    }
    $form['ga_vars']['slots'][$i]['scope'] = array(
      '#default_value' => !empty($contextual_element->ga_vars[$i]['scope']) ? $contextual_element->ga_vars[$i]['scope'] : 3,
      '#description' => t('The scope for the custom variable.'),
      '#title' => t('Custom variable slot #@slot', array('@slot' => $i)),
      '#title_display' => 'invisible',
      '#type' => 'select',
      '#options' => array(
        1 => t('Visitor'),
        2 => t('Session'),
        3 => t('Page'),
      ),
    );
  }
  $form['ga_vars']['gavars_custom_var_description'] = array(
    '#type' => 'item',
    '#description' => t('You can supplement Google Analytics\' basic IP address tracking of visitors by segmenting users based on custom variables. Section 8.1 of the <a href="@ga_tos">Google Analytics terms of use</a> requires you to make sure you will not associate (or permit any third party to associate) any data gathered from your websites (or such third parties\' websites) with any personally identifying information from any source as part of your use (or such third parties\' use) of the Google Analytics\' service.', array('@ga_tos' => 'http://www.google.com/analytics/en-GB/tos.html')),
  );
  $form['ga_vars']['gavars_custom_var_token_tree'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
  );
  
  // Elements
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Elements'),
    '#description' => t('WARNING: Anything you enter in the boxes below will be rendered in the page markup.') . '<br />' . t('You may use standard token replacement patterns in either field.') . $token_link,
    '#collapsible' => TRUE,
    '#weight' => -3,
    '#attached' => array('css' => array(drupal_get_path('module', 'contextual_elements') . '/contextual_elements.css')),
  );

  $form['info']['header'] = array(
    '#type' => 'textarea',
    '#title' => t('Header Elements'),
    '#description' => t('Enter HTML elements to be added to the page header.'),
    '#default_value' => (isset($contextual_element->header) ? $contextual_element->header : ''),
    '#weight' => 4,
  );
  $form['info']['footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer Elements'),
    '#description' => t('Enter HTML elements to be added to the page footer.'),
    '#default_value' => (isset($contextual_element->footer) ? $contextual_element->footer : ''),
    '#weight' => 6,
  );
  if ($token) {
    $form['info']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('contextual_element'),
      '#weight' => 8,
    );
  }
  $form['info']['weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#options' => _contextual_elements_opts('weight'),
    '#default_value' => (isset($contextual_element->weight) ? $contextual_element->weight : 0),
    '#weight' => 10,
  );
  
  
  // Filters
  $form['filter_title'] = array(
    '#type' => 'item',
    '#title' => t('Filters'),
    '#weight' => -2,
  );
  $form['filters'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -1,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'contextual_elements') . '/contextual_elements.js'),
    ),
  );
  $form['domains'] = array(
    '#type' => 'fieldset',
    '#title' => t('Domains'),
    '#collapsible' => TRUE,
    '#group' => 'filters',
  );
  $form['domains']['domain_inc'] = array(
    '#type' => 'radios',
    '#title' => t('Include or Exclude Domains'),
    '#description' => t('Choose whether to include the following domains in the allowed list or exclude them.'),
    '#options' => _contextual_elements_opts('domain_inc'),
    '#default_value' => (isset($contextual_element->domain_inc) ? $contextual_element->domain_inc : 0),
  );
  $form['domains']['domains'] = array(
    '#type' => 'textarea',
    '#title' => t('Domains'),
    '#description' => t('List domains names or host names one per line.'),
    '#rows' => 3,
    '#default_value' => (isset($contextual_element->domains) ? $contextual_element->domains : ''),
  );
  $form['paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Paths'),
    '#collapsible' => TRUE,
    '#group' => 'filters',
  );
  $form['paths']['page_inc'] = array(
    '#type' => 'radios',
    '#title' => t('Include or Exclude Paths'),
    '#description' => t('Choose whether to include the following paths in the allowed list or exclude them.'),
    '#options' => _contextual_elements_opts('page_inc'),
    '#default_value' => (isset($contextual_element->page_inc) ? $contextual_element->page_inc : 0),
  );
  $form['paths']['pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => t('List paths one per line.'),
    '#rows' => 3,
    '#default_value' => (isset($contextual_element->pages) ? $contextual_element->pages : ''),
  );
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#group' => 'filters',
  );
  $form['roles']['role_inc'] = array(
    '#type' => 'radios',
    '#title' => t('Include or Exclude Roles'),
    '#description' => t('Choose whether to include or exclude the following roles.'),
    '#options' => _contextual_elements_opts('role_inc'),
    '#default_value' => (isset($contextual_element->role_inc) ? $contextual_element->role_inc : 0),
  );
  $form['roles']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#description' => t('Select roles to include or exclude.'),
    '#options' => user_roles(),
    '#default_value' => (isset($contextual_element->roles) ? $contextual_element->roles : array()),
  );
  $form['users'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Opt In/Out'),
    '#group' => 'filters',
  );
  $form['users']['users'] = array(
    '#type' => 'radios',
    '#title' => t('User Specific Filters'),
    '#description' => t('Users with the proper permissions may choose to opt-in or opt-out.'),
    '#options' => _contextual_elements_opts('users'),
    '#default_value' => (isset($contextual_element->users) ? $contextual_element->users : 0),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save elements'),
  );
  $form['cancel'] = array(
    '#type' => 'markup',
    '#value' => l(t('Cancel'), 'admin/config/search/contextual-elements'),
  );

  return $form;
}

/**
 * The instance add/edit form.
 * VALIDATE
 *
 * @TODO: Create validation procedures?
 *  This module should be available only to admins so how much do we want to
 *  trust them or limit what they can place into the headers and footers?
 */
function contextual_elements_form_element_validate($form, &$form_state) {
  $vals = $form_state['values'];

  // Names must be unique
  $select = db_select('contextual_elements', 'c')->fields('c')->condition('name', $vals['name'])->condition('cid', $vals['cid'], '<>');
  $cnt = $select->countQuery()->execute()->fetchField();

  if ($cnt > 0) {
    form_set_error('name', t('Name %name is already taken.', array('%name' => $vals['name'])));
  }

  // Require a variable name if anything is entered into the variables textarea.
  if (!empty($vals['var_data']) && empty($vals['var_name'])) {
    form_set_error('var_name', t('Variable name must be provided.'));
  }

  // Validate variable name as plain text.
  if (!empty($vals['var_name']) && !contextual_elements_machine_name($vals['var_name'])) {
    form_set_error('var_name', $form_state['var_name']['#description']);
  }

  // Validate variables formats.
  // Break apart variables.
  if ($vals['var_data']) {
    $vars = explode(PHP_EOL, $vals['var_data']);
    foreach ($vars as $pair) {
      list($key, $value) = explode('|', $pair);
      
      // Check the keys
      if (!contextual_elements_machine_name($key)) {
        form_set_error('var_data', t('The variable key \'%key\' may consist of only letters, numbers and underscores and may not begin with a number.', array('%key' => $key)));
      }
      
      // Check the values
      if (check_plain($value) != $value) {
        form_set_error('var_data', t('The value @value is not valid. Variable values may not consist of any special characters.', array('@value' => $value)));
      }
    }
  }
  
  // Ensure a GA var name is set if any GA vars are set.
  if (empty($vals['ga_vars']['ga_vars_name'])) {
    foreach ($vals['ga_vars']['slots'] as $slot) {
      if ($slot['name']) {
        form_set_error('ga_vars][ga_vars_name', t('You must assign a name for your custom GA variables.'));
        break;
      }
    }
  }
}

/**
 * Validate a form element that should have tokens in it.
 *
 * For example:
 * @code
 * $form['my_node_text_element'] = array(
 *   '#type' => 'textfield',
 *   '#title' => t('Some text to token-ize that has a node context.'),
 *   '#default_value' => 'The title of this node is [node:title].',
 *   '#element_validate' => array('gcontextual_elements_token_element_validate'),
 * );
 * @endcode
 */
function contextual_elements_token_element_validate(&$element, &$form_state) {
  $value = isset($element['#value']) ? $element['#value'] : $element['#default_value'];

  if (!drupal_strlen($value)) {
    // Empty value needs no further validation since the element should depend
    // on using the '#required' FAPI property.
    return $element;
  }

  $tokens = token_scan($value);
  $invalid_tokens = _contextual_elements_get_forbidden_tokens($tokens);
  if ($invalid_tokens) {
    form_error($element, t('The %element-title is using the following forbidden tokens with personal identifying information: @invalid-tokens.', array('%element-title' => $element['#title'], '@invalid-tokens' => implode(', ', $invalid_tokens))));
  }

  return $element;
}

function _contextual_elements_get_forbidden_tokens($value) {
  $invalid_tokens = array();
  $value_tokens = is_string($value) ? token_scan($value) : $value;

  foreach ($value_tokens as $type => $tokens) {
    if (array_filter($tokens, '_contextual_elements_contains_forbidden_token')) {
      $invalid_tokens = array_merge($invalid_tokens, array_values($tokens));
    }
  }

  array_unique($invalid_tokens);
  return $invalid_tokens;
}

/**
 * Validate if a string contains forbidden tokens not allowed by privacy rules.
 *
 * @param $token_string
 *   A string with one or more tokens to be validated.
 * @return boolean
 *   TRUE if blacklisted token has been found, otherwise FALSE.
 */
function _contextual_elements_contains_forbidden_token($token_string) {
  // List of strings in tokens with personal identifying information not allowed
  // for privacy reasons. See section 8.1 of the Google Analytics terms of use
  // for more detailed information.
  //
  // This list can never ever be complete. For this reason it tries to use a
  // regex and may kill a few other valid tokens, but it's the only way to
  // protect users as much as possible from admins with illegal ideas.
  //
  // User tokens are not prefixed with colon to catch 'current-user' and 'user'.
  //
  // TODO: If someone have better ideas, share them, please!
  $token_blacklist = array(
    ':author]',
    ':author:edit-url]',
    ':author:url]',
    ':author:path]',
    ':fid]',
    ':mail]',
    ':name]',
    ':uid]',
    ':one-time-login-url]',
    ':owner]',
    ':owner:cancel-url]',
    ':owner:edit-url]',
    ':owner:url]',
    ':owner:path]',
    'user:cancel-url]',
    'user:edit-url]',
    'user:url]',
    'user:path]',
  );

  return preg_match('/' . implode('|', array_map('preg_quote', $token_blacklist)) . '/i', $token_string);
}

/**
 * The instance add/edit form.
 * SUBMIT
 */
function contextual_elements_form_element_submit($form, &$form_state) {
  $vals = $form_state['values'];
  
  // We could just convert the $vals type to object and send that to the db,
  // but it is a safer practice to control exactly what data is being passed to
  // the db.
  $contextual_element               = new stdClass();
  $contextual_element->cid          = $vals['cid'];
  $contextual_element->status       = $vals['status'];
  $contextual_element->debug        = $vals['debug'];
  $contextual_element->name         = $vals['name'];
  $contextual_element->description  = $vals['description'];
  $contextual_element->header       = $vals['header'];
  $contextual_element->footer       = $vals['footer'];
  $contextual_element->weight       = $vals['weight'];
  $contextual_element->domain_inc   = $vals['domain_inc'];
  $contextual_element->domains      = $vals['domains'];
  $contextual_element->page_inc     = $vals['page_inc'];
  $contextual_element->pages        = $vals['pages'];
  $contextual_element->role_inc     = $vals['role_inc'];
  $contextual_element->roles        = serialize($vals['roles']);
  $contextual_element->users        = $vals['users'];
  $contextual_element->var_name     = $vals['var_name'];
  $contextual_element->var_data     = $vals['var_data'];
  $contextual_element->ga_vars_name = $vals['ga_vars']['ga_vars_name'];
  $contextual_element->ga_vars      = serialize($vals['ga_vars']['slots']);

  contextual_elements_save($contextual_element);

  // Display different messages and redirects if this is an update vs an insert
  $args = array('%name' => $contextual_element->name);
  if (!is_numeric($vals['cid'])) {

    // New element
    if (is_numeric($contextual_element->cid)) {
      drupal_set_message(t('%name successfully created!', $args), 'status');
      drupal_goto('admin/config/search/contextual-elements');
    }
    else {
      // Store the data into the session so the "add" page can repopulate the
      // fields.
      $_SESSION['contextual_elements_add'] = $contextual_element;

      drupal_set_message(t('Error creating element %name.', $args), 'error');
      watchdog('contextual_elements', 'Error creating element %name.  CID was not assigned to new object.', $args, WATCHDOG_ERROR);
    }
  }
  else {
    drupal_set_message(t('%name was successfully updated.', $args));
  }
}

/**
 * Confirm form for deleting a contextual_element.
 * FORM
 */
function contextual_elements_delete_confirm_form($form, &$form_state, $contextual_element) {
  $form['contextual_element'] = array(
    '#type' => 'value',
    '#value' => $contextual_element,
  );
  $question = filter_xss(contextual_elements_title('delete', $contextual_element)) . '?';

  return confirm_form($form, $question, 'admin/config/search/contextual-elements');
}

/**
 * Confirm form for deleting a contextual_element.
 * SUBMIT
 */
function contextual_elements_delete_confirm_form_submit($form, &$form_state) {
  $vals = $form_state['values'];
  $args = array('%name' => $contextual_element->name);

  if ($vals['confirm'] == 1) {
    if ($vals['contextual_element']->cid) {
      contextual_elements_delete($vals['contextual_element']->cid);
    }
    else {
      drupal_set_message(t('Error deleting %name.', $args), 'error');
      watchdog('contextual_elements', 'Error deleting %name.  Missing CID.', $args, WATCHDOG_ERROR);
    }
  }
  drupal_goto('admin/config/search/contextual-elements');
}

/**
 * DnD table form for changing the order of the elements.
 * FORM
 */
function contextual_elements_instances_table_form($form_state) {
  $inst = contextual_elements_get_elements();

  foreach ($inst as $i) {
    $form['#tree'] = TRUE;
    $form['data'][$i->cid]['weight']= array(
      '#type' => 'select',
      '#options' => _contextual_elements_opts('weight'),
      '#default_value' => $i->weight,
      '#attributes' => array('class' => array('weight-drag')),
    );
    $form['data'][$i->cid]['status'] = array(
      '#type' => 'select',
      '#options' => _contextual_elements_opts('status'),
      '#default_value' => $i->status,
    );
    $form['data'][$i->cid]['debug'] = array(
      '#type' => 'select',
      '#options' => _contextual_elements_opts('debug'),
      '#default_value' => $i->debug,
    );
  }

  $form['headers'] = array(
    '#type' => 'value',
    '#value' => array(t('Element Name'), t('Status'), t('Debug'), t('Weight'), t('Operations')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
  );

  return $form;
}

/**
 * DnD table form for changing the order of the elements.
 * SUBMIT
 */
function contextual_elements_instances_table_form_submit($form, &$form_state) {
  $vals = $form_state['values'];

  foreach ($vals['data'] as $cid => $contextual_element_data) {
    $contextual_element = new stdClass();
    $contextual_element = (object)$contextual_element_data;
    $contextual_element->cid = $cid;
    contextual_elements_save($contextual_element);
  }

  drupal_set_message(t('Changes have been saved.'));
}

/**
 * Machine name validator.
 */
function contextual_elements_machine_name($string) {
  return (preg_match('/^[a-z0-9\_]*$/i', $string) && !is_numeric($string[0]));
}

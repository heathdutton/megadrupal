<?php
/**
 * @file
 * The settings screens for the iCopyright plugin.
 */

/**
 * Menu callback; administration settings for icopyright
 */
function icopyright_admin_settings() {
  // Do they have a publication ID already? If not, warn them of this
  $pubid = variable_get('icopyright_publication_id', NULL);
  if ($pubid == NULL) {
    drupal_set_message(t('Your iCopyright article tools are not yet activated. ' .
                'You must <a href="@signup">sign up</a> first.',
        array('@signup' => url('admin/config/content/icopyright/signup'))),
      'warning');
  }
  else {
    $form['info'] = array(
      '#markup' => t('The following settings will determine how the iCopyright Toolbar and ' .
                'Interactive Copyright Notice appear on your nodes. If you need ' .
                'assistance, please email drupal@icopyright.com or <a href="@help">get help</a>.',
        array('@help' => url('http://info.icopyright.com/drupal'))),
    );
    $form['display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display Options'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('Settings for when the iCopyright article tools are displayed. The iCopyright Toolbar normally appears ' .
              ' at the top, and the Interactive Copyright Notice at the bottom.'),
    );
    $form['display']['icopyright_automatic_deployment'] = array(
      '#type' => 'radios',
      '#title' => t('Deployment of iCopyright Tools'),
      '#default_value' => variable_get('icopyright_automatic_deployment', 1),
      '#options' => array(
        ICOPYRIGHT_DISPLAY_AUTOMATIC => t('Automatic (iCopyright Toolbar and Interactive Copyright Notice will be automatically added into node content on load)'),
        ICOPYRIGHT_DISPLAY_MANUAL => t('Manual (you must explicitly call theme function)'),
      ),
    );
    $form['display']['icopyright_full_display_option'] = array(
      '#type' => 'radios',
      '#title' => t('When automatically displaying on full node'),
      '#default_value' => variable_get('icopyright_full_display_option', ICOPYRIGHT_DISPLAY_BOTH),
      '#options' => array(
        ICOPYRIGHT_DISPLAY_BOTH => t('Display both iCopyright Toolbar and Interactive Copyright Notice'),
        ICOPYRIGHT_DISPLAY_TOOLBAR => t('Display only iCopyright Toolbar'),
        ICOPYRIGHT_DISPLAY_COPYRIGHT_NOTICE => t('Display only Interactive Copyright Notice'),
      ),
    );
    $form['display']['icopyright_teaser_display_option'] = array(
      '#type' => 'radios',
      '#title' => t('When automatically displaying on a teaser'),
      '#default_value' => variable_get('icopyright_teaser_display_option', ICOPYRIGHT_DISPLAY_COPYRIGHT_NOTICE),
      '#options' => array(
        ICOPYRIGHT_DISPLAY_BOTH => t('Display both iCopyright Toolbar and Interactive Copyright Notice'),
        ICOPYRIGHT_DISPLAY_TOOLBAR => t('Display only iCopyright Toolbar'),
        ICOPYRIGHT_DISPLAY_COPYRIGHT_NOTICE => t('Display only Interactive Copyright Notice'),
        ICOPYRIGHT_DISPLAY_NEITHER => t('Display neither'),
      ),
    );

    $terms = (module_exists('taxonomy') ? icopyright_get_terms() : array());
    $form['article_tools'] = array(
      '#type' => 'fieldset',
      '#title' => t('iCopyright Toolbar'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('Settings for the iCopyright Toolbar. These normally appear at the top.')
    );
    $form['article_tools']['icopyright_article_tools_theme'] = array(
      '#type' => 'select',
      '#title' => t('Toolbar Theme'),
      '#default_value' => variable_get('icopyright_article_tools_theme', 'CLASSIC'),
      '#options' => icopyright_theme_options(),
      '#description' => t('Select a toolbar theme option.')
    );
    $form['article_tools']['icopyright_article_tools_background'] = array(
      '#type' => 'select',
      '#title' => t('Toolbar Background'),
      '#default_value' => variable_get('icopyright_article_tools_background', 'OPAQUE'),
      '#options' => icopyright_theme_backgrounds(),
      '#description' => t('Select a toolbar background option.')
    );
    $form['article_tools']['icopyright_article_tools_orientation'] = array(
      '#type' => 'select',
      '#title' => t('Orientation'),
      '#default_value' => variable_get('icopyright_article_tools_orientation', 'horizontal'),
      '#options' => array(
        'horizontal' => t('Horizontal'),
        'vertical' => t('Vertical'),
      ),
      '#description' => t('Select orientation of toolbar.'),
    );
    $form['article_tools']['preview'] = array(
      '#markup' => '<iframe id="article-tools-preview" style="border: 0;" scrolling="no" ></iframe>',
    );
    $form['article_tools']['icopyright_article_tools_alignment'] = array(
      '#type' => 'select',
      '#title' => t('Alignment'),
      '#default_value' => variable_get('icopyright_article_tools_alignment', 'right'),
      '#options' => array(
        'right' => t('Right'),
        'left' => t('Left'),
      ),
      '#description' => t('Select alignment of toolbar.'),
    );
    $form['article_tools']['icopyright_article_tools_node_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Node types'),
      '#description' => t('Select node types on which to display the iCopyright Toolbar.'),
      '#default_value' => variable_get('icopyright_article_tools_node_types', array()),
      '#options' => node_type_get_names(),
    );
    if (count($terms) > 0) {
      $form['article_tools']['icopyright_article_tools_category_types'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => t('Terms'),
        '#description' => t('Select taxonomy terms on which to display the iCopyright Toolbar.'),
        '#default_value' => variable_get('icopyright_article_tools_category_types', array()),
        '#options' => icopyright_get_terms(),
      );
    }
    $form['interactive_copyright_notice'] = array(
      '#type' => 'fieldset',
      '#title' => t('Interactive Copyright Notice'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('Settings for the Interactive Copyright Notice. This normally appears at the bottom.'),
    );
    $form['interactive_copyright_notice']['preview'] = array(
      '#markup' => '<iframe id="copyright-notice-preview" style="border: 0;" height="50" scrolling="no" ></iframe>',
    );
    $form['interactive_copyright_notice']['icopyright_interactive_copyright_notice_node_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Node types'),
      '#description' => t('Select node types on which to display the Interactive Copyright Notice.'),
      '#default_value' => variable_get('icopyright_interactive_copyright_notice_node_types', array()),
      '#options' => node_type_get_names(),
    );
    if (count($terms) > 0) {
      $form['interactive_copyright_notice']['icopyright_interactive_copyright_notice_category_types'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => t('Terms'),
        '#default_value' => variable_get('icopyright_interactive_copyright_notice_category_types', array()),
        '#description' => t('Select taxonomy terms on which to display the Interactive Copyright Notice.'),
        '#options' => icopyright_get_terms(),
      );
    }

    $form['features'] = array(
      '#type' => 'fieldset',
      '#title' => t('Features'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('Settings to modify how the iCopyright features work on the site.'),
    );
    $form['features']['icopyright_share'] = array(
      '#type' => 'radios',
      '#title' => t('Share Services'),
      '#default_value' => variable_get('icopyright_share', 1),
      '#description' => t('Share services make it easy for readers to share links to your articles using Facebook, ' .
              'LinkedIn, Twitter, and Google+.'),
      '#options' => array(
        0 => 'Off',
        1 => 'On',
      ),
    );
    $form['features']['icopyright_ez_excerpt'] = array(
      '#type' => 'radios',
      '#title' => t('EZ Excerpt'),
      '#default_value' => variable_get('icopyright_ez_excerpt', 1),
      '#description' => t('When EZ Excerpt is activated, any reader who tries ' .
              'to copy/paste a portion of your article will be presented with a box asking ' .
              '"Obtain a License?" If reader selects "yes" he or she will be offered the ' .
              'opportunity to license the excerpt for purposes of posting on the reader\'s own website. ' .
              'For EZ Excerpt to be enabled, the display option selected above must include ' .
              'the iCopyright Toolbar. '),
      '#options' => array(
        0 => 'Off',
        1 => 'On',
      ),
    );
    $form['features']['icopyright_feed_syndication'] = array(
      '#type' => 'radios',
      '#title' => t('Syndication'),
      '#default_value' => variable_get('icopyright_feed_syndication', 1),
      '#description' => t('The Syndication Feed service enables other websites to subscribe to a feed of your ' .
              'content and pay you based on the number of times your articles are viewed on their site ' .
              'at a CPM rate you specify. When you receive your welcome email, click to go into ' .
              'Conductor and set the business terms you would like. Until you do that, default pricing ' .
              'and business terms will apply.'),
      '#options' => array(
        0 => 'Off',
        1 => 'On',
      ),
    );
  }

  // Advanced settings: let the user set the username, password, and publication ID explicitly (this assumes
  // they've already signed up and have an account.)
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Advanced users only.'),
  );
  $form['advanced']['icopyright_publication_id'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Publication ID'),
    '#default_value' => variable_get('icopyright_publication_id', ''),
    '#description' => t('The Publication ID for this site, as assigned in iCopyright Conductor.'),
    '#required' => TRUE,
  );
  $form['advanced']['icopyright_conductor_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Conductor Email Address'),
    '#default_value' => variable_get('icopyright_conductor_email', ''),
    '#description' => t('The email address to use when communicating with the iCopyright servers.'),
    '#required' => TRUE,
  );
  $form['advanced']['icopyright_conductor_password'] = array(
    '#type' => 'password',
    '#title' => t('Conductor Password'),
    '#default_value' => variable_get('icopyright_conductor_password', ''),
    '#description' => t('The password to use when communicating with the iCopyright servers. Leave it blank to leave it unchanged.'),
    '#required' => FALSE,
  );

  $form['suffix'] = array(
    '#markup' => '<div id="conductor-link" style="padding-top: 1em;">' . t('<a href="@conductor" target="_blank">Log in to Conductor</a> to ' .
              'enable additional services, adjust further settings, and view usage reports.',
      array('@conductor' => url(icopyright_get_server() . '/publisher'))) . '</div>',
  );

  $pubid = variable_get('icopyright_publication_id', NULL);
  if (isset($pubid)) {
    // Build up javascript to enable a live preview of the article tools
    drupal_add_js(array('icopyright' => array('server' => icopyright_get_server())), 'setting');
    drupal_add_js(array('icopyright' => array('pubid' => $pubid)), 'setting');
    drupal_add_js(array('icopyright' => array('pubname' => variable_get('site_name', ''))), 'setting');
    drupal_add_js(drupal_get_path('module', 'icopyright') . '/icopyright.admin.js');
  }
  $form['#submit'][] = 'icopyright_admin_settings_submit';
  $form['#validate'][] = 'icopyright_admin_settings_form_validate';
  return system_settings_form($form);
}

/**
 * Validation routine for the settings form
 * @param $form
 * @param $form_state
 */
function icopyright_admin_settings_form_validate($form, &$form_state) {
  // A little hack. When form is presentend, the password is not shown (a la Drupal standard)
  // So, if user submits the form without changing the password, we must prevent it from being reset.
  if (empty($form_state['values']['icopyright_conductor_password'])) {
    unset($form_state['values']['icopyright_conductor_password']);
  }
  $pubid = $form_state['values']['icopyright_publication_id'];
  if (!is_numeric($pubid) || !is_int((int)$pubid) || ($pubid < 0)) {
    form_set_error('icopyright_publication_id', t('Publication ID must be a non-negative integer.'));
  }
  $mail = $form_state['values']['icopyright_conductor_email'];
  if (!valid_email_address($mail)) {
    form_set_error('email', t('The email address appears to be invalid.'));
  }
  if(isset($form_state['values']['icopyright_conductor_password'])) {
    $pw = $form_state['values']['icopyright_conductor_password'];
    if ((drupal_strlen($pw) > 0) && (drupal_strlen($pw) < 6)) {
      form_set_error('password', 'Please enter a password of at least 6 characters.');
    }
  }
}

/**
 * Simple function to fetch a variable from the form state, if it's set; returns the variable from the
 * variables table if there's nothing in the form
 *
 * @param $form_state the form state
 * @param $var the name of the variable
 * @param string $default the default if there's absolutely nothing we know about this
 * @return the value
 */
function icopyright_formvar($form_state, $var, $default = '') {
  return (isset($form_state['values'][$var]) ? $form_state['values'][$var] : variable_get($var, $default));
}

/**
 * On submission of the administrative settings form, send the changes over the wire to the
 * icopyright servers.
 *
 * @param  $form
 * @param  $form_state
 */
function icopyright_admin_settings_submit($form, &$form_state) {
  // The settings from the admin form
  $share = icopyright_formvar($form_state, 'icopyright_share');
  $ez = icopyright_formvar($form_state, 'icopyright_ez_excerpt');
  $theme = icopyright_formvar($form_state, 'icopyright_article_tools_theme');
  $bg = icopyright_formvar($form_state, 'icopyright_article_tools_background');
  $synd = icopyright_formvar($form_state, 'icopyright_feed_syndication');

  // We need to pull out the pub ID, email, and password to authenticate the submission
  $pid = $form_state['values']['icopyright_publication_id'];
  $email = $form_state['values']['icopyright_conductor_email'];

  // If the user didn't specify a password in the form, use the default one
  $pw = icopyright_formvar($form_state, 'icopyright_conductor_password');

  if ($pid != NULL) {
    // Change the EZ Excerpt *if* the field is different
    if (variable_get('icopyright_share', '') != $share) {
      $shareresp = icopyright_post_share_service($pid, $share, icopyright_user_agent(), $email, $pw);
      if (!icopyright_check_response($shareresp)) {
        // Failed to update, so don't change the field and warn the user
        unset($form_state['values']['icopyright_share']);
        drupal_set_message(t('Failed to update share services setting'), 'warning');
        watchdog('icopyright', 'Failed to update share services with response %code (%expl)',
          array('%code' => $shareresp->http_code, '%expl' => $shareresp->http_expl), WATCHDOG_ERROR);
      }
    }
    // Change the toolbar theme *if* the field is different
    if ((variable_get('icopyright_article_tools_theme', 'CLASSIC') != $theme) ||
            (variable_get('icopyright_article_tools_background', 'OPAQUE') != $bg)
    ) {
      $themeresp = icopyright_post_toolbar_theme($pid, $theme, $bg, icopyright_user_agent(), $email, $pw);
      if (!icopyright_check_response($themeresp)) {
        // Failed to update, so don't change the field and warn the user
        unset($form_state['values']['icopyright_article_tools_theme']);
        drupal_set_message(t('Failed to update toolbar theme'), 'warning');
        watchdog('icopyright', 'Failed to update toolbar theme with response %code (%expl)',
          array('%code' => $themeresp->http_code, '%expl' => $themeresp->http_expl), WATCHDOG_ERROR);
      }
    }
    // Change the EZ Excerpt *if* the field is different
    if (variable_get('icopyright_ez_excerpt', '') != $ez) {
      $ezresp = icopyright_post_ez_excerpt($pid, $ez, icopyright_user_agent(), $email, $pw);
      if (!icopyright_check_response($ezresp)) {
        // Failed to update, so don't change the field and warn the user
        unset($form_state['values']['icopyright_ez_excerpt']);
        drupal_set_message(t('Failed to update EZ Excerpt setting'), 'warning');
        watchdog('icopyright', 'Failed to update EZ Excerpt with response %code (%expl)',
          array('%code' => $ezresp->http_code, '%expl' => $ezresp->http_expl), WATCHDOG_ERROR);
      }
    }
    // Change the syndication setting *if* the field is different
    if (variable_get('icopyright_feed_syndication', '') != $synd) {
      $syndresp = icopyright_post_syndication_service($pid, $synd, icopyright_user_agent(), $email, $pw);
      if (!icopyright_check_response($syndresp)) {
        // Failed to update, so don't change the field and warn the user
        unset($form_state['values']['icopyright_feed_syndication']);
        drupal_set_message(t('Failed to update syndication setting'), 'warning');
        watchdog('icopyright', 'Failed to update syndication with response %code (%expl)',
          array('%code' => $syndresp->http_code, '%expl' => $syndresp->http_expl), WATCHDOG_ERROR);
      }
    }
  }
}

/**
 * Callback for the admin signup form: it's either the signup form or a statement
 * saying you're already signed up.
 */
function icopyright_admin_signup() {
  $pid = variable_get('icopyright_publication_id', NULL);
  if (isset($pid)) {
    return t('You have been assigned publication ID @pid. ' .
              '<a href="@conductor">Log in to Conductor</a> to enable additional services, ' .
              'adjust further settings, and view usage reports.',
      array('@pid' => $pid, '@conductor' => url(icopyright_get_server() . '/publisher')));
  }
  else {
    return drupal_get_form('icopyright_admin_signup_form');
  }
}

/**
 * Form to sign up with iCopyright and get a publication ID
 */
function icopyright_admin_signup_form() {
  global $user;
  $form['you'] = array(
    '#type' => 'fieldset',
    '#title' => t('About You'),
    '#description' => t('Your name and how we can contact you.')
  );
  $form['you']['fname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
  );
  $form['you']['lname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
  );
  $form['you']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#default_value' => $user->mail,
    '#required' => TRUE,
  );
  $form['you']['password'] = array(
    '#type' => 'password',
    '#title' => t('Create Password'),
    '#description' => t('For access to iCopyright Conductor. Must be at least 6 characters.'),
    '#required' => TRUE,
  );

  $form['site'] = array(
    '#type' => 'fieldset',
    '#title' => t('About This Site'),
    '#description' => t('Your publication, site, or blog.')
  );
  $form['site']['pname'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Name'),
    '#default_value' => variable_get('site_name', ''),
    '#required' => TRUE,
  );
  global $base_url;
  $form['site']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Public URL'),
    '#required' => TRUE,
    '#default_value' => $base_url,
  );

  $form['terms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Terms of Use'),
  );
  $form['terms']['tou'] = array(
    '#title' => 'Terms of Use',
    '#type' => 'checkbox',
    '#default_value' => 0,
    '#required' => TRUE,
    '#description' => t('I agree with the <a href="@csa" target="_blank">terms of use</a>.',
      array('@csa' => url(icopyright_get_server() . '/publisher/statichtml/CSA-Online-Plugin.pdf'))),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  return $form;
}

/**
 * Validation routine for the signup form: if everything passes validation, submit
 * to icopyright servers to see if the submission was accepted
 * @param $form
 * @param $form_state
 */
function icopyright_admin_signup_form_validate($form, &$form_state) {
  // Basic field validation
  $mail = $form_state['values']['email'];
  if (!valid_email_address($mail)) {
    form_set_error('email', t('The email address appears to be invalid.'));
  }
  $pw = $form_state['values']['password'];
  if (drupal_strlen($pw) < 6) {
    form_set_error('password', 'Please enter a password of at least 6 characters.');
  }

  // Gotta check that tou box
  if ($form_state['values']['tou'] != 1) {
    form_set_error('tou', 'You must accept the terms of service to proceed.');
  }

  // If it passes basic validation, submit it to iCopyright for a pub ID
  if (count(form_get_errors()) == 0) {
    icopyright_admin_submit_signup_request($form, $form_state);
    variable_set('icopyright_conductor_email', $form_state['values']['email']);
    variable_set('icopyright_conductor_password', $form_state['values']['password']);
  }
}

/**
 * Given a submission form that's been filled out, and that passes basic validation,
 * submit the request to iCopyright's servers so we get a publication ID assigned
 *
 * @param  $form
 * @param  $form_state
 * @return void
 */
function icopyright_admin_submit_signup_request($form, &$form_state) {
  // Build up a collection of arguments containing the form field values
  $args = array('fname', 'lname', 'email', 'password', 'pname', 'url');
  $queryargs = array();
  foreach ($args as $a) {
    $v = trim($form_state['values'][$a]);
    if (drupal_strlen($v) > 0)
      array_push($queryargs, $a . '=' . urlencode($v));
  }

  // Send it to the iCopyright servers to create a publisher record
  $rv = icopyright_post_new_publisher(join('&', $queryargs), icopyright_user_agent(),
    $form_state['values']['email'], $form_state['values']['password']);
  // Parse the response: did a publication ID come back?
  $xml = @simplexml_load_string($rv->response);
  if (icopyright_check_response($rv)) {
    // Success: store the publication ID that got sent as a variable
    $pubid = (string)$xml->publication_id;
    variable_set('icopyright_publication_id', $pubid);
    watchdog('icopyright', 'Created new publication record with ID <pre>%id</pre>',
      array('%id' => $pubid), WATCHDOG_NOTICE);
  }
  else {
    // Errors; report them and stay. For now we don't know which field failed because
    // the icopyright API doesn't tell us.
    $i = 0;
    form_set_error('error-' . $i++, t('Your submission was rejected by the iCopyright servers.'));
    if (isset($xml->status)) {
      foreach ($xml->status->messages->message as $message) {
        $error = (string)$message;
        form_set_error('error-' . $i++, $error);
      }
    } else {
      // No messages, so presume something was wrong with the communication layer itself
      form_set_error('error-' . $i++, t('Submission failed with code %code (%msg)',
        array('%code' => $rv->http_code, '%msg' => $rv->response)));
    }
  }
}


/**
 * On successful submission, send the user to the general settings page
 * @param  $form
 * @param  $form_state
 * @return void
 */
function icopyright_admin_signup_form_submit($form, &$form_state) {
  $icopyright_conductor_url =
          drupal_set_message(t('Congratulations, your website is now live with iCopyright! ' .
                      'Please review the default settings below and make any changes you wish. ' .
                      'You may find it helpful to view the video ' .
                      '<a href="@video" target="_blank">"Introduction to iCopyright"</a>. ' .
                      'Feel free to visit your new <a href="@conductor" target="_blank">iCopyright ' .
                      'Conductor</a> account to explore your new capabilities. A welcome email has been sent to you with ' .
                      'some helpful hints.',
              array('@video' => 'http://info.icopyright.com/icopyright-video',
                '@conductor' => icopyright_get_server() . '/publisher')),
            'status');
  drupal_goto('admin/config/content/icopyright/general');
}


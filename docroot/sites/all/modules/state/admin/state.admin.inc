<?php
/**
 * @file
 *  Drupal State module
 */

/**
 * Implements drupal_get_form().
 *
 * Defines configuration form fields.
 *
 * @param array $form
 * @param array &$form_state
 * @return array
 *   - the return value of system_settings_form()
 */
function _state_admin_form($form, &$form_state) {
  drupal_add_css(
    ($path = drupal_get_path('module', 'state')) . '/admin/state.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );
  drupal_add_js(
    $path . '/admin/state.admin.js',
    array('type' => 'file', 'group' => JS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );

  //  Documentation.
  $doc = '<h5>'
    . t(
      'More detailed documentation available here: <a href="@doc_backend" target="_blank">PHP</a> and <a href="@doc_frontend" target="_blank">Javascript</a>.',
      array(
        '@doc_backend' => 'http://www.simple-complex.net/docs/drupal/modules/state/php/state_8module.html',
        '@doc_frontend' => 'http://www.simple-complex.net/docs/drupal/modules/state/js/symbols/State.html'
      )
    )
    . '</h5>'
    . '<div class="columns-2"><div><h5>' . t('PHP functions') . '</h5>';
  if (function_exists('inspect_doc_functions')
    && ($funcs = inspect_doc_functions('state', '', array(
      'state_preprocess_html',
      'state_ajax_callback',
    )))
  ) {
    foreach ($funcs as $v) {
      $doc .= '<p>' . ($v[2] ? ($v[2] . '<br/>') : '')
        . '<span class="code emph">' . $v[0] . '</span>'
        . '<span class="code">' . $v[1] . '</span></p>';
    }
    $doc .= '</div><div><h5>' . t('Javascript methods') . '</h5>'
      . '<p>' . t('The library is available as Drupal.State and State.') . '</p>';
    $funcs = array(
      array(
        'State.alive',
        '()',
        'Check whether session seems alive and current, and form hasnt expired.',
      ),
      array(
        'State.pageResponseNumber',
        '()',
        'Page load number; how many times backend State has positively detected that a (any) page was delivered.',
      ),
      array(
        'State.sessionProlonged',
        '(date)',
        'Tell State that the current session got prolonged now, due to a (non jQuery) AJAX or iframe request.',
      ),
      array(
        'State.cookieSet',
        '(name, value, options)',
        'Set domain-safe cookie.',
      ),
      array(
        'State.cookieIncrease',
        '(name, increment, options)',
        'Increase value of domain-safe cookie.',
      ),
      array(
        'State.cookieGet',
        '(name)',
        'Get domain-safe cookie.',
      ),
      array(
        'State.cookieRemove',
        '(name, options)',
        'Remove domain-safe cookie.',
      ),
      array(
        'State.cookieDefaults',
        '()',
        'Get list of domain-safe cookie defaults.',
      ),
      array(
        'State.inspect',
        '()',
        'Inspect current state.',
      ),
      array(
        'State.testCheck',
        '()',
        'Do immediate check.',
      ),
      array(
        'State.testInfo',
        '()',
        'Test info (dialog) method.',
      ),
    );
    foreach ($funcs as $v) {
      $doc .= '<p>' . ($v[2] ? ($v[2] . '<br/>') : '')
        . '<span class="code emph">' . $v[0] . '</span>'
        . '<span class="code">' . $v[1] . '</span></p>';
    }
    $doc .= '</div></div>';
  }
  $form['functions'] = array(
    '#type' => 'fieldset',
    '#title' => t('PHP and Javascript'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'content' => array(
      '#type' => 'markup',
      '#markup' => $doc,
    ),
  );

  //  Help.
  $form['help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'content' => array(
      '#type' => 'markup',
      '#markup' =>
      '<h5>' . t('Unconditional versus user-activity conditioned session prolongation') . '</h5>
<p>' . t('Unconditional: the visitors\' session will be kept alive no matter if the visitor is active or not - only limited by form expiration (!hrs hours).',
        array('!hrs' => state::FORM_EXPIRE / (60 * 60))
      ) . '</p>
<p>' . t('Conditional: the visitors session will be prolonged for as long as the visitor seems active - e.g. recently has moved the mouse; pressed a key; scrolled the window.'
      ) . '</p>
<h5>' . t('Getting close to session/form expiration') . '</h5>
<p>' . t('10 to 20% before session/form expiration frontend State pops a dialog box telling the user that the page is about to expire - \'please save your work...\'.'
      ) . '</p>
<h5>' . t('Session or form has expired, or session isn\'t current') . '</h5>
<p>' . t('If a window/tab\'s session or form has expired - or the visitor has a new session or no session at all:
!break- State will pop a dialog box informing the visitor about the state of the affairs.
!paragraphIf State is configured to auto-redirect, the window/tab will redirect to either same url or a custom url.
!paragraphReloading the page is by far the more user-friendly, unless the visitor simply meets a 403 (not allowed to use this page).',
        array('!break' => '<br/>', '!paragraph' => '</p><p>', '!emph' => '<em>', '!_emph' => '</em>')
      ) . '</p>
<h5>' . t('Check interval') . '</h5>
<p>' . t(
        'For every N seconds, check that window/tab session is alive and current, and form hasn\'t expired,
    !breakand - if user-activity conditioned session prolongation - whether the visitor has been active since last check.
    !breakRecommended value, if session timeout (life on server) is !life_seconds (!life_minutes minutes) or more: !check_seconds (!check_minutes minutes).
    !breakState will also perform AJAX requests at this interval, whenever the prolongation conditions are met.
    !paragraphSetting this value correctly is a matter of balancing contradictory interests. Too low: too many requests to the server.
    !breakToo high: user-activity conditioned prolongation will tend to prolong sessions excessively (because the last activity may have taken place almost the interval time ago).
    !paragraphFrontend ensures that the interval is maximally 20% of the session timeout.
    !breakAnd the actual intervals will fluctuate approx. +/-20%, due to fuzzy logic algorithm which ensures that the browser also only "calls in" at the set interval when multiple tabs are open.',
        array(
          '!break' => '<br/>', '!paragraph' => '</p><p>',
          '!life_seconds' => State::SERVER_LIFE_DEFAULT, '!life_minutes' => round(State::SERVER_LIFE_DEFAULT / 60),
          '!check_seconds' => State::CHECK_INTERVAL_DEFAULT, '!check_minutes' => round(State::CHECK_INTERVAL_DEFAULT / 60),
        )
      ) . '</p>'
    ),
  );

  //  Get roles.
  //  Make anonymous user, authenticated user and the administrative role the first three.
  $roles = user_roles();
  $roles_sorted = array();
  try {
    //  Dont trust role ids of anonymous user and authenticated user to be standard; find them and use pseudo ids -2 and -1.
    //  Instead we establish - at runtime - anonymous versus authenticated by simply checking whether current user is logged in.

    //  (later) Not trusting those standard IDs is kind of rubbish, or just too paranoid. But changing it now could seriously break stuff for people.

    unset( $roles[DRUPAL_ANONYMOUS_RID], $roles[DRUPAL_AUTHENTICATED_RID] );
    $roles_sorted['-2'] = t('anonymous user');
    $roles_sorted['-1'] = t('authenticated user');
    //  Use pseudo id -3 for admin user 1.
    $roles_sorted['-3'] = t('admin user 1');
    //  Find administrative role.
    if (($role_admin = variable_get('user_admin_role'))) { // Admin role may not exist at all, if only user 1 is used as administrator.
      $roles_sorted[$role_admin] = t('!role (Administrative role)', array('!role' => $roles[$role_admin]));
      unset( $roles[$role_admin] );
    }
  }
  catch (Exception $xc) {
    watchdog(
      'state',
      $xc->getMessage(),
      NULL,
      WATCHDOG_CRITICAL
    );
    return system_settings_form(array());
  }
  foreach ($roles as $role_id => $name) {
    $roles_sorted[ $role_id ] = $name;
  }

  //  Default roles for user-activity conditioned prolongation: admin user 1 and the administrative role.
  $roles_prlngcnd = variable_get('state__prlngcnd', !$role_admin ? array('-3') : array('-3', $role_admin));
  //  Default roles for unconditional prolongation: anonymous user and authenticated user.
  $roles_prlnguncnd = variable_get('state__prlnguncnd', array('-2', '-1'));

  $timeout_cookie = (int) ini_get('session.cookie_lifetime');
  $timeout_server = (int) ini_get('session.gc_maxlifetime');
  $timeout_server_final = min($timeout_server, ($v = variable_get('state__srvrlife', '')) ? $v : 200000);

  $label_cooklife = t('Enforce session timeout for logged in users');
  $label_srvrlife = t('Override session life on server');

  $form['frontend_session_management'] = array(
    '#type' => 'fieldset',
    '#title' => t('Frontend session management'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    'session_cookie_life' => array(
      '#type' => 'markup',
      '#markup' => '<h5>'
      . t('Session cookie life: !timeout_seconds seconds', array('!timeout_seconds' => $timeout_cookie))
      . '</h5>'
      . '<p>' . (
      !$timeout_cookie ? t('Session expires when the browser is closed. And that\'s also the recommended setting.') :
        t(
          'Session !emphdoesn\'t!_emph expire when the browser is closed. Recommended value: 0 (zero) seconds!breakUse the \'!override_cooklife\' setting in frontend session management, or set it in the settings.php file: !setting',
          array('!override_cooklife' => $label_cooklife, '!setting' => 'ini_set(\'session.cookie_lifetime\', 0);', '!emph' => '<em>', '!_emph' => '</em>', '!break' => '<br/>')
        )
      )
      . '</p>'
    ),
    'session_cookie_server' => array(
      '#type' => 'markup',
      '#markup' => '<h5>'
      . t(
        'Session life on server: !timeout_seconds seconds (~ !timeout_minutes minutes)',
        array('!timeout_seconds' => $timeout_server, '!timeout_minutes' => round($timeout_server / 60, 1))
      )
      . '</h5>'
      . '<p>'
      . t(
        'Recommended value: !recommended_seconds seconds (~ !recommended_minutes minutes). A value of more than form expire (!form_expire_seconds seconds ~ !form_expire_hours hours) rarely makes sense.',
        array('!recommended_seconds' => State::SERVER_LIFE_DEFAULT, '!recommended_minutes' => round(State::SERVER_LIFE_DEFAULT / 60), '!form_expire_seconds' => State::FORM_EXPIRE, '!form_expire_hours' => round(State::FORM_EXPIRE / 60 / 60, 1))
      )
      . (
      $timeout_server < state::FORM_EXPIRE ? '' :
        t(
          '!breakUse the \'!override_serverlife\' setting in frontend session management, or set it in the settings.php file: !setting',
          array('!override_serverlife' => $label_srvrlife, '!setting' => 'ini_set(\'session.gc_maxlifetime\', ' . State::SERVER_LIFE_DEFAULT . ');', '!break' => '<br/>')
        )
      )
      . '</p>'
    ),
    'state__frntnd' => array(
      '#type' => 'checkbox',
      '#title' => t('Enable frontend session management'),
      '#default_value' => variable_get('state__frntnd', 0),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
    ),
    'frontend_session_configure' => array(
      '#type' => 'fieldset',
      '#title' => t('Configure frontend session management'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'state__nfrclgt' => array(
        '#type' => 'checkbox',
        '#title' => check_plain($label_cooklife), // Otherwise Coder Review complains.
        '#description' => t(
          '!emphHighly recommended!_emph when session cookie life isn\'t zero (session doesn\'t expire when the browser is closed).!breakSome performance impact because a session variable will be saved/updated for every request by a logged in user.',
          array('!break' => '<br/>', '!emph' => !$timeout_cookie ? '' : '<strong>', '!_emph' => !$timeout_cookie ? '' : '</strong>')
        ),
        '#default_value' => variable_get('state__nfrclgt', 0),
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
      ),
      'state__enovrrd' => array(
        '#type' => 'textarea',
        '#default_value' => variable_get('state__enovrrd', ''),
        '#maxlength' => 10000,
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
      ),
      'state__srvrlife' => array(
        '#type' => 'textfield',
        '#title' => check_plain($label_srvrlife), // Otherwise Coder Review complains.
        '#description' => t(
          'Overrides the !setting set in the settings.php life, if less than that value (!value).',
          array('!setting' => '\'session.gc_maxlifetime\'', '!value' => $timeout_server)
        ),
        '#default_value' => variable_get('state__srvrlife', $timeout_server_final < state::FORM_EXPIRE ? '' : State::SERVER_LIFE_DEFAULT),
        '#size' => 5,
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
      ),
      'state__prlnguncnd' => array(
        '#type' => 'checkboxes',
        '#title' => t('Prolong session unconditionally'),
        '#description' => t(
          '!emphThis mode is not recommended for administrative roles.!_emph',
          array('!emph' => '<em>', '!_emph' => '</em>')
        ),
        '#options' => $roles_sorted,
        '#default_value' => $roles_prlnguncnd,
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
        '#prefix' => '<div class="columns-3"><div>',
        '#suffix' => '</div>',
      ),
      'state__prlngcnd' => array(
        '#type' => 'checkboxes',
        '#title' => t('Conditional prolongation - require visitor activity'),
        '#description' => t(
          'Specific role(s) in one mode always beat \'!auth\' in the other mode.!breakOtherwise; if \'!auth\' in neither of the modes,!breakand the user has roles in both modes: conditional prolongation rules.',
          array('!break' => '<br/>', '!auth' => t('authenticated user'))
        ),
        '#options' => $roles_sorted,
        '#default_value' => $roles_prlngcnd,
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
      ),
      'state__chcksss' => array(
        '#type' => 'checkbox',
        '#title' => t(
          'For visitors that don\'t get session prolongation:!break- check that session is current and form hasn\'t expired',
          array('!break' => '<br/>')
        ),
        '#default_value' => variable_get('state__chcksss', 0),
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
        '#prefix' => '<div>',
        '#suffix' => '</div></div>',
      ),
      'state__rdrtrg' => array(
        '#type' => 'textfield',
        '#title' => t('Session/form timeout target url'),
        '#description' => t(
          'Leave empty for reloading the page.!breakInternal url - / (~ start page) or /go-somewhere - or external url - http://some.where.',
          array('!break' => '<br/>')
        ),
        '#default_value' => variable_get('state__rdrtrg', ''),
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
        '#prefix' => '<div class="columns-3"><div>',
        '#suffix' => '</div>',
      ),
      'state__autrdr' => array(
        '#type' => 'checkbox',
        '#title' => t(
          'Auto-redirect at session/form timeout!break- after having informed the visitor via dialog box',
          array('!break' => '<br/>')
        ),
        '#description' => t(
          'Otherwise; the dialog stays open, and the redirect!breakis performed when the visitor closes the dialog.',
          array('!break' => '<br/>')
        ),
        '#default_value' => variable_get('state__autrdr', 0),
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
        '#prefix' => '<div>',
        '#suffix' => '</div></div>',
      ),
      'state__chckntrvl' => array(
        '#type' => 'textfield',
        '#title' => t('Check interval'),
        '#description' => t(
          'Recommended value is !check_seconds (~ !check_minutes minutes) when session life on server is !life_seconds (~ !life_minutes minutes) or more.',
          array(
            '!life_seconds' => State::SERVER_LIFE_DEFAULT, '!life_minutes' => round(State::SERVER_LIFE_DEFAULT / 60),
            '!check_seconds' => State::CHECK_INTERVAL_DEFAULT, '!check_minutes' => round(State::CHECK_INTERVAL_DEFAULT / 60),
          )
        ),
        '#default_value' => variable_get('state__chckntrvl', State::CHECK_INTERVAL_DEFAULT),
        '#element_validate' => array('element_validate_integer_positive'),
        '#size' => 4,
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
      ),
      'override_english_warnings' => array(
        '#type' => 'fieldset',
        '#title' => t('Override English session/form expire warnings'),
        '#collapsible' => TRUE,
        '#collapsed' => !variable_get('state__enovrrd', ''),
        'sub_fields__state__enovrrd' => array(
          '#type' => 'markup',
          '#markup' =>
            //  Override English
          '<div class="description-local-en-override">' . t('When translating to other languages, look for the defaults, not these overrides (if any).') . '</div>'

          . '<div class="form-item form-type-textfield local-en-override">
<label>' . t('Session/form about to expire') . '</label>
<div class="locale-default">Default: '
          . str_replace(
            array('<', '>'),
            array('&lt;', '&gt;'),
            'This page is about to expire!newline!newlinePlease save now, if you have made any changes.!newlineOtherwise, please !link_startreload the page!link_end.' // NOT t() - is t()'ed in state.js.
          )
          . '</div>
<input type="text" class="form-text local-en-override" maxlength="255" size="150" value="" name="local_en__preExpire" autocomplete="off">
<div class="description">'
          . t('Allowed replacements: !rplcs. First sentence delimited by double !newline will be used as dialog title.', array('!rplcs' => '!newline'))
          . '</div></div>'

          . '<div class="form-item form-type-textfield local-en-override">
<label>' . t('Session/form expired, reload current page') . '</label>
<div class="locale-default">Default: '
          . str_replace(
            array('<', '>'),
            array('&lt;', '&gt;'),
            'This page has expired!newline!newlinePlease !link_startreload the page!link_end.' // NOT t() - is t()'ed in state.js.
          )
          . '</div>
<input type="text" class="form-text local-en-override" maxlength="255" size="150" value="" name="local_en__expired_reload" autocomplete="off">
<div class="description">'
          . t('Allowed replacements: !rplcs. First sentence delimited by double !newline will be used as dialog title.', array('!rplcs' => '!newline, !url'))
          . '<br/>'
          . t('NB: The visitor will only see this message if the browser prohibits Javascript redirect (Internet Explorer: \'META refresh\').')
          . '</div></div>'

          . '<div class="form-item form-type-textfield local-en-override">
<label>' . t('Session/form expired, redirect to other page - non-empty \'Session/form timeout target url\'') . '</label>
<div class="locale-default">Default: '
          . str_replace(
            array('<', '>'),
            array('&lt;', '&gt;'),
            'This page has expired!newline!newlinePlease !link_startproceed to the start page!link_end.' // NOT t() - is t()'ed in state.js.
          )
          . '</div>
<input type="text" class="form-text local-en-override" maxlength="255" size="150" value="" name="local_en__expired_redirect" autocomplete="off">
<div class="description">'
          . t('Allowed replacements: !rplcs. First sentence delimited by double !newline will be used as dialog title.', array('!rplcs' => '!newline, !url'))
          . '<br/>'
          . t('NB: The visitor will only see this message if the browser prohibits Javascript redirect(Internet Explorer: \'META refresh\').')
          . '</div></div>'

          . '<div class="form-item form-type-textfield local-en-override">
<label>' . t('(Drupal) message when logged out due to session timeout') . '</label>
<div class="locale-default">Default: '
          . 'You were logged out, because the page timed out.' // NOT t() - is t()'ed in state.js.
          . '</div>
<input type="text" class="form-text local-en-override" maxlength="255" size="150" value="" name="local_en__expired_logout" autocomplete="off">
<div class="description">'
          . '</div></div>',
        )
      )
    )
  );
  $form['request_counting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Detect request type - page versus AJAX'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    'state__nonpages' => array(
      '#type' => 'textarea',
      '#title' => t('List of non-page paths - like iframed page views, and AJAX calls failing to send the HTTP_X_REQUESTED_WITH header'),
      '#description' => t(
        'Newline separated. Built-in non-page paths are:!break &nbsp; !built_ins',
        array('!break' => '<br/>', '!built_ins' => join('<br/> &nbsp; ', State::nonPagePaths(TRUE)))
      ),
      '#default_value' => variable_get('state__nonpages', ''),
      '#maxlength' => 1000,
      '#cols' => 100,
      '#rows' => 2,
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
    ),
    'state__cntreqs' => array(
      '#type' => 'checkbox',
      '#title' => t('Count requests, and by type'),
      '#default_value' => variable_get('state__cntreqs', 0),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
    ),
  );
  $form['#submit'][] = 'state_admin_form_submit';
  return system_settings_form($form);
}

/**
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function _state_admin_form_submit($form, &$form_state) {
  //  Fix wrong value of state__srvrlife.
  if (!($server_life = $form_state['values']['state__srvrlife']) || $server_life < 0 || $server_life > State::FORM_EXPIRE) {
    $form_state['values']['state__srvrlife'] = '';
  }

  //  Remove latest_request record from session vars, if not enforcing session timeout for logged in users.
  if (!$form_state['values']['state__nfrclgt']) {
    State::sessionRemove('module', 'state', 'latest_request');
  }

  //  Fix check interval.
  if (!($check_interval = (int) $form_state['values']['state__chckntrvl']) || $check_interval < 0) {
    $check_interval = $form_state['values']['state__chckntrvl'] = State::CHECK_INTERVAL_DEFAULT;
  }
  if ($check_interval > ($timeout_server = min( ini_get('session.gc_maxlifetime'), ($v = $form_state['values']['state__srvrlife']) ? $v : 200000 )) / 6) {
    $form_state['values']['state__chckntrvl'] = (int) floor($timeout_server / 6);
  }

  //  Get rid of un-selected roles.
  $roles = $form_state['values']['state__prlngcnd'];
  foreach ($roles as $k => $v) {
    if (!$v) {
      unset($roles[$k]);
    }
  }
  $form_state['values']['state__prlngcnd'] = $roles;
  $roles = $form_state['values']['state__prlnguncnd'];
  foreach ($roles as $k => $v) {
    if (!$v) {
      unset($roles[$k]);
    }
  }
  $form_state['values']['state__prlnguncnd'] = $roles;

  //  Fix format of redirect target.
  if (strlen($v = trim($form_state['values']['state__rdrtrg']))) {
    if (!preg_match('/^(\/|http)/', $v)) { // Ensure starting slash if none and not absolute url.
      $v = '/' . $v;
    }
  }
  $form_state['values']['state__rdrtrg'] = $v;

  //  Fix format of non-page paths.
  if (strlen($v = trim($form_state['values']['state__nonpages']))) {
    $built_ins = State::nonPagePaths(TRUE);
    $paths = array();
    $le = count($a = explode("\n", str_replace("\r", '', $v)));
    for ($i = 0; $i < $le; $i++) {
      if (!strlen($v = trim($a[$i]))) {
        continue;
      }
      if ($v{0} == '/') { // Remove starting slash.
        $v = substr($v, 1);
      }
      if (!strlen($v)) {
        continue;
      }
      if ($v{ strlen($v) - 1 } != '/') { // Secure trailing slash.
        $v .= '/';
      }
      if ($v != '/' && !in_array($v, $built_ins)) {
        $paths[] = $v;
      }
    }
    $form_state['values']['state__nonpages'] = $paths ? join("\n", $paths) : '';
  }

  //  Log configuration update.
  global $user;
  watchdog(
    'state',
    'State configuration updated by user (%uid) %name.',
    array('%uid' => $user->uid, '%name' => $user->name)
  );
}
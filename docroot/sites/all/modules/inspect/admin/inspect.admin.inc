<?php
/**
 * @file
 * Drupal Inspect module administration layer
 */

//D8:use Drupal\Inspect\Inspect;

/**
 * Defines configuration form fields.
 *
 * @param array $form
 * @param array &$form_state
 *
 * @return array
 */
function inspect_admin_form($form, &$form_state) {
  drupal_add_css(
    drupal_get_path('module', 'inspect') . '/admin/inspect.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );
  $form['help_php'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help - PHP'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'content' => array(
      '#type' => 'markup',
      '#markup' => _inspect_help_php(),
    ),
  );
  $form['help_js'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help - Javascript'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'content' => array(
      '#type' => 'markup',
      '#markup' => _inspect_help_js(),
    ),
  );

  // Establish max/default values for outmax lengths.
  $output_max_default = Inspect::outputMax(TRUE);
  $output_max_absolute = min($output_max_default, Inspect::OUTPUT_MAX);

  $form['output_fuses'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fuses - graceful degradation'),
    '#description' => t(
      'Maximum output lengths of dumps/traces/profiles secure that Inspect degrades gracefully when analyzing an overly long data set.
!breakThe variable inspector will make a second attempt when reaching maximum output (using less depth and higher truncation),
!breakwhereas the tracer automatically reduces stack frame limit when reaching maximum output.
!breakInspections and traces also abort (cleanly) if more than a certain percent of PHP\'s max execution time has passed.'
      , array('!break' => '<br/>')
    ),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'inspect_output_max' => array(
      '#type' => 'textfield',
      '#title' => t('Maximum output length'),
      '#description' => t(
        'Absolute max is !max. Equvalent of MySql max_allowed_packet: !default bytes ~ !mb Mb.!breakActual max will be slightly shorter to allow for metadata and database query.',
        array('!max' => $output_max_absolute, '!default' => $output_max_default, '!mb' => round($output_max_default / 1048576, 1), '!break' => '<br/>')
      ),
      // If no previous setting, and the suggested setting is larger than default: use default.
      '#default_value' => variable_get('inspect_output_max', $output_max_default > Inspect::OUTPUT_DEFAULT ? Inspect::OUTPUT_DEFAULT : $output_max_default),
      '#size' => 8,
      '#required' => TRUE,
    ),
    'inspect_exectime_percent' => array(
      '#type' => 'textfield',
      '#title' => t(
        'Abort variable/trace inspection if called later than this percent of max execution time (@max_execution_time seconds)',
        array('@max_execution_time' => ini_get('max_execution_time'))
      ),
      '#description' => t(
        'If PHP max_execution_time is being changed during run-time - set_time_limit() - this \'fuse\' will not work predictably.
!breakBecause max_execution_time is only being checked at first inspection/trace in a request,
!breakand any later changes will not be reflected by Inspect\'s time limit \'fuse\' in that request.',
        array('!break' => '<br/>')
      ),
      '#default_value' => variable_get('inspect_exectime_percent', 90),
      '#size' => 2,
      '#required' => TRUE,
    ),
  );

  $form['inspection_tracing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inspection and tracing'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'inspect_truncate' => array(
      '#type' => 'textfield',
      '#title' => t('Default string truncation'),
      '#description' => t(
        'Maximum: !max.',
        array('!max' => Inspect::TRUNCATE_MIN)
      ),
      '#default_value' => variable_get('inspect_truncate', Inspect::TRUNCATE_DEFAULT),
      '#size' => 5,
      '#required' => TRUE,
    ),
    'inspect_trace_limit' => array(
      '#type' => 'textfield',
      '#title' => t('Error/back tracer\'s default stack frame limit'),
      '#description' => t(
        'Recommended value: !default; a higher a value (deeper stack tracing) will rarely provide additional usable information.',
        array('!default' => Inspect::TRACE_LIMIT_DEFAULT)
      ),
      '#default_value' => variable_get('inspect_trace_limit', Inspect::TRACE_LIMIT_DEFAULT),
      '#size' => 2,
      '#required' => TRUE,
    ),
    'inspect_fronttoback_sevmax' => array(
      '#type' => 'select',
      '#title' => t('Maximum severity when frontend logs to backend'),
      '#description' => t('A frontend event having higher severity will be logged using this severity.'),
      '#options' => watchdog_severity_levels(),
      '#default_value' => variable_get('inspect_fronttoback_sevmax', WATCHDOG_ERROR),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
    ),
    'inspect_session_counters' => array(
      '#type' => 'checkbox',
      '#title' => t('Enable session counters'),
      '#description' => t('Not recommendable in daily use on a production site, because this feature uses and updates a cookie, and it also adds a slight performance hit to each and every request.'),
      '#default_value' => variable_get('inspect_session_counters', 0),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
    ),
  );

  $form['#submit'][] = '_inspect_admin_form_submit';
  return system_settings_form($form);
}

/**
 * @param array $form
 * @param array &$form_state
 */
function _inspect_admin_form_submit($form, &$form_state) {
  $output_max_default = Inspect::outputMax(TRUE);
  $output_max_absolute = min($output_max_default, Inspect::OUTPUT_MAX);

  $values =& $form_state['values'];
  $values['inspect_output_max'] = !($val = (int)trim($values['inspect_output_max'])) || $val < 0 || $val > $output_max_absolute ?
    $output_max_default : $val;

  $values['inspect_exectime_percent'] = !($val = (int)trim($values['inspect_exectime_percent'])) || $val < 10 ? 10 : ($val > 95 ? 95 : $val);

  $values['inspect_truncate'] = !($val = (int)trim($values['inspect_truncate'])) || $val < 0 || $val > Inspect::TRUNCATE_MIN ?
    Inspect::TRUNCATE_DEFAULT : $val;

  $values['inspect_trace_limit'] = !($val = (int)trim($values['inspect_trace_limit'])) || $val < 0 ? 5 :
    ($val > Inspect::TRACE_LIMIT_MAX ? Inspect::TRACE_LIMIT_MAX : $val);
}

/**
 * Implements hook_help().
 *
 * @param string $path
 *
 * @return string
 */
function _inspect_help($path) {
  if ($path != 'admin/help#inspect') {
    return '';
  }
  $module = 'inspect';
  $headline_tag = 'h3';
  $paragraph_start = 'p class="admin-help-module-' . $module . '-paragraph"';
  $module_path = drupal_get_path('module', $module);

  drupal_add_css(
    $module_path . '/admin/inspect.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );

  $out = '<div><div class="admin-help-module-' . $module . '-pane-first">';

  $out .= '
<' . $headline_tag . '>' . t('Inspect variables, constants, error- and back traces, and profile execution times') . '</' . $headline_tag . '>
<h4>' . t('Backend as well as frontend') . '</h4>
<' . $paragraph_start . '>'
  . t(
'More detailed documentation available here: <a href="@doc_backend" target="_blank">PHP</a> and <a href="@doc_frontend" target="_blank">Javascript</a>.',
    array(
      '@doc_backend' => 'http://www.simple-complex.net/docs/drupal/modules/inspect/php/inspect_8module.html',
      '@doc_frontend' => 'http://www.simple-complex.net/docs/drupal/modules/inspect/js/symbols/inspect.html',
    )
  )
  . '</p>';
  $out .= '<br/>' . _inspect_help_php() . '<br/>' . _inspect_help_js();
  $out .= '</div><div class="admin-help-module-' . $module . '-pane-second">';
  //  Parse README.txt to html.
  $out .= '<a name="readme"></a><h3>Readme</h3>';
  $readme = @file_get_contents($module_path . '/README.txt');
  $out .= str_replace('<p>', '<' . $paragraph_start . '>', _inspect_readme2html($readme) )
    . '</div></div><hr/>';
  return $out;
}

/**
 * @return string
 */
function _inspect_help_php() {
  $options = array(
    '(integer) depth' => t(
      'default !default, max !max - $options as integer is interpretated as depth',
      array('!default' => Inspect::DEPTH_DEFAULT, '!max' => Inspect::DEPTH_MAX)
    ),
    '(integer) truncate' => t('default !default, min !max', array('!default' => Inspect::TRUNCATE_DEFAULT, '!max' => Inspect::TRUNCATE_MIN)),
    '(integer|float) output_max' => t('float less than 1 will be multiplied by default max'),
    '(string) message' => t('will be truncated to 255 - $options as string is interpretated as message'),
    '(string) type' => t('logging type (default \'inspect\')'),
    '(integer) severity' => t('default WATCHDOG_DEBUG'),
    '(integer) wrappers' => t('the (inspect()/inspect_*()) function/method is wrapped in one or more local logging functions/methods'),
    '(string) name' => t('\'$var_name\' or "\\$var_name", must be escaped'),
    '(bool) hide_scalars' => t('hide values of scalar vars, and type of resource'),
    '(bool) hide_paths' => t('only relevant for log (paths always hidden for get)'),
    '(bool) one_lined' => t('create one-lined formatting instead of multi-lined (and default depth 1)'),
  );
  $s = '<h5>' . t('(array) $options for PHP inspection functions') . ':</h5>'
    . '<ul>';
  foreach ($options as $k => $v) {
    $s .= '<li><span class="code">' . $k . '</span><span class="code weak">: ' . $v . '</span></li>';
  }
  $s .= '</ul>';
  $options = array(
    '(integer) limit' => t(
      'default !default, max !max - $options as integer is interpretated as limit',
      array('!default' => variable_get('inspect_trace_limit', 5), '!max' => Inspect::TRACE_LIMIT_MAX)
    ),
    '(integer) depth' => t(
      'default !default',
      array('!default' => Inspect::DEPTH_DEFAULT)
    ),
    '(integer) truncate' => t('default !default, min !max', array('!default' => Inspect::TRUNCATE_DEFAULT, '!max' => Inspect::TRUNCATE_MIN)),
    '(string) type' => t('default \'inspect trace\''),
  );
  $s .= '<h5>' . t('(array) $options for PHP tracing functions - like inspection, except') . ':</h5>'
    . '<ul>';
  foreach ($options as $k => $v) {
    $s .= '<li><span class="code">' . $k . '</span><span class="code weak">: ' . $v . '</span></li>';
  }
  $s .= '</ul>';
  $s .= '<h5>' . t('Functions') . '</h5>';
  if (($funcs = inspect_doc_functions('inspect', '',
      array('inspect_args', 'inspect_args_get', 'inspect_constants', 'inspect_constants_get', 'inspect_ajax_callback')))
  ) {
    foreach ($funcs as $v) {
      $s .= '<p>' . ($v[2] ? ($v[2] . '<br/>') : '')
        . '<span class="code emph">' . $v[0] . '</span>'
        . '<span class="code">' . $v[1] . '</span></p>';
    }
  }
  return $s;
}

/**
 * @return string
 */
function _inspect_help_js() {
  $options = array(
    '(integer) depth' => t(
      'default !default, max !max - and (integer) options interprets to depth',
      array('!default' => 10, '!max' => 10)
    ),
    '(boolean) protos' => t('analyze prototypal properties too - and (boolean) options arg also interprets to protos, and (string) options arg \'protos\' also interprets to protos'),
    '(boolean) func_body' => t('print bodies of functions - and (string) options arg \'func_body\' also interprets to func_body'),
    '(string) message' => t('message - and (string) options arg also interprets to message (except when \'protos\'/\'func_body\''),
    '(integer) wrappers' => t('the (inspect()/inspect.*()) function is wrapped in one or more local logging functions/methods'),
    '(string) type' => t('.log() and .traceLog() only - default \'inspect\'/\'inspect trace\''),
    '(string) severity' => t('.log() and .traceLog() only - default \'debug\'/\'error\''),
  );
  $funcs = array(
    array(
      'inspect',
      '(u, options)',
      'Inspect variable and send output to console log.',
    ),
    array(
      'inspect.get',
      '(u, options)',
      'Inspect variable and get output as string.',
    ),
    array(
      'inspect.log',
      '(u, options)',
      'Inspect variable and log to backend.',
    ),
    array(
      'inspect.typeOf',
      '(u)',
      'Get type or class.',
    ),
    array(
      'inspect.trace',
      '(er, options)',
      'Trace error and send output to console log.',
    ),
    array(
      'inspect.traceGet',
      '(er, options)',
      'Trace error and get output as string.',
    ),
    array(
      'inspect.traceLog',
      '(er, options)',
      'Trace error and log trace to backend.',
    ),
    array(
      'inspect.argsGet',
      '(args, names)',
      'Analyze function arguments, or an options object.',
    ),
    array(
      'inspect.events',
      '(selector, options)',
      'Inspect event listeners added via jQuery, and send output to console.',
    ),
    array(
      'inspect.eventsGet',
      '(selector, options)',
      'Inspect event listeners added via jQuery, and get output as string.',
    ),
    array(
      'inspect.eventsLog',
      '(selector, options)',
      'Inspect event listeners added via jQuery, and log output to backend.',
    ),
    array(
      'inspect.console',
      '(message)',
      'Log to browser console, if exists.',
    ),
    array(
      'inspect.logMessage',
      '(message, type, severity)',
      'Log message to backend.',
    ),
    array(
      'inspect.errorHandler',
      '(error, variable, options)',
      'Log error trace or variable inspection to console and/or backend log',
    ),
    array(
      'inspect.toggleOutput',
      '()',
      'Toggle display of all PHP inspection outputs on current page - or click the ¡!-button.',
    ),
    array(
      'inspect.toggleFolding',
      '()',
      'Toggle PHP inspection folding - in recent logs: click the ¡!-button.',
    ),
  );
  $vars = array(
    array(
      'browser',
      '(object)',
      'Lists browser version (as float) by useragent identifier.',
    ),
    array(
      'tcepsni',
      '(boolean)',
      'Flags whether inspect is the active or passive (all functions return empty) version.',
    ),
    array(
      'tcepsnI',
      '(boolean)',
      'Flags whether inspect is the active or passive (all functions return empty) version.',
    ),
  );
  $s = '<p>' . t('The library is available as Drupal.inspect, inspect and Inspect.') . '</p>'
    . '<h5>' . t('(object) options for Javascript inspect()') . ':</h5>'
    . '<ul>';
  foreach ($options as $k => $v) {
    $s .= '<li><span class="code">' . $k . '</span><span class="code weak">: ' . $v . '</span></li>';
  }
  $s .= '</ul>';

  $s .= '<h5>' . t('Functions') . '</h5>';
  foreach ($funcs as $v) {
    $s .= '<p>' . ($v[2] ? ($v[2] . '<br/>') : '')
      . '<span class="code emph">' . $v[0] . '</span>'
      . '<span class="code">' . $v[1] . '</span></p>';
  }
  $s .= '<h5>' . t('Vars') . '</h5>';
  foreach ($vars as $v) {
    $s .= '<p>' . ($v[2] ? ($v[2] . '<br/>') : '')
      . '<span class="code">' . $v[1] . '</span>'
      . ' <span class="code emph">' . $v[0] . '</span></p>';
  }
  return $s;
}

/**
 * Parse README.txt to html.
 *
 *   Replacements:
 *   - formats headlines; line with no hyphens followed by all hyphens line
 *   - hyphen to bullet; newline+space+*+space becomes newline+space+bullet+space
 *   - emphasizes underscore fragments; _what ever_ becomes <em>what ever</em>
 *   - turns url into link, and http://your-drupal-site.tld/some-path becomes internal link to /some-path
 *   - turns mail address into mailto link
 *
 * Some replacements (like emphasizing) only works for ascii letters, not for letters line like ñ or ö.
 *
 * @param string $readme_txt
 * @param string $headline_tag
 *    Default: h5.
 *
 * @return string
 */
function _inspect_readme2html($readme_txt, $headline_tag = 'h5') {
  if (!strlen($readme_txt)) {
    return 'empty readme';
  }
  $ndls = array(
    '/</',
    '/>/',
    '/\r\n|\r/', // CR -> NL
    '/[\x20\t]+\n/', // trailing spaces (in lines)
    '/\n{3,}/', // 3 or more newlines to double
    '/<([^>@]+@[^>@]+)>/', // mail addresses
    '/([\n\x20])_([a-zA-Z\d][^\.,;\:\n\x20]*[a-zA-Z\d])_([\n\x20\.,;\:])/', // _emphasize_ -> <em>emphasize</em>
  );
  $rplcs = array(
    '&lt;',
    '&gt;',
    "\n",
    "\n",
    "\n\n",
    '<a href="mailto:$1">&#60;$1&#62;</a>',
    '$1<em>$2</em>$3',
  );
  $s = preg_replace($ndls, $rplcs, $readme_txt);

  //  Insert links in CONTENTS OF THIS FILE.
  if (strpos($s, 'CONTENTS OF THIS FILE') !== FALSE) {
    $pos_start = strpos($s, ' * ');
    $pos_end = strpos($s, "\n\n", $pos_start);
    $s_toc = preg_replace('/[ ]+\n/', "\n", substr($s, $pos_start, ($pos_end - $pos_start) + 1) ); // remove trailing space(s)
    //echo $s_toc; exit;
    $s_toc = preg_replace_callback(
      '/ \* ([^\n]+)\n/',
      create_function(
        '$ms',
        '
        return \' * <a href="#\' .
          strtolower( preg_replace(\'/[^a-zA-Z_\-]/\', \'_\', $ms[1]) )
          . \'">\' . $ms[1] . \'</a>\' . "\n";
        '
      ),
      $s_toc
    );
    $s_start = substr($s, 0, $pos_start); // Deliberately not drupal_substr(); multibyte irrelevant.
    $s_end = substr($s, $pos_end); // Deliberately not drupal_substr(); multibyte irrelevant.
    $s = str_replace('CONTENTS OF THIS FILE', 'CONTENTS', $s_start) . $s_toc . $s_end;
  }

  //  Format headlines, and insert anchor.
  $s = preg_replace_callback(
    '/\n([^\n\-])([^\n]*[^\n\-])\n[\-]{2,}\n+/',
    create_function(
      '$ms',
      '
      return \'</p><a name="\'
        . strtolower( preg_replace(\'/[^a-zA-Z_\-]/\', \'_\', $ms[1] . $ms[2]) )
        . \'"></a><headlineTag style="margin-top:20px;">\'
        . $ms[1]
        . \'<span style="text-transform:lowercase;">\' . $ms[2] . \'</span></headlineTag><p>\';
      '
    ),
    $s
  );
  $s = str_replace('headlineTag', $headline_tag, $s);

  $ndls = array(
    '/^[^\n]*\n/', // remove first line
    //'//',
  );
  $rplcs = array(
    '',
    //'',
  );
  $s = preg_replace($ndls, $rplcs, $s);
  //  Links may be followed by a punctuation marker, hard to detect without a callback.
  //  And http://your-drupal-site.tld/path should become /path
  $s = preg_replace_callback(
    '/(https?\:\/\/)(\S+)([\x20\t\n])/',
    create_function(
      '$ms',
      '
      $protocol = $ms[1];
      $le = strlen($addr = $ms[2]);
      $dot = \'\';
      if (!preg_match(\'/[\/a-zA-Z\d]/\', $addr{$le - 1})) {
        $dot = $addr{$le - 1};
        $addr = substr($addr, 0, $le - 1);
      }
      if (strpos($addr, \'your-drupal-site.tld/\') === 0) {
        $protocol = \'/\';
        $addr = str_replace(\'your-drupal-site.tld/\', \'\', $addr);
      }
      return \'<a href="\' . $protocol . $addr . \'">\' . $addr . \'</a>\' . $dot . $ms[3];
      '
    ),
    $s
  );
  $ndls = array(
    '/([\n>]) \* /', // \n *  to \n bullet
    '/^\n?<\/p>/', // first ending <p> added by headline (in total)
    '/\n?$/', // trailing newlines (in total)
    '/\n{2,}/',
    //'//',
  );
  $rplcs = array(
    '$1 &bull; ',
    '',
    '',
    '</p><p>',
    //'',
  );
  return nl2br( preg_replace($ndls, $rplcs, $s) ) . '</p>';
}

<?php
/**
 * @file
 * Include file for Writup Filter module containing functions that are common between different versions of Drupal.
 *
 */

/**
 * Common Writeup Processing
 *
 * @param $tmpfile
 *   Name of temporary file in which unprocessed text was saved
 *
 * @param $formatname
 *   Full name of format to pass to Writeup in $format variable
 *
 * @param $format_incname
 *   Include file specified for this input format
 *
 * @param $format_settings
 *   Any special settings for this input format
 *
 * @return
 *   string containing the processed text
 */
function _writeup_process($tmpfile, $formatname, $format_incname, $format_settings) {
  $incfile = rtrim(variable_get('writeup_incdir', ''), '/\\') . '/' . $format_incname;
  if (!is_file($incfile)) $incfile='NONE';
  $settings = "-s include=\"$incfile\" -s format=\"$formatname\" " . escapeshellcmd($format_settings);
  $ver = variable_get('writeup_ver', '');
  if ($ver != '') $settings .= " -s VER=$ver";
  $writeup = rtrim(variable_get('writeup_loc', ''), '/\\') . '/writeup';
  $command = "$writeup -Iyqbrc $settings $tmpfile";
  $output = array();
  $retvar = 0;
  exec($command, $output, $retvar);
  $errorblock = "";
  if ($retvar != 0) {
    $errors = array();
    _writeup_get_errors($tmpfile, $formatname, $format_incname, $format_settings, $errors);
    if ($retvar == 1) {
      $severity =  WATCHDOG_WARNING;
      $warnings = t('There are warnings in your content:');
    }
    else {
      $severity =  WATCHDOG_ERROR;
      $warnings = t('There are errors in your content:');
    }
    $errormsg = '<pre>' . $errors . '</pre>';
    if (variable_get('writeup_logerrors', 0)) watchdog('writeup', $errormsg, NULL, $severity);
    if (variable_get('writeup_showerrors', 1)) $errorblock = '<div class="messages error writeuperror">' . $errormsg . '</div>';
    drupal_set_message($warnings . $errormsg, 'error');
  }
  return $errorblock . implode("\n", $output);
}

/**
 * Test Writeup document and get any error messages
 *
 * @param $tmpfile
 *   Name of temporary file in which unprocessed text was saved
 *
 * @param $formatname
 *   Full name of format to pass to Writeup in $format variable
 *
 * @param $format_incname
 *   Include file specified for this input format
 *
 * @param $format_settings
 *   Any special settings for this input format
 *
 * @param $errors
 *   String containing any error messages
 *
 * @return
 *   Return code from Writeup (0=good)
 */
function _writeup_get_errors($tmpfile, $formatname, $format_incname, $format_settings, &$errors='') {
  $incfile = rtrim(variable_get('writeup_incdir', ''), '/\\') . '/' . $format_incname;
  if (!is_file($incfile)) $incfile='NONE';
  $settings = "-s include=\"$incfile\" -s format=\"$formatname\" " . escapeshellcmd($format_settings);
  $ver = variable_get('writeup_ver', '');
  if ($ver != '') $settings .= " -s VER=$ver";
  $writeup = rtrim(variable_get('writeup_loc', ''), '/\\') . '/writeup';
  $command = "$writeup -Iyqbc -X NULL $settings $tmpfile 2>&1";
  $output = array();
  $retvar = 0;
  exec($command, $output, $retvar);
  $errors = filter_xss_admin(implode("\n", $output)); // sanitize before display
  return $retvar;
}

/********************** Admin Functions **************************/
/**
 * Implements hook_help().
 * Display help and module information
 *
 * @param path
 *   which path of the site we're displaying help
 *
 * @param arg
 *   array that holds the current path as would be returned from arg() function
 *
 * @return
 *   help text for the path
 */
function writeup_help($path = 'admin/help#writeup', $arg) {
  $output = ''; // declare output variable
  switch ($path) {
    case "admin/help#writeup":
      $output = '<p>' . t('Input filter that processes text using the <a href="http://writeup.org">Writeup</a> markup language. See <a href="http://writeup.org">writeup.org</a> for details.')
         . '</p><p>' . t('In order for the filter to work, the Writeup executable binary must be installed on the server.')
         . '</p><p>' . t('The linux binary may be downloaded here: https://writeup.googlecode.com/svn/bin/writeup')
         . '</p><p>' . t('or from http://sourceforge.net/projects/writeup/files') . '</p>';
      break;
  }
  return $output;
}

/**
 * Implements hook_menu().
 */
function writeup_menu() {
  $items = array();
  $items[ADMIN_WRITEUP] = array( // value of 'ADMIN_WRITEUP' defined as a constant
    'title' => 'Writeup settings',
    'description' => 'Administer Writeup',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('writeup_admin_settings'),
    'access arguments' => array('administer Writeup input filter'),
    'type' => MENU_NORMAL_ITEM,
  );
  $items['writeup_status'] = array(
    'title' => 'Status of pages that use the Writeup input filter',
    'description' => 'List status of nodes that use the Writeup input filter',
    'page callback' => 'writeup_status_page',
    'access arguments' => array('view status of Writeup pages'),
    'type' => MENU_CALLBACK,
  );
  return $items;
}

/**
 * Administration form.
 */
function writeup_admin_settings() {
  $form['writeup_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global settings for Writeup input filter'),
    '#description' => '<p>' . t('Here is where common settings for the Writeup input filter are set. Each format can be separately configured on the input format configuration page where different include files can be assigned to each input format. <em>It is essential that this be done for the filter to work.</em></p><ul><li>See: ')
     . l(ADMIN_FORMATS, ADMIN_FORMATS) . '</li><li>'
     . t('All pages that use the Writeup filter in their bodies are listed here: ') . l('writeup_status', 'writeup_status') . '</li></ul>',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['writeup_admin']["writeup_loc"] = array(
    '#type' => 'textfield',
    '#title' => t('Location for Writeup binary (common to all formats).'),
    '#description' => t('The directory containing the Writeup executable binary, e.g. /opt/writeup')
       . '<br />' . t('Found version:') . ' ' . _writeup_version(),
    '#default_value' => variable_get('writeup_loc', ''),
    '#after_build' => array('writeup_check_directory'),
    '#size' => 70,
  );
  $form['writeup_admin']["writeup_incdir"] = array(
    '#type' => 'textfield',
    '#title' => t('Directory for definitions include file(s) (common to all formats).'),
    '#description' => t('The directory for definitions files that are included in writeup processing.') . '<br />'
      . t('It is suggested that this be somewhere in your theme directory, e.g. sites/default/themes/<em>mytheme</em>'),
    '#default_value' => variable_get('writeup_incdir', drupal_get_path('module', 'writeup')),
    '#after_build' => array('writeup_check_directory'),
    '#size' => 70,
  );
  $form['writeup_admin']["writeup_ver"] = array(
    '#type' => 'textfield',
    '#title' => t('Default version of Writeup (common to all formats).'),
    '#description' => t('The setting $VER=n.nn is prepended to each file before processing. Leave blank if not required.'),
    '#default_value' => variable_get('writeup_ver', '2.50'),
    '#after_build' => array('writeup_check_number'),
    '#size' => 40,
  );
  $form['writeup_admin']['writeup_logerrors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log all Writeup errors'),
    '#description' => t('Create an entry in the log every time there is an error processing a page.
      This makes it easier to ensure that there are no errors in any pages on the site.'),
    '#default_value' => variable_get('writeup_logerrors', 0),
  );
  $form['writeup_admin']['writeup_showerrors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show all Writeup errors'),
    '#description' => t('Show all Writeup errors in a block at the top of the content.'),
    '#default_value' => variable_get('writeup_showerrors', 1),
  );
  return system_settings_form($form);
}

/**
 * Checks the existence of the directory specified in $form_element.
 * If validation fails, the form element is flagged.
 *
 * @param $form_element
 *   The form element containing the name of the directory to check.
 */
function writeup_check_directory($form_element) {
  $directory = rtrim($form_element['#value'], '/\\');
  if (!is_dir($directory)) {   // Check if directory exists.
    form_set_error($form_element['#parents'][0], t('The directory %directory does not exist.', array('%directory' => $directory)));
  }
  return $form_element;
}

/**
 * Checks the existence of the file specified in $form_element.
 * If validation fails, the form element is flagged.
 *
 * @param $form_element
 *   The form element containing the name of the file to check.
 */
function writeup_check_file($form_element) {
  $directory = rtrim(variable_get('writeup_incdir', ''), '/\\');
  if (!is_dir($directory)) {   // Check if directory exists.
    form_set_error($form_element['#parents'][0], t('The Writeup directory %directory set on the administration page does not exist.', array('%directory' => $directory)));
  }
  else {
    $path = $directory . '/' . $form_element['#value'];
    if (!is_file($path)) {   // Check if file exists.
      form_set_error($form_element['#parents'][0], t('The file %path does not exist.', array('%path' => $path)));
    }
  }
  return $form_element;
}

/* As above but allows an empty filename */
function writeup_check_file_or_empty($form_element) {
  if ($form_element['#value'] == '') return $form_element;
  else return writeup_check_file($form_element);
}

/**
 * Validates a number in $form_element.
 * If validation fails, the form element is flagged.
 *
 * @param $form_element
 *   The form element containing the name of the file to check.
 */
function writeup_check_number($form_element) {
  $number = rtrim($form_element['#value'], '/\\');
  if (!is_numeric($number)) {   // Check if directory exists.
    form_set_error($form_element['#parents'][0], t('Not a valid number: %number', array('%number' => $number)));
  }
  return $form_element;
}

/**
 * Returns version of Writeup binary
 *
 * @return
 *   version of Writeup binary, or error message
 */
function _writeup_version() {
  $versionmin = 2.51;
  $err = ' <span style="color:red;font-weight:bold;">';
  $errend = '</span>';
  $writeup = rtrim(variable_get('writeup_loc', ''), '/\\') . '/writeup';
  if (!is_file($writeup)) {   // Check if Writeup binary exists.
    $msg =  $err . t('No Writeup binary executable was found in this folder.') . $errend;
  }
  else {
    $msg = shell_exec("$writeup --version");
    if ($versionmin > shell_exec("$writeup --versionnum")) {
      $msg .= $err
      . t('Error: module requires a minimum of version %versionmin of the Writeup binary.', array('%versionmin' => $versionmin))
      . $errend;
    }
  }
  return $msg;
}

/**
 * Called by _writeup_filter_tips().
 * Does all the work in making the filter tips.
 *
 * @param $format_specific_help
 *   If set, this will override both the short and long help for this format
 *
 * @param $long
 *   True if long format for help
 */
function _writeup_tips($format_specific_help, $long) {
  if ($format_specific_help != '') {
    return filter_xss($format_specific_help);
  }
  else {
    if ($long) {
      return t('Quick Tips:<ul>
        <li>_underscores_ => <em>Emphasis</em></li>
        <li>*asterisks* => <strong>Strong</strong></li>
        <li><strong>-</strong> at start of line => unordered lists</li>
        <li><strong>1.</strong> at start of line => ordered lists (or <strong>A. a. I.</strong> etc.)</li>
        <li><strong>--</strong> => &mdash; (em dash)</li>
        <li><strong>[[imageref.jpg</strong> <em>optional alt words</em><strong>]]</strong> => &lt;img src="<strong>imageref.jpg</strong>" alt="<em>optional alt words</em>"&gt;</li>
        <li><strong>[[http://gw.ca</strong> <em>optional linking text</em><strong>]]</strong> => &lt;a href="<strong>http://gw.ca</strong>"&gt;<em>optional linking text</em>&lt;/a&gt;</li>
        <li><strong>..</strong>Heading level 2</li>
        <li><strong>....</strong>Heading level 4 etc.</li>
        </ul>For complete details on the Writeup syntax, see the <a href="http://writeup.org/quickref">Writeup documentation</a>');
    }
    else {
      return t('You can use <a href="@filter_tips">Writeup syntax</a> to format and style the text.<br />
                e.g. *bold*, _italics_, --emdash, "-" at start of line for lists, .heading1 ..heading2 etc.',
                array('@filter_tips' => url('filter/tips')));
    }
  }
}

/**
 * Modified version of Drupal 7 function until core adds new tags
 */
function writeup_filter_xss_admin($string) {
  // over twice as fast if this is static since array does not have to be built on every call
  static $allowed_tags = array('a', 'abbr', 'acronym', 'address', 'article', 'aside', 'b', 'bdi', 'bdo', 'big', 'blockquote', 'br', 'caption', 'cite', 'code', 'col', 'colgroup', 'command', 'dd', 'del', 'details', 'dfn', 'div', 'dl', 'dt', 'em', 'figcaption', 'figure', 'footer', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6', 'header', 'hgroup', 'hr', 'i', 'img', 'ins', 'kbd', 'li', 'mark', 'menu', 'meter', 'nav', 'ol', 'output', 'p', 'pre', 'progress', 'q', 'rp', 'rt', 'ruby', 's', 'samp', 'section', 'small', 'span', 'strong', 'sub', 'summary', 'sup', 'table', 'tbody', 'td', 'tfoot', 'th', 'thead', 'time', 'tr', 'tt', 'u', 'ul', 'var', 'wbr', '!--',
  'audio', 'canvas', 'embed', 'fieldset', 'legend', 'path', 'rect', 'source', 'svg', 'track','video' // new tags added
  );
  // 'object' tag not added because of the <object data="javascript:..."> attack
  return filter_xss($string, $allowed_tags);
}

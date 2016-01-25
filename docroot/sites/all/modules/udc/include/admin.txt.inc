<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 - User Data Connector module - configuration/help texts
 * -----------------------------------------------------------------------------
 *
 * @author Stefan Wilhelm (stfwi)
 * @package org.drupal.project.udc
 * @license GPL2
 */

/**
 * Returns highlighted PHP code of a given file path
 *
 * @param string $file
 * @return string
 */
function udc_admin_get_highlighted_source($file) {
  @ob_start();
  @highlight_string(@file_get_contents($file));
  return '<br/><br/><div style="background-color: #eee;">' . @ob_get_clean() . '</div>';
}

/**
 * Returns a translated version of a html text. Text between html tags will be
 * html decoded for the translation tables, then check_plain() is performed.
 *
 * @param string $html
 * @return string
 */
function udc_admin_get_translated_html($html) {
  $chunks = preg_split('/(<[^>]*[^\/]>)/i', $html, -1, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
  foreach ($chunks as $chunk) {
    if (strpos($chunk, '<') === FALSE) {
      $chunk = check_plain(t(str_ireplace(array_values(get_html_translation_table()), array_keys(get_html_translation_table()), $chunk)));
    }
  }
  return implode('', $chunks);
}

/**
 * Parses and formats the README file (only once) into an array and returns the
 * specified section text.
 *
 * @param string $section
 * @param bool $only_nonempty_sections
 * @return string
 */
function udc_admin_get_readme_section($section, $only_nonempty_sections=TRUE) {
  static $sections = NULL;
  if (!is_array($sections)) {
    // Read whole file to store in static $sections as array.
    $sections = array();
    $file = dirname(dirname(__FILE__)) . '/README.txt';

    // Leave, the next time this function is called static $sections=array().
    if (!is_file($file)) {
      return array();
    }

    // Parse text file, which has to be in proper indentation format.
    $readme = explode("\n", trim(str_replace(array("\r\v", "\t"), array('', '  '), file_get_contents($file)), "\n"));
    $text = '';
    $sect = NULL;

    while (!empty($readme)) {
      $line = rtrim(array_shift($readme));
      $indent = drupal_strlen($line) - drupal_strlen(ltrim($line));
      $line = ltrim($line);

      if (empty($line)) {
        // Add blank element, which means "new paragraph".
        if (!empty($sect)) {
          $sect[] = '';
        }
      } elseif ($indent == 0) {
        // No indent means section header, only one header line is allowed.
        $sections[$line] = array();
        $sect = &$sections[$line];
      } else {
        // At least one indent menas section content

        if (strpos($line, '*') === 0) {
          // This is interpreted as list, list item lasts until a new list
          // marker or an empty line is found. No nested lists allowed yet,
          // a recursive solution would be overdone here.
          $list = $line;
          while (!empty($readme)) {
            $line = trim(array_shift($readme));
            if (empty($line)) {
              // End of list
              break;
            } elseif (strpos($line, '*') === 0) {
              // Next list item
              $sect[] = $list;
              $list = $line;
            } else {
              // Append list item text
              $list .= ' ' . $line;
            }
          }

          if (!empty($list)) {
            // Add list item at EOF
            $sect[] = $list;
          }

          // Add blank line for paragraph combining.
          $sect[] = '';
        } else {
          $sect[] = $line;
        }
      }
    }
    // Dereference $sect
    unset($sect);

    // Combine paragraph texts
    foreach ($sections as $title => $sect) {
      $csect = array();
      $text = '';
      foreach ($sect as $line) {
        $text .= ' ' . $line;
        if (trim($text) != '' && $line == '' || strpos($line, '*') === 0) {
          $csect[] = trim($text);
          $text = '';
        }
      }
      if ($text != '') {
        $csect[] = trim($text);
      }

      // Translate and check_plain every paragraph/list item.
      $was_li = FALSE;
      $text = '<p>';
      foreach ($csect as $line) {
        if (strpos($line, '*') === 0) {
          $text .= (!$was_li ? "</p>\n<ul>" : '');
          $line = check_plain(t(ltrim($line, '* ')));
          $line = preg_replace('/((http|https|ftp):\/\/[\w\/\.]+)/i', '<a href="$1">$1</a>', $line);
          $text .= "<li>" . $line . "</li>\n";
          $was_li = TRUE;
        } else {
          $text .= ($was_li ? "</ul>\n<p>" : "</p>\n<p>");
          $line = check_plain(t($line));
          $line = preg_replace('/((http|https|ftp):\/\/[\w\/\.]+)/i', '<a href="$1">$1</a>', $line);
          $text .= $line;
          $was_li = FALSE;
        }
      }
      $text .= ($was_li ? "</ul>\n" : "</p>\n");
      $text = preg_replace('/<p>[\s]*<\/p>/i', '', $text);
      $ttitle = check_plain(t($title));
      $text = trim($text, "\n ");
      $empty = empty($text);
      $text = "\n\n<h1>$ttitle</h1>\n" . $text;

      $sections[$title] = array(
          'empty' => $empty,
          'title' => $ttitle,
          'content' => $text
      );
    }
  }

  // Return parsed sections
  $return = '';
  foreach ($sections as $title => $sect) {
    if (($section == '*' || $title == $section)) {
      if (!$sect['empty'] || !$only_nonempty_sections) {
        $return .= $sect['content'] . "\n\n";
      }
    }
  }
  return $return;
}

/**
 * Returns the introduction text for the configutation page
 *
 * @return string
 */
function udc_admin_get_form_introduction() {
  ob_start();
  ?>

  Here you can specify the settings of the server script, which is the Drupal side of the connector. The full API (client classes)
  help is provided on the <a href="<? print base_path(); ?>admin/help/udc">help page</a>. You can use the API in the external PHP
  scripts to request user information from the server.

  <?php
  return udc_admin_get_translated_html(ob_get_clean());
}

/**
 * Returns the additional information text for the configutation page
 *
 * @return string
 */
function udc_admin_get_form_sample_code($token, $need_http_host, $is_ssl) {
ob_start();?>
require_once('<? print DRUPAL_ROOT; ?>/sites/all/modules/udc/client/DrupalUserAuth.class.inc');
<? if($need_http_host) print "define('USER_DATA_CONNECTOR_USE_HTTP_HOST_INSTEAD_OF_LOCALHOST', TRUE);\n" ?>
<? if($is_ssl) print "define('USER_DATA_CONNECTOR_USE_SSL', TRUE);\n" ?>
DrupalUserBase::setToken('<? print $token; ?>');

// This would be the user/pass from your form, HTTP Basic Auth, XML-RPC, etc.
$login_name = "[...]";
$password = "[...]";

try {
  // Create the user auth object
  $udc = new DrupalUserAuth();

  // Send request
  $udc->request(
    $login_name,  // login name
    $password,    // password
    NULL,         // email address
    FALSE,        // allow active users only (non blocked)
    TRUE          // include all user fields in the response
  );

  // Check the results
  if (!$udc->valid) {
    // Login incorrect
    print "Sorry, your login data are incorrect, please try again.";
  } else if(!$udc->active) {
    // User inactive/blocked
    print "Sorry, your login data are ok, but your account is deactivated";
  } else {
    // Let's print the whole object for now ...
    print "<pre>" . htmlspecialchars(print_r($udc, TRUE)) . "</pre>";
  }
} catch (Exception $e) {
  // UDC will throw exceptions if:
  //  - User name or password is "" (not worth to make a query)
  //  - SSL is used but cURL not installed
  //  - The token is wrong
  //  - Any connections errors
  //  - Internal errors
  print "Exception: \n$e\n";
  if (isset($udc)) {
    print "Debug message: " . $udc->getDebug() . "\n";
  }
}

<?php
  $text = "<?php\n" . ob_get_clean();
  if (function_exists('highlight_string')) {
    $text = highlight_string($text, TRUE);
  }
  return t('This is an example to check if a user name and password is correct. The code adapts to your current settings.') . '<br/><br/>' . $text;
}

/**
 * Returns the help text for the help page
 *
 * @return string
 */
function udc_admin_get_help() {
  $help = '<div>';

  $help .= udc_admin_get_readme_section('Summary');
  $help .= udc_admin_get_readme_section('Requirements');
  $help .= udc_admin_get_readme_section('FAQ');
  $help .= udc_admin_get_readme_section('Troubleshooting');
  $help .= udc_admin_get_readme_section('Maintainers');

  if (is_file(dirname(dirname(__FILE__)) . '/CHANGELOG')) {
    $help .= '<h1>Changelog</h1>
              <pre>' . check_plain(t(@file_get_contents(dirname(dirname(__FILE__)) . '/CHANGELOG'))) . '</pre>'
    ;
  }

  $help .= udc_admin_get_translated_html('
    <h1>Examples</h1>
    Here the list of API examples.
    You can find exactly these files in the directory <i>sites/all/modules/udc/examples</i>.
    <h3>Example common Include file</h3>
    This file is included in all examples. If provides some timing functions and prevents
    the examples form being executed if the http server is not "localhost". You have to
    change the variable $TOKEN to the token that you configured in the module configuration
    form.
  ');

  $help .= udc_admin_get_highlighted_source(dirname(dirname(__FILE__)) . '/examples/enable_examples.inc');

  $help .= udc_admin_get_translated_html('
    <h3>Example 1: Ping the server to check the connection and token</h3>
    You can use <code>ping</code> with all <code>DrupalUser*****</code> classes,
    as the feature is implemented in <code>DrupalUserBase</code> and the other
    classes are derived from this class.
  ');

  $help .= udc_admin_get_highlighted_source(dirname(dirname(__FILE__)) . '/examples/ping.php');

  $help .= udc_admin_get_translated_html('
    <h3>Example 2: Request user authentication</h3>
    This is an example for a HTTPS basic authentication using the <code>DrupalUserAuth</code>
    class. If your (external) script is invoked quite often, you should save the response user
    data in the session instead of sending a local authentication request everytime. The reason
    is not only the additional localhost HTTP connection, but the simple fact that calculating
    the SHA256 checksum takes Drupal comparatively much processing time.
  ');

  $help .= udc_admin_get_highlighted_source(dirname(dirname(__FILE__)) . '/examples/userauth.php');

  $help .= udc_admin_get_translated_html('
    <h3>Example 3: Request single user information</h3>
    If you want to get information about a user, but don\'t want to check the password,
    use the <code>DrupalUserInfo</code> class. The server script response will contain the
    same information as the <code>DrupalUserAuth</code> class response - except that the
    password check is skipped and the <code>valid</code> property is <code>TRUE</code> if
    the user exists.
  ');

  $help .= udc_admin_get_highlighted_source(dirname(dirname(__FILE__)) . '/examples/userinfo.php');

  $help .= udc_admin_get_translated_html('
    <h3>Example 4: Request a user list</h3>
    There is a variety of situations where is necessary to get a simple ist of users from the
    Drupal database. The method of getting the data using User Data Connector is the <code>DrupalUserList</code>
    class:
  ');

  $help .= udc_admin_get_highlighted_source(dirname(dirname(__FILE__)) . '/examples/userlist.php');

  $help .= '</div>';
  return $help;
}

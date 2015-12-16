<?php

/**
 * @file
 * Admin page callbacks for the boost mobile module.
 */

/**
 * Default apache server name.
 */
define('BOOST_SERVER_NAME_HTTP_HOST', '%{HTTP_HOST}');

/**
 * Default apache document root.
*/
define('BOOST_DOCUMENT_ROOT', '%{DOCUMENT_ROOT}');

/**
 * Default setting for SSL pages.
*/
define('BOOST_SSL_BYPASS', TRUE);

/**
 * Default setting for cookie lifetime.
 */
define('BOOST_MOBILE_COOKIE_LIFETIME', 1440);


/**
 * Form builder; Configure boost mobile settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function boost_mobile_settings() {

  // List existing themes.
  $themes = list_themes();
  $default_theme = variable_get('default_theme', 'bartik');
  $options = array();
  $options[$default_theme] = $themes[$default_theme]->info['name'];
  $form = array();

  // Retrieving active themes/
  foreach ($themes as $value_theme) {
    if ($value_theme->status == 1 && $value_theme->name != $default_theme) {
      $options[$value_theme->name] = $value_theme->info['name'];
    }
  }

  // Define the mobile cache directory.
  $form['boost_mobile_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile cache directory'),
    '#default_value' => variable_get('boost_mobile_dir', BOOST_MOBILE_DIR),
  );

  // Choose an active theme.
  $form['boost_mobile_theme'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Choose your mobile theme'),
    '#default_value' => variable_get('boost_mobile_theme', $default_theme),
  );

  // Define the cookie lifetime.
  $form['boost_mobile_cookie_lifetime'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie lifetime in minutes'),
    '#description' => t('When a user chooses to display the site in a specific version using the switcher block,
       we write a cookie to remember the last choice.'),
    '#default_value' => variable_get('boost_mobile_cookie_lifetime', BOOST_MOBILE_COOKIE_LIFETIME),
  );

  // Reset htaccess on submit;
  $form['#submit'][] = 'boost_form_submit_handler';
  $form['#submit'][] = 'boost_mobile_form_submit_handler';
  return system_settings_form($form);
}

/**
 * The boost_mobile_settings form validation handler.
 */
function boost_mobile_settings_validate($form, &$form_state) {
  // Cookie lifetime must be an integer.
  if (!ctype_digit($form_state['values']['boost_mobile_cookie_lifetime'])) {
    form_set_error('boost_mobile_cookie_lifetime', t('The value of the cookie lifetime is in minutes and must be an integer.'));
  }
}

/**
 * Always run these functions after a form submit from boost mobile.
 */
function boost_mobile_form_submit_handler() {
  register_shutdown_function('boost_mobile_htaccess_cache_dir_put');
}

/**
 * Form builder; Configure boost mobile settings.
 *
 * @ingroup forms
 */
function boost_mobile_admin_htaccess_generation() {
  // Generated .htaccess output.
  $htaccess = boost_mobile_admin_htaccess_generate_htaccess();
  // You must not use t() in the the text for l() in
  // "l('admin/config/development/performance/boost/htaccess-settings'...".
  $form['boost_generated'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Generated Rules'),
    '#default_value' => $htaccess,
    '#rows'          => count(explode("\n", $htaccess)) + 1,
    '#wysiwyg'       => FALSE,
    '#description'   => t("Copy this into your .htaccess file below <pre><tt>  # If your site is running in a VirtualDocumentRoot at http://example.com/,
  # uncomment the following line:
  # RewriteBase / </tt></pre> and above <pre><tt>  # Pass all requests not referring directly to files in the filesystem to
  # index.php. Clean URLs are handled in drupal_environment_initialize().</tt></pre><br />Note that the generated rules' settings can be configure at !link.", array('!link' => l('admin/config/development/performance/boost/htaccess-settings', 'admin/config/development/performance/boost/htaccess-settings'))),
  );

  return $form;
}


/**
 * Generate htaccess code.
 *
 * See "http://www.askapache.com/htaccess/mod_rewrite-variables-cheatsheet.html".
 *
 * @return string
 *   htaccess code
 */
function boost_mobile_admin_htaccess_generate_htaccess() {
  global $base_path;
  $server_name = variable_get('boost_server_name_http_host', BOOST_SERVER_NAME_HTTP_HOST);
  $document_root = variable_get('boost_document_root', BOOST_DOCUMENT_ROOT);

  // Various dir's
  $cache_dir = variable_get('boost_root_cache_dir', BOOST_ROOT_CACHE_DIR);
  $normal_dir = variable_get('boost_normal_dir', BOOST_NORMAL_DIR);
  $mobile_dir = variable_get('boost_mobile_dir', BOOST_MOBILE_DIR);
  $bm_cookie_lifetime = variable_get('boost_mobile_cookie_lifetime', BOOST_MOBILE_COOKIE_LIFETIME);
  $char = variable_get('boost_char', BOOST_CHAR);

  // Go through every storage type getting the extension
  // and if it supports gzip.
  $enabled_file_extensions = array();
  $types = boost_get_storage_types();
  foreach ($types as $content_types) {
    foreach ($content_types as $type => $values) {
      if ($values['enabled']) {
        $enabled_file_extensions[$values['extension']]['gzip'] = $values['gzip'];
        if (empty($enabled_file_extensions[$values['extension']]['content_type'])) {
          $enabled_file_extensions[$values['extension']]['content_type'] = $type;
        }
      }
    }
  }

  $output = array('gzip' => '', 'normal' => '');
  $gzip_count = 0;
  $normal_count = 0;
  foreach ($enabled_file_extensions as $extension => $values) {
    $type = $values['content_type'];
    if ($values['gzip']) {
      $output['gzip'] .= "  RewriteCond $document_root$base_path$cache_dir/%{ENV:boostpath}/$server_name%{REQUEST_URI}$char%{QUERY_STRING}\.$extension -s\n";
      $output['gzip'] .= "  RewriteRule .* $cache_dir/%{ENV:boostpath}/$server_name%{REQUEST_URI}$char%{QUERY_STRING}\.$extension [L,T=$type,E=no-gzip:1]\n";
      $gzip_count++;
    }
    $output['normal'] .= "  RewriteCond $document_root$base_path$cache_dir/%{ENV:boostpath}/$server_name%{REQUEST_URI}$char%{QUERY_STRING}\.$extension -s\n";
    $output['normal'] .= "  RewriteRule .* $cache_dir/%{ENV:boostpath}/$server_name%{REQUEST_URI}$char%{QUERY_STRING}\.$extension [L,T=$type]\n";
    $normal_count++;
  }
  $skip = !empty($gzip_count) ? $normal_count + $gzip_count + 1 : $normal_count;

  // Generate the rules.
  $string  = "  ### BOOST START ###\n";

  if (!empty($output)) {
    $string .= "\n";
    $string .= "  # Allow for alt paths to be set via htaccess rules; allows for cached variants (future mobile support)\n";
    $string .= "  # RewriteCond on user Agent to detect mobile devices\n";
    $string .= "  # These RewriteCond are based on open source mobile detection site: http://detectmobilebrowsers.com/\n";
    $string .= "  RewriteRule .* - [E=boostpath:$normal_dir]\n";
    $string .= "  RewriteCond %{HTTP_USER_AGENT} (android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge\ |maemo|midp|mmp|mobile.+firefox|netfront|opera\ m(ob|in)i|palm(\ os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows\ (ce|phone)|xda|xiino [NC,OR]\n";
    $string .= "  RewriteCond %{HTTP_USER_AGENT} ^(1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a\ wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r\ |s\ )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1\ u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp(\ i|ip)|hs\-c|ht(c(\-|\ |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac(\ |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt(\ |\/)|klon|kpt\ |kwc\-|kyo(c|k)|le(no|xi)|lg(\ g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-|\ |o|v)|zz)|mt(50|p1|v\ )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v\ )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-|\ )|webc|whit|wi(g\ |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-) [NC]\n";
    $string .= "  RewriteRule .* - [E=boostpath:$mobile_dir]\n";

    $string .= "\n";
    $string .= "  # Check cookie\n";
    $string .= "  RewriteCond %{HTTP_COOKIE} bm_version=desktop\n";
    $string .= "  RewriteRule .* - [E=boostpath:$normal_dir]\n";
    $string .= "  RewriteCond %{HTTP_COOKIE} bm_version=mobile\n";
    $string .= "  RewriteRule .* - [E=boostpath:$mobile_dir]\n";

    $string .= "\n";
    $string .= "  # Get bm_version\n";
    $string .= "  RewriteCond %{QUERY_STRING} bm_version=desktop\n";
    $string .= "  RewriteRule .* - [E=boostpath:$normal_dir,CO=bm_version:desktop:$server_name:$bm_cookie_lifetime:/]\n";
    $string .= "  RewriteCond %{QUERY_STRING} bm_version=mobile\n";
    $string .= "  RewriteRule .* - [E=boostpath:$mobile_dir,CO=bm_version:mobile:$server_name:$bm_cookie_lifetime:/]\n";

    $string .= "\n";
    $string .= "  # Caching for anonymous users\n";
    $string .= "  # Skip boost IF not get request OR uri has wrong dir OR cookie is set OR request came from this server" . (variable_get('boost_ssl_bypass', BOOST_SSL_BYPASS) ? " OR https request" : "") . "\n";
    $string .= "  RewriteCond %{REQUEST_METHOD} !^(GET|HEAD)$ [OR]\n";
    $string .= "  RewriteCond %{REQUEST_URI} (^$base_path(admin|$cache_dir|misc|modules|sites|system|openid|themes|node/add|comment/reply))|(/(edit|user|user/(login|password|register))$) [OR]\n";
    if (variable_get('boost_ssl_bypass', BOOST_SSL_BYPASS)) {
      $string .= "  RewriteCond %{HTTPS} on [OR]\n";
    }
    $string .= "  RewriteCond %{HTTP_COOKIE} " . variable_get('boost_cookie', BOOST_COOKIE) . " [OR]\n";
    $string .= "  RewriteCond %{ENV:REDIRECT_STATUS} 200\n";
    $string .= "  RewriteRule .* - [S=$skip]\n";
    $string .= "\n";
    $string .= "  # GZIP\n";
    $string .= "  RewriteCond %{HTTP:Accept-encoding} !gzip\n";
    $string .= "  RewriteRule .* - [S=$gzip_count]\n";
    $string .= $output['gzip'];
    $string .= "\n";
    $string .= "  # NORMAL\n";
    $string .= $output['normal'];
  }
  $string .= "\n";
  $string .= "  ### BOOST END ###\n";

  return $string;
}

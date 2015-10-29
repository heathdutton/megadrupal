<?php
/**
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function bigdaddy_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * Override or insert variables in the html_tag theme function.
 */
function bigdaddy_process_html_tag(&$variables) {
  $tag = &$variables['element'];

  if ($tag['#tag'] == 'style' || $tag['#tag'] == 'script') {
    // Remove redundant type attribute and CDATA comments.
    unset($tag['#attributes']['type'], $tag['#value_prefix'], $tag['#value_suffix']);

    // Remove media="all" but leave others unaffected.
    if (isset($tag['#attributes']['media']) && $tag['#attributes']['media'] === 'all') {
      unset($tag['#attributes']['media']);
    }
  }
}

/**
 * Override or insert variables into the HTML templates.
 *
 * @param $vars
 *  A sequential array of variables to pass to the theme template.
 */
function bigdaddy_preprocess_html(&$vars) {

  /* HTML classes
  ---------------------------------------------------------------------- */
  // To add dynamically your own classes use $vars['classes_array'][] = 'my_class';

  // Optionaly add the theme setting name in the <body> when this one is activate
  if (theme_get_setting('bigdaddy_wireframes')) {
    $vars['classes_array'][] = 'wireframes';
  }
  if (theme_get_setting('bigdaddy_grid_system')) {
    $vars['classes_array'][] = 'grid-system';
  }
  if (theme_get_setting('bigdaddy_display_viewport')) {
    $vars['classes_array'][] = 'display-viewport';
  }

}

/**
 * Override or insert variables into the PAGE templates.
 *
 * @param $vars
 *  A sequential array of variables to pass to the theme template.
 */
function bigdaddy_preprocess_page(&$vars) {

  /* PAGE classes
  ---------------------------------------------------------------------- */
  // To add dynamically your own classes use $vars['classes_array'][] = 'my_class';

  /* USER ACCOUNT
  ---------------------------------------------------------------------- */
  // Removes the tabs from user login, register, & password. Also fixes page titles

  switch (current_path()) {
    case 'user':
      $vars['title'] = t('Login'); // Don't work in the preprocess fonction
      unset($vars['tabs']); // Undefined variable
      break;
    case 'user/register':
      $vars['title'] = t('New account');
      unset($vars['tabs']);
      break;
    case 'user/password':
      $vars['title'] = t('DOH! I forget my password');
      unset($vars['tabs']);
      break;
    default:
      // No actions
      break;
  }

}

/**
 * Override or insert variables into the NODE templates.
 *
 * @param $vars
 *  A sequential array of variables to pass to the theme template.
 */
function bigdaddy_preprocess_node(&$vars) {

  /* NODE classes
  ---------------------------------------------------------------------- */
  // To add dynamically your own classes use $vars['classes_array'][] = 'my_class';

}

/**
 * Override or insert variables into the BLOCK templates.
 *
 * @param $vars
 *  A sequential array of variables to pass to the theme template.
 */
function bigdaddy_preprocess_block(&$vars) {

  /* BLOCK classes
  ---------------------------------------------------------------------- */
  // To add dynamically your own classes use $vars['classes_array'][] = 'my_class';

}

/**
* Alter the default Drupal's system styles.
*/
function bigdaddy_css_alter(&$css) {

  unset($css[drupal_get_path('module', 'comment') . '/comment.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.messages.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.menus.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.theme.css']);
  unset($css[drupal_get_path('module', 'user') . '/user.css']);

}

/**
* Alter the default Drupal's system javascripts.
*/
function bigdaddy_js_alter(&$js) {

  if(theme_get_setting('bigdaddy_js_jquery')){
    if (isset($js['misc/jquery.js'])) {
      $js['misc/jquery.js']['data'] ='http://ajax.googleapis.com/ajax/libs/jquery/1.10.1/jquery.min.js';
      $js['misc/jquery.js']['type'] = 'external';
      $js['misc/jquery.js']['weight'] = -100;
    }
  }

}

/**
 * Alter the primary links.
 */
function bigdaddy_menu_tree__main_menu($variables) {

  return '<nav role="navigation"><ul class="menu">' . $variables['tree'] . '</ul></nav>';

}

/**
 * Changes the search form to use the HTML5 "search" input attribute
 */
function bigdaddy_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}

/**
* Alter the user login form.
*/
function bigdaddy_form_user_login_alter(&$form, &$form_state) {

  $form['#prefix'] = '<h1>'.t('Login/Register').'</h1>';

  // Add a link above the form to the user account creation page
  $form['name']['#prefix'] = l(t('Register a new account'), 'user/register', array('attributes' => array('class' => 'login-register')));

  // Add a link under the form to the forgotten password page
  $form['pass']['#suffix'] = l(t('Forgot password?'), 'user/password', array('attributes' => array('class' => 'login-password')));

  // Add the HTML5 PLaceholder attribute into the login and password input and unset the label + descrption
  unset($form['name']['#title']);
  unset($form['name']['#description']);
  unset($form['pass']['#title']);
  unset($form['pass']['#description']);
  $form['name']['#attributes']['placeholder'] = t('Your login');
  $form['pass']['#attributes']['placeholder'] = t('Your password');

}

/**
* Alter any form.
*/
function bigdaddy_form_your_form_id_alter(&$form, &$form_state) {

  //print dpm($form);

}



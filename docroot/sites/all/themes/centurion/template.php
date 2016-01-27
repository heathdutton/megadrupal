<?php

/**
 * @file
 * template.php file for Centrion theme
 *
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */

/**
 * Preprocessor for page.tpl.php template file.
 */
function centurion_preprocess_page(&$vars, $hook) {

  // For easy printing of variables.
  $vars['logo_img'] = '';
  if (!empty($vars['logo'])) {
    $vars['logo_img'] = theme('image', array(
      'path'  => $vars['logo'],
      'alt'   => t('Home'),
      'title' => t('Home'),
    ));
  }
  $vars['linked_logo_img']  = '';
  if (!empty($vars['logo_img'])) {
    $vars['linked_logo_img'] = l($vars['logo_img'], '<front>', array(
      'attributes' => array(
        'rel'   => 'home',
        'title' => t('Home'),
      ),
      'html' => TRUE,
    ));
  }
  $vars['linked_site_name'] = '';
  if (!empty($vars['site_name'])) {
    $vars['linked_site_name'] = l($vars['site_name'], '<front>', array(
      'attributes' => array(
        'rel'   => 'home',
        'title' => t('Home'),
      ),
    ));
  }

  // Site navigation links.
  $vars['main_menu_links'] = '';
  if (isset($vars['main_menu'])) {
    $vars['main_menu_links'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'id' => 'main-menu',
        'class' => array('inline', 'main-menu'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }
  $vars['secondary_menu_links'] = '';
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu_links'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'id'    => 'secondary-menu',
        'class' => array('inline', 'secondary-menu'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

}

/**
 * Borrowed from NineSixty Theme built by dvessel
 * http://drupal.org/user/56782
 *
 * The first parameter passed is the *default class*. All other parameters must
 * be set in pairs like so: "$variable, 3". The variable can be anything
 * available within a template file and the integer is the width set for the
 * adjacent box containing that variable.
 *
 * class="<?php print ns('grid-12', $var_a, 6); ?>"
 *
 * If $var_a contains data, the next parameter (integer) will be subtracted from
 * the default class.
 */
function centurion_chclass() {
  $args = func_get_args();
  $default = array_shift($args);
  // Get the type of class, i.e., 'grid'.
  // Also get the default unit for the type to be procesed and returned.
  list($type, $return_unit) = explode('-', $default);

  // Process the conditions.
  $flip_states = array('var' => 'int', 'int' => 'var');
  $state = 'var';
  foreach ($args as $arg) {
    if ($state == 'var') {
      $var_state = !empty($arg);
    }
    elseif ($var_state) {
      $return_unit = $return_unit - $arg;
    }
    $state = $flip_states[$state];
  }

  $output = '';
  // Anything below a value of 1 is not needed.
  if ($return_unit > 0) {
    $output = $type . '-' . $return_unit;
  }
  return $output;
}

/**
 * Implements hook_form_alter().
 */
function centurion_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    // Toggle label visibilty.
    $form['search_block_form']['#title_display'] = 'invisible';
    // Define size of the textfield.
    $form['search_block_form']['#size'] = 15;
    // Set a default value for the textfield.
    $form['search_block_form']['#default_value'] = t('Find something...');
    // Change the text on the submit button.
    $form['actions']['submit']['#value'] = t('Search');
    // Add extra attributes to the text box.
    $form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Find something...';}";
    $form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Find something...') {this.value = '';}";
  }
}

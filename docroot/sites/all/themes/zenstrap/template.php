<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * A QUICK OVERVIEW OF DRUPAL THEMING
 *
 *   The default HTML for all of Drupal's markup is specified by its modules.
 *   For example, the comment.module provides the default HTML markup and CSS
 *   styling that is wrapped around each comment. Fortunately, each piece of
 *   markup can optionally be overridden by the theme.
 *
 *   Drupal deals with each chunk of content using a "theme hook". The raw
 *   content is placed in PHP variables and passed through the theme hook, which
 *   can either be a template file (which you should already be familiary with)
 *   or a theme function. For example, the "comment" theme hook is implemented
 *   with a comment.tpl.php template file, but the "breadcrumb" theme hooks is
 *   implemented with a theme_breadcrumb() theme function. Regardless if the
 *   theme hook uses a template file or theme function, the template or function
 *   does the same kind of work; it takes the PHP variables passed to it and
 *   wraps the raw content with the desired HTML markup.
 *
 *   Most theme hooks are implemented with template files. Theme hooks that use
 *   theme functions do so for performance reasons - theme_field() is faster
 *   than a field.tpl.php - or for legacy reasons - theme_breadcrumb() has "been
 *   that way forever."
 *
 *   The variables used by theme functions or template files come from a handful
 *   of sources:
 *   - the contents of other theme hooks that have already been rendered into
 *     HTML. For example, the HTML from theme_breadcrumb() is put into the
 *     $breadcrumb variable of the page.tpl.php template file.
 *   - raw data provided directly by a module (often pulled from a database)
 *   - a "render element" provided directly by a module. A render element is a
 *     nested PHP array which contains both content and meta data with hints on
 *     how the content should be rendered. If a variable in a template file is a
 *     render element, it needs to be rendered with the render() function and
 *     then printed using:
 *       <?php print render($variable); ?>
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. With this file you can do three things:
 *   - Modify any theme hooks variables or add your own variables, using
 *     preprocess or process functions.
 *   - Override any theme function. That is, replace a module's default theme
 *     function with one you write.
 *   - Call hook_*_alter() functions which allow you to alter various parts of
 *     Drupal's internals, including the render elements in forms. The most
 *     useful of which include hook_form_alter(), hook_form_FORM_ID_alter(),
 *     and hook_page_alter(). See api.drupal.org for more information about
 *     _alter functions.
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   If a theme hook uses a theme function, Drupal will use the default theme
 *   function unless your theme overrides it. To override a theme function, you
 *   have to first find the theme function that generates the output. (The
 *   api.drupal.org website is a good place to find which file contains which
 *   function.) Then you can copy the original function in its entirety and
 *   paste it in this template.php file, changing the prefix from theme_ to
 *   zenstrap_. For example:
 *
 *     original, found in modules/field/field.module: theme_field()
 *     theme override, found in template.php: zenstrap_field()
 *
 *   where zenstrap is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_field() function.
 *
 *   Note that base themes can also override theme functions. And those
 *   overrides will be used by sub-themes unless the sub-theme chooses to
 *   override again.
 *
 *   Zen core only overrides one theme function. If you wish to override it, you
 *   should first look at how Zen core implements this function:
 *     theme_breadcrumbs()      in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called theme hook suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node--forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and theme hook suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440 and http://drupal.org/node/1089656
 */


/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function zenstrap_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  zenstrap_preprocess_html($variables, $hook);
  zenstrap_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
/* -- Delete this line if you want to use this function
function zenstrap_preprocess_html(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // The body tag's classes are controlled by the $classes_array variable. To
  // remove a class from $classes_array, use array_diff().
  //$variables['classes_array'] = array_diff($variables['classes_array'], array('class-to-remove'));
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function zenstrap_preprocess_page(&$variables, $hook) {
  global $user;
  //we don't support default logo.
  if (theme_get_setting('default_logo')) {
    $variables['logo'] ='';
  }
  //modal login block
  if (theme_get_setting('zenstrap_login_modal') && empty($user->uid)) {
    drupal_add_js(array('zenstrap_login_modal' => TRUE), 'setting');
    //add the login as a block to the page. otherwise modal will not work
    $login_form = drupal_get_form('user_login_block');
    $login = '<div id="login-modal" class = "modal hide">';
    $login .= '<div class="modal-header"> <button type="button" class="close" data-dismiss="modal">×</button>';
    $login .= '<h3>' . t('Login') . ' </h3>';
    $login .= '</div>';
    $login .= '<div class="modal-body">';
    $login .= drupal_render($login_form['name']);
    $login .= drupal_render($login_form['pass']);
    $login .= '</div>';
    $login .= '<div class="modal-footer">';
    $login .= drupal_render($login_form['actions']); 
    $login .= drupal_render($login_form['links']);
    unset($login_form['actions']['#attributes']['class']);
    $login .= '</div>';
    $login .= '</div>';
    $login .= drupal_render_children($login_form);
    $login_form['#children'] = $login;
    $variables['login_block'] = theme('form', array('element' => $login_form));
  }
  else {
    drupal_add_js(array('zenstrap_login_modal' => FALSE), 'setting');
    $variables['login_block'] = FALSE;
  }
  
  if (theme_get_setting('zenstrap_searchbox') && user_access('search content')) {
    $search_block = drupal_get_form('search_block_form');
    $search_block['#attributes']['class'][] = 'form-inline';
    $search_block['#attributes']['class'][] = 'navbar-form';
    $search_block['#attributes']['class'][] = 'pull-right';
    $variables['search_block'] = drupal_render($search_block);
  }
  else {
    $variables['search_block'] = '';
  }
  
  if (($modal_form_paths = theme_get_setting('zenstrap_modal_forms'))) {
    $modal_forms = explode("\r\n", $modal_form_paths);
    drupal_add_js(array('zenstrap_forms_modal' => $modal_forms), 'setting');
  }
  
  if (!(isset($_GET['modal']) && $_GET['modal'])) {
    //we don't make bootstrap.js part of info file as it will be delivered each time
    //and this will break the modal forms
    drupal_add_js(drupal_get_path('theme', 'zenstrap') . '/bootstrap/js/bootstrap.js');
  }
}

/**
 * Overriden modal form theme function.
 */
function zenstrap_modal_form($element) {
    $form = $element['form'];
    $output = '<div class="modal-body">';
    $add_footer = TRUE;
    foreach (element_children($form) as $key) {
      if (isset($form[$key]['#type'])) {
        switch ($form[$key]['#type']) {
          case 'submit':
          case 'button':
            if ($add_footer) {
              $add_footer = FALSE;
              //close the body div
              $output .= '</div>';
              $output .= '<div class="modal-footer">';
            }
            //deliberate fall through
          default:
            $form[$key]['#printed'] = FALSE;
            $output .= drupal_render($form[$key]);
            break;
        }
      }
    }
    $output .= '</div>';
    $output .= drupal_render_children($form);
    return $output;  
}

/**
 * Implements hook_form_alter().
 */
function zenstrap_form_alter(&$form, &$form_state, $form_id) {
  if (isset($_GET['modal']) && $_GET['modal']) {
    $form['#theme'] = 'modal_form';
    //remove the modal=TRUE from form action. otherwise if errors occur in validate, then we have trouble!!!
    if (strpos($form['#action'], 'modal=TRUE&') !== FALSE) {
      //if there are other parameters after & don't break them.
      $form['#action'] = str_replace('modal=TRUE&', '', $form['#action']);
    }
    else {
      $form['#action'] = str_replace('modal=TRUE', '', $form['#action']);
    }
  }
}

/**
 * Implements hook_theme().
 */
function zenstrap_theme() {
  return array(
    'modal_form' => array('render element' => 'form'),
    'accordion' => array(
      'variables' => array('key' => NULL, 'title' => NULL, 'content' => NULL),
      'template' => 'accordion',
      'path' => drupal_get_path('theme', 'zenstrap') . '/templates',
    )
  );
}

/**
 * Implements template_preprocess_block().
 */
function zenstrap_preprocess_block(&$variables) {
  $block = $variables['block'];
  //block groups as accordion
  static $acc_menu;
  if (!isset($acc_menu)) {
    $acc_menu = theme_get_setting('zenstrap_menu_accordion');
    if (!empty($acc_menu)) {
      $acc_menu = array_filter($acc_menu);
    }
  }
  $block_is_menu = FALSE;
  if (!empty($acc_menu)) {
    //check if this is a menu block
    $block_is_menu = in_array($block->delta, $acc_menu);
  }
  $variables['block_is_menu'] = $block_is_menu;
  //single menu block as accordion
  $single_block = theme_get_setting('zenstrap_single_accordion');
  $single_accordion = FALSE;
  if (!empty($single_block) && $single_block == $block->delta) {
    $content = '';
    $elements = $variables['elements'];
    foreach (element_children($elements) as $key) {
      $content .=  theme('accordion', array(
        'parent' => 'p-' . $key,
        'key' => 'k-' . $key,
        'title' => $elements[$key]['#title'],
        'content' => $elements[$key]['#children'],
       ));
    }
    $variables['content'] = '<div class="accordion" id="single-accordion">' . $content . '</div>';
    $single_accordion = TRUE;
  }
  $variables['single_accordion'] = $single_accordion;
}
 
/**
 * load all the overridden include file
 */
$path = drupal_get_path('theme', 'zenstrap');
$files = file_scan_directory($path . '/includes', '/.inc/');
foreach ($files as $absolute => $file) {
  require $absolute;
}

function zenstrap_preprocess_views_view_table(&$vars) {
  drupal_add_js(drupal_get_path('theme', 'zenstrap') . '/js/zenstrap_tableheader.js');
}

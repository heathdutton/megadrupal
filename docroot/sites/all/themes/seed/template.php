<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the SeeD theme.
 */

/**
 * Implements hook_css_alter().
 *
 * @TODO: Once http://drupal.org/node/901062 is resolved, determine whether
 * this can be implemented in the .info file instead.
 *
 */
function seed_css_alter(&$css) {
  $exclude = array();

  // Exclude Drupal core stylesheets
  $drupal_stylesheets = theme_get_setting('seed_exclude_css');

  $counter = 1;
  foreach ($drupal_stylesheets as $name) {
    if ($name != '--') {
      $saved_value = (bool)theme_get_setting('drupal_exclude_css_' . (string)$counter);

      if( $saved_value ) {
        $exclude[$name] = $saved_value;
      }

      $counter++;
    }
  }

  // Exclude custom stylesheets
  $custom_stylsheets = explode(PHP_EOL, theme_get_setting('custom_exclusion_info'));

  $counter = 1;
  foreach ($custom_stylsheets as $name) {
    $saved_value = (bool)theme_get_setting( 'custom_exclusion_info' );

    $exclude[trim($name)] = $saved_value;

    $counter++;
  }

  $css = array_diff_key($css, $exclude);
}

/**
 * Implements hook_html_head_alter().
 */
function seed_html_head_alter(&$head_elements) {
  // Changes the default meta content-type tag to the shorter HTML5 version
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}

/**
 * Changes the search form to use the HTML5 "search" input attribute
 */
function seed_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}

/**
 * Implements hook_process_HOOK().
 *
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 */
function seed_preprocess_html(&$vars) {
  // Fixes page titles for login, register & password pages
  switch (current_path()) {
    case 'user':
      $vars['head_title_array']['title'] = t('Login');
      $head_title = $vars['head_title_array'];
      $vars['head_title'] = implode(' | ', $head_title);
      break;

    case 'user/register':
      $vars['head_title_array']['title'] = t('Create new account');
      $head_title = $vars['head_title_array'];
      $vars['head_title'] = implode(' | ', $head_title);
      break;

    case 'user/password':
      $vars['head_title_array']['title'] = t('Request new password');
      $head_title = $vars['head_title_array'];
      $vars['head_title'] = implode(' | ', $head_title);
      break;

    case 'node':
    case 'front':

    default:
      break;
  }

  // Ensure that the $vars['rdf'] variable is an object.
  if (!isset($vars['rdf']) || !is_object($vars['rdf'])) {
    $vars['rdf'] = new StdClass();
  }

  if (module_exists('rdf')) {
    $vars['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $vars['rdf']->version = 'version="HTML+RDFa 1.1"';
    $vars['rdf']->namespaces = $vars['rdf_namespaces'];
    $vars['rdf']->profile = ' profile="' . $vars['grddl_profile'] . '"';
  }
  else {
    $vars['doctype'] = '<!DOCTYPE html>' . "\n";
    $vars['rdf']->version = '';
    $vars['rdf']->namespaces = '';
    $vars['rdf']->profile = '';
  }

  // Use html5shiv.
  if (theme_get_setting('html5shim')) {
    $element = array(
      'element' => array(
        '#tag' => 'script',
        '#value' => '',
        '#attributes' => array(
          'type' => 'text/javascript',
          'src' => file_create_url(drupal_get_path('theme', 'seed') . '/js/html5shiv-printshiv.js'),
        ),
      ),
    );
    $html5shim = array(
      '#type' => 'markup',
      '#markup' => "<!--[if lt IE 9]>\n" . theme('html_tag', $element) . "<![endif]-->\n",
    );
    drupal_add_html_head($html5shim, 'seed_html5shim');
  }

  // Use css3-mediaqueries-js.
  if (theme_get_setting('css3_mediaqueries')) {
    drupal_add_js('//css3-mediaqueries-js.googlecode.com/svn/trunk/css3-mediaqueries.js', array('type' => 'external', 'group' => JS_LIBRARY, 'weight' => -100));
  }

  // Use normalize.css
  if (theme_get_setting('normalize_css')) {
    drupal_add_css(drupal_get_path('theme', 'seed') . '/css/normalize.css', array('group' => CSS_SYSTEM, 'weight' => -100));
  }

  if (!module_exists('seed_tools')) {
    // We depend on seed_tools for many things.
    $vars['page']['page_top']['seed_tools_error'] = array(
      '#markup' => '<div id="seed-tools-error">' . t('Module SeeD Tools (!seed_tools) is required for this theme to work properly', array('!seed_tools' => l('seed_tools', 'http://drupal.org/project/seed_tools'))) . "</div>",
    );
  }

  // If the current page is a node, add a class page-node-type-NODETYPE
  $node = menu_get_object();
  if ($node) {
    $vars['classes_array'][] = 'page-node-type-' . $node->type;
  }
}

/**
 * Implements hook_THEME().
 *
 * Return a themed breadcrumb trail.
 *
 * @param (array) $vars
 *   An array containing the breadcrumb links.
 *
 * @return string
 *   A string containing the breadcrumb output.
 */
function seed_breadcrumb($vars) {
  $breadcrumb = $vars['breadcrumb'];
  // Determine if we are to display the breadcrumb.
  $show_breadcrumb = theme_get_setting('breadcrumb_display');
  if ($show_breadcrumb == 'yes') {

    // Optionally get rid of the homepage link.
    $show_breadcrumb_home = theme_get_setting('breadcrumb_home');
    if (!$show_breadcrumb_home) {
      array_shift($breadcrumb);
    }

    // Return the breadcrumb with separators.
    if (!empty($breadcrumb)) {
      $separator = filter_xss(theme_get_setting('breadcrumb_separator'));
      $trailing_separator = $title = '';

      // Add the title and trailing separator.
      if (theme_get_setting('breadcrumb_title')) {
        if ($title = drupal_get_title()) {
          $trailing_separator = $separator;
        }
      }
      // Just add the trailing separator.
      elseif (theme_get_setting('breadcrumb_trailing')) {
        $trailing_separator = $separator;
      }

      // Assemble the breadcrumb.
      return implode($separator, $breadcrumb) . $trailing_separator . $title;
    }
  }
  // Otherwise, return an empty string.
  return '';
}

/**
 * Implements hook_preprocess_HOOK().
 *
 * Save variables to be printed/used in blocks instead of in page.tpl.php
 */
function seed_preprocess_page(&$vars) {
  $seed = &drupal_static('seed');
  if (isset($seed['messages_as_block']) && $seed['messages_as_block']) {
    $seed['show_messages'] = $vars['show_messages'];
    // If no messages yet, save as FALSE so that template_process_page does invoke
    // theme('status_messages')
    if (!isset($vars['messages'])) {
      $vars['messages'] = FALSE;
    }
  }
  unset($seed['messages_as_block']);
}

/**
 * Implements hook_process_HOOK().
 *
 * Save variables to be printed/used in blocks instead of in page.tpl.php
 */
function seed_process_page(&$vars) {
  $seed = &drupal_static('seed');
  $seed['title_prefix'] = $vars['title_prefix'];
  $seed['title_suffix'] = $vars['title_suffix'];
  $seed['title'] = $vars['title'];
}

/**
 * Process blocks created with seed_tools:
 *  - seed_breadcrumb
 *  - seed_messages
 *  - seed_title
 *  - seed_sitename
 */
function seed_process_block(&$vars) {
  if ($vars['block']->module == 'seed_tools') {
    $seed = &drupal_static('seed');
    switch ($vars['block']->delta) {
      case 'seed_breadcrumb':
        $vars['content'] = '<div id="breadcrumb">' . theme('breadcrumb', array('breadcrumb' => drupal_get_breadcrumb())) . '</div>';
        break;

      case 'seed_messages':
        if ($seed['show_messages']) {
          $vars['content'] = theme('status_messages');
          unset($seed['show_messages']);
        }
        break;

      case 'seed_title':
        $vars['content'] = render($seed['title_prefix']);
        if ($seed['title']) {
          $vars['content'] .= '<h1 class="title" id="page-title">' . $seed['title'] . '</h1>';
        }
        $vars['content'] .= render($seed['title_suffix']);
        unset($seed['title_prefix']);
        unset($seed['title']);
        unset($seed['title_suffix']);
        break;


    case 'seed_sitename':
        if (current_path() == 'front') {
          $vars['content'] = '<h1 class="site-name">' . t('Welcome') . '</h1>';
        }
        break;
    }
  }
}

/**
 * Implements hook_preprocess_HOOK().
 * Size set using CSS
 */
function seed_preprocess_textfield(&$vars) {
  $vars['element']['#size'] = NULL;
}

/**
 * Implements hook_preprocess_HOOK().
 * Size set using CSS
 */
function seed_preprocess_password(&$vars) {
  $vars['element']['#size'] = NULL;
}

/**
 * Implements hook_preprocess_HOOK().
 * Size set using CSS
 */
function seed_preprocess_file(&$vars) {
  $vars['element']['#size'] = NULL;
}

/**
 * Implements hook_preprocess_HOOK().
 */
function seed_preprocess_block(&$vars, $hook) {
  $vars['classes_array'][] = drupal_html_class('block-' . $vars['block']->bid);
  $vars['title_attributes_array']['class'][] = 'block-title';
  $vars['content_attributes_array']['class'][] = 'block-content';
}

/**
 * Implements hook_process_HOOK().
 */
function seed_process_pager(&$vars) {
  $original = isset($vars['tags']) ? $vars['tags'] : array();

  $tags = array(
    0 => FALSE,
    1 => FALSE,
    3 => FALSE,
    4 => FALSE,
  );

  if (theme_get_setting('pager_first')) {
    $tags[0] = isset($original[0]) ? $original[0] : t('« first');
  }
  if (theme_get_setting('pager_next')) {
    $tags[1] = isset($original[1]) ? $original[1] : t('‹ previous');
  }
  if (theme_get_setting('pager_previous')) {
    $tags[3] = isset($original[3]) ? $original[3] : t('next ›');
  }
  if (theme_get_setting('pager_last')) {
    $tags[4] = isset($original[4]) ? $original[4] : t('last »');
  }

  $vars['tags'] = $tags;
}

/**
 * Implements hook_THEME().
 */
function seed_pager($vars) {
  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];
  $quantity = $vars['quantity'];
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  // CHANGED: Print pager links that have a tag to use.
  // Others are ignored.
  $li_first = FALSE;
  if ($tags[0]) {
    $args = array(
      'text' => $tags[0],
      'element' => $element,
      'parameters' => $parameters,
    );
    $li_first = theme('pager_first', $args);
  }

  $li_previous = FALSE;
  if ($tags[1]) {
    $args = array(
      'text' => $tags[1],
      'element' => $element,
      'interval' => 1,
      'parameters' => $parameters,
    );
    $li_previous = theme('pager_previous', $args);
  }

  $li_next = FALSE;
  if ($tags[3]) {
    $args = array(
      'text' => $tags[3],
      'element' => $element,
      'interval' => 1,
      'parameters' => $parameters,
    );
    $li_next = theme('pager_next', $args);
  }

  $li_last = FALSE;
  if ($tags[4]) {
    $args = array(
      'text' => $tags[4],
      'element' => $element,
      'parameters' => $parameters,
    );
    $li_last = theme('pager_last', $args);
  }

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '…',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current'),
            'data' => $i,
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '…',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }

    return '<h2 class="element-invisible">' . t('Pages') . '</h2>' . theme('item_list', array(
      'items' => $items,
      'attributes' => array('class' => array('pager', 'clearfix')),
    ));
  }
}

/**
 * Implements hook_THEME().
 */
function seed_pager_link($vars) {
  $text = $vars['text'];
  $page_new = $vars['page_new'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];
  $attributes = $vars['attributes'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  // path only (which is always the current path for pager links). Apparently,
  // none of the pager links is active at any time - but it should still be
  // possible to use l() here.
  // @see http://drupal.org/node/1410574
  $attributes['href'] = url($_GET['q'], array('query' => $query));
  // CHANGED: allow html in link text.
  return '<a' . drupal_attributes($attributes) . '>' . $text . '</a>';
}

/**
 * Implements theme_HOOK().
 */
function seed_select($vars) {
  $original = theme_select($vars);
  $classes = 'seed-select-wrapper';
  $classes .= isset($vars['element']['#disabled']) && $vars['element']['#disabled'] ? ' seed-select-disabled' : '';
  return '<span class="' . $classes . '">' . $original . '</span>';
}

/**
 * Implements theme_HOOK().
 */
function seed_file($vars) {
  $original = theme_file($vars);

  return '<span class="seed-file-wrapper">' . $original . '</span>';
}

/**
* Implements theme_HOOK()
*/
function seed_menu_link(array $variables) {
  // Add menu-item class to li
  $variables['element']['#attributes']['class'][] = 'menu-item';
  return theme_menu_link($variables);
}

/**
* Implements theme_HOOK()
**/
function seed_textarea($vars) {
  // Removes re-sizable functionality on all text areas
  $element = $vars['element'];

  $element['#attributes']['id'] = $element['#id'];
  $element['#attributes']['name'] = $element['#name'];
  $element['#attributes']['cols'] = $element['#cols'];
  $element['#attributes']['rows'] = $element['#rows'];
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper')
  );

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
* Implements theme_HOOK()
**/
function seed_status_messages($variables) {
  // Add a close button to messages
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );
  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= '<div class="messages ' . $type . '">';
    $output .= '<a href="#close-msg" class="btn-close" title="' . t('Close') . '">&times;</a>';
    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>";
    }
    if (count($messages) > 1) {
      $output .= '<ul>';
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . '</li>';
      }
      $output .= '</ul>';
    }
    else {
      $output .= $messages[0];
    }
    $output .= '</div>';
  }

  return $output;
}

/**
 * Implements hook_preprocess_theme().
 */
function seed_preprocess_node(&$vars) {
  // This allows us to put fields on a group "pre_title", which will print
  // things, well, before the title.
  // Easier than creating the title as a field.
  if (isset($vars['content']['group_pre_title'])) {
    $vars['group_pre_title'] = $vars['content']['group_pre_title'];
    hide($vars['content']['group_pre_title']);
  }
  // Avoid strict warnings.
  elseif (!isset($vars['group_pre_title'])) {
    $vars['group_pre_title'] = NULL;
  }

  // Mark the node that is being printed as main content.
  if (node_is_page($vars['node'])) {
    $vars['classes_array'][] = 'seed-node-is-page';
  }

  // Better variable for title printing!
  if (isset($vars['node']->hide_title) && $vars['node']->hide_title) {
    $vars['print_title'] = FALSE;
  }
  $vars['print_title'] = isset($vars['print_title']) ? $vars['print_title'] : !$vars['page'];
}

/**
 * Implements hook_preprocess_file_entity().
 */
function seed_preprocess_field(&$vars) {
  // $field
  $field = field_info_field($vars['element']['#field_name']);
  if ($field['cardinality'] === 1 || $field['cardinality'] === '1') {
    $vars['field-wrapper'] = FALSE;
  }

  $vars['field-wrapper'] = isset($vars['field-wrapper']) ? $vars['field-wrapper'] : TRUE;
  $vars['field-top-level'] = isset($vars['field-top-level']) ? $vars['field-top-level'] : TRUE;
  $vars['field-tag'] = isset($vars['field-tag']) ? $vars['field-tag'] : 'div';
}

/**
* Implements hook_form_FORM_ID_alter()
*
**/
function seed_form_user_login_alter(&$form, &$form_state, $form_id) {
  $actions_suffix = '<div class="actions-suffix">';
  $actions_suffix .= l(t('Request new password'), 'user/password', array('attributes' => array('class' => array('btn-login-password'), 'title' => t('Get a new password'))));
  if (user_register_access()):
    $actions_suffix .= l(t('Create new account'), 'user/register', array('attributes' => array('class' => array('btn-login-register'), 'title' => t('Create a new user account'))));
  endif;
  $actions_suffix .= '</div>';
  $form['actions']['#suffix'] = $actions_suffix;
}

function seed_form_alter(&$form, &$form_state, $form_id){
  if ($form_id == 'search_form'){
      $form['advanced']['submit']['#prefix'] = '<div class="action form-actions">';
  }
}


/**
 * Implements hook_THEME().
 *
 * This is so much better markup than original field!
 * Try to remove as much as we can without breaking things.
 */
function seed_field($vars) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<div class="field-label"' . $vars['title_attributes'] . '>' . $vars['label'] . ':&nbsp;</div>';
  }

  // Render the items.
  if ($vars['field-wrapper'] || !$vars['label_hidden']) {
    $output .= '<div class="field-items"' . $vars['content_attributes'] . '>';
  }
  foreach ($vars['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    if ($vars['field-wrapper']) {
      $output .= '<div class="' . $classes . '"' . $vars['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
    }
    else {
      $output .= drupal_render($item);
    }
  }
  if ($vars['field-wrapper'] || !$vars['label_hidden']) {
    $output .= '</div>';
  }

  // Render the top-level DIV.
  if ($vars['field-top-level']) {
    $output = '<' . $vars['field-tag'] . ' class="' . $vars['classes'] . '"' . $vars['attributes'] . '>' . $output . '</' . $vars['field-tag'] . '>';
  }
  else {
    $output = $output;
  }

  return $output;
}

function seed_preprocess_region(&$vars) {
  $vars['classes_array'][] = 'clearfix';
}

/**
 * Check if a group has regions to be printed, regions should be named
 * GROUP_whatever
 * Call from within page.tpl.php
 *
 * @param string $name
 *   the group name to check against (SECTION)
 * @param array $page
 *   $page variable from page.tpl.php
 *
 * @return boolean
 *   TRUE if group has regions to be printed, FALSE otherwise
 */
function seed_print_group($name, $page) {
  foreach (system_region_list(variable_get('theme_default', 'none')) as $region_key => $region) {
    if (strpos($region_key, $name) === 0 && isset($page[$region_key]) && $page[$region_key]) {
      return TRUE;
    }
  }
  return FALSE;
}

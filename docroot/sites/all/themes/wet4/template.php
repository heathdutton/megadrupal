<?php

/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */


function wet4_preprocess_maintenance_page(&$vars) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  // wet4_preprocess_html($vars);
  // wet4_preprocess_page($vars);

  // This preprocessor will also be used if the db is inactive. To ensure your
  // theme is used, add the following line to your settings.php file:
  // $conf['maintenance_theme'] = 'wet4';
  // Also, check $vars['db_is_active'] before doing any db queries.
}

// Copy from here Template for Content type
function wet4_preprocess_page(&$vars) {
    // - page--example.tpl.php
    if (isset($vars['node'])) {
        $vars['theme_hook_suggestion'] = 'page__'.$vars['node']->type; //
    }
  if (theme_get_setting('base') == 0) {
    drupal_add_js(drupal_get_path('theme', 'wet4') . '/custom.js', array(
        'scope' => 'footer',
        'group' => JS_THEME,
        'preprocess' => FALSE
      ));
  } elseif (theme_get_setting('base') == 1){
    //drupal_add_js(drupal_get_path('theme', 'wet4') .'/intranet/js/wet-boew.min.js', array('scope' => 'footer', 'group' => JS_THEME, 'preprocess' => FALSE));
    //drupal_add_js(drupal_get_path('theme', 'wet4') .'/intranet/js/theme.min.js', array('scope' => 'footer', 'group' => JS_THEME, 'preprocess' => FALSE));
  }

  $logged = $vars['logged_in'];
  drupal_add_js(array("logged" => $logged), 'setting');

  if (theme_get_setting('base') == 0) {
    if (isset($vars['node']->type) && $vars['node']->type == 'splash_page') {
      drupal_add_css(drupal_get_path('theme', 'wet4') . '/css/theme-sp-pe.min.css', array('group' => CSS_THEME,));
    }
    elseif (isset($vars['node']->type) && $vars['node']->type == 'server_error_page') {
      drupal_add_css(drupal_get_path('theme', 'wet4') . '/css/theme-srv.min.css', array('group' => CSS_THEME,));
    }
    elseif (!isset($vars['node']->type)) {
      drupal_add_css(drupal_get_path('theme', 'wet4') . '/css/theme.min.css', array('group' => CSS_THEME,));
    }
    else {
      drupal_add_css(drupal_get_path('theme', 'wet4') . '/css/theme.min.css', array('group' => CSS_THEME,));
    }
  } elseif (theme_get_setting('base') == 1){
    drupal_add_css(drupal_get_path('theme', 'wet4') . '/intranet/css/wet-boew.min.css', array('group' => CSS_THEME,));
    drupal_add_css(drupal_get_path('theme', 'wet4') . '/intranet/css/theme.min.css', array('group' => CSS_THEME,));
  }

  //This is for the sidebar menu styling.
  $element = &$vars['page']['sidebar_first'];
  foreach($element as &$elements){
    if (is_array($elements) && array_key_exists('#block', $elements)){
      $menuBlock = (array) $elements['#block'];
      if ($menuBlock['module'] == "system" || $menuBlock['module'] == "menu" ){
        $elements['#theme_wrappers'][0] = 'menu_tree__sidebar_menu';
        foreach ($elements as &$menuLinks){
          if(is_array($menuLinks) && array_key_exists('#theme', $menuLinks)){
            $menuLinks['#theme'] = 'menu_link__sidebar_menu';
          }
        }
      } elseif ($menuBlock['module'] == "menu_block"){
        $elements['#content']['#theme_wrappers'][0] = 'menu_tree__sidebar_menu';
        foreach ($elements['#content'] as &$menuLinks){
          if(is_array($menuLinks) && array_key_exists('#theme', $menuLinks)){
            $menuLinks['#theme'][0] = 'menu_link__sidebar_menu';
            foreach ($menuLinks['#below'] as &$menuLinksSub){
              if(is_array($menuLinksSub) && array_key_exists('#theme', $menuLinksSub)){
                $menuLinks['#below']['#theme_wrappers'][0] = 'menu_tree__sidebar_menu';
                $menuLinksSub['#theme'][0] = 'menu_link__sidebar_menu_sub';
              }
            }
          }
        }
      }
    }
  }
  //This is for the main menu.
  $mainMenu = &$vars['page']['menu'];
  foreach($mainMenu as &$mainMenuBlock){
    if (is_array($mainMenuBlock) && array_key_exists('#theme_wrappers', $mainMenuBlock)){
      $mainMenuBlock['#content']['#theme_wrappers'][0] = "menu_tree__main_menu";
      foreach($mainMenuBlock['#content'] as &$mainMenuLink){
        if (is_array($mainMenuLink) && array_key_exists('#theme', $mainMenuLink)){
          $mainMenuLink['#theme'] = 'menu_link__main_menu';
        }
      }
    }
  }
}

/**
 * Implements theme_menu_tree to style sidebar menus().
 */
function wet4_menu_tree__sidebar_menu(&$variables) {
  return '<ul class="list-group menu list-unstyled">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_link to style sidebar menus().
 */
function wet4_menu_link__sidebar_menu($variables) {
  // Add class for a.
  $variables['element']['#localized_options']['attributes']['class'][] = 'list-group-item';
  return theme_menu_link($variables);
}

function wet4_menu_link__sidebar_menu_sub($variables) {
  // Add class for a.
  $variables['element']['#localized_options']['attributes']['class'][] = 'list-group-item sub-group-item';
  return theme_menu_link($variables);
}



function wet4_css_alter(&$css){
  unset($css[drupal_get_path('module','system').'/system.menus.css']);
}

/**
 * Implements theme_menu_tree to style main menu().
 */
function wet4_menu_tree__menu_top(&$variables) {
    return '<ul class="list-inline" id="gc-bar">' . $variables['tree'] . '</ul>';
}

function wet4_menu_tree__main_menu(&$variables) {
  return '<ul class="list-inline menu" role="menubar">' . $variables['tree'] . '</ul>';
}

function wet4_menu_tree__main_menu_second($variables) {
  return '<ul class="sm list-unstyled" role="menu">' . $variables['tree'] . '</ul>';
}

function wet4_menu_link__main_menu($variables) {
  $element = $variables['element'];
  $submenu = '';
  if ($element['#below']) {
    // You can set a theme wrapper here or put an empty array() only
    // and theme the second level directly by adding <ul></ul> one line below.
    $element['#below']['#theme_wrappers'] = array('menu_tree__main_menu_second');
    $element['#localized_options']['attributes']['class'][] = 'item';
    $submenu = drupal_render($element['#below']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $submenu . "</li>\n";
}
/**
 * Implements theme_menu_tree to style the main menu().
 */
function wet4_menu_tree__menu_main(&$variables) {
    return '<ul class="list-inline menu" role="menubar">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree to style sidebar menus().
 */
function wet4_menu_tree__menu_sidebar(&$variables) {
    return '<ul class="list-group menu list-unstyled">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree to style content bottom menu().
 */
function wet4_menu_tree__menu_content_bottom(&$variables) {
    return '<ul id="gc-tctr" class="list-inline">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree to style footer column menus().
 */
function wet4_menu_tree__menu_footer(&$variables) {
    return '<ul class="list-unstyled">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree to style page footer menu().
 */
function wet4_menu_tree__menu_page_footer(&$variables) {
    return '<ul class="list-inline">' . $variables['tree'] . '</ul>';
}
/**
 * Implements theme_menu_link to style page footer menu().
 */
function wet4_menu_link(array $variables) {
    $element = $variables['element'];
    $sub_menu = '';

    if ($element['#below']) {
        $sub_menu = drupal_render($element['#below']);
    }

    if ($element['#original_link']['menu_name'] == 'menu-footer') {
        $linkText = '<span>' . $element['#title'] . '</span>';
        $element['#localized_options']['html'] = true;
        if ($element['#title'] == "Canada.ca"){
            $linkText = $element['#title'];
            $element['#localized_options']['html'] = true;
            $element['#attributes']['id'] = "canada-ca";
        }
    } else {
        $linkText = $element['#title'];
    }

    $output = l($linkText, $element['#href'], $options = $element['#localized_options']);

    return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


/**
 * Implements hook_block_view_alter.
 */
function wet4_block_view_alter(&$data, $block) {
  if ( $block->module =="menu"){
    if($block->region == "about_site") {
      // Apply the wet4 page content bottom styling
      $data['content']['#theme_wrappers'] = array('menu_tree__menu_content_bottom');
    } elseif($block->region == "footer_1" || $block->region == "footer_2" || $block->region == "footer_3" || $block->region == "footer_4"){
      // Apply the wet4 footer column menu styling
      $data['content']['#theme_wrappers'] = array('menu_tree__menu_footer');
    } elseif($block->region == "footer_bottom"){
      // Apply the wet4 page footer menu styling
      $data['content']['#theme_wrappers'] = array('menu_tree__menu_page_footer');
    } elseif($block->region == "top_links"){
      // Apply the wet4 page top menu styling
      $data['content']['#theme_wrappers'] = array('menu_tree__menu_top');
    }
  } elseif ($block->module =="menu_block"){
    if($block->region == "about_site") {
      // Apply the wet4 page content bottom styling
      $data['content']['#content']['#theme_wrappers'] = array('menu_tree__menu_content_bottom');
    } elseif($block->region == "footer_1" || $block->region == "footer_2" || $block->region == "footer_3" || $block->region == "footer_4"){
      // Apply the wet4 footer column menu styling
      $data['content']['#content']['#theme_wrappers'] = array('menu_tree__menu_footer');
    } elseif($block->region == "footer_bottom"){
      // Apply the wet4 page footer menu styling
      $data['content']['#content']['#theme_wrappers'] = array('menu_tree__menu_page_footer');
    } elseif($block->region == "top_links"){
      // Apply the wet4 page top menu styling
      $data['content']['#content']['#theme_wrappers'] = array('menu_tree__menu_top');
    }
  }
}

/**
 * Implements theme_breadcrumb().
 */
function wet4_breadcrumb($variables) {
    $breadcrumb = array_unique($variables['breadcrumb']);

  if (!empty($breadcrumb)) {
      $crumbs = '<ol class="breadcrumb">';

      foreach($breadcrumb as $value) {
           $crumbs .= '<li>'.$value.'</li>';
      }
      $crumbs .= '<li>'.drupal_get_title().'</li>';
      $crumbs .= '</ol>';
   
      return $crumbs;
	 }
}

/**
 * Implements theme_form_FORM_ID_alter() for the Search block.
 */
function wet4_form_search_block_form_alter(&$form, &$form_state, $form_id) {
    $form['#attributes']['class'][] = 'form-inline';
    $form['search_block_form']['#prefix'] = '<div class="form-group">';
    $form['search_block_form']['#suffix'] = '</div>';
    $form['search_block_form']['#title'] = t('Search website');
    $form['search_block_form']['#size'] = 27;  // define size of the textfield
    //$form['search_block_form']['#attributes']['id'] = 'wb-srch-q';
    $form['search_block_form']['#attributes']['class'][] = 'form-control';
    $form['actions']['submit'] = array (
      '#type' => 'submit',
      '#value' => t('Search'),
      //'#prefix' => '<button type="submit" id="wb-srch-sub" class="btn btn-default">' . t('Search') . '</button>',
      '#submit' => array('search_box_form_submit'),
      '#attributes' => array('class' => array( 'btn  btn-default' )),
      //'style' => array( 'display: none' ),  <--- old hide the input
    );
  //dpm($form_state);
}

/**
 * Modify the output of the language selector block.
 */
function wet4_links__locale_block(&$variables) {
  // the global $language variable tells you what the current language is
  global $language;

  // hide active language
  unset($variables['links'][$language->language]);

// an array of list items
  $items = array();
  foreach($variables['links'] as $lang => $info) {

    $name     = $info['language']->native;
    $href     = isset($info['href']) ? $info['href'] : '';
    // if the global language is that of this item's language, add the active class
    if($lang === $language->language){
      $li_classes[] = 'active';
    }
    $options = array('language' => $info['language'],
      'html'     => true
    );
    $link = l($name, $href, $options);

    // display only translated links
    if ($href) $items[] = array('data' => $link);
  }

// output
  $attributes = array('class' => array('list-inline'), 'id' => 'gc-bar');
  $output = theme_item_list(array('items' => $items,
    'title' => '',
    'type'  => 'ul',
    'attributes' => $attributes,
  ));
  return $output;
}



/**
 * Implements theme_form_element_label() for the field labels.
 */
function wet4_form_element_label($variables) {
    $element = $variables['element'];
    if (isset($element['#required']) && $element['#required'] == 'TRUE') {
        return '<label class="required">' . $element['#title'] . '</label>';
    } else {
        return '<label>' . $element['#title'] . '</label>';
    }

}

/*
 * Implements theme_status_messages();
 */
function wet4_status_messages($variables) {
    $display = $variables['display'];
    $output = '';
    $status_heading = array(
        'status' => t('Status message'),
        'danger' => t('Error message'),
        'warning' => t('Warning message'),
    );
    foreach (drupal_get_messages($display) as $type => $messages) {
        if ($type == "error"){
            $alert_type = "alert-danger";
        } elseif ($type == "status"){
            $alert_type = "alert-success";
        } elseif ($type == "warning"){
            $alert_type = "alert-warning";
        }
      if (isset($alert_type)) {
        $output .= "<section><details class=\"alert $alert_type\" id=\"alert-success\" open=\"open\">\n";
      }
        if (!empty($status_heading[$type])) {
            $output .= '<summary class="h3"><h3>' . $status_heading[$type] . "</h3></summary>\n";
        }
        if (count($messages) > 1) {
            foreach ($messages as $message) {
                $output .= '<p>' . $message . "</p>\n";
            }
        }
        else {
            $output .= $messages[0];
        }
        $output .= "</details></section>\n";
    }
    return $output;
}


/*
 * Implements hook_menu_local_tasks_alter() to style menu_local_tabs;
 */
function wet4_menu_local_tasks($variables) {
  $links = $variables['primary'];
  if (!empty($links)){
    print '<div class="local-tabs local-tabs-inited tabs-acc" id="local-auto-1"><ul role="tablist" aria-live="off" class="generated" aria-hidden="false">';
    foreach($links as &$link){
      $tab = $link['#link'];
      if (is_array($link) && array_key_exists('#active', $link)){
        print '<li role="presentation" class="active">' . l($tab['title'], $tab['href'], $tab['localized_options']) . "</li>\n";
      } elseif (is_array($link) && !array_key_exists('#active', $link)) {
        print '<li role="presentation">' . l($tab['title'], $tab['href'], $tab['localized_options']) . "</li>\n";
      }
    }
    print '</ul></div>';
  }
}
/*
 * Implements hook_form_FORM_ID_alter() for the login form.
 */
function wet4_form_user_login_alter(&$form, &$form_state, $form_id) {
  $form['#prefix'] = '<div class="form-group">';
  $form['#suffix'] = '</div>';
  $form['name']['#attributes']['class'][] = 'form-control';
  $form['pass']['#attributes']['class'][] = 'form-control';
  $form['actions']['submit']['#attributes']['class'][] = 'btn btn-default';
}

/*
 * Implements hook_form_FORM_ID_alter() for the login form.
 */
function wet4_form_user_pass_alter(&$form, &$form_state, $form_id) {
  $form['#prefix'] = '<div class="form-group">';
  $form['#suffix'] = '</div>';
  $form['name']['#attributes']['class'][] = 'form-control';
  $form['actions']['submit']['#attributes']['class'][] = 'btn btn-default';
}

/*
 * Implements hook_form_FORM_ID_alter() for the login form.
 */
function wet4_form_user_register_form_alter(&$form, &$form_state, $form_id) {
  $form['#prefix'] = '<div class="form-group">';
  $form['#suffix'] = '</div>';
  $form['account']['name']['#attributes']['class'][] = 'form-control';
  $form['account']['mail']['#attributes']['class'][] = 'form-control';
  $form['actions']['submit']['#attributes']['class'][] = 'btn btn-default';
}

/**
 * Implements hook_modernizr_load_alter().
 *
 * @return
 *   An array to be output as yepnope testObjects.
 */
/* -- Delete this line if you want to use this function
function wet4_modernizr_load_alter(&$load) {

}



/**
 * Implements hook_preprocess_html()
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
/* -- Delete this line if you want to use this function

function wet4_preprocess_html(&$vars) {
    drupal_add_js(drupal_get_path('theme', 'wet4') . "/js/wet-boew.min.js", array('type'=>'file', 'scope'=>'footer'));
    drupal_add_js(drupal_get_path('theme', 'wet4') . "/js/theme.min.js", array('type'=>'file', 'scope'=>'footer'));

}

/**
 * Override or insert variables into the page template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
/* -- Delete this line if you want to use this function
function wet4_preprocess_page(&$vars) {

}

/**
 * Override or insert variables into the region templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
/* -- Delete this line if you want to use this function
function wet4_preprocess_region(&$vars) {

}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
/* -- Delete this line if you want to use this function
function wet4_preprocess_block(&$vars) {

}
// */

/**
 * Override or insert variables into the entity template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
/* -- Delete this line if you want to use this function
function wet4_preprocess_entity(&$vars) {

}
// */

/**
 * Override or insert variables into the node template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
/* -- Delete this line if you want to use this function
function wet4_preprocess_node(&$vars) {
  $node = $vars['node'];
}
// */

/**
 * Override or insert variables into the field template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("field" in this case.)
 */
/* -- Delete this line if you want to use this function
function wet4_preprocess_field(&$vars, $hook) {

}
// */

/**
 * Override or insert variables into the comment template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
/* -- Delete this line if you want to use this function
function wet4_preprocess_comment(&$vars) {
  $comment = $vars['comment'];
}
// */

/**
 * Override or insert variables into the views template.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
/* -- Delete this line if you want to use this function
function wet4_preprocess_views_view(&$vars) {
  $view = $vars['view'];
}
// */


/**
 * Override or insert css on the site.
 *
 * @param $css
 *   An array of all CSS items being requested on the page.
 */
/* -- Delete this line if you want to use this function
function wet4_css_alter(&$css) {

}
// */

/**
 * Override or insert javascript on the site.
 *
 * @param $js
 *   An array of all JavaScript being presented on the page.
 */
/* -- Delete this line if you want to use this function
function wet4_js_alter(&$js) {

}
// */

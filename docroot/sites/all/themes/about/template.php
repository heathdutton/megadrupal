<?php

/**
 * Provides hook_about_links() so that modules may add additional incons, links,
 * and functionality to the items under the main story. Return from the hook 
 * should be an array of the form:
 *   array(
 *     $item => array(
 *       'icon' => '',
 *       'path' => '',
 *       'js' => '',
 *       'js options' => '',
 *       'css' => '',
 *       'css options' => '',
 *       'form elements' => array(),
 *     ),
 *   );
 * 
 * In the subarray, all items are optional, but the 'icon' item must be present.
 * for a link to be drawn. The css and js "options" items default to 'file', but
 * can be provided to be passed as the (second) $options argument to the 
 * drupal_add_[j|cs]s() function call.
 * 
 * If this item is configurable in the about theme settings form, the renderable
 * array for its elements can be supplied in 'form elements' to be used in the 
 * theme-settings.php file.
 * 
 * Elements will eventually be included named as "$module_$item" to avoid
 * namespace collisions with other implementing modules.
 */


/**
 * Unfortunately, hook_init() isn't available for themes... 
 * We will place our initialization logic in hook_preprocess(), which is the
 * earliest place in the boot cycle that a theme can get involved.
 * 
 * possible todo: move the cookie logic to about_tools.module 
 * 
 * /
function about_init() {
  _about_ensure_cookie();
}
*/
/**
 * implementation of hook_preprocess()
 */
function about_preprocess(&$vars) {
  _about_ensure_cookie();
}
/* */
function _about_ensure_cookie() {
  require(drupal_get_path('theme', 'about') . '/tools/cookieFactory.php');  
}  


/**
 * implementation of hook_preprocess_page()
 * 
 * Load our css and js files here, rather than through our .info file because
 * we only want them for our frontpage.
 */
function about_preprocess_page(&$vars) {
  
  if (drupal_is_front_page()) {

    $about_path = drupal_get_path('theme', 'about');
    $inline_opts['type'] = 'inline';
    $ext_opts['type'] = 'external';
    
    // verbiage
    $vars['about_name'] = array(
      '#type' => 'markup',
      '#markup' => theme_get_setting('about_name'),
    );
    $about_story = theme_get_setting('about_story');
    $vars['about_story'] = array(
      '#type' => 'markup',
      '#markup' => check_markup($about_story['value'], $about_story['format']),
    );
    
    // link icons
    $vars['about_links'] = about_get_links();    

    // js (load some in the footer, as they use Drupal.settings in ready() functions)
    drupal_add_js("$about_path/js/transify-min.js");
    drupal_add_js("$about_path/js/jquery.corner.js");
    drupal_add_js("$about_path/js/about.js", array('type'=>'file', 'scope'=>'footer'));
    drupal_add_js("$about_path/js/resize.js");    

    // general css
    drupal_add_css($about_path . '/style.css');

    // headline css
    $about_name_font = theme_get_setting('about_name_font');
    $about_name_color = theme_get_setting('about_name_color');
    $google_font_link = 'http://fonts.googleapis.com/css?family=' . str_replace(' ', '+', $about_name_font);
    drupal_add_css($google_font_link, $ext_opts);
    $about_name_font_size = theme_get_setting('about_name_font_size');
    drupal_add_css("#about-name { 
                      font-family:'$about_name_font'; 
                      font-size:${about_name_font_size}px;
                      color:$about_name_color;
                    }", $inline_opts);
    
    // story box css
    $js['about'] = array(
      'fontFamily' => 'ariel', 
      'fontSize' => '1.0em',
      'color' => theme_get_setting('about_story_color'),
      'backgroundColor' => theme_get_setting('about_story_bg_color'),
      'backgroundOpacity' => .5,
    );
    drupal_add_js($js, 'setting');
    
    // background image css
    $bg = theme_get_setting('about_bg_path');
    $bg_color = theme_get_setting('about_bg_color');
    // below should already be called by now, but we need to set the $about_cookie_name var.
    require($about_path . '/tools/cookieFactory.php'); 
    list($height, $width) = explode('x', $_COOKIE[$about_cookie_name]); 
    // imageFactory.php?i=sethfreach/IMG_1870r2.JPG&h=889&w=1314
    $bg_path = "$about_path/tools/imageFactory.php?i=$bg&h=$height&w=$width";
    drupal_add_css("body {background: $bg_color url($bg_path) no-repeat fixed right top;}", $inline_opts);
  }
}

/*
function about_process_page(&$vars) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}

function about_process_html(&$vars) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}
*/

function about_get_links() {
  $links = array(
    '#attributes' => array(
      'id' => 'links'
    ),
  );
  
  $opts = array(
    'html' => true,
    'attributes' => array(
      'target' => '_blank',
    ),
  );
  $img_base = drupal_get_path('theme', 'about') . '/images';
  
  $fb = theme_get_setting('about_facebook');
  if (isset($fb) && !empty($fb)) {
    $links['facebook'] = array(
      '#theme' => 'image_formatter',
      '#item' => array(
        'uri' => "$img_base/facebook_32.png"
      ),
      '#path' => array (
        'path' => 'http://www.facebook.com/' . $fb,
        'options' => array('html' => TRUE, 'attributes' => array('target' => '_blank')),
        'attributes' => array('target' => '_blank'),
      ),  
    );
  }
  $li = theme_get_setting('about_linkedin');
  if (isset($li) && !empty($li)) {
    $links['linkedin'] = array(
      '#theme' => 'image_formatter',
      '#item' => array(
        'uri' => "$img_base/linkedin_32.png"
      ),
      '#path' => array (
        'path' => 'http://www.linkedin.com/in/' . $li,
        'options' => array('html' => TRUE, 'attributes' => array('target' => '_blank')),
      ),  
    );
  }
  $tw = theme_get_setting('about_twitter');
  if (isset($tw) && !empty($tw)) {
    $links['twitter'] = array(
      '#theme' => 'image_formatter',
      '#item' => array(
        'uri' => "$img_base/twitter_32.png"
      ),
      '#path' => array (
        'path' => 'http://twitter.com/' . $tw,
        'options' => array('html' => TRUE, 'attributes' => array('target' => '_blank')),
      ),  
    );
  }
  $fl = theme_get_setting('about_flickr');
  if (isset($fl) && !empty($fl)) {
    $links['flickr'] = array(
      '#theme' => 'image_formatter',
      '#item' => array(
        'uri' => "$img_base/flickr_32.png"
      ),
      '#path' => array (
        'path' => 'http://flickr.com/photos/' . $fl,
        'options' => array('html' => TRUE, 'attributes' => array('target' => '_blank')),
      ),  
    );
  }
   
  // get any links that are supplied by outside modules.
  $hook_modules = array();
  foreach (module_implements('about_links') as $module) {
    $link_data_array = module_invoke($module, 'about_links');
    $hook_modules[$module] = $link_data_array;
    foreach($link_data_array as $link => $link_data) {
      // avoid name space collisions
      $link_name = $module . '_' . $link;
      
      // add js
      if (isset($link_data['js']) && !empty($link_data['js'])) {
        if (isset($link_data['js options']) && !empty($link_data['js options'])) {
          drupal_add_js($link_data['js'], $link_data['js options']);
        }
        else {
          drupal_add_js($link_data['js']);
        }
      }
      
      // add css
      if (isset($link_data['css']) && !empty($link_data['css'])) {
        if (isset($link_data['css options']) && !empty($link_data['css options'])) {
          drupal_add_css($link_data['css'], $link_data['css options']);
        }
        else {
          drupal_add_css($link_data['css']);
        }
      }
      
      // build the link
      if (isset($link_data['icon']) && !empty($link_data['icon'])) {
        $links[$link_name] = array(
          '#theme' => 'image_formatter',
          '#item' => array(
            'uri' => $link_data['icon'],
          ),
        );
        if (isset($link_data['path']) && !empty($link_data['path'])) {
          $links[$link_name]['#path'] = array (
            'path' => $link_data['path'],
            'options' => array('html' => TRUE, 'attributes' => array('target' => '_blank')),
          );
        }
      }
    }
  }
  
  foreach ($links as $k => $link) {
    $links[$k]['#prefix'] = '<div class="link link-' . str_replace('_', '-', $k) . '">';
    $links[$k]['#suffix'] = '</div>';
  }
  
  return $links;
}


/**
 * implementation of hook_theme() 
 */
function about_theme() {
  return array(
    // radio group.
    'about_font_radios' => array(
      'render element' => 'element',
    ),
    // single radio.
    'about_font_radio' => array(
      'render element' => 'element',
    ),
  );
}


function theme_about_font_radios($variables) {
dpm($variables);
  $element = $variables['element'];
  $keys = array_keys($element['#options']);
  $type = $element[$keys[0]]['#type'];
 
  // Start wrapper div for the group.
  $output .= '<div class="form-radios about-font-radios">';
 
  foreach ($keys as $key) {
    // Each radios is theme by calling our custom 'my_radio' theme function.
    $output .= theme('about_font_radio', $element[$key]);
  }
 
  $output .= '</div>';
  return $output;
}


function theme_about_font_radio($variables) {
  $element = $variables['element'];
  
  _form_set_class($element, array('form-radio'));
  $output = '<input type="radio" ';
  $output .= 'id="' . $element['#id'] . '" ';
  $output .= 'name="' . $element['#name'] . '" ';
  $output .= 'value="' . $element['#return_value'] . '" ';
  $output .= (check_plain($element['#value']) == $element['#return_value']) ? ' checked="checked" ' : ' ';
  $output .= drupal_attributes($element['#attributes']) . ' />';
  if (!is_null($element['#title'])) {
    $output = '<label class="option" for="' . $element['#id'] . '">' . $output . ' ' . $element['#title'] . '</label>';
  }
 
  unset($element['#title']);
  // Default 'form_element' theme is uncommented below, which create a DIV wrapper for each item.
  //return theme('form_element', $element, $output);
  return $output;
}


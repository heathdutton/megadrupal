<?php // $Id $

require_once("common_methods.php");

function get_full_path_to_theme() {
  return base_path() . path_to_theme();
}

function energetic_service_links_node_format($variables) {
  $links = &$variables['links'];
  return '<div class="service-links"><div class="service-label">'. t('Bookmark/Search this post with: ') .'</div>'. art_links_worker($links) .'</div>';
}

/**
 * Generate the HTML output for a single local task link.
 *
 * @ingroup themeable
 */
function energetic_menu_local_task($variables) {
  $link = &$variables['element']['#link'];
  $active = &$variables['element']['#active'];
  $link_text = '<span class="btn">'
    .'<span class="l"></span>'
    .'<span class="r"></span>'
    .'<span class="t' . (!empty($active)? ' active': '') . '">' . $link['title'] . '</span>'
    .'</span>';
  $link['localized_options']['html'] = TRUE;
  $link['localized_options']['attributes']['class'][] = 'Button';
  return l($link_text, $link['href'], $link['localized_options']);
  }

function energetic_breadcrumb($variables) {
  if(!($breadcrumb = drupal_get_breadcrumb()))
    $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">' . implode(' | ', $breadcrumb) . '</div>';
  }
}

/**
 * Theme a form button.
 *
 * @ingroup themeable
 */
function energetic_button($variables) {
  $element = &$variables['element'];
  // Make sure not to overwrite classes.
  if (isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = 'Button form-' . $element['#button_type'] . ' ' . $element['#attributes']['class'];
  }
  else {
    $element['#attributes']['class'] = 'Button form-' . $element['#button_type'];
  }

  // Skip admin pages due to some issues with ajax looking for <input> not <button>.
  if (arg(0) == 'admin') {
    return '<input type="submit" '. (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ') .'id="'. $element['#id'] .'" value="'. check_plain($element['#value']) .'" '. drupal_attributes($element['#attributes']) ." />\n";
  }

  return '<button type="submit" ' . (empty($element['#name']) ? '' : 'name="' . $element['#name']
         . '" ')  . 'id="' . $element['#id'] . '" value="' . check_plain($element['#value']) . '" ' . drupal_attributes($element['#attributes']) . '>'
         . '<span class="btn">'
         . '<span class="l"></span>'
         . '<span class="r"></span>'
         . '<span class="t">' . check_plain($element['#value']) . '</span>'
         . '</span></button>';
}

/** Image Assist support removed -- module is not present in D7 **/

function energetic_links($variables){
  if(is_array($variables['attributes']['class'])){
    $idx = array_search('inline', $variables['attributes']['class']);
    if($idx !== FALSE){
      unset( $variables['attributes']['class'][$idx] );
    }
  }
  return theme_links($variables);
}

function energetic_preprocess_node(&$variables, $hook) {
  $content = &$variables['content'];
  $terms = array();
  foreach(preg_grep('/^[^#]/', array_keys($content)) as $key){
    if(array_key_exists('#field_type', $content[$key])
       && $content[$key]['#field_type'] === 'taxonomy_term_reference'){
      $terms[$key] = & $content[$key];
      unset($content[$key]);
    }
  }
  $variables['terms'] = &$terms;
}
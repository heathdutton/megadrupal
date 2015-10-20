<?php
/**
 * Created by JetBrains PhpStorm.
 * User: Nico Heulsen
 * Email: info@artx.be
 * Date: 25/11/11
 * Time: 12:42
 */



function theme_backdrop_active($vars) {
  $backdrop = $vars['backdrop'];
  $image = '';
  $output = '';

  
  if (module_exists('image')) {
    $file = file_load($backdrop['fid']);
    $image_path = image_style_url('medium', $file->uri);
    $image = theme('image', array('path' => $image_path));
  }

  $output .= '<div class="backdrop">';
  $output .= '  <div class="left">' . $image . '</div>';
  $output .= '  <div class="right">';
  $output .= '    <h3>'. $backdrop['name'] . '</h3>';
  $output .= '    <p>'. $backdrop['description'] . '</p>';

  $items = array();
  $items[] = $backdrop['links']['settings'];
  $items[] = $backdrop['links']['disable'];

  $output .= '    <div class="links">' . theme('item_list', array('items' => $items)) . '</div>';
  $output .= '  </div>';
  $output .= '</div>';


  return $output;
}

/**
 * Theming function for an inactive item
 *
 * @param $vars
 * @return string
 */
function theme_backdrop_inactive($vars) {
  $backdrop = $vars['backdrop'];

  $output = '';
  
  if (module_exists('image')) {
    $file = file_load($backdrop['fid']);
    $image_path = image_style_url('medium', $file->uri);
    $image = theme('image', array('path' => $image_path));
  }

  $output .= '<div class="backdrop inactive">';
  $output .= $image;
  $output .= '<h3>'. $backdrop['name'] . '</h3>';
  $output .= '<p>'. $backdrop['description'] . '</p>';

  $items = array();
  $items[] = $backdrop['links']['settings'];
  $items[] = $backdrop['links']['enable'];

  $output .= '<div class="links">' . theme('item_list', array('items' => $items)) . '</div>';

  $output .= '</div>';

  return $output;
}

/**
 * Theming new backdrop form
 *
 * @param $vars
 * @return bool|string
 */
function theme_backdrop_options_form($vars) {
  $output = '';

  $form = $vars['form'];
  $output .= '<div id="backdrop-wrapper">';

  //the div '#backdrop-elements-wrapper' should be left since this is used to add extra elements
  $output .= '  <div id="backdrop-elements-wrapper">';

  foreach (element_children($form) as $key) {
    $output .= '<div class="backdrop-item clearfix">';

    //top region (file upload)
    $output .= '  <div class="top">';
    if (!empty($form[$key]['file']['#value']['fid'])) {
      if (module_exists('image')) {
        $thumbnail_path = image_style_url('thumbnail', $form[$key]['file']['#file']->uri);
        $thumbail = theme('image', array('path' => $thumbnail_path));
        $form[$key]['file']['filename']['#markup'] = $thumbail;
      }
    }
    $output .= drupal_render($form[$key]['file']);
    $output .= '  </div>';

    //middle region (name + description)
    $output .= '  <div class="middle">';
    $output .= drupal_render($form[$key]['name']);
    $output .= drupal_render($form[$key]['description']);
    $output .= '  </div>';

    //bottom region (css attributes)
    $output .= '  <div class="bottom">';
    $output .= '    <div class="bottom-left">';
    $output .= '      <div class="bottom-left-element">' . drupal_render($form[$key]['color']) . '</div>';
    $output .= '      <div class="bottom-left-element">' . drupal_render($form[$key]['repeat']) . '</div>';
    $output .= '    </div>';
    $output .= '    <div class="bottom-right">' . drupal_render($form[$key]['data']) . '</div>';
    $output .= '  </div>';

    $output .= drupal_render($form[$key]);
    $output .= '</div>';
  }

  $output .= '  </div>';
  $output .= drupal_render_children($form);
  $output .= '</div>';

  return $output;
}
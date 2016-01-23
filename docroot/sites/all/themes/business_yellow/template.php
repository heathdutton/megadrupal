<?php

  /**
  * @file
  * Business Yellow Theme
  * Created by Zyxware Technologies
  */

function business_yellow_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    //$form['search_block_form']['#size'] = 11;  // define size of the textfield
    $form['search_block_form']['#attributes']['placeholder'] = t('Search');
    $form['actions']['submit']['#value'] = ''; // Change the text on the submit button
    $form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/images/search_icon.png');
  }
}

function ago($timestamp) {
  $difference = time() - $timestamp;
  $periods = array("sec", "minute", "hour", "day", "week", "month", "year", "decade");
  $lengths = array("60", "60", "24", "7", "4.35", "12", "10");
  if ($difference <= 60) {
    $text="Within a Minute";
  }
  elseif ($difference >= 86400 && $difference <= 172800) {
    $text="Yesterday";
  }
  else {
    for ($j = 0; $difference >= $lengths[$j]; $j++)
      $difference /= $lengths[$j];
      $difference = round($difference);
    if ($difference != 1) $periods[$j] .= "s";
      $text="$difference $periods[$j] ago";
  }
  return $text;
}

function business_yellow_preprocess_comment(&$variables) {
  $variables['comment_created']   = $variables['comment']->created;
  $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  $variables['default_photo'] = '<img src="' . base_path( ) . $theme_path . '/images/profile_picture.jpeg" />';
}
  function business_yellow_preprocess_page(&$variables) {
  $image_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  $variables['up_arrow'] = '<img src="' . base_path( ) . $image_path . '/images/up_arrow .png" />';
  $variables['down_arrow'] = '<img src="' . base_path( ) . $image_path . '/images/down_arrow.png" />';
}
  function business_yellow_preprocess_node(&$variables) {
  $about_image_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  $variables['up_arrow'] = '<img src="' . base_path( ) . $about_image_path . '/images/up_arrow .png" />';
  $variables['down_arrow'] = '<img src="' . base_path( ) . $about_image_path . '/images/down_arrow.png" />';
}

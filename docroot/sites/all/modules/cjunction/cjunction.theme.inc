<?php

/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 */

/**
 * A custom theme function for showing a remote image
 */
function theme_cjunction_image_url($variables) {
  $output = theme('image', array("path" => $variables['image_url'], "alt" => '', "title" => '',  "attributes" => array("height" => "100px"), "getsize" => FALSE));
  return $output;
}

/**
 * A custom theme function for showing a buy link
 */
function theme_cjunction_buy_url($variables) {
  $output = l(t('Buy It'), $variables['buy_url']);
  return $output;
}

/**
 * Theme function for showing a product's list for filter
 */
function theme_cjunction_shortcode_listing($variables) {

  $output = '<div class="cjshortcode"><ul>';
  foreach ($variables['items'] as $row) {
    $output .= '<li>';
    $output .= '<div class="cjimg">' . theme('image', array("path" => $row->cj_image_url['und'][0]['value'], "alt" => '', "title" => '', "attributes" => array("height" => "110px"), "getsize" => FALSE)) . '</div>';
    $output .= '<div class="cjpr">' . l($row->cj_name['und'][0]['value'], $row->cj_buy_url['und'][0]['value'], array("attributes"=>array("class"=>"cjtitle")))  . "<br>";
    $output .= t("Manufacturer: ") . $row->cj_manufacturer_name['und'][0]['value'] . "<br>";
    $output .= t("Offered by: ") . $row->cj_advertiser_name['und'][0]['value'] . "<br>";
    $output .= '<span class="cjprice">' . t("Price: ") . $row->cj_currency['und'][0]['value'] . " " . $row->cj_price['und'][0]['value'] . '</span>' . "<br>";
    $output .= l(t('Buy now!'), $row->cj_buy_url['und'][0]['value'], array("attributes"=>array("class"=>"cjbtn")));
    $output .=  "</div>";

    $output .= '</li>';
  }
  $output .= '</ul></div>';

  return $output;
}

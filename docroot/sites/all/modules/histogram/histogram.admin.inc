<?php

/**
 * @file
 * Help page for Histogram module.
 *
 * Gives an overview over the available options for the histograms.
 */

/**
 * Text for the help page.
 * @return string
 *   HTML String
 */
function histogram_admin_settings() {

  drupal_add_css(drupal_get_path('module', 'histogram') . '/histogram.admin.css');

  $output = '';

  $output .= '<h3>' . t('About') . '</h3>';

  $output .= '<p>';

  $output .= t('The Histogram module allows you to generate a histogram for images in nodes.');

  $output .= '</p>';

  $output .= '<p>';

  $output .= t('It can create histograms for JPG, GIF or PNG images.');

  $output .= '</p>';

  $output .= '<p>';

  $output .= t('The module creates a new field type and widget called "histogram". Once attached to a node, it will create a histogram for the image field on the node when the node is saved. Currently, it only supports one image field per node per histogram field.');

  $output .= '</p>';

  $output .= '<p>';

  $output .= t('It can create stacked three color histograms as well as merged histograms. The background color can be set to any hexadecimal value. These options can be selected on an image by image basis.');

  $output .= '</p>';

  $output .= '<h3>' . t('Installation and Usage') . '</h3>';

  $output .= '<p>';

  $output .= t('Go to the manage fields page for the nodetype that you want to create a histogram for and add a new field with the field type "Histogram."');

  $output .= '</p>';

  $output .= '<p>';

  $output .= t('If your node has multiple image fields, select which one you want to generate a histogram with on the settings page.');

  $output .= '</p>';

  $output .= '<p>';

  $output .= t('The "Histogram Type" option determines whether your histogram will be three separate channels stacked on top of each other or a single merged histogram. On black and white images, a single channel histogram will be generated regardless of the value of this option.');

  $output .= '</p>';

  $output .= '<p>';

  $output .= t('"Force Single Channel Histogram" allows you to generate a single channel histogram by forcing the histogram generator to process the image as if it were black and white. This is useful on monochrome images which are tinted because they fail to automatically be seen as black and white by the processor.');

  $output .= '</p>';

  $output .= '<p>';

  $output .= t('The background color field allows you to specify a hexadecimal background color for your histograms. If left blank, a transparent background will be generated. This is useful for situations where the histogram should appear on a patterned background.');

  $output .= '</p>';

  $output .= '<p>';

  $output .= t('Samples of the different histogram options and their results can be seen below.');

  $output .= '</p>';

  $output .= '<h3>';

  $output .= t('Example of sample image and histogram');

  $output .= '</h3>';

  $filepath = drupal_get_path('module', 'histogram') . '/sample.jpg';

  $url = url($filepath);

  $output .= '<img class="sample-image" src="' . $url . '"/>';

  $filepath = drupal_get_path('module', 'histogram') . '/hist_sample_stacked.png';

  $url = url($filepath);

  $output .= '<p>';

  $output .= '<img class="sample-histogram" src="' . $url . '"/>';

  $output .= t('A sample stacked three channel histogram with a #333333 color background.');

  $output .= '</p>';

  $filepath = drupal_get_path('module', 'histogram') . '/hist_sample.png';

  $url = url($filepath);

  $output .= '<div class="sample-bg">';

  $output .= '<p>';

  $output .= '<img class="sample-histogram" src="' . $url . '"/>';

  $output .= t('A sample merged histogram with a transparent background');

  $output .= '</p>';

  $output .= '</div>';

  $filepath = drupal_get_path('module', 'histogram') . '/hist_sample_bw.png';

  $url = url($filepath);

  $output .= '<p>';

  $output .= '<img class="sample-histogram" src="' . $url . '"/>';

  $output .= t('A sample forced single channel histogram with a white background.');

  $output .= '</p>';

  return $output;

}

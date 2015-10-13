<?php
/**
 * @file
 * Theme functions for Views Slideshow: RoyalSlider.
 */

/**
 * Theme function for RoyalSlider Views Slideshow main frame.
 *
 * @see views_slideshow_royalslider_theme()
 */
function theme_views_slideshow_royalslider_main_frame($variables) {
  // We need to re-write the rows into an array the RoyalSlider theme function
  // can handle.
  $items = array();

  for ($i = 0, $n = count($variables['rows']); $i < $n; ++$i) {
    $row = $variables['rows'][$i];
    $view_result = $variables['view']->result[$i];

    $data = NULL;
    // @TODO figure out where the alt info should come from.
    // If no caption field is selected, the alt should already be in the image or link.
    $data['alt'] = '';
    $view = $variables['view'];

    $data['width'] = 0;
    $data['height'] = 0;
    $data['uri'] = '';

    $main_image_field_name = $variables['settings']['main_image'];
    if ($main_image_field_name) {
      $view_field_name = 'field_' . $main_image_field_name;
      $image_render_field = current($view->result[$i]->$view_field_name);
      $image_dimensions = array(
        'width' => $image_render_field['raw']['width'],
        'height' => $image_render_field['raw']['height']
      );
      $image_style = $image_render_field['rendered']['#image_style'];

      // If the image is preloaded, remove the #image_style from its' render array.
      if ($image_render_field['rendered']['#theme'] == 'link') {
        unset($image_render_field['rendered']['#image_style']);
      }

      image_style_transform_dimensions($image_style, $image_dimensions);
      $data['width'] = $image_dimensions['width'];
      $data['height'] = $image_dimensions['height'];
      $data['uri'] = $image_render_field['raw']['uri'];
    }
    // @TODO: what to do if no field was selected?

    // If we haven't found it yet, try to figure it out.
    // @TODO
    $data['row'] = $row;
    $items[] = $data;
  }

  $build = array(
    '#theme' => 'royalslider',
    '#royalslider_id' => $variables['vss_id'],
    '#optionset' => $variables['settings']['optionset'],
    '#skin' => $variables['settings']['skin'],
    '#items' => $items,
  );

  return drupal_render($build);
}
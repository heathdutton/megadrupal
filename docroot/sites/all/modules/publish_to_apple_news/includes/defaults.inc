<?php
/**
 * @file
 * Default component layouts, styles, and text styles
 */

/**
 * Returns default properties for components, layouts, styles, and text styles as an array
 */
function publish_to_apple_news_get_component_defaults() {
  $defaults = array();
  
  $defaults['component_properties'] = array(
    'divider' => array(
      'stroke_color' => '#E6E6E6',
      'stroke_width' => 1,
    ),
    'body' => array(
      'image_column_start' => 3,
      'image_column_span' => 4,
    ),
  );
  $defaults['layouts'] = array(
    'default_banner_advertisement_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 25,
      'margin_bottom' => 25,
    ),
    'default_author_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 10,
      'margin_bottom' => 10,
    ),
    'default_body_layout' => array(
      'column_start' => 0,
      'column_span' => 5,
      'margin_top' => 25,
      'margin_bottom' => 25,
    ),
    'default_caption_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 10,
      'margin_bottom' => 10,
    ),
    'default_container_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 0,
      'margin_bottom' => 25,
    ),
    'default_divider_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 25,
      'margin_bottom' => 25,
    ),
    'default_figure_layout' => array(
      'column_start' => 3,
      'column_span' => 4,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_gallery_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 25,
      'margin_bottom' => 25,
      'ignore_document_margin' => 'left',
    ),
    'default_header_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 0,
      'margin_bottom' => 25,
    ),
    'default_heading1_layout' => array(
      'column_start' => 0,
      'column_span' => 5,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_heading2_layout' => array(
      'column_start' => 0,
      'column_span' => 5,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_heading3_layout' => array(
      'column_start' => 0,
      'column_span' => 5,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_heading4_layout' => array(
      'column_start' => 0,
      'column_span' => 5,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_heading5_layout' => array(
      'column_start' => 0,
      'column_span' => 5,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_heading6_layout' => array(
      'column_start' => 0,
      'column_span' => 5,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_intro_layout' => array(
      'column_start' => 0,
      'column_span' => 5,
      'margin_top' => 25,
      'margin_bottom' => 25,
    ),
    'default_mosaic_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 25,
      'margin_bottom' => 25,
      'ignore_document_margin' => 'left',
    ),
    'default_photo_layout' => array(
      'column_start' => 3,
      'column_span' => 4,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_pullquote_layout' => array(
      'column_start' => 3,
      'column_span' => 4,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_title_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 15,
      'margin_bottom' => 15,
    ),
    'default_section_layout' => array(
      'column_start' => 0,
      'column_span' => 7,
      'margin_top' => 0,
      'margin_bottom' => 25,
    ),
  );
  $defaults['styles'] = array();
  $defaults['text_styles'] = array(
    'default_author_text_style' => array(
      'font_name' => 'AvenirNext-Medium',
      'font_size' => 16,
      'text_color' => '#000',
      'text_alignment' => 'left',
    ),
    'default_body_text_style' => array(
      'font_name' => 'AvenirNext-Regular',
      'font_size' => 16,
      'line_height' => 24,
      'text_color' => '#000',
      'link_style_font_name' => 'AvenirNext-Regular',
      'link_style_font_size' => 16,
      'link_style_text_color' => '#007AFF',
      'dropcap_number_of_lines' => 2,
      'dropcap_number_of_characters' => 1,
    ),
    'default_caption_text_style' => array(
      'font_name' => 'AvenirNext-Medium',
      'font_size' => 14,
      'line_height' => 17,
      'text_color' => '#000',
    ),
    'default_heading1_text_style' => array(
      'font_name' => 'AvenirNext-Bold',
      'font_size' => 24,
      'text_color' => '#000',
    ),
    'default_heading2_text_style' => array(
      'font_name' => 'AvenirNext-Bold',
      'font_size' => 18,
      'text_color' => '#000',
    ),
    'default_heading3_text_style' => array(
      'font_name' => 'AvenirNext-Bold',
      'font_size' => 16,
      'text_color' => '#000',
    ),
    'default_heading4_text_style' => array(
      'font_name' => 'AvenirNext-Bold',
      'font_size' => 14,
      'text_color' => '#000',
    ),
    'default_heading5_text_style' => array(
      'font_name' => 'AvenirNext-Bold',
      'font_size' => 12,
      'text_color' => '#000',
    ),
    'default_heading6_text_style' => array(
      'font_name' => 'AvenirNext-Bold',
      'font_size' => 10,
      'text_color' => '#000',
    ),
    'default_intro_text_style' => array(
      'font_name' => 'AvenirNext-Regular',
      'font_size' => 16,
      'line_height' => 24,
      'text_color' => '#000',
    ),
    'default_pullquote_text_style' => array(
      'font_name' => 'AvenirNext-Bold',
      'font_size' => 48,
      'text_alignment' => 'left',
      'text_color' => '#000',
    ),
    'default_title_text_style' => array(
      'font_name' => 'AvenirNext-Bold',
      'font_size' => 48,
      'text_color' => '#000',
    ),
  );
  
  return $defaults;
}

/**
 * Create default layouts, styles, and text styles for an article type.
 */
function publish_to_apple_news_create_component_defaults($article_tid) {
  $defaults = publish_to_apple_news_get_component_defaults();
  
  if (!empty($defaults['layouts'])) {
    foreach ($defaults['layouts'] as $identifier => $values) {
      $values['tid'] = $article_tid;
      $values['identifier'] = $identifier;
      $layout = entity_create('publish_to_apple_news_component_layout', $values);
      $layout->save();
    }
  }
  
  if (!empty($defaults['text_styles'])) {
    foreach ($defaults['text_styles'] as $identifier => $values) {
      $values['tid'] = $article_tid;
      $values['identifier'] = $identifier;
      $text_style = entity_create('publish_to_apple_news_component_text_style', $values);
      $text_style->save();
    }
  }
}


































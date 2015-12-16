<?php

/**
 * @file
 * Creates a simple default Publish to Apple News article type.
 */

function publish_to_apple_news_create_article_type_from_example($node_type, $article, $folder_path) {
  // Article type
  $article_type = entity_create('publish_to_apple_news_article_type', array(
    'identifier' => '[node:nid]',
    'content_type' => $node_type,
    'description' => t($article['title']),
    'title' => '[node:title]',
    'excerpt' => '[node:summary]',
    'language' => '[node:language]',
    'metadata_thumbnail_url' => '[node:field_image]',
    'metadata_canonical_url' => '[node:url]',
    'metadata_authors' => '',
    'metadata_keywords' => '',
    'layout_columns' => $article['layout']['columns'], //7
    'layout_width' => $article['layout']['width'], //1024
    'layout_margin' => $article['layout']['margin'], //75
    'layout_gutter' => $article['layout']['gutter'], //20
  ));
  $article_type->save();
  
  $lid_lookup = array();
  $sid_lookup = array();
  $tsid_lookup = array();
  // Component layouts
  
  foreach ($article['componentLayouts'] as $id => $layout) {
    $layout_entity = array(
      'tid' => $article_type->tid,
      'identifier' => $id,
    );
    
    if (! empty($layout['columnStart'])) $layout_entity['column_start'] = $layout['columnStart'];
    if (! empty($layout['columnSpan'])) $layout_entity['column_span'] = $layout['columnSpan'];
    if (! empty($layout['margin']) && ! empty($layout['margin']['top'])) $layout_entity['margin_top'] = $layout['margin']['top'];
    if (! empty($layout['margin']) && ! empty($layout['margin']['bottom'])) $layout_entity['margin_bottom'] = $layout['margin']['bottom'];
    if (! empty($layout['contentInset'])) {
      if ($layout['contentInset'] === TRUE) {
        $layout_entity['content_inset_top'] = TRUE;
        $layout_entity['content_inset_right'] = TRUE;
        $layout_entity['content_inset_bottom'] = TRUE;
        $layout_entity['content_inset_left'] = TRUE;
      } elseif ($layout['contentInset'] === FALSE) {
        $layout_entity['content_inset_top'] = FALSE;
        $layout_entity['content_inset_right'] = FALSE;
        $layout_entity['content_inset_bottom'] = FALSE;
        $layout_entity['content_inset_left'] = FALSE;
      } else {
        if (!empty($layout['contentInset']['top']) && $layout['contentInset']['top']) {
          $layout_entity['content_inset_top'] = TRUE;
        }
        if (!empty($layout['contentInset']['right']) && $layout['contentInset']['right']) {
          $layout_entity['content_inset_right'] = TRUE;
        }
        if (!empty($layout['contentInset']['bottom']) && $layout['contentInset']['bottom']) {
          $layout_entity['content_inset_bottom'] = TRUE;
        }
        if (!empty($layout['contentInset']['left']) && $layout['contentInset']['left']) {
          $layout_entity['content_inset_left'] = TRUE;
        }
      }
    }
    if (! empty($layout['ignoreDocumentMargin'])) $layout_entity['ignore_document_margin'] = $layout['ignoreDocumentMargin'] ? 'both' : 'none';
    if (! empty($layout['ignoreDocumentGutter'])) $layout_entity['ignore_document_gutter'] = $layout['ignoreDocumentGutter'] ? 'both' : 'none';
    if (! empty($layout['minimumHeight'])) $layout_entity['minimum_height'] = $layout['minimumHeight'];
    $component_layout = entity_create('publish_to_apple_news_component_layout', $layout_entity);
    $component_layout->save();
    $lid_lookup[$id] = $component_layout->lid;
  }
  
  foreach ($article['componentTextStyles'] as $id => $style) {
    $style_entity = array(
      'tid' => $article_type->tid,
      'identifier' => $id,
    );
    
    if (! empty($style['fontName'])) $style_entity['font_name'] = $style['fontName'];
    if (! empty($style['fontSize'])) $style_entity['font_size'] = $style['fontSize'];
    if (! empty($style['textColor'])) $style_entity['text_color'] = $style['textColor'];
    if (! empty($style['textTransform'])) $style_entity['textTransform'] = $style['textTransform'];
    if (! empty($style['underline']))$style_entity['underline'] = $style['underline']? 1 : 0;
    if (! empty($style['strikethrough'])) $style_entity['strikethrough'] = $style['strikethrough']? 1 : 0;
    if (! empty($style['backgroundColor'])) $style_entity['background_color'] = $style['backgroundColor'];
    if (! empty($style['verticalAlignment'])) $style_entity['vertical_alignment'] = $style['verticalAlignment'];
    if (! empty($style['textAlignment'])) $style_entity['text_alignment'] = $style['textAlignment'];
    if (! empty($style['lineHeight'])) $style_entity['line_height'] = $style['lineHeight'];
    if (! empty($style['hyphenation'])) $style_entity['hyphenation'] = $style['hyphenation'] ? 1 : 0;
    
    if (! empty($style['linkStyle'])) {
      if (! empty($style['linkStyle']['fontName'])) $style_entity['link_style_font_name'] = $style['linkStyle']['fontName'];
      if (! empty($style['linkStyle']['fontSize'])) $style_entity['link_style_font_size'] = $style['linkStyle']['fontSize'];
      if (! empty($style['linkStyle']['textColor'])) $style_entity['link_style_text_color'] = $style['linkStyle']['textColor'];
      if (! empty($style['linkStyle']['underline'])) $style_entity['link_style_underline'] = $style['linkStyle']['underline'] ? 1 : 0;
    }
    
    if (! empty($style['dropCapStyle'])) {
      if (! empty($style['dropCapStyle']['numberOfLines'])) $style_entity['dropcap_number_of_lines'] = $style['dropCapStyle']['numberOfLines'];
      if (! empty($style['dropCapStyle']['numberOfCharacters'])) $style_entity['dropcap_number_of_characters'] = $style['dropCapStyle']['numberOfCharacters'];
      if (! empty($style['dropCapStyle']['fontName'])) $style_entity['dropcap_font_name'] = $style['dropCapStyle']['fontName'];
      if (! empty($style['dropCapStyle']['textColor'])) $style_entity['dropcap_text_color'] = $style['dropCapStyle']['textColor'];
      if (! empty($style['dropCapStyle']['padding'])) $style_entity['dropcap_padding'] = $style['dropCapStyle']['padding'];
      if (! empty($style['dropCapStyle']['backgroundColor'])) $style_entity['dropcap_background_color'] = $style['dropCapStyle']['backgroundColor'];
    }

    $component_text_style = entity_create('publish_to_apple_news_component_text_style', $style_entity);
    $component_text_style->save();
    $tsid_lookup[$id] = $component_text_style->tsid;
  }
  
  foreach ($article['componentStyles'] as $id => $style) {
    $style_entity = array(
      'tid' => $article_type->tid,
      'identifier' => $id,
    );
    
    if (! empty($style['backgroundColor'])) $style_entity['background_color'] = $style['backgroundColor'];
    if (! empty($style['opacity'])) $style_entity['opacity'] = $style['opacity'];
    
    if (! empty($style['border'])) {
      if (! empty($style['border']['all'])) {
        if (! empty($style['border']['all']['color'])) $style_entity['border_color'] = $style['border']['all']['color'];
        if (! empty($style['border']['all']['width'])) $style_entity['border_width'] = $style['border']['all']['width'];
        if (! empty($style['border']['all']['style'])) $style_entity['border_style'] = $style['border']['all']['style'];
      }
      if (! empty($style['border']['top'])) $style_entity['border_top'] = $style['border']['top'] ? 1 : 0;
      if (! empty($style['border']['bottom'])) $style_entity['border_bottom'] = $style['border']['bottom'] ? 1 : 0;
      if (! empty($style['border']['left'])) $style_entity['border_left'] = $style['border']['left'] ? 1 : 0;
      if (! empty($style['border']['right'])) $style_entity['border_right'] = $style['border']['right'] ? 1 : 0;
    }
    
    if (! empty($style['fill'])) {
      if (! empty($style['fill']['URL'])) $style_entity['image_fill_url'] = str_replace('bundle://', $folder_path, $style['fill']['URL']);
      if (! empty($style['fill']['fillMode'])) $style_entity['image_fill_mode'] = $style['fill']['fillMode'];
      if (! empty($style['fill']['attachment'])) $style_entity['image_fill_attachment'] = $style['fill']['attachment'];
      if (! empty($style['fill']['verticalAlignment'])) $style_entity['image_fill_vertical_alignment'] = $style['fill']['verticalAlignment'];
      if (! empty($style['fill']['horizontalAlignment'])) $style_entity['image_fill_horizontal_alignment'] = $style['fill']['horizontalAlignment'];
    }
    
    $component_style = entity_create('publish_to_apple_news_component_style', $style_entity);
    $component_style->save();
    $sid_lookup[$id] = $component_style->sid;
  }
  
  $lookups = array(
    'lid' => $lid_lookup,
    'sid' => $sid_lookup,
    'tsid' => $tsid_lookup,
  );
  
  $parent = array(
    'cid' => 0,
    'type' => '',
  );
  
  foreach ($article['components'] as $index => $component) {
    if ($component['role'] == 'body') {
      if (!empty($article['componentLayouts']['default_photo_layout'])) {
        $photo_layout = $article['componentLayouts']['default_photo_layout'];
        $component['image_column_start'] = $photo_layout['columnStart'];
        $component['image_column_span'] = $photo_layout['columnSpan'];
      }
    }
    
    publish_to_apple_news_create_component_from_example($component, $article_type->tid, $index, $parent, $lookups);
  }
}

function publish_to_apple_news_create_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $new_parent = array(
    'cid' => 0,
    'type' => 'publish_to_apple_news_' . $component['role'] . '_component',
  );
  $function_name = 'publish_to_apple_news_create_' . $component['role'] . '_component_from_example';
  
  if (function_exists($function_name)) {
    $new_parent['cid'] = $function_name($component, $tid, $weight, $parent, $lookups);
    
    if ($new_parent['cid'] && ! empty($component['components'])) {
      foreach ($component['components'] as $index => $child_component) {
        publish_to_apple_news_create_component_from_example($child_component, $tid, $index, $new_parent, $lookups);
      }
    }
  }
  
}

function publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = array(
    'tid' => $tid,
    'parent_cid' => $parent['cid'],
    'parent_type' => $parent['type'],
    'name' => (! empty($component['identifier'])) ? $component['identifier'] : $component['role'] . '-' . $parent['cid'] . '-' . $weight,
    'weight' => $weight,
  );
  
  $entity['identifier'] = $entity['name'];
  
  if (! empty($component['layout']) && is_string($component['layout'])) {
    $layout = $component['layout'];
    $entity['lid'] = (! empty($lookups['lid'][$layout])) ? $lookups['lid'][$layout] : 0;
  }

  if (! empty($component['style']) && is_string($component['style'])) {
    $style = $component['style'];
    $entity['sid'] = (! empty($lookups['sid'][$style])) ? $lookups['sid'][$style] : 0;
  }
  
  if (! empty($component['textStyle']) && is_string($component['textStyle'])) {
    $style = $component['textStyle'];
    $entity['tsid'] = (! empty($lookups['tsid'][$style])) ? $lookups['tsid'][$style] : 0;
  }
  
  if (! empty($component['anchor'])) {
    if (! empty($component['anchor']['targetComponentIdentifier'])) $entity['anchor_identifier'] = $component['anchor']['targetComponentIdentifier'];
    if (! empty($component['anchor']['targetAnchorPosition'])) $entity['anchor_target_position'] = $component['anchor']['targetAnchorPosition'];
    if (! empty($component['anchor']['originAnchorPosition'])) $entity['anchor_origin_position'] = $component['anchor']['originAnchorPosition'];
    if (! empty($component['anchor']['rangeStart'])) $entity['anchor_range_start'] = $component['anchor']['rangeStart'];
    if (! empty($component['anchor']['rangeLength'])) $entity['anchor_range_length'] = $component['anchor']['rangeLength'];
  }
  
  return $entity;
  
}

function publish_to_apple_news_create_banner_advertisement_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['bannerType'])) $entity['banner_type'] = $component['bannerType'];
  
  //don't save component if user left input empty
  if (empty($entity['banner_type'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_ad_component', $entity);
  $component->save();
  
  return $component->cid;
}

function publish_to_apple_news_create_author_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['text'])) $entity['text'] = $component['text'];
  
  if (empty($entity['text'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_author_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_body_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['text'])) $entity['text'] = $component['text'];
  
  if (empty($entity['text'])) {
    return 0;
  }
  
  if (isset($component['image_column_start'])) $entity['image_column_start'] = $component['image_column_start'];
  if (isset($component['image_column_span'])) $entity['image_column_span'] = $component['image_column_span'];
  
  $component = entity_create('publish_to_apple_news_body_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_caption_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['text'])) $entity['text'] = $component['text'];
  
  if (empty($entity['text'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_caption_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_container_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  $component = entity_create('publish_to_apple_news_container_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_divider_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['stroke'])) {
    if (! empty($component['stroke']['color'])) $entity['stroke_color'] = $component['stroke']['color'];
    if (! empty($component['stroke']['width'])) $entity['stroke_width'] = $component['stroke']['width'];
  }
  
  if (empty($entity['stroke_color']) && empty($entity['stroke_width'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_divider_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_figure_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['URL'])) $entity['url'] = $component['URL'];
  if (! empty($component['caption'])) $entity['caption'] = $component['caption'];
  
  if (empty($entity['url'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_figure_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_gallery_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['images_field'])) {
    $entity['images_field'] = $component['images_field'];
  }
  
  if (empty($entity['images_field'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_gallery_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_header_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  $component = entity_create('publish_to_apple_news_header_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_heading_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['size'])) $entity['size'] = $component['size'];
  if (! empty($component['text'])) $entity['text'] = $component['text'];
  
  if (empty($entity['text'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_heading_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_intro_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['text'])) $entity['text'] = $component['text'];
  
  if (empty($entity['text'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_intro_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_mosaic_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['images_field'])) {
    $entity['images_field'] = $component['images_field'];
  }
  
  if (empty($entity['images_field'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_mosaic_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_photo_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['URL'])) $entity['url'] = $component['URL'];
  if (! empty($component['caption'])) $entity['caption'] = $component['caption'];
  
  if (empty($entity['url'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_photo_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_pullquote_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['text'])) $entity['text'] = $component['text'];
  
  if (empty($entity['text'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_pullquote_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_section_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  $component = entity_create('publish_to_apple_news_section_component', $entity);
  $component->save();
  return $component->cid;
}

function publish_to_apple_news_create_title_component_from_example($component, $tid, $weight, $parent, $lookups = array()) {
  $entity = publish_to_apple_news_create_base_component_from_example($component, $tid, $weight, $parent, $lookups);
  
  if (! empty($component['text'])) $entity['text'] = $component['text'];
  
  if (empty($entity['text'])) {
    return 0;
  }
  
  $component = entity_create('publish_to_apple_news_title_component', $entity);
  $component->save();
  return $component->cid;
}
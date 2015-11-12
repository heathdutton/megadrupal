<?php

/**
 * @file
 * Functions for generating a Publish to Apple News article.
 */

/**
 * Generate the json for an Apple News article.
 *
 * @param $node
 *  The fully-loaded node to generate the article from.
 * 
 * @return array
 *  An array representing the entire article, ready to be json_encode'd, or FALSE on failure.
 */
function publish_to_apple_news_generate_article($node) {
  // Get the article type mapped to this node type
  $typeQuery = new EntityFieldQuery();
  $typeResult = $typeQuery->entityCondition('entity_type', 'publish_to_apple_news_article_type')
    ->propertyCondition('content_type', $node->type)
    ->range(0, 1)
    ->execute();
  
  if (!isset($typeResult['publish_to_apple_news_article_type'])) {
    drupal_set_message(t('No article type found for the @node_type node type.', array(
      '@node_type' => $node->type,
    )), 'error');
    
    return FALSE;
  } else {
    $ids = array_keys($typeResult['publish_to_apple_news_article_type']);
    $article_type = entity_load_single('publish_to_apple_news_article_type', $ids[0]);
    $files = array();
    
    $json = publish_to_apple_news_generate_article_top_level($article_type, $node, $files);
    
    $componentLayouts = publish_to_apple_news_generate_component_layouts($article_type);
    $failedStyles = array();
    $componentStyles = publish_to_apple_news_generate_component_styles($article_type, $node, $files, $failedStyles);
    $componentTextStyles = publish_to_apple_news_generate_component_text_styles($article_type);
    $components = publish_to_apple_news_generate_components($article_type, $node, $files, 0, '', $failedStyles);
    
    if (!empty($components)) {
      $json['components'] = $components;
    }
    if (!empty($componentLayouts)) {
      $json['componentLayouts'] = $componentLayouts;
    }
    if (!empty($componentStyles)) {
      $json['componentStyles'] = $componentStyles;
    }
    if (!empty($componentTextStyles)) {
      $json['componentTextStyles'] = $componentTextStyles;
    }
    
    return array(
      'article' => $json,
      'files' => $files,
    );
  }
}

/**
 * Generates the json for the top level of the Apple News article.
 *
 * @param $article_type
 *  The fully-loaded article type.
 * @param $node
 *  The fully-loaded node.
 * @param $files
 *  An array of full file paths to append to.
 *
 * @return array
 *  An array ready to be json_encode'd
 */
function publish_to_apple_news_generate_article_top_level($article_type, $node, &$files) {
  $json = array(
    'version' => APPLE_NEWS_FORMAT_VERSION,
    'identifier' => publish_to_apple_news_process_text_value(variable_get('publish_to_apple_news_article_identifier', '[site:name]--[node:nid]'), array('node' => $node)),
    'language' => publish_to_apple_news_process_text_value($article_type->language, array('node' => $node)),
    'title' => publish_to_apple_news_process_text_value($article_type->title, array('node' => $node)),
    'layout' => array(
      'columns' => (int)$article_type->layout_columns,
      'width' => (int)$article_type->layout_width,
      'margin' => (int)$article_type->layout_margin,
      'gutter' => (int)$article_type->layout_gutter,
    ),
  );
  
  if (!empty($article_type->subtitle)) {
    $json['subtitle'] = publish_to_apple_news_process_text_value($article_type->subtitle, array('node' => $node));
  }
  
  if (!empty($article_type->excerpt)) {
    $json['metadata']['excerpt'] = publish_to_apple_news_process_text_value($article_type->excerpt, array('node' => $node));
  }
  
  if (!empty($article_type->metadata_thumbnail_url)) {
    global $base_url;
    $thumbnailURL = publish_to_apple_news_process_text_value($article_type->metadata_thumbnail_url, array('node' => $node));
    $imageData = publish_to_apple_news_convert_image($thumbnailURL);
    
    if ($imageData) {
      $json['metadata']['thumbnailURL'] = $imageData['bundle'];
      $files[] = $imageData['realpath'];
    }
  }
  
  if (!empty($article_type->metadata_canonical_url)) {
    $canonical_url = publish_to_apple_news_process_text_value($article_type->metadata_canonical_url, array('node' => $node));
    global $base_url;
    $live_base_url = variable_get('publish_to_apple_news_live_base_url', '');
    if ($live_base_url) {
      // Replace w/ live base URL from settings
      $canonical_url = str_replace($base_url, $live_base_url, $canonical_url);
    }
    $json['metadata']['canonicalURL'] = $canonical_url;
  }
  
  if (!empty($article_type->metadata_authors)) {
    $json['metadata']['authors'] = json_decode($article_type->metadata_authors);
  }
  
  if (!empty($article_type->metadata_keywords)) {
    $json['metadata']['keywords'] = json_decode($article_type->metadata_keywords);
  }
  
  return $json;
}

/**
 * Generate the json for all of the components in a given article type.
 *
 * @param $article_type
 *  The fully-loaded article type to get the components from.
 * @param $node
 *  The fully-loaded node to replace tokens with.
 * @param $files
 *  An array of full file paths to append to.
 * @param $parent_cid
 *  The cid of the parent component to get all child components from.
 *  This function calls itself recursively.
 * @param $parent_type
 *  The entity machine name of the parent component's type.
 * @param $failedStyles
 *  An array of styles that were defined, but resolved to empty.
 *  Used to prevent referencing empty styles
 *
 * @return array
 *  An array of components ready to be json_encode'd.
 */
function publish_to_apple_news_generate_components($article_type, $node, &$files, $parent_cid = 0, $parent_type = '', &$failedStyles) {
  $json = array();
  $component_types = publish_to_apple_news_component_types();
  $component_entity_types = array_keys($component_types);
  
  $components = array();
  foreach ($component_entity_types as $component_entity_type) {
    $componentsQuery = new EntityFieldQuery();
    $componentsResult = $componentsQuery->entityCondition('entity_type', $component_entity_type)
      ->propertyCondition('parent_cid', $parent_cid)
      ->propertyCondition('parent_type', $parent_type)
      ->propertyCondition('tid', $article_type->tid)
      ->propertyOrderBy('weight')
      ->execute();
    
    if (!empty($componentsResult[$component_entity_type])) {
      $ids = array_keys($componentsResult[$component_entity_type]);
      foreach ($ids as $id) {
        $component = entity_load_single($component_entity_type, $id);
        $component->entity_type = $component_entity_type;
        $components[$component->weight] = $component;
      }
    }
  }
  
  if (!empty($components)) {
    ksort($components);
  }
  
  foreach ($components as $component) {
    $class = new $component_types[$component->entity_type]($article_type, $component);
    
    $componentJson = $class->generateJson($node, $files, $failedStyles);
    
    if (!empty($componentJson)) {
      if ($class->acceptsComponents) {
        $childComponents = publish_to_apple_news_generate_components($article_type, $node, $files, $component->cid, $component->entity_type, $failedStyles);
        if (!empty($childComponents)) {
          $componentJson['components'] = $childComponents;
        }
      }
      
      if (array_values($componentJson) !== $componentJson) {
        // Associative array - just one component
        $json[] = $componentJson;
      } else {
        // Sequential array - The component has generated multiple components - see body component
        foreach ($componentJson as $generatedComponent) {
          $json[] = $generatedComponent;
        }
      }
    }
  }
  
  return $json;
}

/**
 * Generate the json for the article's componentLayouts.
 *
 * @param $article_type
 *  The fully-loaded article type to get the layouts from.
 *
 * @return array
 *  An array representation of the article's componentLayouts, ready to be json_encode'd.
 */
function publish_to_apple_news_generate_component_layouts($article_type) {
  $json = array();
  
  $layoutsQuery = new EntityFieldQuery();
  $layoutsResult = $layoutsQuery->entityCondition('entity_type', 'publish_to_apple_news_component_layout')
    ->propertyCondition('tid', $article_type->tid)
    ->execute();
  
  if (isset($layoutsResult['publish_to_apple_news_component_layout'])) {
    $lids = array_keys($layoutsResult['publish_to_apple_news_component_layout']);
    $layouts = entity_load('publish_to_apple_news_component_layout', $lids);
    
    foreach ($layouts as $layout) {
      $json_layout = array();
      
      if (!empty($layout->column_start)) {
        $json_layout['columnStart'] = (int)$layout->column_start;
      }
      if (!empty($layout->column_span)) {
        $json_layout['columnSpan'] = (int)$layout->column_span;
      }
      if (!empty($layout->margin_top) || !empty($layout->margin_bottom)) {
        $json_layout['margin'] = array();
        if (!empty($layout->margin_top)) {
          $json_layout['margin']['top'] = (int)$layout->margin_top;
        }
        if (!empty($layout->margin_bottom)) {
          $json_layout['margin']['bottom'] = (int)$layout->margin_bottom;
        }
      }
      $json_layout['contentInset'] = array(
        'top' => ($layout->content_inset_top) ? true : false,
        'right' => ($layout->content_inset_right) ? true : false,
        'bottom' => ($layout->content_inset_bottom) ? true : false,
        'left' => ($layout->content_inset_left) ? true : false,
      );
      if (!empty($layout->ignore_document_margin)) {
        $json_layout['ignoreDocumentMargin'] = $layout->ignore_document_margin;
      }
      if (!empty($layout->ignore_document_gutter)) {
        $json_layout['ignoreDocumentGutter'] = $layout->ignore_document_gutter;
      }
      if (!empty($layout->minimum_height)) {
        $json_layout['minimumHeight'] = $layout->minimum_height;
      }
      
      if (!empty($json_layout)) {
        $json[$layout->identifier] = $json_layout;
      }
    }
  }
  
  return $json;
}

/**
 * Generate the json for the article's componentStyles.
 *
 * @param $article_type
 *  The fully-loaded article type to get the styles from.
 * @param $node
 *  The fully-loaded node to replace tokens with.
 * @param $files
 *  An array of full file paths to append to.
 * @param $failedStyles
 *  If a style resolves to empty, add to this array so components won't reference them.
 *
 * @return array
 *  An array representation of the article's componentStyles, ready to be json_encode'd.
 */
function publish_to_apple_news_generate_component_styles($article_type, $node, &$files, &$failedStyles) {
  $json = array();
  
  $stylesQuery = new EntityFieldQuery();
  $stylesResult = $stylesQuery->entityCondition('entity_type', 'publish_to_apple_news_component_style')
    ->propertyCondition('tid', $article_type->tid)
    ->execute();
  
  if (isset($stylesResult['publish_to_apple_news_component_style'])) {
    $sids = array_keys($stylesResult['publish_to_apple_news_component_style']);
    $styles = entity_load('publish_to_apple_news_component_style', $sids);
    
    foreach ($styles as $style) {
      $jsonStyle = array();
      
      //$json[$style->identifier] = array();
      
      if (!empty($style->background_color)) {
        $jsonStyle['backgroundColor'] = $style->background_color;
      }
      
      if (!empty($style->opacity)) {
        $jsonStyle['opacity'] = (int)$style->opacity;
      }
        
      if (!empty($style->image_fill_url)) {
        $imageURL = publish_to_apple_news_process_text_value($style->image_fill_url, array('node' => $node));
        $imageData = publish_to_apple_news_convert_image($imageURL);
        
        if ($imageData) {
          $fill = array();
          
          if (!empty($style->image_fill_attachment)) {
            $fill['attachment'] = $style->image_fill_attachment;
          }
          
          if (!empty($style->image_fill_mode)) {
            $fill['fillMode'] = $style->image_fill_mode;
          }
          
          if (!empty($style->image_fill_vertical_alignment)) {
            $fill['verticalAlignment'] = $style->image_fill_vertical_alignment;
          }
          
          if (!empty($style->image_fill_horizontal_alignment)) {
            $fill['horizontalAlignment'] = $style->image_fill_horizontal_alignment;
          }
          
          $jsonStyle['fill'] = array(
            'type' => 'image',
            'URL' => $imageData['bundle'],
          ) + $fill;
          
          if (!in_array($imageData['realpath'], $files)) {
            $files[] = $imageData['realpath'];
          }
        }
      }
      
      if (!empty($style->border_color)) {
        $jsonStyle['border'] = array(
          'all' => array(
            'color' => $style->border_color,
            'width' => $style->border_width,
            'style' => $style->border_style,
          ),
          'top' => ($style->border_top) ? TRUE : FALSE,
          'bottom' => ($style->border_bottom) ? TRUE : FALSE,
          'left' => ($style->border_left) ? TRUE : FALSE,
          'right' => ($style->border_right) ? TRUE : FALSE,
        );
      }
      
      if (!empty($jsonStyle)) {
        $json[$style->identifier] = $jsonStyle;
      } else {
        $failedStyles[] = $style->identifier;
      }
    }
  }
  
  return $json;
}

/**
 * Generate the json for the article's componentTextStyles.
 *
 * @param $article_type
 *  The fully-loaded article type to get the text styles from.
 *
 * @return array
 *  An array representation of the article's componentTextStyles, ready to be json_encode'd.
 */
function publish_to_apple_news_generate_component_text_styles($article_type) {
  $json = array();
  
  $textStylesQuery = new EntityFieldQuery();
  $textStylesResult = $textStylesQuery->entityCondition('entity_type', 'publish_to_apple_news_component_text_style')
    ->propertyCondition('tid', $article_type->tid)
    ->execute();
  
  if (isset($textStylesResult['publish_to_apple_news_component_text_style'])) {
    $tsids = array_keys($textStylesResult['publish_to_apple_news_component_text_style']);
    $text_styles = entity_load('publish_to_apple_news_component_text_style', $tsids);
    
    foreach ($text_styles as $text_style) {
      $json[$text_style->identifier] = array(
        'underline' => ($text_style->underline) ? TRUE : FALSE,
        'strikethrough' => ($text_style->strikethrough) ? TRUE : FALSE,
        'hyphenation' => ($text_style->hyphenation) ? TRUE : FALSE,
      );
      
      if (!empty($text_style->vertical_alignment)) {
        $json[$text_style->identifier]['verticalAlignment'] = $text_style->vertical_alignment;
      }
      
      if (!empty($text_style->text_alignment)) {
        $json[$text_style->identifier]['textAlignment'] = $text_style->text_alignment;
      }
      
      if (!empty($text_style->font_name)) {
        $json[$text_style->identifier]['fontName'] = $text_style->font_name;
      }
      
      if (!empty($text_style->font_size)) {
        $json[$text_style->identifier]['fontSize'] = (int)$text_style->font_size;
      }
      
      if (!empty($text_style->line_height)) {
        $json[$text_style->identifier]['lineHeight'] = (int)$text_style->line_height;
      }
      
      if (!empty($text_style->vertical_alignment)) {
        $json[$text_style->identifier]['verticalAlignment'] = $text_style->vertical_alignment;
      }
      
      if (!empty($text_style->text_alignment)) {
        $json[$text_style->identifier]['textAlignment'] = $text_style->text_alignment;
      }
      
      if (!empty($text_style->text_color)) {
        $json[$text_style->identifier]['textColor'] = $text_style->text_color;
      }
      
      if (!empty($text_style->text_transform)) {
        $json[$text_style->identifier]['textTransform'] = $text_style->text_transform;
      }
      
      if (!empty($text_style->background_color)) {
        $json[$text_style->identifier]['backgroundColor'] = $text_style->background_color;
      }
      
      if (!empty($text_style->tracking)) {
        $json[$text_style->identifier]['tracking'] = (float)$text_style->tracking;
      }
      
      if (!empty($text_style->link_style_font_name)) {
        $link_style = array(
          'underline' => ($text_style->link_style_underline) ? true : false,
        );
        
        if (!empty($text_style->link_style_font_name)) {
          $link_style['fontName'] = $text_style->link_style_font_name;
        }
        if (!empty($text_style->link_style_font_size)) {
          $link_style['fontSize'] = (int)$text_style->link_style_font_size;
        }
        
        if (!empty($text_style->link_style_text_color)) {
          $link_style['textColor'] = $text_style->link_style_text_color;
        }
        
        $json[$text_style->identifier]['linkStyle'] = $link_style;
      }
      
      if (!empty($text_style->dropcap_number_of_lines)) {
        $dropcap_style = array();
        
        if (!empty($text_style->dropcap_number_of_lines)) {
          $dropcap_style['numberOfLines'] = (int)$text_style->dropcap_number_of_lines;
        }
        if (!empty($text_style->dropcap_number_of_characters)) {
          $dropcap_style['numberOfCharacters'] = (int)$text_style->dropcap_number_of_characters;
        }
        if (!empty($text_style->dropcap_font_name)) {
          $dropcap_style['fontName'] = $text_style->dropcap_font_name;
        }
        if (!empty($text_style->dropcap_text_color)) {
          $dropcap_style['textColor'] = $text_style->dropcap_text_color;
        }
        if (!empty($text_style->dropcap_padding)) {
          $dropcap_style['padding'] = (int)$text_style->dropcap_padding;
        }
        if (!empty($text_style->dropcap_background_color)) {
          $dropcap_style['backgroundColor'] = $text_style->dropcap_background_color;
        }
        
        if (!empty($dropcap_style)) {
          $json[$text_style->identifier]['dropCapStyle'] = $dropcap_style;
        }
      }
    }
  }
  
  return $json;
}

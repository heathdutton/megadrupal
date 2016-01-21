<?php

/**
 * @file
 * Token callbacks for the schemaorg_tokens module.
 */

/**
 * Implements hook_token_info().
 *
 * @ingroup schemaorg_tokens
 */
function schemaorg_tokens_token_info() {

  // Node tokens.
  $info['tokens']['node']['schemaorg-image'] = array(
      'name' => t('Image'),
      'description' => t("URL of an image of the node."),
  );
  $info['tokens']['node']['schemaorg-images'] = array(
      'name' => t('All Images section'),
      'description' => t("Array of URL's of all images of the node."),
      'type' => 'array'
  );
  $info['tokens']['node']['schemaorg-description'] = array(
      'name' => t('Description'),
      'description' => t("A short description of the node."),
  );
  $info['tokens']['node']['schemaorg-url'] = array(
      'name' => t('URL'),
      'description' => t("URL in the node."),
  );
  
  $styles = image_styles();
  foreach ($styles as $style => $desc) {
    $info['tokens']['node']['schemaorg-images-'.$style] = array(
        'name' => t('All Images section of style '. $style),
        'description' => t("Array of URL's of all images of the node of style ".$style),
        'type' => 'array'
    );
  }
  
  return $info;
}

/**
 * Implements hook_tokens().
 *
 * @ingroup schemaorg_tokens
 */
function schemaorg_tokens_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];
    // Extracts relevant values from the product entity using Entity API wrapper.
    if(isset(entity_metadata_wrapper('node', $node)->field_product)){
      $commerce_price = entity_metadata_wrapper('node', $node)->field_product[0]->commerce_price->value();
      $commerce_image = entity_metadata_wrapper('node', $node)->field_product[0]->field_images->value();
      if(!empty($commerce_image)){
        $uri = $commerce_image[0]['uri'];
        $output = file_create_url($uri);
        if(isset($tokens['schemaorg-image'])){
          $replacements[$tokens['schemaorg-image']] = $sanitize ? filter_xss($output) : $output;
        }
        
        $images_array = array();
        $styles = image_styles();
        $images_style_array = array();
        foreach ($commerce_image as $file_value) {
          $uri = $file_value['uri'];
          $images_array[]= file_create_url($uri);
          foreach ($styles as $style => $desc) {
			$images_style_array[$style][] = image_style_url($style, $uri);
		  }
        }

        if(isset($tokens['schemaorg-images'])){
          $replacements[$tokens['schemaorg-images']] = token_render_array($images_array, array('sanitize'=>FALSE));
        }
        
        foreach ($styles as $style => $desc) {
		  if(isset($tokens['schemaorg-images-'.$style])){
			$replacements[$tokens['schemaorg-images-'.$style]] = token_render_array($images_style_array[$style], array('sanitize'=>FALSE));
		  }
		}
        
        if ($image_tokens = token_find_with_prefix($tokens, 'schemaorg-images')) {
          $replacements += token_generate('array', $image_tokens, array('array' => $images_array), $options);
        }
        foreach ($styles as $style => $desc) {
		  if ($image_tokens = token_find_with_prefix($tokens, 'schemaorg-images-'.$style)) {
			$replacements += token_generate('array', $image_tokens, array('array' => $images_style_array[$style]), $options);
		  }
		}
      }
    }
    $mapping = rdf_mapping_load("node", $node->type);
    foreach ($mapping as $field_key => $schema_value) {
      if(isset($schema_value['predicates']) && isset($schema_value['predicates'][0])){
        switch ($schema_value['predicates'][0]){
          case 'schema:image':
            if(isset($node->{$field_key}[$node->language])){
              $field_image = $node->{$field_key}[$node->language];
            }else{
              $field_image = $node->{$field_key}['und'];
            }
            if(isset($field_image[0]['uri'])){
              $uri = $field_image[0]['uri'];
              $output = file_create_url($uri);
              if(isset($tokens['schemaorg-image'])){
                $replacements[$tokens['schemaorg-image']] = $sanitize ? filter_xss($output) : $output;
              }
            }

            $images_array = array();
            $styles = image_styles();
            $images_style_array = array();
            foreach ($field_image as $file_value) {
              if(isset($file_value['uri'])){
                $uri = $file_value['uri'];
                $path = file_create_url($uri);
                $images_array[]= $path;
                foreach ($styles as $style => $desc) {
                  $images_style_array[$style][] = image_style_url($style, $uri);
                }
              }
            }

            if(isset($tokens['schemaorg-images'])){
              $replacements[$tokens['schemaorg-images']] = token_render_array($images_array, array('sanitize'=>FALSE));
            }
            
            foreach ($styles as $style => $desc) {
              if(isset($tokens['schemaorg-images-'.$style])){
                $replacements[$tokens['schemaorg-images-'.$style]] = token_render_array($images_style_array[$style], array('sanitize'=>FALSE));
              }
            }
            
            if ($image_tokens = token_find_with_prefix($tokens, 'schemaorg-images')) {
              $replacements += token_generate('array', $image_tokens, array('array' => $images_array), $options);
            }
            foreach ($styles as $style => $desc) {
              if ($image_tokens = token_find_with_prefix($tokens, 'schemaorg-images-'.$style)) {
                $replacements += token_generate('array', $image_tokens, array('array' => $images_style_array[$style]), $options);
              }
            }

            break;
          case 'schema:summary':
            if(isset($node->{$field_key}[$node->language])){
              $output = $node->{$field_key}[$node->language][0]['value'];
            }else{
              $output = $node->{$field_key}['und'][0]['value'];
            }
            if(isset($tokens['schemaorg-description'])){
              $replacements[$tokens['schemaorg-description']] = $sanitize ? filter_xss($output) : $output;
            }
            break;
          case 'schema:url':
            if(isset($node->{$field_key})){
              if(isset($node->{$field_key}[$node->language])){
                $output = $node->{$field_key}[$node->language][0]['url'];
              }else{
                $output = $node->{$field_key}['und'][0]['url'];
              }
            }
            if(empty($output)){
              $output = url("node/".$node->nid, array('absolute' => true));
            }
            if(isset($tokens['schemaorg-url'])){
              $replacements[$tokens['schemaorg-url']] = $sanitize ? filter_xss($output) : $output;
            }
            break;
        }
      }
    }
  }

  return $replacements;
}

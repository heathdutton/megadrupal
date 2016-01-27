<?php
/**
 * Remove the div wrappers around images embedded in the wysiwyg vai the media module.
 * @param $element
 * @param $tag_info
 * @param $settings
 */
function feature_wysiwyg_editor_media_wysiwyg_token_to_markup_alter(&$element, $tag_info, $settings) {


  if (isset($element['content']) && $element['content']['#bundle'] == 'image') {
    // remove the container div around images
    unset( $element['content']['#type'] );
  }
}





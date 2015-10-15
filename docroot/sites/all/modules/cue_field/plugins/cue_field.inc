<?php

/**
 * @file
 * RichMedia plugin for Cue Player.
 */

class CueFieldRichMedia extends SharedContentRichMediaBase {

  /**
   * Overrides getCssFiles().
   */
  public function getInlineJs($index, $view_mode, $langcode) {
    $js = array();
    if ($index->entity_type == 'field_collection_item') {
      $field_collection = entity_load_single('field_collection_item', $index->entity_id);
      $host_entity = $field_collection->hostEntity();
      list(, , $host_bundle) = entity_extract_ids($field_collection->hostEntityType(), $host_entity);
      list(, , $bundle) = entity_extract_ids('field_collection_item', $field_collection);
      $field_instance = field_info_instance($field_collection->hostEntityType(), $bundle, $host_bundle);
      if ($start = field_get_items('field_collection_item', $field_collection, CUE_FIELD_FIELD_NAME_START)) {
        if ($field_instance && $field_instance['display']['default']['type'] == 'cue_field_jplayer') {
          $js[] = 'window.setTimeout( function() {
                   jQuery("#sharedcontent-' . $index->uuid . ' .jp-jplayer").jPlayer("pause", ' . $start[0]['value'] . ');
                 }, 2000);';
        }
        elseif ($field_instance && $field_instance['display']['default']['type'] == 'cue_field_jwplayer') {
          $js[] = 'window.setTimeout( function() {
                   var id = jQuery("#sharedcontent-' . $index->uuid . ' div.jwplayer-video > div").attr("id");
                   var first = true;
                   jwplayer(id).onPlay(function () { if (first) { jwplayer(id).pause(); first = false;} });
                   jwplayer(id).seek(' . $start[0]['value'] . ');
                 }, 2000);';
        }
      }
    }
    return $js;
  }
}

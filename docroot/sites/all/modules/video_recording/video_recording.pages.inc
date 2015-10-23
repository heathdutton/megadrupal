<?php

/**
 * Video Recording view callback.
 */
function video_recording_view($video_recording) {
  drupal_set_title(entity_label('video_recording', $video_recording));
  return entity_view('video_recording', array(entity_id('video_recording', $video_recording) => $video_recording), 'full');
}

<div class="agenda-chapters">


  <div class="item-list"><ul>
<?php
//dsm($events);
foreach($events as $event) {

// @TODO: the array should be cleaned before passing it to the tpl
$id = $event->media_event_id;
$title = $event->field_media_event_title[LANGUAGE_NONE][0]['value'];
$start = $event->field_media_event_start[LANGUAGE_NONE][0]['value'];
$fid = $event->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id'];
?>
  <li><a href="#" data-popcorn-chapter-source="<?php print $fid ?>" data-popcorn-start="<?php print $start ?>" data-popcorn-title="test" data-popcorn-type="cue_field_popcornjs"  class="active agenda-chapter"><?php print $title ?></a></li>
<?php } ?>
  </ul></div>
</div>

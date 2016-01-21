<div class="abs-verse">
  <h3 class="reference"><a href="?viewid=<?php echo $passage->chapter_id; ?>#<?php echo $passage->chapter_id . '.' . $passage->start_verse; ?>"><?php print $passage->display;?> (<?php
		echo $passage->version;
	?>)</a></h3>
  <div class="abs-verse-text">
    <?php echo str_replace( '&amp;', '&', $passage->text_preview );?>
  </div>
</div>
<?php
/**
 * @file
 * This template handles the layout of transcript display.
 */
?>
<div class="transcript">
	<div class="video-iframe">
	<iframe id="transcript-youtube-video" type="text/html" src="https://www.youtube.com/embed/<?php print $video_id;?>?enablejsapi=1&controls=1&modestbranding=1&showinfo=0&wmode=transparent&autoplay=<?php echo $auto_play; ?>&rel=0" frameborder="0" height="<?php print $height; ?>" width="<?php print $width; ?>" alt="" title=""></iframe>
	</div>

    <div><?php print t('View Transcript'); ?></div>
	<div class="video-transcript">	
	<?php foreach($transcript as $row): ?>
	<p id="<?php echo $row['id']; ?>" class="transcript-tracks"><span><?php echo $row['minute'] . ':' . $row['seconds'] . ' '; ?></span> <?php echo ' ' . $row['txt']; ?></p>
	<?php endforeach; ?>
	</div>
</div>

<center><b><?php print $show['cleantitle'] ?></b><br />
<?php if ($show['starttimestamp']) { ?>
<?php print date("g:iA", $show['starttimestamp']) ?> - <?php print date("g:iA", $show['endtimestamp']) ?><br />
<img src="<?php print $show['thumbnail'] ?>" class="now-playing-thumb">
<?php } ?>
</center>
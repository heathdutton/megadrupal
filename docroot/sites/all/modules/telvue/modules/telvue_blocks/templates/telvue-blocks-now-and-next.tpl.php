<div class="now-next-airing"><b><?php print $now['cleantitle'] ?></b><br />
<?php if ($now['starttimestamp']) { ?>
<?php print date("g:iA", $now['starttimestamp']) ?> - <?php print date("g:iA", $now['endtimestamp']) ?> <br />
<img src="<?php print $now['thumbnail'] ?>" class="now-playing-thumb">
<?php } ?>
<?php if ($next['starttimestamp']) { ?>
<br />Playing Next: <b><?php print $next['cleantitle'] ?></b>
<?php } ?>
</div>
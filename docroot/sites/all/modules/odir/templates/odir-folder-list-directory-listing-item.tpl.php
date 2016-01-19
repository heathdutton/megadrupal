<?php
/**
 * @file
 * Display an individual directory entry
 */
?>
<div
  id="odir_dirlist_details_<?php print $filecounter ?>"
  class="<?php if ($mayenter) :?>odir_mayenter <?php if ($maymove) :?>draggable_directory<?php endif?><?php else: ?>odir_maynotenter<?php endif ?>"
  file="<?php print $path ?>"
>

<?php if ($mayenter) :?><a href="<?php print $href ?>"><?php endif ?>
<?php print $filename ?>
<?php if ($mayenter) :?></a><?php endif ?>

</div>

<div class="abs-verse">
  <h3 class="reference"><a href="?viewid=<?php echo $verse->parent; ?>#<?php echo $verse->id; ?>"><?php print $verse->reference;?> (<?php
	  echo substr( $verse->version, 1, 3 );
  ?>)</a></h3>
  <div class="abs-verse-text">
    <?php print str_replace( '&amp;', '&', $verse->text );?>
  </div>
  <?php
  $safe_id = str_replace('.','',str_replace(':','',$verse->id));
  $versions = explode(',', $version);
  if ( count($versions) > 1 ):
  ?>
  <div class="abs-verses-versions-trigger" verse_id="<?php print $verse->id;?>" versions="<?php echo $version; ?>" id="trigger<?php print $safe_id;?>">
    <div class="expandtext">Expand to show <?php echo $verse->reference; ?> in all selected Bible Versions</div>
    <div class="collapsetext" style="display: none;">[Hide]</div>
  </div>
  <span id="throbber<?php print $safe_id;?>"></span>
  <div class="abs-verse-versions" style="display:none" id="<?php print $safe_id;?>"></div><!-- // abs-verse-versions -->
  <?php endif; ?>
</div>
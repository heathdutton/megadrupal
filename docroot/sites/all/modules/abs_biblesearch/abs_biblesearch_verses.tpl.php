<div id="abs-biblesearch-browse">
  <div class="passageheader">
    <h1><?php echo $book;?> <?php echo $chapter;?> (<?php echo $version; ?>)</h1>
    
      <?php if ( $next && $next->id != '' ) { ?><a id="nextchapter" href="?viewid=<?php echo $next->id; ?>"><?php echo $next->name; ?> &raquo;</a><?php } ?>
      <?php if ( $prev && $prev->id != '' ) { ?><a id="prevchapter" href="?viewid=<?php echo $prev->id; ?>">&laquo; <?php echo $prev->name; ?></a><?php } ?>
    
    <div class="clear: both;"></div>
  </div>

  <div class="abs-verse">
  <?php
    /*
    foreach ($verses as $verse) {
      echo '<a name="' . $verse->id . '" id="tag' .$verse->id . '"></a>' . str_replace( '&amp;', '&', str_replace( '</li></ul>', '', str_replace( '<ul><li>', '', $verse->text ) ) );
    }
    */
    print $verses;
  ?>
  </div>

  <div class="passagefooter">
    <?php if ( $next && $next->id != '' ) { ?><a id="nextchapter" href="?viewid=<?php echo $next->id; ?>"><?php echo $next->name; ?> &raquo;</a><?php } ?>
    <?php if ( $prev && $prev->id != '' ) { ?><a id="prevchapter" href="?viewid=<?php echo $prev->id; ?>">&laquo; <?php echo $prev->name; ?></a><?php } ?>
    <div class="clear: both;"></div>
  </div>

  <div class="abs-biblesearch-versions-copyright">
    <?php 
    $verse = new ABS_Verse(abs_biblesearch_get_api());
    try {
      $verse->setFromReceivedChapterID( $_REQUEST['viewid'] );
      foreach ( $versionlist as $versiont ) {
        if ( $version == $versiont->id ) {
          print "<div class=\"version\"><a href=\"http://biblesearch.americanbible.org/"
            . $verse->getVersion() . "/" . $verse->getBook() . "/" . $verse->getChapter() . "/\" target=\"_blank\">"
            . $verse->getVersion() . " " . $verse->getBook() . " " . $verse->getChapter()
            . "</a>: Scripture taken from " . $versiont->id . ': ' . $versiont->copyright . '</div>';
          break;
        }
      }
    }
    catch ( Exception $ex ) {
    }
    ?>
  </div>
</div>

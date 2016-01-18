<div id="abs-biblesearch-results">
  <div id="abs-bible-search-meta">
    <div id="metainfo">
      <p id="metafor"><?php echo t( "You searched for" ); ?>: <?php echo filter_xss( $_REQUEST['keys'] ); ?></p>
      <p id="metacharacteristics"><?php echo t( "YOUR SEARCH" ); ?>: <strong><?php echo filter_xss( $_REQUEST['keys'] ); ?></strong> <?php echo t( "Versions" ); ?>: <strong><?php 
      if ( is_array( $_REQUEST['versions'] ) ) {
        foreach( $_REQUEST['versions'] as $version ) {
          echo filter_xss( "$version " );
        }
      }
      ?></strong></p>
    </div>
    
    <div class="clear"></div>
  </div>

  <div id="abs-search-results">
  <?php
  foreach ($passages as $passage) {
    echo theme( 'abs_biblesearch_verse_passage', array(
      'passage' => $passage,
      'version' => $version
      ) );
  }
  ?>
  </div>
</div>

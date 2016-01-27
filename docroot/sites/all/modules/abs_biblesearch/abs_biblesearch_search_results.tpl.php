<div id="abs-biblesearch-results">
  <div id="abs-bible-search-meta">
    <div id="metainfo">
      <p id="metafor"><?php echo t( "You searched for" ); ?>: <?php echo filter_xss( $_REQUEST['keys'] ); ?></p>
      <p id="metanumbers"><?php echo t( "We found" ); ?> <strong><?php echo $total; ?></strong> <?php echo t( "results for your search" ); ?>.  <strong><?php echo t( "Showing" ); ?> <?php
        if ( $offset == '' ) {
          $offset = 1;
        }
        $start = ( $offset - 1 ) * $limit + 1;
        $end = $start + $limit - 1;
        if ( $total < $end ) {
          $end = $total;
        }
        echo "$start - $end";
      ?></strong>.</p>
      <p id="metacharacteristics"><?php echo t( "YOUR SEARCH" ); ?>: <strong><?php echo filter_xss( $_REQUEST['keys'] ); ?></strong> <?php echo t( "Versions" ); ?>: <strong><?php echo $version; ?></strong></p>
    </div>
    
    <div id="metachange">
      <form id="resultsperpage" action="" method="get"><?php echo abs_biblesearch_build_hidden_inputs(); ?><?php echo t( "Results per page" ); ?>:
        <select name="limit" id="limit">
          <option value="10" <?php if ( isset( $_REQUEST['limit'] ) && $_REQUEST['limit'] == 10 ) { echo 'selected'; } ?>>10</option>
          <option value="20" <?php if ( isset( $_REQUEST['limit'] ) && $_REQUEST['limit'] == 20 ) { echo 'selected'; } ?>>20</option>
          <option value="30" <?php if ( isset( $_REQUEST['limit'] ) && $_REQUEST['limit'] == 30 ) { echo 'selected'; } ?>>30</option>
        </select>
      </p></form>
      <p><?php echo t( "Sort By" ); ?>: 
      <br /><strong><?php
        if ( ! isset( $_REQUEST['sort_order'] ) || $_REQUEST['sort_order'] == 'relevance' ) {
          echo t( 'Relevance' );
        } else {
          echo '<a href="/?' .abs_biblesearch_build_params('sort_order') . '&sort_order=relevance">' . t( "Relevance" ) . '</a>';
        }
      ?> | <?php
        if ( isset( $_REQUEST['sort_order'] ) && $_REQUEST['sort_order'] == 'canonical' ) {
          echo t( 'Book Order' );
        } else {
          echo '<a href="?' . abs_biblesearch_build_params('sort_order') . '&sort_order=canonical">' . t( "Book Order" ) . '</a>';
        }
      ?></strong></p>
    </div>
    
    <div class="clear"></div>
  </div>

  <div class="abs-paging">
    <?php print $pages;?>
  </div>
  <div id="abs-search-results">
  <?php
  foreach ($verses as $verse) {
      print theme('abs_biblesearch_verse', array(
        'verse' => $verse,
        'version' => $version
        ) );
  }
  ?>
  </div>
  <div class="abs-paging">
    <?php print $pages;?>
  </div>
  <div class="abs-biblesearch-versions-copyright">
    <?php 
      foreach ( $versionlist as $version ) {
        print "<div class=\"version\">" . $version->id . ': ' . $version->copyright . '</div>';
      }
    ?>
  </div>
</div>

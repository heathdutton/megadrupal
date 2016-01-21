  <div id="searchformheader">
    <div id="searchlogo"></div>
    <div id="searchinfolinks">
      <a href="#searchtipscontent" id="searchtips">Search Tips</a> |
      <a href="/abs_biblesearch/ajax/versions" id="browsethebible">Browse the Bible</a>
    </div>

    <div style="display: none;">
      <div id="searchtipscontent">
        <h2><?php echo t("Search Tips"); ?></h2>

        <?php echo t("<h3>That verse that goes something like...</h3>
          <p>You can search on the words you remember from a verse (even if you don't know the version that was quoted). For instance, if you heard a verse that said \"God works for the good of those who love him,\" but you didn't know where that was quoted from, then you could:</p>

          <ul>
            <li>Click on \"all versions\"</li>
            <li>Then type in the words you do remember, e.g., \"God works for good\"</li>
          </ul>

          <p>Romans 8:28 will show up as one of the passages, and from there you can continue to dig into that passage.</p>

        <h3>One term, One Place</h3>
          <p>Sometimes you're looking for a very specific term in a specific book or group of books. For instance, if you want to see all the references to Jesus in the book of Romans, or all references to marriage in the New Testament, you could type in:</p>

          <ul>
            <li>Jesus in: Romans</li>
            <li>Marriage in: NT</li>
          </ul>

          <p>That will immediately narrow your search results to just the books you're interested in.</p>

        <h3>I know exactly what I want</h3>
          <p>If you know the passage you want to study, you can type in any of the standard book names and abbreviations along with the chapter and reference. You can search on a single verse, a span, or even a group of different verse spans such as:</p>

          <ul>
            <li>Rom 3:23</li>
            <li>Rev 4:22-25</li>
            <li>Romans 8:20, 35-39</li>
          </ul>

        <h3>Pick a Version</h3>
          <p>When searching, you can pick the versions you want to focus on. If you don't know where to start, \"All versions\" is an easy way to search for a term or phrase without having to know which version of the Bible has those exact words.</p>

          <p>If you're more comfortable with particular versions, though, simply select your favorite versions in the \"Specific Versions\" list.</p>" ); ?>
      </div>
    </div>
  </div>

  <div id="searchform">
    <div id="selectionmarker" class="<?php
      if (  count( $form['versions']['#options'] ) == count( $form['versions']['#value'] ) ) {
        echo 'all';
      } else {
        echo 'specific';
      }
    ?>"></div>
    <div id="searchallversions"><div class="text">All Versions</div></div>
    <div id="searchsomeversions"><div class="text">Specific Version</div></div>
    <div id="keyscontainer"><?php echo drupal_render( $form['keys'] ); ?></div>
    <div id="searchformsubmit"><span class="text"><input type="submit" value="<?php echo t( "Search" );?>" /></span></div>
  </div>

  <div id="versions">
    <?php echo drupal_render( $form['versions'] ); ?>
    <div class="clear"></div>
  </div>

<?php /*
  <input type="hidden" name="form_build_id" id="<?php echo $form['#build_id']; ?>" value="<?php echo $form['#build_id']; ?>"  />
<?php if (isset($form['form_token']) && isset($form['form_token']['#id'])) { ?>
  <input type="hidden" name="form_token" id="<?php echo $form['form_token']['#id']; ?>" value="<?php echo $form['form_token']['#default_value']; ?>"  />
<?php } ?>
  <input type="hidden" name="form_id" id="<?php echo $form['form_id']['#id']; ?>" value="<?php echo $form['form_id']['#value']; ?>"  />
<?php */?>

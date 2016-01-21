<?php
/**
 * @file
 * Theme file that emits javascript to display article tools (email, print, post, etc.) on a node
 */

/**
 * Displays the iCopyright Toolbar for this story. When clicked by one of your readers, the
 * toolbar is displayed so the user can purchase rights to this node.
 *
 * The standard article tools include support for EZ-Excerpt and for page view counting. However, you
 * can theme the article tools however you like. Some publishers do this so that the article tools
 * match the look and feel of their site.
 *
 * The hyperlinks to use for the categories of service are:
 *
 * Email      http://license.icopyright.net/g1/3.[$pubid]?icx_id=[$node->nid]
 * Print      http://license.icopyright.net/g2/3.[$pubid]?icx_id=[$node->nid]
 * Post       http://license.icopyright.net/g3/3.[$pubid]?icx_id=[$node->nid]
 * Republish  http://license.icopyright.net/g4/3.[$pubid]?icx_id=[$node->nid]
 */
?>
<!-- iCopyright <?php print ucwords($orientation) ?> Tag -->
<div class="icopyright-article-tools-<?php print $orientation ?> icopyright-article-tools-<?php print $alignment ?>">
  <script type="text/javascript">
    var icx_publication_id = <?php print $pubid ?>;
    var icx_content_id = '<?php print $node->nid ?>';
  </script>
  <script type="text/javascript"
          src="<?php print $server ?>/rights/js/<?php print($orientation == 'horizontal' ? 'horz' : 'vert') ?>-toolbar.js"></script>
  <noscript>
    <a class="icopyright-article-tools-noscript"
       href="<?php print $server ?>/3.<?php print $pubid ?>?icx_id=<?php print $node->nid?>"
       target="_blank"
       title="Main menu of all reuse options">
      <img height="25" width="27" border="0" align="bottom"
           alt="[Reuse options]"
           src="<?php print $server ?>/images/icopy-w.png"/>
      Click here for reuse options!
    </a>
  </noscript>
</div>
<?php if($orientation == 'horizontal') print '<div style="clear:both;"></div>'; ?>
<!-- iCopyright Tag -->



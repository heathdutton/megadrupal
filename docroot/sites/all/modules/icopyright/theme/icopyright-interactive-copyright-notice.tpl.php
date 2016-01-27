<?php
/**
 * @file
 * Theme file that renders the interactive copyright notice (a copyright notice with a hyperlink to iCopyright)
 */

/**
 * Displays the interactive copyright notice, normally at the bottom of your article. It includes
 * both the copyright notice and the name of your publication. When clicked it takes the reader to
 * the main menu of services for your article.
 *
 * You can theme the copyright notice however you like, but the right hyperlink to use for a node is
 *
 *   http://license.icopyright.net/3.[$pubid]?icx_id=[$node->nid]
 */
?>
<!-- iCopyright Interactive Copyright Notice -->
<script type="text/javascript">
    var icx_publication_id = <?php print $pubid ?>;
    var icx_copyright_notice = '<?php print date('Y', $node->created) ?> <?php print variable_get('site_name', '') ?>';
    var icx_content_id = '<?php print $node->nid ?>';
</script>
<script type="text/javascript"
        src="<?php print $server ?>/rights/js/copyright-notice.js"></script>
<noscript>
    <a style="color: #336699; font-family: Arial, Helvetica, sans-serif; font-size: 12px;"
       href="<?php print $server ?>/3.<?php print $pubid ?>?icx_id=<?php print $node->nid ?>"
       target="_blank" title="Main menu of all reuse options">
      <img height="25" width="27" border="0" align="bottom"
           alt="[Reuse options]"
           src="http://<?php print $server ?>/images/icopy-w.png"/>Click here for reuse options!</a>
</noscript>
<!-- iCopyright Interactive Copyright Notice -->

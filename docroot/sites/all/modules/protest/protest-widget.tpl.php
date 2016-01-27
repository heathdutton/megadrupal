<?php
/**
 * @file protest-widget.tpl.php
 * Main widget template
 *
 * Variables available:
 * - $overlay: Boolean to determine if an overlay is applyed or not.
 * - $body: The main content.
 */
?>

<?php if ($overlay) { ?>
<div id="protest-overlay"></div>
<?php } ?>

<div id="protest-wrap">
  <div id="protest-wrap-inner">
    <?php // Close button ?>
    <a href="#" id="protest-close" title="close"><?php print t('close'); ?></a>

    <?php // Message ?>
    <div id="protest-message">
      <div id="protest-message-inner">
        <?php print $body; ?>
      </div>
    </div>
  </div>
</div>
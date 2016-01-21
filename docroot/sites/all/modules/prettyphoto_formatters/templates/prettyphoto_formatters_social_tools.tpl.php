<?php
/**
 * @file
 * Template file to customize social markup in prettyphoto.
 */
?>
<?php if (isset($twitter) && (bool) $twitter): ?>
  <div class="twitter">
    <a href="http://twitter.com/share" class="twitter-share-button" data-count="none"><?php print $tweet; ?></a>
    <script type="text/javascript" src="http://platform.twitter.com/widgets.js"></script>
  </div>
<?php endif; ?>

<?php if (isset($fb_like) && (bool) $fb_like): ?>
  <div class="facebook" style="width: auto;">
    <iframe src="//www.facebook.com/plugins/like.php?href={location_href}&amp;width&amp;layout=button&amp;action=like&amp;show_faces=false&amp;share=<?php print ((isset($fb_share) && (bool) $fb_share) ? "true" : "false"); ?>&amp;height=23" scrolling="no" frameborder="0" style="border:none; overflow:hidden; height:23px;" allowTransparency="true"></iframe>
  </div>
<?php endif; ?>

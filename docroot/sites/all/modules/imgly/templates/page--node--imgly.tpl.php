<?php
/**
 * @file
 * Imgly's theme implementation to display a single Drupal page when viewing an image node.
 */
?>
<div id="imgly-page-wrapper" class="imgly-page-wrapper">

  <div id="imgly-main" class="imgly-main clearfix">

    <?php print render($imgly_links); ?>

    <?php print render($page['content']); ?>

  </div> <!-- /#imgly-main -->

  <div id="footer-wrapper"><div class="section">

      <?php if ($page['footer']): ?>
        <div id="footer" class="clearfix">
          <?php print render($page['footer']); ?>
        </div> <!-- /#footer -->
      <?php endif; ?>

    </div></div> <!-- /.section, /#footer-wrapper -->

</div> <!-- /#imgly-page-wrapper -->
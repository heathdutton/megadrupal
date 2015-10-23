<?php
// $Id$
/**
 * @file page.footer.inc
 * Footer template.
 * @ingroup page
 */
?>


</div><!--/wrap-inner-->
</div><!--/wrap-->


  <div id="footer" class="clearfix clear">
  <div id="footer-inner" class="center-on-page clearfix">

  <div class="clearfix" id="footer-logo-subscribe">

        <?php print $footer_logo ?>
        <?php if ($site_name) : ?>
          <div class = 'site-name'><?php print l($site_name, $front_page) ?></div>
        <?php endif; ?>

    <div id="footer-subscribe">
    <?php print drupal_render($subscribe_form)  ?>
    </div><!--/footer-subscribe-->
  </div>

    <div id="footer-nav">
      <?php print drupal_render($footer_menu); ?>
    </div>
   
  </div><!--/footer inner-->
    
  </div><!--/#footer-->

</div><!--/page wrapper-->
<div id="closure"><div class="closure-wrapper">
  <?php print render($page['footer']); ?>
  <?php print $footer_utility ?>
</div></div><!--/closure and closure-wrapper-->
<?php
/**
 * footer template
 * Boot Press
 * @author Pitabas Behera
 *
 **/
?>
<?php if (theme_get_setting('scrolltop_display')): ?>
<div id="boot-press-to-top"><span class="glyphicon glyphicon-chevron-up"></span></div>
<?php endif; ?>

<footer id="footer" class="clearfix">
  <div class="container">
    <?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third'] || $page['footer_fourth']):?>
    <div id="footer-inside" class="clearfix">
      <div class="row">
        <div class="col-md-3">
          <?php if ($page['footer_first']):?>
          <div class="footer-area animated fadeInLeft">
          <?php print render($page['footer_first']); ?>
          </div>
          <?php endif; ?>
        </div>

        <div class="col-md-3">
          <?php if ($page['footer_second']):?>
          <div class="footer-area animated fadeInLeft">
          <?php print render($page['footer_second']); ?>
          </div>
          <?php endif; ?>
        </div>

        <div class="col-md-3">
          <?php if ($page['footer_third']):?>
          <div class="footer-area animated fadeInLeft">
          <?php print render($page['footer_third']); ?>
          </div>
          <?php endif; ?>
        </div>

        <div class="col-md-3">
          <?php if ($page['footer_fourth']):?>
          <div class="footer-area animated fadeInLeft">
          <?php print render($page['footer_fourth']); ?>
          </div>
          <?php endif; ?>
        </div>
      </div>
    </div>
    <?php else: ?>
    <div id="subfooter-inside" class="clearfix">
      <div class="row">
        <div class="col-md-12">
          <div class="subfooter-area">
          <?php print theme('links__system_main_menu', array('links' => $secondary_menu, 'attributes' => array('class' => array('menu', 'secondary-menu', 'links', 'clearfix')))); ?>                        
          <?php if ($page['footer']):?>
          <?php print render($page['footer']); ?>
          <?php endif; ?>
          </div>
        </div>
      </div>
    </div>
    <?php endif; ?>
    
    <div class="footer-bottom clearfix animated fadeInUp">
      <div class="row">
        <div class="col-lg-5 col-md-5 col-sm-5 col-xs-12">
          <?php
          if (theme_get_setting('copyright_info')) {
            print filter_xss_admin(theme_get_setting('copyright_info'));
          }
          ?>
        </div>
        <div class="col-lg-7 col-md-7 col-sm-7 col-xs-12  clearfix">
          <?php require_once _boot_press_include_template('social-media.tpl.php'); ?>
        </div>
      </div>
    </div>
  </div>
</footer> 

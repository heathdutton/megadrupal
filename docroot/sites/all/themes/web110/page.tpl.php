<div id="page-wrap">
  <div id="header">
    <div id="header-region" class="clearfix">
      <?php print render($page['header']); ?>
    </div>
  <?php if ($site_name): ?>
    <h1 id="site-name">
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
        <?php print $site_name; ?>
        <?php if($site_slogan) : ?>
        <span class="site-slogan">
          <?php print $site_slogan; ?>
        </span>
      <?php endif; ?> 
      </a>
      
    </h1>
  <?php endif; ?> <!-- /h1 site_name-->

  <?php if ($page['mainmenu_region']): ?>
    <div id="navigation" class="clearfix">
      <?php print render($page['mainmenu_region']); ?>
    </div>
  <?php endif; ?><!-- /menu region  -->
  </div><!-- / header-->

  <div id="main-content" class="left">
    <?php if ($tabs): ?>
      <div class="tabs">
        <?php print render($tabs); ?>
      </div>
    <?php endif; ?>

    <div class="section">
      <?php if ($title): ?>
        <h2 class="page-title">
          <?php print  $title; ?>
        </h2>
      <?php endif; ?>
      <?php print render($page['content']); ?>

    </div><!--/ main section-->
  </div><!-- /main-content-->
  
  <?php if ($page['right']): ?>
    <div class="aside right">
      <?php print render($page['right']); ?>
    </div>
  <?php endif; ?><!-- /right siderbar-->

  <div id="footer">

          
      <div class="footer-section left">
        <div class="section">  
          <?php print render ($page['footer_left']);?>
        </div>
        <span>
          Powered by <a href="http://drupal.org">Drupal</a> and <a href="http://drupal.org/project/web110" title="web110 Drupal theme">web110 theme</a>.
        </span>
      </div>

  
    <?php if ($page['footer_right']) : ?>
      <div class="footer-section right">
        <div class="section  right">
          <?php print render ($page['footer_right']);?>
        </div>
      </div>
    <?php endif; ?><!--/ footer right -->
    <div class="clearfix" />
  </div><!-- /footer -->
</div><!-- /page-wrap-->

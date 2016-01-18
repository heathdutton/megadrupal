<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 */
$fbw = null;
if($page['footer_1'] && $page['footer_2'] && $page['footer_3'] && $page['footer_4']){
     $fbw = 'fb4';   
    }  
else{
        if($page['footer_1'] && $page['footer_2'] && $page['footer_3'] || $page['footer_1']  && $page['footer_2'] && $page['footer_4']  || $page['footer_1'] && $page['footer_3']  && $page['footer_4'] || $page['footer_2'] && $page['footer_3'] && $page['footer_4'] ) {
          $fbw = 'fb3';
        } 
   else {
		    if($page['footer_1'] && $page['footer_2'] || $page['footer_1'] && $page['footer_3']  || $page['footer_1'] && $page['footer_4']  || $page['footer_2'] && $page['footer_3']  || $page['footer_2'] && $page['footer_4'] || $page['footer_3'] && $page['footer_4'] ){
            $fbw = 'fb2';  
            }
	   else {
			    if($page['footer_1'] || $page['footer_2'] || $page['footer_3'] || $page['footer_4']){
                 $fbw = 'fb1';  
                }
			}
		}
        
    } 
   
?>

  <div id="top-wrapper" class="clear-block">
    <div id="header" class="section width">

        <div id="branding" class="clear-block">
          <?php if ($logo): ?>
            <a href="<?php print check_url($front_page) ?>" title="<?php print t('Home') ?>"><img id="logo" src="<?php print $logo ?>" alt="<?php print t('Site Logo')?>" /></a>
          <?php endif; ?>

          <?php if ($site_name): ?>
            <h1 id="site-name">
              <a href="<?php print check_url($front_page) ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a>
            </h1>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <p id="slogan"><?php print $site_slogan ?></p>
          <?php endif; ?>
        </div> <!-- /branding -->

    <?php if ($main_menu): ?>
      <div id="navigation"><div class="section">
        <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix')), 'heading' => t('Main menu'))); ?>
      </div></div> <!-- /.section, /#navigation -->
    <?php endif; ?>

    </div>
  </div> <!--/top-wrapper-->

  <div id="middle-wrapper" class="clear-block">
  <div id="middle-inner" class="clear-block">
  <div class="section width clear-block">

        <?php if (($feed_icons) || !empty($secondary_menu)): ?>
          <div id="menu-bar" class="clear-block">


            <?php if (!empty($secondary_menu)): ?>
              <h3 class="hidden">Secondary Menu</h3>
                <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix')), 'heading' => t('Secondary menu'))); ?>
            <?php endif; ?>

            <?php print $feed_icons ?>

          </div>
        <?php endif; ?>

    <div id="main-content" class="column">
    <div class="content-inner clear-block">

        <?php if ($breadcrumb): ?><div class="top-breadcrumb clear-block"><?php print $breadcrumb; ?></div><?php endif; ?>

        <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>

        <?php if ($page['content_top']): ?>
          <div id="top-content-block" class="content-block clear-block">
            <?php print render($page['content_top']); ?>
          </div>
        <?php endif; ?>

        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>

        <?php if ($page['content_bottom']): ?>
          <div id="bottom-content-block" class="content-block clear-block">
            <?php print render($page['content_bottom']) ?>
          </div>
        <?php endif; ?>

        <?php if ($breadcrumb): ?><div class="bottom-breadcrumb clear-block"><?php print $breadcrumb; ?></div><?php endif; ?>

    </div>
    </div>

      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-left" class="column sidebar"><div class="section">
          <?php print render($page['sidebar_first']); ?>
        </div></div> <!-- /.section, /#sidebar-first -->
      <?php endif; ?>

      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-right" class="column sidebar"><div class="section">
          <?php print render($page['sidebar_second']); ?>
        </div></div> <!-- /.section, /#sidebar-second -->
      <?php endif; ?>

  </div>
  </div> <!--/middle-inner-->
  </div> <!--/middle-wrapper-->


  <div id="bottom-wrapper" class="clear-block">
    <div id="footer" class="section width">

        <div class="wrap-col column-wrapper clear-block <?php print $fbw; ?>">

          <?php if ($page['footer_1']): ?>
            <div class="footer-column">
              <?php print render($page['footer_1']); ?>
            </div>
          <?php endif; ?>

          <?php if ($page['footer_2']): ?>
            <div class="footer-column">
              <?php print render($page['footer_2']); ?>
            </div>
          <?php endif; ?>

          <?php if ($page['footer_3']): ?>
            <div class="footer-column">
              <?php print render($page['footer_3']); ?>
            </div>
          <?php endif; ?>

          <?php if ($page['footer_4']): ?>
            <div class="footer-column">
              <?php print render($page['footer_4']); ?>
            </div>
          <?php endif; ?>

        </div> <!-- /footer-column-wrap -->

      <div id="credit-wrap" class="clear-block">
       <?php print render($page['footer']); ?>
      </div> <!--/credit-wrap-->
    </div>
  </div> <!--/bottom-wrapper-->

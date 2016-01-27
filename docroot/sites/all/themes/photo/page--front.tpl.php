<?php
/**
 * @file
 * Photo Theme implementation to display a single Drupal page.
 * structure of a single Drupal page.
 */
?>
 <div  class="background_overlay"></div>
  <div id="slideshow"><ul>					
	<li><img src="<?php print $photo_1; ?>" alt="Css Template Preview" /><div class="atr">Photo by : <?php print $attribute_photo_1; ?></div></li>
	<li><img src="<?php print $photo_2; ?>" alt="Css Template Preview" /><div class="atr">Photo by : <?php print $attribute_photo_2; ?></div></li>
	<li><img src="<?php print $photo_3; ?>" alt="Css Template Preview" /><div class="atr">Photo by : <?php print $attribute_photo_3; ?></div></li>	</ul>
  </div><!--#slider-->
 <div id="header">
      <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <div id="name-and-slogan">

        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name">
              <strong>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </strong>
            </div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id="site-slogan">
            <?php print $site_slogan; ?>
          </div>
        <?php endif; ?>

      </div> <!-- /#name-and-slogan -->
    <?php endif; ?>
 
 </div><!--end header-->
<div id="frontwrap">
<img src="sites/all/themes/photo/css/images/scroll2.png" />
<div id="page">
 <div class="contenar">
  <div id="manubar">
     <?php if ($main_menu): ?>
      <div id="main-menu" class="navigation">
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu-links',
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </div> <!-- /#main-menu -->
    <?php endif; ?>

  </div> <!-- #manubar -->
 <div id="content" class="clearfix">
   <?php if ($messages): ?>
    <div id="messages"><div class="section clearfix">
      <?php print $messages; ?>
    </div></div> <!-- /.section, /#messages -->
   <?php endif; ?>
   <?php if ($breadcrumb): ?>
      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
   <?php endif; ?>
   <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title">
          <?php print $title; ?>
        </h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?>
        <div class="tabs">
          <?php print render($tabs); ?>
        </div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>

  </div><!--end content-->
 </div><!--contenar-->
</div><!--end page-->

<div id="footer">
 <div class="contenar">
  <div id="subfooter" class="clearfix">

  <div style="clear: both;"> &nbsp; </div>
 </div><!--end subfooter--> 
    <?php if ($page['footer']): ?>
      <div class="footer" class="clearfix">
        <?php print render($page['footer']); ?>
      </div> <!-- /#footer -->
    <?php endif; ?>
		 </div><!--contenar-->
</div><!--end footer-->

</div><!--end wrap-->

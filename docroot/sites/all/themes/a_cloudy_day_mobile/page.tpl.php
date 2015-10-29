<?php // $Id$ ?>
  <?php print render($page['header']); ?>      
  <?php if($main_menu): ?>
    <div id="top">
      <div id="topmenu">
        <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('menu', 'inline', 'clearfix')))); ?>
      </div>    
		</div><!-- /top -->
  <?php endif; ?>
  <?php if($secondary_menu): ?>
    <div id="submenu">
      <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('menu', 'inline', 'clearfix')))); ?>
    </div>
  <?php endif; ?>  
  <div id="header">
    <div id="headertitle">
      <h1><a href="<?php print $front_page;?>" title="<?php print t('Home') ?>"><?php print $site_name;?></a></h1>
      <div class='site-slogan'>
        <?php print $site_slogan ;?>
      </div>
    </div>
    <?php if ($logo) : ?>
      <div id="logo">
        <a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" /></a>
      </div>
    <?php endif; ?>
    <?php if($page['header_block']): ?><div class="block block-theme"><?php print render($page['header_block']) ?></div><?php endif; ?>
  </div><!-- /header -->
  <?php if($messages): echo "<div id=\"messagebox\">" . $messages . "</div>"; endif; ?>         
  <div id="contentcontainer">
    <div id="container">
      <div id="contentleft">
        <div id="page">
          <?php print render($page['highlight']); ?>
          <?php print $breadcrumb; ?>
          <a id="main-content"></a>
          <?php print render($title_prefix); ?>
          <?php if ($title): ?>
            <h1 class="title" id="page-title"><?php print $title; ?></h1>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs): ?>
            <div class="tabs"><?php print render($tabs); ?></div>
          <?php endif; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?>
            <ul class="action-links"><?php print render($action_links); ?></ul>
          <?php endif; ?>
          <?php print render($page['content']); ?>
          <div class="feedicons">
            <?php echo $feed_icons ?>
          </div>
        </div>
        <?php if ($page['footer_block']): print "<div id=\"footer\">" . render($page['footer_block']) . "</div>"; endif;?>
      </div><!-- /contentleft -->
    </div><!-- /container -->
  </div><!-- /contentcontainer -->
  <div id="bottompage">
    <div id="skyline"></div>
  </div>    
  <?php print render($page['footer']); ?>
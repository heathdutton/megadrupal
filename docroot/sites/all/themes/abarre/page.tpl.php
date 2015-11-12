<?php 
// $Id:$
?>


<div id="header">
    <?php if ($logo): ?>
      <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" />
      </a>
    <?php endif; ?>
    
    <?php if ($site_name): ?>
      <h1 id='site-name'>
        <a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>">
          <?php print $site_name; ?>
        </a>
      </h1>
    <?php endif; ?>
    
    <?php if ($site_slogan): ?>
      <div id='site-slogan'>
        <?php print $site_slogan; ?>
      </div>
    <?php endif; ?>  
</div> <!-- /header -->

<div id="main">
	<div id="content" class="column">
  	<?php print $feed_icons; ?>
    <?php if ($breadcrumb): ?><?php print $breadcrumb; ?><?php endif; ?>
  	<?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
    <?php if ($tabs && !empty($tabs['#primary'])): ?><div class="tabs clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php print render($title_suffix); ?>
  	<?php print $messages; ?>
		<?php print render($page['content']); ?>
  </div> <!-- /content -->

  <div id="sidebar-first" class="column">
    <?php if (isset($main_menu)) : ?>
      <?php print theme('links', $main_menu, array('class' => 'links main-menu')); ?>
    <?php endif; ?>

		<?php if (isset($secondary_menu)): ?>
       <?php print theme('links', $secondary_menu, array('class' => 'links secondary-menu')); ?>
    <?php endif; ?>

    <?php print render($page['sidebar_first']); ?>
  </div> <!-- /sidebar-first -->

</div> <!-- /main -->

<div id="footer">
	<div id="corner"></div>
	<div class="content">
  	<?php print render($page['footer']); ?>
	</div>
</div> <!-- /footer -->
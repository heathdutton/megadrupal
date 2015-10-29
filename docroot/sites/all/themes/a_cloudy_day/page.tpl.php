<?php // $Id$ ?>
  <?php print render($page['header']); ?>
    <?php if($primary_nav): ?>
			<div id="top">
	  	  <div id="topmenu">
	  		  <?php
	        print $primary_nav;
					?>
				</div>
	  	</div><!-- /top -->
		<?php endif; ?>
		<div id="header">
			<div id="headertitle">
				<h1><a href="<?php echo $front_page;?>" title="<?php echo t('Home') ?>"><?php echo $site_name;?></a></h1>
				<?php if($site_slogan): ?>
				  <div class='site-slogan'>
					  <?php echo $site_slogan ;?>
				  </div>
			  <?php endif; ?>
			</div><!-- /headertitle -->
			
			<?php if ($logo): ?>
				<div id="logo">
          <a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" /></a>
      	</div>
			<?php endif; ?>	
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
					</div><!-- /page -->
					
					<?php if ($page['footer_block']): print "<div id=\"footer\">" . render($page['footer_block']) . "</div>"; endif;?>
				</div><!-- /contentleft -->
				
				<div id="contentright">
					<?php print render($page['sidebar_second']); ?>
				</div><!-- /contentright -->
				
			</div><!-- /container -->
			
		</div><!-- /contentcontainer -->
		
		<div id="bottompage">
			<div id="skyline"></div>
			<div id="bottomtext">
				Theme designed by <a href="http://www.carettedonny.be" title="Donny Carette">Donny Carette</a> - Powered by <a href="http://www.drupal.org" title="Drupal">Drupal</a> - copyright &copy; <?php echo date("Y");?>
			</div>
		</div><!-- /bottompage -->
		<?php print render($page['footer']); ?>
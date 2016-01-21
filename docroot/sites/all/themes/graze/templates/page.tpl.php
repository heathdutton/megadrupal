<?php // page template ?>
<div id="wrap">
<div id="allbutfooter">
<!-- top band that is full width-->
	<div id="top">
			<div class="fixed">
		        <?php if ($main_menu): ?>
			      <div id="main-menu">
			        <?php print theme('links__system_main_menu', array(
			          'links' => $main_menu,
			          'attributes' => array(
			            'id' => 'main-menu-links',
			            'class' => array('links', 'clearfix'),
			          ),
			          'heading' => array(
			            'text' => t('Main menu'),
			            'level' => 'h2',
			            'class' => array('element-invisible'),
			          ),
			        )); ?>
			      </div> <!-- /#main-menu -->
			    <?php endif; ?>
			<?php print render($page['search']); ?>
			</div> 
	</div>
<!-- start portion of site that's a fixed width container -->
<div class="fixed">   	
<div id="box" class="clearfix">
	<div id="branding">
    <!-- logo -->
		<?php if ($logo): ?>
        	<div id="logo">
				<a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo ?>"alt="<?php print t('Home'); ?>" /></a>
			</div>
		<?php endif; ?>
        <!-- site name -->
		<?php if ($site_name): ?>
			<div id="sitename">
				<h1><a href="<?php print $base_path ?>" title="<?php print t('Home')?>"><?php print $site_name; ?></a></h1>
			</div>
		<?php endif; ?>
    </div>
	<div class="shadow"></div>
        <!-- Region: Content -->
	<div id="main">
		<div id="content" class="clearfix">
			<!-- title -->
			<?php if ($title): ?>
				<h2 class="content-title"><?php print $title; ?></h2>
			<?php endif; ?>
			<?php print render($page['tabs']); ?>
			<?php print render($page['help']); ?>
			<?php print render($page['messages']); ?>
        	<?php print render($page['content']); ?>
        </div>
		<!-- Region: Sidebar Two -->
		<?php if ($page['sidebar_second']): ?>
			<div class="bar">
				<?php print render($page['sidebar_second']); ?>
			</div>
        <?php endif; ?>
        <!-- Region: Sidebar One -->
		<?php if ($page['sidebar_first']): ?>
            <div class="bar">
				<?php print render($page['sidebar_first']); ?>
			</div>
		<?php endif; ?>
		<!-- little box region at bottom of main container, with darker background -->
		<?php if ($page['boxbottom']): ?>
			<div id="boxbottom" class="clearfix">
  				<?php print render($page['boxbottom']); ?>
			</div> 
		<?php endif; ?>
	</div><!-- fixed width container area ends -->
</div>
</div>
<?php if ($page['middle']): ?>
	<div class="fixed">
		<div id="middle" class="clearfix">
			<?php print render($page['middle']); ?>
		</div> 
	</div>
<?php endif; ?>
</div><!-- /#allbutfooter -->
</div><!-- /#wrap -->
<!-- bottom band that is full width-->
<div id="footer" class="clearfix">
	<div class="fixed clearfix">
	  	<?php print render($page['footer']); ?>
	</div> 
</div><!-- /#footer -->
<?php // page template ?>
<div id="wrap">
<div id="allbutend">
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
			</div><!-- /.fixed -->
	</div><!-- /#top -->
<!-- start portion of site that's a fixed width container -->
<div class="fixed">   	
	<div id="branding" class="clearfix">
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
		<div class="social"><?php print render($page['social']); ?></div>
  </div><!-- /#branding -->
<div id="box" class="clearfix">
  <!-- Region: Content -->
	<div id="main">
		<div id="content" class="clearfix">
			<!-- title -->
			<?php if ($title): ?>
				<h2 class="content-title"><?php print $title; ?></h2>
			<?php endif; ?>
			<?php print render($tabs); ?>
			<?php print render($help); ?>
			<?php print render($messages); ?>
        	<?php print render($page['content']); ?>
    </div>
		<!-- Region: Sidebar One -->
		<?php if ($page['sidebar_first']): ?>
     	<div class="bar">
				<?php print render($page['sidebar_first']); ?>
			</div>
		<?php endif; ?>		
		<!-- Region: Sidebar Two -->
		<?php if ($page['sidebar_second']): ?>
			<div class="bar">
				<?php print render($page['sidebar_second']); ?>
			</div>
    <?php endif; ?>
	</div><!-- fixed width container area ends -->
</div>
</div>

</div><!-- /#allbutend -->
</div><!-- /#wrap -->

<div id="end" class="clearfix">
	
<!-- bottom band that is full width-->
<?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third']): ?>
<div id="footer" class="clearfix">
	<div class="fixed">
	<!-- Region: Footer Left -->
	<?php if ($page['footer_first']): ?>
		<div class="footerblock">
			<?php print render($page['footer_first']); ?>
		</div>
  <?php endif; ?>
	<!-- Region: Footer Middle -->
	<?php if ($page['footer_second']): ?>
		<div class="footerblock">
			<?php print render($page['footer_second']); ?>
		</div>
  <?php endif; ?>
	<!-- Region: Footer Right -->
	<?php if ($page['footer_third']): ?>
		<div class="footerblock last">
			<?php print render($page['footer_third']); ?>
		</div>
  <?php endif; ?>
 </div><!-- /.fixed -->
</div><!-- /footer -->
<?php endif; ?>

	<div class="fixed">
	<!-- Region: Credits -->
	<?php if ($page['credits']): ?>
		<div class="credits">
			<?php print render($page['credits']); ?>
		</div>
  <?php endif; ?>
 </div><!-- /.fixed -->
</div><!-- /#end -->
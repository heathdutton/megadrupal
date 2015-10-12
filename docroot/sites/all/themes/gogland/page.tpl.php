<div id="wrapper" class="clearfix"><!-- Begin wrapper -->
	<div id="header"><!-- Begin header -->
		<h1>
			<a href="<?php print base_path() ?>" title="Back to home"><?php print $site_name ?></a>
		</h1>
		<p><?php print $site_slogan ?></p>
	</div>
	<hr />
	<div id="outer-space" class="clearfix"><!-- Begin outer-space -->
		<div id="hfeed">
			<!-- @todo check this title and how to display it. -->
			<?php //if($title) : ?>
				<!-- <p id="page-info" class="page-title"> -->
					<!-- <span><?php //print $title; ?></span> -->
				<!-- </p> -->
			<?php //endif;?>
			<?php if($tabs): ?>
				<div class="tabs">
					<?php print render($tabs); ?>
				</div>
			<?php endif; ?>
			<?php if($page['content']): ?>
				<?php print render($page['content']); ?>
			<?php endif; ?>
		</div>
	<?php if($page['left']) : ?>
		<div id="left-sidebar">
			<?php print render($page['left']); ?>
		</div>
	<?php endif ?>
	</div><!-- End outer-space -->
	<hr />
	<?php if($page['right']) : ?>
		<div id="right-sidebar">
			<?php print render($page['right']); ?>
		</div>
	<?php endif; ?>
	<hr />
</div><!-- End wrapper -->

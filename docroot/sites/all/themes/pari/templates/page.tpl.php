<header id="header-top-bg">
	<?php include ($directory."/includes/header.tpl.php"); ?>
</header> <!--/header -->
	<?php include ($directory."/includes/page_header.tpl.php"); ?>
<div class="container">
<div id="node-container">
	<section id="page-content" role="main" class="page-content">
		<?php if ($messages): ?>
			<?php print $messages; ?>
		<?php endif; ?> <!--/ End message-->
		<?php if ($page['highlighted']): ?>
			<div class="highlighted">
				<?php print render($page['highlighted']); ?>
			</div>
		<?php endif; ?> <!--/ End Highlighted -->
		<?php if ($tabs): ?>
			<div class="tabs">
				<?php print render($tabs); ?>
			</div>
		<?php endif; ?> <!--/ End Tab-->
		<?php print render($page['help']); ?>
		<?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
		<?php print render($page['content']) ?>
	</section> <!--/section -->
	<?php include ($directory."/includes/left_sidebar.tpl.php"); ?>
	<?php include ($directory."/includes/right_sidebar.tpl.php"); ?>
</div> <!--/#node-container -->
</div> <!--/container -->
<?php include ($directory."/includes/footer.tpl.php"); ?>

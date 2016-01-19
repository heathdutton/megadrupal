<div id="shell">
  <div id="header">
	<?php if ($logo) : ?>
      <a class='site-logo' href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" border="0" /></a>
    <?php endif; ?>
    <?php if ($site_name) : ?>
      <div class='site-name'><h1><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><?php print($site_name) ?></a></h1></div>
    <?php endif;?>
    <?php if ($site_slogan) : ?>
      <div class='site-slogan'><h2 class='site-slogan'><?php print($site_slogan) ?></h2></div>
    <?php endif;?>
	<a href="/rss.xml" class="rss">Subscribe</a>
  </div><!-- End #header -->
  <div id="main">
	<div class="cl">&nbsp;</div>
	<div id="content" class="narrowcolumn" role="main">
	  <?php if ($page['content_top']): ?>
          <div id="content-top">
            <?php print render($page['content_top']); ?>
          </div><!-- End #content-top -->
        <?php endif; ?>
	  <?php print $breadcrumb; ?>
      <?php if ($page['highlighted']): print '<div id="mission">'. render($page['highlighted']) .'</div>'; endif; ?>
      <?php if ($tabs): print '<div id="tabs-wrapper" class="clearfix">'; endif; ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): print '<ul class="tabs primary">'. render($tabs) .'</ul></div>'; endif; ?>
      <?php if (!empty($tabs2)): echo '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
      <?php if ($show_messages && $messages): print $messages; endif; ?>
      <?php print render($help); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <div class="clearfix">
        <?php print render($page['content']); ?>
      </div>
      <br />
	  <?php if ($page['content_bottom']): ?>
        <div id="content-bottom">
          <?php print render($page['content_bottom']); ?>
        </div><!-- End #content-bottom -->
      <?php endif; ?>
	</div><!-- End #content -->
    <div id="sidebar" role="complementary">
	  <?php print render($page['sidebar_first']); ?>
	</div><!-- End #sidebar -->
    <div class="cl">&nbsp;</div>
  </div><!-- End #main -->
</div><!-- End #shell -->
<div id="footer">
  <p class="copy"><?php print render($page['footer_message']); ?> <br /> <a href="http://drupalservers.net" title="Drupal Themes">Drupal Themes</a> by <a href="http://arborwebdevelopment.com">Arbor Web Development</a> <?php print render($page['footer']); ?></p>
</div><!-- End #footer -->
<?php print render($page['page_bottom']); ?>
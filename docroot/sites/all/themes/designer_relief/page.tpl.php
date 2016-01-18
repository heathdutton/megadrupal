<div id="page">
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
		<?php if ($main_menu): ?>
          <div id="primary-menu">
            <?php print $main_menu_tree; ?>
          </div><!-- /end #primary-menu -->
        <?php endif; ?>
	</div>
	<div id="content">
	    <!-- begin left-column -->
		<div id="leftcolumn">
		  <?php if ($page['content_top']): ?>
            <div id="content-top">
              <?php print render($page['content_top']); ?>
            </div>
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
		  <?php if ($page['content_bottom']): ?>
            <div id="content-bottom">
              <?php print render($page['content_bottom']); ?>
            </div>
          <?php endif; ?>
		</div>
		<!-- end left-column -->
		<!-- begin sidebar -->
        <div id="sidebar">
          <?php print render($page['search_box']); ?>
          <?php print render($page['sidebar_first']); ?>
          <div id="meta" class="meta"><h2>Meta</h2>
            <ul>
              <li><a href="/rss.xml" title="Syndicate this site using RSS">RSS</a></li>
            </ul>
          </div>
         </div>
        <!-- end sidebar -->
	</div>
	<div id="footer">
		<?php print render($page['footer_message']) ?> <?php print render($page['footer']); ?>
		 <br /><a href="http://drupalservers.net/drupal-7-themes" title="Drupal 7 themes">Drupal 7 themes</a> by <a href="http://arborwebdevelopment.com" title="Arbor Web Development">Arbor Web Development</a>
		
	</div>
</div>
<?php print render($page['page_bottom']); ?>
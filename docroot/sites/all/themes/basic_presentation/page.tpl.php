<div id="wrap">
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
  </div>
  <div id="sidebar">
    <div class="navmenu">
	  <?php if ($page['search_box']): ?>
	  <?php print render($page['search_box']); ?>
	  <?php endif; ?>
	  <?php if ($main_menu): ?>
        <div id="primary-menu">
          <?php print $main_menu_tree; ?>
        </div><!-- /end #primary-menu -->
      <?php endif; ?>
	  <?php if ($page['sidebar_first']): ?>
	  <?php print render($page['sidebar_first']); ?>
	  <?php endif; ?>
    </div>
  </div>
  <div id="main">
    <div class="headerimage"></div>
	<?php if ($page['content_top']): ?>
      <div id="content-top">
        <?php print render($page['content_top']); ?>
      </div>
    <?php endif; ?>
	<div class="content">
      <?php print $breadcrumb; ?>
      <?php if ($page['highlighted']): print '<div id="mission">'. render($page['highlighted']) .'</div>'; endif; ?>
      <?php if (!empty($tabs)): print '<div id="tabs-wrapper" class="clearfix">'; endif; ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
      <?php print render($title_suffix); ?>
      <?php if (!empty($tabs)): print '<ul class="tabs primary">'. render($tabs) .'</ul></div>'; endif; ?>
      <?php if (!empty($tabs2)): print '<ul class="tabs secondary">'. render($tabs2) .'</ul>'; endif; ?>
      <?php if ($show_messages && $messages): print $messages; endif; ?>
      <?php print render($help); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <div class="clearfix">
        <?php print render($page['content']); ?>
      </div>
	</div><!-- end .content -->
    <?php if ($page['content_bottom']): ?>
      <div id="content-bottom">
        <?php print render($page['content_bottom']); ?>
      </div>
    <?php endif; ?><!-- end #content-bottom -->
    <?php if ($page['q1'] || $page['q2'] || $page['q3'] || $page['q4']): ?>
	  <div id="quarters">
	    <?php if ($page['q1']): ?>
          <div class="q1">
            <?php print render($page['q1']); ?>
		  </div>
        <?php endif; ?>
	    <?php if ($page['q2']): ?>
          <div class="q2">
            <?php print render($page['q2']); ?>
          </div>
	    <?php endif; ?>
	    <?php if ($page['q3']): ?>
          <div class="q3">
            <?php print render($page['q3']); ?>
          </div>
	    <?php endif; ?>
	    <?php if ($page['q4']): ?>
          <div class="q4">
            <?php print render($page['q4']); ?>
          </div>
	    <?php endif; ?>
      </div>
	<?php endif; ?><!-- end #quarters -->
  </div><!-- end #main -->
    <div id="footer">
	  <div id="footer_1">
	    <div id="copy1"><p class="copy1">&copy; 2011 <a href="#"><?php print $site_name ?></a> |</p></div>
	    <div id="secondary_links">
		  <?php print theme('links', array('links' => menu_navigation_links('user-menu'), 'attributes' => array('class'=> array('links', 'user-menu')) ));?>
		</div>
	  </div>
	  <div id="copy2">
	    <p class="copy2"><a href="http://andreasviklund.com/templates/learn/" title="Template design">Template design</a> by <a href="http://andreasviklund.com/" title="Andreas Viklund">Andreas Viklund</a> | <a href="http://themeroot.com" title="Drupal themes">Drupal Themes</a> by <a href="http://arborwebdevelopment.com" title="Arbor Web Development">Arbor Web Development</a></p>
	  </div>
	  <div id="footer_2">
	  <?php print render($page['footer']); ?>
	  </div>
    </div>
</div>
<?php print render($page['page_bottom']); ?>
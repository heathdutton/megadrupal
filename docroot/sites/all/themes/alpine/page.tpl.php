<div id="frame">
  <div id="nav">
    <?php if ($main_menu): ?>
      <div id="primary-menu">
        <?php print $main_menu_tree; ?>
      </div><!-- /primary links menu -->
    <?php endif; ?>
</div>
<div id="header">
<?php if ($page['content_header']): ?>
    <div id="content-header">
      <?php print render($page['content_header']); ?>
    </div>
  <?php endif; ?>

  <?php if ($logo || $site_name): ?>
    <a class='site-logo' href="<?php print $front_page ?>" title="<?php print $site_name ?>"><img src="<?php print($logo) ?>" alt="<?php print $site_name ?>" border="0" /></a>
  <?php endif; ?>
    
  <?php if ($site_name) : ?>
      <div class='site-name'><h1><a href="<?php print $front_page ?>" title="<?php print $site_name ?>"><?php print($site_name) ?></a></h1></div>
  <?php endif;?>
    
  <?php if ($site_slogan) : ?>
    <div class='site-slogan'><h2 class='site-slogan'><?php print($site_slogan) ?></h2></div>
  <?php endif;?>
   
</div>
<div id="content">
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
  <br />
  <?php print $feed_icons ?>
  <?php if ($page['content_bottom']): ?>
  <div id="content-bottom">
    <?php print render($page['content_bottom']); ?>
  </div>
  <?php endif; ?>
</div>
<?php if ($page['sidebar_first']): ?>	
<div id="sidebar">
  <div id="sidebar">
    <?php print render($page['search_box']); ?>
    <?php print render($page['sidebar_first']); ?>
  </div>
</div>
<?php endif; ?>
<br clear="all" />
<?php if ($page['tryptych_col1'] || $page['tryptych_col2'] || $page['tryptych_col3']): ?>
  <div id="tryptych">
    <div id="tryptych-col">
      <?php print render($page['tryptych_col1']); ?>
    </div>
    <div id="tryptych-col">
      <?php print render($page['tryptych_col2']); ?>
    </div>
    <div id="tryptych-col">
      <?php print render($page['tryptych_col3']); ?>
    </div>
  </div><!-- /#tryptych -->
<?php endif;?>
<br clear="all" />
</div>
<div id="footer">
  <p><?php print '<div class="footer">'. render($page['footer']) .'</div>' . '<div class="credits"><a href="http://themeroot.com/" title="Drupal 7 Themes">Drupal 7 themes</a> by <a href="http://arborwebdev.com" title="Arbor Web Development">Arbor Web Development</a></div>'; ?></p>
</div>
<?php print render($page['page_bottom']); ?>
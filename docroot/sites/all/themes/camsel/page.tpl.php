<?php print render($page['page_top']); ?>
<div id="page">
  <div id="header">
    <?php if ($logo): ?>
      <div id="logo-floater"><a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo ?>" alt="<?php print $title ?>" /></a></div>
    <?php endif; ?>
    <div id="name-and-slogan"><h1 id="sitename"><a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name ?></a></h1><h2><a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_slogan ?></a></h2></div>
    <div id="header-region"><?php print render($page['header']); ?></div>
  </div><!-- /header -->
  <div id="container" class="clearfix">
    <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-first" class="column sidebar">
        <?php print render($page['sidebar_first']); ?>
      </div><!-- /sidebar-first -->
    <?php endif; ?>
    <div id="main" class="column">
      <?php if (!empty($breadcrumb)): ?>
        <div id="breadcrumb"><?php print $breadcrumb; ?></div>
      <?php endif; ?>
      <?php if (!empty($title)): ?>
        <h2 class="title" id="page-title"><?php print $title; ?></h2>
      <?php endif; ?>
      <?php if (!empty($tabs)): ?>
        <div class="tabs"><?php print $tabs; ?></div>
      <?php endif; ?>
      <?php if (!empty($messages)): print $messages; endif; ?>
      <?php if (!empty($help)): print $help; endif; ?>
      <div id="content-content" class="clearfix">
        <?php print render($page['content']); ?>
      </div><!-- /content-content -->
    </div><!-- /main -->
    <?php if (!empty($page['sidebar_second'])): ?>
      <div id="sidebar-second" class="column sidebar">
        <?php print render($page['sidebar_second']); ?>
      </div><!-- /sidebar-second -->
    <?php endif; ?>
  </div><!-- /container -->
  <div id="footer">
    <?php if (!empty ($footer)): ?><p class="left"><?php print $footer; ?>&nbsp;</p><?php endif; ?>
    <?php if (!empty($secondary_links)): ?><span class="right"><?php print theme('links', $secondary_links, array('id' => 'secondary')); ?></span><?php endif; ?>
    <p class="left">Drupal theme by <a href="http://webdesign.magnity.co.uk" title="Magnity Webdesign (Drupal Website Design)">Magnity Webdesign</a>.</p>
  </div>
</div><!-- /page -->
<?php print render($page['page_bottom']); ?>
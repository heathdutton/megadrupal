<div id="header" class="clearfix">

  <?php print render($page['search']); ?>

  <?php if ($logo): ?>
    <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
      <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
    </a>
  <?php endif; ?>
  <?php if ($site_name): ?>
    <?php if ($title): ?>
      <div id="site-name"><strong>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
      </strong></div>
    <?php else: /* Use h1 when the content title is empty */ ?>
      <h1 id="site-name">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
      </h1>
    <?php endif; ?>
  <?php endif; ?>
  <?php if ($site_slogan): ?>
    <div id="site-slogan"><?php print $site_slogan; ?></div>
  <?php endif; ?>

  <div id="menu">
    <?php print theme('links__system_secondary_menu', array(
      'links' => $secondary_menu,
      'attributes' => array(
        'id' => 'subnavlist',
        'class' => array('links'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    )); ?>
    <?php print theme('links__system_main_menu', array(
      'links' => $main_menu,
      'attributes' => array(
        'id' => 'navlist',
        'class' => array('links'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    )); ?>
  </div>

  <?php print render($page['header']); ?>

</div>

<div class="layout-columns clearfix">

  <?php print render($page['sidebar_first']); ?>

  <div id="main" class="column">
    <?php print render($page['highlighted']); ?>

    <div class="inner">
      <?php print $breadcrumb; ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
      <?php print render($page['help']); ?>
      <?php print $messages; ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div>
  </div>

  <?php print render($page['sidebar_second']); ?>

</div>

<div id="footer">
  <?php print render($page['footer']); ?>
</div>

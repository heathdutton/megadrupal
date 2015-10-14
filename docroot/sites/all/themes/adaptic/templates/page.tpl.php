<?php
/**
 * @file
 * adaptIC's implementation to display a page.
 */

 ?>
<header>
  <div class="inner">
    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
    <?php endif; ?>
    <?php if ($site_name): ?>
      <div id="site-name" class="site-name">
        <?php print $site_name; ?>
      </div>
    <?php endif; ?>
    <?php if ($site_slogan): ?>
      <div id="site-slogan" class="site-slogan">
        <?php print $site_slogan; ?>
      </div>
    <?php endif; ?>
    <nav>
      <?php if ($main_menu): ?>
      <?php print theme('links__system_main_menu', array(
        'links' => $main_menu,
        'attributes' => array(
          'id' => 'main-menu',
          'class' => array('links', 'inline', 'clearfix'),
        ),
      )); ?>

      <?php print render($page['navigation']); ?>
      <?php endif; ?>
    </nav>
  </div>
</header>

<div id="container">
  <div class="inner">

    <?php print render($page['sidebar_left']); ?>


    <article>
      <?php print render($page['highlighted']); ?>
      <?php print $breadcrumb; ?>
      <a id="main-content"></a>
      <?php if ($title): ?>
        <h1 class="title" id="page-title">
          <?php print $title; ?>
        </h1>
      <?php endif; ?>

      <?php print $messages; ?>

      <?php if ($tabs = render($tabs)): ?>
        <div class="tabs"><?php print $tabs; ?></div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>

      <?php print render($page['content']); ?>

    </article>

    <?php print render($page['sidebar_right']); ?>

  </div>
</div>

<footer>
  <div class="inner">
    <?php print render($page['footer']); ?>
  </div>
</footer>

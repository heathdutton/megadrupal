<div id="header">

  <?php if ($logo) : ?>
    <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
  <?php endif; ?>

  <?php if ($site_name) : ?>
    <h1 class="site-name">
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
    </h1>
  <?php endif; ?>

  <?php if ($site_slogan) : ?>
    <div class="site-slogan"><?php print $site_slogan; ?></div>
  <?php endif; ?>

  <?php print theme('links__system_secondary_menu', array(
    'links' => $secondary_menu,
    'attributes' => array(
      'id' => 'secondary-menu',
      'class' => array('links', 'inline', 'clearfix'),
    ),
    'heading' => array(
      'text' => t('Secondary menu'),
      'level' => 'h2',
      'class' => array('element-invisible'),
    ),
  )); ?>

</div>

<?php if ($main_menu) : ?>
  <div class="navlinks">
    <?php print theme('links__system_main_menu', array(
      'links' => $main_menu,
      'attributes' => array(
        'id' => 'main-menu',
        'class' => array('links', 'inline', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    )); ?>
  </div>
<?php endif; ?>

<table id="content">
  <tr>

  <?php if ($sidebar_first = drupal_render($page['sidebar_first'])) : ?>
    <td id="sidebar-first" class="sidebar">
      <?php print $sidebar_first; ?>
    </td>
  <?php endif; ?>

  <td id="main">

    <?php print render($page['highlighted']); ?>
    <?php print $breadcrumb; ?>
    <a id="main-content"></a>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h1 class="title" id="page-title"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php print $messages; ?>
    <?php if ($tabs = render($tabs)): ?>
      <div class="tabs"><?php print $tabs; ?></div>
    <?php endif; ?>
    <?php print render($page['help']); ?>
    <?php if ($action_links): ?>
      <ul class="action-links"><?php print render($action_links); ?></ul>
    <?php endif; ?>
    <?php print render($page['content']); ?>
    <?php print $feed_icons; ?>

    <?php print render($page['footer']); ?>

  </td>

  <?php if ($sidebar_second = drupal_render($page['sidebar_second'])) : ?>
    <td id="sidebar-second" class="sidebar">
      <?php print $sidebar_second; ?>
    </td>
  <?php endif; ?>

  </tr>
</table>

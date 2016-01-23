<?php if ($logo): ?>
  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
    <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/>
  </a>
<?php endif; ?>

<?php if ($site_name): ?>
  <h1><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a></h1>
<?php endif; ?>

<?php if ($site_slogan): ?>
  <h2><?php print $site_slogan; ?></h2>
<?php endif; ?>

<?php if ($page['header']): ?>
  <?php print render($page['header']); ?>
<?php endif; ?>

<?php print $breadcrumb; ?>

<?php if ($page['highlight']): ?>
  <?php print render($page['highlight']) ?>
<?php endif; ?>

<?php print render($title_prefix); ?>
<?php if ($title): ?>
  <h1><?php print $title; ?></h1>
<?php endif; ?>
<?php print render($title_suffix); ?>

<?php print $messages; ?>
<?php print render($page['help']); ?>

<?php if ($tabs): ?>
  <?php print render($tabs); ?>
<?php endif; ?>

<?php if ($action_links): ?>
  <ul><?php print render($action_links); ?></ul>
<?php endif; ?>

<?php print render($page['content']) ?>
<?php print $feed_icons; ?>

<?php if ($main_menu || $secondary_menu): ?>
  <?php print theme('links', array('links' => $main_menu, 'attributes' => array('id' => 'primary', 'class' => array('links', 'clearfix', 'main-menu')))); ?>
  <?php print theme('links', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary', 'class' => array('links', 'clearfix', 'sub-menu')))); ?>
<?php endif; ?>

<?php if ($page['sidebar_first']): ?>
  <?php print render($page['sidebar_first']); ?>
<?php endif; ?> <!-- /sidebar-first -->

<?php if ($page['sidebar_second']): ?>
  <?php print render($page['sidebar_second']); ?>
<?php endif; ?> <!-- /sidebar-second -->

<?php if ($page['footer']): ?>
  <?php print render($page['footer']); ?>
<?php endif; ?>


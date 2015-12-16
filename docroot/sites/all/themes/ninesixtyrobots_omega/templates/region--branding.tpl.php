<div<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
    <h1 id="logo-text"><a href="<?php print $front_page; ?>"><?php print $site_name; ?></a></h1>
    <p id="slogan"><?php print $site_slogan; ?></p>
  </div><!-- navigation -->
  <div id="nav">
    <?php print theme('links__system_main_menu', array(
      'links' => $main_menu,
      'attributes' => array(
        'id' => 'main-menu-links',
        'class' => array('links', 'clearfix'),
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
</div>
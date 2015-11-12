<div class="wrapper">
  <div id="header" class="clearfix">
    <div id="logo">
      <a href="<?php print $front_page;?>">
        <img src="<?php print $logo ?>" alt="<?php print $site_name;?> logo" />
      </a>
    </div>

    <?php print render($page['header']); ?>
  </div><!-- /#header -->

  <?php if ($page['highlight']): ?>  
    <div id="highlight">
      <?php print render($page['highlight']); ?>
    </div><!-- /#highlight -->
  <?php endif; ?> 

  <div id="content" class="clearfix">

    <div id="main_content" class="clearfix">
      <?php print render($messages); ?>
      <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      
      <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1><?php print $title; ?></h1>
        <?php endif; ?>
      <?php print render($title_suffix); ?>

      <?php print render($page['content']); ?>
    </div><!-- /#main_content -->

  </div>

  <div id="footer">
    <?php if ($page['footer']): ?>
      <?php print render($page['footer']); ?>
    <?php endif; ?>
  </div><!-- /#footer -->
</div>
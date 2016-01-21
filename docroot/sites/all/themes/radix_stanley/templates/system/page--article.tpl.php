<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 */
?>
<nav class="navbar navbar-inverse navbar-static-top" role="navigation">
  <div class="container">
    <div class="navbar-header">
      <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#navbar-collapse">
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>

      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" id="logo" class="pull-left">
          <img src="<?php print $logo; ?>" />
        </a>
      <?php endif; ?>

      <?php if ($site_name): ?>
        <a href="<?php print $front_page; ?>" class="navbar-brand">
          <?php print $site_name; ?>
        </a>
      <?php endif; ?>
      
    </div> <!-- /.navbar-header -->
    <div class="collapse navbar-collapse" id="navbar-collapse">
      <?php if ($main_menu): ?>
        <?php print render($main_menu); ?>
      <?php endif; ?>
    </div><!-- /.navbar-collapse -->
  </div>
</nav>

<?php if (!empty($page['welcome'])): ?>
  <div id="ww">
    <div class="container">
    <div class="row">
      <div class="col-lg-8 col-lg-offset-2 centered">
        <?php print render($page['welcome']); ?>
      </div><!-- /col-lg-8 -->
    </div><!-- /row -->
    </div> <!-- /container -->
  </div><!-- /ww -->
<?php endif; ?>

<div id="white">
  <div class="container">
  <div class="row">
    <div class="col-lg-8 col-lg-offset-2">
      <?php if ($messages): ?>
        <div id="messages">
          <?php print $messages; ?>
        </div>
      <?php endif; ?>

      <?php if ($title): ?>
        <div class="page-header centered">
          <h1 class="title"><?php print $title; ?></h1>
        </div>
      <?php endif; ?>

      <?php if ($tabs): ?>
        <div id="tabs">
          <?php print render($tabs); ?>
        </div>
      <?php endif; ?>

      <?php if ($action_links): ?>
        <?php print render($action_links); ?>
      <?php endif; ?>

      <?php print render($page['content']); ?>
    </div>
  </div><!-- /row -->
  </div> <!-- /container -->
</div><!-- /white -->

<div id="footer" class="footer">
  <div class="container">
    <div class="row">
      <div class="col-lg-4">
        <?php print render($page['footer_col_left']); ?>
      </div><!-- /col-lg-4 -->
      
      <div class="col-lg-4">
        <?php print render($page['footer_col_center']); ?>
      </div><!-- /col-lg-4 -->
      
      <div class="col-lg-4">
        <?php print render($page['footer_col_right']); ?>
      </div><!-- /col-lg-4 -->
    </div>
  </div>
</div>
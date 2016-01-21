<div class="page">

  <div class="head-part page-part inline-menu clearfix">

    <div class="column-c clearfix">
      <?php print render($page['ms_top_left']); ?>
      <?php print render($page['ms_top_right']); ?>
    </div>

    <div class="column-l clearfix">
      <a class="logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
        <img src="<?php print '/'.path_to_theme().'/logo.png'; ?>" alt="<?php print t('Home'); ?>">
      </a>
    </div>

  </div>

  <?php print render($page['ms_search']); ?>

  <div class="body-part page-part clearfix <?php if (!$page['ms_left']) print 'page-part-no-left'; ?>">

    <?php if ($page['ms_left']) { ?>
      <div class="column-l">
        <?php print render($page['ms_left']); ?>
      </div>
    <?php } ?>

    <div class="column-c">
      <?php print $breadcrumb; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title) { ?>
        <h1 class="page-title"><?php print $title; ?></h1>
      <?php } ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs) { ?>
        <div class="tabs clearfix">
          <?php print render($tabs); ?>
        </div>
      <?php } ?>
      <?php if ($messages) { ?>
        <div class="messages-all">
          <?php print $messages; ?>
        </div>
      <?php } ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links) { ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php } ?>
      <?php print render($page['content']); ?>
    </div>

  </div>

  <div class="foot-part page-part inline-menu clearfix">

    <div class="column-l">
      <?php print render($page['ms_counters']); ?>
    </div>

    <div class="column-c">
      <?php print render($page['ms_bottom_left']); ?>
      <?php print render($page['ms_bottom_right']); ?>
    </div>

  </div>

</div>
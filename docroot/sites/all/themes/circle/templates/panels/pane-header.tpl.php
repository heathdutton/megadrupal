<?php
/**
 * @file
 * Pane header template.
 */
?>
<div class="pe-header">
  <div class="pe-section clearfix">
    <div class="logo-wrapper">
      <?php if (!empty($logo)): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>
      <div class="name-and-slogan">
        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div class="site-name"><strong>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
            </strong></div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 class="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if (!empty($site_slogan)): ?>
          <div class="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div> <!-- /name-and-slogan -->
    </div> <!-- /logo-title -->

    <?php if (!empty($search_box)): ?>
      <div class="search-box"><?php print $search_box; ?></div>
    <?php endif; ?>
  </div> <!-- /section -->
</div> <!-- /header -->

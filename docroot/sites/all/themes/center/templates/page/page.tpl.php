<div id="page" class="<?php print $classes; ?>">

  <header id="header">
    <div class="l-constrained">
      <div id="branding">
        <h1><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><?php if ($logo): ?><img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" /><?php elseif ($site_name): ?><?php print $site_name; ?><?php endif; ?></a></h1>
        <?php if ($site_slogan): ?>
          <h2 id="site-slogan">
           <?php print $site_slogan; ?>
          </h2>
        <?php endif; ?>
      </div>
      <?php if ($page['utility']): ?>
        <div id="utility">
          <?php print render($page['utility']); ?>
        </div>
      <?php endif; // end utility ?>
      <?php if ($page['header']): ?>
          <?php print render($page['header']); ?>
      <?php endif; // end header ?>
    </div>
  </header>

  <?php if ($page['navigation']): ?>
    <div id="navigation" class="clearfix">
      <div class="l-constrained">
        <?php print render($page['navigation']); ?>
      </div>
    </div>
  <?php endif; // end navigation?>

  <div id="main">
    <div class="l-constrained">

      <?php if ($messages): ?>
        <div id="messages">
            <?php print $messages; ?>
        </div>
      <?php endif; // end messages ?>

      <?php if ($page['above_content']): ?>
        <div id="above-content">
          <div class="l-constrained">
            <?php print render($page['above_content']); ?>
          </div>
        </div>
      <?php endif; // end Above Content ?>

      <div id="main-content" class="clearfix">

        <?php if (render($tabs)): ?>
          <div id="tabs">
            <?php print render($tabs); ?>
          </div>
        <?php endif; // end tabs ?>

        <div id="content">

          <?php if ($page['highlighted']): ?>
            <div id="highlighted">
              <div class="l-constrained">
                <?php print render($page['highlighted']); ?>
              </div>
            </div>
          <?php endif; // end highlighted ?>

          <?php if (!$is_front && strlen($title) > 0): ?>
            <h1 <?php if (!empty($title_attributes)) print $title_attributes ?>>
              <?php print $title; ?>
            </h1>
          <?php endif; ?>

          <?php if ($page['help']): ?>
            <div id="help">
              <?php print render($page['help']); ?>
            </div>
          <?php endif; // end help ?>

          <?php print render($page['content']); ?>

        </div>

        <?php if ($page['sidebar_first']): ?>
          <div id="sidebar-first" class="aside">
            <?php print render($page['sidebar_first']); ?>
          </div>
        <?php endif; // end sidebar_first ?>

        <?php if ($page['sidebar_second']): ?>
          <div id="sidebar-second" class="aside">
            <?php print render($page['sidebar_second']); ?>
          </div>
        <?php endif; // end sidebar_second ?>
      </div>

      <?php if ($page['below_content']): ?>
        <div id="below-content">
          <div class="l-constrained">
            <?php print render($page['below_content']); ?>
          </div>
        </div>
      <?php endif; // end Below Content ?>

    </div>
  </div>

  <div id="footer">
    <div class="l-constrained">
      <?php print render($page['footer']); ?>
    </div>
  </div>

  <?php if ($page['admin_footer']): ?>
    <div id="admin-footer">
      <div class="l-constrained">
        <?php print render($page['admin_footer']); ?>
      </div>
    </div>
  <?php endif; // end admin_footer ?>

</div>

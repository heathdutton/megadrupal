<div id="wrapper">

  <!-- Header -->
  <div id="header">
    <!-- Logo -->
    <?php if (!empty($logo)): ?>
      <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" /></a>
    <?php endif; ?>

    <!-- Header -->
    <?php if (!empty($page['header'])): ?>
      <p><?php print render($page['header']); ?></p>
    <?php endif; ?>

    <!-- Navigation -->
    <div id="hright"  class="menu <?php if ($primary_nav): print "withprimary"; endif; if ($secondary_nav): print " withsecondary"; endif; ?> ">
      <?php if ($secondary_nav): ?> <div id="secondary" class="clearfix"> <?php print $secondary_nav; ?> </div> <?php endif; ?>
      <?php if ($primary_nav): ?> <div id="primary" class="clearfix"> <?php print $primary_nav; ?> </div> <?php endif; ?>
    </div>

    <!-- Site title - currently duped over the banner image -->
    <!--
    <div id="title">
      <?php if ($site_name): ?>
        <h1 id='home'> <a href="<?php /*print $base_path; */ ?>" title="<?php /* print t('Home'); */ ?>"><?php /* print $site_name; */ ?></a> </h1>
      <?php endif; ?>
    </div>
    -->

  </div>

  <!-- Site name / slogan - original design displayed site name above instead of here. -->
  <div id="homepic">
    <div class="message">
      <!-- Site name -->
      <?php if (!empty($site_name)): ?>
        <h4 id='site-name'><a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h4>
      <?php endif; ?>

      <!-- Site slogan -->
      <?php if (!empty($site_slogan)): ?>
        <p id='site-slogan'><?php print $site_slogan; ?></p>
      <?php endif; ?>
    </div>
  </div>

  <!-- top div -->
  <div id="bottomcontenttop"></div>

  <!-- Main content area -->
  <div id="bottomcontent">
    <div id="container" class="clearfix">

      <!-- Sidebar first -->
      <div class="sidebar-first">
        <?php if (!empty($page['sidebar_first'])): ?>
          <div id="sidebar-first" class="column sidebar"><?php print render($page['sidebar_first']); ?></div>
        <?php endif; ?>
      </div>

      <!-- Main -> squeeze -->
      <div id="main" class="middle column">
        <div id="squeeze">

          <!-- Mission statement -->
          <?php if (!empty($page['highlighted'])): ?><div id="mission"><?php print render($page['highlighted']); ?></div><?php endif; ?>

          <!-- Page title -->
          <?php print render($title_prefix); ?>
          <?php if (!empty($title)): ?><h1 class="title"><?php print $title; ?></h1><?php endif; ?>
          <?php print render($title_suffix); ?>

          <!-- Tabs -->
          <?php if (!empty($tabs)): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>

          <!-- Help -->
          <?php if (!empty($page['help'])): print render($page['help']); endif; ?>

          <!-- Messages -->
          <?php if (!empty($messages)): print $messages; endif; ?>

          <!-- Content -->
          <?php if (!empty($page['content'])): print render($page['content']); endif; ?>
        </div>
      </div>

      <!-- Sidebar second -->
      <?php if (!empty($page['sidebar_second'])): ?>
        <div id="sidebar-second" class="column sidebar"><?php print render($page['sidebar_second']); ?></div>
      <?php endif; ?>
    </div>
  </div>

  <!-- bottom div -->
  <div id="bottomcontentbtm"></div>

  <!-- Footer -->
  <?php if (!empty($page['footer'])): ?>
    <div id="footer"><?php print render($page['footer']); ?></div>
  <?php endif; ?>

</div>

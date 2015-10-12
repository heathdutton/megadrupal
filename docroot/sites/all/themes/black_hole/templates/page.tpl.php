<div id="bg1"><div id="bg2">

<div id="top_bg" class="page0">
<div class="sizer0">
<div id="topex" class="expander0">
<div id="top_left">
<div id="top_right">
<div id="headimg">

<div id="above" class="clearfix">
  <?php if ($page['above']): ?><?php print render ($page['above']); ?><?php endif; ?>
</div>

<div id="header" class="clearfix">
  <div id="top-elements">
  	<?php if ($page['search']): ?>
  		<div id="search"><?php print render ($page['search']); ?></div>
  	<?php endif; ?>
    <?php if (function_exists('toplinks')): ?>
      <div id="toplinks"><?php print toplinks() ?></div>
    <?php endif; ?>
  	<?php if (theme_get_setting('social_links_display')): ?>
      <div id="toplinks"><?php print black_hole_social_links(); ?></div>
    <?php endif; ?>
    <?php if (theme_get_setting('loginlinks')): ?>
      <div id="user_links"><?php print black_hole_login() ?></div>
    <?php endif; ?>
  	<?php if ($page['banner']): ?>
  		<div id="banner" class="clearfix"><?php print render ($page['banner']); ?></div>
  	<?php endif; ?>
  </div><!-- /top-elements -->
  <div id="logo">
  <?php if ($logo): ?>
    <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
      <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
    </a>
  <?php endif; ?>
  </div> <!-- /logo -->
  <div id="name-and-slogan">
  <?php if ($site_name): ?>
    <?php if ($title): ?>
      <p id="site-name"><a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></p>
    <?php else: ?>
      <h1 id="site-name"><a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
    <?php endif; ?>
  <?php endif; ?>
  <?php if ($site_slogan): ?>
    <div id="site-slogan"><?php print $site_slogan; ?></div>
  <?php endif; ?>
  </div> <!-- /name-and-slogan -->

<div class="brclear"></div>

<?php if ($page['header']): ?>
	<?php print render ($page['header']); ?>
<?php endif; ?>

<?php if ($main_menu || $page['dropdown']): ?>
  <div id="navlinks" class="<?php print menupos() ?>">
  <?php if (!empty($page['dropdown'])) { ?><?php print render($page['dropdown']); ?><?php } ?>
  <?php if (empty($page['dropdown'])) { ?>
  <?php print theme('links__system_main_menu', array(
            'links' => $main_menu,
            'attributes' => array(
              'id' => 'navlist',
              'class' => array('links', 'clearfix'),
            ),
            'heading' => array(
              'text' => t('Main menu'),
              'level' => 'h2',
              'class' => array('element-invisible'),
            ),)); ?><?php } ?>
  </div>
<?php endif; ?>
</div> <!-- /header -->

</div>
</div><!-- /top_right -->
</div><!-- /top_left -->
</div><!-- /expander0 -->
</div><!-- /sizer0 -->
</div><!-- /page0 -->

<div id="body_bg" class="page0">
<div class="sizer0">
<div class="expander0">
<div id="body_left">
<div id="body_right">

<?php if ($secondary_menu): ?>
  <div class="<?php print menupos() ?>">
  <?php print theme('links__system_secondary_menu', array(
    'links' => $secondary_menu,
    'attributes' => array(
      'id' => 'subnavlist',
      'class' => array('links', 'clearfix'),
    ),
    'heading' => array(
      'text' => t('Secondary menu'),
      'level' => 'h2',
      'class' => array('element-invisible'),
    ),
  )); ?>
  </div>
<?php endif; ?>

<div id="breadcrumb">
  <?php print $breadcrumb; ?>
</div>

<?php if($page['user1'] || $page['user2'] || $page['user3'] || $page['user4']) : ?>
<div id="section1" class="sections col<?php print (bool) $page['user1'] + (bool) $page['user2'] + (bool) $page['user3'] + (bool) $page['user4']; ?> clearfix">
  <?php if($page['user1']) : ?><div class="section u1"><?php print render ($page['user1']); ?></div><?php endif; ?> 
  <?php if($page['user2']) : ?><div class="section u2 <?php print divider() ?>"><?php print render ($page['user2']); ?></div><?php endif; ?> 
  <?php if($page['user3']) : ?><div class="section u3 <?php print divider() ?>"><?php print render ($page['user3']); ?></div><?php endif; ?> 
  <?php if($page['user4']) : ?><div class="section u4 <?php print divider() ?>"><?php print render ($page['user4']); ?></div><?php endif; ?> 
</div>  <!-- /section1 -->
<?php endif; ?>

<div id="middlecontainer">
  <div id="wrapper">
    <div class="outer">
      <div class="float-wrap">
        <div class="colmain">
          <div id="main">
            <?php if ($page['highlighted']): ?><div id="mission"><?php print render ($page['highlighted']); ?></div><?php endif; ?>
            <?php print render($title_prefix); ?>
            <?php if ($title): if ($is_front){ print '<h2 class="title">'. $title .'</h2>'; } else { print '<h1 class="title">'. $title .'</h1>'; } endif; ?>
            <?php print render($title_suffix); ?>
            <div class="tabs"><?php print render($tabs); ?></div>
            <?php print render($page['help']); ?>
            <?php print $messages ?>
            <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
            <?php if ($page['content']) : ?><?php print render ($page['content']); ?><?php endif; ?>
            <?php print $feed_icons; ?>
          </div>
        </div> <!-- /colmain -->
        <?php if ($page['sidebar_first']) { ?>
          <div class="colleft">
            <div id="sidebar-left"><?php print render($page['sidebar_first']); ?></div>
          </div>
        <?php } ?>
        <br class="brclear" />
      </div> <!-- /float-wrap -->
      <?php if ($page['sidebar_second']) { ?>
        <div class="colright">
          <div id="sidebar-right"><?php print render($page['sidebar_second']); ?></div>
        </div>
      <?php } ?>
      <br class="brclear" />
    </div><!-- /outer -->
  </div><!-- /wrapper -->
</div>

<div id="bar"></div>

<?php if($page['user5'] || $page['user6'] || $page['user7'] || $page['user8']) : ?>
<div id="section2" class="sections col<?php print (bool) $page['user5'] + (bool) $page['user6'] + (bool) $page['user7'] + (bool) $page['user8']; ?> clearfix">
  <?php if($page['user5']) : ?><div class="section u5"><?php print render ($page['user5']); ?></div><?php endif; ?> 
  <?php if($page['user6']) : ?><div class="section u6 <?php print divider() ?>"><?php print render ($page['user6']); ?></div><?php endif; ?> 
  <?php if($page['user7']) : ?><div class="section u7 <?php print divider() ?>"><?php print render ($page['user7']); ?></div><?php endif; ?> 
  <?php if($page['user8']) : ?><div class="section u8 <?php print divider() ?>"><?php print render ($page['user8']); ?></div><?php endif; ?> 
</div>  <!-- /section2 -->
<?php endif; ?>

<?php if ($main_menu): ?>
  <?php print theme('links__system_main_menu', array(
            'links' => $main_menu,
            'attributes' => array(
              'id' => 'navlist2',
              'class' => array('links', 'clearfix'),
            ),
            'heading' => array(
              'text' => t('Main menu 2'),
              'level' => 'h2',
              'class' => array('element-invisible'),
            ),)); ?>
<?php endif; ?>

</div><!-- /body_right -->
</div><!-- /body_left -->
</div><!-- /expander0 -->
</div><!-- /sizer0 -->
</div><!-- /page0 -->

<div class="eopage">
<div id="bottom_bg" class="page0">
<div class="sizer0">
<div class="expander0">
<div id="bottom_left">
<div id="bottom_right">

<div id="footer"  class="clearfix">
  <div class="legal">
    <?php if ($page['footer']): ?><?php print render ($page['footer']); ?><?php endif; ?>
    <div id="brand"></div>
  </div>
</div>

<div id="belowme">
</div>

</div><!-- /bottom_right -->
</div><!-- /bottom_left -->
</div><!-- /expander0 -->
</div><!-- /sizer0 -->
</div><!-- /page0 -->
</div>

</div></div><!-- /bg# -->
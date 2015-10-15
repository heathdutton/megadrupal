<div id="page">
  <div class="ui transparent inverted main top collapsible menu">
    <div class="header fl-left">
        <?php if ($logo): ?>
          <a class="logo fl-left" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>
        <?php if (!empty($site_name)): ?>
          <a class="name fl-left item" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
        <?php endif; ?>
    </div>
    <div class="ui item mini left floated nomargin collapse icon borderless"><i class="white reorder large icon"></i></div>
    <?php if (!empty($secondary_nav)): ?>
      <div class="secondary fl-right">
        <?php print render($secondary_nav); ?>
      </div>
    <?php endif; ?>
    <?php if(isset($loginpopup)){
      print render($loginpopup);
    }
    if(isset($registerpopup)){
      print render($registerpopup);
    }
    ?>
    <?php if ($user->uid == 0):?>
      <div class="secondary fl-right">
        <?php if(isset($login_user)){
            print render($login_user);
          }
          if(isset($register_user)){
            print render($register_user);
          }
        ?>
      </div>
    <?php endif; ?>  
    <?php if (!empty($primary_nav)): ?>
      <div class="navbar">
        <nav role="navigation">
          <?php if (!empty($primary_nav)): ?>
            <div class="primary fl-left">
              <?php print render($primary_nav); ?>
            </div>
          <?php endif; ?>
        </nav>
        </div>
    <?php endif; ?>
  </div>

  <?php if (!empty($page['header'])): ?>
    <div class="top"> <?php print render($page['header']); ?>
    </div>
  <?php endif; ?><!-- /.header  -->

  <!--icon for sidebar  -->
  
  <div class="<?php print $icon_hide;?>ui black huge launch right attached button sidebar-icon first">
        <i class="icon reorder"></i>
  </div>
  <div class="<?php print $stacked_sidebars;?>ui grid content main">
    <?php if (!empty($page['sidebar_first'])): ?>
      <aside class="<?php print $sidebar_left;?> sidebars ui sidebar-first" role="complementary">
        <div class="sidebar-inner">
          <?php print render($page['sidebar_first']); ?>
        </div>
      </aside>  <!-- /#sidebar-second -->
    <?php endif; ?>

      <section class="<?php print $main_grid; ?> main-content" role="main">
        <?php if (!empty($page['highlighted'])): ?>
          <div class="highlighted ui massive message"><?php print render($page['highlighted']); ?></div>
        <?php endif; ?>
        <?php if (!empty($breadcrumb)): print $breadcrumb; endif;?>
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if (!empty($title)): ?>
          <!--<h1 class="ui dividing header page-title"><?php //print $title; ?></h1>-->
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php print $messages; ?>
        <?php if (!empty($tabs)): ?>
          <?php print render($tabs); ?>
        <?php endif; ?>
        <?php if (!empty($page['help'])): ?>
          <?php print render($page['help']); ?>
        <?php endif; ?>
        <?php if (!empty($action_links)): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
        <?php print render($page['content']); ?>
	    </section>

      <?php if (!empty($page['sidebar_second'])): ?>
      <aside class="<?php print $sidebar_right; ?> sidebars ui sidebar-last right" role="complementary">
        <div class="sidebar-inner ui raised segment">
          <?php print render($page['sidebar_second']); ?>
        </div>
        <div class="<?php print $icon_hide;?>ui black huge launch left attached button sidebar-icon last">
          <i class="icon reorder"></i>
        </div>
      </aside>  <!-- /#sidebar-second -->
      
    <?php endif; ?>
    
    </div> <!-- /main  -->
 <div class="ui divider"></div>
  <div class="ui page grid">
    <div class="column page">
      <?php print render($page['footer']); ?>
    </div> <!-- /footer -->
  </div> <!-- /.footer column -->

</div> <!-- /#page -->

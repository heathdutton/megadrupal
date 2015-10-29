<div id="page" class="container"> 
  <?php print render($page['leaderboard']); ?>
  <?php print $messages; ?>
  <!-- old template header-->
  <div id="header" class="clear-block">
    <div id="header-inner">
      <?php if ($linked_site_logo): ?> 
        <?php print $linked_site_logo ?>
      <?php endif ?>
    </div>
    <div class="account_list">
    <?php 
      print theme('links', array('links' => $xc_secondary_links,'attributes' => array('class' => array('account_links', 'inline'))));
    ?>
    </div>
    <div id="header-links-primary">
    <?php 
      print theme('links', array('links' => $xc_primary_links,'attributes' => array('class' => array('account_links', 'inline'))));
    ?>
    </div>
  </div>
  <!-- end old header-->

  <?php print render($page['menu_bar']); ?>
  <?php //if ($primary_navigation): print $primary_navigation; endif; ?>
  <?php //if ($secondary_navigation): print $secondary_navigation; endif; ?>

  <?php if ($breadcrumb): ?>
    <section id="breadcrumb"><?php print $breadcrumb; ?></section>
  <?php endif; ?>

  <?php print render($page['help']); ?>

  <?php //print render($page['secondary_content']); ?>

  <div id="columns"><div class="columns-inner clearfix">
    <div id="content-column">
      <?php 
        if($tabs['#primary']){
          $content_layout = 'content-inner tabs' ;
        } else {
          $content_layout = 'content-inner';
        }
        if($is_node_page == TRUE){
            $content_layout .= ' node_page';
        }
      ?>
      <div class="<?php print($content_layout);?>">

      <?php print render($page['highlighted']); ?>

      <?php $tag = $title ? 'section' : 'div'; ?>
      <<?php print $tag; ?> id="main-content">

        <?php if ($title || $primary_local_tasks || $secondary_local_tasks || $action_links = render($action_links)): ?>
          <header> 
            <!--show the search form in header-->
            <?php if ($tabs['#primary']): ?>
              <div class="tabs"><ul class="tabs primary"><?php print(render($tabs['#primary']));?></ul></div>
            <?php endif ?>
            <?php print render($page['header']); ?>
             <!-- Tabs -->
            
          </header>
        <?php endif; ?>

        <div id="content">
          <div id="content_bottom">
            <?php print render($page['content_top']); ?>
          </div>
          <?php print render($page['content']); ?>
          <div id="content_bottom">
            <?php print render($page['content_bottom']); ?>
          </div>
        </div>

        <?php print $feed_icons; ?>

      </<?php print $tag; ?>>

      <?php print render($page['content_aside']); ?>

    </div></div>

    <?php print render($page['sidebar_first']); ?>
    <?php print render($page['sidebar_second']); ?>

  </div></div>

  <?php print render($page['tertiary_content']); ?>

  <?php if ($page['footer']): ?>
    <footer><?php print render($page['footer']); ?></footer>
  <?php endif; ?>

</div>
<!--- New template -->

<?php
// $Id $
?>
<div id="wrapper" <?php print $attributes; ?>>

  <!-- Header -->
  <div id="header">
    <?php if ($page['header']): ?>
      <?php print render($page['header']); ?>
    <?php endif; ?>
    <?php
      // prepare header
      $site_fields = array();
      if ($site_name):
        $site_fields[] = check_plain($site_name);
      endif;
      if ($site_slogan):
        $site_fields[] = check_plain($site_slogan);
      endif;
      $site_title = implode(' ', $site_fields);
    ?>

    <!-- Site Name -->
    <?php if ($site_name): ?>
      <h1 id="site-name"><a href="<?php print url(); ?>" title="<?php print $site_title; ?>"><?php print($site_name); ?></a></h1>
    <?php endif; ?>

    <!-- Site Slogan -->
    <?php if ($site_slogan): ?>
      <div class="site-slogan"><?php print($site_slogan); ?></div>
    <?php endif; ?>

    <!-- Primary Links -->
    <?php if (isset($main_menu)): ?>
      <div id="primary" class="menu<?php if ($site_slogan) { print ' withslogan'; } ?>">
        <?php print theme('menu_links', $main_menu); ?>
     </div>
     <?php endif; ?>

    <!-- Secondary Links -->
    <?php if (isset($secondary_menu)): ?>
      <?php
        $secondary_class = '';
        if ($main_menu && $site_slogan):
          $secondary_class = 'withprimaryslogan';
        elseif ($main_menu):
          $secondary_class = 'withprimary';
        elseif ($site_slogan):
          $secondary_class = 'withslogan';
        endif;
      ?>
      <div id="secondary" class="<?php print $secondary_class; ?> menu">
        <?php print theme('menu_links', $secondary_menu); ?>
      </div>
    <?php endif; ?>
  </div>


  <!-- Main content -->
  <div id="content" <?php print $content_attributes; ?>>

    <!-- Breadcrumb -->
    <?php if (!empty($breadcrumb)): ?>
      <div id="breadcrumb"><?php print $breadcrumb ?></div>
    <?php endif; ?>

    <!-- Page title -->
    <?php print render($title_prefix); ?>
    <?php if (!empty($title)): ?>
      <h2 class="content-title" <?php print $title_attributes; ?>><?php print $title ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    
    <!-- Navigation tabs -->
    <?php if (!empty($tabs)): ?>
      <?php print render($tabs); ?>
    <?php endif; ?>
         
    <!-- Mission statement -->
    <?php if (!empty($mission)): ?>
      <p id="mission"><?php print $mission; ?></p>
    <?php endif; ?>
         
    <!-- Help text -->
    <?php if (!empty($page['help'])): ?>
      <p id="help"><?php print render($page['help']); ?></p>
    <?php endif; ?>
         
    <!-- Messages -->
    <?php if (!empty($messages)): ?>
      <div id="message"><?php print $messages; ?></div>
    <?php endif; ?>
    
    
    <!-- Actual page content -->
    <?php print render($page['content']); ?>
  
  </div>

  
  <!-- Sidebar -->
  <div id="sidebar">
    <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-first"><?php print render($page['sidebar_first']); ?></div>
    <?php endif; ?>
    <?php if ($page['sidebar_second']): ?>
      <div id="sidebar-second"><?php print render($page['sidebar_second']); ?></div>
    <?php endif; ?>
  </div>
  
  <!-- Footer -->
  <div id="footer">
    <p>
    Design By: <a href="http://beccary.com" title="Theme designed by Beccary">Beccary</a>
    and: <a href="http://scarto.abshost.net" title="Modified for Drupal by Scar_T">Scar_T</a>
    and <a href="http://www.stellapowerdesign.net" title="Currently maintained by Stella Power">snpower</a>
    </p>
    <?php if ($page['footer']): ?>
      <?php print render($page['footer']); ?>
    <?php endif; ?>
  </div>

</div>


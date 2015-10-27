<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  
  <!--[if lte IE 8]>
  <style type="text/css" media="all">
    .node,#sidebar .block,.social-links,.comment,.comment-form,ul.tabs li a,.poll .bar,.poll .bar .foreground,.ui-dialog, #top-menu-inner,#footer .content{behavior: url(<?php print $theme_path; ?>/ie-fixes/css3pie.htc);}
    .node-navigation {behavior: url(<?php print $theme_path; ?>/ie-fixes/display-table.min.htc);}
  </style>
  <![endif]-->

  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
<?php
// $Id:$
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
  <title><?php print $head_title ?></title>
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
  <!--[if lte IE 7]><link rel="stylesheet" type="text/css" href="/styleIE.css" /><![endif]-->
  <!--[if lt IE 7]><link rel="stylesheet" type="text/css" href="/styleIE6.css" /><![endif]-->
  <!--[if lt IE 7]><script src="http://ie7-js.googlecode.com/svn/version/2.0(beta3)/IE7.js" type="text/javascript"></script><![endif]-->
</head>
<body class="<?php print $classes; ?> page" <?php print $attributes;?>>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>

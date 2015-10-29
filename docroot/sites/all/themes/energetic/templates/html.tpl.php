<?php // $Id: $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php echo $language->language; ?>" xml:lang="<?php echo $language->language; ?>">

  <head>
    <title><?php print $head_title; ?></title>
    <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
    <?php print $head; ?>
    <?php print $styles; ?>
    <?php print $scripts; ?>
    <!--[if IE 6]><link rel="stylesheet" href="<?php print base_path() . path_to_theme(); ?>/style.ie6.css" type="text/css" /><![endif]-->  
    <!--[if IE 7]><link rel="stylesheet" href="<?php print base_path() . path_to_theme(); ?>/style.ie7.css" type="text/css" media="screen" /><![endif]-->
    <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
  </head>

  <body class="<?php print $classes; ?>" <?php print $attributes; ?>>
    <?php print $page_top; ?>
    <?php print $page; ?>
    <?php print $page_bottom; ?>
  </body>
</html>

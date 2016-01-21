<?php

/**
 * @file
 * Default dynamic template for the KBC Paypage.
 *
 * This template will be used to generate the static kbc-dynamic-template.html
 * template.
 *
 * Available variables:
 * - $css: CSS code to be used on the KBC Paypage.
 * - $logo: logo to be displayed on the KBC Paypage.
 */
?>
<html>
<head>
  <title>KBC Paypage</title>
</head>
<style type="text/css">
  <?php
    print $css;
  ?>
</style>
<body>
  <div id="header">
    <!--Logo-->
    <div class="logo">
      <?php
        print $logo;
      ?>
    </div>
    <h1><?php print $title; ?></h1>
  </div>
  <div id="content">
    <div id="payment_zone">
      $$$PAYMENT ZONE$$$
    </div>
  </div>
  <div id="footer">
  </div>
</body>
</html>

<?php
/**
 * @file
 *  wildfire_example_plain.tpl.php
 *
 * Plain example template for Wildfire
 *
 * Original art and code:
 * @author Paul Lomax <paul@tiger-fish.com>
 *
 * Code for Wildfire 2:
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * This template uses the most minimal features
 *
 */
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title><?php print $message['name']; ?></title>
</head>

<body bgcolor="#ffffff">

  <table width="550" cellspacing="10" cellpadding="0" border="0" align="center" style="font-family: Arial, Helvetica, sans-serif;">
    <tr>
      <td style="color:#999; font-size: 11px;">
        You're receiving this newsletter because you signed up on <?php print variable_get('site_name', ''); ?>.
        If you can't read this email, you can <a href="{{wildfire:browserview}}" style="color: #0084bb;">view it in your browser</a>.
        Not interested any more? <a href="{{wildfire:unsubscribe}}" style="color: #0084bb;">Unsubscribe now</a>.
      </td>
    </tr>
    <tr>
      <td><hr size="1" color="#dddddd" /></td>
    </tr>
    <tr>
      <td style="color: #555; font-size: 12px; line-height: 18px;">

        <!-- Message title -->
        <h1><?php print $message['title']; ?></h1>

        <!-- Area for snippets[content] -->
        <?php if (isset($snippets_content)) :?>
          <?php print $snippets_content[0]; ?>
        <?php endif; ?>

      </td>
    </tr>
    <tr>
      <td><hr size="1" color="#dddddd" /></td>
    </tr>
    <tr>
      <td style="color:#999; font-size: 11px;">
        <!-- Subscription options -->
        <p>If you have received this email and would like to unsubscribe, please
          <a href="{{wildfire:unsubscribe}}" style="color: #0084bb;">unsubscribe now</a>, or
          <?php print l(t('log in'), 'user/login', array('absolute' => TRUE, 'attributes' => array('style' => 'color: #0084bb;'))); ?>
          and manage your subscription preferences.</p>
        <p>All contents copyright &copy; <?php print variable_get('site_name', ''); ?> <?php print date("Y"); ?></p>
      </td>
    </tr>
  </table>

</body>
</html>

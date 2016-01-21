<?php
/**
 * @file
 *  wildfire_example_newsletter.tpl.php
 *
 * Example template for Wildfire
 *
 * Original art and code:
 * @author Paul Lomax <paul@tiger-fish.com>
 *
 * Code for Wildfire:
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * This mail template uses most of the basic features available within Wildfire
 * mail templates. You can use this as a basis for your own templates, or you
 * can create your own from scratch following the advice below.
 *
 * There's a couple of pointers to be aware of with creating your own templates
 *
 * 1/ Include "view on the web" and "unsubscribe" tokens
 *  Tokens for replacement should be added where appropriate; these will be
 *  replaced by the real content by the Wildfire HQ server when it performs the
 *  mail out.
 *
 *  The most common tokens to use are:
 *
 *  {{wildfire:browserview}}
 *    Provides a link to the "view on the web" version of the email
 *  {{wildfire:unsubscribe}}
 *    A one-click link for the user to unsubscribe from future mailings
 *  {{wildfire:email}}
 *    The users actual email address (useful for text such as "this email was
 *    sent to <email>" in your template).
 *
 *  ============================================================================
 *  AT LEAST ONE INSTANCE OF THE {{wildfire:unsubscribe}} TOKEN SHOULD BE
 *  PRESENT IN THE CONTENT OF THE TEMPLATE.
 *
 *  Bulk email rules in many territories require that you provide an
 *  unsubscribe link within your bulk mailing, so there should always be at
 *  least one instance within the template.
 *  ============================================================================
 *
 * 2/ Check, and double check, the email renders across many different clients.
 * There's a lot greater variance in standards compliance in Email Clients than
 * in Web Browsers, so while it would be tidier to use CSS in a <style> block
 * at the top of the page markup, many email clients don't like it (including
 * big names like Google Mail, as of August 2012). Hence most of the styles
 * for the layout of the mail are inline, and we still use tables for layout
 * for rendering the template more or less reliably across many different
 * potential clients.
 *
 * There are many third-party checkers for this if you don't want to do all of
 * the leg work yourself (e.g. http://litmus.com/ and many others). It's wise
 * to check that all of your prospective clients will be able to view your
 * email correctly before you do your fist send.
 *
 * 3/ Send yourself a test broadcast from Wildfire using the new template
 *  This is the "final stage", in that the results will be exactly as a live
 *  send would appear, so you can verify everything is correct before
 *  performing your first live send.
 */
?>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title><?php print $message['name']; ?></title>
</head>

<body bgcolor="#ffffff">

<table width="550" bgcolor="#ffffff" cellspacing="0" cellpadding="0" border="0" align="center" style="font-family: <?php print $styles['font-family'] ?>; font-size: 12px;">
  <tr>
    <td colspan="3" style="padding: 10px 20px; color:#999; font-size: 10px; text-align: center;">
      You're receiving this newsletter because you signed up on <?php print variable_get('site_name', ''); ?>. It was sent to {{wildfire:email}}.
      If you can't read this email, you can <a href="{{wildfire:browserview}}" style="<?php print $linkstyle; ?>">view it in your browser</a>.
      Not interested any more? <a href="{{wildfire:unsubscribe}}" style="<?php print $linkstyle; ?>">Unsubscribe now</a>.
    </td>
  </tr>
  <tr>
    <td colspan="3" bgcolor="<?php print $colors['top-banner']; ?>">

      <!-- Main header -->
      <table cellspacing="10" cellpadding="0" border="0" width="100%">
        <tr>
          <td valign="bottom"><?php print $images['header']; ?></td>
          <td valign="bottom">
            <!-- Base the title off the broadcast name  -->
            <h1 style="font-size: 1.5em; font-weight: normal; text-transform: uppercase; color: <?php print $colors['top-banner-text']; ?>; margin:0; letter-spacing: -0.05em;">
              <?php print $message['name']; ?>
            </h1>
          </td>
          <td valign="bottom" align="right" style="color: #444; font-size: 12px;"><?php print date('j M Y'); ?></td>
        </tr>
      </table>

    </td>
  </tr>
  <tr>
    <td colspan="3" bgcolor="<?php print $colors['sub-banner']; ?>" style="color:<?php print $colors['sub-banner-text']; ?>; padding: 10px 15px; text-transform: uppercase; font-size: 10px; letter-spacing: 3px;">
      <!-- Subject line -->
      <?php print $message['title']; ?>
    </td>
  </tr>
  <tr>
    <td style="color:<?php print $colors['body-text']; ?>; line-height: 18px;" valign="top" width="340">
      <br />

      <!-- Editors note snippet -->
      <?php if (isset($snippets_editors_note)) :?>
        <?php print $snippets_editors_note; ?>
      <?php endif; ?>

      <hr size="1" color="#ddd" style="margin: 2em 0;" />

      <!-- Main repeater -->
      <?php if (isset($repeaters_main)) : ?>
        <?php foreach ($repeaters_main as $article) :?>

          <!-- Article title -->
          <div style="font-size: 1.5em; font-weight: normal;">
            <?php print l(t($article->title), 'node/' . $article->nid, array('absolute' => TRUE, 'attributes' => array('style' => $linkstyle))); ?>
          </div>

          <!-- Article creation date -->
          <div><?php print format_date($article->created, 'long'); ?></div>

          <!-- Render the image if it exists -->
          <?php if (!empty($article->field_image)) : ?>
            <div style="float: left; margin: 15px 15px 15px 0;"><?php print theme('wildfire_field_render', array('entity' => $article, 'field' => 'field_image', 'display' => 'wildfire')); ?></div>
          <?php endif; ?>

          <!-- Render the body using the teaser display -->
          <div><?php print theme('wildfire_field_render', array('entity' => $article, 'field' => 'body', 'display' => 'wildfire')); ?></div>

          <?php print l(t('Read the whole entry'), 'node/' . $article->nid, array('absolute' => TRUE, 'attributes' => array('style' => $linkstyle))); ?>

          <hr size="1" color="#ddd" style="margin: 2em 0;" />

        <?php endforeach; ?>
      <?php endif; ?>

    </td>
    <td width="30">
      <?php print $images['spacer']; ?>
    </td>
    <td style="color: <?php print $colors['body-text']; ?>;" width="180" valign="top">
      <br />

      <!-- Main repeater index block -->
      <?php if (isset($repeaters_main)) :?>
        <?php print $images['sidebar-issue']; ?>
        <ul style="margin: 0; padding: 0; list-style: none; border: solid 1px <?php print $colors['borders']; ?>;">
          <?php foreach ($repeaters_main as $article) :?>
            <li style="border-bottom: solid 1px <?php print $colors['borders']; ?>; padding: 5px 10px; margin: 0;">
              <?php print l(t($article->title), 'node/' . $article->nid, array('absolute' => TRUE, 'attributes' => array('style' => $linkstyle))); ?>
            </li>
          <?php endforeach; ?>
        </ul>
        <br />
      <?php endif; ?>

      <!-- Contact details -->
      <?php print $images['sidebar-contact']; ?>
      <div style="padding: 15px 10px; border: solid 1px <?php print $colors['borders']; ?>;">
        234 Example street<br />
        Example road<br />
        Some Town<br />
        Somewhere<br />
        ABC 123A
        <?php print $contact; ?>
      </div>
      <br />

      <!-- Unsubscribe -->
      <?php print $images['sidebar-unsubscribe']; ?>
      <div style="padding: 15px 10px; border: solid 1px <?php print $colors['borders']; ?>;">
        Don't want to receive emails any more?
        <a href="{{wildfire:unsubscribe}}" style="<?php print $linkstyle; ?>">Unsubscribe</a>
      </div>

    </td>
  </tr>
  <tr>
    <td colspan="3" style="color: <?php print $colors['footer-text']; ?>; font-size: 10px; text-align: center;">

      <br />
      <!-- Footer information -->
      <p>If you have received this email and would like to unsubscribe, please
        click the unsubscribe link in your email, or <?php print $login; ?>
        and manage your subscription preferences.</p>
      <p>All contents copyright &copy; <?php print variable_get('site_name', ''); ?> <?php print date("Y"); ?></p>

    </td>
  </tr>
</table>

</body>
</html>

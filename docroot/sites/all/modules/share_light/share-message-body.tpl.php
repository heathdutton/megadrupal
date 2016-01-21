<?php
/**
 * This template should only contain the contents of the body
 * of the email, what would be inside of the body tags, and not
 * the header.  You should use tables for layout since Microsoft
 * actually regressed Outlook 2007 to not supporting CSS layout.
 * All styles should be inline.
 *
 * For more information, consult this page:
 * http://www.anandgraves.com/html-email-guide#effective_layout
 */
?>
<html>
  <body>
    <table width="400px" cellspacing="0" cellpadding="10" border="0">
      <tbody>
        <tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php if ($values['message']): ?>
            <p><?php print t('Message from Sender'); ?></p><p><?php print $values['message']; ?></p>
            <?php endif; ?>
            <?php echo $values['footer']; ?>
          </td>
        </tr>
      </tbody>
    </table>
  </body>
</html>
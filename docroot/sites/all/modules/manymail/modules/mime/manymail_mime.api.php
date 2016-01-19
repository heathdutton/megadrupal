<?php
/**
 * @file
 * Hooks provided by ManyMail MIME module.
 */

/**
 * Alter the HTML output of the MIME submodule.
 *
 * Use this function if you need to change the render array that is
 * eventually used by the MIM submodule to construct the mail body.
 *
 * @param array $html
 *   The render array constructed in manymail_mime_manymail_mail_alter().
 * @param PHPMailer $mail
 *   The PHPMailer instance created by ManyMail.
 * @param array $meta
 *   The metadata gathered by hook_manymail_mail_meta().
 *
 * @see manymail_send_mail()
 * @see hook_manymail_mail_alter()
 * @see manymail_mime_manymail_mail_alter()
 */
function hook_manymail_mime_mail_alter(&$html, $mail, $meta) {
  // Change title and add a footer.
  $html['head']['title']['#markup'] = 'My branding: ' . $html['head']['title']['#markup'];
  $html['body']['#markup'] .= '<footer>Hello world</footer>';
}

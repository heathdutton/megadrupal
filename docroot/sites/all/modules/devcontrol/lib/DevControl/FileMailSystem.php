<?php

/**
 * @file
 * MailSystemInterface for logging mails to the filesystem.
 *
 * To enable, save a variable in settings.php (or otherwise) whose value
 * can be as simple as:
 *
 * $conf['mail_system'] = array(
 *   'default-system' => 'DevControl_FileMailSystem',
 * );
 *
 * Saves to temporary://devel-mails dir by default. Can be changed using
 * 'devcontrol_mail_path' variable.
 *
 * IMPORTANT NOTE: This work is the work of the Devel module developers, they
 * deserve all the credit for it. This file is a copy of their own code, a bit
 * modified for customization purposes.
 */
class DevControl_FileMailSystem extends DefaultMailSystem
{
    /**
     * @var string
     */
    protected $ouputDirectory;

    public function composeMessage($message)
    {
        $mimeheaders = array();
        $message['headers']['To'] = $message['to'];
        foreach ($message['headers'] as $name => $value) {
            $mimeheaders[] = $name . ': ' . mime_header_encode($value);
        }
  
        $line_endings = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
        $output = join($line_endings, $mimeheaders) . $line_endings;
        $output .= $message['subject'] . $line_endings;
        $output .= preg_replace('@\r?\n@', $line_endings, $message['body']);

        return $output;
    }

    public function getFileName($message)
    {
        $tokens = array(
            '%to' => $message['to'],
            '%subject' => $message['subject'],
            '%datetime' => date('y-m-d-his'),
        );

        $filename = strtr('%datetime-%to-%subject.mail.out', $tokens);
        $filename = preg_replace('/[^a-zA-Z0-9_\-\.@]/', '_', $filename);

        return $this->getOutputDirectory() . '/' . $filename;
    }

    /**
     * Save an e-mail message to a file, using Drupal variables and default settings.
     *
     * @see http://php.net/manual/en/function.mail.php
     * @see drupal_mail()
     *
     * @param $message
     *   A message array, as described in hook_mail_alter().
     * @return
     *   True if the mail was successfully accepted, otherwise false.
     */
    public function mail(array $message)
    {
        $output = $this->composeMessage($message);
        $output_file = $this->getFileName($message);

        return file_put_contents($output_file, $output);
    }

    public function getOutputDirectory()
    {
        if (!isset($this->ouputDirectory)) {
            $this->ouputDirectory = variable_get('devcontrol_mail_path', 'temporary://devel-mails');

            if (!file_prepare_directory($this->ouputDirectory, FILE_CREATE_DIRECTORY)) {
                throw new RuntimeException(sprintf(
                    "Unable to continue sending mail, %s is not writable", $this->ouputDirectory));
            }
        }

        return $this->ouputDirectory;
    }
}

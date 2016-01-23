<?php
/**
 * @file
 * Definition of ManyMailMailer.
 */

class ManyMailMailer extends PHPMailer {

  /**
   * Assembles message header.
   *
   * @return string
   *   The assembled header.
   */
  public function CreateHeader() {
    $result = '';

    // Set the boundaries.
    $uniq_id = md5(uniqid(time()));
    $this->boundary[1] = 'b1_' . $uniq_id;
    $this->boundary[2] = 'b2_' . $uniq_id;
    $this->boundary[3] = 'b3_' . $uniq_id;

    $result .= $this->HeaderLine('Date', self::RFCDate());

    if (!variable_get('manymail_options_return_path_address')) {
      if ($this->Sender == '') {
        $result .= $this->HeaderLine('Return-Path', trim($this->From));
      }
      else {
        $result .= $this->HeaderLine('Return-Path', trim($this->Sender));
      }
    }

    // To be created automatically by mail()
    if ($this->Mailer != 'mail') {
      if ($this->SingleTo === TRUE) {
        foreach ($this->to as $t) {
          $this->SingleToArray[] = $this->AddrFormat($t);
        }
      }
      else {
        if (count($this->to) > 0) {
          $result .= $this->AddrAppend('To', $this->to);
        }
        elseif (count($this->cc) == 0) {
          $result .= $this->HeaderLine('To', 'undisclosed-recipients:;');
        }
      }
    }

    $from = array();
    $from[0][0] = trim($this->From);
    $from[0][1] = $this->FromName;
    $result .= $this->AddrAppend('From', $from);

    // Sendmail and mail() extract Cc from the header before sending.
    if (count($this->cc) > 0) {
      $result .= $this->AddrAppend('Cc', $this->cc);
    }

    // Sendmail and mail() extract Bcc from the header before sending.
    if ((($this->Mailer == 'sendmail') || ($this->Mailer == 'mail')) && (count($this->bcc) > 0)) {
      $result .= $this->AddrAppend('Bcc', $this->bcc);
    }

    if (count($this->ReplyTo) > 0) {
      $result .= $this->AddrAppend('Reply-to', $this->ReplyTo);
    }

    // The function mail() sets the subject itself.
    if ($this->Mailer != 'mail') {
      $result .= $this->HeaderLine('Subject', $this->EncodeHeader($this->SecureHeader($this->Subject)));
    }

    if ($this->MessageID != '') {
      $result .= $this->HeaderLine('Message-ID', $this->MessageID);
    }
    else {
      $result .= sprintf("Message-ID: <%s@%s>%s", $uniq_id, $this->ServerHostname(), $this->LE);
    }
    $result .= $this->HeaderLine('X-Priority', $this->Priority);
    if ($this->XMailer) {
      $result .= $this->HeaderLine('X-Mailer', $this->XMailer);
    }
    else {
      $result .= $this->HeaderLine('X-Mailer', 'PHPMailer ' . $this->Version . ' (http://code.google.com/a/apache-extras.org/p/phpmailer/)');
    }

    if ($this->ConfirmReadingTo != '') {
      $result .= $this->HeaderLine('Disposition-Notification-To', '<' . trim($this->ConfirmReadingTo) . '>');
    }

    // Add custom headers.
    for ($index = 0; $index < count($this->CustomHeader); $index++) {
      $result .= $this->HeaderLine(trim($this->CustomHeader[$index][0]), $this->EncodeHeader(trim($this->CustomHeader[$index][1])));
    }
    if (!$this->sign_key_file) {
      $result .= $this->HeaderLine('MIME-Version', '1.0');
      $result .= $this->GetMailMIME();
    }

    return $result;
  }

}

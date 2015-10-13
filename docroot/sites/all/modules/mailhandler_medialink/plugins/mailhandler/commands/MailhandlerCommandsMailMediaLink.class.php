<?php
/**
 * @file
 * MailhandlerCommandsMailMediaLink class.
 */

class MailhandlerCommandsMailMediaLink extends MailhandlerCommands {

  /**
   * Parse commands from email body
   *
   * @param $object
   *   Node object of the node being built for submission.
   */
  public function parse(&$message, $source) {
    // This might be useful?  But doesn't really seem to be.
    $config = $source->importer->getConfig();

    // Parse the body of the of the email for any links contained within.
    $links = array();
    // Available keys for this array are currently body_text and body_html.
    $parts = variable_get('mailhandler_medialink_mail_parts', array('body_text' => TRUE));
    foreach (array('body_text', 'body_html') as $part) {
      if (!empty($parts[$part])) {
        $links = array_merge($links, $this->parse_email_body_for_links($message[$part]));
      }
    }

    $commands['mailmedia'] = $links;
    $this->commands = $commands;
  }

  /**
   * Private helper function for parsing out links from the body of the email
   * Most of the code snagged from filter module which surprisingly doesn't
   * have a stand alone link parsing function.
   */
  private function parse_email_body_for_links($body) {

      // Tags to skip and not recurse into.
    $ignore_tags = 'a|script|style|code|pre';

    // Create an array which contains the regexps for each type of link.
    // The key to the regexp is the name of a function that is used as
    // callback function to process matches of the regexp. The callback function
    // is to return the replacement for the match. The array is used and
    // matching/replacement done below inside some loops.
    $tasks = array();

    // Prepare protocols pattern for absolute URLs.
    // check_url() will replace any bad protocols with HTTP, so we need to support
    // the identical list. While '//' is technically optional for MAILTO only,
    // we cannot cleanly differ between protocols here without hard-coding MAILTO,
    // so '//' is optional for all protocols.
    // @see filter_xss_bad_protocol()
    $protocols = variable_get('filter_allowed_protocols', array('http', 'https', 'ftp', 'news', 'nntp', 'telnet', 'mailto', 'irc', 'ssh', 'sftp', 'webcal', 'rtsp'));
    $protocols = implode(':(?://)?|', $protocols) . ':(?://)?';

    // Prepare domain name pattern.
    // The ICANN seems to be on track towards accepting more diverse top level
    // domains, so this pattern has been "future-proofed" to allow for TLDs
    // of length 2-64.
    $domain = '(?:[A-Za-z0-9._+-]+\.)?[A-Za-z]{2,64}\b';
    $ip = '(?:[0-9]{1,3}\.){3}[0-9]{1,3}';
    $auth = '[a-zA-Z0-9:%_+*~#?&=.,/;-]+@';
    $trail = '[a-zA-Z0-9:%_+*~#&\[\]=/;?!\.,-]*[a-zA-Z0-9:%_+*~#&\[\]=/;-]';

    // Prepare pattern for optional trailing punctuation.
    // Even these characters could have a valid meaning for the URL, such usage is
    // rare compared to using a URL at the end of or within a sentence, so these
    // trailing characters are optionally excluded.
    $punctuation = '[\.,?!]*?';

    // Match absolute URLs.
    $url_pattern = "(?:$auth)?(?:$domain|$ip)/?(?:$trail)?";
    $patterns[] = "`((?:$protocols)(?:$url_pattern))($punctuation)`";

    // Match www domains.
    $url_pattern = "www\.(?:$domain)/?(?:$trail)?";
    $patterns[] = "`($url_pattern)($punctuation)`";

    foreach ($patterns as $pattern) {
      // If we've got some links in the body.
      if (preg_match_all($pattern, $body, $matches)) {
        foreach ($matches[0] as $link) {
          // Remove them from the body text so we don't find the same link twice
          // as one of the patterns can double match.
          $body = str_replace($link, '', $body);
          $links[] = $link;
        }
      }
    }
    return $links;
  }

  /**
   * Helper function to use media_internet modules validation to see if
   * we are cool to use these links and generate media entries for them
   */
  private function media_links_validate($link) {

    try {
      $provider = media_internet_get_provider($link);
      $provider->validate();
    } catch (MediaInternetNoHandlerException $e) {
      drupal_set_message($e->getMessage(), 'warning');
      return FALSE;
    } catch (MediaInternetValidationException $e) {
      drupal_set_message($e->getMessage(), 'warning');
      return FALSE;
    }

    // Originally this would have been validated in a form
    // But in this case we don't have any of the stuff in the original form.
    // Media internet module added a #validators callback function in place (media_file_validate_types)
    // which is used to validate any 'types' of media (check if they are allowed).  We don't have a type list
    // though.  @TODO figure out how to get one -
    // for now still call the media_file_validate_types
    $validators = array('media_file_validate_types');

    try {
      $file = $provider->getFileObject();
      // Check for errors. See media_add_upload_validate calls file_save_upload().
      // this code is ripped from file_save_upload because we just want the validation part.
      // Call the validation functions specified by this function's caller.
      $errors = file_validate($file, $validators);
      if (!empty($errors)) {
        $message = t('%url could not be added.', array('%url' => $link));
        if (count($errors) > 1) {
          $message .= theme('item_list', array('items' => $errors));
        }
        else {
          $message .= ' ' . array_pop($errors);
        }
        drupal_set_message($message, 'warning');
        return FALSE;
      }
      return TRUE;
    } catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'warning');
      return FALSE;
    }
  }

  /**
   * Parse and process commands
   */
  function process(&$message, $source) {
    if (!empty($this->commands)) {
      foreach ($this->commands as $command => $data) {
        if ($command == 'mailmedia' && isset($data)) {
          foreach ($data as $link) {
            // Validate each link based on media module settings
            if ($this->media_links_validate($link)) {
              // If the link is fine - get the meta data and make it part of the source here.
              $message['mailmedia'][] = $link;
              // @TODO So I'm guessing what I could do here is call all the media handler things
              // to get the information about the embedded links if anything
              // make that part of the of the $message data -
              // then some feed processor can do the actual media element creation and insertion?
            }
            else{
              // @TODO Do somethine else- because the link is not valid.
              // send an email - queue one up
              // skip further processing probably not because other plugins can do their work still.
            }
          }
        }
      }
    }
  }

  function getMappingSources() {
    $sources = array();
    $sources['mailmedia'] = array(
      'name' => t('Link to a video'),
      'description' => t('A link to a video service site parsed out of the email body'),
    );
    return $sources;
  }
}

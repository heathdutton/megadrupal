<?php
/**
 * @file
 * Define the checker and address classes for the useremaildomain module.
 *
 * This checker class parses and analyzes the user email address.
 * The address class provides a structure for the address to be analyzed.
 */

/**
 * Class for processing an email address field and comparing it
 * to a list of approved domains.
 *
 * Fills attributes to be used by the calling functions.
 */

class useremaildomain_checker {
  // Initial raw values
  var $title = "Email";            // string | Title of the email field.
  var $value;                      // string | Original value of the field contents.

  // Config page values
  var $approved_domains_field;    // string | Contents of the field in this module's config page.
  var $approved_domains = array();// array | Strings of approved domains.

  // Processed results
  var $clean_value;                // string | Processed value of the field contents.
  var $domain;                    // string | Domain extracted from clean_value
  var $domain_approved = FALSE;    // boolean | Whether domain matches a domain from the approved list.

  // Feedback
  var $notice = array();          // array | Info strings for the Status messages.
  var $fault = array();            // array | Info strings for the Error messages.

  /*
   * Constructor for useremaildomain_checker
   *
   * Loads the basic parameters to perform this class task.
   * Populates all attributes.
   *
   * @param string $mail (Required)
   *   Contents of the mail field.
   * @param array $title (Optional)
   *   Title of the email field, defaulting to "Email".
   *
   * @return
   *   Reference to the useremaildomain_checker object.
   *---------------------------------------------------------------
   */
    public function __construct($value, $title = "Email") {
      $this->title = $title;
      $this->value = $value;
      $this->clean_value = $value; // initialize

          $this->approved_domains_field = variable_get('useremaildomain_domains');
      $this->parse_approved_domains();

      // Select the first approved address
      $address = $this->evaluate_addresses();

      $this->load_address($address);

    }


  /**
   * Process value as if it contains multiple email addresses,
   * then select the first viable address with an approved domain.
   * Populates array $notice;
   *
   * @return
   *   Final processed address object and it's array of resulting attributes.
   */
    public function evaluate_addresses() {
      $mail = str_replace(' ', ',', $this->value);

      $mailarray = explode(',', $mail);

      foreach ($mailarray as $key => $mailaddress) {
        // Return first occurance of a valid address
        $address = $this->evaluate_address($mailaddress);
        if ($address->domain_approved) {
          if (count($mailarray) > 1) {
            $this->notice['multiple'] = t("Unable to register more than one address.");
            if ($key > 0) {
              $this->notice['previous_invalid'] = t("Previous entries were invalid. Saving '@address'.", array('@address' => $address->clean_address));
            }
          }
          return ($address);
        }
      }

      // If no valid address returned above, return $result anyway;
      return ($address);
    }


  /**
   * Single iteration of evaluate_addresses
   *
   * Instantiates $address object and compares against approved domains.
   *
   * @param string $mailaddress (Required)
   *   Single email address for evaluation.
   *
   * @return
   *   Processed address object and attributes.
   */
    public function evaluate_address($mailaddress) {
      $address = new useremaildomain_checker_address($mailaddress, $this->title);

      if (!empty($address->domain)) {
        if (count($this->approved_domains)) {
          // Parse through approved domains to find a match.
          foreach ($this->approved_domains as $key => $domain) {
            if ($domain == $address->domain) {
              $address->domain_approved = TRUE;
              return ($address);
            }
          }


        }
        else {
          // No domains specified. Default to Approved.
          $address->domain_approved = TRUE;
        }
      }

      return ($address);
    }


  /**
   * Merge attributes from $address object into this object.
   *
   * Populates processed attributes and $fault array.
   *
   * @param object $address (Required)
   *   Chosen address object.
   */
    public function load_address($address) {
      $this->clean_value       = $address->clean_address;
      $this->domain            = $address->domain;
      $this->domain_approved   = $address->domain_approved;

      $this->fault = array_merge($this->fault, $address->fault);
    }


  /**
   * Parse the string value from the email field.
   *
   * Given string of domains, convert to array and
   * confirm each is a valid domain. Populates, filters and
   * reloads $approved_domains array.
   */
    public function parse_approved_domains() {
      $domainstr = str_replace(' ', ',', $this->approved_domains_field);
      $domainstr = _clear_whitespace_useremaildomain($domainstr);
      $domainarray = explode(',', $domainstr);

      $newdomainarray = array();

      // Verify each domain.
      foreach ($domainarray as $key => $domain) {
        $domainparts = explode('.', $domain);
        $tld = array_pop($domainparts);
        if (!count($domainparts)) {
          continue;
        }
        elseif (strlen($tld) < 2) {
          continue;
        }
        elseif (strlen($tld) > 4) {
          continue;
        }
        $nullpart = FALSE;
        $toomanyparts = FALSE;
        $maxparts = 16;
        foreach ($domainparts as $i => $part) {
          if (!strlen($part)) {
            $nullpart = TRUE;
            break;
          }
          if ($i >= $maxparts) {
            $toomanyparts = TRUE;
            break;
          }
        }
        if ($nullpart || $toomanyparts) continue;

        $newdomainarray[] = $domain;

      }

      $this->approved_domains = $newdomainarray;
    }

}


/**
 * Class to represent a single email address for evaluation.
 *
 * Approved object's values are incorporated into
 * useremaildomain_checker object.
 */

class useremaildomain_checker_address {
  // Initial raw values
  var $title;                    // string | Title of the email field.
  var $address;                  // string | The email address.

  // Processed results
  var $clean_address;            // string | Processed value of the address.
  var $domain;                  // string | Domain extracted from clean_address

  // Result of evaluation
  var $domain_approved = FALSE;  // boolean | Whether domain matches a domain from the approved list.

  // Feedback
  var $fault = array();          // array | Info strings for the Error messages.

  /*
   * Constructor for useremaildomain_checker_address
   *
   * Loads the basic parameters to perform this class task.
   * Populates all attributes.
   *
   * @param string $mailaddress (Required)
   *   Single unprocessed email address.
   * @param array $title (optional)
   *   Name of the field in the User form.
   *
   * @return
   *   Reference to useremaildomain_checker_address object.
   *---------------------------------------------------------------
    */
    public function __construct($mailaddress, $title = "Email") {
      $this->address = $mailaddress;
      $this->title = $title;
      $this->clean_address = _clear_whitespace_useremaildomain($mailaddress);
      $this->extract_mail_domain();
    }


  /**
   * Extract domain, check for viability.
   *
   * Extract the domain from this object's address, then check for viability.
   * Populates $fault array, $domain (only if viable)
   */
    public function extract_mail_domain() {
      if ($this->clean_address != $this->address) $this->fault['whitespace'] = t("Entry contains whitespace.");
      if (!strlen($this->clean_address)) {
        $this->fault['empty'] = t("Entry is empty.");
        return FALSE;
      }

      // Verify email address
      $mailparts = explode('@', $this->clean_address);
      if (count($mailparts) < 2) {
        $this->fault['incomplete_address'] = t("@title entry, '@address', is not a complete address.", array('@title' => $this->title, '@address' => $this->clean_address));
      }
      elseif (count($mailparts) > 2) {
        $this->fault['invalid_address'] = t("@title entry, '@address', is not a valid email address.", array('@title' => $this->title, '@address' => $this->clean_address));

      }
      else {
        // Verify domain from email address
        $domain = array_pop($mailparts);
        $domainparts = explode('.', $domain);
        if (count($domainparts) < 2) {
          $this->fault['incomplete_domain'] = t("@title entry domain, '@domain', is incomplete.", array('@title' => $this->title, '@domain' => $domain));
        }
        else {
          $this->domain = $domain;
        }
      }
    }

}

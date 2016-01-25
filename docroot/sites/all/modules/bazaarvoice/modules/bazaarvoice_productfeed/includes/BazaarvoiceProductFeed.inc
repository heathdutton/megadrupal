<?php
/**
 * @file
 * Contains wrapper code for a BazaarvoiceProductFeed object.
 */

/**
 * Class BazaarvoiceProductFeed.
 */
class BazaarvoiceProductFeed extends Bazaarvoice {

  private $ftp_host;
  private $ftp_mode;
  private $ftp_username;
  private $ftp_password;

  /**
   * Construct function.
   */
  public function __construct() {
    $bazaarvoice_mode = variable_get('bazaarvoice_mode', 'stg');
    $this->ftp_mode = variable_get('bazaarvoice_ftp_mode', 'ftp');
    $this->ftp_host = $this->ftp_mode . ($bazaarvoice_mode == 'stg' ? '-stg' : '') . '.bazaarvoice.com';
    $this->ftp_username = variable_get('bazaarvoice_ftp_username', NULL);
    $this->ftp_password = variable_get('bazaarvoice_ftp_password', NULL);
    $this->ftp_mode = variable_get('bazaarvoice_ftp_mode', 'ftp');
    parent::__construct();
  }

  /**
   * Attempt to FTP file to Bazaarvoice.
   *
   * @param string $file_path
   *   Path to feedfile to FTP.
   *
   * @return bool
   *   Boolean status if FTP was successful.
   */
  public function ftpProductFeed($file_path) {
    $ftp_path = "/import-inbox/" . basename($file_path);
    $ftp_uri = $this->ftp_mode . '://' . $this->ftp_username . ':' . $this->ftp_password . '@' . $this->ftp_host . $ftp_path;
    $file_path = 'file://' . $file_path;
    // @TODO: try/catch?
    if (file_put_contents($ftp_uri, file_get_contents($file_path))) {
      return TRUE;
    }
    return FALSE;
  }

}

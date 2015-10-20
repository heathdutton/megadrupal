<?php
/**
 * @file
 * Utility class for adding plus information for cURL function return types.
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Class for resolving cURL error and info codes.
 */
class CurlUtils {

  private $errorCodes = array();

  /**
   * Info codes is an associative array for resolving the meaning the keys of
   * the associative array returned by curl_getinfo() function.
   *
   * @var (array)
   */
  private $infoCodes = array();

  /**
   * Initializes the error codes
   */
  private function initErrorCodes() {
    if (!empty($this->errorCodes)) {
      return;
    }

    $this->errorCodes = array(
      0 => array(
        'code' => 'CURLE_OK',
        'text' => t('All fine. Proceed as usual.'),
      ),
      1 => array(
        'code' => 'CURLE_UNSUPPORTED_PROTOCOL',
        'text' => t('The URL you passed to libcurl used a protocol that this libcurl does not support. The support might be a compile-time option that you didn\'t use, it can be a misspelled protocol string or just a protocol libcurl has no code for.'),
      ),
      2 => array(
        'code' => 'CURLE_FAILED_INIT',
        'text' => t('Very early initialization code failed. This is likely to be an internal error or problem, or a resource problem where something fundamental couldn\'t get done at init time.'),
      ),
      3 => array(
        'code' => 'CURLE_URL_MALFORMAT',
        'text' => t('The URL was not properly formatted. '),
      ),
      4 => array(
        'code' => 'CURLE_URL_MALFORMAT_USER',
        'text' => t('A requested feature, protocol or option was not found built-in in this libcurl due to a build-time decision. This means that a feature or option was not enabled or explicitly disabled when libcurl was built and in order to get it to function you have to get a rebuilt libcurl.'),
      ),
      5 => array(
        'code' => 'CURLE_COULDNT_RESOLVE_PROXY',
        'text' => t('Couldn\'t resolve proxy. The given proxy host could not be resolved.'),
      ),
      6 => array(
        'code' => 'CURLE_COULDNT_RESOLVE_HOST',
        'text' => t('Couldn\'t resolve host. The given remote host was not resolved.'),
      ),
      7 => array(
        'code' => 'CURLE_COULDNT_CONNECT',
        'text' => t('Failed to connect() to host or proxy.'),
      ),
      8 => array(
        'code' => 'CURLE_FTP_WEIRD_SERVER_REPLY',
        'text' => t('After connecting to a FTP server, libcurl expects to get a certain reply back. This error code implies that it got a strange or bad reply. The given remote server is probably not an OK FTP server.'),
      ),
      9 => array(
        'code' => 'CURLE_REMOTE_ACCESS_DENIED',
        'text' => t('We were denied access to the resource given in the URL. For FTP, this occurs while trying to change to the remote directory.'),
      ),
      10 => array(
        'code' => 'CURLE_FTP_ACCEPT_FAILED',
        'text' => t('While waiting for the server to connect back when an active FTP session is used, an error code was sent over the control connection or similar.'),
      ),
      11 => array(
        'code' => 'CURLE_FTP_WEIRD_PASS_REPLY',
        'text' => t('After having sent the FTP password to the server, libcurl expects a proper reply. This error code indicates that an unexpected code was returned.'),
      ),
      12 => array(
        'code' => 'CURLE_FTP_ACCEPT_TIMEOUT',
        'text' => t('During an active FTP session while waiting for the server to connect, the CURLOPT_ACCEPTTIMOUT_MS (or the internal default) timeout expired.'),
      ),
      13 => array(
        'code' => 'CURLE_FTP_WEIRD_PASV_REPLY',
        'text' => t('libcurl failed to get a sensible result back from the server as a response to either a PASV or a EPSV command. The server is flawed.'),
      ),
      14 => array(
        'code' => 'CURLE_FTP_WEIRD_227_FORMAT',
        'text' => t('FTP servers return a 227-line as a response to a PASV command. If libcurl fails to parse that line, this return code is passed back.'),
      ),
      15 => array(
        'code' => 'CURLE_FTP_CANT_GET_HOST',
        'text' => t('An internal failure to lookup the host used for the new connection.'),
      ),
      17 => array(
        'code' => 'CURLE_FTP_COULDNT_SET_TYPE',
        'text' => t('Received an error when trying to set the transfer mode to binary or ASCII.'),
      ),
      18 => array(
        'code' => 'CURLE_PARTIAL_FILE',
        'text' => t('A file transfer was shorter or larger than expected. This happens when the server first reports an expected transfer size, and then delivers data that doesn\'t match the previously given size.'),
      ),
      19 => array(
        'code' => 'CURLE_FTP_COULDNT_RETR_FILE',
        'text' => t('This was either a weird reply to a \'RETR\' command or a zero byte transfer complete.'),
      ),
      21 => array(
        'code' => 'CURLE_QUOTE_ERROR',
        'text' => t('When sending custom "QUOTE" commands to the remote server, one of the commands returned an error code that was 400 or higher (for FTP) or otherwise indicated unsuccessful completion of the command.'),
      ),
      22 => array(
        'code' => 'CURLE_HTTP_RETURNED_ERROR',
        'text' => t('This is returned if CURLOPT_FAILONERROR is set TRUE and the HTTP server returns an error code that is >= 400.'),
      ),
      23 => array(
        'code' => 'CURLE_WRITE_ERROR',
        'text' => t('An error occurred when writing received data to a local file, or an error was returned to libcurl from a write callback.'),
      ),
      25 => array(
        'code' => 'CURLE_UPLOAD_FAILED',
        'text' => t('Failed starting the upload. For FTP, the server typically denied the STOR command. The error buffer usually contains the server\'s explanation for this.'),
      ),
      26 => array(
        'code' => 'CURLE_READ_ERROR',
        'text' => t('There was a problem reading a local file or an error returned by the read callback.'),
      ),
      27 => array(
        'code' => 'CURLE_OUT_OF_MEMORY',
        'text' => t('A memory allocation request failed. This is serious badness and things are severely screwed up if this ever occurs.'),
      ),
      28 => array(
        'code' => 'CURLE_OPERATION_TIMEDOUT',
        'text' => t('Operation timeout. The specified time-out period was reached according to the conditions.'),
      ),
      30 => array(
        'code' => 'CURLE_FTP_PORT_FAILED',
        'text' => t('The FTP PORT command returned error. This mostly happens when you haven\'t specified a good enough address for libcurl to use. See CURLOPT_FTPPORT.'),
      ),
      31 => array(
        'code' => 'CURLE_FTP_COULDNT_USE_REST',
        'text' => t('The FTP REST command returned error. This should never happen if the server is sane.'),
      ),
      33 => array(
        'code' => 'CURLE_RANGE_ERROR',
        'text' => t('The server does not support or accept range requests.'),
      ),
      34 => array(
        'code' => 'CURLE_HTTP_POST_ERROR',
        'text' => t('This is an odd error that mainly occurs due to internal confusion.'),
      ),
      35 => array(
        'code' => 'CURLE_SSL_CONNECT_ERROR',
        'text' => t('A problem occurred somewhere in the SSL/TLS handshake. You really want the error buffer and read the message there as it pinpoints the problem slightly more. Could be certificates (file formats, paths, permissions), passwords, and others.'),
      ),
      36 => array(
        'code' => 'CURLE_BAD_DOWNLOAD_RESUME',
        'text' => t('The download could not be resumed because the specified offset was out of the file boundary.'),
      ),
      37 => array(
        'code' => 'CURLE_FILE_COULDNT_READ_FILE',
        'text' => t('A file given with FILE:// couldn\'t be opened. Most likely because the file path doesn\'t identify an existing file. Did you check file permissions?'),
      ),
      38 => array(
        'code' => 'CURLE_LDAP_CANNOT_BIND',
        'text' => t('LDAP cannot bind. LDAP bind operation failed.'),
      ),
      39 => array(
        'code' => 'CURLE_LDAP_SEARCH_FAILED',
        'text' => t('LDAP search failed.'),
      ),
      41 => array(
        'code' => 'CURLE_FUNCTION_NOT_FOUND',
        'text' => t('Function not found. A required zlib function was not found.'),
      ),
      42 => array(
        'code' => 'CURLE_ABORTED_BY_CALLBACK',
        'text' => t('Aborted by callback. A callback returned "abort" to libcurl.'),
      ),
      43 => array(
        'code' => 'CURLE_BAD_FUNCTION_ARGUMENT',
        'text' => t('Internal error. A function was called with a bad parameter.'),
      ),
      45 => array(
        'code' => 'CURLE_INTERFACE_FAILED',
        'text' => t('Interface error. A specified outgoing interface could not be used. Set which interface to use for outgoing connections\' source IP address with CURLOPT_INTERFACE.'),
      ),
      47 => array(
        'code' => 'CURLE_TOO_MANY_REDIRECTS',
        'text' => t('Too many redirects. When following redirects, libcurl hit the maximum amount. Set your limit with CURLOPT_MAXREDIRS.'),
      ),
      48 => array(
        'code' => 'CURLE_UNKNOWN_TELNET_OPTION',
        'text' => t('An option passed to libcurl is not recognized/known. Refer to the appropriate documentation. This is most likely a problem in the program that uses libcurl. The error buffer might contain more specific information about which exact option it concerns.'),
      ),
      49 => array(
        'code' => 'CURLE_TELNET_OPTION_SYNTAX',
        'text' => t('A telnet option string was Illegally formatted.'),
      ),
      51 => array(
        'code' => 'CURLE_PEER_FAILED_VERIFICATION',
        'text' => t('The remote server\'s SSL certificate or SSH md5 fingerprint was deemed not OK.'),
      ),
      52 => array(
        'code' => 'CURLE_GOT_NOTHING',
        'text' => t('Nothing was returned from the server, and under the circumstances, getting nothing is considered an error.'),
      ),
      53 => array(
        'code' => 'CURLE_SSL_ENGINE_NOTFOUND',
        'text' => t('The specified crypto engine wasn\'t found.'),
      ),
      54 => array(
        'code' => 'CURLE_SSL_ENGINE_SETFAILED',
        'text' => t('Failed setting the selected SSL crypto engine as default!'),
      ),
      55 => array(
        'code' => 'CURLE_SEND_ERROR',
        'text' => t('Failed sending network data.'),
      ),
      56 => array(
        'code' => 'CURLE_RECV_ERROR',
        'text' => t('Failure with receiving network data.'),
      ),
      58 => array(
        'code' => 'CURLE_SSL_CERTPROBLEM',
        'text' => t('Problem with the local client certificate.'),
      ),
      59 => array(
        'code' => 'CURLE_SSL_CIPHER',
        'text' => t('Couldn\'t use specified cipher.'),
      ),
      60 => array(
        'code' => 'CURLE_SSL_CACERT',
        'text' => t('Peer certificate cannot be authenticated with known CA certificates.'),
      ),
      61 => array(
        'code' => 'CURLE_BAD_CONTENT_ENCODING',
        'text' => t('Unrecognized transfer encoding. '),
      ),
      62 => array(
        'code' => 'CURLE_LDAP_INVALID_URL',
        'text' => t('Invalid LDAP URL.'),
      ),
      63 => array(
        'code' => 'CURLE_FILESIZE_EXCEEDED',
        'text' => t('Maximum file size exceeded.'),
      ),
      64 => array(
        'code' => 'CURLE_USE_SSL_FAILED',
        'text' => t('Requested FTP SSL level failed.'),
      ),
      65 => array(
        'code' => 'CURLE_SEND_FAIL_REWIND',
        'text' => t('When doing a send operation curl had to rewind the data to retransmit, but the rewinding operation failed.'),
      ),
      66 => array(
        'code' => 'CURLE_SSL_ENGINE_INITFAILED',
        'text' => t('Initiating the SSL Engine failed.'),
      ),
      67 => array(
        'code' => 'CURLE_LOGIN_DENIED',
        'text' => t('The remote server denied curl to login.'),
      ),
      68 => array(
        'code' => 'CURLE_TFTP_NOTFOUND',
        'text' => t('File not found on TFTP server.'),
      ),
      69 => array(
        'code' => 'CURLE_TFTP_PERM',
        'text' => t('Permission problem on TFTP server.'),
      ),
      70 => array(
        'code' => 'CURLE_REMOTE_DISK_FULL',
        'text' => t('Out of disk space on the server.'),
      ),
      71 => array(
        'code' => 'CURLE_TFTP_ILLEGAL',
        'text' => t('Illegal TFTP operation.'),
      ),
      72 => array(
        'code' => 'CURLE_TFTP_UNKNOWNID',
        'text' => t('Unknown TFTP transfer ID.'),
      ),
      73 => array(
        'code' => 'CURLE_REMOTE_FILE_EXISTS',
        'text' => t('File already exists and will not be overwritten.'),
      ),
      74 => array(
        'code' => 'CURLE_TFTP_NOSUCHUSER',
        'text' => t('This error should never be returned by a properly functioning TFTP server.'),
      ),
      75 => array(
        'code' => 'CURLE_CONV_FAILED',
        'text' => t('Character conversion failed.'),
      ),
      76 => array(
        'code' => 'CURLE_CONV_REQD',
        'text' => t('Caller must register conversion callbacks.'),
      ),
      77 => array(
        'code' => 'CURLE_SSL_CACERT_BADFILE',
        'text' => t('Problem with reading the SSL CA cert (path? access rights?)'),
      ),
      78 => array(
        'code' => 'CURLE_REMOTE_FILE_NOT_FOUND',
        'text' => t('The resource referenced in the URL does not exist.'),
      ),
      79 => array(
        'code' => 'CURLE_SSH',
        'text' => t('An unspecified error occurred during the SSH session.'),
      ),
      80 => array(
        'code' => 'CURLE_SSL_SHUTDOWN_FAILED',
        'text' => t('Failed to shut down the SSL connection.'),
      ),
      81 => array(
        'code' => 'CURLE_AGAIN',
        'text' => t('Socket is not ready for send/recv wait till it\'s ready and try again.'),
      ),
      82 => array(
        'code' => 'CURLE_SSL_CRL_BADFILE',
        'text' => t('Failed to load CRL file.'),
      ),
      83 => array(
        'code' => 'CURLE_SSL_ISSUER_ERROR',
        'text' => t('Issuer check failed.'),
      ),
      84 => array(
        'code' => 'CURLE_FTP_PRET_FAILED',
        'text' => t('The FTP server does not understand the PRET command at all or does not support the given argument. Be careful when using CURLOPT_CUSTOMREQUEST, a custom LIST command will be sent with PRET CMD before PASV as well.'),
      ),
      85 => array(
        'code' => 'CURLE_RTSP_CSEQ_ERROR',
        'text' => t('Mismatch of RTSP CSeq numbers.'),
      ),
      86 => array(
        'code' => 'CURLE_RTSP_SESSION_ERROR',
        'text' => t('Mismatch of RTSP Session Identifiers. '),
      ),
      87 => array(
        'code' => 'CURLE_FTP_BAD_FILE_LIST',
        'text' => t('Unable to parse FTP file list (during FTP wildcard downloading).'),
      ),
      88 => array(
        'code' => 'CURLE_CHUNK_FAILED',
        'text' => t('Chunk callback reported error.'),
      ),
    );
  }

  /**
   * Initializes the info codes
   */
  function initInfoCodes() {
    if (!$this->infoCodes) {
      return;
    }

    $this->infoCodes = array(
      'url' => array(
        'code' => 'CURLINFO_EFFECTIVE_URL',
        'text' => t('Last effective URL'),
      ),
      'http_code' => array(
        'code' => 'CURLINFO_HTTP_CODE',
        'text' => t('Last received HTTP code'),
      ),
      'filetime' => array(
        'code' => 'CURLINFO_FILETIME',
        'text' => t('Remote time of the retrieved document, if -1 is returned the time of the document is unknown'),
      ),
      'total_time' => array(
        'code' => 'CURLINFO_TOTAL_TIME',
        'text' => t('Total transaction time in seconds for last transfer'),
      ),
      'namelookup_time' => array(
        'code' => 'CURLINFO_NAMELOOKUP_TIME',
        'text' => t('Time in seconds until name resolving was complete'),
      ),
      'connect_time' => array(
        'code' => 'CURLINFO_CONNECT_TIME',
        'text' => t('Time in seconds it took to establish the connection'),
      ),
      'pretransfer_time' => array(
        'code' => 'CURLINFO_PRETRANSFER_TIME',
        'text' => t('Time in seconds from start until just before file transfer begins'),
      ),
      'starttransfer_time' => array(
        'code' => 'CURLINFO_STARTTRANSFER_TIME',
        'text' => t('Time in seconds until the first byte is about to be transferred'),
      ),
      'redirect_time' => array(
        'code' => 'CURLINFO_REDIRECT_TIME',
        'text' => t('Time in seconds of all redirection steps before final transaction was started'),
      ),
      'size_upload' => array(
        'code' => 'CURLINFO_SIZE_UPLOAD',
        'text' => t('Total number of bytes uploaded'),
      ),
      'size_download' => array(
        'code' => 'CURLINFO_SIZE_DOWNLOAD',
        'text' => t('Total number of bytes downloaded'),
      ),
      'speed_download' => array(
        'code' => 'CURLINFO_SPEED_DOWNLOAD',
        'text' => t('Average download speed'),
      ),
      'speed_upload' => array(
        'code' => 'CURLINFO_SPEED_UPLOAD',
        'text' => t('Average upload speed'),
      ),
      'header_size' => array(
        'code' => 'CURLINFO_HEADER_SIZE',
        'text' => t('Total size of all headers received'),
      ),
      'request_size' => array(
        'code' => 'CURLINFO_REQUEST_SIZE',
        'text' => t('Total size of issued requests, currently only for HTTP requests'),
      ),
      'ssl_verify_result' => array(
        'code' => 'CURLINFO_SSL_VERIFYRESULT',
        'text' => t('Result of SSL certification verification requested by setting CURLOPT_SSL_VERIFYPEER'),
      ),
      'download_content_length' => array(
        'code' => 'CURLINFO_CONTENT_LENGTH_DOWNLOAD',
        'text' => t('content-length of download, read from Content-Length: field'),
      ),
      'upload_content_length' => array(
        'code' => 'CURLINFO_CONTENT_LENGTH_UPLOAD',
        'text' => t('Specified size of upload'),
      ),
      'content_type' => array(
        'code' => 'CURLINFO_CONTENT_TYPE',
        'text' => t('Content-Type of the requested document'),
      ),
      'redirect_count' => array(
        'code' => 'CURLINFO_REDIRECT_COUNT',
        'text' => t('The number of redirects it went through if CURLOPT_FOLLOWLOCATION was set.'),
      ),
    );
  }

  /**
   * Resolve the error code.
   *
   * @param $errorCode (int)
   *   The cURL error code such as 22.
   *
   * @return (array)
   *   The explanation of the error. It is an associative array with two keys:
   *   - 'code' (string): the string form of the cURL constant, such as 'CURLE_CHUNK_FAILED'.
   *   - 'text' (string): the human readable explanation of the code
   */
  public function resolveErrorCode($errorCode) {
    $this->initErrorCodes();

    if (isset($this->errorCodes[(int) $errorCode])) {
      return $this->errorCodes[(int) $errorCode];
    }

    return FALSE;
  }

  /**
   * Resolve the info code returned by curl_getinfo() call.
   *
   * @param $infoCode (string)
   *   The cURL info code such as content_type.
   *
   * @return (array)
   *   The explanation of the code. It is an associative array with two keys:
   *   - 'code' (string): the string form of the cURL constant, such as 'CURLINFO_CONTENT_LENGTH_DOWNLOAD'.
   *   - 'text' (string): the human readable explanation of the code
   */
  public function resolveInfoCode($infoCode) {
    $this->initInfoCodes();

    if (isset($this->infoCodes[$infoCode])) {
      return $this->infoCodes[$infoCode];
    }

    return FALSE;
  }
}

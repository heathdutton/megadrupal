<?php

class VerifyModule {
    const TRUSTED_MODULES_MODULE_CERT_FILE = '.module.cert.pem';
    const TRUSTED_MODULES_MODULE_SIG_FILE = '.module.cert.sig';
    const TRUSTED_MODULES_MODULE_CHECKSUM_FILE = '.checksum';
    const TRUSTED_MODULES_MODULE_CHECKSUM_SIG_FILE = '.checksum.sig';

    private $module_cert_pem_path;
    private $module_cert_sig_path;
    private $checksum_path;
    private $checksum_sig_path;
    private $drupal_cert;
    private $drupal_public_key;
    private $module_cert;
    private $module_cert_sig;

    public function __construct($module_path) {
        $this->module_path = $module_path;
        $this->module_cert_pem_path = $module_path . '/' . self::TRUSTED_MODULES_MODULE_CERT_FILE;
        $this->module_cert_sig_path = $module_path . '/' . self::TRUSTED_MODULES_MODULE_SIG_FILE;
        $this->checksum_path = $module_path . '/' . self::TRUSTED_MODULES_MODULE_CHECKSUM_FILE;
        $this->checksum_sig_path = $module_path . '/' . self::TRUSTED_MODULES_MODULE_CHECKSUM_SIG_FILE;

        // Load Drupal.org certificate
        // TODO: Remove hard coding
        $this->drupal_cert = variable_get('trusted_modules_certificate_drupal_org', '');
        if (empty($this->drupal_cert)) {
            throw new Exception("Drupal.org certificate not found. It can be downloaded at /admin/reports/trusted/certificates");
        }

        $this->drupal_cert = openssl_x509_parse($this->drupal_cert);
        $this->drupal_public_key = $this->load_file('file:///home/yonas/fake.drupal.org.cert.pem',  'openssl_get_publickey', 'Drupal.org public key');

        // Load module certificate
        $this->module_cert = $this->load_file(array('file' => $this->module_cert_pem_path, 'load_content' => true), 'openssl_x509_parse', 'Module certificate');

        // Load module certificate signature
        $this->module_cert_sig = $this->load_file($this->module_cert_sig_path, 'file_get_contents', 'Module certificate signature');
    }

    public function verify() {
        $this->verify_module_cert();
        $this->verify_module_checksum();
        return true;
    }

    public function verify_module_cert() {
        // TODO: Verify module certificate is created by Drupal.org
        /*
        if (false) {
            throw new Exception('Module certificate was not created by Drupal.org.');
        }
        */

        // Verify module certificate is signed by Drupal.org
        module_load_include('inc', 'trusted_modules', 'trusted_modules.sign');
        if (verify_signature($this->drupal_public_key, $this->module_cert_sig, $this->module_cert_pem_path)) {
            throw new Exception('Module certificate was not signed by Drupal.org.');
        }

        // Verify module certificate belongs to this module
        $module_domain = str_replace('_', '.', basename($this->module_path));
        if ($this->module_cert['subject']['CN'] != $module_domain . '.drupal.org') {
            throw new Exception('Module certificate does not belong to this module.');
        }
    }

    public function verify_module_checksum() {
        // Load directory checksum
        $checksum = $this->load_file($this->checksum_path, 'file_get_contents', 'Module checksum');

        // Load directory checksum signature
        $checksum_sig = $this->load_file($this->checksum_sig_path, 'file_get_contents', 'Module checksum signature');

        // Load module public key
        $module_public_key = $this->load_file('file://' . $this->module_cert_pem_path,  'openssl_get_publickey', 'Module public key');

        // Verify directory checksum signature is valid
        if (verify_signature($module_public_key, $checksum_sig, $this->checksum_path)) {
            throw new Exception('Module checksum was signed by signed by project maintainer.');
        }

        // Calculate directory checksum
        $dir = new \fizk\DirectoryHash\Standard($this->module_path);
        $ignore_files = array(basename($this->checksum_path), basename($this->checksum_sig_path), basename($this->module_cert_pem_path), basename($this->module_cert_sig_path));
        $current_checksum = 'sha256: ' . $dir->hash(array('ignore_files' => $ignore_files));

        // Verify computed directory checksum matches signed directory checksum
        if (trim($current_checksum) != trim($checksum)) {
            throw new Exception('Module checksum is invalid. The contents of this directory have been altered.' . "\nComputed checksum: " . $current_checksum . "\nSigned checksum: " . $checksum);
        }
    }

    protected function load_file($file, $loader = 'file_get_contents', $desc = '', $empty_ok = false) {
        if (!is_array($file)) {
            $file = array('file' => $file);
        }

        if (empty($desc)) {
            $desc = $file['file'];
        }

        if (!file_exists($file['file'])) {
            throw new Exception($desc . ' not found.');
        }

        if (!empty($file['load_content']) && $file['load_content']) {
            $content = $loader(file_get_contents($file['file']));
        } else {
            $content = $loader($file['file']);
        }

        if (!$empty_ok && empty($content)) {
            throw new Exception($desc . ' file is empty.');
        }

        return $content;
    }
}

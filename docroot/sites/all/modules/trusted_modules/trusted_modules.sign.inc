<?php

function generate_signature($private_key, $file) {
    $data = trim(file_get_contents($file));
    $sig = "";
    $ok = openssl_sign($data, $sig, $private_key, OPENSSL_ALGO_SHA256);
    openssl_free_key($private_key);

    if ($ok == 1) {
        return "----BEGIN SIGNATURE----\n" . base64_encode($sig) . "\n----END SIGNATURE----\n";
    }
    elseif ($ok == 0) {
        throw new Exception('Could not sign checksum (bad).');
    }
    else {
        throw new Exception('Could not sign checksum (ugly).');
    }
}

function get_raw_sig($sig) {
    list($header, $base64, $footer) = explode("\n", $sig);
    $raw_sig = base64_decode(trim($base64));
    return $raw_sig;
}

function verify_signature($public_key, $sig, $file) {
    $raw_sig = get_raw_sig($sig);
    $ok = openssl_verify(trim(file_get_contents($file)), $raw_sig, $public_key, 'sha256WithRSAEncryption');
    if (!$ok) {
        throw new Exception('Signature verification failed.');
    }
}

function sign($file, $private_key_file, $public_key_file) {
    if (!file_exists($file) || !strlen(file_get_contents($file))) {
        throw new Exception("File \"{$file}\" must exist and contain your checksum.");
    }

    $private_key = openssl_get_privatekey($private_key_file);
    $public_key = openssl_get_publickey($public_key_file);
    $sig = generate_signature($private_key, $file);
    verify_signature($public_key, $sig, $file);

    return $sig;
}

function sign_as_drupalorg($file = '.module.cert.pem', $private_key_file = 'file:///home/yonas/fake.drupal.org.key.pem', $public_key_file = 'file:///home/yonas/fake.drupal.org.cert.pem') {
    return sign($file, $private_key_file, $public_key_file);
}

function sign_as_module_maintainer($file = '.checksum', $private_key_file = 'file:///home/yonas/filesize.filter.drupal.org.key.pem', $public_key_file = 'file:///home/yonas/filesize.filter.drupal.org.cert.pem') {
    return sign($file, $private_key_file, $public_key_file);
}

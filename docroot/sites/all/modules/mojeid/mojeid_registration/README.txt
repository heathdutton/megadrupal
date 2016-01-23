
                        +++ mojeID Registrations +++

                              A Drupal module

This module allows you to use the mojeID Incentive Programme for web service
providers, who are offered financial bonus for each new mojeID account
registered through their web page. The aim of this programme is to increase the
total number of mojeID users and also raise the number of web services accepting
mojeID login.

This module redirects the user to the mojeid.cz provider in a secure way and
receive the responses from the mojeID provider. However it doesn't do anything
on its own with the response. If you want to e.g. pre-create user profile or log
all the successful registrations you can use the module API. Look into the file
mojeid_registration.api.php for more info and examples how to use the API.

You can read more about that programme here: http://www.mojeid.cz/page/877

________________________________________________________________________________

INSTALLATION
________________________________________________________________________________

1. Visit http://www.mojeid.cz/page/877 and read the instructions.

2. Apply for contract with mojeID.cz

3. Make sure can use SSL/HTTPS on the server, otherwise this module won't work
   for you.

4. Configure the module at admin/config/people/mojeid/registration

5. Test if you're site respond to SSL request with curl. (See Requirements
   section).

6. Ready for action!

________________________________________________________________________________

REQUIREMENTS
________________________________________________________________________________

You need SSL enabled server (e.g. mod_ssl when using Apache as the webserver),
and you need a valid a certicate signed by a certified authority, self-signed
certificates won't work. You can check it with CURL.

This is how the response with good certificate look like:

$ curl https://example.com
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
... etc.

This is how the response with bad certificate look like:

$ curl https://example.com
curl: (60) Peer's Certificate issuer is not recognized.
More details here: http://curl.haxx.se/docs/sslcerts.html

If you have mod_ssl enabled, there are two most common reasons for that error:

1. Certificate authority that issued the cerificate isn't part of CA bundle,
   which are used by CURL and it is the same, which is included in Firefox
   browser by default. See http://curl.haxx.se/.

2. Intermediate certificate is missing or it is misconfigured. Browsers often
   ignores that kind of mistake, but CURL isn't tolerant like that. Check if you
   have "SSLCertificateChainFile" in your Apache SSL configuration.

________________________________________________________________________________

ADDITIONAL SECURITY LAYER
________________________________________________________________________________

For extra security, you can protect the path "/mojeid/registration", which is
used as endpoint for receiving messages from the mojeID provider.

You can protect it either by one or by both of the following techniques:

1. Set allowed IP addresses to 217.31.192.0/20 and 2001:1488::/32

<Location /mojeid/registration>
  Order Deny,Allow
  Deny from all
  Allow from 217.31.192.0/20 2001:1488::/32
</Location>

2. Set certificate verification using "SSLVerifyClient require" directive.
   See http://httpd.apache.org/docs/2.2/ssl/ssl_howto.html#accesscontrol for
   more information. You can download the certificates from here:
   http://www.mojeid.cz/files/mojeid/new-CZ.NIC-cacert.pem

<Location /mojeid/registration>
  SSLCACertificateFile /path/to/CZ.NIC-cacert.pem
  SSLVerifyClient require
  SSLVerifyDepth 1
</Location>

LAuth is a protocol to authenticate HTTP requests.

There are three actors in LAuth. A CLIENT needs to have access to a USER's resources on a SERVER.

The goal of LAuth is to enable servers to verify the identity of a client
acting on behalf of a user without the user having to give to the client the secret
he shares with the server.

What does this mean?

For example, let's say Alice have uploaded photos on photos.example.com, using a login name
and a password that give her access to the whole range of services offered by photos.example.com.

Alice also have a Drupal site, drupal.example.com, on which she installed the ExampleAlbum module.
Using ExampleAlbum, Alice can display the photos she uploaded on her account on
photos.example.com. ExampleAlbum thus needs access to Alice's account on photos.example.com.
However, for security reasons, Alice does not want to give her photos.example.com credentials
to ExampleAlbum module.

Fortunately, ExampleAlbum and photos.example.com both support LAuth.

So Alice logs on photos.example.com and generates a new LAuth key. She then copies the key
(a key identifier and a secret) on the configuration page of the ExampleAlbum module. The
ExampleAlbum module will use this key to make signed requests to photos.example.com on behalf
of Alice.

If Alice discovers that the key have been compromised, or if she wants to stop ExampleAlbum
to access her photos.example.com account, she can simply revoke the key.


USAGE AND INSTALLATION
======================

LAuth is made of three modules, lauth_api, lauth_server and lauth_service, to better cover the
three basic use cases for this set of modules:

- you are writing a client module that needs to access resources on a server that uses LAuth
  as an authentication mechanism. In this case, you only need to enable lauth_api and
  call lauth_api_http_request() to make authenticated request to the server. This function is
  very similar to drupal_http_request().

- you wish to provides Web services, using the Services module, and you wish to use LAuth as
  an authentication mechanism. In this case, enable lauth_api, lauth_server and lauth_service.
  Create Drupal roles to control access to your resources. User will be able to create LAuth
  keys by going on the /user page and associate keys with the roles you created. Using Drupal
  permission system, you can choose which roles are assignable

- you wish to provides Web services, without using the Services module. In this case, enable
  lauth_api and lauth_server, and verify the authenticity of the HTTP requests by calling
  lauth_server_verify_request(). User will be able to create LAuth keys by going on the /user
  page and clicking on the LAuth tab.


TECHNICAL DETAILS
=================

LAuth computes a SHA256 HMAC of (some parts of) the HTTP request and uses this as the
request signature.

A client that need to access a resource protected by LAuth will build a HTTP request,
compute its SHA256 HMAC and add the following information to the Authorization header:
  - lauth_key_id: the user public key
  - lauth_timestamp: the time at which the request is made
  - lauth_nonce: a unique request number
  - lauth_protocol_version
  - lauth_signature: the SHA256 HMAC of the request

When the server receives the request, it computes its SHA256 HMAC using the secret associated
with lauth_key_id and verifies the following:
  - is lauth_timestamp recent enough?
  - is this the first time, recently, that the server receives a request with this lauth_nonce?
  - does the computed SHA256 HMAC matches lauth_signature?

If the answers to all three questions are yes, the request is authenticated.


CREDITS
=======

Code and ideas were shamelessly borrowed from the OAuth specification, the OAuth PHP library
(http://code.google.com/p/oauth/) and Drupal OAuth module (http://www.drupal.org/project/oauth).

The development of this module was sponsored by Laboratoire NT2 (nt2.uqam.ca) and
Whisky Echo Bravo (www.whiskyechobravo.com).

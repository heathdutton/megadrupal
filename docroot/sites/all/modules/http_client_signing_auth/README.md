HTTP Client Signing Authentication
----------------------------------

Simple request signing authentication based on OAuth but greatly simplified. Useful when you need simple authentication of POST request between the server and client.

Depends on [HTTP Client](http://drupal.org/project/http_client) for doing the actual requests.

Example server implementation: https://github.com/hugowetterberg/tattlebird-server
Node library for server implementation: https://github.com/hugowetterberg/signing_auth

Client implementation example:
https://github.com/hugowetterberg/tattlebird/commit/eab0519dbc7595540c00e09621f7b76bc124978e#L6R49
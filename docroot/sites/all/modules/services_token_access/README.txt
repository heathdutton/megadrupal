This module allows site users to authenticate towards a web service, using an
access token.

An individual token can be generated for each user and token access can be
controlled on a per role basis.

An administrator can revoke all tokens at once, and with the "administer users"
permission, it is possible to assign tokens to other users.

Usage:
Go to Permissions and enable the Services Token Access permissions for the
appropriate roles. Then go to the user page and generate a token for the given
user. When accessing the webservice, add services_token=<YOUR TOKEN> as an URL
parameter, like http://example.com/service/resource?services_token=678874321232

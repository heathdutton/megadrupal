Feeds OAuth provides a new Feeds fetcher plugin that performs OAuth or OAuth 2.0 authorization before requesting a feed. 

# Usage

* Obtain standard OAuth information from host: consumer key, consumer secret, request token URL, access token URL, authorize URL.
* Create a new Feeds importer and select the fetcher to be of type OAuthHTTPFetcher or OAuth2HTTPSFetcher.
* In the fetcher settings, enter the OAuth information above, as well as a unique site identifier that represents the host (e.g., twitter).
* When saved, the configuration screen will show you a callback URL: add it to the host's configuration for your Drupal site.
* Associate the new importer with a content type.
* When you create a new node of this content type, the form will warn you that you haven't received an authorization from the host. Click the link to do so and complete the authentication steps.
* OAuth 2.0: you can also enter the scope argument that's sent during authorization, to request special permissions from the server.
You're now ready to create new feeds from your protected data! 

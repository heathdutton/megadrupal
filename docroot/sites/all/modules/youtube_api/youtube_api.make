; Make file for Youtube API module.
; Fetch the Google Youtube library from Github.

api = 2
core = 7.x

libraries[google-api-php-client][download][type] = git
libraries[google-api-php-client][download][url] = git@github.com:google/google-api-php-client.git
libraries[google-api-php-client][download][tag] = 1.1.4
libraries[google-api-php-client][destination] = libraries

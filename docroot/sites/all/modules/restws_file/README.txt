# RESTful web services support for files and images (restws_file)

This module provides support for creating and updating fields of type file and 
image. It accepts (multiple) image/file data as base64_encoded strings in the 
JSON payload.

## Installation

Requires RESTful web services (restws).
No additional steps needed, just enable the module in Drupal.

## Usage example

1. Enable and configure restws (and optionally restws_basic_auth), making sure 
you can create/update a regular 'article' node.
2. Configure the 'article' stock content type of your Drupal installation, to 
have an image field called "field_image".
3. Using any REST client extension, POST a JSON body payload like the one in 
examples/image-payload.json.
4. Check the newly created article: it has an image!.
5. You can do the same with files, try for yourself (hint: use 
https://www.base64decode.org/).
6. You can issue a PUT request to modify the article (do NOT set a type, and 
use the id returned by the POST).

## Credits
Mariano E. Barcia Calderón (mbarcia@asmws.com)
Originally started with a patch contributed at 
https://www.drupal.org/node/2320083.

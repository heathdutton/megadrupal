BLIZZARD API
------------
Provides an interface for interacting with Battle.net APIs and allows users to
login using their Battle.net accounts.

Module developers may use this module to quickly access game data from World
of Warcraft, Diablo 3, and StarCraft 2. For users that login using a Battle.net
account, they may also access protected user information, such as StarCraft 2
profiles and World of Warcraft characters. Source code is fully documented and
additional code examples are provided in the included *.api.php files.

REQUIREMENTS
------------
Drupal 7

The Blizzard API module requires:
- PHP extensions: OpenSSL

The Blizzard API Login module requires:
- PHP 5.3.2 (or later)
- A secure, HTTPS enabled, web server.
- Libraries API (https://www.drupal.org/project/libraries)

INSTALLATION (BLIZZARD API)
---------------------------
1. DOWNLOAD THE BLIZZARD API MODULE

  Extract the downloaded files into the sites/*/modules directory of your
  Drupal installation. Then enable the module at the Administration > Modules
  page.

2. CREATE A MASHERY ACCOUNT

  In order to access Battle.net APIs, you must create a Mashery account.

    https://dev.battle.net/member/register

  You do not need to provide a callback URL during registration unless you are
  using a Battle.net login module.

3. SET CONFIGURATION OPTIONS

  At admin/config/services/blizzardapi you can set the API authentication keys
  given to you by Blizzard.

4. DOWNLOAD AND INSTALL MODULES THAT USE THIS API

  This module is an API for other module developers to access Battle.net game
  information. Install those modules to start getting data.

INSTALLATION (BLIZZARD API LOGIN)
---------------------------------
1. DOWNLOAD THE LIBRARIES API MODULE

  Extract the downloaded files into the sites/*/modules directory of your
  Drupal installation. Then enable the module at the Administration > Modules
  page.

2. CHOOSE AN OAUTH 2.0 IMPLEMENTATION

  The Blizzard API Login module includes interfaces for two possible clients.
  If your site uses any third-party modules that access protected APIs, consult
  its documentation for which clients are supported.

  IMPORTANT: If your site is using PHP 5.5 or earlier, secure connections are
  not created by default. Therefore you must provide a Certificate Authority
  bundle if you are using the Blizzard API Default Client (see installation
  instructions below). If you are using the Battle.net for Google API Clients
  library, this file is provided for you. In both cases, you need to make sure
  that the file remains up-to-date.

  A. USE THE BLIZZARD API DEFAULT CLIENT

    This is the default client that is included with this module. It provides a
    basic OAuth 2.0 implementation that can only be used to login users and
    request protected account information.

    You can configure this client at admin/config/services/blizzardapi/oauth.

    If you need to provide a Certificate Authority bundle, the settings page
    has download instructions.

  B. USE THE BATTLE.NET FOR GOOGLE API CLIENTS LIBRARY

    This library is an extension for the Google APIs Client library that allows
    access to Battle.net services. The Google APIs Client library, provides a
    default OAuth 2.0 implementation and a default CA bundle (it can also be
    used to add Google logins to your site).

    I. INSTALLATION

    Download and extract the following libraries into the sites/*/libraries
    directory of your Drupal installation.

      https://github.com/google/google-api-php-client/releases
      https://github.com/mattacosta/battlenet-google-client/releases

    NOTE: Library names must not contain version numbers, if present, and are
    case-sensitive. The directory containing the library must be entirely lower
    case (such as 'battlenet-google-client').

    If you require help with installing libraries see:

      https://www.drupal.org/node/1440066

    If your site is using PHP 5.5 or earlier, enabling the cURL extension is
    also recommended.

    II. CONFIGURATION

    At admin/config/services/blizzardapi/oauth, select this library from the
    list of authentication clients.

  C. USE A CLIENT PROVIDED BY ANOTHER MODULE

    If you do not wish to use either client, this module also allows other
    modules to provide compatible OAuth 2.0 implementations. Follow
    installation instructions provided by those modules.

3. REGISTER THE SITE CALLBACK URL

  The site callback URL must now be registered with Blizzard Entertainment
  through your Mashery account. For your convenience, the URL is displayed on
  the Blizzard API settings page.

4. CONFIGURE USER LOGIN AND REGISTRATION OPTIONS

  This module also includes custom user registration options, and alters some
  e-mails sent by the system. These can be be changed from the Blizzard API
  settings page, if you have the 'administer users' permission.

5. ENABLE THE BATTLE.NET LOGIN BLOCK (OPTIONAL)

  If you would like to use the Battle.net login block, it can be managed from
  the Administration > Structure > Blocks page.

AUTHOR
------
MattA <http://drupal.org/user/343467/>

This module is not affiliated with Blizzard Entertainment.

Battle.net and Blizzard Entertainment are trademarks or registered trademarks of
Blizzard Entertainment, Inc. in the U.S. and/or other countries. All rights
reserved.

All other trademarks are the property of their respective owners.

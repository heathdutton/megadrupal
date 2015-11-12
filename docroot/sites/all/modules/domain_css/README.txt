This module is an extension to the Domain Access module and allows drupal to
use one additional stylesheet per domain. This is useful when you want to
have differences in terms of design between domains.

Example:
forest.example.com includes green.css
cars.example.com includes grey.css

Usage:
1. Install the module (make sure Domain Access is installed)
2. Browse to admin/structure/domain
3. Click on "edit domain" for a domain that you want to add custom css for
4. Once in the domain detailed view, you should see a new tab called
"Domain CSS". Click on that.
5. Enter a css file name located in your current theme's directory or
subdirectory. Valid values are "custom.css" or "css/custom.css".

Note: do NOT use full paths like /sites/all/themes/...

6. Save & enjoy.

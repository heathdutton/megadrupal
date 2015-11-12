= INSTALLATION =

Extract the BOM Weather Lite module to one of the designated module
directories on your server. Then visit the modules page and enable the module.


= DEPENDENCIES =

BOM Weather Lite uses the simplehtmldom module to fetch and parse weather
forecast information. Make sure that https://drupal.org/project/simplehtmldom
is installed before you enable BOM Weather Lite.


= CONFIGURATION =

Visit the blocks administration page and configure the BOM Weather Lite #0.
You will need to enter at least a the Forecast Page URL. You can find the URL
for your location by visiting the BOM site (http://www.bom.gov.au).

Browse to the forecast information for your area and paste the URL from your
web browser's location bar into the Forecast Page URL field.

The URL will be in the form of:
  http://www.bom.gov.au/[state]/forecasts/[location].shtml

If you leave the Block title blank, it will be generated from the Forecast page
information.

Once a block is configured, a new unconfigured block will become available in
the blocks list. This means you can add as many weather forecast blocks as
you like.

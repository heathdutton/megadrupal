# Ricochet Maintenance Helper module

Install this module to be able to communicate the site's update status
with the Ricochet Maintenance server (RMS) at {SERVER_URL tbd}.

### To install:

- Enable the Ricochet Maintenance Helper module on you site
- Set up a site on the RMS server
- Fill in your API key in the settings form at admin/config/ricochet_maintenance_helper, or in your settings.php file.

### Put the API key in settings.php:
If you are using development and testing environments in addition to your production site, 
you should make sure to place the API key inside your site's settings.php file. To do that,
insert your API key in settings.php using the following line:
 
        $conf['ricochet_maintenance_helper_environment_token'] = 'your-api-key'
    
Replace your-api-key with your actual API key.
Once there, the module will automatically start using this API key.

### Why is the environment setting important?
The server uses the API key to communicate updates to the Ricochet Maintenance Server. 
If that key is duplicated on more than one environment, it will report updates from those environments as well.
This may lead to an update being marked as done when they really aren't, if your other domains have a different
set of module versions.

### To use:
- Updates will be sent to the RMP server at cron run.
- You will be able to see a summary about your available updates at your account at https://maintenance.projectricochet.com/
- You will be able to receive useful email reports about your site's update status.
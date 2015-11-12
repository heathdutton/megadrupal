$Id$

Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
Copyright © 2009-2012 New Signature

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
You can contact New Signature by electronic mail at labs@newsignature.com -or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 
Most Popular Module
------------------------
by Andrew Marcus, amarcus@newsignature.com
New Signature
http://www.drupalmostpopular.com
http://drupal.org/project/mostpopular
http://www.newsignature.com


Description
-----------
This module provides a block that reports the most popular nodes on the site 
using a variety of metrics and time intervals.

The most popular nodes can be retrieved in a variety of ways, including
3rd-party services such as Google Analytics and AddThis.com.


Features
------------
* Extensible:
  - It is easy to add new services, which can pull data from:
    - Drupal's internal databases.
    - A 3rd-party analytics provider.
    
* Scalable:
  - Data is retrieved from services whenever the Drupal cron job runs.
    - The data is cached locally until the next time the cron runs.
    
  - It is easy to control how often each service can refresh its data.
    - Many 3rd-party providers specify quotas limiting how many times you can 
      query their service within a period of time.
      
    - You can specify a different wait period for each of the intervals over 
      which you wish to receive data.
    
* Customizable:
  - Among other things, you can configure:
    - How many results to retrieve from each service.
    - Which time intervals to use.
    - The order in which to display the services and intervals in the block.

* Themeable:
  - You can override the themes used to render each component.
  - You can override the stylesheets.
  - You can override the javascript configuration to:
    - Change the selectors the javascript attaches to.
    - Change the transition animations and behaviors.
      
* Gracefully degrades:
  - If javascript is disabled, the module provides a fully-functional alternate
    interface for viewing the most popular nodes. 
    
* Integrates across multiple sites:
  - You can setup a remote Most Popular block that pulls its data from another
    site running the mostpopular module, provided you have access to the database
    of that site.


Installation 
------------
1) Download and install the mostpopular module.

2) Download any service provider modules you wish to use.
   Typically, each of these has additional module requirements:

  * Drupal Service
    - Requires the Statistics module to be enabled and configured in order to 
      collect metrics about page views.
     
  * Google Analytics Service
    - Requires the Google Analytics module in order to collect analytics info.
    - Requires the Google Analytics API module in order to connect with Google.
     
3) Configure the Most Popular settings
  - How many items to retrieve for each service.
  - Any additional site base URLs to link.
  - Which blocks to you want available.
  - Which intervals to use for each block, and in which order to display them.
  - Which services to use for each block, and in which order to display them.
  - Which types of entities should be returned from each service.
  - Any service-specific settings, such as authentication credentials.
  
4) Refresh stats
   Gets the initial most popular data from each of enabled services. 
  
5) Add the Most Popular block
  - The block can be added in the usual way on the Block administration page.
  - No block-specific customizations are provided.  Instead, use the overall
    most popular configuration pages to control how the block looks.
 
6) Verify that the Drupal cron job is being run regularly.
    
7) Verify that you are receiving data.
  - At this point, assuming your services are configured correctly and assuming
    your services are collecting analytics data, you should see it appear in 
    the most popular block.
    

Troubleshooting
------------

If you don't see any results in the most popular block:

1) Make sure to click on all of the service and interval tabs.
  - It's possible that although one service/interval might not have any data,
    other ones do.

2) Make sure your services are collecting analytics data.
  * For the Drupal services:
    - Make sure there are comments posted
    - Make sure you have viewed some nodes since the Statistics module was enabled.
    
  * For the Google Analytics service:
    - Make sure the Google page tracker appears on every page.
    - Make sure you've connected the page tracker to the correct profile.
    - Make sure you've viewed some pages since you enabled the page tracker.
    - GA does not report activity for today, so make sure that at least one 
      day has elapsed since you started tracking page views.
      
  * For the AddThis.com service:
    - Make sure the AddThis widget is pointing to the same username.
    - Make sure you have used the service to email at least one page.
  
3) Go to the services administration page.  
  - If any of the services reports "OK", it has successfully received at least
    one most popular page since it was last configured.
    
  - If any of the services reports "Configured", it has not yet received any
    data since it was most recently configured.  This could indicate a problem.
    
4) Try to clear the caches and reset the services.
  - This will remove all the most popular items for the cache, and it will
    reset the values indicating the last time each service was run.  This will
    force every service to run again the next time you click "Refresh stats".
    
  - Run "Refresh stats" and see whether each service is returning any values.
  
  
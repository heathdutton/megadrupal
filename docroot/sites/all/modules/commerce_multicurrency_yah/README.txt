This module provides the Yahoo Finance currency exchange rate synchronization
 provider for Commerce Multicurrency module.

Yahoo! Finance provides financial information and commentary with a focus on
US markets and offers currency exchange rates in almost all the available
currencies.

Just select the Yahoo Finance sync provider and get the multicurrency work
for the site perfectly.

DEPENDENCIES
This module depends on the Commerce Multicurrency module.

INSTALLATION
1) Download the module to sites/all/modules or sites/all/modules/contrib.
2) Install/Enable the module at admin/modules page.

CONFIGURATION
Select "Yahoo Finance" as "Service to fetch exchange rates from:" on currency
 conversion settings page which is available at: 
 admin/commerce/config/currency/conversion

Run cron or sync manually to synchronize the rates from Yahoo Finance.

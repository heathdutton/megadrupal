INTRODUCTION
------------
The Commerce FirstData Connect module provides integration with the 
FirstData Connect Payment gateway. Which is primly built for the 
UK market.
The module provides two types of payment methods integrated with
the Connect service, direct payment method which is called:
First Data EMEA Connect - Merchant Payment Page and hosted payment method 
or offsite payment method called:
First Data EMEA Connect - Hosted Payment Page
The first one gives the possibility to merchants of displaying their own
layout of the payment form without redirecting the customer to offsite 
pages. The second payment method redirects the customers offsite to a 
Connect payment page. This module will always require 3D secure authentication
and SSL certificate for merchants, if you want to avoid 3D secure authentication
for your customers you must contact first data connect support:
https://www.firstdatams.co.uk/fdms/en_gb/home/contact-us.html
and state that you want to bypass 3D secure for your merchant account.
Firstdata(https://www.firstdatams.co.uk) integration module for Drupal Commerce.

REQUIREMENTS
------------
The module requires mostly the drupal commerce core modules, which are
included in the commerce package.
This module requires the following modules:
  * Commerce (https://www.drupal.org/project/commerce)
  * Commerce Payment (https://www.drupal.org/project/commerce)
  * Commerce Payment UI (https://www.drupal.org/project/commerce)
  * Commerce Checkout (https://www.drupal.org/project/commerce)
  * Commerce Cart (https://www.drupal.org/project/commerce)
  * Commerce Order (https://www.drupal.org/project/commerce)
  * Commerce Order UI (https://www.drupal.org/project/commerce)
  * Commerce Customer (https://www.drupal.org/project/commerce)
  * Views (https://www.drupal.org/project/views)
  * Rules (https://www.drupal.org/project/rules)
  * Entity (https://www.drupal.org/project/entity)
  
RECOMMENDED MODULES
-------------------  
  * Commerce Card on file (https://www.drupal.org/project/commerce_cardonfile):
    If data vault functionality will be used card on file module 
    should be installed.
  * Commerce Price (https://www.drupal.org/project/commerce): 
    Will allow adding price field to products.
  * Commerce Product (https://www.drupal.org/project/commerce):
    Allows you to create products in your eCommerce site.
  * Commerce Product UI (https://www.drupal.org/project/commerce):
    Allows you to access the admin UI of the commerce_product module.
  * Commerce Product Pricing (https://www.drupal.org/project/commerce):
    Provides price management.
  * Commerce Product Reference (https://www.drupal.org/project/commerce):
    Provides the possibility of creating a node that will reference a
    product.
  
INSTALLATION
------------
The first you need to do is to obtain credentials from FirstData Connect 
(https://www.firstdatams.co.uk/fdms/en_gb/home.html).
1.  Download and enable the commerce package:
    (https://www.drupal.org/project/commerce)
2.  Clone the module to your local repository by using the following command: 
    git clone --branch 7.x-1.x http://git.drupal.org/sandbox/veso_83/2333675.git
    commerce_firstdata_connect
    cd commerce_firstdata_connect
    or just visit the link below and follow the instructions: 
    https://www.drupal.org/project/2333675/git-instructions 
3.  If you want to use Data vault functionality you should download 
    an enable the card on file module. Link to the card on file project: 
    https://www.drupal.org/project/commerce_cardonfile 

CONFIGURATION
-------------
When you have your credentials and you've installed the module you need to
configure the payment methods
1.  Go to Store-> Configuration-> Payment methods
    Enable one of the payment methods (
    First Data EMEA Connect - Merchant Payment Page
    or First Data EMEA Connect - Hosted Payment Page), or both of them. 
    The First Data EMEA Connect – merchants that do not want to redirect offsite 
    their customers can use merchant payment page. 
    The First Data EMEA Connect – Hosted Payment Page method 
    is offsite payment method that will redirect a customer offsite
    where they can enter their payment details.
2.  The next step is to enter the credentials in the admin UI 
    for each of the payment methods, click on the name of the payment method 
    or click on edit, then on the next screen click edit on the bottom 
    right side of the screen to edit the rule method. 
3.  The first setting is Test or Production url to be used. 
    If Test url is selected the merchants and their customers can 
    perform test transactions, 
    this is strongly recommended for new merchants, 
    first  you should test the module and later apply the 
    production environment. 
4.  The next step is to enter the Store id and Secret 
    they should be the same as provided from 
    first data (Store Number and Shared Secret).
5.  The next option is Transaction capture method – allows merchants to 
    choose whether or not the transactions will automatically be authorized and 
    captured (sale option) or will be authorized and give the 
    possibility to be captured later (Pre-Auth option).
6.  The next setting is called Select payment mode where merchants 
    can choose the payonly option – which means only the main information 
    required from the customers will be send to firstdata. 
    The PayPlus option means that some additional customer information will be 
    send, more detailed billing information. 
    The third option is Full mode, which means that additional 
    shipping information will be recorded on firstdata servers.
7. Second option is called apply extended hash, 
    which allows merchants to apply more complex hash 
    algorithm to create a hashed parameter that will be send to 
    firstdata servers as an extra data protection. 
    This is optional parameter.
8. It is very important that you set up your time zone correctly, 
    Under Default timezone you can find the link regional settings 
    and there you should select your actual server timezone, 
    otherwise your transactions may fail. 
    If you enabled the card on file module a new option will be presented     
    called: “Enable Card on File functionality with this payment method.” 
    If you tick the checkbox you will be able to use the data 
    vault (card on file) functionality. 
9. Next option allows you to select language to let firstdata know 
    what language should be used when displaying pages in front of customers.
10. Last setting is Log issues – this option allows you to store 
    any failure with some brief description why a transaction failed this 
    can be reviewed in your log messages.
11. Merchants should know that firstdata connect module will require always 
    3D secure authentication from customers, if merchants wants to by pass that, 
    they should contact firstdata support: 
    https://www.firstdatams.co.uk/fdms/en_gb/home/contact-us.html
12. In regards to the above information to use the 3D secure 
    merchants should be able to provide secure 
    certificate, otherwise transactions may be rejected. 
13. Merchants can capture transactions manually by going to 
    Store->Orders select on the right side of the screen 
    ‘payment’, if the payment was authorized then a ‘capture’ 
    link will appear on the right side of the screen merchants 
    can click on that link and capture just part of the amount or all of it.
14. Be aware that refund functionality is not available through Drupal; 
    this can be done through the virtual terminal. 
15. You can void a transaction, which was previously captured; 
    the void link will appear on the right side of the screen 
    replacing the capture link.
16. There is more automated capture functionality merchants can trigger 
    manually that functionality by going to Store->Orders and at the top 
    right corner of the current view a tab 
    will appear (Firstdata Connect Capture), 
    this tab is next to the Orders tab, 
    if you click that tab you will have, 
    the option to chose from what time the module,
    will start capturing transactions, 
    by default it’s says now, which means that if you push the button Capture, 
    the module will start capturing transactions created from this moment. 
    You can change that by replacing 'now' with -1 day which means 
    that the module will start capturing transactions from the day before
    or you can write '-2 days' which means the module 
    will start capture transactions 
    from 2 days ago, so on and so on.
17. You can use even more automated method to capture 
    all authorized transactions; 
    an example rule was created for that purpose that 
    will be triggered on cron run. 
    By default that rule is set to capture transactions 
    starting from 'now' (today), 
    but you can modify that rule to fit your needs.
    You can set that value to '-1 day' which means
    that the rule will start capturing transactions 
    from the day before 'today'.

In order to use this module you will need a Canada Post merchant "CPC ID".
You can obtain this number by siging up on the Canada Post webserver at:

 http://sellonline.canadapost.ca/servlet/GuideGeneratorWizard

If you want to try out this module without siging up for your own number, you
can use the test CPC ID instead:

  CPC_DEMO_XML

(Enter your CPC ID on the uc_canadapost administration page at
admin/store/settings/quotes/settings/canadapost)

This module automatically divides order products into packages of 30kg or less.
30kg is the Canada Post weight limit for a package.  This version will calculate
how many packages need to be sent, calculate the cost of shipping each package,
and display the total number of packages and the total shipping cost to the
customer.

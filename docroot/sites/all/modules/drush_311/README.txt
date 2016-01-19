Drush 311 provides a quick way for developers to access frequently needed information.

For example, install Drush 311 on your team's intranet and add entries such as 'Office phone number', 'Printer IP address', 'Guest WIFI password'. The entries are managed at /admin/config/user-interface/drush_311. Then the team can access this info via:

drush @SITENAME 311 KEYWORD

You can also add entries with drush 311-add.

Each entry has a name, keywords, and information. For example,

Name: Our Office Phone Number
Keywords: phone number telephone
Information: (215) 555-1212

$ drush 311 phone
Our Office Phone Number: (215) 555-1212

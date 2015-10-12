This is a very simple helper module to map webform data to SF data.

Step-by-step:

1. Create a entityform or other entity type
2. Create a salesforce mapping for it and note the name (ex. lead_mapping)
3. Create a webform
4. Add a hidden field salesforce_mapping to the webform
5. Use the salesforce mapping "machine_name" and add it as key; example: 'lead_mapping'|webform_anonymous
6. The data after the | can be one of 4 types:
  a. webform - Use the webform data.
  b. node - Will be replaced with the Webform's SF ID (usually mapped to a campaign).
  c. user - Will be replaced with the User's SF ID (usually mapped to the global user - if user is logged in).
  d. webform_anonymous - Use the webform data, but process only if user is anonymous.

Now every webform having the salesforce_mapping hidden field will automatically create the entities specified as mappings and sync them to salesforce.

Install
  - Place the entirety of this directory in 
    sites/all/modules/commerce_file_bundle
  - Navigate to administer >> build >> modules. Enable Commerce File.
  - Be sure that any commerce_file and commerce_file_bundle 
    configurations are complete

Recommended modules for use with Commerce File
  - File Field Sources
  
Usage:
  - This module provides 2 rules and 2 rule actions to manage files 
    within commerce product bundles. They are named itentically to 
    those provided by commerce_file, except that '(Bundle Friendly)' 
    is appended to the end. Since these rules/actions recapitulate 
    those functions provided by commerce_file, we recommend that you 
    disable the commerce_file provided rules to improve performance.

Description
-----------
The module allows your users to have an ability of legendary Phoenix creature on site reinstall.

The Phoenix using its unique ability in two steps: burn yourself when death looking in its eyes and then resurrect when time is come.

To achieve step 1 - burn&save - please include in your bash install/reinstall script the following command:

drush user-phoenix-save

To activate step 2 - resurrection just include module in install profile or enable it manually.

Of course you can use module only for the step one - this allows, for example, to store users in your project repository.

Please note that Phoenix does not take care about Administrator user. This one should be able to take care about yourself, right?

Requirements
------------
- Drush
- Writable subdirectory 'data' in module directory


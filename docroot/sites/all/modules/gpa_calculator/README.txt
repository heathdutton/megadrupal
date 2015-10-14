INTRODUCTION
------------
This module provides schools with a GPA calculator to use in a block.
The gpa calculator form initially display 6 rows with 3 columns
(Class/Course Name, Grade, Credits Earned). It has the ability to dynamically
add more rows. The form also has a section for cumulative (past) GPA
and Credits Earned as part of the calculation.

The description section under the header is configurable. Administrators also
have the ability to customize the title by amending the title with a school name
to personalize the calculator. Finally, the Grade select boxes themselves are
configurable with validation to ensure the option values are numerical for
correct calculations. If the Grades setting is left blank then the select
options will be filled with hard coded values.

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
 * Configure user permissions in Administration » People » Permissions:
   - Grant the Administer GPA Calculator permission to any roles that
     should have the ability to configure the GPA Calculator.
 * Customize the GPA Calculator in Administration > Configuration >
   GPA Calculator Administration > GPA Calculator settings.
   - Add a school name to the block title, instructions on how to use
     the GPA Calculator and custom grades for the grade dropdown if
     different than the default options.

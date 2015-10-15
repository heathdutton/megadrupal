# Percentage Circle Svg module

## SUMMARY
This module allows you to create Percentage Circle Svg.
Really customizable, you can change the radius of the circle, the percentage,
the stroke width, the colors, the text, etc...

## FEATURES
- Create Percentage Circle Svg.

## REQUIREMENTS
- [libraries module](https://www.drupal.org/project/libraries)
- The external library [PercentageCircleSvg](https://github.com/geberele/PercentageCircleSvg)
  You need to clone or download that repo first.

## INSTALLATION
* Download the module and extract it in the folder sites/all/modules/contrib.
  It is recommended to place all third-party modules in a subfolder called contrib.
* Clone the library "PercentageCircleSvg" from https://github.com/geberele/PercentageCircleSvg
  into the folder 'sites/all/libraries/percentage-circle-svg'
  using git from the root folder of your site:
  `cd sites/all/libraries && git clone git@github.com:geberele/PercentageCircleSvg.git percentage-circle-svg`
  or downloading directly the repo.
  At the end the file PercentageCircleSvg.php has to have the following path:
  "sites/all/libraries/percentage-circle-svg/PercentageCircleSvg.php"
* Go to the Module page at Administer > Modules (http://example.com/admin/modules) and enable it.
  Read more about installing modules in D7: https://www.drupal.org/documentation/install/modules-themes/modules-7

## HOW TO USE IT
* You can use it as a theme function, for example:
  `print theme('percentage_circle_svg', array('radius' => 55, 'percentage' => 70));`
* For more examples have a look at the function:
  `_percentage_circle_svg_menu_example_page()`

-- CONTACT --
Current maintainers:
* Gabriele Manna (geberele) - https://drupal.org/user/1183748

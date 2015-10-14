This module makes the iGrowl library available for use from within Drupal, enabling you to create iGrowl alerts
from either your own custom javascript code or by using an included AJAX command function to spawn an alert.

Dependencies

- Libraries
- Animate.css

Installation

- Download the animate.css zip file and unzip to (path to libraries folder)/animate
- Download the iGrowl zip file and unzip to (path to libraries folder)/igrowl
- Enable the animate_css module
- Enable the igrowl module
- Clear all cache, the status report page should indicate both libraries are included successfully

Usage

In a custom theme or module, you can spawn an iGrowl alert within your javascript by invoking it directly and providing it whatever options you want to use:

<code>
$.iGrowl({
 message: "Your message here",
})
</code>

This module also defines a custom AJAX command, so you can leverage iGrowl from an AJAX response like so:


$igrowl = igrowl_default_options();
$igrowl['title'] = 'Excellent!';
$igrowl['message'] = 'We have added ' . $foo . ' ' . $bar .' to your order!';
$igrowl['type'] = 'success';
$igrowl['icon'] = 'feather-circle-check';

$commands = array();
$commands[] = igrowl_ajax_command_growl($igrowl);
return array('#type' => 'ajax', '#commands' => $commands);


igrowl_default_options() returns an array of overrideable iGrowl options, like type, icon, message, title and more. igrowl_ajax_command_growl() takes this array and invokes iGrowl against it,
giving you flexibility to spawn and create dozens of iGrowls to your liking.
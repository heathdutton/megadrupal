Drupal views universal accordion module:

Author - Eugene Sorochinskiy webmaster at darkguard dot net
Requires - Drupal 7


Overview:

The Views Universal Accordion module is a Views style plugin that displays the
results in a accordion style.
The module uses uAccordion library based on zAccordion. Unlike the base version, the
library has a major enhancement:
it can now display the accordion in horizontal or vertical style.
Like its ancestor the library also has flexible handler
function mechanism which allows to customize the accordion behaviour.

Description:

IMPORTANT: The visible parts of the closed accordion component are used to
"trigger" the accordion action.

How the view's fields are displayed:
All non-image fields are wrapped with a <div class="wrapper"> markup element
which allows to customize display styles
Choose Accordionin the Style dialog within your view, which will prompt you
to configure:

Trigger: The event used to trigger the accordion. Choices are: mouse over and
click;
Autoplay:  If the event should be triggered automatically;
Accordion type: The type of the accordion: horizontal or vertical
Invert the accordion: Whether or not the last slide stays in the same position,
rather than the first slide    
Display errors: Whether or not display errors in the console
Timeout: Timeout between slides when autoplaying
Pause on hover: Pause on hover when autoplaying

Theming information

This module comes with a default style, which you can disable in the options
(see above). Files included for your convinence:
views-universal-acordion.css - with how the classes the author thought
would be best used, mostly empty.
views-view-universal-accordion.tpl.php - copy/paste into your theme directory,
please read the comments in this file for requirements/instructions.

Dependencies

- Views;
- Libraries

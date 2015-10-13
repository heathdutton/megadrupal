The Setup module adds the ability to have scripted, setup wizards which are run
automatically on the first access of a completed or upgraded site.

Think of it as a method of introducing a client to their new/updated site and
having them fill out required information that may not have been available at
the time of development, such as their Google Analytics tracking code or Twitter
account.

The module is intended for use by Developers as the creation of your setup
script is done in a profile/module/theme .info file, however the syntax is not
complex and could be attempted by an adventurous site builder.

Setup was written by Stuart Clark and is maintained by Stuart Clark (deciphered)
and Brian Gilbert (realityloop) of Realityloop Pty Ltd.
- http://www.realityloop.com
- http://twitter.com/realityloop



Required modules
--------------------------------------------------------------------------------

* Chaos tool suite (ctools) - http://drupal.org/project/ctools



Recommended modules
--------------------------------------------------------------------------------

* Libraries                 - http://drupal.org/project/libraries
* Variable                  - http://drupal.org/project/variable



How to use
--------------------------------------------------------------------------------

Setup scripts are defined in a profile/module/theme .info file, using the
standard .info syntax, similar to that of the Profiler module/library.

The structure of a setup script has two parts:


* Options

  Options allow you to define the behaviour of the Setup module, both
  aesthetically and functionally, but all options are optional.

  Here's an example of a Setup Options configuration:


      setup[options][finish_redirect] = welcome
      setup[options][show_cancel] = 0
      setup[options][style] = drupalsetup



  This will do the following:

  - Redirect to http://[www.yoursite.com/path/to/drupal]/welcome on completion
    of the Setup script.

  - Not show a cancel/skip setup link during the Setup script.

  - Use the 'drupalsetup' Setup style during the Setup script.


  The available Options can be found below.



* Steps

  The Steps are the guts of the Setup script, containing the type of content and
  the necessary settings for each Step of the Setup script.

  Here's an example of a Setup Step:


      setup[steps][theme][type] = theme
      setup[steps][theme][title] = Choose a theme
      setup[steps][theme][options][] = bartik
      setup[steps][theme][options][] = seven



  This will display a theme chooser Step allowing the set either the Bartik or
  Seven theme as their default site theme.


  Core Steps and their expected arguments can be found below.

  Additional Step types can be defined by modules using hook_setup_info().


Once you've configured your Setup script you need to set the source and put the
site into Setup mode. This can be done on the Maintenance mode settings page
(admin/config/developnent/maintenance) or done programatically (see the Setup
example module for an example).



Options
--------------------------------------------------------------------------------

* `cancel_disable`

  Whether the Setup module should be disabled upon the cancellation of the
  Setup script.

  Expects a boolean value.


      setup[options][cancel_disable] = 0



* `cancel_message`

  A message to display to the user via drupal_set_message() upon the
  cancellation of the Setup script.


      setup[options][cancel_message] = Setup has been skipped.



* `cancel_redirect`

  Page to redirect user to upon the cancellation of the Setup script.


      setup[options][cancel_redirect] = <front>



* `finish_disable`

  Whether the Setup module should be disabled upon the completion of the Setup
  script.

  Expects a boolean value.


      setup[options][finish_disable] = 1



* `finish_message`

  A message to display to the user via drupal_set_message() upon the
  completion of the Setup script.


      setup[options][finish_message] = Setup has been completed.



* `finish_redirect`

  Page to redirect user to upon the completion of the Setup script.


      setup[options][finish_redirect] = node/1



* `maintenance_mode`

  Whether to put the site into Maintenance mode for the duration of the Setup
  script.

  Expects a boolean value.


      setup[options][maintenance_mode] = 0



* `show_back`

  Whether to show a 'Back' button during the Setup script, allowing the user to
  navigate back to previous Steps.

  Expects a boolean value.


      setup[options][show_back] = 0



* `show_cancel`

  Whether to show a 'Skip setup' link during the Setup script, allowing the user
  to cancel the Setup script.

  Expects a boolean value.


      setup[options][show_cancel] = 1



* `show_steps`

  Whether to show a list of steps, including the status of where the user is
  in the list.

  Expects a boolean value.


      setup[options][show_steps] = 1



* `style`

  The name of the Setup style to use during the Setup script.

  By default the Setup comes with one style, drupalsetup, which is based on the
  Drupal installation theme, but additional styles may be added in the future or
  alternatively you can define your own with hook_setup_styles().


      setup[options][style] = drupalapi



* `style_override`

  Allows for certain CSS files to be disabled while the Setup script is running
  in the case that they interfere with Setup style.

  Options:

  - theme

    Disable all css styles defined by the active theme (default).


  - none

    Don't disable any css styles.



      setup[options][style_override] = theme



Steps
--------------------------------------------------------------------------------

Steps are defined in the following style:


      setup[steps][step_name][type] = step_type
      setup[steps][step_name][title] = Step title
      setup[steps][step_name][help][markup] = <h1>Optional help markup</h1>
      setup[steps][step_name][help][filter] = filter_name


All steps require a unique identifier (step_name) and a type, however each Step
type will have it's own expected arguments to be defined.

The optional help markup can be used on all steps and will inject the markup
HTML, processed by the optional filter or filter_xss(), above the main content
of the defined step.



* `markup`

  Used for the creation of basic informational or display Steps.


  Provides one required argument:

  - `markup`

    One unbroken line of HTML markup to be displayed on the page.


  Provides one optional argument:

  - `filter`

    The machine name of the filter to be used to process the defined markup. If
    no value is defined the markup will be processed with filter_xss().


      setup[steps][intro][type] = markup
      setup[steps][intro][title] = Welcome
      setup[steps][intro][markup] = <h1>Welcome to your new site</h1><p>...</p>
      setup[steps][intro][filter] = filtered_html



* `node_view`

  Renders a Node via node_view().

  Useful for showing a large chunk of informational content without having to
  implement it all in the .info file, to demonstrate standard content types for
  the site or to allow editing of content via a inline editor (like Aloha).


  Provides one required argument:

  - `nid`

    The numerical Node ID for the Node to be rendered.


  Provides on optional argument:

  - `view_mode`

    The view mode (full, teaser, etc) to be used when rendering the Node.


      setup[steps][blog_posts][type] = node_view
      setup[steps][blog_posts][title] = Blog posts
      setup[steps][blog_posts][nid] = 1
      setup[steps][blog_posts][view_mode] = full



* `theme`

  Provides the user with a Theme chooser step, allowing them to choose the
  default theme for their site.


  Provides one required argument:

  - `options`

    An array of theme machine names to provide as options.

    At least two values should be supplied.


      setup[steps][theme][type] = theme
      setup[steps][theme][title] = Choose a theme
      setup[steps][theme][options][] = bartik
      setup[steps][theme][options][] = seven



  Note: If you have the Libraries module enabled and the jQuery Cycle library
        the theme chooser will utilize jQuery Cycle to provide a nicer
        interface.



* `variable_set`

  Provides an interface for setting the values of any defined variables.

  Useful for having the client set important values, such as their Google
  Analytics tracking code, their site name or even your own variables.


  Provides one required argument:

  - `variables`

    A keyed array of FAPI elements, where the key is the name of the variable
    and any FAPI elements defined on that key are used to generate the form.

    If no FAPI type is set the field will default to a Textfield.


        setup[steps][info][type] = variable_set
        setup[steps][info][title] = Site information
        setup[steps][info][variables][site_name][title] = Site name
        setup[steps][info][variables][site_name][required] = 1



  Note: If you have the Variable module enabled and the variable is defined by
        hook_variable_info(), default FAPI elements will be provided for you.



TODO / Roadmap
--------------------------------------------------------------------------------

- Finish/fix 'Node Edit' type.

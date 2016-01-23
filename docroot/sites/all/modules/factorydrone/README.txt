# Factory Drone
The primary aim of Factory Drone is to a fill a void in the world of Drupal
development and testing. Factory Drone provides an interface for creating
various types of objects (vocabularies, terms, content types, nodes), both
through code and through Drush.

## Background
When working on high-level acceptance testing with [Behat](http://behat.org),
the difficulty of creating specific data for tests quickly became apparent.
Outside of navigating through the Drupal user interface, No way to create
content with specific attributes existed. As soon as roles and user permissions
added restricted to the content creation workflow, this methodology became
inordinately cumbersome. We were forced to swtich to fixture data, but that has
its own problems.

*There is a better way.*

## Factories
Factories are a way of creating data based on a template. Configuring your test
environment to use Factory Drone factories is as easy as:

  1. Telling Factory Drone where to find your factory templates and
  2. Telling Factory Drone to load the templates

The above steps can be accomplished wherever it is most fitting to do so. The
following code snippet is an example of the methods to do so.

    FactoryDrone::addFactoryPath(join(DIRECTORY_SEPARATOR,
    						                      array(__DIR__, 'factories')));
    FactoryDrone::loadDefinitions();

Factory Drone can load factory definitions from multiple locations if necessary,
simply by adding additional paths.

## Factory Definitions
Factory definition files, which must have the extension `.php`, which have been
registered and loaded will be avilable for use. Factory definitions are composed
of three parts:

  1. A type (e.g, node, vocabulary, etc.),
  2. A name by which the factory will be referenced later, and
  3. An array of options that will represent the default options for the factory

    FactoryDrone::define('node', 'simple page', array(
      'type' => FactoryDrone::association('a content type factory'),
      'title' => FactoryDrone::sequence(function ($n) {
        return "node_title{$n}";
      }),
      'body' => FactoryDrone::random(function ($n) {
        return "node_body{$n}";
      }),
    ));

When using a factory, it is possible to override any of the options provided by
the definition. Consequently, you always have control over the data at the time
you create it. Using factories is as simple as:

    FactoryDrone::create('simple page');

or, to override options:

    FactoryDrone::create('simple page', array('title' => 'Overridden Title'));

## Drush Integration
Factory Drone provides integration with Drush to facilitate the use of factories
even when a bootstrapped Drupal environment is not immediately available. Doing
so is as simple as running a command such as the following
`drush fd-create FACTORY_NAME optional_aguments=optional_values`.

Read `drush help fd-create` for more information.

## Supported Types
At present, Factory Drone has support for the following types of data in Drupal:

* Vocabularies
* Terms
* Roles
* Users
* Content Types
* Nodes
* Menus
* Menu Links

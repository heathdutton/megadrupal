[![Build Status](https://travis-ci.org/Plug-Drupal/plug_config.svg?branch=7.x-1.x)](https://travis-ci.org/Plug-Drupal/plug_config)
# Plug Config
Create exportable entities as **annotation plugins**.

The Plug Config module allows developers to provide exportable entities as annotated plugins, emulating the behavior provided by Config Entities in Drupal 8.

This module adds a wrapper around Drupal 7 common entities to generate easily new exportable entity types just creating an annotated class.

## Why this module?
Plug Config is a module for developers that want to provide an easy and extensible way to provide different configurations in their modules that can also be automatically exported using features. You can use this module as an alternative to your custom CTools exportables.

The main advantages of Plug Config compared to CTools Exportables are listed below:
 - Plug Config items are **object oriented plugins**, so you can generate a cleaner code because all the code is wrapped in a single file and you can take advantage of inheritance and all the other benefits of OOP.
 - Plug config items are **Drupal entities**, this allow developers to make use of all the **Entity API** functions and helpers like **[EntityMetadataWrapper](https://www.drupal.org/documentation/entity-metadata-wrappers)** to handle them or **[EntityFieldQuery](https://www.drupal.org/node/1343708)** to make queries and filter Plug Config items.

## Implementation examples
There is an [example module](modules/plug_config_example) shipped with this module that will show you how to create your plugins and use them.

## Installation
This module belongs to the [Plug](https://github.com/Plug-Drupal/plug) family and requires the awesome **Plug** module that provides the Drupal 8 plugin system for Drupal 7 developments. Plug installation process is described in the [documentation](https://github.com/Plug-Drupal/plug#installing). 

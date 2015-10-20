<?php
/**
 * @file
 * Add functions to dc_scratch.
 */

/**
 * A QUICK OVERVIEW OF DRUPAL THEMING.
 *
 * Drupal deals with each chunk of content using a "theme hook". The raw
 * content is placed in PHP variables and passed through the theme hook, which
 * can either be a template file or a theme function.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 * The template.php file is one of the most useful files when creating or
 * modifying Drupal themes. With this file you can do three things:
 * - Modify any theme hooks variables or add your own variables, using
 * preprocess or process functions.
 * - Override any theme function. That is, replace a module's default theme
 * function with one you write.
 * - Call hook_*_alter() functions which allow you to alter various parts of
 * Drupal's internals, including the render elements in forms. The most
 * useful of which include hook_form_alter(), hook_form_FORM_ID_alter(),
 * and hook_page_alter(). See api.drupal.org for more information about
 * _alter functions.
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 * Each tpl.php template file has several variables which hold various pieces
 * of content. You can modify those variables (or add new ones) before they
 * are used in the template files by using preprocess functions.
 *
 * This makes THEME_preprocess_HOOK() functions the most powerful functions
 * available to themers.
 *
 * It works by having one preprocess function for each template file or its
 * derivatives (called theme hook suggestions). For example:
 * THEME_preprocess_page alters the variables for page.tpl.php
 * THEME_preprocess_node alters the variables for node.tpl.php
 *
 * For more information on preprocess functions and theme hook suggestions,
 * please visit the Theme Developer's Guide on Drupal.org:
 * http://drupal.org/node/223440 and http://drupal.org/node/1089656
 */

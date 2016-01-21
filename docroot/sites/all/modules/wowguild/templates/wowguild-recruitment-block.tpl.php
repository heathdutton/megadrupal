<?php
// $Id$

/**
 * @file
 * Default theme for guild recruitment block.
 *
 * Available variables:
 * - $header: The sanitized HTML from block configuration.
 * - $footer: The sanitized HTML from block configuration.
 * - $application_link: The full HTML of the application link: <a href="$application_url">Apply to Guild</a>
 * - $rendered_roles: HTML Table of guild needs.  See $recruitment.
 *
 * Other variables:
 * - $application_url: The URL to link to for user to apply to guild.
 * - $recruitment: array of recruitment needs if you want to render your own table.
 *   Array (
 *     [tanks] => Array
 *       (
 *           [#title] => Tanks
 *           [#value] => 4        - Numeric value of #text: 4 => 'High', 3 => 'Meduim', 2 => 'Low', 1 => 'Closed', 0=>'Hidden'
 *           [#text] => High
 *       )
 *       .
 *       .
 *       .
 *     [death-knights] => Array
 *       (
 *           [#title] => Death Knights
 *           [#value] => 1
 *           [#text] => Closed
 *           [#class] => role-name shadow color-c6    - default classes
 *           [#classId] => 6                          - class ID for coloring by class
 *       )
 *
 * @see template_preprocess_wowguild_recruitment_block()
 * @see template_preprocess()
 * @see template_process()
 */

?>

<?php if ($header) :?>
<div class="recruitment-block-header"><?php echo $header; ?></div>
<?php endif; ?>

<div class="recruitment-block-roles"><?php echo $rendered_roles; ?></div>

<?php if ($footer) :?>
<div class="recruitment-block-footer"><?php echo $footer; ?></div>
<?php endif; ?>

<?php if ($application_link) :?>
<div class="recruitment-block-application-link"><?php echo $application_link; ?></div>
<?php endif; ?>
